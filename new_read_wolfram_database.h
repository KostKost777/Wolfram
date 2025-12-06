#ifndef NEW_READ_WOLFRAM_DATABASE
#define NEW_READ_WOLFRAM_DATABASE

const size_t MAX_FUNC_LEN = 10;

const size_t MAX_NAME_LAN = 50;

#include "wolfram_funcs.h"

struct Buffer
{
    char* data;
    int size;
};

void SkipSpaces(char** cur_pos);

void BufferDtor(Buffer* buffer);

int GetSizeOfFile(const char* filename);

Status GetDataBaseFromFile(Buffer* buffer,
                           const char* input_filename);

Node* GetGeneral(char** str, Tree* tree, Node* node);

Node* GetDoubleNumber(char** str, Tree* tree, Node* node);

Node* GetAddSubOp(char** str, Tree* tree, Node* node);

Node* GetMulDivOp(char** str, Tree* tree, Node* node);

Node* GetMostPreority(char** str, Tree* tree, Node* node);

Node* GetPowOp(char** str, Tree* tree, Node* node);

Node* GetFunction(char** str, Tree* tree, Node* node);

Node* GetWord(char** str, Tree* tree, Node* node);

Node* GetVariable(char** str, Tree* tree, Node* node);

bool IsNum(char sym);

bool IsSymbolInVarName(char sym);

char* GetFuncName(char* str);

void UpdateVarArray(Tree* tree, Variable new_var);

void ParseTree(Tree* tree, char* buffer);

#endif
