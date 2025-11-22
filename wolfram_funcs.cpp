#include <TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "read_wolfram_database.h"

FILE* log_file = NULL;

Status TreeCtor(Tree* tree)
{
    assert(tree);

    tree->size = 0;
    tree->root = NULL;

    return success;
}

double StartExpression(Tree* tree)
{
    assert(tree);

    RequestVariableValue(tree);

    double ans = CalculateExpression(tree, tree->root);

    printf("Результат выражения: %lf\n", ans);

    return ans;
}

Tree StartDefferentiating(Tree* tree)
{
    assert(tree);

    Tree deff_tree = {};
    TreeCtor(&deff_tree);
    deff_tree.var = tree->var;

    deff_tree.root = Defferentiate(&deff_tree, tree->root);

    TreeDump(&deff_tree);

    return deff_tree;
}

Node* Defferentiate(Tree* tree, Node* node)
{
    assert(node);
    assert(tree);

    switch(node->type)
    {
        case NUM:
            return NewNumNode(0, NULL, NULL, tree);

        case VAR:
            return NewNumNode(1, NULL, NULL, tree);

        case OP:
            switch(node->value.op)
            {
                case MUL:
                    return ADD_ ( MUL_ ( dL, cR ), MUL_ ( cL, dR ) );

                case ADD:
                    return ADD_ ( dL, dR );

                case SUB:
                    return SUB_ ( dL, dR );

                case DIV:
                    return DIV_ ( SUB_ ( MUL_ ( dL, cR ), MUL_ ( cL, dR ) ), MUL_ ( cR, cR ) );

                default: break;
            }

        default: break;
    }

    return NULL;
}

Node* CopySubtree(Tree* tree, Node* node)
{
    assert(tree);

    if (node == NULL)
        return NULL;

    switch(node->type)
    {
        case OP:
            return NewOpNode(node->value.op, CopySubtree(tree, node->left),
                                             CopySubtree(tree, node->right),
                                             tree);

        case NUM:
            return NewNumNode(node->value.num, CopySubtree(tree, node->left),
                                               CopySubtree(tree, node->right),
                                               tree);

        case VAR:
            return NewVarNode(node->value.var, CopySubtree(tree, node->left),
                                               CopySubtree(tree, node->right),
                                               tree);

        default:
            break;
    }

    return NULL;
}

Node* NewOpNode(Operation op, Node* left, Node* right, Tree* tree)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    assert(new_node);

    new_node->left = left;
    new_node->right = right;
    new_node->type = OP;
    new_node->value.op = op;

    tree->size++;

    return new_node;
}

Node* NewNumNode(double num, Node* left, Node* right, Tree* tree)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    assert(new_node);

    new_node->left = left;
    new_node->right = right;
    new_node->type = NUM;
    new_node->value.num = num;

    tree->size++;

    return new_node;
}

Node* NewVarNode(Variable var, Node* left, Node* right, Tree* tree)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    assert(new_node);

    new_node->left = left;
    new_node->right = right;
    new_node->type = VAR;
    new_node->value.var.var_name = var.var_name;
    new_node->value.var.var_hash = var.var_hash;
    new_node->value.var.var_data = var.var_data;

    tree->size++;

    return new_node;
}

void RequestVariableValue(Tree* tree)
{
    assert(tree);

    for (size_t i = 0; i < tree->var->size; ++i)
    {
        printf("Введите значение переменной %s: ", tree->var->arr[i].var_name);
        scanf("%lf", &tree->var->arr[i].var_data);
        //printf("DATA: %lf\n\n", tree->var->arr[i].var_data);
    }
}

double CalculateExpression(Tree* tree, Node* node)
{
    assert(tree);
    assert(node);

    double left_result = 0;
    double right_result = 0;

    switch(node->type)
    {
        case NUM:
            //printf("RET NUM: %lf\n", node->value.num);
            return node->value.num;
        case VAR:
            //printf("RET VAR: %lf\n", node->value.var.var_data);
            return GetVariableValue(tree, node->value.var.var_name);

        case OP:
            if (node->left != NULL)
                left_result = CalculateExpression(tree, node->left);

            if (node->right != NULL)
                right_result = CalculateExpression(tree, node->right);

            //printf("RET OP: LEFT: %lf  RIGHT %lf\n", left_result, right_result);
            switch(node->value.op)
            {
                case ADD: return left_result + right_result;
                case SUB: return left_result - right_result;
                case MUL: return left_result * right_result;
                case DIV:
                    if (right_result != 0)
                        return left_result / right_result;
                    else
                        fprintf(log_file, "Деление на ноль\n");
                    break;

                default: break;
            }
            break;

        default: break;
    }

    return 0;
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

    fprintf(log_file, "Не знаю такой переменной |%s|\n\n", var_name);
    return 0;
}

Node* NodeCtor(Node* parent)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    assert(new_node);

    new_node->parent = parent;

    return new_node;
}

size_t GetHash(char* str)
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

void OpenLogFile(const char* log_file_name)
{
    log_file = fopen(log_file_name, "w");

    if (log_file != NULL)
        printf("Logfile open\n");

    else
        printf("Logfile open ERROR\n");

    fprintf(log_file, "<pre>\n");
}
