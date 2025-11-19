#include "Image.h"
#include "mainwindow.h"
#include "BrightnessDialog.h"
#include "./ui_mainwindow.h"
#include "ImageProcessor.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QSlider>
#include <QVBoxLayout>
#include <QDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionInvert_Colors, &QAction::triggered, this, &MainWindow::invertColors);
    //connect(ui->actionBrightness, &QAction::triggered, this, &MainWindow::changeBrightness);
    connect(ui->actionBrightness, &QAction::triggered, this, &MainWindow::openBrightnessDialog);

    connect(ui->actionContrast, &QAction::triggered, this, [=](){
        bool ok;
        double factor = QInputDialog::getDouble(this, "Contrast", "Factor (0.1–3.0):", 1.0, 0.1, 3.0, 2, &ok);
        if (ok) changeContrast(factor);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open PNM file",
        QString(),
        "PNM Images (*.ppm *.pgm *.pbm)"
        );

    if (fileName.isEmpty())
        return;
    Image img;
    if (!img.load(fileName.toStdString())) {
        QMessageBox::warning(this, "Błąd", "Nie udało się wczytać obrazu!");
        return;
    }

    QMessageBox::information(this, "Sukces",
                             QString("Obraz wczytany"));

    // Image -> QImage
    currentImage = QImage(img.getSizeX(), img.getSizeY(), QImage::Format_RGB888);

    for (unsigned int y = 0; y < img.getSizeY(); ++y) {
        for (unsigned int x = 0; x < img.getSizeX(); ++x) {
            unsigned char r, g, b;
            img.getPixelRGB(x, y, r, g, b);
            currentImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    QPixmap pix = QPixmap::fromImage(currentImage);

    ui->label->setAlignment(Qt::AlignCenter);

    ui->label->setPixmap(
        pix.scaled(ui->label->size(),
                   Qt::KeepAspectRatio,
                   Qt::SmoothTransformation)
        );
    modifiedImage = currentImage;
    // ui->label->setPixmap(QPixmap::fromImage(modifiedImage));
    // ui->label->adjustSize();
}
void MainWindow::invertColors() {
    modifiedImage = ImageProcessor::invertColors(modifiedImage);
    //updateLabel();
    ui->label->setPixmap(QPixmap::fromImage(modifiedImage));
}

void MainWindow::changeContrast(double factor) {
    modifiedImage = ImageProcessor::changeContrast(currentImage, factor);
    ui->label->setPixmap(QPixmap::fromImage(modifiedImage));
}

void MainWindow::openBrightnessDialog() {
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Brightness");
    dialog->setFixedSize(300, 70);

    QSlider *slider = new QSlider(Qt::Horizontal, dialog);
    slider->setMinimum(-255);
    slider->setMaximum(255);
    slider->setValue(0);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(slider);
    dialog->setLayout(layout);

    connect(slider, &QSlider::valueChanged, this, [=](int value){
        modifiedImage = ImageProcessor::changeBrightness(currentImage, value);
        ui->label->setPixmap(QPixmap::fromImage(modifiedImage));
    });
    currentImage = modifiedImage;
    dialog->exec();
}
