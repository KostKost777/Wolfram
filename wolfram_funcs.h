#ifndef WOLFRAM_FUNCS
#define WOLFRAM_FUNCS

extern FILE* log_file;

const int NUM_OF_VARIABLE = 50;

const int NUM_OF_OP = 21;

const double EPSILON = 10E-6;

extern const char* log_file_name;

enum Status
{
    success = 0,
    error
};

enum Type
{
    OP = 0,
    VAR = 1,
    NUM = 2,
};

enum ArgType
{
    UNARY = 1,
    BINARY = 2,
};

enum Operation
{
    ADD = 0,
    SUB = 1,
    MUL = 2,
    DIV = 3,
    EXP = 4,
    POW = 5,
    LN = 6,
    LOG = 7,
    COS = 8,
    SIN = 9,
    TG = 10,
    CTG = 11,
    SH = 12,
    CH = 13,
    ARCSIN = 14,
    ARCCOS = 15,
    ARCTG = 16,
    ARCCTG = 17,
    TH = 18,
    CTH = 19,
    FACT = 20
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
    ArgType args;
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

    VariableArr* var;
    Node* root;
};

Status TreeCtor(Tree* tree);

Node* NodeCtor(Node* parent);

void CloseLogFile();

void OpenLogFile();

size_t GetHash(const char* str);

bool IsEqualHash(size_t hash1, size_t hash2);

void TreeDtor(Tree* tree);

void DeleteNode(Tree* tree, Node* node);

ArgType GetArgsType(Node* node);

void DefineAndSetArgType(Node* node);

bool IsDoubleEqual(double num1, double num2);

bool IsDoubleBigger(double num1, double num2);

bool IsVarInTree(Node* node);

void StartTaylor (Tree* tree);

void SetVarXValue(Tree* tree, double value);

Node* MakeTaylorTree(Tree* tree,
                     double* taylor_coeff, size_t member,
                     size_t accuracy, Variable var_x);

void VariableArrDtor(Tree* tree);

void VariableArrInit(Tree* tree);

#endif
