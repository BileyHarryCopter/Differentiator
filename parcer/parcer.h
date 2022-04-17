#ifndef PARCER
#define PARCER

#include "../includes/standarts.h"
#include "../includes/lextypes.h"
#include "../includes/constant.h"
#include "../includes/parctypes.h"

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
