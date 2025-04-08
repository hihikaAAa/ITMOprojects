#include "../include/number_to_parse.h"

#include "../include/int128.h"
#include "../include/lib_usage.h"

int numb(const char *number, int128_t *value)
{
	bool is_negative = false;
	int base = 10;
	const char *p = number;

	if (*p == '-')
	{
		is_negative = true;
		p++;
	}

	if (p[0] == '0')
	{
		if (p[1] == 'x' || p[1] == 'X')
		{
			base = 16;
			p += 2;
		}
		else if (p[1] == 'b' || p[1] == 'B')
		{
			base = 2;
			p += 2;
		}
		else if(p[1] =='z')
			{
				base = 3;
				p += 2;
				}
		else if (isdigit(p[1]))
		{
			base = 8;
			p++;
		}
	}

	int128_t result = { 0, 0 };
	while (*p)
	{
		char c = tolower(*p);
		uint64_t digit = 0;

		if (c >= '0' && c <= '9')
		{
			digit = c - '0';
		}
		else if (base == 16 && c >= 'a' && c <= 'f')
		{
			digit = c - 'a' + 10;
		}
		else
		{
			fprintf(stderr, "Error: Invalid character '%c' in number\n", c);
			return 1;
		}

		result = multiply128(result, base);
		result = add128(result, (int128_t){ .high = 0, .low = digit });
		p++;
	}

	if (is_negative)
	{
		result = negate128(result);
	}

	*value = result;
	return 0;
}
// преобразование строки в int128