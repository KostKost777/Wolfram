#include<TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "new_read_wolfram_database.h"
#include "common_funcs.h"
#include "tex_funcs.h"
#include "calculate_expression_funcs.h"
#include "double_compare_funcs.h"
#include "derivate_tree_funcs.h"
#include "taylor_funcs.h"
#include "graph_funcs.h"

const char* database_file_name = "database2.txt";

int main(int argc, const char* argv[])
{
    atexit(CloseLogFile);
    atexit(CloseLaTex);
    OpenLogFile();
    LatexInit();

    if (argc > 1)
        database_file_name = argv[1];

    Buffer buffer = {};
    GetDataBaseFromFile(&buffer, database_file_name);

    Tree tree = {};
    TreeCtor(&tree);
    VariableArrInit(&tree);
    Scale scale = {};
    ParseTree(&tree, &scale, buffer.data);

    //printf("BUFFER: %s\n", buffer.data);

    PrintMessageInLaTex("Было введено следующие выражение: ");

    TreeDump(&tree);

    RequestVariableValue(&tree);
    double ans = CalculateExpression(&tree, tree.root);
    printf("Result: %lf\n", ans);

    Tree derivate_tree = {};
    TreeCtor(&derivate_tree);

    PrintMessageInLaTex("Найдем первую производную: ");

    derivate_tree = GetDerivateTree(&tree);

    PrintMessageInLaTex("Первая производная: ");
    TreeDump(&derivate_tree);

    TreeDtor(&derivate_tree);

    double x_point = 0;

    printf("Enter a point in the neighborhood "
           "of which you want to obtain the Taylor expansion: ");
    scanf("%lf", &x_point);

    size_t accuracy = 0;
    printf("Enter the accuracy of the expansion: ");
    scanf("%llu", &accuracy);

    PrintMessageInLaTex("Найдем разложение по Тейлору: ");
    Tree taylor_tree = GetTaylorTree(&tree, x_point, accuracy);

    char str_taylor_tree[MAX_LEN_OF_EXPRESSION] = {};
    SetStringOfExpression(&taylor_tree, str_taylor_tree);

    char str_tree[MAX_LEN_OF_EXPRESSION] = {};
    SetStringOfExpression(&tree, str_tree);

    PrintMessageInLaTex("Построим графики: ");
    MakeGraph(str_tree, str_taylor_tree, &scale);

    TreeDtor(&taylor_tree);

    VariableArrDtor(&tree);
    BufferDtor(&buffer);
    TreeDtor(&tree);

    return 0;
}
