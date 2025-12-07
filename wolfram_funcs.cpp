#include <TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "new_read_wolfram_database.h"
#include "common_funcs.h"
#include "calculate_expression_funcs.h"

FILE* log_file = NULL;
const char* log_file_name = "wolfram_log_file.html";

Status TreeCtor(Tree* tree)
{
    assert(tree);

    tree->size = 0;
    tree->root = NULL;

    return success;
}

void SetXValue(Tree* tree, double value)
{
    assert(tree);

    size_t hash_x = GetHash("x");

    for (size_t i = 0; i < tree->var->size; ++i)
    {
        if (tree->var->arr[i].var_hash == hash_x)
        {
            tree->var->arr[i].var_data = value;
            return;
        }
    }
}

bool IsDoubleEqual(double num1, double num2)
{
    assert(!isnan(num1));
    assert(!isnan(num2));

    return abs(num1 - num2) < EPSILON;
}

bool IsDoubleBigger(double num1, double num2)
{
    assert(!isnan(num1));
    assert(!isnan(num2));

    return num1 - num2 > EPSILON;
}

ArgType GetArgsType(Node* node)
{
    assert(node);

    for (int i = 0; i < NUM_OF_OP; ++i)
    {
        if (all_op[i].op == node->value.op)
            return all_op[i].args;
    }

    return UNARY;
}

void DefineAndSetArgType(Node* node)
{
    assert(node);

    for (int i = 0; i < NUM_OF_OP; ++i)
    {
        if (all_op[i].op == node->value.op)
            node->args = all_op[i].args;
    }
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

void BufferDtor(Buffer* buffer)
{
    assert(buffer);

    free(buffer->data);
    buffer->data = NULL;

    buffer->size = 0;
}

void TreeDtor(Tree* tree)
{
    DeleteNode(tree, tree->root);

    free(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

void DeleteNode(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    if (node->left != NULL)
        DeleteNode(tree, node->left);

    if (node->left != NULL)
        tree->size--;

    free(node->left);
    node->left = NULL;

    if (node->right != NULL)
        DeleteNode(tree, node->right);

    if (node->right != NULL)
        tree->size--;

    free(node->right);
    node->right = NULL;
}

void CloseLogFile()
{
    fclose(log_file);
    printf("Logfile close");
}

bool IsVarInTree(Node* node)
{
    if (node->type == VAR)
        return true;

    if (   node->left != NULL
        && IsVarInTree(node->left))
        return true;

    if (   node->right != NULL
        && IsVarInTree(node->right))
        return true;

    return false;
}

void OpenLogFile()
{
    log_file = fopen(log_file_name, "w");

    if (log_file != NULL)
        perror("Logfile open ");

    else
        perror("Logfile open ERROR ");

    fprintf(log_file, "<pre>\n");
}

void VariableArrInit(Tree* tree)
{
    tree->var = (VariableArr*)calloc(1, sizeof(VariableArr));

    if (tree->var == NULL)
    {
        perror("Память не аллоцировалась в VariableArrInit()");
        return;
    }

    tree->var->size = 0;
}

void VariableArrDtor(Tree* tree)
{
    for (size_t i = 0; i < tree->var->size; ++i)
    {
        free(tree->var->arr[i].var_name);
        tree->var->arr[i].var_name = NULL;
    }

    free(tree->var);
    tree->var = NULL;
}
