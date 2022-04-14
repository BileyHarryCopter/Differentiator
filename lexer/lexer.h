#ifndef LEXER
#define LEXER

#include "../includes/standarts.h"

#define KRIT_KF 0.6
#define ENCR_KF 2
#define CAP_INIT 20
#define STR_INIT 20

typedef char*  var_t;
typedef double data_t;
#define PI 3.1415
#define E  2.7182

typedef enum errors     {NO_ERROR = 0, ERROR} errors;

typedef enum kind_t     {BRACKET = 1, NUMBER, VARIABLE, OPERATOR, FUNCTION} kind_t;

typedef enum bracket_t  {LRBRACE = 40, RRBRACE = 41, LSBRACE = 91, RSBRACE = 93} bracket_t;

typedef enum operator_t {ADD = 43, SUB = 45, MUL = 42, DIV = 47, DEG = 94} operator_t;

typedef enum function_t {SIN = 0, COS, TAN, COT, ARCSIN, ARCCOS, ARCTAN, ARCCOT, SINH, COSH, TANH, COTH, FUNC_NUMB} function_t;

static char *function_names[FUNC_NUMB] =
{
    "SIN",
    "COS",
    "TAN",
    "COT",
    "ARCSIN",
    "ARCCOS",
    "ARCTAN",
    "ARCCOT",
    "SINH",
    "COSH",
    "TANH",
    "COTH"
};

typedef union lexem
{
    bracket_t           brac;
    operator_t          oper;
    function_t          func;
    var_t                var;
    data_t              data;
} lexem;

typedef struct lexem_t
{
    kind_t              kind;
    lexem               lexm;
} lexem_t;

typedef struct lexem_array_t
{
    lexem_t          *lexems;
    unsigned  capacity, size;
} lexem_array_t;

//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
lexem_array_t *LexsInit (unsigned capacity);
int LexsResz (lexem_array_t *lexus);
int LexsDelete (lexem_array_t *lexus);
int LexsInsert (lexem_array_t *lexus, kind_t kind, ...);
int LexsPrint (lexem_array_t *lexus, const char * output_name);
FILE * FileOpen (const char *file_name, const char *mode);
int FileClose (FILE *file);
char * StrCtor (char first, FILE *file);
int StrDelete (char *str);
int IstrCmp (char *str1, const char *str2);
int IsFunc (char *str);
int LexsFill (lexem_array_t *lexus, const char *file_name);
#endif
