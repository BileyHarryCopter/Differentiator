#ifndef DUMP
#define DUMP

#include "../includes/standarts.h"
#include "../includes/constant.h"
#include "../includes/parctypes.h"
#include "../includes/lextypes.h"

int GraphDump (node_t *tree, modes mode);
int NodeGraphLink (node_t *top, FILE *file);
int DryNode (node_t *top, FILE *file, modes mode);
int NodeGraphInit (node_t *top, FILE *file, modes mode);

#endif
