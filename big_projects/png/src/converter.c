#include "../include/converter.h"
#include "../include/lib_usage.h"
#include "../include/print.h"

#include <png.h>

typedef struct
{
    unsigned char r, g, b;
} RGBColor;

/**
 * Функция сравнения двух RGB-цветов, используется для сортировки и бинарного поиска.
 */
int compare_rgbcolor(const void *a, const void *b)
{
    const RGBColor *c1 = a;
    const RGBColor *c2 = b;
    if (c1->r != c2->r)
        return (int)c1->r - (int)c2->r;
    if (c1->g != c2->g)
        return (int)c1->g - (int)c2->g;
    return (int)c1->b - (int)c2->b;
}

/**
 * Преобразует пиксели из GRAY в RGB:
 * Каждый пиксель (одно значение) становится тремя компонентами (r, g, b).
 */
png_bytep *gray_to_rgb(png_bytep *rows, int width, int height)
{
    png_bytep *out = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++)
    {
        // Выделяю память под 3 байта на пиксель
        out[y] = (png_byte *)malloc(width * 3);
        png_bytep inrow = rows[y];
        png_bytep outrow = out[y];
        for (int x = 0; x < width; x++)
        {
            // Извлекаю значение серого
            png_byte g = inrow[x];
            // Записываю одинаковые компоненты в r, g, b
            outrow[x * 3 + 0] = g;
            outrow[x * 3 + 1] = g;
            outrow[x * 3 + 2] = g;
        }
    }
    return out;
}

/**
 * Обратная gray_to_rgb
 */
png_bytep *rgb_to_gray(png_bytep *rows, int width, int height)
{
    png_bytep *out = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++)
    {
        out[y] = (png_byte *)malloc(width);
        png_bytep inrow = rows[y];
        png_bytep outrow = out[y];
        for (int x = 0; x < width; x++)
        {
            png_byte r = inrow[x * 3 + 0];
            png_byte gray = r;
            outrow[x] = gray;
        }
    }
    return out;
}

/**
 * Преобразует пиксели из индексированного формата (с палитрой) в RGB.
 * Каждый индекс заменяется на соответствующий цвет (r, g, b) из palette.
 */
png_bytep *palette_to_rgb(png_bytep *rows, int width, int height, png_colorp palette, int num_palette)
{
    png_bytep *out = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++)
    {
        out[y] = (png_byte *)malloc(width * 3);
        png_bytep inrow = rows[y];
        png_bytep outrow = out[y];
        for (int x = 0; x < width; x++)
        {
            int idx = inrow[x];
            if (idx < 0 || idx >= num_palette)
            {
                print_error_and_exit("Invalid palette index encountered.");
            }
            outrow[x * 3 + 0] = palette[idx].red;
            outrow[x * 3 + 1] = palette[idx].green;
            outrow[x * 3 + 2] = palette[idx].blue;
        }
    }
    return out;
}

