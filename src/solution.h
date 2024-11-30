#ifndef SOLUTION_H
#define SOLUTION_H

/**
 * Read .png file specified by filename
 */
void read_png_file(char *filename, int *width, int *height, png_bytep **row_ptrs);

/**
 * Write .png file specified by filename with data from row_ptrs
 */
void write_png_file(char *filename, int width, int height, png_bytep *row_ptrs);

/**
 * Convert .png image to grayscale
 */
void convert_to_grayscale(int width, int height, png_bytep **row_ptrs);

/**
 * Find brightness center (brightest rectangle of [answer_width x answer_height] size) and paint it red
 */
void find_brightness_center(int width, int height, png_bytep **row_ptrs,
                            int answer_width, int answer_height, int *asnwer_x, int *answer_y);

/**
 * Paint red borders of [paint_width x paint_height] size
 * with paint_offset_x and paint_offset_y offsets
 * 
 * @param add_border_px 0 -> exact size; 1-> add 1 px to each side of border
 */
void paint_borders(int add_border_px, int width, int height, png_bytep **row_ptrs,
                   int paint_width, int paint_height, int paint_offset_x, int paint_offset_y);

#endif