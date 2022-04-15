#ifndef LEXINIT
#define LEXINIT

typedef union lexem
{
    bracket_t              brac;
    operator_t             oper;
    function_t             func;
    var_t                   var;
    data_t                 data;
} lexem;

typedef struct lexem_t
{
    kind_t                 kind;
    lexem                  lexm;
} lexem_t;

typedef struct lexem_array_t
{
    lexem_t             *lexems;
    unsigned  capacity, current;
} lexem_array_t;

#endif
