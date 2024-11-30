#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#include "./solution.h"

int main(int argc, char **argv) {
    char filename[256];
    int width, height;
    png_bytep *row_ptrs = NULL;

    printf("Enter filename: ");
    scanf("%s", filename);

    read_png_file(filename, &width, &height, &row_ptrs);
    process_png_file(width, height, &row_ptrs);
    write_png_file("GOOOoooOoAL.png", width, height, row_ptrs);

    return 0;
}

void process_png_file(int width, int height, png_bytep **row_ptrs) {
    for(int y = 0; y < height; y++) {
        png_bytep row = (*row_ptrs)[y];
        for(int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
        }
    }
}