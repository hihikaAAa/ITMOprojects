#include "../include/constructor.h"
#include "../include/lib_usage.h"
#include "../include/operation_types.h"
#include "../include/operator.h"
#include "../include/parser.h"
#include "../include/tokenisation.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: program \"expression\"\n");
		if (g_error_code == 0)
			g_error_code = ERROR_OTHER;
		return g_error_code;
	}

	//Структура для токенов
	Tokens lexer;
	initTokens(&lexer, argv[1]);	// Инициализация
	tokenize(&lexer);				//Токенизация
	if (g_error_code != 0)
	{
		return g_error_code;
	}

	//Парсер
	Parser parser = createParser(&lexer);
	// Парщу всё выражение, получаю корень дерева
	OperationType* root = parseAll(&parser);
	if (root == NULL && g_error_code == 0)
	{
		// Если root == NULL, но код ошибки не выставлен
		g_error_code = ERROR_PARSE;
	}
	if (g_error_code != 0)
	{
		// Если возникла ошибка, освобождаю память
		if (root)
			freeOperationType(root);
		return g_error_code;
	}

	// Вычисляю результат, обойдя дерево
	int result = constructOperationType(root);
	if (g_error_code == 0)
	{
		// Если не было ошибок, вывожу результат
		printf("%d\n", result);
	}

	// Освобождаем дерево
	freeOperationType(root);
	return g_error_code;
}
