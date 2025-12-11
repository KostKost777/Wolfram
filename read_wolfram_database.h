#ifndef READ_WOLFRAM_DATABASE
#define READ_WOLFRAM_DATABASE

#include "wolfram_funcs.h"

struct Buffer
{
    char* data;
    int size;
};

void SkipSpaces(char** cur_pos);

void DefineAndSetArgType(Node* node);

void BufferDtor(Buffer* buffer);

int GetSizeOfFile(const char* filename);

Status GetDataBaseFromFile(Buffer* buffer,
                           const char* input_filename);

Node* FillNodeDataFromBuffer(char** cur_pos, Tree* tree, Node* parent);

bool IsNil(char* cur_pos);

double ReadNumber(char** cur_pos);

Operation ReadOperation(char** cur_pos);

char* ReadVariable(char** cur_pos);

Type DefineNewNodeType(char* cur_pos);

bool IsOperation(char* name);

void UpdateVarArray(Tree* tree, Variable new_var);

int OpHashComparator(const void* param1, const void* param2);

// int BinSearchCompForOpHash(const void* param1, const void* param2);

#endif
