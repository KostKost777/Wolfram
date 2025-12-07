#ifndef TAYLOR_FUNCS
#define TAYLOR_FUNCS

Tree GetTaylorTree (Tree* tree, double x_point, size_t accuracy);

Node* MakeTaylorTree (Tree* tree, double* taylor_coeffs,
                     size_t member, size_t accuracy, Variable* var_x);

Variable* SetNeighborhoodInVarXAndReturn(Tree* tree, double value);

#endif
