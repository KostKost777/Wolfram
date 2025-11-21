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
    char* var_name;
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

size_t GetHash(char* str);

bool IsEqualHash(size_t hash1, size_t hash2);

void TreeDtor(Tree* tree);

void DeleteNode(Tree* tree, Node* node);

void RequestVariableValue(Tree* tree);

double CalculateExpression(Tree* tree, Node* node);

double StartExpression(Tree* tree);

double GetVariableValue(Tree* tree, char* var_name);

Tree StartDefferentiating(Tree* tree);

Node* Defferentiate(Tree* tree, Node* node);

Node* CopySubtree(Tree* tree, Node* node);

Node* NewNumNode(double num, Node* left, Node* right, Tree* tree);

Node* NewVarNode(Variable var, Node* left, Node* right, Tree* tree);

Node* NewOpNode(Operation op, Node* left, Node* right, Tree* tree);

#define dL               Defferentiate(tree, node->left)
#define dR               Defferentiate(tree, node->right)
#define cL               CopySubtree(tree, node->left)
#define cR               CopySubtree(tree, node->right)
#define DIV_(var1, var2) NewOpNode(DIV, var1, var2, tree)
#define ADD_(var1, var2) NewOpNode(ADD, var1, var2, tree)
#define SUB_(var1, var2) NewOpNode(SUB, var1, var2, tree)
#define MUL_(var1, var2) NewOpNode(MUL, var1, var2, tree)
#define DIV_(var1, var2) NewOpNode(DIV, var1, var2, tree)

#endif
