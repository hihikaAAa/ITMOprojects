#ifndef PARSER_H
#define PARSER_H

#include "operation_types.h"
#include "operator.h"
#include "tokenisation.h"

// Структура, представляющая парсер.
// tkn - токены, полученные на этапе токенизации
// pos - текущая позиция в массиве токенов

typedef struct
{
	const Tokens *tkn;
	int pos;
} Parser;

// Создаёт парсер, инициализируя его токенами, и начальной позицией 0.

Parser createParser(const Tokens *lx);

// Парсит всё выражение целиком. Возвращает корень AST.
// Если в процессе парсинга возникла ошибка, устанавливает g_error_code.

OperationType *parseAll(Parser *p);

// Возвращает следующий токен (не удаляя его из потока).
Token peekToken(Parser *p);

/**
 * Возвращает следующий токен и увеличивает позицию в потоке на 1.
 */
Token getToken(Parser *p);

/**
 * Если следующий токен имеет тип type, то функция "съедает" (getToken)
 * и возвращает 1. Иначе возвращает 0.
 */
int match(Parser *p, int type);

/**
 * Аналогично match, но если токен не совпадает по типу, вызывает ошибку.
 */
void expect(Parser *p, int type);

/**
 * Ниже идут функции, которые последовательно парсят выражение
 * с учётом приоритетов операторов, используя метод рекурсивного спуска.
 */
OperationType *expression(Parser *p);
OperationType *parseBitOr(Parser *p);
OperationType *parseBitXor(Parser *p);
OperationType *parseBitAnd(Parser *p);
OperationType *parseShift(Parser *p);
OperationType *parseAddSub(Parser *p);
OperationType *parseMulDivMod(Parser *p);
OperationType *parsePow(Parser *p);
OperationType *parseUnary(Parser *p);
OperationType *parsePrimary(Parser *p);
OperationType *parseConcat(Parser *p);

#endif
