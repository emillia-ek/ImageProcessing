#ifndef IMAGE_H
#define IMAGE_H

#include <string>

enum ImageType {
    RGB,
    BIN,
    GRAY
};
class Image
{
private:
    unsigned int sizeX;
    unsigned int sizeY;
    unsigned char** pixels;
    ImageType type;
    int maxValue;

public:
    Image();
    ~Image();
    Image(unsigned int sizeX, unsigned int sizeY, ImageType t = RGB, int maxValue = 255);

    bool load(const std::string& filename);
    bool save(const std::string& filename);

    void getPixelRGB(unsigned int x, unsigned int y, unsigned char& r, unsigned char& g, unsigned char& b) const;
    unsigned char getPixel(int x, int y) const;
    void setPixelRGB(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);
    void setPixel(unsigned int x, unsigned int y, unsigned char value);

    unsigned int getSizeX() const { return sizeX; }
    unsigned int getSizeY() const { return sizeY; }
    ImageType getType() const { return type; }
};

#endif
