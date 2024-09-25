#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

using namespace std;

struct PGMImage {
    string magic;
    int width, height, maxval;
    vector<int> pixels;
};

void printError(const string& message) {
    cerr << "Error: " << message << endl;
    exit(1);
}

PGMImage readPGM() {
    PGMImage img;
    if (!(cin >> img.magic) || img.magic != "P2") {
        printError("Invalid PGM file - must start with P2");
    }
    
    if (!(cin >> img.width >> img.height) || img.width <= 0 || img.height <= 0) {
        printError("Invalid PGM dimensions");
    }
    
    if (!(cin >> img.maxval) || img.maxval != 255) {
        printError("Invalid max gray value - must be 255");
    }
    
    img.pixels.resize(img.width * img.height);
    for (int& pixel : img.pixels) {
        if (!(cin >> pixel) || pixel < 0 || pixel > 255) {
            printError("Invalid pixel value");
        }
    }
    
    int extra;
    if (cin >> extra) {
        printError("Too many pixels in input");
    }
    
    return img;
}

void writePGM(const PGMImage& img) {
    printf("P2\n%d %d\n255\n", img.width, img.height);
    for (int i = 0; i < img.pixels.size(); ++i) {
        printf("%d", 255 - img.pixels[i]);
        if ((i + 1) % img.width == 0 || i == img.pixels.size() - 1) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
}

int main() {
    try {
        PGMImage img = readPGM();
        writePGM(img);
    } catch (const exception& e) {
        printError(e.what());
    }
    return 0;
}