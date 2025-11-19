#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include "Image.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //Image img;
    QImage currentImage;
    QImage modifiedImage;
    // QLabel *label;
    QScrollArea *scrollArea;
    double scaleFactor;

private slots:
    void openFile();
    void invertColors();
    // void changeBrightness();
    void openBrightnessDialog();
    void changeContrast(double factor);
};


#endif // MAINWINDOW_H
