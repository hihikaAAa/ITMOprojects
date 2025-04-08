#ifndef G_PNG_H
#define G_PNG_H

#include "lib_usage.h"
#include <stdbool.h>
#include <png.h>

/**
 * Считывает PNG-файл и заполняет параметры изображения:
 * return true, если успешно, иначе false
 */
bool read_png(
    const char *filename, //Путь к входному PNG-файлу
    png_bytep **rows, //[Выход] Указатель на двумерный массив данных
    int *width,
    int *height,
    int *color_type, //[Выход] Тип цвета
    int *bit_depth, //[Выход] Глубина цвета
    png_colorp *palette, //[Выход] Указатель на палитру (если тип PALETTE)
    int *num_palette //[Выход] Количество цветов в палитре
);

/**
 * Записывает PNG-файл по указанному пути.
 * return true, если запись успешна, иначе false
 */
bool write_png(
    const char *filename, //Путь к выходному PNG-файлу
    png_bytep *rows, //Данные изображения
    int width,
    int height,
    int color_type, //Тип цвета (GRAY, RGB, PALETTE)
    int bit_depth, //Глубина цвета (обычно 8)
    png_colorp palette, //Палитра (если тип PALETTE)
    int num_palette  //Количество цветов в палитре (если тип PALETTE)
);

/**
 * Освобождает память, выделенную под строки PNG.
 */
void free_png_rows(png_bytep *rows, int height);

#endif  // G_PNG_H
