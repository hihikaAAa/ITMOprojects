#include "../include/parser.h"

#include "../include/lib_usage.h"

int parser(const char *format, struct formater *helper)
{
	memset(helper->flags, 0, sizeof(helper->flags));
	helper->size = 0;
	helper->conversion_helper = 0;

	const char *p = format;

	p = strchr(format, '%');
	if (!p)
	{
		fprintf(stderr, "Error: Invalid format (no '%%' found)\n");
		return 1;
	}
	p++;

	while (*p == ' ' || *p == '-' || *p == '+' || *p == '#' || *p == '0')
	{
		if (strchr(helper->flags, *p) == NULL)
		{
			size_t len = strlen(helper->flags);
			if (len < sizeof(helper->flags) - 1)
			{
				helper->flags[len] = *p;
				helper->flags[len + 1] = '\0';
			}
			else
			{
				fprintf(stderr, "Error: Invalid format\n");
				return 1;
			}
		}
		p++;
	}

	if (isdigit((unsigned char)*p))
	{
		helper->size = 0;
		while (isdigit((unsigned char)*p))
		{
			helper->size = helper->size * 10 + (*p - '0');
			p++;
		}
	}

	if (*p == 'b' || *p == 'o' || *p == 'd' || *p == 'x' || *p == 'X' || *p == 'z')
	{
		helper->conversion_helper = *p;
	}
	else
	{
		fprintf(stderr, "Error: Unsupported format specifier '%c'\n", *p);
		return 1;
	}
	p++;

	if (*p != '\0')
	{
		fprintf(stderr, "Error: Invalid format (extra characters after format specifier)\n");
		return 1;
	}
	return 0;
}
// анализ строки формата , выделение флагов , ширину и спецификатор "%#08x": flags = "#0", size = 8, conversion_helper = 'x'
