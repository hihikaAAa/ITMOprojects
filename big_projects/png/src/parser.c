#include "../include/parser.h"
#include "../include/lib_usage.h"
#include <png.h>

bool parse_args(int argc, char *argv[], const char **input_file, const char **output_file)
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <input_png> <output_png>\n", (argc > 0) ? argv[0] : "program");
		return false;
	}
	*input_file = argv[1];
	*output_file = argv[2];
	return true;
}
