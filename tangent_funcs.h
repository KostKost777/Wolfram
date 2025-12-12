#ifndef TANGET_FUNCS
#define TANGET_FUNCS

Tree GetTangentTree (Tree* tree, double x_0);

Node* MakeTangentTree(Tree* tree, Variable* var_x,
                      double x_0, double y_0, double incline_coeff);

#endif
