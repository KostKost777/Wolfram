#include <TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "read_wolfram_database.h"

FILE* log_file = NULL;

Status TreeCtor(Tree* tree)
{
    assert(tree);

    tree->size = 1;
    tree->var.size = 0;

    return success;
}

Node* NodeCtor(Node* parent)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    assert(new_node);

    new_node->parent = parent;

    return new_node;
}

size_t GetHash(const char* str)
{
    size_t hash = 5381;
    int c = 0;

    while ((c = *str++) != '\0')
        hash = ((hash << 5) + hash) + c;

    return hash;
}

bool IsEqualHash(size_t hash1, size_t hash2)
{
    if (hash1 != hash2)
        return false;

    return true;
}

void CloseLogFile()
{
    fclose(log_file);
    printf("Logfile close");
}

void OpenLogFile(const char* log_file_name)
{
    log_file = fopen(log_file_name, "w");

    if (log_file != NULL)
        printf("Logfile open\n");

    else
        printf("Logfile open ERROR\n");

    fprintf(log_file, "<pre>\n");
}
