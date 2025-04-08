#include "../include/converter.h"
#include "../include/g_png.h"
#include "../include/lib_usage.h"
#include "../include/parser.h"
#include "../include/print.h"

#include <png.h>

int main(int argc, char *argv[])
{
	const char *input_file, *output_file;
	// Парщу аргументы
	if (!parse_args(argc, argv, &input_file, &output_file))
	{
		return 1;
	}

	png_bytep *rows = NULL;
	int width, height, color_type, bit_depth;
	png_colorp palette = NULL;
	int num_palette = 0;

	// Считываю изображение
	if (!read_png(input_file, &rows, &width, &height, &color_type, &bit_depth, &palette, &num_palette))
	{
		print_error_and_exit("Invalid input file: not a supported 8-bit PNG of type 0, 2, or 3.");
	}

	// Создаю новые параметры для конвертированного изображения
	int new_color_type;
	png_colorp new_palette = NULL;
	int new_num_palette = 0;

	// Конвертирую
	png_bytep *converted_rows = convert_image(
		rows,
		width,
		height,
		color_type,
		bit_depth,
		palette,
		num_palette,
		&new_color_type,
		&new_palette,
		&new_num_palette
	);

	if (!converted_rows)
	{
		print_error_and_exit("Conversion failed.");
	}

	// Записываю результат
	if (!write_png(output_file, converted_rows, width, height, new_color_type, 8, new_palette, new_num_palette))
	{
		print_error_and_exit("Could not write output PNG file.");
	}

	// Освобождаю память
	free_png_rows(rows, height);
	free_png_rows(converted_rows, height);
	if (palette)
		free(palette);
	if (new_palette)
		free(new_palette);

	return 0;
}
