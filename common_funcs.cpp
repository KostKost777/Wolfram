#include <TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "read_wolfram_database.h"
#include "common_funcs.h"

StructOperation all_op[NUM_OF_OP] =
{
{"+",   ADD, GetHash("+"),   BINARY,   ADD_func, ADD_diff_func},
{"-",   SUB, GetHash("-"),   BINARY,   SUB_func, SUB_diff_func},
{"*",   MUL, GetHash("*"),   BINARY,   MUL_func, MUL_diff_func},
{"/",   DIV, GetHash("/"),   BINARY,   DIV_func, DIV_diff_func},
{"exp", EXP, GetHash("exp"), UNARY,  EXP_func, EXP_diff_func},
{"pow", POW, GetHash("pow"), BINARY, POW_func, POV_diff_func},
{"ln",  LN,  GetHash("ln"),  UNARY,  LN_func,  LN_diff_func},
{"log", LOG, GetHash("log"), BINARY, LOG_func, LOG_diff_func},
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

double POW_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);
    assert(node->left);
    assert(node->right);

    double left_result = CalculateExpression(tree, node->left);
    double right_result = CalculateExpression(tree, node->right);

    return pow(left_result, right_result);
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

double EXP_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);
    assert(node->right);

    double right_result = CalculateExpression(tree, node->right);

    return exp(right_result);
}

double POW_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);
    assert(node->right);
    assert(node->left)

    double right_result = CalculateExpression(tree, node->right);
    double left_result = CalculateExpression(tree, node->left);

    return pow(left_result, right_result);
}

double LN_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);
    assert(node->right);

    double right_result = CalculateExpression(tree, node->right);

    if (signbit(right_result))
    {
        fprintf(log_file, "Ошибка в подсчете натурального логарифма");
        return NAN;
    }

    return log(right_result);
}

double LOG_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);
    assert(node->right);
    assert(node->left)

    double right_result = CalculateExpression(tree, node->right);
    double left_result = CalculateExpression(tree, node->left);

    if (   signbit(right_result)
        || signbit(left_result)
        || IsDoubleEqual(left_result, 1))
    {
        fprintf(log_file, "Ошибка в подсчете логарифма");
        return NAN;
    }

    return log(right_result) / log(left_result);
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

Node* POW_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    if (IsVarInTree(node->left) && IsVarInTree(node->right))
    {
        return MUL_ ( POW_ ( cL, cR ), ADD_ ( DIV_ (MUL_ ( dL , cR ), cL ), MUL_ ( dR, LN_ ( cL ) ) ) );
    }

    else if (IsVarInTree(node->left) && !IsVarInTree(node->right))
    {
        return LEFT_COMP_FUNC_ ( MUL_ ( cR, ( POW_ ( cL, SUB_ ( cR, 1 ) ) ) ) );
    }

    else if (!IsVarInTree(node->left) && IsVarInTree(node->right))
    {
        return RIGHT_COMP_FUNC_ ( MUL_ ( LN_ ( cL ), POW_ ( cL, cR) ) );
    }

    return NewNumNode(0, NULL, NULL, tree);
}

Node* EXP_diff_func(Tree* tree, Node* node)
{
    assert(tree);
    assert(node);

    return RIGHT_COMP_FUNC_( EXP_ ( cR ) );
}

Node* LN_diff_func(Tree* tree, Node* node)
{
    assert(tree);
    assert(node);

    return RIGHT_COMP_FUNC_ ( DIV_ ( CNST_ ( 1 ), cR));
}

Node* LOG_diff_func(Tree* tree, Node* node)
{
    assert(tree);
    assert(node);

    if (IsVarInTree(node->left) && IsVarInTree(node->right))
    {
        return  DIV_ ( SUB_ ( MUL_ ( DIV_ (dR, cR), LN_ ( cL ) ),  MUL_ ( DIV_ (dL, cL), LN_ ( cR ) ) ), POW_ ( LN_ ( cL ), CNST_ (2) ) );
    }

    else if (IsVarInTree(node->left) && !IsVarInTree(node->right))
    {
        return LEFT_COMP_FUNC_ MUL_ ( ( CNST_ (-1), DIV_ ( LN_ ( cR ), MUL_ ( cL, POW_ ( LN_ ( cL ), CNST_ (2) ) ) ) ) );
    }

    else if (!IsVarInTree(node->left) && IsVarInTree(node->right))
    {
        return RIGHT_COMP_FUNC_ ( DIV_ ( CNST_ (1), MUL_ ( cR, LN_ (cL) ) ) );
    }

    return CNST_ (0);
}


