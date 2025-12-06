#include <TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "common_funcs.h"
#include "calculate_expression_funcs.h"

void RequestVariableValue(Tree* tree)
{
    assert(tree);

    for (size_t i = 0; i < tree->var->size; ++i)
    {
        printf("Введите значение переменной \"%s\": ",
                tree->var->arr[i].var_name);

        scanf("%lf", &tree->var->arr[i].var_data);
    }
}

double CalculateExpression(Tree* tree, Node* node)
{
    assert(tree);
    assert(node);

    switch(node->type)
    {
        case NUM:
            return node->value.num;

        case VAR:
            return GetVariableValue(tree, node->value.var.var_name);

        case OP:
            return ExecuteOperation(tree, node);

        default: break;
    }

    return NAN;
}

double ExecuteOperation(Tree* tree, Node* node)
{
    assert(tree);
    assert(node);

    StructOperation* now_op = GetStructOperationOfNode(node);

    switch(now_op->args)
    {
        case UNARY:
            return ExecuteUnaryOperation(tree, node, now_op->op_func);

        case BINARY:
            return ExecuteBinaryOperation(tree, node, now_op->op_func);

        default:
            return NAN;
    }

    return NAN;
}

double ExecuteUnaryOperation(Tree* tree, Node* node, OpFuncPtr func)
{
    assert(tree);
    assert(node);

    ArgsValue args_value = {};

    double right_res = CalculateExpression(tree, node->right);

    args_value.num1 = right_res;

    return func(args_value);
}

double ExecuteBinaryOperation(Tree* tree, Node* node, OpFuncPtr func)
{
    assert(tree);
    assert(node);

    ArgsValue args_value = {};

    double left_res = CalculateExpression(tree, node->left);

    double right_res = CalculateExpression(tree, node->right);

    args_value.num1 = left_res;
    args_value.num2 = right_res;

    return func(args_value);
}

double GetVariableValue(Tree* tree, char* var_name)
{
    assert(tree);
    assert(var_name);

    for (size_t i = 0; i < tree->var->size; ++i)
    {
        if (strcmp(var_name, tree->var->arr[i].var_name) == 0)
            return tree->var->arr[i].var_data;
    }

    fprintf(log_file, "Я не знаю такой переменной |%s|\n\n", var_name);
    return 0;
}
