#ifndef PARCINIT
#define PARCINIT

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

#endif
