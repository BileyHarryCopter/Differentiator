#ifndef DUMP
#define DUMP

#include "../includes/standarts.h"
#include "../includes/constant.h"
#include "../includes/parctypes.h"
#include "../includes/lextypes.h"

int DryNode (node_t *top, FILE *file);
int NodeGraphInit (node_t *top, FILE *file);
int GraphDump (node_t *tree);

#endif
