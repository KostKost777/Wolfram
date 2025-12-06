#ifndef DSL_FUNCS
#define DSL_FUNCS

#include "derivate_tree_funcs.h"

#define dL               DifferentiateTree(tree, node->left)
#define dR               DifferentiateTree(tree, node->right)
#define cL               CopySubtree(tree, node->left)
#define cR               CopySubtree(tree, node->right)

#define VAR_(var)        NewVarNode(var, NULL, NULL, tree)
#define CNST_(var)       NewNumNode(var, NULL, NULL, tree)
#define ADD_(var1, var2) NewOpNode(ADD, var1, var2, tree)
#define SUB_(var1, var2) NewOpNode(SUB, var1, var2, tree)
#define MUL_(var1, var2) NewOpNode(MUL, var1, var2, tree)
#define DIV_(var1, var2) NewOpNode(DIV, var1, var2, tree)
#define POW_(var1, var2) NewOpNode(POW, var1, var2, tree)
#define EXP_(var)        NewOpNode(EXP, NULL, var, tree)
#define LN_(var)         NewOpNode(LN, NULL, var, tree)
#define LOG_(var1, var2) NewOpNode(LOG, var1, var2, tree)
#define COS_(var)        NewOpNode(COS, NULL, var, tree)
#define SIN_(var)        NewOpNode(SIN, NULL, var, tree)
#define TG_(var)         NewOpNode(TG, NULL, var, tree)
#define CTG_(var)        NewOpNode(CTG, NULL, var, tree)
#define SH_(var)         NewOpNode(SH, NULL, var, tree)
#define CH_(var)         NewOpNode(CH, NULL, var, tree)
#define TH_(var)         NewOpNode(TH, NULL, var, tree)
#define CTH_(var)        NewOpNode(CTH, NULL, var, tree)
#define ARCCOS_(var)     NewOpNode(ARCCOS, NULL, var, tree)
#define ARCSIN_(var)     NewOpNode(ARCSIN, NULL, var, tree)
#define ARCTG_(var)      NewOpNode(ARCTG, NULL, var, tree)
#define ARCCTG_(var)     NewOpNode(ARCCTG, NULL, var, tree)
#define FACT_(var)       NewOpNode(FACT, NULL, var, tree)

#define DUMP_(node)      node

#define RIGHT_COMP_FUNC_(var) NewOpNode(MUL, var, dR, tree)
#define LEFT_COMP_FUNC_(var) NewOpNode(MUL, var, dL, tree)

Node* NewOpNode(Operation op, Node* left, Node* right, Tree* tree);

Node* NewNumNode(double num, Node* left, Node* right, Tree* tree);

Node* NewVarNode(Variable var, Node* left, Node* right, Tree* tree);

Node* CopySubtree(Tree* tree, Node* node);

#endif
