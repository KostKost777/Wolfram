#include<TXLib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "new_read_wolfram_database.h"
#include "common_funcs.h"
#include "DSL_funcs.h"

void ParseTree(Tree* tree, char* buffer)
{
    assert(tree);
    assert(buffer);

    tree->root = GetGeneral(&buffer,tree, tree->root);
}

Status GetDataBaseFromFile(Buffer* buffer,
                           const char* input_filename)
{
    assert(input_filename != NULL);

    int file_descriptor = open(input_filename, O_RDONLY);

    if (file_descriptor == -1)
    {
        fprintf(log_file, "<h3>Error opening file |%s|</h3>", input_filename);
        close(file_descriptor);

        return error;
    }

    buffer->size = GetSizeOfFile(input_filename);

    if (buffer->size == -1)
        return error;

    buffer->data = (char* )calloc(buffer->size + 1, sizeof(char));
    assert(buffer->data != NULL);

    buffer->size = read(file_descriptor, buffer->data, buffer->size);

    if (buffer->size == -1)
    {
        fprintf(log_file, "<h3>Error reading file |%s|</h3>", input_filename);
        close(file_descriptor);

        return error;
    }

    buffer->data[buffer->size] = '\0';

    close(file_descriptor);

    return success;
}

Node* GetGeneral(char** str, Tree* tree, Node* node)
{
    char* str_begin = *str;

    fprintf(log_file, "<strong>Вызов GetGeneral</strong>\n");
    fprintf(log_file, "<strong>Начинаю считывать</strong>\n");
    fprintf(log_file, "<strong>Состояние буфера: </strong>\n |%s| \n\n", *str);

    node = GetAddSubOp(str, tree, node);

    if (**str != '$')
    {
        fprintf(log_file, "<strong>Буфер во время ошибки: </strong>\n |%s| \n", *str);
        fprintf(log_file, "<strong>Ошибочный символ \"%c\" </strong>\n", **str);
        fprintf(log_file, "<strong>Позиция: %lld</strong>\n", *str - str_begin + 1);
    }

    return node;
}

Node* GetAddSubOp(char** str, Tree* tree, Node* node)
{
    fprintf(log_file, "<strong>Вызов GetAddSubOp</strong>\n");
    fprintf(log_file, "<strong>Состояние буфера: </strong>\n |%s| \n\n", *str);

    node = GetMulDivOp(str, tree, node);

    fprintf(log_file, "<strong>Наичнаю проверку на наличие \"+/-\": </strong>\n |%s| \n\n", *str);

    while (**str == '+' || **str == '-')
    {
        fprintf(log_file, "<strong>Нашел</strong>\n\n");

        char op = **str;

        fprintf(log_file, "<strong>Состояние буферера до пропуска \"+/-\": </strong>\n |%s| \n\n", *str);
        *str += 1;
        fprintf(log_file, "<strong>Состояние буферера после пропуска \"+/-\": </strong>\n |%s| \n\n", *str);
        SkipSpaces(str);

        if (op == '+')
        {
            fprintf(log_file, "<strong>Создал новую вершину с операцией \"+\" </strong>\n\n");
            node = NewOpNode(ADD, node, GetMulDivOp(str, tree, node), tree);
        }
        else
        {
            fprintf(log_file, "<strong>Создал новую вершину с операцией \"+\" </strong>\n\n");
            node = NewOpNode(SUB, node, GetMulDivOp(str, tree, node), tree);
        }
    }

    fprintf(log_file, "<strong>Не нашел, возвращаю NULL </strong>\n\n");

    return node;
}

