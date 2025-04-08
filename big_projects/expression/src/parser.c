#include "../include/parser.h"

#include "../include/lib_usage.h"
#include "../include/operation_types.h"
#include "../include/operator.h"
#include "../include/tokenisation.h"

extern int g_error_code;

/**
 * Создаёт парсер, инициализируя его массивом токенов и позицией 0.
 */
Parser createParser(const Tokens* lx)
{
	Parser p;
	p.tkn = lx;
	p.pos = 0;
	return p;
}

/**
 * Функция для безопасного получения токена по индексу,
 * чтобы избежать выхода за границы массива токенов.
 */
const Token* safeGetTokenPtr(const Parser* p, int index)
{
	const Token* tk = getTokenPtr(p->tkn, index);
	return tk;
}

/**
 * Возвращает текущий токен (не удаляя его из потока).
 */
Token peekToken(Parser* p)
{
	return *safeGetTokenPtr(p, p->pos);
}

/**
 * Возвращает текущий токен и сдвигает позицию парсера вперёд.
 */
Token getToken(Parser* p)
{
	Token t = peekToken(p);
	// Если токен не TOKEN_END, то двигаемся вперёд
	if (t.type != TOKEN_END)
	{
		p->pos++;
	}
	else
	{
		// TOKEN_END может встречаться несколько раз при переполнении
		int count = getCount(p->tkn);
		if (p->pos < count)
			p->pos++;
	}
	return t;
}

/**
 * Если следующий токен имеет тип type, то "съедаю" его и возвращаю 1.
 * Иначе ничего не делаю и возвращаю 0.
 */
int match(Parser* p, int type)
{
	if (peekToken(p).type == type)
	{
		getToken(p);
		return 1;
	}
	return 0;
}

/**
 * Если следующий токен имеет тип type, "съедаю" его.
 */
void expect(Parser* p, int type)
{
	if (!match(p, type))
	{
		fprintf(stderr, "Expected token not found\n");
		if (g_error_code == 0)
			g_error_code = ERROR_PARSE;
	}
}

/**
 * Парсит всё выражение до конца. Проверяет отсутствие лишних токенов.
 */
OperationType* parseAll(Parser* p)
{
	// Парщу выражение
	OperationType* root = expression(p);
	if (g_error_code != 0)
		return root;

	// Проверяю, что в конце (TOKEN_END)
	if (peekToken(p).type != TOKEN_END)
	{
		fprintf(stderr, "Extra tokens after expression\n");
		if (g_error_code == 0)
			g_error_code = ERROR_PARSE;
	}
	return root;
}

/**
 * Самая верхняя функция.
 */
OperationType* expression(Parser* p)
{
	return parseBitOr(p);
}
OperationType* parseConcat(Parser* p)
{
    OperationType* left = parsePrimary(p);

    while (g_error_code == 0 && peekToken(p).type == TOKEN_CONCAT)
    {
        getToken(p);

        OperationType* right = parsePrimary(p);

        left = createBinaryNode(BOP_CONCAT, left, right);
    }
    return left;
}

/**
 * Парсим операции '|'.
 * Приоритет (самый нижний): expr -> parseBitOr -> parseBitXor -> ...
 */
OperationType* parseBitOr(Parser* p)
{
	OperationType* left = parseBitXor(p);
	while (g_error_code == 0 && peekToken(p).type == TOKEN_BITOR)
	{
		getToken(p);	// пропускаем оператор '|'
		OperationType* right = parseBitXor(p);
		left = createBinaryNode(BOP_BITOR, left, right);
	}
	return left;
}

/**
 * Парсим операции '^'.
 */
OperationType* parseBitXor(Parser* p)
{
	OperationType* left = parseBitAnd(p);
	while (g_error_code == 0 && peekToken(p).type == TOKEN_BITXOR)
	{
		getToken(p);
		OperationType* right = parseBitAnd(p);
		left = createBinaryNode(BOP_BITXOR, left, right);
	}
	return left;
}

/**
 * Парсим операции '&'.
 */
OperationType* parseBitAnd(Parser* p)
{
	OperationType* left = parseShift(p);
	while (g_error_code == 0 && peekToken(p).type == TOKEN_BITAND)
	{
		getToken(p);
		OperationType* right = parseShift(p);
		left = createBinaryNode(BOP_BITAND, left, right);
	}
	return left;
}

/**
 * Парсим сдвиги << и >>.
 */
