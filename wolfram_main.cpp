#include<TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "read_wolfram_database.h"

int main(int argc, const char* argv[])
{
    atexit(CloseLogFile);

    const char* database_file_name = "database.txt";

    if (argc > 1)
        database_file_name = argv[1];

    OpenLogFile("wolfram_log_file.html");

    Buffer buffer = {};
    GetDataBaseFromFile(&buffer, database_file_name);

    Tree tree = {};

    TreeCtor(&tree);

    char* cur_pos = buffer.data;
    tree.root = FillNodeDataFromBuffer(&cur_pos, &tree, tree.root);

//     printf("BUFF: %s\n", buffer.data);
//     printf("CUR_POS: %s\n", cur_pos);
//
//     printf("TREE_ROOT: %p\n", tree.root);
//     printf("VAR_SIZE: %llu", tree.var.size);

    //StartExpression(&tree);

    StartDefferentiating(&tree);

    TreeDump(&tree);

    TreeDtor(&tree);
    BufferDtor(&buffer);

    return 0;
}
