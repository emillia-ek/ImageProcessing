#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>

class ImageProcessor {
public:
    static QImage invertColors(const QImage &img);
    static QImage changeBrightness(const QImage &img, int value);
    static QImage changeContrast(const QImage &img, double factor);
};

#endif
