#include "../include/formater.h"
#include "../include/converter.h"
#include "../include/lib_usage.h"

void format_number(int128_t value, struct formater *helper, char *out_buf)
{
	int base; // Переменная для хранения системы счисления
	int uppercase = 0; // Флаг использования заглавных букв для шестнадцатеричных чисел

	// Определение системы счисления и регистра на основе спецификатора формата
	switch (helper->conversion_helper)
	{
	case 'd': // Десятичная система
	case 'i': // Целое число
		base = 10;
		break;
	case 'x': // Шестнадцатеричная система (строчные буквы)
		base = 16;
		uppercase = 0;
		break;
	case 'X': // Шестнадцатеричная система (заглавные буквы)
		base = 16;
		uppercase = 1;
		break;
	case 'o': // Восьмеричная система
		base = 8;
		break;
	case 'b': // Двоичная система
		base = 2;
		break;
	case 'z':
		base = 3;
		break;
	default: // По умолчанию — десятичная система
		base = 10;
		break;
	}

	char num_str[256]; // Буфер для хранения строки с числом
	converter(value, base, num_str, uppercase); // Преобразование числа в строку в указанной системе счисления

	bool is_negative = is_negative128(value); // Проверка, является ли число отрицательным

	char sign_char = '\0'; // Символ знака (если требуется)
	if (num_str[0] == '-') // Если строка начинается с "-", значит, число отрицательное
	{
		sign_char = '-'; // Устанавливаем знак
		memmove(num_str, num_str + 1, strlen(num_str) + 1); // Удаляем знак из строки, сдвигая её влево
	}
	else if (strchr(helper->flags, '+')) // Если включен флаг '+', добавляем знак "+" для положительных чисел
	{
		sign_char = '+';
	}
	else if (strchr(helper->flags, ' ')) // Если включен флаг ' ', добавляем пробел для положительных чисел
	{
		sign_char = ' ';
	}

	char prefix[3] = ""; // Префикс, например "0x" для шестнадцатеричных или "0" для восьмеричных чисел
	if (strchr(helper->flags, '#') && helper->conversion_helper != 'd' && helper->conversion_helper != 'i') // Проверка флага '#'
	{
		if (helper->conversion_helper == 'o') // Восьмеричная система
		{
			if (!is_zero(value)) // Префикс добавляется только для ненулевых чисел
			{
				strcpy(prefix, "0");
			}
		}
		else if (helper->conversion_helper == 'x') // Шестнадцатеричная система (строчные буквы)
		{
			strcpy(prefix, "0x");
		}
		else if (helper->conversion_helper == 'X') // Шестнадцатеричная система (заглавные буквы)
		{
			strcpy(prefix, "0X");
		}
		else if (helper->conversion_helper == 'b') // Двоичная система
		{
			strcpy(prefix, "0b");
		}
		else if(helper->conversion_helper == 'z')
			{
				strcpy(prefix, "0z");
			}
	}

	int sign_len = (sign_char != '\0') ? 1 : 0; // Длина знака (1, если знак есть, иначе 0)
	int prefix_len = strlen(prefix); // Длина префикса
	int num_len = strlen(num_str); // Длина числа в строке
	int total_len = sign_len + prefix_len + num_len; // Общая длина результата

	int padding_len = (helper->size > total_len) ? (helper->size - total_len) : 0; // Вычисление длины заполнения (если ширина больше длины результата)

	char temp_buf[256]; // Временный буфер для построения строки
	int pos = 0; // Текущая позиция в буфере

	char pad_char = ' '; // Символ заполнения (по умолчанию пробел)
	if (strchr(helper->flags, '0') && !strchr(helper->flags, '-')) // Если включен флаг '0' и нет выравнивания влево
	{
		pad_char = '0'; // Используется '0' для заполнения
	}

	if (strchr(helper->flags, '-')) // Если включен флаг '-', выравнивание по левому краю
	{
		if (sign_char != '\0') // Если есть знак, добавляем его
		{
			temp_buf[pos++] = sign_char;
		}
		if (prefix_len > 0) // Если есть префикс, добавляем его
		{
			strcpy(&temp_buf[pos], prefix);
			pos += prefix_len;
		}
		strcpy(&temp_buf[pos], num_str); // Копируем число
		pos += num_len;

		while (padding_len-- > 0) // Добавляем пробелы для выравнивания
		{
			temp_buf[pos++] = ' ';
		}
	}
	else // Если выравнивание по правому краю
	{
		if (pad_char == '0') // Если символ заполнения — '0'
		{
			if (sign_char != '\0') // Добавляем знак перед заполнением
			{
				temp_buf[pos++] = sign_char;
			}
			if (prefix_len > 0) // Добавляем префикс перед заполнением
			{
				strcpy(&temp_buf[pos], prefix);
				pos += prefix_len;
			}
			while (padding_len-- > 0) // Заполняем '0'
			{
				temp_buf[pos++] = '0';
			}
			strcpy(&temp_buf[pos], num_str); // Добавляем число
			pos += num_len;
		}
		else // Если символ заполнения — пробел
		{
			while (padding_len-- > 0) // Сначала добавляем пробелы
			{
				temp_buf[pos++] = ' ';
			}
			if (sign_char != '\0') // Затем добавляем знак
			{
				temp_buf[pos++] = sign_char;
			}
			if (prefix_len > 0) // Затем префикс
			{
				strcpy(&temp_buf[pos], prefix);
				pos += prefix_len;
			}
			strcpy(&temp_buf[pos], num_str);
			pos += num_len;
		}
	}

	temp_buf[pos] = '\0'; // Завершаем строку

	strcpy(out_buf, temp_buf); // Копируем готовую строку в выходной буфер
}
