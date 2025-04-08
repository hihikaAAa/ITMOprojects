#ifndef OPERATOR_H
#define OPERATOR_H

#include "lib_usage.h"

//Различных типы токенов, токен - это наименьший элемент на C++, который имеет значение для компилятора
#define TOKEN_NUMBER 1	   // Токен числа
#define TOKEN_PLUS 2	   // Токен оператора сложения "+"
#define TOKEN_MINUS 3	   // Токен оператора вычитания "-"
#define TOKEN_MUL 4		   // Токен оператора умножения "*"
#define TOKEN_DIV 5		   // Токен оператора деления "/"
#define TOKEN_MOD 6		   // Токен оператора взятия остатка "%"
#define TOKEN_LSHIFT 7	   // Токен оператора сдвига влево "<<"
#define TOKEN_RSHIFT 8	   // Токен оператора сдвига вправо ">>"
#define TOKEN_BITAND 9	   // Токен побитового И "&"
#define TOKEN_BITOR 10	   // Токен побитового ИЛИ "|"
#define TOKEN_BITXOR 11	   // Токен побитового исключающего ИЛИ "^"
#define TOKEN_BITNOT 12	   // Токен побитового НЕ "~"
#define TOKEN_POW 13	   // Токен возведения в степень "**"
#define TOKEN_LPAREN 14	   // Токен открывающей скобки "("
#define TOKEN_RPAREN 15	   // Токен закрывающей скобки ")"
#define TOKEN_END 16	// Токен конца выражения
#define TOKEN_UNKNOWN 17	// Токен неизвестного/неподдерживаемого символа
#define TOKEN_CONCAT 18

// Типы узлов
#define NODE_NUMBER 1	 /// Узел-число
#define NODE_UNARY 2	 /// Узел унарного оператора
#define NODE_BINARY 3	 /// Узел бинарного оператора

// Типы унарных операторов
#define UOP_UPLUS 1		 /// Унарный плюс
#define UOP_UMINUS 2	 /// Унарный минус
#define UOP_UBITNOT 3	 /// Побитовое НЕ

// Типы бинарных операторов
#define BOP_ADD 1		 /// Бинарный оператор сложения (+)
#define BOP_SUB 2		 /// Бинарный оператор вычитания (-)
#define BOP_MUL 3		 /// Бинарный оператор умножения (*)
#define BOP_DIV 4		 /// Бинарный оператор деления (/)
#define BOP_MOD 5		 /// Бинарный оператор остатка (%)
#define BOP_LSHIFT 6	 /// Бинарный оператор сдвига влево (<<)
#define BOP_RSHIFT 7	 /// Бинарный оператор сдвига вправо (>>)
#define BOP_BITAND 8	 /// Бинарный оператор побитового И (&)
#define BOP_BITOR 9		 /// Бинарный оператор побитового ИЛИ (|)
#define BOP_BITXOR 10	 /// Бинарный оператор побитового исключающего ИЛИ (^)
#define BOP_POW 11		 /// Бинарный оператор возведения в степень (**)
#define BOP_CONCAT 12

// Коды ошибок, которые могут возникать во время работы программы.
#define ERROR_UNSUPPORTED_OPERATION 1	 /// Ошибка при использовании неподдерживаемой операции
#define ERROR_ARITHMETIC 2	  ///< Аритметическая ошибка
#define ERROR_PARSE 3	 /// Ошибка парсинга выражения
#define ERROR_OTHER 4	 /// Любая другая ошибка

/**
 * Глобальная переменная, в которой хранятся коды ошибок
 */
extern int g_error_code;

/**
 * Структура, описывающая отдельный токен.
 * type  — тип токена
 * value — числовое значение
 * raw   — строка, содержащая исходный текст токена
 */
typedef struct
{
	int type;		  /// Тип токена
	int value;		  /// Числовое значение (если это число), иначе 0
	char raw[128];	  /// Исходная текстовая форма токена
} Token;

/**
 * Функция, парсящая строку в целое число.
 * Поддерживает десятичные и шестнадцатеричные значения, а также знаки "+" и "-".
 * В случае ошибки устанавливает g_error_code и возвращает 0.
 */
int parseInt(const char *s);

#endif	  // OPERATOR_H
