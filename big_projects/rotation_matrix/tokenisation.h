#ifndef TOKENISATION_H
#define TOKENISATION_H

#include "operator.h"

#define MAX_TOKENS 1024

/**
 * Структура, представляющая результат токенизации.
 * input      - исходная строка с выражением.
 * tokens     - массив токенов, полученных из строки.
 * tokenCount - количество токенов.
 */
typedef struct
{
	char input[1024];
	Token tokens[MAX_TOKENS];
	int tokenCount;
} Tokens;

/**
 * Инициализирует структуру Tokens, копируя входную строку в поле input.
 */
void initTokens(Tokens *t, const char *input);

/**
 * Функция, которая анализирует строку t->input и разбивает её на токены,
 * добавляя их в t->tokens и увеличивая t->tokenCount. Возвращает общее
 * количество токенов (или меньше, если возникла ошибка).
 */
int tokenize(Tokens *t);

/**
 * Возвращает количество токенов.
 */
int getCount(const Tokens *t);

/**
 * Возвращает указатель на токен по индексу (для внутреннего использования парсером).
 */
const Token *getTokenPtr(const Tokens *t, int index);

/**
 * Вспомогательная функция для добавления токена в массив t->tokens.
 * type  - тип токена (TOKEN_...).
 * raw   - строковое представление токена.
 * value - числовое значение, если это число, иначе 0.
 */
void addToken(Tokens *t, int type, const char *raw, int value);

#endif	  // TOKENISATION_H
