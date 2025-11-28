#include <TXLib.h>
#include <math.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "read_wolfram_database.h"
#include "common_funcs.h"

StructOperation all_op[NUM_OF_OP] =
{
{"+",      ADD,     0,       BINARY,   ADD_func,    ADD_diff_func},
{"-",      SUB,     0,       BINARY,   SUB_func,    SUB_diff_func},
{"*",      MUL,     0,       BINARY,   MUL_func,    MUL_diff_func},
{"/",      DIV,     0,       BINARY,   DIV_func,    DIV_diff_func},
{"exp",    EXP,     0,     UNARY,    EXP_func,    EXP_diff_func},
{"pow",    POW,     0,     BINARY,   POW_func,    POW_diff_func},
{"ln",     LN,      0,      UNARY,    LN_func,     LN_diff_func},
{"log",    LOG,     0,     BINARY,   LOG_func,    LOG_diff_func},
{"cos",    COS,     0,     UNARY,    COS_func,    COS_diff_func},
{"sin",    SIN,     0,     UNARY,    SIN_func,    SIN_diff_func},
{"tg",     TG,      0,      UNARY,    TG_func,     TG_diff_func},
{"ctg",    CTG,     0,     UNARY,    CTG_func,    CTG_diff_func},
{"ch",     CH,      0,      UNARY,    CH_func,     CH_diff_func},
{"sh",     SH,      0,      UNARY,    SH_func,     SH_diff_func},
{"th",     TH,      0,      UNARY,    TH_func,     TH_diff_func},
{"cth",    CTH,     0,     UNARY,    CTH_func,    CTH_diff_func},
{"arcsin", ARCSIN,  0,  UNARY,    ARCSIN_func, ARCSIN_diff_func},
{"arccos", ARCCOS,  0,  UNARY,    ARCCOS_func, ARCCOS_diff_func},
{"arctg",  ARCTG,   0,   UNARY,    ARCTG_func,  ARCTG_diff_func},
{"arcctg", ARCCTG,  0,  UNARY,    ARCCTG_func, ARCCTG_diff_func},
};

void SetAllOpHash()
{
    for (size_t i = 0; i < NUM_OF_OP; ++i)
        all_op[i].hash = GetHash(all_op[i].name);
}

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

double TG_func(ArgsValue args_value)
{
    return tan(args_value.num1);
}

double CTG_func(ArgsValue args_value)
{
    return 1.0 / tan(args_value.num1);
}

double SH_func(ArgsValue args_value)
{
    return sinh(args_value.num1);
}

double CH_func(ArgsValue args_value)
{
    return cosh(args_value.num1);
}

double TH_func(ArgsValue args_value)
{
    return tanh(args_value.num1);
}

double CTH_func(ArgsValue args_value)
{
    return 1.0 / tanh(args_value.num1);
}

double ARCSIN_func(ArgsValue args_value)
{
    if (   IsDoubleBigger(args_value.num1, 1)
        || IsDoubleBigger(-1, args_value.num1))
    {
        fprintf(log_file, "ERROR arcsin\n\n");
        return NAN;
    }

    return asin(args_value.num1);
}

double ARCCOS_func(ArgsValue args_value)
{

    if (   IsDoubleBigger(args_value.num1, 1)
        && IsDoubleBigger(-1, args_value.num1))
    {
        fprintf(log_file, "ERROR arccos\n\n");
        return NAN;
    }

        return acos(args_value.num1);
}

double ARCTG_func(ArgsValue args_value)
{
    return atan(args_value.num1);
}

double ARCCTG_func(ArgsValue args_value)
{
    return 1.0 / atan(args_value.num1);
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

Node* TG_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return RIGHT_COMP_FUNC_ ( DIV_ ( CNST_ (1), POW_ ( COS_ ( cR ), CNST_(2) ) ) );
}

Node* CTG_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return RIGHT_COMP_FUNC_ ( DIV_ ( CNST_ (-1), POW_ ( SIN_ ( cR ), CNST_(2) ) ) );
}

Node* SH_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return RIGHT_COMP_FUNC_ ( CH_ ( cR ) );
}

Node* CH_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

   return RIGHT_COMP_FUNC_ ( SH_ ( cR ) );
}

Node* TH_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return RIGHT_COMP_FUNC_ ( DIV_ ( CNST_(1), POW_ ( CH_ ( cR ), CNST_ (2) ) ) );
}

Node* CTH_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

   return RIGHT_COMP_FUNC_ ( DIV_ ( CNST_(-1), POW_ ( SH_ ( cR ), CNST_ (2) ) ) );
}

Node* ARCSIN_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return RIGHT_COMP_FUNC_ ( DIV_ ( CNST_(1), POW_ ( SUB_ ( CNST_(1), POW_ (cR, CNST_ (2) ) ), CNST_ (0.5) ) ) );
}

Node* ARCCOS_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

   return RIGHT_COMP_FUNC_ ( DIV_ ( CNST_(-1), POW_ ( SUB_ ( CNST_(1), POW_ ( cR, CNST_ (2) ) ), CNST_ (0.5) ) ) );
}

Node* ARCTG_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return RIGHT_COMP_FUNC_ ( DIV_ ( CNST_(1), ADD_ ( CNST_(1), POW_ ( cR, CNST_ (2) ) ) ) );
}

Node* ARCCTG_diff_func(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    return RIGHT_COMP_FUNC_ ( DIV_ ( CNST_(-1), ADD_ ( CNST_(1), POW_ ( cR, CNST_ (2) ) ) ) );
}
