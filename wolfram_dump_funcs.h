#ifndef WOLFRAM_DUMP_FUNCS
#define WOLFRAM_DUMP_FUNCS

enum ErrCodes
{
    BAD_SIZE = 1,
    BAD_ROOT = 2,
    BAD_PARENT = 4,
};

void PrintTree(Node* node, FILE* graphiz_file, FILE* tex_file);

void TreeDump(Tree* tree);

void PrintTex(FILE* tex_file, Node* node);

void PrintGraphizNode(FILE* graphiz_file, Node* node);

void PrintGraphizEdge(FILE* graphiz_file, Node* node);

void PrintBazeNode(FILE* graphiz_file, Tree* tree);

void PrintBazeEdge(FILE* graphiz_file, Tree* tree);

void FillLogFile(char* image_file_name, Tree* tree, int file_counter);

const char* GetNodeTypeName(Node* node);

const char* GetNodeValueName(Node* node);

static char* GetNewDotCmd(int file_counter);

static char* GetNewImageFileName(int file_counter);

#endif

