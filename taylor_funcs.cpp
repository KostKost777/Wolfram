#include <TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "common_funcs.h"
#include "calculate_expression_funcs.h"
#include "tex_funcs.h"
#include "DSL_funcs.h"
#include "tree_optimization_funcs.h"
#include "taylor_funcs.h"

Tree GetTaylorTree(Tree* tree, double x_point, size_t accuracy)
{
    assert(tree);

    Variable* var_x = SetNeighborhoodInVarXAndReturn(tree, x_point);

    Tree taylor_tree = {};
    TreeCtor(&taylor_tree);
    taylor_tree.var = tree->var;

    Tree derivate_tree = {};
    TreeCtor(&derivate_tree);
    derivate_tree.var = tree->var;
    derivate_tree.root = CopySubtree(tree, tree->root);

    double* taylor_coeffs = (double*)calloc(accuracy + 1, sizeof(double));

    for (size_t i = 0; i <= accuracy; ++i)
    {
        taylor_coeffs[i] = CalculateExpression(&derivate_tree,
                                              derivate_tree.root);

        PrintMessageInLaTex("%d-ая производная: ", i + 1);

        Tree new_derivate_tree = GetDerivateTree(&derivate_tree);

        TreeDtor(&derivate_tree);
        derivate_tree = new_derivate_tree;
    }

    TreeDtor(&derivate_tree);

    size_t member = 0;
    taylor_tree.root = MakeTaylorTree(&taylor_tree, taylor_coeffs,
                                      member, accuracy, var_x);

    TreeDump(&taylor_tree);

    OptimizeTree(&taylor_tree);

    PrintTaylor(taylor_coeffs, x_point, accuracy);

    return taylor_tree;
}

Node* MakeTaylorTree(Tree* tree, double* taylor_coeffs,
                     size_t member, size_t accuracy, Variable* var_x)
{
    assert(tree);
    assert(taylor_coeffs);
    assert(var_x);

    if (member > accuracy)
        return CNST_ (0);

    Node* taylor_member =  MUL_
                              ( DIV_
                                   ( CNST_ ( taylor_coeffs[member] ),
                                     FACT_ ( CNST_ ( (double)member ) ) ),

                                 POW_
                                    ( SUB_
                                         ( VAR_ (*var_x),
                                           CNST_ (var_x->var_data) ),
                                      CNST_ ( (double)member) ) );

    return ADD_ ( taylor_member,
                    MakeTaylorTree(tree, taylor_coeffs,
                                    member + 1, accuracy, var_x));
}

Variable* SetNeighborhoodInVarXAndReturn(Tree* tree, double value)
{
    size_t hash_x = GetHash("x");

    for (size_t i = 0; i < tree->var->size; ++i)
    {
        if (tree->var->arr[i].var_hash == hash_x)
        {
            tree->var->arr[i].var_data = value;
            return &tree->var->arr[i];
        }
    }

    //unreachebale
    assert(false);
    return NULL;
}
