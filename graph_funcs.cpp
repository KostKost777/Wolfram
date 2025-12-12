#include <TXLib.h>

#include "wolfram_funcs.h"
#include "calculate_expression_funcs.h"
#include "graph_funcs.h"
#include "tex_funcs.h"
#include "double_compare_funcs.h"
#include "file_with_points_funcs.h"

void FillFileWIthPointForGraph(const char* file_name,
                               Tree* tree, double x_min, double x_max)
{
    FILE* file_with_points = fopen(file_name, "w");

    double division = (x_max - x_min) / 1000;

    for(double i = x_min; i < x_max; i += division)
    {
        SetXValue(tree, i);
        double res = CalculateExpression(tree, tree->root);

        fprintf(file_with_points, "%lf %lf\n", i, res);
    }

    fclose(file_with_points);
}

void MakeGraph(char* func1, char* func2, char* func3, Scale* scale)
{
    const char* name_of_graph_file = "graph.png";
    FILE *gnuplot = popen("gnuplot", "w");

    fprintf(gnuplot, "set terminal png size 800,600\n");
    fprintf(gnuplot, "set output '%s'\n", name_of_graph_file);

    fprintf(gnuplot, "set title 'График функции'\n");
    fprintf(gnuplot, "set grid\n");
    fprintf(gnuplot, "set key top right\n");
    fprintf(gnuplot, "set samples 100000\n");
    fprintf(gnuplot, "set xlabel 'x'\n");
    fprintf(gnuplot, "set ylabel 'y'\n");

    if (   IsDoubleEqual(scale->x_min, 0)
        && IsDoubleEqual(scale->x_max, 0)
        && IsDoubleEqual(scale->y_min, 0)
        && IsDoubleEqual(scale->y_max, 0))
    {
        fprintf(gnuplot, "set autoscale\n");
    }

    else
    {
        fprintf(gnuplot, "set xrange [%lf:%lf]\n", scale->x_min, scale->x_max);
        fprintf(gnuplot, "set yrange [%lf:%lf]\n", scale->y_min, scale->y_max);
    }

    fprintf(gnuplot, "plot %s lw 2 title 'Исходная функция', \\\n", func1);
    fprintf(gnuplot, "%s lw 2 title 'Тейлор', \\\n", func2);
    fprintf(gnuplot, "%s lw 2 title 'Касательная', \\\n", func3);
    fprintf(gnuplot, "\"%s\" using 1:2 with points pt 7 ps 1 lc \"red\" notitle \n", points_file_name);


    fflush(gnuplot);
    pclose(gnuplot);

    PrintPictureInLatex(name_of_graph_file);
}

void SetStringOfExpression(Tree* tree, char* str_expression)
{
    assert(tree);

    FILE* expression_buffer = fopen("expression_buffer.txt", "w");
    GetNodeString(tree->root, expression_buffer);
    fclose(expression_buffer);

    expression_buffer = fopen("expression_buffer.txt", "r");
    fgets(str_expression, MAX_LEN_OF_EXPRESSION, expression_buffer);
    fclose(expression_buffer);
    str_expression[strlen(str_expression)] = '\0';
}

void GetNodeString(Node* node, FILE* expression_buffer)
{
    assert(expression_buffer);
    assert(node);

    switch(node->type)
    {
        case NUM: PrintExpression_NUM(expression_buffer, node->value.num);
                  break;

        case VAR:
            fprintf(expression_buffer, "%s", node->value.var.var_name);
            break;

        case OP:
            switch(node->value.op)
            {
                case SUB:
                case ADD: PrintExpression_ADD_SUB(node, expression_buffer);
                          break;

                case MUL: PrintExpression_MUL(node, expression_buffer);
                          break;

                case DIV: PrintExpression_DIV(node, expression_buffer);
                          break;

                case POW: PrintExpression_POW(node, expression_buffer);
                          break;

                case EXP: PrintExpression_EXP(node, expression_buffer);
                          break;

                case LN: PrintOpByNameInExpression(node, expression_buffer, "log");
                         break;

                case LOG: PrintExpression_LOG(node, expression_buffer);
                          break;

                case SIN: PrintOpByNameInExpression(node, expression_buffer, "sin");
                          break;

                case COS: PrintOpByNameInExpression(node, expression_buffer, "cos");
                          break;

                case TG: PrintOpByNameInExpression(node, expression_buffer, "tan");
                         break;

                case CTG: PrintOpByNameInExpression(node, expression_buffer, "cot");
                          break;

                case SH: PrintOpByNameInExpression(node, expression_buffer, "sinh");
                         break;

                case CH: PrintOpByNameInExpression(node, expression_buffer, "cosh");
                         break;

                case TH: PrintOpByNameInExpression(node, expression_buffer, "tanh");
                         break;

                case CTH: PrintOpByNameInExpression(node, expression_buffer, "coth");
                          break;

                case ARCSIN: PrintOpByNameInExpression(node, expression_buffer, "arcsin");
                             break;

                case ARCCOS: PrintOpByNameInExpression(node, expression_buffer, "arccos");
                             break;

                case ARCTG: PrintOpByNameInExpression(node, expression_buffer, "arctan");
                            break;

                case ARCCTG: PrintOpByNameInExpression(node, expression_buffer, "arccot");
                             break;

                case FACT: PrintExpression_FACT(node, expression_buffer);
                           break;

                default: break;
            }

        default: break;
    }
}

