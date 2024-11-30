#ifndef SOLUTION_H
#define SOLUTION_H

void read_png_file(char *filename, int *width, int *height, png_bytep **row_ptrs);
void write_png_file(char *filename, int width, int height, png_bytep *row_ptrs);
void process_png_file(int width, int height, png_bytep **row_ptrs);

#endif