#ifndef WOLFRAM_OP_FUNCS
#define WOLFRAM_OP_FUNCS

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "read_wolfram_database.h"

typedef Node* (*diff_op_func_ptr)(Tree* tree, Node* node);

enum ArgType
{
    UNARY = 1,
    BINARY = 2,
};

struct ArgsValue
{
    double num1;
    double num2;
};

typedef double (*op_func_ptr)(ArgsValue);

struct StructOperation
{
    const char* name;
    Operation op;
    size_t hash;
    ArgType args;
    const op_func_ptr op_func;
    const diff_op_func_ptr diff_op_func;
};

extern StructOperation all_op[NUM_OF_OP];

//OP_FUNCS

double ADD_func(ArgsValue args_value);

double SUB_func(ArgsValue args_value);

double MUL_func(ArgsValue args_value);

double DIV_func(ArgsValue args_value);

double EXP_func(ArgsValue args_value);

double POW_func(ArgsValue args_value);

double LN_func(ArgsValue args_value);

double LOG_func(ArgsValue args_value);

double COS_func(ArgsValue args_value);

double SIN_func(ArgsValue args_value);

double TG_func(ArgsValue args_value);

double CTG_func(ArgsValue args_value);

double CH_func(ArgsValue args_value);

double SH_func(ArgsValue args_value);

double TH_func(ArgsValue args_value);

double CTH_func(ArgsValue args_value);

double ARCSIN_func(ArgsValue args_value);

double ARCCOS_func(ArgsValue args_value);

double ARCTG_func(ArgsValue args_value);

double ARCCTG_func(ArgsValue args_value);

//DIFF_FUNCS

Node* ADD_diff_func(Tree* tree, Node* node);

Node* SUB_diff_func(Tree* tree, Node* node);

Node* MUL_diff_func(Tree* tree, Node* node);

Node* DIV_diff_func(Tree* tree, Node* node);

Node* EXP_diff_func(Tree* tree, Node* node);

Node* POW_diff_func(Tree* tree, Node* node);

Node* LN_diff_func(Tree* tree, Node* node);

Node* LOG_diff_func(Tree* tree, Node* node);

Node* COS_diff_func(Tree* tree, Node* node);

Node* SIN_diff_func(Tree* tree, Node* node);

Node* TG_diff_func(Tree* tree, Node* node);

Node* CTG_diff_func(Tree* tree, Node* node);

Node* CH_diff_func(Tree* tree, Node* node);

Node* SH_diff_func(Tree* tree, Node* node);

Node* TH_diff_func(Tree* tree, Node* node);

Node* CTH_diff_func(Tree* tree, Node* node);

Node* ARCSIN_diff_func(Tree* tree, Node* node);

Node* ARCCOS_diff_func(Tree* tree, Node* node);

Node* ARCTG_diff_func(Tree* tree, Node* node);

Node* ARCCTG_diff_func(Tree* tree, Node* node);

#endif
