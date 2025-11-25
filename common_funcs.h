#ifndef WOLFRAM_OP_FUNCS
#define WOLFRAM_OP_FUNCS

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "read_wolfram_database.h"

typedef double (*op_func_ptr)(Tree* tree, Node* node);
typedef Node* (*diff_op_func_ptr)(Tree* tree, Node* node);

struct StructOperation {
    const char* name;
    Operation op;
    size_t hash;
    const op_func_ptr op_func;
    const diff_op_func_ptr diff_op_func;
};

extern StructOperation all_op[NUM_OF_OP];

//OP_FUNCS

double ADD_func(Tree* tree, Node* node);

double SUB_func(Tree* tree, Node* node);

double MUL_func(Tree* tree, Node* node);

double DIV_func(Tree* tree, Node* node);

//DIFF_FUNCS

Node* ADD_diff_func(Tree* tree, Node* node);

Node* SUB_diff_func(Tree* tree, Node* node);

Node* MUL_diff_func(Tree* tree, Node* node);

Node* DIV_diff_func(Tree* tree, Node* node);

#endif
