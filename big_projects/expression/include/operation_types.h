#ifndef OPERATION_TYPES_H
#define OPERATION_TYPES_H

#include "operator.h"

// Структура, описывающая узел дерева операций.
typedef struct OperationTypeNode
{
	int nodeType;	 // Тип узла (NODE_NUMBER, NODE_UNARY, NODE_BINARY)
	int value;		 // Значение, если это числовой узел (NODE_NUMBER)
	int uop;		 // Тип унарного оператора (UOP_UPLUS, UOP_UMINUS, UOP_UBITNOT)
	int bop;		 // Тип бинарного оператора (BOP_ADD, BOP_SUB и т.д.)
	struct OperationTypeNode* left;		// Указатель на левый дочерний узел
	struct OperationTypeNode* right;	// Указатель на правый дочерний узел
} OperationType;

/**
 * Создаёт узел, представляющий число (NODE_NUMBER).
 */
OperationType* createNumberNode(int val);

/**
 * Создаёт узел унарного оператора (NODE_UNARY).
 * op - унарная операция (UOP_UPLUS, UOP_UMINUS, UOP_UBITNOT).
 * child - узел, над которым будет применяться унарный оператор.
 */
OperationType* createUnaryNode(int op, OperationType* child);

/**
 * Создаёт узел бинарного оператора (NODE_BINARY).
 * op - бинарная операция (BOP_ADD, BOP_SUB и т.д.).
 * left - левый узел.
 * right - правый узел.
 */
OperationType* createBinaryNode(int op, OperationType* left, OperationType* right);

/**
 * Рекурсивно освобождает память, занимаемую деревом операций (AST).
 */
void freeOperationType(OperationType* root);

#endif	  // OPERATION_TYPES_H