OperationType* parseShift(Parser* p)
{
	OperationType* left = parseAddSub(p);
	while (g_error_code == 0)
	{
		int t = peekToken(p).type;
		if (t == TOKEN_LSHIFT)
		{
			getToken(p);
			OperationType* right = parseAddSub(p);
			left = createBinaryNode(BOP_LSHIFT, left, right);
		}
		else if (t == TOKEN_RSHIFT)
		{
			getToken(p);
			OperationType* right = parseAddSub(p);
			left = createBinaryNode(BOP_RSHIFT, left, right);
		}
		else
		{
			break;
		}
	}
	return left;
}

/**
 * Парсим сложение и вычитание '+' '-'.
 */
OperationType* parseAddSub(Parser* p)
{
	OperationType* left = parseMulDivMod(p);
	while (g_error_code == 0)
	{
		int t = peekToken(p).type;
		if (t == TOKEN_PLUS)
		{
			getToken(p);
			OperationType* right = parseMulDivMod(p);
			left = createBinaryNode(BOP_ADD, left, right);
		}
		else if (t == TOKEN_MINUS)
		{
			getToken(p);
			OperationType* right = parseMulDivMod(p);
			left = createBinaryNode(BOP_SUB, left, right);
		}
		else
		{
			break;
		}
	}
	return left;
}

/**
 * Парсим умножение, деление и взятие остатка '*', '/', '%'.
 */
OperationType* parseMulDivMod(Parser* p)
{
	OperationType* left = parsePow(p);
	while (g_error_code == 0)
	{
		int t = peekToken(p).type;
		if (t == TOKEN_MUL)
		{
			getToken(p);
			OperationType* right = parsePow(p);
			left = createBinaryNode(BOP_MUL, left, right);
		}
		else if (t == TOKEN_DIV)
		{
			getToken(p);
			OperationType* right = parsePow(p);
			left = createBinaryNode(BOP_DIV, left, right);
		}
		else if (t == TOKEN_MOD)
		{
			getToken(p);
			OperationType* right = parsePow(p);
			left = createBinaryNode(BOP_MOD, left, right);
		}
		else
		{
			break;
		}
	}
	return left;
}

/**
 * Парсим возведение в степень '**'.
 * Реализовано рекурсией: left = parseUnary, затем
 * while TOKEN_POW -> right = parsePow(p) (а не parseUnary).
 */
OperationType* parsePow(Parser* p)
{
	OperationType* left = parseUnary(p);
	while (g_error_code == 0 && peekToken(p).type == TOKEN_POW)
	{
		getToken(p);
		OperationType* right = parseUnary(p);
		left = createBinaryNode(BOP_POW, left, right);
	}
	return left;
}

/**
 * Парсим унарные операции: +, -, ~
 */
OperationType* parseUnary(Parser* p)
{
	int t = peekToken(p).type;
	if (t == TOKEN_PLUS || t == TOKEN_MINUS || t == TOKEN_BITNOT)
	{
		getToken(p);
		int next = peekToken(p).type;
		// Запрещаем множественные унарные операторы подряд
		if (next == TOKEN_PLUS || next == TOKEN_MINUS || next == TOKEN_BITNOT)
		{
			fprintf(stderr, "Multiple unary operators are not allowed\n");
			if (g_error_code == 0)
				g_error_code = ERROR_PARSE;
			return NULL;
		}
		// Для оператора +, -, ~ парсим операнд как степеня (parsePow) —
		// во многих языках унарный +, - имеют приоритет более высокий, чем **
		OperationType* child = parseUnary(p);
		if (g_error_code != 0)
			return NULL;
		if (t == TOKEN_PLUS)
			return createUnaryNode(UOP_UPLUS, child);
		else if (t == TOKEN_MINUS)
			return createUnaryNode(UOP_UMINUS, child);
		else if (t == TOKEN_BITNOT)
			return createUnaryNode(UOP_UBITNOT, child);
	}
	else
	{
		return parseConcat(p);
	}
}

/**
 * Парсим "первичные" выражения: число или скобки.
 */
OperationType* parsePrimary(Parser* p)
{
	Token tt = peekToken(p);
	if (tt.type == TOKEN_NUMBER)
	{
		getToken(p);
		return createNumberNode(tt.value);
	}
	else if (tt.type == TOKEN_LPAREN)
	{
		getToken(p);
		OperationType* expr = expression(p);
		if (!match(p, TOKEN_RPAREN))
		{
			// Если не встретили закрывающую скобку, выставляем ошибку
			freeOperationType(expr);
			fprintf(stderr, "Missing closing parenthesis\n");
			if (g_error_code == 0)
				g_error_code = ERROR_PARSE;
			return NULL;
		}
		return expr;
	}
	else
	{
		fprintf(stderr, "Unexpected token in primary\n");
		if (g_error_code == 0)
			g_error_code = ERROR_PARSE;
		return NULL;
	}
}
