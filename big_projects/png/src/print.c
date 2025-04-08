#include "../include/print.h"
#include "../include/lib_usage.h"
#include <png.h>

void print_error_and_exit(const char *message)
{
	fprintf(stderr, "%s\n", message);
	return;
}
