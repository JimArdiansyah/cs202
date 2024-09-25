#include "pgm.hpp"
#include <fstream>  // For file input/output
#include <sstream>
#include <iostream> // For error messages
#include <iomanip>  // For formatting output

using namespace std;

// Function to read a PGM file
bool Pgm::Read(const string &file) {
    ifstream infile(file.c_str());  // Open the file for reading
    if (!infile) {  // If the file couldn't be opened, show an error and return false
        cerr << "Error opening file: " << file << endl;
        return false;
    }

    string magic;
    infile >> magic;  // Read the PGM file header (P2)
    if (magic != "P2") {  // If the format is not "P2", it's an invalid file
        cerr << "Invalid PGM format. Expected P2." << endl;
        return false;
    }

    size_t rows, cols, max_val;
    infile >> cols >> rows >> max_val;  // Read the image width, height, and max pixel value

    // Resize the Pixels 2D array to hold the image data
    Pixels.resize(rows, vector<int>(cols));
    for (size_t r = 0; r < rows; ++r) {  // Loop through each row
        for (size_t c = 0; c < cols; ++c) {  // Loop through each column
            if (!(infile >> Pixels[r][c])) {  // Read the pixel value; if it fails, show an error
                cerr << "Error reading pixel value." << endl;
                return false;
            }
        }
    }

    return true;  // Successfully read the file
}

// Function to write a PGM file
bool Pgm::Write(const string &file) const {
    ofstream outfile(file.c_str());  // Open the file for writing
    if (!outfile) {  // If the file couldn't be opened, show an error and return false
        cerr << "Error opening file for writing: " << file << endl;
        return false;
    }

    size_t rows = Pixels.size();  // Get the number of rows (height)
    size_t cols = Pixels[0].size();  // Get the number of columns (width)

    // Write the PGM file header
    outfile << "P2\n";
    outfile << cols << " " << rows << "\n255\n";  // Write image dimensions and max pixel value

    // Write each pixel value, 20 values per line
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            outfile << Pixels[r][c];  // Write the pixel value
            if (c < cols - 1) outfile << " ";  // Add a space between pixels
            if ((c + 1) % 20 == 0) outfile << "\n";  // Add a newline after 20 pixels
        }
        if (cols % 20 != 0) outfile << "\n";  // Ensure the line ends properly
    }

    return true;  // Successfully wrote the file
}

// Function to create a new PGM image with the given size and pixel value
bool Pgm::Create(size_t r, size_t c, size_t pv) {
    if (pv > 255) {  // Pixel value must be between 0 and 255
        cerr << "Pixel value must be between 0 and 255." << endl;
        return false;
    }

    Pixels.assign(r, vector<int>(c, pv));  // Set all pixels to the specified value
    return true;  // Image created successfully
}

// Rotate the image 90 degrees clockwise
bool Pgm::Clockwise() {
    if (Pixels.empty()) return false;  // If there's no image data, return false
    
    size_t rows = Pixels.size();
    size_t cols = Pixels[0].size();
    // Create a new array with swapped dimensions for the rotated image
    vector<vector<int> > new_pixels(cols, vector<int>(rows));  // Correct template nesting

    // Rotate each pixel to its new position
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            new_pixels[c][rows - r - 1] = Pixels[r][c];  // Rotate pixel
        }
    }

    Pixels = new_pixels;  // Update the image with the rotated version
    return true;  // Rotation successful
}

// Rotate the image 90 degrees counterclockwise
bool Pgm::Cclockwise() {
    if (Pixels.empty()) return false;  // If there's no image data, return false

    size_t rows = Pixels.size();
    size_t cols = Pixels[0].size();
    vector<vector<int> > new_pixels(cols, vector<int>(rows));  // Correct template nesting

    // Rotate each pixel to its new position
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            new_pixels[cols - c - 1][r] = Pixels[r][c];  // Rotate pixel
        }
    }

    Pixels = new_pixels;  // Update the image with the rotated version
    return true;  // Rotation successful
}

// Add padding around the image with a specified width and pixel value
bool Pgm::Pad(size_t w, size_t pv) {
    if (pv > 255) {  // Pixel value must be between 0 and 255
        cerr << "Pixel value must be between 0 and 255." << endl;
        return false;
    }

    size_t rows = Pixels.size();
    size_t cols = Pixels[0].size();
    // Create a new image larger by 2*w on all sides, filled with the padding value
    vector<vector<int> > new_pixels(rows + 2 * w, vector<int>(cols + 2 * w, pv));

    // Copy the original image into the center of the new padded image
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            new_pixels[r + w][c + w] = Pixels[r][c];
        }
    }

    Pixels = new_pixels;  // Update the image with the padded version
    return true;  // Padding successful
}

// Duplicate the image into a grid of r rows and c columns
bool Pgm::Panel(size_t r, size_t c) {
    if (Pixels.empty()) return false;  // If there's no image data, return false

    size_t orig_rows = Pixels.size();
    size_t orig_cols = Pixels[0].size();
    // Create a new image large enough to hold r rows and c columns of the original image
    vector<vector<int> > new_pixels(orig_rows * r, vector<int>(orig_cols * c));

    // Copy the original image into each panel position
    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) {
            for (size_t rr = 0; rr < orig_rows; ++rr) {
                for (size_t cc = 0; cc < orig_cols; ++cc) {
                    new_pixels[i * orig_rows + rr][j * orig_cols + cc] = Pixels[rr][cc];
                }
            }
        }
    }

    Pixels = new_pixels;  // Update the image with the panel version
    return true;  // Panel creation successful
}

// Crop the image to a specified rectangle
bool Pgm::Crop(size_t r, size_t c, size_t rows, size_t cols) {
    // If the crop dimensions are out of bounds, return false
    if (r + rows > Pixels.size() || c + cols > Pixels[0].size()) {
        cerr << "Invalid crop dimensions." << endl;
        return false;
    }

    // Create a new image to hold the cropped region
    vector<vector<int> > new_pixels(rows, vector<int>(cols));

    // Copy the specified rectangle from the original image
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            new_pixels[i][j] = Pixels[r + i][c + j];
        }
    }

    Pixels = new_pixels;  // Update the image with the cropped version
    return true;  // Cropping successful
}
