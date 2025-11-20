#ifndef WOLFRAM_FUNCS
#define WOLFRAM_FUNCS

extern FILE* log_file;

const int NUM_OF_VARIABLE = 50;

enum Status
{
    success = 0,
    error
};

enum Type
{
    OP = 0,
    VAR = 1,
    NUM = 2
};

enum Operation
{
    ADD = 0,
    SUB = 1,
    MUL = 2,
    DIV = 3
};

struct Variable
{
    const char* var_name;
    size_t var_hash;
    double var_data;
};

union Value
{
    Operation op;
    Variable var;
    double num;
};

struct Node
{
    Type type;
    Value value;

    Node* parent;
    Node* left;
    Node* right;
};

struct VariableArr
{
    Variable arr[NUM_OF_VARIABLE];
    size_t size;
};

struct Tree
{
    size_t size;
    int code_err;
    VariableArr var;
    Node* root;
};

Status TreeCtor(Tree* tree);

Node* NodeCtor(Node* parent);

void CloseLogFile();

void OpenLogFile(const char* log_file_name);

size_t GetHash(const char* str);

bool IsEqualHash(size_t hash1, size_t hash2);

#endif
