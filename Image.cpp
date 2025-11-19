#include "Image.h"
#include <fstream>
#include <sstream>
using namespace std;

Image::Image() : sizeX(0), sizeY(0), pixels(nullptr), type(RGB), maxValue(255) {}

Image::Image(unsigned int sx, unsigned int sy, ImageType t, int mv) :
    sizeX(sx), sizeY(sy), pixels(nullptr), type(t), maxValue(mv) {

    if (sizeX > 0 && sizeY > 0) {
        int bytesPerPixel = (type == RGB) ? 3 : 1;
        pixels = new unsigned char* [sizeY];
        for (unsigned int i = 0; i < sizeY; i++) {
            pixels[i] = new unsigned char[sizeX * bytesPerPixel]();
        }
    }
}
Image::~Image() {
    if (pixels != nullptr) {
        for (unsigned int i = 0; i < sizeY; i++) {
            delete[] pixels[i]; //zwolnienie pamięci zaalokowanej dynamicznie dla tablicy
        }
        delete[] pixels;
    }
};

unsigned char Image::getPixel(int x, int y) const {
    if (x < 0 || x >= static_cast<int>(sizeX) || y < 0 || y >= static_cast<int>(sizeY)) {
        return 0;
    }

    if (type == BIN || type == GRAY) {
        return pixels[y][x];
    }
    else if (type == RGB) {
        unsigned char r = pixels[y][x * 3 + 0];
        unsigned char g = pixels[y][x * 3 + 1];
        unsigned char b = pixels[y][x * 3 + 2];
        return static_cast<unsigned char>((r + g + b) / 3);
    }
    else {
        return 0;
    }
}
void Image::getPixelRGB(unsigned int x, unsigned int y, unsigned char& r, unsigned char& g, unsigned char& b) const {
    if (x >= sizeX || y >= sizeY) {
        r = g = b = 0;
        return;
    }

    if (type == RGB) {
        r = pixels[y][x * 3 + 0];
        g = pixels[y][x * 3 + 1];
        b = pixels[y][x * 3 + 2];
    }
    else {
        unsigned char value = pixels[y][x];
        r = g = b = value;
    }
}
void Image::setPixel(unsigned int x, unsigned int y, unsigned char value) {
    if (x >= sizeX || y >= sizeY) return;
    switch (type) {
    case BIN:
        pixels[y][x] = (value > 0) ? 1 : 0;
        break;
    case GRAY:
        pixels[y][x] = value;
        break;
    case RGB:
        pixels[y][x * 3 + 0] = value;
        pixels[y][x * 3 + 1] = value;
        pixels[y][x * 3 + 2] = value;
        break;
    }
}
void Image::setPixelRGB(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b) {
    if (type == RGB) {
        pixels[y][x * 3 + 0] = r;
        pixels[y][x * 3 + 1] = g;
        pixels[y][x * 3 + 2] = b;
    }
}

bool Image::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    std::string line, format;
    std::getline(file, line);
    std::istringstream iss(line);
    iss >> format;

    bool isBinary = false;
    if (format == "P1") type = BIN;
    else if (format == "P2") type = GRAY;
    else if (format == "P3") type = RGB;
    else if (format == "P4") { type = BIN; isBinary = true; }
    else if (format == "P5") { type = GRAY; isBinary = true; }
    else if (format == "P6") { type = RGB; isBinary = true; }
    else return false;

    // rozmiar
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream dim(line);
        dim >> sizeX >> sizeY;
        if (dim) break;
    }

    // maxValue
    if (type != BIN) {
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::istringstream mv(line);
            mv >> maxValue;
            if (mv) break;
        }
    }
    else {
        maxValue = 1;
    }

    // Alokacja
    int bytesPerPixel = (type == RGB) ? 3 : 1;
    if (pixels) {
        for (unsigned int i = 0; i < sizeY; i++) delete[] pixels[i];
        delete[] pixels;
    }
    pixels = new unsigned char* [sizeY];
    for (unsigned int i = 0; i < sizeY; i++)
        pixels[i] = new unsigned char[sizeX * bytesPerPixel]();

    // Wczytywanie danych
    if (!isBinary) {
        for (unsigned int y = 0; y < sizeY; y++) {
            for (unsigned int x = 0; x < sizeX; x++) {
                if (type == RGB) {
                    for (int c = 0; c < 3; c++) {
                        int val;
                        if (!(file >> val)) {
                            return false;
                        }
                        pixels[y][x * 3 + c] = static_cast<unsigned char>(val);
                    }
                }
                else {
                    int val;
                    if (!(file >> val)) {
                        return false;
                    }
                    pixels[y][x] = static_cast<unsigned char>(val);
                }
            }
        }
    }
    else {
        if (type == BIN) {
            // 1-bit na piksel
            for (unsigned int y = 0; y < sizeY; y++) {
                int rowBytes = (sizeX + 7) / 8;
                unsigned char* buf = new unsigned char[rowBytes];

                file.read(reinterpret_cast<char*>(buf), rowBytes);
                if (file.gcount() != rowBytes) {
                    delete[] buf;
                    return false;
                }

                for (unsigned int x = 0; x < sizeX; x++) {
                    pixels[y][x] = ((buf[x / 8] >> (7 - (x % 8))) & 1) ? 1 : 0;
                }
                delete[] buf;
            }
        }
    }

    return true;
}
bool Image::save(const string& filename) {
    ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    if (type == BIN) file << "P1" << "\n";
    else if (type == GRAY) file << "P2" << "\n";
    else if (type == RGB) file << "P3" << "\n";

    file << sizeX << " " << sizeY << "\n";
    if (type != BIN) file << maxValue << "\n";
    file << '\n';

    //int bytesPerPixel = (type == RGB) ? 3 : 1;

    // ASCII
    for (unsigned int y = 0; y < sizeY; y++) {
        for (unsigned int x = 0; x < sizeX; x++) {
            if (type == RGB) {
                file << static_cast<int>(pixels[y][x * 3 + 0]) << " " << static_cast<int>(pixels[y][x * 3 + 1]) << " " << static_cast<int>(pixels[y][x * 3 + 2]);
            }
            else {
                file << static_cast<int>(pixels[y][x]);
            }
            if (x < sizeX - 1) file << " ";
        }
        file << "\n";
    }


    return file.good();
}
