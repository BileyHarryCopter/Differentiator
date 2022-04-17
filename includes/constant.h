#ifndef CONSTANTS
#define CONSTANTS

//  main constants  //
#define KRIT_KF 0.6 //
#define ENCR_KF 2   //
#define CAP_INIT 20 //
#define STR_INIT 20 //
#define PI 3.1415   //
#define E  2.7182   //
//  main constants  //

typedef char*  var_t;

typedef double data_t;

typedef enum modes      {NO_ERROR = 0, ERROR, OPEN, JUST, DEBUG} modes;

typedef enum kind_t     {BRACKET = 1, NUMBER, VARIABLE, OPERATOR, FUNCTION} kind_t;

typedef enum bracket_t  {LRBRACE = 40, RRBRACE = 41, LSBRACE = 91, RSBRACE = 93} bracket_t;

typedef enum operator_t {ADD = 43, SUB = 45, MUL = 42, DIV = 47, DEG = 94} operator_t;

typedef enum function_t {SIN = 0, COS, TAN, COT, ARCSIN, ARCCOS, ARCTAN, ARCCOT, SINH, COSH, TANH, COTH, LN, FUNC_NUMB} function_t;

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
    "COTH",
    "LN"
};

#endif
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
