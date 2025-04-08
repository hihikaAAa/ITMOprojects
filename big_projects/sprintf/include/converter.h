#ifndef CONVERTER_H
#define CONVERTER_H

#include "int128.h"
#include "lib_usage.h"

void converter(int128_t value, int base, char *str, int uppercase);

#endif	  // CONVERTER_H
//Конвертирует большое число (int128_t) в строку в системе счисления base (от 2 до 16) c поддержкой строчных и загл.букв.
// для систем счисления
