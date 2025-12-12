#include <TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "common_funcs.h"
#include "calculate_expression_funcs.h"
#include "tex_funcs.h"
#include "DSL_funcs.h"
#include "tree_optimization_funcs.h"
#include "double_compare_funcs.h"
#include "file_with_points_funcs.h"
#include "taylor_funcs.h"
#include "tangent_funcs.h"

Tree GetTangentTree (Tree* tree, double x_0)
{
    assert(tree);

    Tree derivate_tree = GetDerivateTree(tree);

    Variable* var_x = SetVarXAndReturn(tree, x_0);

    double y_0 = CalculateExpression(tree, tree->root);
    double incline_coeff = CalculateExpression(&derivate_tree,
                                               derivate_tree.root);

    PrintMessageInLaTex("Касание в точке A(%g, %g)", x_0, y_0);

    Tree tangent_tree = {};
    TreeCtor(&tangent_tree);
    tangent_tree.root = MakeTangentTree(&tangent_tree, var_x,
                                        x_0, y_0, incline_coeff);

    AddPointInFileWithPoints(x_0, y_0);

    return tangent_tree;
}

Node* MakeTangentTree(Tree* tree, Variable* var_x,
                      double x_0, double y_0, double incline_coeff)
{
    return ADD_
              ( MUL_
                   ( CNST_ ( incline_coeff ),
                     SUB_
                        ( VAR_ ( *var_x ),
                          CNST_ ( x_0 )
                        )
                    ),

                CNST_( y_0 )
              );
}
