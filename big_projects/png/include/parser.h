#ifndef PARSER_H
#define PARSER_H

#include "lib_usage.h"
#include <png.h>
#include <stdbool.h>

bool parse_args(int argc, char *argv[], const char **input_file, const char **output_file);

#endif  // PARSER_H
