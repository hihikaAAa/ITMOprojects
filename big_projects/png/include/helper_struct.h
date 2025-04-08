#ifndef HELPER_STRUCT_H
#define HELPER_STRUCT_H

#include <stdio.h>
#include <stdbool.h>
#include <png.h>


#define PNG_OK                                 0
#define PNG_ERROR_CANNOT_OPEN_FILE             1
#define PNG_ERROR_FILE_TOO_SHORT               2
#define PNG_ERROR_NOT_A_PNG                    3
#define PNG_ERROR_CREATE_READ_STRUCT           4
#define PNG_ERROR_CREATE_INFO_STRUCT           5
#define PNG_ERROR_READ                         6
#define PNG_ERROR_INVALID_BIT_DEPTH            7
#define PNG_ERROR_UNSUPPORTED_COLOR_TYPE       8
#define PNG_ERROR_INVALID_PALETTE              9
#define PNG_ERROR_CREATE_WRITE_STRUCT         10
#define PNG_ERROR_CREATE_INFO_STRUCT_WRITE    11
#define PNG_ERROR_WRITE                       12
#define PNG_ERROR_UNKNOWN                     13

const char *png_error_message_table[] = {
     "No error",
     "Cannot open file",
     "File too short",
     "Not a PNG file",
     "Failed to create png read struct",
     "Failed to create png info struct",
     "Error during png read",
     "Unsupported bit depth (only 8 bits supported)",
     "Unsupported color type (no alpha supported here)",
     "Invalid or empty palette",
     "Failed to create png write struct",
     "Failed to create png info struct (write)",
     "Error during png write",
     "Unknown error"
};


typedef struct
{
    FILE *fp;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep *row_pointers;
    png_colorp palette;
    int height;
} PngContext;


int release_png_context(PngContext *ctx, int error_code);

bool read_png(
    const char *filename,
    png_bytep  **rows,
    int        *width,
    int        *height,
    int        *color_type,
    int        *bit_depth,
    png_colorp *palette,
    int        *num_palette
);


bool write_png(
    const char   *filename,
    png_bytep    *rows,
    int           width,
    int           height,
    int           color_type,
    int           bit_depth,
    png_colorp    palette,
    int           num_palette
);


void free_png_rows(png_bytep *rows, int height);

#endif //HELPER_STRUCT_H
