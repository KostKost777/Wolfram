#ifndef WOLFRAM_DUMP_FUNCS
#define WOLFRAM_DUMP_FUNCS

extern const char* tex_file_name;

enum ErrCodes
{
    BAD_SIZE = 1,
    BAD_ROOT = 2,
    BAD_PARENT = 4,
};

void PrintTree(Node* node, FILE* graphiz_file);

void TreeDump(Tree* tree);

Node* PrintTexTree(Node* node);

void PrintTexNode(Node* node, FILE* tex_file);

void PrintGraphizNode(FILE* graphiz_file, Node* node);

void PrintGraphizEdge(FILE* graphiz_file, Node* node);

void PrintBazeNode(FILE* graphiz_file, Tree* tree);

void PrintBazeEdge(FILE* graphiz_file, Tree* tree);

void FillLogFile(char* image_file_name, Tree* tree, int file_counter);

const char* GetNodeTypeName(Node* node);

const char* GetNodeValueName(Node* node);

static char* GetNewDotCmd(int file_counter);

static char* GetNewImageFileName(int file_counter);

void PrintLaTex_ADD_SUB(Node* node, FILE* tex_file);

void PrintLaTex_MUL(Node* node, FILE* tex_file);

void PrintLaTex_DIV(Node* node, FILE* tex_file);

void PrintLaTex_POW(Node* node, FILE* tex_file);

void PrintLaTex_EXP(Node* node, FILE* tex_file);

void PrintOpByNameInLaTex(Node* node, FILE* tex_file, const char* name);

void PrintLaTex_FACT(Node* node, FILE* tex_file);

void PrintLaTex_LOG(Node* node, FILE* tex_file);

void PrintLaTex_NUM(FILE* tex_file, double num);

#endif

