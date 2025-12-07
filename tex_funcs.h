#ifndef TEX_FUNCS
#define TEX_FUNCS

#include "wolfram_funcs.h"

extern const char* tex_file_name;

void LatexInit();

void CloseLaTex();

char* GetTexCmd();

void PrintMessageInLaTex(const char* message, ...);

void PrintTaylor(double* taylor_coeffs, double x_point, size_t accuracy);

void PrintTaylorSign(FILE* tex_file, double coeff, bool is_first_member);

void PrintTaylorMember(FILE* tex_file, double coeff,
                       double x_point_, size_t member);

void PrintNeutralElement(FILE* tex_file, double coeff);

void PrintPictureInLatex(const char* name_of_graph_file);

#endif
