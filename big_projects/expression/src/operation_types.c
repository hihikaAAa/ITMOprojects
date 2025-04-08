#include "../include/operation_types.h"

#include "../include/lib_usage.h"

/**
 * Создаёт узел для числа (NODE_NUMBER).
 */
OperationType* createNumberNode(int val)
{
	OperationType* n = malloc(sizeof(OperationType));
	n->nodeType = NODE_NUMBER;
	n->value = val;
	n->uop = 0;
	n->bop = 0;
	n->left = 0;
	n->right = 0;
	return n;
}

/**
 * Создаёт узел унарного оператора (NODE_UNARY).
 */
OperationType* createUnaryNode(int op, OperationType* child)
{
	OperationType* n = malloc(sizeof(OperationType));
	n->nodeType = NODE_UNARY;
	n->value = 0;
	n->uop = op;
	n->bop = 0;
	n->left = child;
	n->right = 0;
	return n;
}

/**
 * Создаёт узел бинарного оператора (NODE_BINARY).
 */
OperationType* createBinaryNode(int op, OperationType* left, OperationType* right)
{
	OperationType* n = malloc(sizeof(OperationType));
	n->nodeType = NODE_BINARY;
	n->value = 0;
	n->uop = 0;
	n->bop = op;
	n->left = left;
	n->right = right;
	return n;
}

/**
 * Рекурсивно освобождает узлы дерева.
 */
void freeOperationType(OperationType* root)
{
	if (!root)
		return;
	freeOperationType(root->left);
	freeOperationType(root->right);
	free(root);
}
