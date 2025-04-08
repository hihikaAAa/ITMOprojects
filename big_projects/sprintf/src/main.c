#include "../include/lib_usage.h"
#include "../include/print.h"

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <format> <number>\n", argv[0]);
		return 1;
	}
	char *format = argv[1];
	char *number = argv[2];
	char out_buf[256];
	if (print(out_buf, format, number) != 0)
	{
		return 1;
	}
	puts(out_buf);
	return 0;
}
