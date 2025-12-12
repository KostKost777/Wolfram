#ifndef GRAPH_FUNCS
#define GRAPH_FUNCS

const int MAX_LEN_OF_EXPRESSION = 1000;

struct Scale
{
    double x_min;
    double x_max;
    double y_min;
    double y_max;
};

void FillFileWIthPointForGraph(const char* file_name,
                               Tree* tree, double x_min, double x_max);

void GetNodeString(Node* node, FILE* expression_buffer);

void SetStringOfExpression(Tree* tree, char* str_expression);

void MakeGraph(char* func1, char* func2, char* func3, Scale* scale);

void PrintExpression_ADD_SUB(Node* node, FILE*expression_buffer);

void PrintExpression_MUL(Node* node, FILE*expression_buffer);

void PrintExpression_DIV(Node* node, FILE*expression_buffer);

void PrintExpression_POW(Node* node, FILE*expression_buffer);

void PrintExpression_EXP(Node* node, FILE*expression_buffer);

void PrintOpByNameInExpression(Node* node, FILE*expression_buffer,
                               const char* name);

void PrintExpression_FACT(Node* node, FILE*expression_buffer);

void PrintExpression_LOG(Node* node, FILE*expression_buffer);

void PrintExpression_NUM(FILE* expression_buffer, double num);

#endif
