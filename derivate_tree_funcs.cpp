#include <TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "common_funcs.h"
#include "derivate_tree_funcs.h"
#include "tree_optimization_funcs.h"
#include "double_compare_funcs.h"
#include "DSL_funcs.h"
#include "tex_funcs.h"

Tree GetDerivateTree(Tree* tree)
{
    assert(tree);

    Tree derivate_tree = {};
    TreeCtor(&derivate_tree);
    derivate_tree.var = tree->var;

    derivate_tree.root = DifferentiateTree(&derivate_tree, tree->root);

    TreeDump(&derivate_tree);

    OptimizeTree(&derivate_tree);

    return derivate_tree;
}

Node* DifferentiateTree(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    switch(node->type)
    {
        case NUM:
            return CNST_ (0);

        case VAR:
            return CNST_ (1);

        case OP:
            return GetDerivateOfOperation(tree, node);

        default: break;
    }

    return NULL;
}

Node* GetDerivateOfOperation(Tree* tree, Node* node)
{
    for (size_t i = 0; i < NUM_OF_OP; ++i)
    {
        if (all_op[i].op == node->value.op)
            return all_op[i].diff_op_func(tree, node);
    }

    //unreachable
    assert(false);
    return NULL;
}