int binary_search_color(RGBColor *array, int size, RGBColor key)
{
    int low = 0;
    int high = size - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        int cmp = compare_rgbcolor(&key, &array[mid]);
        if (cmp == 0)
        {
            return mid;
        }
        else if (cmp < 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return -1;
}

/**
 * Преобразует изображение из RGB в формат палитры (индексированный).
 * 1) Собирает все цвета, сортирует их и определяет их уникальное количество.
 * 2) Если уникальных цветов более 256, программа завершается (для палитры допустимо только до 256).
 * 3) Формирует готовую палитру (png_color).
 * 4) Заменяет каждый пиксель на индекс соответствующего цвета в палитре.
 */
png_bytep *rgb_to_palette(
    png_bytep *rows,
    int width,
    int height,
    int *new_color_type,
    png_colorp *out_palette,
    int *out_num_palette
)
{
    int max_pixels = width * height;
    // Выделяю массив для хранения всех цветов
    RGBColor *colors = malloc(sizeof(RGBColor) * max_pixels);
    int count = 0;

    // Перебираю все пиксели и сохраняем их в массив colors
    for (int y = 0; y < height; y++)
    {
        png_bytep row = rows[y];
        for (int x = 0; x < width; x++)
        {
            RGBColor c;
            c.r = row[x * 3 + 0];
            c.g = row[x * 3 + 1];
            c.b = row[x * 3 + 2];
            colors[count++] = c;
        }
    }

    // Сортирую все цвета для последующего выделения уникальных
    qsort(colors, count, sizeof(RGBColor), compare_rgbcolor);

    // Подсчитываю количество уникальных цветов
    int unique_count = 1;
    for (int i = 1; i < count; i++)
    {
        if (compare_rgbcolor(&colors[i], &colors[i - 1]) != 0)
        {
            unique_count++;
        }
    }

    // Если уникальных цветов больше 256, завершаю с ошибкой
    if (unique_count > 256)
    {
        free(colors);
        print_error_and_exit("Too many unique colors for palette.");
        return NULL;
    }

    // Сохраняю уникальные цвета в отдельном массиве
    RGBColor *unique_colors = (RGBColor *)malloc(sizeof(RGBColor) * unique_count);
    unique_colors[0] = colors[0];
    int uidx = 0;
    for (int i = 1; i < count; i++)
    {
        if (compare_rgbcolor(&colors[i], &colors[i - 1]) != 0)
        {
            uidx++;
            unique_colors[uidx] = colors[i];
        }
    }
    uidx++; // т.к. я начинал с 0

    free(colors);

    // Создаю выходной массив скан-линий (только индексы)
    png_bytep *out = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++)
    {
        out[y] = (png_byte *)malloc(width);
        png_bytep inrow = rows[y];
        png_bytep outrow = out[y];
        for (int x = 0; x < width; x++)
        {
            RGBColor c;
            c.r = inrow[x * 3 + 0];
            c.g = inrow[x * 3 + 1];
            c.b = inrow[x * 3 + 2];

            // Нахожу индекс этого цвета в уникальном списке
            int found_index = binary_search_color(unique_colors, uidx, c);
            if (found_index < 0)
            {
                free(unique_colors);
                print_error_and_exit("Color not found in palette (unexpected).");
            }
            outrow[x] = (png_byte)found_index;
        }
    }

    // Формирую готовую палитру png_color
    png_color *palette_array = malloc(sizeof(png_color) * uidx);
    for (int i = 0; i < uidx; i++)
    {
        palette_array[i].red   = unique_colors[i].r;
        palette_array[i].green = unique_colors[i].g;
        palette_array[i].blue  = unique_colors[i].b;
    }
    free(unique_colors);

    // Заполняем выходные параметры
    *out_palette = palette_array;
    *out_num_palette = uidx;
    *new_color_type = PNG_COLOR_TYPE_PALETTE;

    return out;
}

/**
 * Вспомогательная функция, которая проверяет, являются ли все пиксели
 * шкалой серого (r == g == b). Если да, можно переводить RGB в GRAY.
 */
bool is_all_grayscale(png_bytep *rows, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        png_bytep row = rows[y];
        for (int x = 0; x < width; x++)
        {
            png_byte r = row[x * 3 + 0];
            png_byte g = row[x * 3 + 1];
            png_byte b = row[x * 3 + 2];
            if (r != g || g != b)
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * Главная функция преобразования изображения.
 * В зависимости от типа color_type (GRAY/RGB/PALETTE) выбирает,
 * какую функцию вызвать, и возвращает новое изображение.
 */
png_bytep *convert_image(
    png_bytep *rows,
    int width,
    int height,
    int color_type,
    int bit_depth,
    png_colorp palette,
    int num_palette,
    int *new_color_type,
    png_colorp *new_palette,
    int *new_num_palette
)
{
    // Сбрасываем значения палитры и её размера (если не понадобится, останется NULL)
    *new_palette = NULL;
    *new_num_palette = 0;

    // Если изображение в оттенках серого (GRAY), переводим в RGB
    if (color_type == PNG_COLOR_TYPE_GRAY)
    {
        png_bytep *out = gray_to_rgb(rows, width, height);
        *new_color_type = PNG_COLOR_TYPE_RGB;
        return out;
    }
    // Если уже RGB, смотрим, все ли пиксели серые
    else if (color_type == PNG_COLOR_TYPE_RGB)
    {
        if (is_all_grayscale(rows, width, height))
        {
            // Переводим в GRAY
            png_bytep *out = rgb_to_gray(rows, width, height);
            *new_color_type = PNG_COLOR_TYPE_GRAY;
            return out;
        }
        else
        {
            // Иначе переводим в палитру
            png_bytep *out = rgb_to_palette(rows, width, height, new_color_type, new_palette, new_num_palette);
            return out;
        }
    }
    // Если изображение индексированное (палитра), переводим в RGB
    else if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_bytep *out = palette_to_rgb(rows, width, height, palette, num_palette);
        *new_color_type = PNG_COLOR_TYPE_RGB;
        return out;
    }

    // Если вдруг встретился другой тип (RGBA и пр.), завершаем
    print_error_and_exit("Unsupported color type in convert_image.");
    return NULL;
}