Node* GetMulDivOp(char** str, Tree* tree, Node* node)
{
    fprintf(log_file, "<strong>Вызов GetMulDivOp</strong>\n");
    fprintf(log_file, "<strong>Состояние буфера: </strong>\n |%s| \n\n", *str);

    node = GetPowOp(str, tree, node);

    fprintf(log_file, "<strong>Наичнаю проверку на наличие \"* или /\": </strong>\n |%s| \n\n", *str);
    while (**str == '*' || **str == '/')
    {
        fprintf(log_file, "<strong>Нашел </strong>\n\n");

        char op = **str;
        fprintf(log_file, "<strong>Состояние буферера до пропуска \"* или /\": </strong>\n |%s| \n\n", *str);
        *str += 1;
        fprintf(log_file, "<strong>Состояние буферера после пропуска \"* или /\": </strong>\n |%s| \n\n", *str);
        SkipSpaces(str);

        if (op == '*')
        {
            fprintf(log_file, "<strong>Создал новую вершину с операцией \"*\" </strong>\n\n");
            node = NewOpNode(MUL, node, GetPowOp(str, tree, node), tree);
        }
        else
        {
            fprintf(log_file, "<strong>Создал новую вершину с операцией \"/\" </strong>\n\n");
            node = NewOpNode(DIV, node, GetPowOp(str, tree, node), tree);
        }
    }

    fprintf(log_file, "<strong>Не нашел, возвращаю NULL </strong>\n\n");

    return node;
}

Node* GetMostPreority(char** str, Tree* tree, Node* node)
{
    fprintf(log_file, "<strong>Вызов GetMostPreority</strong>\n");
    fprintf(log_file, "<strong>Состояние буфера: </strong>\n |%s| \n\n", *str);

    if (**str == '(')
    {
        fprintf(log_file, "<strong>Нашел скобку \"(\": </strong>\n |%s| \n\n", *str);
        *str += 1;
        fprintf(log_file, "<strong>Пропустил скобку \"(\": </strong>\n |%s| \n\n", *str);
        SkipSpaces(str);

        node = GetAddSubOp(str, tree, node);

        if (**str == ')')
        {
            fprintf(log_file, "<strong>Нашел скобку \")\": </strong>\n |%s| \n\n", *str);
            *str += 1;
            fprintf(log_file, "<strong>Пропустил скобку \")\": </strong>\n |%s| \n\n", *str);
            SkipSpaces(str);
            return node;
        }

        fprintf(log_file, "<strong>Ошибка не хвататет \")\"</strong>\n |%s| \n\n", *str);
    }

    node = GetDoubleNumber(str, tree, node);
    if (node != NULL)
    {
        fprintf(log_file, "<strong>Это оказалось число</strong>\n |%s| \n\n", *str);
        return node;
    }

    node = GetWord(str, tree, node);
    if (node != NULL)
    {
        fprintf(log_file, "<strong>Это оказалось функция</strong>\n |%s| \n\n", *str);
        return node;
    }

    return NULL;
}

Node* GetPowOp(char** str, Tree* tree, Node* node)
{
    fprintf(log_file, "<strong>Вызов GetPowOp</strong>\n");
    fprintf(log_file, "<strong>Состояние буфера: </strong>\n |%s| \n\n", *str);

    node = GetMostPreority(str, tree, node);

    while (**str == '^')
    {
        fprintf(log_file, "<strong>Состояние буферера до пропуска \"^\": </strong>\n |%s| \n\n", *str);
        *str += 1;
        fprintf(log_file, "<strong>Состояние буферера после пропуска \"^\": </strong>\n |%s| \n\n", *str);
        SkipSpaces(str);

        node = NewOpNode(POW, node, GetMostPreority(str, tree, node), tree);
    }

    return node;
}

