#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H

#include "operation_types.h"
#include "operator.h"

// Функция, которая обходит дерево операций и вычисляет
// итоговое значение выражения. Возвращает целое число - результат.
// Параметр root указывает на корень дерева операций.

int constructOperationType(OperationType* root);

#endif	  // CONSTRUCTOR_H
