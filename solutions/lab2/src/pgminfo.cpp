#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

int main() {
    string magic;
    int rows, cols, maxval;
    vector<int> pixels;

    // Read the magic number
    if (!(cin >> magic) || magic != "P2") {
        cerr << "Bad PGM file -- first line must be P2\n";
        return 1;
    }

    // Read rows and columns
    if (!(cin >> cols >> rows) || cols <= 0 || rows <= 0) {
        cerr << "Bad PGM file -- invalid size\n";
        return 1;
    }

    // Read max value
    if (!(cin >> maxval) || maxval != 255) {
        cerr << "Bad PGM file -- invalid max gray value\n";
        return 1;
    }

    // Read pixels
    int pixel;
    long long sum = 0;
    for (int i = 0; i < rows * cols; i++) {
        if (!(cin >> pixel) || pixel < 0 || pixel > 255) {
            cerr << "Bad PGM file -- invalid pixel value\n";
            return 1;
        }
        pixels.push_back(pixel);
        sum += pixel;
    }

    // Check if there are extra pixels
    if (cin >> pixel) {
        cerr << "Bad PGM file -- too many pixels\n";
        return 1;
    }

    // Calculate average
    double average = static_cast<double>(sum) / (rows * cols);

    // Print results
    printf("# Rows:    %d\n", rows);
    printf("# Columns: %d\n", cols);
    printf("# Pixels:  %d\n", rows * cols);
    printf("Avg Gray:  %.3f\n", average);

    return 0;
}