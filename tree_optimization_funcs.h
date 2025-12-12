#ifndef TREE_OPTIMIZATION_FUNCS
#define TREE_OPTIMIZATION_FUNCS

void OptimizeTree(Tree* tree);

void OptimizeNeutralElement(Node* node, Tree* tree);

double OptimizeConstants(Node* node, Tree* tree);

void Optimize_ADD_NeutralElement(Node* node, Tree* tree);
void Optimize_MUL_NeutralElement(Node* node, Tree* tree);
void Optimize_SUB_NeutralElement(Node* node, Tree* tree);
void Optimize_DIV_NeutralElement(Node* node, Tree* tree);
void Optimize_POW_EXP_NeutralElement(Node* node, Tree* tree);
void Optimize_LOG_LN_NeutralElement(Node* node, Tree* tree);

void ExecuteOperationWithConstants(Node* node, Tree* tree);

void ExecuteUnaryOperationWithConstants(Node* node, Tree* tree,
                                        OpFuncPtr func);

void ExecuteBinaryOperationWithConstants(Node* node, Tree* tree,
                                         OpFuncPtr func);

void RemoveNeutralElement(Tree* tree, Node* node_parent,
                          Node* neutral_node);

#endif
