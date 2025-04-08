#include "../include/constructor.h"
extern int g_error_code;	// Глобальный код ошибки

/**
 * Функция для безопасного деления:
 * - Проверяет деление на ноль.
 * - Проверяет переполнение (INT_MIN / -1).
 * - В случае ошибки выставляет g_error_code и возвращает 0.
 */
int safeDiv(int a, int b)
{
	if (b == 0)
	{
		fprintf(stderr, "Division by zero\n");
		if (g_error_code == 0)
			g_error_code = ERROR_ARITHMETIC;
		return 0;
	}
	if (a == INT_MIN && b == -1)
	{
		fprintf(stderr, "Integer overflow in division\n");
		if (g_error_code == 0)
			g_error_code = ERROR_ARITHMETIC;
		return 0;
	}
	return a / b;
}

/**
 * Функция для безопасного остатка (a % b):
 * Аналогично проверяет деление на ноль и переполнение.
 */
int safeMod(int a, int b)
{
	if (b == 0)
	{
		fprintf(stderr, "Division (mod) by zero\n");
		if (g_error_code == 0)
			g_error_code = ERROR_ARITHMETIC;
		return 0;
	}
	if (a == INT_MIN && b == -1)
	{
		fprintf(stderr, "Integer overflow in module\n");
		if (g_error_code == 0)
			g_error_code = ERROR_ARITHMETIC;
		return 0;
	}
	return a % b;
}

/**
 * Безопасный сдвиг влево:
 * - Проверяет, что количество сдвигов в диапазоне [0..31].
 */
int safeLShift(int a, int b)
{
	if (b < 0 || b >= 32)
	{
		fprintf(stderr, "Shift out of range\n");
		if (g_error_code == 0)
			g_error_code = ERROR_ARITHMETIC;
		return 0;
	}
	// Сдвиг влево
	return (int)((unsigned int)a << b);
}

/**
 * Безопасный сдвиг вправо:
 */
int safeRShift(int a, int b)
{
	if (b < 0 || b >= 32)
	{
		fprintf(stderr, "Shift out of range\n");
		if (g_error_code == 0)
			g_error_code = ERROR_ARITHMETIC;
		return 0;
	}
	return a >> b;
}

/**
 * Безопасная операция возведения в степень:
 * - Проверяет, что показатель степени неотрицательный.
 * - Возвращает 0 при отрицательном показателе.
 * - Использует бинарное возведение в степень.
 */
int safePow(int base, int exp)
{
	if (exp < 0)
	{
		fprintf(stderr, "Negative exponent\n");
		if (g_error_code == 0)
			g_error_code = ERROR_ARITHMETIC;
		return 0;
	}
	long result = 1;
	long b = base;
	int e = exp;
	while (e > 0)
	{
		// Если текущий бит экспоненты равен 1, умножаем result на b
		if ((e & 1) != 0)
		{
			result = result * b;
		}
		b = b * b;
		e = e >> 1;
	}
	return (int)result;
}
int safeConcat(int leftVal, int rightVal)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%d%d", leftVal, rightVal);

    long long temp = atoll(buf);
    if (temp < INT_MIN || temp > INT_MAX)
    {
        fprintf(stderr, "Concat result out of 32-bit range\n");
        if (g_error_code == 0)
            g_error_code = ERROR_ARITHMETIC;
        return 0;
    }
    return (int)temp;
}

/**
 * Вспомогательная рекурсивная функция:
 * - Если узел - число, вернуть его value.
 * - Если унарный оператор, рекурсивно вычислить левый узел, применить унарную операцию.
 * - Если бинарный, рекурсивно вычислить левый и правый узлы, применить бинарную операцию.
 */
int constructorNode(OperationType* node)
{
	// Если нет узла или уже выставлен код ошибки, возвращаем 0.
	if (!node || g_error_code != 0)
		return 0;

	// Узел - число
	if (node->nodeType == NODE_NUMBER)
	{
		return node->value;
	}
	// Узел - унарный оператор
	else if (node->nodeType == NODE_UNARY)
	{
		int val = constructorNode(node->left);	  // Рекурсивно вычисляем левый (единственный) узел
		if (node->uop == UOP_UPLUS)
		{
			// Унарный плюс ничего не меняет
			return val;
		}
		else if (node->uop == UOP_UMINUS)
		{
			// Унарный минус
			return -val;
		}
		else if (node->uop == UOP_UBITNOT)
		{
			// Побитовое НЕ
			return ~val;
		}
		else
		{
			fprintf(stderr, "Unsupported unary operation\n");
			if (g_error_code == 0)
				g_error_code = ERROR_UNSUPPORTED_OPERATION;
			return 0;
		}
	}
	// Узел - бинарный оператор
	else if (node->nodeType == NODE_BINARY)
	{
		int leftVal = constructorNode(node->left);
		int rightVal = constructorNode(node->right);
		// Если при вычислении дочерних узлов возникла ошибка, прерываемся
		if (g_error_code != 0)
			return 0;

		// Применяем операцию к leftVal и rightVal
		switch (node->bop)
		{
		case BOP_ADD:
			return leftVal + rightVal;
		case BOP_SUB:
			return leftVal - rightVal;
		case BOP_MUL:
			return leftVal * rightVal;
		case BOP_DIV:
			return safeDiv(leftVal, rightVal);
		case BOP_MOD:
			return safeMod(leftVal, rightVal);
		case BOP_LSHIFT:
			return safeLShift(leftVal, rightVal);
		case BOP_RSHIFT:
			return safeRShift(leftVal, rightVal);
		case BOP_BITAND:
			return leftVal & rightVal;
		case BOP_BITOR:
			return leftVal | rightVal;
		case BOP_BITXOR:
			return leftVal ^ rightVal;
		case BOP_POW:
			return safePow(leftVal, rightVal);
		case BOP_CONCAT:
			return safeConcat(leftVal, rightVal);
		default:
			fprintf(stderr, "Unsupported binary operation\n");
			if (g_error_code == 0)
				g_error_code = ERROR_UNSUPPORTED_OPERATION;
			return 0;
		}
	}
	else
	{
		// Если тип узла неизвестен, выставляем ошибку
		fprintf(stderr, "Unknown node type\n");
		if (g_error_code == 0)
			g_error_code = ERROR_OTHER;
		return 0;
	}
}

/**
 * Основная функция, объявленная в заголовке constructor.h.
 * Рекурсивно вычисляет значение корня дерева, используя constructorNode().
 */
int constructOperationType(OperationType* root)
{
	int val = constructorNode(root);
	return val;
}
