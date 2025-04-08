#ifndef PRINT_H
#define PRINT_H
#include <png.h>

/**
 * Выводит сообщение об ошибке в stderr и завершает программу.
 */
void print_error_and_exit(const char *message);

#endif  // PRINT_H
