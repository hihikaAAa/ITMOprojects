#ifndef CONVERTER_H
#define CONVERTER_H

#include "lib_usage.h"
#include <png.h>

/**
 * Функция convert_image осуществляет преобразование PNG-изображения
 * в разные форматы (GRAY -> RGB, PALETTE -> RGB, RGB -> GRAY/PALETTE).
 * return Массив строк (скан-линий) преобразованного изображения
 */
png_bytep *convert_image(
    png_bytep *rows, //Массив строк (скан-линий) исходного изображения
    int width, //Ширина изображения
    int height, //Высота изображения
    int color_type, //Исходный тип цветности (PNG_COLOR_TYPE_*: GRAY, RGB, PALETTE и т.д.)
    int bit_depth, //Глубина цвета (в нашем случае ожидается 8)
    png_colorp palette, //Указатель на палитру (если есть), используется когда color_type = PALETTE
    int num_palette, // Количество цветов в палитре
    int *new_color_type, //[Выходной параметр] Тип цветности после конвертации
    png_colorp *new_palette, //[Выходной параметр] Новая палитра (если после конвертации получилось PALETTE)
    int *new_num_palette //[Выходной параметр] Количество цветов в новой палитре (если после конвертации получилось PALETTE)
);

/**
 * Преобразует оттенки серого (GRAY) в RGB.
 * return Массив строк (скан-линий) в формате RGB
 */
png_bytep *gray_to_rgb(png_bytep *rows, int width, int height);

/**
 * Преобразует RGB в палитру (индексированное изображение).
 * return Массив строк (скан-линий) индексированного изображения
 */
png_bytep *rgb_to_palette(
    png_bytep *rows, //Исходные пиксели в формате RGB
    int width,
    int height,
    int *new_color_type, //[Выход] тип изображения будет PNG_COLOR_TYPE_PALETTE
    png_colorp *out_palette, //[Выход] указатель на сгенерированную палитру
    int *out_num_palette //[Выход] количество цветов в палитре
);

/**
 * Преобразует индексированное изображение (палитра) в RGB.
 * return Массив строк (скан-линий) в формате RGB
 */
png_bytep *palette_to_rgb(
    png_bytep *rows, //Исходные данные (индексы цветов)
    int width,
    int height,
    png_colorp palette, //Палитра, где каждый индекс соответствует цвету
    int num_palette //Количество цветов в палитре
);

#endif  // CONVERTER_H
