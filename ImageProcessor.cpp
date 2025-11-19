#include "ImageProcessor.h"
#include <QColor>
#include <QtGlobal>
#include <algorithm>

QImage ImageProcessor::invertColors(const QImage &img)
{
    QImage result = img;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            QColor c = img.pixelColor(x, y);
            c.setRed(255 - c.red());
            c.setGreen(255 - c.green());
            c.setBlue(255 - c.blue());
            result.setPixelColor(x, y, c);
        }
    }
    return result;
}

QImage ImageProcessor::changeBrightness(const QImage &img, int value)
{
    QImage result = img;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            QColor c = img.pixelColor(x, y);
            int r = std::clamp(c.red() + value, 0, 255);
            int g = std::clamp(c.green() + value, 0, 255);
            int b = std::clamp(c.blue() + value, 0, 255);
            result.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return result;
}

QImage ImageProcessor::changeContrast(const QImage &img, double factor)
{
    QImage result = img;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            QColor c = img.pixelColor(x, y);
            int r = std::clamp(int((c.red() - 128) * factor + 128), 0, 255);
            int g = std::clamp(int((c.green() - 128) * factor + 128), 0, 255);
            int b = std::clamp(int((c.blue() - 128) * factor + 128), 0, 255);
            result.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return result;
}
