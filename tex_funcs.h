#ifndef TEX_FUNCS
#define TEX_FUNCS

extern const char* tex_file_name;

void LatexInit();

void CloseLaTex();

char* GetTexCmd();

void PrintMessageInLaTex(const char* message, ...);

#endif
