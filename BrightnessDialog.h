#ifndef BRIGHTNESSDIALOG_H
#define BRIGHTNESSDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QVBoxLayout>

class BrightnessDialog : public QDialog {
    Q_OBJECT
public:
    BrightnessDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Zmiana jasności");
        setFixedSize(300, 70);

        slider = new QSlider(Qt::Horizontal);
        slider->setMinimum(-255);
        slider->setMaximum(255);
        slider->setValue(0);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(slider);
        setLayout(layout);

        connect(slider, &QSlider::valueChanged, this, &BrightnessDialog::valueChanged);
    }

signals:
    void valueChanged(int); // przekazuje nową wartość do MainWindow

private:
    QSlider *slider;
};

#endif
