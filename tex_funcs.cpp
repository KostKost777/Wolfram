#include <TXLib.h>
#include <errno.h>

#include "wolfram_funcs.h"
#include "tex_funcs.h"
#include "common_funcs.h"
#include "wolfram_dump_funcs.h"

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

void PrintTaylor(double* taylor_coeffs, double x_point, size_t accuracy)
{
    assert(taylor_coeffs);
    FILE* tex_file = fopen(tex_file_name, "a");

    PrintMessageInLaTex("Разложение по Тейлору: ");

    fprintf(tex_file, "\\begin{dmath*}");

    bool is_first_member = true;

    for (size_t i = 0; i <= accuracy; ++i)
    {
        if (IsDoubleEqual(taylor_coeffs[i], 0))
            continue;

        double coeff = fabs(taylor_coeffs[i]) / fact((double)i);

        PrintTaylorSign(tex_file, taylor_coeffs[i], is_first_member);

        PrintTaylorMember(tex_file, coeff, x_point, i);

        is_first_member = false;
    }

    if (IsDoubleEqual(x_point, 0))
        fprintf(tex_file, " + o(x ^ %llu)", accuracy);
    else
        fprintf(tex_file, " + o((x - %lf) ^ %llu)", x_point, accuracy);

    fprintf(tex_file, "\\end{dmath*}");

    fprintf(tex_file, "\n\\vspace{1cm}\n");

    fclose(tex_file);
}

void PrintTaylorSign(FILE* tex_file, double coeff, bool is_first_member)
{
    assert(tex_file);

    if (IsDoubleBigger(0, coeff))
        fprintf(tex_file, " - ");

    else if (IsDoubleBigger(coeff, 0) && !is_first_member)
        fprintf(tex_file, " + ");
}

void PrintTaylorMember(FILE* tex_file, double coeff,
                       double x_point, size_t member)
{
    assert(tex_file);

    if (IsDoubleEqual(x_point, 0))
    {
        if (member > 1)
        {
            PrintNeutralElement(tex_file, coeff);
            fprintf(tex_file, "x ^ %llu", member);
        }

        else if (member == 1)
        {
            PrintNeutralElement(tex_file, coeff);
            fprintf(tex_file, "x");
        }

        else
            fprintf(tex_file, "%g", coeff);

        return;
    }

    if (member > 1)
    {
        PrintNeutralElement(tex_file, coeff);
        fprintf(tex_file, "(x - %g) ^ %llu", x_point, member);
    }

    else if (member == 1)
    {
        PrintNeutralElement(tex_file, coeff);
        fprintf(tex_file, "(x - %g)", x_point);
    }

    else
        fprintf(tex_file, "%g", coeff);
}

void PrintNeutralElement(FILE* tex_file, double coeff)
{
    assert(tex_file);

    if (IsDoubleEqual(coeff, 1))
        return;

    else if (IsDoubleEqual(coeff, -1))
        fprintf(tex_file, "-");

    else
        fprintf(tex_file, "%g \\cdot ", coeff);
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
