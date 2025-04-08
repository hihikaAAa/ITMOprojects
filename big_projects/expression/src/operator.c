#include "../include/operator.h"

int g_error_code = 0;

/**
 * Функция для парсинга строки в целое число (int).
 * Поддерживает форматы:
 *  - Десятичный
 *  - Шестнадцатеричный (0xFF, 0XFF)
 *  - Знак '+' или '-'
 * Выполняет проверки на переполнение 32-битного int.
 */
int parseInt(const char *str)
{
	int sign = 1;
	int i = 0;
	// Проверяем знак
	if (str[0] == '-')
	{
		sign = -1;
		i = 1;
	}
	else if (str[0] == '+')
	{
		i = 1;
	}

	// Проверка на шестнадцатеричный формат: 0x или 0X
	if ((str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X')))
	{
		i += 2;
		long result = 0;
		for (; str[i] != '\0'; i++)
		{
			char c = (char)tolower((unsigned char)str[i]);
			int digit;
			if (c >= '0' && c <= '9')
				digit = c - '0';
			else if (c >= 'a' && c <= 'f')
				digit = 10 + (c - 'a');
			else
			{
				fprintf(stderr, "Invalid hex number format\n");
				if (g_error_code == 0)
					g_error_code = ERROR_PARSE;
				return 0;
			}
			// Проверяю переполнение (2147483647L ~ INT_MAX)
			if (result > (2147483647L / 16))
			{
				fprintf(stderr, "Integer literal out of 32-bit range\n");
				if (g_error_code == 0)
					g_error_code = ERROR_PARSE;
				return 0;
			}
			result = result * 16 + digit;
			if (result > 2147483647L && sign == 1)
			{
				fprintf(stderr, "Integer literal out of 32-bit range\n");
				if (g_error_code == 0)
					g_error_code = ERROR_PARSE;
				return 0;
			}
			else if (-result < (-2147483647L - 1) && sign == -1)
			{
				fprintf(stderr, "Integer literal out of 32-bit range\n");
				if (g_error_code == 0)
					g_error_code = ERROR_PARSE;
				return 0;
			}
		}
		if (sign == -1)
			result = -result;
		return (int)result;
	}
	else
	{
		// Парсинг десятичного
		long result = 0;
		for (; str[i] != '\0'; i++)
		{
			if (!isdigit((unsigned char)str[i]))
			{
				fprintf(stderr, "Invalid number format\n");
				if (g_error_code == 0)
					g_error_code = ERROR_PARSE;
				return 0;
			}
			int digit = str[i] - '0';
			long limit = (sign == 1) ? 2147483647L : (long)(-2147483647L - 1);
			// Проверяю, не превысит ли результат INT_MAX или INT_MIN
			if (result > (limit - digit) / 10)
			{
				fprintf(stderr, "Integer literal out of 32-bit range\n");
				if (g_error_code == 0)
					g_error_code = ERROR_PARSE;
				return 0;
			}
			result = result * 10 + digit;
		}
		if (sign == -1)
		{
			result = -result;
			if (result < (-2147483647L - 1))
			{
				fprintf(stderr, "Integer literal out of 32-bit range\n");
				if (g_error_code == 0)
					g_error_code = ERROR_PARSE;
				return 0;
			}
		}
		else
		{
			if (result > 2147483647L)
			{
				fprintf(stderr, "Integer literal out of 32-bit range\n");
				if (g_error_code == 0)
					g_error_code = ERROR_PARSE;
				return 0;
			}
		}
		return (int)result;
	}
}
