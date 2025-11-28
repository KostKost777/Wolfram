#include<TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "read_wolfram_database.h"
#include "common_funcs.h"

int main(int argc, const char* argv[])
{
    atexit(FilesClosingProcessing);

    const char* database_file_name = "database.txt";

    if (argc > 1)
        database_file_name = argv[1];

    FilesOpeningProcessing();
    SetAllOpHash();

    qsort(all_op, NUM_OF_OP, sizeof(StructOperation), OpHashComparator);

    Buffer buffer = {};
    GetDataBaseFromFile(&buffer, database_file_name);

    Tree tree = {};

    TreeCtor(&tree);
    tree.var = (VariableArr*)calloc(1, sizeof(VariableArr));
    tree.var->size = 0;

    char* cur_pos = buffer.data;
    tree.root = FillNodeDataFromBuffer(&cur_pos, &tree, tree.root);

//     printf("BUFF: %s\n", buffer.data);
//     printf("CUR_POS: %s\n", cur_pos);
//
//     printf("TREE_ROOT: %p\n", tree.root);
//     printf("VAR_SIZE: %llu\n", tree.var->size);

    StartExpression(&tree);

    TreeDump(&tree);

    StartDefferentiating(&tree);

    TreeDtor(&tree);
    BufferDtor(&buffer);

    return 0;
}
