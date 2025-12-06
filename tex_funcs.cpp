#include <TXLib.h>
#include <errno.h>

#include "tex_funcs.h"

void LatexInit()
{
    FILE* tex_file = fopen(tex_file_name, "w");

    if (tex_file == NULL)
    {
        perror("Ошибка открытия файла");
        return;
    }

    fclose(tex_file);

    tex_file = fopen(tex_file_name, "a");

    fprintf(tex_file, "\\documentclass{article}\n"
                       "\\usepackage{graphicx}\n\n"
                       "\\usepackage{times}\n"
                       "\\usepackage[utf8]{inputenc}\n"
                       "\\usepackage[russian]{babel}\n"
                       "\\usepackage{breqn}\n"


                        "\\title{Вольфрам}\n"
                        "\\author{Zinawe}\n"
                        "\\date{November 2025}\n\n"


                        "\\begin{document}\n"
                        "\\maketitle\n");

    fclose(tex_file);
}

void CloseLaTex()
{
    FILE* tex_file = fopen(tex_file_name, "a");

    fprintf(tex_file, "\n\\end{document}");

    fclose(tex_file);

    char* tex_cmd = GetTexCmd();

    system(tex_cmd);
    free(tex_cmd);
}

void PrintMessageInLaTex(const char* message, ...)
{
    assert(message);

    FILE* tex_file = fopen(tex_file_name, "a");

    va_list args = {};
    va_start(args, message);

    fprintf(tex_file, "\\centering");
    fprintf(tex_file, "{\\large");
    vfprintf(tex_file, message, args);
    fprintf(tex_file, "}\n\\vspace{1cm}\n");

    va_end(args);

    fclose(tex_file);
}

char* GetTexCmd()
{
    char str_command[100] = "";

    snprintf(str_command, sizeof(str_command),
            "pdflatex %s", tex_file_name);

    return strdup(str_command);
}