void PrintExpression_ADD_SUB(Node* node, FILE* expression_buffer)
{
    assert(node);
    assert(expression_buffer);

    char sign = '\0';
    if (node->value.op == ADD) sign = '+';
    else sign = '-';

    if (   node->parent != NULL
        && node->parent->value.op == MUL)
    {
        fprintf(expression_buffer, "(");
       GetNodeString(node->left, expression_buffer);
        fprintf(expression_buffer, " %c ", sign);
       GetNodeString(node->right, expression_buffer);
        fprintf(expression_buffer, ")");
        return;
    }

   GetNodeString(node->left, expression_buffer);
    fprintf(expression_buffer, " %c ", sign);
   GetNodeString(node->right, expression_buffer);
}

void PrintExpression_MUL(Node* node, FILE* expression_buffer)
{
    assert(node);
    assert(expression_buffer);

   GetNodeString(node->left, expression_buffer);
    fprintf(expression_buffer, " * ");
   GetNodeString(node->right, expression_buffer);
}

void PrintExpression_DIV(Node* node, FILE* expression_buffer)
{
    assert(node);
    assert(expression_buffer);

    fprintf(expression_buffer, "( ");
   GetNodeString(node->left, expression_buffer);
    fprintf(expression_buffer, " )");

    fprintf(expression_buffer, " / ");

    fprintf(expression_buffer, "( ");
   GetNodeString(node->right, expression_buffer);
    fprintf(expression_buffer, " )");
}

void PrintExpression_POW(Node* node, FILE* expression_buffer)
{
    assert(node);
    assert(expression_buffer);

    if (   node->left->value.op == ADD
        || node->left->value.op == SUB)
    {
        fprintf(expression_buffer, "( ");
       GetNodeString(node->left, expression_buffer);
        fprintf(expression_buffer, " )");
    }

    else
       GetNodeString(node->left, expression_buffer);

    fprintf(expression_buffer, " ** ( ");
   GetNodeString(node->right, expression_buffer);
    fprintf(expression_buffer, " )");
}

void PrintExpression_EXP(Node* node, FILE* expression_buffer)
{
    assert(node);
    assert(expression_buffer);

    fprintf(expression_buffer, "exp( ");
   GetNodeString(node->right, expression_buffer);
    fprintf(expression_buffer, " )");
}

void PrintOpByNameInExpression(Node* node, FILE* expression_buffer, const char* name)
{
    assert(node);
    assert(expression_buffer);

    fprintf(expression_buffer, "%s (", name);
   GetNodeString(node->right, expression_buffer);
    fprintf(expression_buffer, " )");
}

void PrintExpression_FACT(Node* node, FILE* expression_buffer)
{
    assert(node);
    assert(expression_buffer);

   GetNodeString(node->right, expression_buffer);
    fprintf(expression_buffer, "!");
}

void PrintExpression_LOG(Node* node, FILE* expression_buffer)
{
    assert(node);
    assert(expression_buffer);

    fprintf(expression_buffer, "log( ");
   GetNodeString(node->right, expression_buffer);
    fprintf(expression_buffer, " )");

    fprintf(expression_buffer, " / ");

    fprintf(expression_buffer, "log( ");
   GetNodeString(node->left, expression_buffer);
    fprintf(expression_buffer, " )");
}

void PrintExpression_NUM(FILE* expression_buffer, double num)
{
    assert(expression_buffer);

    if (num < 0)
        fprintf(expression_buffer, "(%g)", num);
    else
        fprintf(expression_buffer, "%g", num);
}
