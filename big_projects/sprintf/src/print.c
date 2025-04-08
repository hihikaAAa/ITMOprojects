#include "../include/print.h"

#include "../include/formater.h"
#include "../include/int128.h"
#include "../include/lib_usage.h"
#include "../include/number_to_parse.h"
#include "../include/parser.h"

int print(char *out_buf, const char *format, const char *number)
{
	struct formater helper;
	if (parser(format, &helper) != 0)
	{
		return 1;
	}

	int128_t value;
	if (numb(number, &value) != 0)
	{
		return 1;
	}

	format_number(value, &helper, out_buf);
	return 0;
}
