#include "../include/converter.h"

#include "../include/int128.h"
#include "../include/lib_usage.h"

void converter(int128_t value, int base, char *str, int uppercase)
{
	static const char digits_lower[] = "0123456789abcdefz";
	static const char digits_upper[] = "0123456789ABCDEF";
	const char *digits = uppercase ? digits_upper : digits_lower;

	char buffer[256];
	int index = 255;
	buffer[index--] = '\0';

	bool is_negative = false;

	if (is_negative128(value))
	{
		is_negative = true;
		value = negate128(value);
	}

	if (is_zero(value))
	{
		buffer[index--] = '0';
	}
	else
	{
		uint64_t remainder;
		while (!is_zero(value))
		{
			value = divide128(value, base, &remainder);
			buffer[index--] = digits[remainder];
		}
	}

	if (is_negative)
	{
		buffer[index--] = '-';
	}

	strcpy(str, &buffer[index + 1]);
}
// Создали массив buffer в котором на последнем элементе ставится символ конца строки. Далее проверяется на отрицательность.
// Проверяется, установлен ли старший бит, если установлен = отриц число. Далее проверка на 0. Если старший и младший биты
// равны 0 => число = 0. Если нет -> смотрим остаток деления и записываем в массив. Если число отриц - добавляем минус,
// после чего копируем в str значение buffer.
