#include<TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "new_read_wolfram_database.h"
#include "common_funcs.h"
#include "tex_funcs.h"
#include "calculate_expression_funcs.h"
#include "derivate_tree_funcs.h"
#include "taylor_funcs.h"

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
    ParseTree(&tree, buffer.data);

    PrintMessageInLaTex("Было введено следующие выражение: ");

    TreeDump(&tree);

    RequestVariableValue(&tree);
    double ans = CalculateExpression(&tree, tree.root);
    printf("Result: %lf\n", ans);

    Tree derivate_tree = {};
    TreeCtor(&derivate_tree);

    derivate_tree = GetDerivateTree(&tree);

    PrintMessageInLaTex("Первая производная: ");
    TreeDump(&derivate_tree);

    TreeDtor(&derivate_tree);

    Tree taylor_tree = GetTaylorTree(&tree);

    TreeDtor(&taylor_tree);

    VariableArrDtor(&tree);
    BufferDtor(&buffer);
    TreeDtor(&tree);

    return 0;
}
