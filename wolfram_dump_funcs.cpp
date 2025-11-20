
#include<TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "read_wolfram_database.h"

void TreeDump(Tree* tree)
{
    assert(tree);

    fprintf(log_file, "<h3>SIZE: %llu\n</h3>", tree->size);

    static int file_counter = 0;
    char* image_file_name = GetNewImageFileName(file_counter);
    FILE* graphiz_file = fopen(image_file_name, "w");

    fprintf(graphiz_file, "digraph {\n"
                          "graph [charset=\"utf-8\"]"
                          "rankdir = HR;\n"
                          "splines = true\n");

    PrintBazeNode(graphiz_file, tree);

    if (tree->size > 0) {

        PrintBazeEdge(graphiz_file, tree);
        PrintTree(tree->root, graphiz_file);
    }

    fprintf(graphiz_file, "}");

    fclose(graphiz_file);


    char* command = GetNewDotCmd(file_counter);

    system(command);
    free(command);

    FillLogFile(image_file_name, tree, file_counter);

    free(image_file_name);
    file_counter++;
}

void PrintBazeNode(FILE* graphiz_file,  Tree* tree)
{
    assert(tree);
    assert(graphiz_file);

    fprintf(graphiz_file,
            "node%p "
            "[shape = Mrecord, "
            "style = filled, "
            "fillcolor = \"#DD7538\", "
            "color = \"#00000\", "
            "label = \" {PTR: %p | SIZE: %llu | ROOT: %p} \"]",
            tree, tree, tree->size, tree->root);

}

void PrintBazeEdge(FILE* graphiz_file,  Tree* tree)
{
    assert(tree);
    assert(graphiz_file);

    fprintf(graphiz_file, "node%p -> { node%p } [dir = both]\n",
                           tree, tree->root);
}

void PrintTree( Node* node, FILE* graphiz_file)
{
    assert(node);
    assert(graphiz_file);

    //printf("(");

    PrintGraphizNode(graphiz_file, node);
    PrintGraphizEdge(graphiz_file, node);

    if (node->left){
        //printf("PTR: %p   PTR_LEFT: %p\n  ",node,  node->left);
        PrintTree(node->left, graphiz_file);
    }

    //printf("%d", GetData(node));

    if (node->right)
        PrintTree(node->right, graphiz_file);

    //printf(")");

}

void PrintGraphizEdge(FILE* graphiz_file,  Node* node)
{
    assert(node);
    assert(graphiz_file);

    if (node->left != NULL && node->right != NULL)
    {
        fprintf(graphiz_file,
                "node%p -> node%p "
                "[dir = both tailport=sw]\n",
                node, node->left);

        fprintf(graphiz_file,
                "node%p -> node%p "
                "[dir = both tailport=se]\n",
                node, node->right);
    }
}

void PrintGraphizNode(FILE* graphiz_file,  Node* node)
{
    assert(node);
    assert(graphiz_file);

    fprintf(graphiz_file,
            "node%p "
            "[shape = Mrecord, "
            "style = filled, "
            "fillcolor = \"#8ABAD3\", "
            "color = \"#00000\", "
            "label = \" {PARANT_PTR: %p| PTR: %p | "
            "TYPE: %s | ",
            node, node->parent, node, GetNodeTypeName(node));

    if (node->type == NUM)
        fprintf(graphiz_file, "VALUE: %lf | ",
                              node->value.num);

    else
        fprintf(graphiz_file, "VALUE: %s  | ",
                              GetNodeValueName(node));

    if (node->left != NULL && node->right != NULL)
        fprintf(graphiz_file,
                "{LEFT \\n PTR: %p | "
                " RIGHT \\n PTR: %p}} \" ]\n",
                node->left,
                node->right);
    else
        fprintf(graphiz_file, "} \" ]\n");

}

void FillLogFile(char* image_file_name,  Tree* tree, int file_counter)
{
    assert(image_file_name != NULL);
    assert(tree != NULL);

    const int PICTURE_WIDTH = 2000;

    fprintf(log_file, "\n\n<img src=image%d.png width=%dpx>\n\n",
                       file_counter, PICTURE_WIDTH);

    fprintf(log_file, "--------------------------------------------------------------------------------------------------------------------------------------------\n\n");

    fflush(log_file);
}

const char* GetNodeTypeName(Node* node)
{
    assert(node);

    switch(node->type)
    {
        case OP: return "OP";
        case VAR: return "VAR";
        case NUM: return "NUM";
        default: return NULL;
    }

    return NULL;
}

const char* GetNodeValueName(Node* node)
{
    assert(node);

    switch(node->type)
    {
        case OP:
            switch(node->value.op)
            {
                case ADD: return "ADD";
                case SUB: return "SUB";
                case MUL: return "MUL";
                case DIV: return "DIV";
                default : return NULL;
            }

        case VAR:
            return node->value.var.var_name;

        case NUM:
        default: return NULL;
    }

    return NULL;
}

static char* GetNewDotCmd(int file_counter)
{
    char str_command[100] = "";

    snprintf(str_command, sizeof(str_command),
            "dot -Tpng image%d.txt -o image%d.png",
             file_counter, file_counter);

    return strdup(str_command);
}

static char* GetNewImageFileName(int file_counter)
{
    char str_file_counter[100] = "";

    snprintf(str_file_counter, sizeof(str_file_counter),
             "image%d.txt", file_counter);

    return strdup(str_file_counter);
}


