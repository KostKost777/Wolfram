#include<TXLib.h>

#include "wolfram_funcs.h"
#include "wolfram_dump_funcs.h"
#include "new_read_wolfram_database.h"
#include "common_funcs.h"

const char* tex_file_name = "wolfram_res.tex";

void TreeDump(Tree* tree)
{
    assert(tree);

    fprintf(log_file, "<h3>SIZE: %llu\n</h3>", tree->size);

    static int file_counter = 0;
    char* image_file_name = GetNewImageFileName(file_counter);

    //printf("FILE_NAME: %s\n", image_file_name);

    FILE* tex_file = fopen(tex_file_name, "a");
    FILE* graphiz_file = fopen(image_file_name, "w");

    fprintf(graphiz_file, "digraph {\n"
                          "graph [charset=\"utf-8\"]"
                          "rankdir = HR;\n"
                          "splines = true\n");

    PrintBazeNode(graphiz_file, tree);

    if (tree->size > 0) {
        //printf("DUMP_TREE_ROOT: %p\n", tree->root);
        PrintBazeEdge(graphiz_file, tree);
        PrintTree(tree->root, graphiz_file);

        PrintTexTree(tree->root);
    }

    fprintf(graphiz_file, "}");

    fclose(graphiz_file);
    fclose(tex_file);

    char* dot_cmd = GetNewDotCmd(file_counter);

    system(dot_cmd);
    free(dot_cmd);

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
            "label = \" {PTR: %p | SIZE: %llu | ROOT: %p} \"]\n",
            tree, tree, tree->size, tree->root);

}

void PrintBazeEdge(FILE* graphiz_file,  Tree* tree)
{
    assert(tree);
    assert(graphiz_file);

    fprintf(graphiz_file, "node%p -> { node%p } [dir = both]\n",
                           tree, tree->root);
}

void PrintTexNode(Node* node, FILE* tex_file)
{
    assert(tex_file);
    assert(node);

    switch(node->type)
    {
        case NUM:
            fprintf(tex_file, "%g", node->value.num);
            break;

        case VAR:
            fprintf(tex_file, "%s", node->value.var.var_name);
            break;

        case OP:
            switch(node->value.op)
            {
                case SUB:
                case ADD: PrintLaTex_ADD_SUB(node, tex_file);
                          break;

                case MUL: PrintLaTex_MUL(node, tex_file);
                          break;

                case DIV: PrintLaTex_DIV(node, tex_file);
                          break;

                case POW: PrintLaTex_POW(node, tex_file);
                          break;

                case EXP: PrintLaTex_EXP(node, tex_file);
                          break;

                case LN: PrintOpByNameInLaTex(node, tex_file, "ln");
                         break;

                case LOG: PrintLaTex_LOG(node, tex_file);
                          break;

                case SIN: PrintOpByNameInLaTex(node, tex_file, "sin");
                          break;

                case COS: PrintOpByNameInLaTex(node, tex_file, "cos");
                          break;

                case TG: PrintOpByNameInLaTex(node, tex_file, "tan");
                         break;

                case CTG: PrintOpByNameInLaTex(node, tex_file, "cot");
                          break;

                case SH: PrintOpByNameInLaTex(node, tex_file, "sinh");
                         break;

                case CH: PrintOpByNameInLaTex(node, tex_file, "cosh");
                         break;

                case TH: PrintOpByNameInLaTex(node, tex_file, "tanh");
                         break;

                case CTH: PrintOpByNameInLaTex(node, tex_file, "coth");
                          break;

                case ARCSIN: PrintOpByNameInLaTex(node, tex_file, "arcsin");
                             break;

                case ARCCOS: PrintOpByNameInLaTex(node, tex_file, "arccos");
                             break;

                case ARCTG: PrintOpByNameInLaTex(node, tex_file, "arctan");
                            break;

                case ARCCTG: PrintOpByNameInLaTex(node, tex_file, "arccot");
                             break;

                case FACT: PrintLaTex_FACT(node, tex_file);
                           break;

                default: break;
            }

        default: break;
    }
}


void PrintTree(Node* node, FILE* graphiz_file)
{
    assert(node);
    assert(graphiz_file);

    PrintGraphizNode(graphiz_file, node);
    PrintGraphizEdge(graphiz_file, node);

    if (node->left){
        //printf("PTR: %p   PTR_LEFT: %p\n  ",node,  node->left);
        PrintTree(node->left, graphiz_file);
    }

    if (node->right)
        PrintTree(node->right, graphiz_file);

}

