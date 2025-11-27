#ifndef WOLFRAM_FUNCS
#define WOLFRAM_FUNCS

extern FILE* log_file;

const int NUM_OF_VARIABLE = 50;

const int NUM_OF_OP = 20;

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
    CTH = 19
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
    VariableArr* var;
    Node* root;
};

Status TreeCtor(Tree* tree);

Node* NodeCtor(Node* parent);

void FilesClosingProcessing();

void FilesOpeningProcessing();

size_t GetHash(const char* str);

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

bool IsDoubleEqual(double num1, double num2);

bool IsDoubleBigger(double num1, double num2);

Node* NewNumNode(double num, Node* left, Node* right, Tree* tree);

Node* NewVarNode(Variable var, Node* left, Node* right, Tree* tree);

Node* NewOpNode(Operation op, Node* left, Node* right, Tree* tree);

double ConstantsOptimization(Node* node, Tree* tree);

Status NeutralElementOptimization(Node* node, Tree* tree);

void RemoveNeutralElement(Tree* tree, Node* node_dad, Node* node_first_son);

bool IsVarInTree(Node* node);

#define dL               Defferentiate(tree, node->left)
#define dR               Defferentiate(tree, node->right)
#define cL               CopySubtree(tree, node->left)
#define cR               CopySubtree(tree, node->right)

#define CNST_(var)       NewNumNode(var, NULL, NULL, tree)
#define ADD_(var1, var2) NewOpNode(ADD, var1, var2, tree)
#define SUB_(var1, var2) NewOpNode(SUB, var1, var2, tree)
#define MUL_(var1, var2) NewOpNode(MUL, var1, var2, tree)
#define DIV_(var1, var2) NewOpNode(DIV, var1, var2, tree)
#define POW_(var1, var2) NewOpNode(POW, var1, var2, tree)
#define EXP_(var)        NewOpNode(EXP, NULL, var, tree)
#define LN_(var)         NewOpNode(LN, NULL, var, tree)
#define LOG_(var1, var2) NewOpNode(LOG, var1, var2, tree)
#define COS_(var)        NewOpNode(COS, NULL, var, tree)
#define SIN_(var)        NewOpNode(SIN, NULL, var, tree)
#define TG_(var)         NewOpNode(TG, NULL, var, tree)
#define CTG_(var)        NewOpNode(CTG, NULL, var, tree)
#define SH_(var)         NewOpNode(SH, NULL, var, tree)
#define CH_(var)         NewOpNode(CH, NULL, var, tree)
#define TH_(var)         NewOpNode(TH, NULL, var, tree)
#define CTH_(var)        NewOpNode(CTH, NULL, var, tree)
#define ARCCOS_(var)     NewOpNode(ARCCOS, NULL, var, tree)
#define ARCSIN_(var)     NewOpNode(ARCSIN, NULL, var, tree)
#define ARCTG_(var)      NewOpNode(ARCTG, NULL, var, tree)
#define ARCCTG_(var)     NewOpNode(ARCCTG, NULL, var, tree)

#define RIGHT_COMP_FUNC_(var) NewOpNode(MUL, var, dR, tree)
#define LEFT_COMP_FUNC_(var) NewOpNode(MUL, var, dL, tree)

#endif
