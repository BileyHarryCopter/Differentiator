#ifndef PARCER
#define PARCER

#include "../includes/standarts.h"
#include "../includes/lextypes.h"
#include "../includes/constant.h"

typedef union node_vd
{
    int             token;
    data_t           data;
    var_t             var;
} node_vd;

typedef struct node_data_t
{
    kind_t           kind;
    node_vd       content;
} node_data_t;

typedef struct node_t
{
    struct node_t   *left;
    struct node_t  *right;
    node_data_t     ndata;
} node_t;


//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int TreeDelete (node_t *top);
node_t * NodeCtor (kind_t kind, ...);
int TreePrintIn (node_t *top, FILE * file);
node_t * Build_Syntax_Tree (lexem_array_t *lexus);
int TreePrint (node_t *top, const char *output_name);

node_t *ParceVar (lexem_array_t *lexus);
node_t *ParceNumb (lexem_array_t *lexus);
node_t *ParceBrac (lexem_array_t *lexus);
node_t *Parce_Expr (lexem_array_t *lexus);
node_t *Parce_Degr (lexem_array_t *lexus);
node_t *Parce_Mult (lexem_array_t *lexus);
node_t *Parce_Term (lexem_array_t *lexus);
#endif
