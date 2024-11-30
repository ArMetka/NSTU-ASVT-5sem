#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#include "./solution.h"

int main(int argc, char **argv) {
    char i_filename[256];
    char o_filename[256];
    int find_width, find_height;
    int center_x, center_y;
    int width, height;
    png_bytep *row_ptrs = NULL;

    printf("\n");

    printf("Enter input image filename (.png): ");
    scanf("%s", i_filename);

    printf("Enter output image filename (.png): ");
    scanf("%s", o_filename);

    printf("Enter brightness center dimensions (x y): ");
    scanf("%d %d", &find_width, &find_height);

    printf("\n");

    printf("Input data:\n");
    printf("\tinput filename(path): %s\n", i_filename);
    printf("\toutput filename(path): %s\n", o_filename);
    printf("\tbrightness center width: %d\n", find_width);
    printf("\tbrightness center height: %d\n", find_height);

    read_png_file(i_filename, &width, &height, &row_ptrs);
    convert_to_grayscale(width, height, &row_ptrs);
    find_brightness_center(width, height, &row_ptrs, find_width, find_height, &center_x, &center_y);
    write_png_file(o_filename, width, height, row_ptrs);

    printf("\n");

    return 0;
}

void convert_to_grayscale(int width, int height, png_bytep **row_ptrs) {
    for (int y = 0; y < height; y++) {
        png_bytep row = (*row_ptrs)[y];
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            int grayscale = 0.2126f * px[0] + 0.7152f * px[1] + 0.0722f * px[2];
            px[0] = px[1] = px[2] = grayscale;
        }
    }
}

void find_brightness_center(int width, int height, png_bytep **row_ptrs,
                            int answer_width, int answer_height, int *asnwer_x, int *answer_y) {
    int max_x = width - answer_width;
    int max_y = height - answer_height;

    int **sum_matrix = (int **)malloc(sizeof(int *) * max_y);
    for (int i = 0; i < max_y; i++) {
        sum_matrix[i] = (int *)malloc(sizeof(int) * max_x);
    }

    for (int y = 0; y < max_y; y++) {
        for (int x = 0; x < max_x; x++) {
            sum_matrix[y][x] = 0;
        }
    }

    for (int offset_y = 0; offset_y < max_y; offset_y++) {
        for (int offset_x = 0; offset_x < max_x; offset_x++) {
            for (int y = offset_y; y < offset_y + answer_height; y++) {
                png_bytep row = (*row_ptrs)[y];
                for (int x = offset_x; x < offset_x + answer_width; x++) {
                    png_bytep px = &(row[x * 4]);
                    sum_matrix[offset_y][offset_x] += px[0];
                }
            }
        }
    }

    int max_sum = -1;

    for (int y = 0; y < max_y; y++) {
        for (int x = 0; x < max_x; x++) {
            if (sum_matrix[y][x] > max_sum) {
                max_sum = sum_matrix[y][x];
            }
        }
    }

    printf("max_sum = %d\n", max_sum);

    for (int y = 0; y < max_y; y++) {
        for (int x = 0; x < max_x; x++) {
            if (sum_matrix[y][x] == max_sum) {
                paint_borders(1, width, height, row_ptrs, answer_width, answer_height, x, y);
                return;
            }
        }
    }
}

void paint_borders(int add_border_px, int width, int height, png_bytep **row_ptrs,
                   int paint_width, int paint_height, int paint_offset_x, int paint_offset_y) {
    if (add_border_px) {
        paint_width += 2;
        paint_height += 2;
        paint_offset_x -= 1;
        paint_offset_y -= 1;
    }
    int max_x = paint_offset_x + paint_width;
    int max_y = paint_offset_y + paint_height;

    for (int y = paint_offset_y; y < max_y; y++) {
        png_bytep row = (*row_ptrs)[y];
        for (int x = paint_offset_x; x < max_x; x++) {
            png_bytep px = &(row[x * 4]);
            if (x < width && y < height) {
                if (y == paint_offset_y || y == max_y - 1 || x == paint_offset_x || x == max_x - 1) {
                    px[0] = 0xFF;
                    px[1] = px[2] = 0;
                }
            }
        }
    }
}