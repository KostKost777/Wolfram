#include<TXLib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "read_wolfram_database.h"

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

Node* FillNodeDataFromBuffer(char** cur_pos, Tree* tree, Node* parent)
{
    assert(cur_pos);

    const int NIL_LEN = 3;
    char* node_ptr = NULL;

    fprintf(log_file, "<h2>=======Новый вызов функции создания узла=======</h2>\n\n");

    fprintf(log_file, "<strong>BUFFER:</strong> \n|%s|\n\n", *cur_pos);
    fprintf(log_file, "<strong>SIZE:</strong> %llu \n\n", tree->size);

    SkipSpaces(cur_pos);

    if (**cur_pos == '(')
    {
        fprintf(log_file, "<strong>Обнаружил скобку \"(\"</strong>\n\n");

        *cur_pos += 1;
        fprintf(log_file, "<strong>Пропустил скобку:</strong> \n|%s|\n\n",
                          *cur_pos);

        SkipSpaces(cur_pos);

        fprintf(log_file, "<strong>Пытаюсь понять какой тип объекта:</strong>\n\n");

        Node* node = (Node*)calloc(1, sizeof(Node));

        switch(DefineNewNodeType(*cur_pos))
        {
            case VAR:
                node->type = VAR;
                node->value.var.var_name = ReadVariable(cur_pos);
                node->value.var.var_hash = GetHash(node->value.var.var_name);

                UpdateVarArray(tree, node->value.var);
                break;

            case OP:
                node->type = OP;
                node->value.op = ReadOperation(cur_pos);
                break;

            case NUM:
                node->type = NUM;
                node->value.num = ReadNumber(cur_pos);
                break;

            default: break;
        }

        node->parent = parent;

        fprintf(log_file, "<strong>Создал новую вершину</strong>\n"
                          "<strong>NODE_PTR:</strong> %p \n"
                          "<strong>TYPE:</strong> |%s| \n",
                          node, GetNodeTypeName(node));

        if (node->type != NUM)
        {
            fprintf(log_file, "<strong>VALUE:</strong> |%s| \n\n",
                                          GetNodeValueName(node));
        }
        else
           fprintf(log_file, "<strong>VALUE:</strong> |%lf| \n\n",
                                        node->value.num);

        tree->size++;

        fprintf(log_file, "<strong>Увеличил размер SIZE:</strong> %llu \n\n", tree->size);

        node->left = FillNodeDataFromBuffer(cur_pos, tree, node);

        fprintf(log_file, "<strong>Завершил левый узел:</strong> %p \n"
                          "<strong>У этого узла:</strong> %p \n\n",
                                                   node->left, node);
        free(node_ptr);

        node->right = FillNodeDataFromBuffer(cur_pos, tree, node);

        fprintf(log_file, "<strong>Завершил правый узел:</strong> %p \n"
                          "<strong>У этого узла:</strong> %p\n\n ",
                                                node->right, node);

        SkipSpaces(cur_pos);

        *cur_pos += 1;

        fprintf(log_file, "Пропустил \")\": \n|%s|\n\n", *cur_pos);

        fprintf(log_file, "<strong>Сейчас верну этот указатель %p</strong> \n\n",
                                                                       node);
        return node;
    }

    else if (IsNil(*cur_pos))
    {
        *cur_pos += NIL_LEN;

        fprintf(log_file, "<strong>Пропустил \"nil\":</strong> \n|%s|\n\n", *cur_pos);

        SkipSpaces(cur_pos);

        return NULL;
    }

    return NULL;
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

Type DefineNewNodeType(char* cur_pos)
{
    assert(cur_pos);

    const int MAX_OBJ_LEN = 100;
    char obj_name[MAX_OBJ_LEN] = {};

    double new_num = 0;
    size_t obj_len = 0;

    int status = sscanf(cur_pos, "%lf%llu", &new_num, &obj_len);
    if (status != 0)
        return NUM;

    sscanf(cur_pos, "%s", obj_name);
    fprintf(log_file, "<strong>Прочитал такой объект: </strong> %s\n", obj_name);

    obj_len = strlen(obj_name);
    fprintf(log_file, "<strong>Его длина: </strong> %llu\n", obj_len);

    if (IsOperation(obj_name, obj_len))
        return OP;

    return VAR;
}

bool IsOperation(char* name, size_t len)
{
    assert(name);

    if (strcmp(name, "sin") == 0 ||
        strcmp(name, "cos") == 0)
        return true;

    if (len == 1)
    {
        switch(name[0])
        {
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case '%':
                return true;

            default:
                return false;
        }
    }

    return false;
}

char* ReadVariable(char** cur_pos)
{
    assert(cur_pos);
    assert(*cur_pos);

    int len = 0;
    sscanf(*cur_pos, "%*s%n", &len);

    fprintf(log_file, "<strong>Посчитал длину переменной LEN:</strong> %d\n\n",
                                                                          len);

    *(*cur_pos + len) = '\0';
    char* var_name = *cur_pos;
    fprintf(log_file, "<strong>Получил имя переменой NAME:</strong> |%s|\n\n",
                                                                     *cur_pos);

    *cur_pos += len + 1;

    fprintf(log_file, "<strong>Буфер после считывания переменной</strong>\n"
                      "<strong>BUFFER:</strong> |%s|\n\n", *cur_pos);

    return var_name;
}

Operation ReadOperation(char** cur_pos)
{
    assert(cur_pos);
    assert(*cur_pos);

    int len = 0;

    sscanf(*cur_pos, "%*s%n", &len);

    fprintf(log_file, "<strong>Посчитал длину операции LEN:</strong> %d\n\n",
                                                                        len);

    *(*cur_pos + len) = '\0';
    char* op_name = *cur_pos;
    fprintf(log_file, "<strong>Получил имя операции NAME:</strong> |%s|\n\n",
                                                                    *cur_pos);

    *cur_pos += len + 1;
    fprintf(log_file, "<strong>Буфер после считывания операции</strong>\n"
                      "<strong>BUFFER:</strong> |%s|\n\n", *cur_pos);

    switch(op_name[0])
    {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '/': return DIV;
        default: break;
    }

    return ADD;
}

double ReadNumber(char** cur_pos)
{
    assert(cur_pos);
    assert(*cur_pos);

    double num = 0;
    int len = 0;

    sscanf(*cur_pos, "%lf%n", &num, &len);

    fprintf(log_file, "<strong>Посчитал длину числа LEN:</strong> %d\n\n",
                                                                     len);

    fprintf(log_file, "<strong>Получил число NUM:</strong> |%lf|\n\n",
                                                                 num);
    *cur_pos += len;

    fprintf(log_file, "<strong>Буфер после считывания числа</strong>\n"
                      "<strong>BUFFER:</strong> |%s|\n\n", *cur_pos);

    return num;
}

bool IsNil(char* cur_pos)
{
    char checker[4] = {};

    sscanf(cur_pos, "%3s", checker);
    fprintf(log_file, "<strong>Прочитал потенциальный NIL получил:"
                      "</strong> |%s|\n\n",
                      checker);

    if (strcmp(checker, "nil") == 0)
        return true;

    return false;
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
