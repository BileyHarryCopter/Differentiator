#ifndef LEXER
#define LEXER

#include "../includes/standarts.h"
#include "../includes/constant.h"
#include "../includes/lextypes.h"

//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
lexem_array_t *LexsInit (unsigned capacity);
int LexsResz (lexem_array_t *lexus);
int LexsDelete (lexem_array_t *lexus);
int LexsInsert (lexem_array_t *lexus, kind_t kind, ...);
int LexsPrint (lexem_array_t *lexus, const char * output_name);
char * StrCtor (char first, FILE *file);
int StrDelete (char *str);
int IstrCmp (char *str1, const char *str2);
int IsFunc (char *str);
int LexsFill (lexem_array_t *lexus, const char *file_name);
#endif