Node* GetDoubleNumber(char** str, Tree* tree, Node* node)
{
    fprintf(log_file, "<strong>Вызов GetDoubleNumber</strong>\n");
    fprintf(log_file, "<strong>Состояние буфера: </strong>\n |%s| \n\n", *str);

    double val = 0;
    double sgn = 1;

    if (**str == '-')
    {
        fprintf(log_file, "<strong>Обнаружил минус, значит число отрицательное: </strong>\n |%s| \n\n", *str);
        sgn = -1;

        str += 1;
    }
    if (!IsNum(**str))
    {
        fprintf(log_file, "<strong>Походуэто не число, пусть кто-то другой чекает: </strong>\n |%s| \n\n", *str);
        return NULL;
    }

    while (IsNum(**str))
    {
        val = 10 * val + (**str - '0');
        *str += 1;
    }

    if (**str == '.' && IsNum(*(*str + 1)) )
    {
        fprintf(log_file, "<strong>Это число типа double: </strong>\n |%s| \n\n", *str);
        *str += 1;

        size_t count_num_after_point = 0;

        while (IsNum(**str))
        {
            val = 10 * val + (**str - '0');

            *str += 1;

            count_num_after_point++;
        }

        val /= pow(10, count_num_after_point);
    }

    SkipSpaces(str);

    fprintf(log_file, "<strong>Прочитал такое число %g </strong>\n\n", val * sgn);

    return NewNumNode(val * sgn, NULL, NULL, tree);
}

Node* GetWord(char** str, Tree* tree, Node* node)
{
    fprintf(log_file, "<strong>Вызов GetFunction</strong>\n");
    fprintf(log_file, "<strong>Состояние буфера: </strong>\n |%s| \n\n", *str);

    node = GetFunction(str, tree, node);

    if (node != NULL)
    {
        fprintf(log_file, "<strong>Это был функция</strong>\n\n");
        return node;
    }

    node = GetVariable(str, tree, node);

    if (node != NULL)
    {
        fprintf(log_file, "<strong>Это была переменная</strong>\n\n");
        return node;
    }

    return NULL;
}

Node* GetFunction(char** str, Tree* tree, Node* node)
{
    fprintf(log_file, "<strong>Вызов GetFunction</strong>\n");
    fprintf(log_file, "<strong>Состояние буфера: </strong>\n |%s| \n\n", *str);

    char* func_name = GetFuncName(*str);
    size_t hash_func_name = GetHash(func_name);
    bool is_op = false;
    size_t i = 0;

    fprintf(log_file, "<strong>Получил имя: |%s|</strong>\n\n", func_name);

    for (; i < NUM_OF_OP; ++i)
    {
        if (hash_func_name == all_op[i].hash)
        {
            is_op = true;
            break;
        }
    }

    if (is_op)
    {
        free(func_name);
        func_name = NULL;

        fprintf(log_file, "<strong>Состояние буферера до пропуска \"%s\": </strong>\n |%s| \n\n", all_op[i].name, *str);
        *str += strlen(all_op[i].name);
        fprintf(log_file, "<strong>Состояние буферера после пропуска \"%s\": </strong>\n |%s| \n\n", all_op[i].name, *str);

        if (all_op[i].args == UNARY)
        {
            if (**str == '(')
            {
                *str += 1;
                fprintf(log_file, "<strong>Пропустил скобку \"(\": </strong>\n |%s| \n\n", *str);
                SkipSpaces(str);

                node = GetAddSubOp(str, tree, node);

                if (**str == ')')
                {
                    *str += 1;
                    fprintf(log_file, "<strong>Пропустил скобку \")\": </strong>\n |%s| \n\n", *str);
                    SkipSpaces(str);
                    return NewOpNode(all_op[i].op, NULL, node, tree);
                }
                fprintf(log_file, "<strong>Ошибка не хвататет \")\"</strong>\n |%s| \n\n", *str);
            }
        }

        else
        {
            if (**str == '(')
            {
                *str += 1;
                fprintf(log_file, "<strong>Пропустил скобку \"(\": </strong>\n |%s| \n\n", *str);
                SkipSpaces(str);

                node = GetAddSubOp(str, tree, node);

                if (**str == ',')
                {
                    *str += 1;
                    fprintf(log_file, "<strong>Пропустил запятую \",\": </strong>\n |%s| \n\n", *str);
                    SkipSpaces(str);

                    Node* right = NULL;
                    right = GetAddSubOp(str, tree, right);

                    if (**str == ')')
                    {
                        *str += 1;
                        fprintf(log_file, "<strong>Пропустил скобку \")\": </strong>\n |%s| \n\n", *str);
                        SkipSpaces(str);
                        return NewOpNode(all_op[i].op, node, right, tree);
                    }
                    fprintf(log_file, "<strong>Ошибка не хвататет \")\"</strong>\n |%s| \n\n", *str);
                }
                fprintf(log_file, "<strong>Ошибка не хвататет \",\"</strong>\n |%s| \n\n", *str);
            }
        }
    }

    return NULL;
}