void PrintGraphizEdge(FILE* graphiz_file,  Node* node)
{
    assert(node);
    assert(graphiz_file);

    if (node->left != NULL)
    {
        fprintf(graphiz_file,
                "node%p -> node%p "
                "[dir = both tailport=sw]\n",
                node, node->left);
    }

    if (node->right != NULL)
    {
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

    #ifndef DEBUG

    switch(node->type)
    {
        case OP:

            fprintf(graphiz_file,
                    "node%p "
                    "[shape = Mrecord, "
                    "style = filled, "
                    "fillcolor = \"#8ABAD3\", "
                    "color = \"#00000\", "
                    "label = \" {%s} \" ]\n",
                    node, GetNodeValueName(node));
            break;

        case VAR:

            fprintf(graphiz_file,
                    "node%p "
                    "[shape = octagon,"
                    "style = filled, "
                    "fillcolor = \"#29f206ff\", "
                    "color = \"#00000\", "
                    "label = \" %s \" ]\n",
                    node, GetNodeValueName(node));
            break;

        case NUM:

            fprintf(graphiz_file,
                    "node%p "
                    "[shape = ellipse,"
                    "style = filled, "
                    "fillcolor = \"#f7cb3dff\", "
                    "color = \"#00000\", "
                    "label = \" %g \" ]\n",
                    node, node->value.num);
            break;

        default: break;

    }

    #endif

    #ifdef DEBUG

    fprintf(graphiz_file,
            "node%p "
            "[shape = Mrecord, "
            "style = filled, "
            "fillcolor = \"#8ABAD3\", "
            "color = \"#00000\", "
            "label = \" {PARANT_PTR: %p| PTR: %p | "
            "TYPE: %s",
            node, node->parent, node, GetNodeTypeName(node));

    if (node->type == OP)
        fprintf(graphiz_file, "| ARGS: %d",
                              node->args);

    if (node->type == NUM)
        fprintf(graphiz_file, "| VALUE: %lf",
                              node->value.num);

    else
        fprintf(graphiz_file, "| VALUE: %s",
                              GetNodeValueName(node));

    if (node->left != NULL && node->right != NULL)
        fprintf(graphiz_file,
                " | {LEFT \\n PTR: %p | "
                " RIGHT \\n PTR: %p}} \" ]\n",
                node->left,
                node->right);
    else
        fprintf(graphiz_file, "} \" ]\n");

    #endif

}

void FillLogFile(char* image_file_name,  Tree* tree, int file_counter)
{
    assert(image_file_name != NULL);
    assert(tree != NULL);

    const int PICTURE_WIDTH = 2000;

    fprintf(log_file, "\n\n<img src=Images\\image%d.svg width=%dpx>\n\n",
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
            for (size_t i = 0; i < NUM_OF_OP; ++i)
            {
                if (all_op[i].op == node->value.op)
                    return all_op[i].name;
            }
            break;

        case VAR:
            return node->value.var.var_name;

        case NUM:
        default: return NULL;
    }

    return NULL;
}

Node* PrintTexTree(Node* node)
{
    assert(node);

    FILE* tex_file = fopen(tex_file_name, "a");

    fprintf(tex_file, "\\begin{dmath*}");
    PrintTexNode(node, tex_file);
    fprintf(tex_file, "\\end{dmath*}");

    fprintf(tex_file, "\n\\vspace{1cm}\n");

    fclose(tex_file);

    return node;
}

static char* GetNewDotCmd(int file_counter)
{
    char str_command[100] = "";

    snprintf(str_command, sizeof(str_command),
            "dot -Tsvg Images\\image%d.txt -o Images\\image%d.svg",
             file_counter, file_counter);

    return strdup(str_command);
}

static char* GetNewImageFileName(int file_counter)
{
    char str_file_counter[100] = "";

    snprintf(str_file_counter, sizeof(str_file_counter),
             "Images\\image%d.txt", file_counter);

    return strdup(str_file_counter);
}

void PrintLaTex_ADD_SUB(Node* node, FILE* tex_file)
{
    assert(node);
    assert(tex_file);

    char sign = '\0';
    if (node->value.op == ADD) sign = '+';
    else sign = '-';

    if (   node->parent != NULL
        && node->parent->value.op == MUL)
    {
        fprintf(tex_file, "\\left(");
        PrintTexNode(node->left, tex_file);
        fprintf(tex_file, " %c ", sign);
        PrintTexNode(node->right, tex_file);
        fprintf(tex_file, "\\right)");
        return;
    }

    PrintTexNode(node->left, tex_file);
    fprintf(tex_file, " %c ", sign);
    PrintTexNode(node->right, tex_file);
}

void PrintLaTex_MUL(Node* node, FILE* tex_file)
{
    assert(node);
    assert(tex_file);

    PrintTexNode(node->left, tex_file);
    fprintf(tex_file, " \\cdot ");
    PrintTexNode(node->right, tex_file);
}

void PrintLaTex_DIV(Node* node, FILE* tex_file)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\frac{");
    PrintTexNode(node->left, tex_file);
    fprintf(tex_file, "}");

    fprintf(tex_file, "{");
    PrintTexNode(node->right, tex_file);
    fprintf(tex_file, "}");
}

void PrintLaTex_POW(Node* node, FILE* tex_file)
{
    assert(node);
    assert(tex_file);

    if (   node->left->value.op == ADD
        || node->left->value.op == SUB)
    {
        fprintf(tex_file, "\\left(");
        PrintTexNode(node->left, tex_file);
        fprintf(tex_file, "\\right)");
    }

    else
        PrintTexNode(node->left, tex_file);

    fprintf(tex_file, " ^ {");
    PrintTexNode(node->right, tex_file);
    fprintf(tex_file, "}");
}

void PrintLaTex_EXP(Node* node, FILE* tex_file)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "e ^ {");
    PrintTexNode(node->right, tex_file);
    fprintf(tex_file, "}");
}

void PrintOpByNameInLaTex(Node* node, FILE* tex_file, const char* name)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\%s {", name);

    fprintf(tex_file, "\\left(");
    PrintTexNode(node->right, tex_file);
    fprintf(tex_file, "\\right)");

    fprintf(tex_file, "}");
}

void PrintLaTex_FACT(Node* node, FILE* tex_file)
{
    assert(node);
    assert(tex_file);

    PrintTexNode(node->right, tex_file);
    fprintf(tex_file, "!");
}

void PrintLaTex_LOG(Node* node, FILE* tex_file)
{
    assert(node);
    assert(tex_file);

    fprintf(tex_file, "\\log_{");
    PrintTexNode(node->left, tex_file);
    fprintf(tex_file, "}");

    fprintf(tex_file, " \\left(");
    PrintTexNode(node->right, tex_file);
    fprintf(tex_file, " \\right)");
}



