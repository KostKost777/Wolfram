#include <TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "common_funcs.h"
#include "tree_optimization_funcs.h"
#include "double_compare_funcs.h"
#include "tex_funcs.h"
#include "DSL_funcs.h"

void OptimizeTree(Tree* tree)
{
    size_t old_size = 0;

    while(old_size != tree->size)
    {
       old_size = tree->size;

       OptimizeConstants(tree->root, tree);
       OptimizeNeutralElement(tree->root, tree);

    //    if (old_size != tree->size)
    //    {
    //         PrintMessageInLaTex("Немного упростим: ");
    //         TreeDump(tree);
    //    }
    }
}

void OptimizeNeutralElement(Node* node, Tree* tree)
{
    assert(tree);

    if (node == NULL)
        return;

    if (node->type == OP)
    {
        switch(node->value.op)
        {
            case ADD: Optimize_ADD_NeutralElement(node, tree);
                      break;

            case SUB: Optimize_SUB_NeutralElement(node, tree);
                      break;

            case MUL: Optimize_MUL_NeutralElement(node, tree);
                      break;

            case DIV: Optimize_DIV_NeutralElement(node, tree);
                      break;

            case POW:
            case EXP: Optimize_POW_EXP_NeutralElement(node, tree);
                      break;

            case LN:
            case LOG: Optimize_LOG_LN_NeutralElement(node, tree);
                      break;

            case SIN:
            case COS:
            case TG:
            case CTG:
            case TH:
            case CTH:
            case ARCSIN:
            case ARCCOS:
            case ARCTG:
            case ARCCTG:
            case SH:
            case CH:
            case FACT:
            default: break;
        }
    }

    if (node != NULL)
        OptimizeNeutralElement(node->left, tree);

    if (node != NULL)
        OptimizeNeutralElement(node->right, tree);
}

void Optimize_ADD_NeutralElement(Node* node, Tree* tree)
{
    assert(tree);
    assert(node);
    bool status = true;

    SonNeutralOptimize(tree, node, node->right,  0, &status);
    SonNeutralOptimize(tree, node, node->left,  0, &status);
}

void Optimize_SUB_NeutralElement(Node* node, Tree* tree)
{
    assert(tree);
    assert(node);
    bool status = true;

    SonNeutralOptimize(tree, node, node->right,  0, &status);
}

void Optimize_MUL_NeutralElement(Node* node, Tree* tree)
{
    assert(tree);
    assert(node);
    bool status = true;

    SonNeutralOptimize(tree, node, node->right,  1, &status);
    SonNeutralOptimize(tree, node, node->left,  1, &status);

    AbsorbOptimize(tree, node, node->right, 0, 0, &status);
    AbsorbOptimize(tree, node, node->left, 0, 0, &status);
}

void Optimize_DIV_NeutralElement(Node* node, Tree* tree)
{
    assert(tree);
    assert(node);
    bool status = true;

    SonNeutralOptimize(tree, node, node->right,  1,&status);

    AbsorbOptimize(tree, node, node->left, 0, 0, &status);
}

void Optimize_POW_EXP_NeutralElement(Node* node, Tree* tree)
{
    assert(tree);
    assert(node);
    bool status = true;

    SonNeutralOptimize(tree, node, node->right,  1, &status);

    AbsorbOptimize(tree, node, node->right, 0, 1, &status);
}

void Optimize_LOG_LN_NeutralElement(Node* node, Tree* tree)
{
    assert(tree);
    assert(node);
    bool status = true;

    AbsorbOptimize(tree, node, node->right, 1, 0, &status);
}

double OptimizeConstants(Node* node, Tree* tree)
{
    assert(tree);
    assert(node);

    switch(node->type)
    {
        case NUM:
            return node->value.num;

        case VAR:
            return NAN;

        case OP:
            ExecuteOperationWithConstants(node, tree);

        default: break;
    }

    return NAN;
}

void ExecuteOperationWithConstants(Node* node, Tree* tree)
{
    assert(tree);
    assert(node);

    StructOperation* now_op = GetStructOperationOfNode(node);

    switch(now_op->args)
    {
        case UNARY:
            ExecuteUnaryOperationWithConstants(node, tree, now_op->op_func);
            break;

        case BINARY:
            ExecuteBinaryOperationWithConstants(node, tree, now_op->op_func);
            break;

        default: break;
    }
}

void ExecuteUnaryOperationWithConstants(Node* node, Tree* tree,
                                        OpFuncPtr func)
{
    assert(node);
    assert(tree);

    double right_res = OptimizeConstants(node->right, tree);

    if (isnan(right_res))
        return;

    free(node->right);
    node->right = NULL;

    tree->size -= 1;
    node->type = NUM;

    ArgsValue args_value = {right_res, 0};

    node->value.num = func(args_value);
}

void ExecuteBinaryOperationWithConstants(Node* node, Tree* tree,
                                         OpFuncPtr func)
{
    assert(node);
    assert(tree);

    double left_res = OptimizeConstants(node->left, tree);

    double right_res = OptimizeConstants(node->right, tree);

    if (isnan(left_res) || isnan(right_res))
        return;

    free(node->left);
    free(node->right);

    node->left = NULL;
    node->right = NULL;

    tree->size -= 2;

    ArgsValue args_value = {left_res, right_res};

    node->type = NUM;
    node->value.num = func(args_value);
}

void RemoveNeutralElement(Tree* tree, Node* node_parent, Node* neutral_node)
{
    assert(node_parent);
    assert(neutral_node);

    Node* node_second_son = NULL;

    if (neutral_node == node_parent->right)
        node_second_son = node_parent->left;
    else
        node_second_son = node_parent->right;

    if (node_parent->parent == NULL)
    {
        tree->root = node_second_son;
        node_second_son->parent = NULL;
    }

    else if (node_parent->parent->left == node_parent)
    {
        node_parent->parent->left = node_second_son;
        node_second_son->parent = node_parent->parent;
    }

    else
    {
        node_parent->parent->right = node_second_son;
        node_second_son->parent = node_parent->parent;
    }

    free(neutral_node);
    neutral_node = NULL;

    free(node_parent);
    node_parent = NULL;

    tree->size -= 2;
}
