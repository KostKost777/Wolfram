#include <TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "read_wolfram_database.h"
#include "common_funcs.h"

StructOperation all_op[NUM_OF_OP] =
{
{"+",      ADD,     GetHash("+"),       BINARY,   ADD_func, ADD_diff_func},
{"-",      SUB,     GetHash("-"),       BINARY,   SUB_func, SUB_diff_func},
{"*",      MUL,     GetHash("*"),       BINARY,   MUL_func, MUL_diff_func},
{"/",      DIV,     GetHash("/"),       BINARY,   DIV_func, DIV_diff_func},
{"exp",    EXP,     GetHash("exp"),     UNARY,    EXP_func, EXP_diff_func},
{"pow",    POW,     GetHash("pow"),     BINARY,   POW_func, POW_diff_func},
{"ln",     LN,      GetHash("ln"),      UNARY,    LN_func,  LN_diff_func},
{"log",    LOG,     GetHash("log"),     BINARY,   LOG_func, LOG_diff_func},
{"cos",    COS,     GetHash("cos"),     UNARY,    COS_func, COS_diff_func},
{"sin",    SIN,     GetHash("sin"),     UNARY,    SIN_func, SIN_diff_func},
{"tg",     TG,      GetHash("tg"),      UNARY,    TG_func,     SIN_diff_func},
{"ctg",    CTG,     GetHash("ctg"),     UNARY,    CTG_func,    SIN_diff_func},
{"ch",     CH,      GetHash("ch"),      UNARY,    CH_func,     SIN_diff_func},
{"sh",     SH,      GetHash("sh"),      UNARY,    SH_func,     SIN_diff_func},
{"arcsin", ARCSIN,  GetHash("arcsin"),  UNARY,    ARCSIN_func, SIN_diff_func},
{"arccos", ARCCOS,  GetHash("arccos"),  UNARY,    ARCCOS_func, SIN_diff_func},
{"arctg",  ARCTG,   GetHash("arctg"),   UNARY,    ARCTG_func,  SIN_diff_func},
{"arcctg", ARCCTG,  GetHash("arcctg"),  UNARY,    ARCCTG_func, SIN_diff_func},
};

double ADD_func(ArgsValue args_value)
{
    return args_value.num1 + args_value.num2;
}

double SUB_func(ArgsValue args_value)
{
    return args_value.num1 - args_value.num2;
}

double MUL_func(ArgsValue args_value)
{
    return args_value.num1 * args_value.num2;
}

double DIV_func(ArgsValue args_value)
{

    if (!IsDoubleEqual(args_value.num2, 0))
        return args_value.num1 / args_value.num2;
    else
        fprintf(log_file, "Деление на ноль\n");

    return NAN;
}

double EXP_func(ArgsValue args_value)
{
    return exp(args_value.num1);
}

double POW_func(ArgsValue args_value)
{
    return pow(args_value.num1, args_value.num2);
}

double LN_func(ArgsValue args_value)
{

    return log(args_value.num1);
}

double LOG_func(ArgsValue args_value)
{

    if (   signbit(args_value.num1)
        || signbit(args_value.num2)
        || IsDoubleEqual(args_value.num1, 1))
    {
        fprintf(log_file, "Ошибка в подсчете логарифма");
        return NAN;
    }

    return log(args_value.num1) / log(args_value.num2);
}

double COS_func(ArgsValue args_value)
{
    return cos(args_value.num1);
}

double SIN_func(ArgsValue args_value)
{
    return sin(args_value.num1);
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
        return LEFT_COMP_FUNC_ ( MUL_ ( cR, ( POW_ ( cL, SUB_ ( cR, CNST_ (1) ) ) ) ) );
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

    return RIGHT_COMP_FUNC_ ( DIV_ ( CNST_ (1), cR));
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
        return LEFT_COMP_FUNC_ ( MUL_  ( CNST_ (-1), DIV_ ( LN_ ( cR ), MUL_ ( cL, POW_ ( LN_ ( cL ), CNST_ (2) ) ) ) ) );
    }

    else if (!IsVarInTree(node->left) && IsVarInTree(node->right))
    {
        return RIGHT_COMP_FUNC_ ( DIV_ ( CNST_ (1), MUL_ ( cR, LN_ (cL) ) ) );
    }

    return CNST_ (0);
}

Node* SIN_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return RIGHT_COMP_FUNC_ ( COS_ ( cR ) );
}

Node* COS_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return RIGHT_COMP_FUNC_ ( MUL_ ( CNST_ (-1), SIN_ ( cR ) ) );
}


