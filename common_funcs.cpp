#include <TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "read_wolfram_database.h"
#include "common_funcs.h"

StructOperation all_op[NUM_OF_OP] =
{
{"ADD", ADD, GetHash("ADD"), ADD_func, ADD_diff_func},
{"SUB", SUB, GetHash("SUB"), SUB_func, SUB_diff_func},
{"MUL", MUL, GetHash("MUL"), MUL_func, MUL_diff_func},
{"DIV", DIV, GetHash("DIV"), DIV_func, DIV_diff_func}
};

double ADD_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);
    assert(node->left);
    assert(node->right);

    double left_result = CalculateExpression(tree, node->left);
    double right_result = CalculateExpression(tree, node->right);

    return left_result + right_result;
}

double SUB_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);
    assert(node->left);
    assert(node->right);

    double left_result = CalculateExpression(tree, node->left);
    double right_result = CalculateExpression(tree, node->right);

    return left_result - right_result;
}

double MUL_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);
    assert(node->left);
    assert(node->right);

    double left_result = CalculateExpression(tree, node->left);
    double right_result = CalculateExpression(tree, node->right);

    return left_result * right_result;
}

double DIV_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);
    assert(node->left);
    assert(node->right);

    double left_result = CalculateExpression(tree, node->left);
    double right_result = CalculateExpression(tree, node->right);

    if (!IsDoubleEqual(right_result, 0))
        return left_result / right_result;
    else
        fprintf(log_file, "Деление на ноль\n");

    return NAN;
}

//DIFF_FUNCS

Node* ADD_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return ADD_ ( dL, dR );
}

Node* SUB_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return SUB_ ( dL, dR );
}

Node* MUL_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return ADD_ ( MUL_ ( dL, cR ), MUL_ ( cL, dR ) );
}

Node* DIV_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return DIV_ ( SUB_ ( MUL_ ( dL, cR ), MUL_ ( cL, dR ) ), MUL_ ( cR, cR ) );
}
