#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#include "./solution.h"

void read_png_file(char *filename, int *width, int *height, png_bytep **row_ptrs) {
    png_byte color_type, bit_depth;

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("failed to open file \"%s\"\n", filename);
        exit(1);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        printf("failed to create png read struct!\n");
        exit(1);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        printf("failed to create png info struct!\n");
        exit(1);
    }

    // ??????
    if (setjmp(png_jmpbuf(png))) {
        printf("failed to setjmp!\n");
        exit(1);
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) {
        png_set_strip_16(png);
    }

    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png);
    }

    if (png_get_valid(png, info, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png);
    }

    if (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE) {
            png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
        }

    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            png_set_gray_to_rgb(png);
        }

    png_read_update_info(png, info);

    *row_ptrs = (png_bytep *)malloc(sizeof(png_bytep) * *height);
    for (int y = 0; y < *height; y++) {
        (*row_ptrs)[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, *row_ptrs);

    fclose(fp);
    png_destroy_read_struct(&png, &info, NULL);
}

void write_png_file(char *filename, int width, int height, png_bytep *row_ptrs) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("failed to open file \"%s\"\n", filename);
        exit(1);
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        printf("failed to create png write struct!\n");
        exit(1);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        printf("failed to create png info struct!\n");
        exit(1);
    }

    // ??????
    if (setjmp(png_jmpbuf(png))) {
        printf("failed to setjmp!\n");
        exit(1);
    }

    png_init_io(png, fp);

    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
        png,
        info,
        width, height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    if (!row_ptrs) {
        printf("failed to write image: row_ptrs is NULL!\n");
        exit(1);
    }

    png_write_image(png, row_ptrs);
    png_write_end(png, NULL);

    for(int y = 0; y < height; y++) {
        free(row_ptrs[y]);
    }
    free(row_ptrs);

    fclose(fp);
    png_destroy_write_struct(&png, &info);
}