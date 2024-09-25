#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

void print_error(const char* message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

int main(int argc, char* argv[]) {
    // Check number of arguments
    if (argc != 3) {
        print_error("Usage: graycard <rows> <columns>");
    }

    // Parse and validate rows and columns
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    if (rows <= 0 || cols <= 0) {
        print_error("Error: Rows and columns must be positive integers");
    }

    if (rows % 3 != 0) {
        print_error("Error: Number of rows must be a multiple of 3");
    }

    // Print PGM header
    printf("P2\n");
    printf("%d %d\n", cols, rows);
    printf("255\n");

    // Calculate the number of rows for each section
    int section_rows = rows / 3;

    // Print pixel values
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i < section_rows) {
                printf("0 ");  // Black
            } else if (i < 2 * section_rows) {
                printf("209 ");  // 18% Gray
            } else {
                printf("255 ");  // White
            }
        }
        printf("\n");
    }

    return 0;
}