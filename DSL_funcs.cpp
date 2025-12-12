#include <TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "double_compare_funcs.h"
#include "common_funcs.h"
#include "DSL_funcs.h"
#include "tree_optimization_funcs.h"

Node* NewOpNode(Operation op, Node* left, Node* right, Tree* tree)
{
    assert(tree);

    Node* new_node = (Node*)calloc(1, sizeof(Node));
    assert(new_node);

    new_node->left = left;
    new_node->right = right;

    if (left) left->parent = new_node;
    if (right) right->parent = new_node;

    if (left == NULL || right == NULL)
        new_node->args = UNARY;
    else
        new_node->args = BINARY;

    new_node->type = OP;
    new_node->value.op = op;

    tree->size++;

    return new_node;
}

Node* NewNumNode(double num, Node* left, Node* right, Tree* tree)
{
    assert(tree);

    Node* new_node = (Node*)calloc(1, sizeof(Node));
    assert(new_node);

    new_node->left = left;
    new_node->right = right;

    if (left) left->parent = new_node;
    if (right) right->parent = new_node;

    new_node->type = NUM;
    new_node->value.num = num;

    tree->size++;

    return new_node;
}

Node* NewVarNode(Variable var, Node* left, Node* right, Tree* tree)
{
    assert(tree);

    Node* new_node = (Node*)calloc(1, sizeof(Node));
    assert(new_node);

    new_node->left = left;
    new_node->right = right;

    if (left) left->parent = new_node;
    if (right) right->parent = new_node;

    new_node->type = VAR;
    new_node->value.var.var_name = var.var_name;
    new_node->value.var.var_hash = var.var_hash;
    new_node->value.var.var_data = var.var_data;

    tree->size++;

    return new_node;
}

Node* CopySubtree(Tree* tree, Node* node)
{
    assert(tree);

    if (node == NULL)
        return NULL;

    switch(node->type)
    {
        case OP:
            return NewOpNode(node->value.op, CopySubtree(tree, node->left),
                                             CopySubtree(tree, node->right),
                                             tree);

        case NUM:
            return NewNumNode(node->value.num, CopySubtree(tree, node->left),
                                               CopySubtree(tree, node->right),
                                               tree);

        case VAR:
            return NewVarNode(node->value.var, CopySubtree(tree, node->left),
                                               CopySubtree(tree, node->right),
                                               tree);

        default: break;
    }

    return NULL;
}

void SonNeutralOptimize(Tree* tree, Node* node,
                        Node* son,  double el, bool* status)
{
    assert(tree);

    if (node != NULL && *status && son!= NULL && son->type == NUM
        && IsDoubleEqual(son->value.num, el))
    {                                                                                                           \
        if (son == node->right ) OptimizeNeutralElement(node->left, tree);
        if (son == node->left ) OptimizeNeutralElement(node->right, tree);
        RemoveNeutralElement(tree, node, son);
        *status = false;
    }
}

void AbsorbOptimize(Tree* tree, Node* node,
                    Node* son, double el, double new_el, bool* status)
{
    assert(tree);

    if (node != NULL && *status && son != NULL && son->type == NUM
            && IsDoubleEqual(son->value.num, el))
    {
        DeleteNode(tree, node);
        node->type = NUM;
        node->value.num = new_el;
        *status = false;
    }
}

