#ifndef CALCULATE_EXPRESSION_FUNCS
#define CALCULATE_EXPRESSION_FUNCS

#include "common_funcs.h"

void RequestVariableValue(Tree* tree);

double CalculateExpression(Tree* tree, Node* node);

double GetVariableValue(Tree* tree, char* var_name);

double ExecuteOperation(Tree* tree, Node* node);

double ExecuteUnaryOperation(Tree* tree, Node* node, OpFuncPtr func);

double ExecuteBinaryOperation(Tree* tree, Node* node, OpFuncPtr func);

void SetXValue(Tree* tree, double value);

#endif
