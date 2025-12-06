#ifndef DERIVATE_TREE_FUNCS
#define DERIVATE_TREE_FUNCS

Tree GetDerivateTree(Tree* tree);

Node* DifferentiateTree(Tree* tree, Node* node);

Node* GetDerivateOfOperation(Tree* tree, Node* node);

#endif
