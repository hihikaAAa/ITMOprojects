#include "../include/tokenisation.h"

#include "../include/lib_usage.h"
#include "../include/operator.h"

extern int g_error_code;

/**
 * Инициализируем структуру Tokens, копируя входную строку.
 */
void initTokens(Tokens *t, const char *input)
{
	strncpy(t->input, input, 1023);
	t->input[1023] = '\0';
	t->tokenCount = 0;
}

/**
 * Вспомогательная функция для добавления токена в массив.
 */
void addToken(Tokens *t, int type, const char *raw, int value)
{
	if (t->tokenCount >= MAX_TOKENS)
	{
		fprintf(stderr, "Too many tokens\n");
		if (g_error_code == 0)
			g_error_code = ERROR_PARSE;
		return;
	}
	t->tokens[t->tokenCount].type = type;
	t->tokens[t->tokenCount].value = value;
	strncpy(t->tokens[t->tokenCount].raw, raw, 127);
	t->tokens[t->tokenCount].raw[127] = '\0';
	t->tokenCount++;
}

/**
 * Разбиваю строку t->input на токены:
 *  1. Пропускаю пробелы.
 *  2. Проверяю двухсимвольные операторы (<<, >>, **).
 *  3. Парщу числа, учитывая возможный знак '+' или '-'.
 *  4. Определяю остальные операторы.
 *  5. В конце добавляю TOKEN_END.
 */
int tokenize(Tokens *t)
{
    int i = 0;
    int length = (int)strlen(t->input);

    while (i < length)
    {
        while (i < length && isspace((unsigned char)t->input[i]))
            i++;
        if (i >= length)
            break;

        char c = t->input[i];

        if (c == '(')
        {
            addToken(t, TOKEN_LPAREN, "(", 0);
            i++;
            continue;
        }
        else if (c == ')')
        {
            addToken(t, TOKEN_RPAREN, ")", 0);
            i++;
            continue;
        }

        if (i + 3 < length && strncmp(&t->input[i], "conc", 4) == 0)
        {
            addToken(t, TOKEN_CONCAT, "conc", 0);
            i += 4;
            continue;
        }

        if (i + 1 < length)
        {
            char twoChars[3];
            twoChars[0] = t->input[i];
            twoChars[1] = t->input[i + 1];
            twoChars[2] = '\0';

            if (strcmp(twoChars, "**") == 0)
            {
                addToken(t, TOKEN_POW, "**", 0);
                i += 2;
                continue;
            }
            else if (strcmp(twoChars, "<<") == 0)
            {
                addToken(t, TOKEN_LSHIFT, "<<", 0);
                i += 2;
                continue;
            }
            else if (strcmp(twoChars, ">>") == 0)
            {
                addToken(t, TOKEN_RSHIFT, ">>", 0);
                i += 2;
                continue;
            }
        }

        if (isdigit((unsigned char)c))
        {
            int start = i;
            while (i < length && isdigit((unsigned char)t->input[i]))
                i++;
            int len = i - start;
            if (len > 127) len = 127;
            char numStr[128];
            memcpy(numStr, t->input + start, len);
            numStr[len] = '\0';

            int val = parseInt(numStr);
            addToken(t, TOKEN_NUMBER, numStr, val);
            continue;
        }

        if (c == '+')
        {
            addToken(t, TOKEN_PLUS, "+", 0);
            i++;
            continue;
        }
        else if (c == '-')
        {
            addToken(t, TOKEN_MINUS, "-", 0);
            i++;
            continue;
        }
        else if (c == '~')
        {
            addToken(t, TOKEN_BITNOT, "~", 0);
            i++;
            continue;
        }
        else if (c == '*')
        {
            addToken(t, TOKEN_MUL, "*", 0);
            i++;
            continue;
        }
        else if (c == '/')
        {
            addToken(t, TOKEN_DIV, "/", 0);
            i++;
            continue;
        }
        else if (c == '%')
        {
            addToken(t, TOKEN_MOD, "%", 0);
            i++;
            continue;
        }
        else if (c == '&')
        {
            addToken(t, TOKEN_BITAND, "&", 0);
            i++;
            continue;
        }
        else if (c == '|')
        {
            addToken(t, TOKEN_BITOR, "|", 0);
            i++;
            continue;
        }
        else if (c == '^')
        {
            addToken(t, TOKEN_BITXOR, "^", 0);
            i++;
            continue;
        }

        fprintf(stderr, "Unknown token: %c\n", c);
        if (g_error_code == 0)
            g_error_code = ERROR_UNSUPPORTED_OPERATION;
        return t->tokenCount;
    }

    addToken(t, TOKEN_END, "", 0);
    return t->tokenCount;
}


/**
 * Возвращает количество токенов.
 */
int getCount(const Tokens *t)
{
	return t->tokenCount;
}

/**
 * Безопасно возвращает указатель на токен по индексу.
 */
const Token *getTokenPtr(const Tokens *t, int index)
{
	if (index < 0 || index >= t->tokenCount)
	{
		fprintf(stderr, "Lexer index out of range\n");
		if (g_error_code == 0)
			g_error_code = ERROR_OTHER;
		// Возвращаем указатель на самый первый токен, чтобы избежать NULL
		return &t->tokens[0];
	}
	return &t->tokens[index];
}