Node* GetVariable(char** str, Tree* tree, Node* node)
{
    fprintf(log_file, "<strong>Вызов GetVariable</strong>\n");
    fprintf(log_file, "<strong>Состояние буфера: </strong>\n |%s| \n\n", *str);

    int counter = 0;
    char var_name[MAX_NAME_LAN] = {};

    while(true)
    {
        if (!((counter != 0 && IsNum(**str)) || IsSymbolInVarName(**str)))
            break;

        var_name[counter] = **str;

        counter++;
        *str += 1;
    }

    SkipSpaces(str);

    fprintf(log_file, "<strong>Состояние буфера после считывания переменной: </strong>\n |%s| \n\n", *str);
    fprintf(log_file, "<strong>Имя переменной: </strong>|%s| \n\n", var_name);

    Variable new_var = {strdup(var_name), GetHash(var_name), 0};

    UpdateVarArray(tree, new_var);

    return NewVarNode(new_var, NULL, NULL, tree);
}

bool IsSymbolInVarName(char sym)
{
    return isalpha(sym) || sym == '_';
}

char* GetFuncName(char* str)
{
    fprintf(log_file, "<strong>Вызов GetFuncName</strong>\n");
    fprintf(log_file, "<strong>Состояние буфера: </strong>\n |%s| \n\n", str);

    char func_name[MAX_FUNC_LEN] = {};
    size_t size = 0;

    while(*str >= 'a' && *str <= 'z')
    {
        func_name[size] = *str;
        size++;
        str++;
    }

    func_name[size] = '\0';
    fprintf(log_file, "<strong>Прочитал имя: |%s|</strong>\n |%s| \n\n", func_name, str);

    return strdup(func_name);
}

//  TODO: isdigit();
bool IsNum(char sym)
{
    return (sym >= '0' && sym <= '9');
}

void SkipSpaces(char** cur_pos)
{
    assert(cur_pos);

    fprintf(log_file, "<strong>Пропускаю все пробельные символы</strong>\n\n");
    fprintf(log_file, "<strong>До пропуска BUFFER:</strong> \n|%s|\n\n",
                                                              *cur_pos);

    while (isspace(**cur_pos))
        *cur_pos += 1;

    fprintf(log_file, "<strong>После пропуска BUFFER:</strong> \n|%s|\n\n",
                                                                 *cur_pos);
}

int GetSizeOfFile(const char* filename)
{
    assert(filename != NULL);

    struct stat file_info = {};

    if (stat(filename, &file_info) == -1)
    {
        fprintf(log_file, "Не открылся файл |%s|", filename);

        return -1;
    }

    return (int)file_info.st_size;
}

void UpdateVarArray(Tree* tree, Variable new_var)
{
    assert(tree);

    bool is_really_new_var = true;

    fprintf(log_file, "<strong>Наичнаю проверку новая ли это переменная</strong>\n\n");

    for (size_t i = 0; i < tree->var->size; ++i)
    {
        if (new_var.var_hash == tree->var->arr[i].var_hash &&
            strcmp(new_var.var_name, tree->var->arr[i].var_name) == 0)
        {
            fprintf(log_file, "<strong>Переменная |%s| уже есть</strong>\n\n", new_var.var_name);
            is_really_new_var = false;
        }
    }

    if (is_really_new_var)
    {
        fprintf(log_file, "<strong>Добавил новую переменную |%s|</strong>\n\n", new_var.var_name);

        tree->var->arr[tree->var->size].var_name = new_var.var_name;
        tree->var->arr[tree->var->size].var_hash = GetHash(new_var.var_name);

        tree->var->size++;
    }
}


