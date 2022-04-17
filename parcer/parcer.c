#include "parcer.h"
#include "../includes/lexinit.h"
#include "../includes/parcinit.h"
#include "../service/service.h"
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                    //  REALIZATION OF TREE SERVICES    //
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int TreePrintIn (node_t *top, FILE * file)
{
    assert (top);
    assert (file);

    if (!top->left && !top->right)
    {
        if (top->ndata.kind == NUMBER)
            fprintf (file, "NUMBER: %lf ", top->ndata.content.data);
        else if (top->ndata.kind == VARIABLE)
            fprintf (file, "VARIABLE: %s ", top->ndata.content.var);
    }
    if (top->ndata.kind == OPERATOR)
        fprintf (file, "OPERATOR: %c ", top->ndata.content.token);
    else if (top->ndata.kind == FUNCTION)
        fprintf (file, "FUNCTION: %s ", function_names[top->ndata.content.token]);

    if (top->left)
    {
        if (TreePrintIn (top->left, file) == NO_ERROR && top->right)
            return TreePrintIn (top->right, file);
    }
    if (top->right)
    {
        if (TreePrintIn (top->right, file) == NO_ERROR && top->left)
            return TreePrintIn (top->left, file);
    }

    return NO_ERROR;
}

int TreePrint (node_t *top, const char *output_name)
{
    assert (top);
    assert (output_name);

    FILE * file = FileOpen (output_name, "w");

    if (TreePrintIn (top, file) == NO_ERROR)
    {
        FileClose (file);
        return NO_ERROR;
    }

    return ERROR;
}

int TreeDelete (node_t *top)
{
    if (top->left && !top->right)
        TreeDelete (top->left);
    if (!top->left && top->right)
        TreeDelete (top->right);
    if (top->left && top->right && TreeDelete (top->left) == NO_ERROR)
        TreeDelete (top->right);
    if (top->ndata.kind == VARIABLE)
        free (top->ndata.content.var);
    free (top);
    return NO_ERROR;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                    //  REALIZATION OF PARCER FUNCTION  //
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
node_t * NodeCtor (kind_t kind, ...)
{
    va_list args;
    int token;
    data_t numb;
    var_t var = NULL;

    node_t * node = (node_t *) calloc (1, sizeof (node_t));
    // printf ("address: %p\n", node);
    va_start (args, kind);

    switch (kind)
    {
        case VARIABLE:
            var = va_arg (args, var_t);
            assert (var);
            node->ndata.kind         = VARIABLE;
            node->ndata.content.var  = (var_t) calloc (strlen (var), sizeof (char));
            node->ndata.content.var  = memmove (node->ndata.content.var, var, strlen (var));
            // printf ("address: %p\n", node->ndata.content.var);
            break;
        case NUMBER:
            node->ndata.kind         = NUMBER;
            node->ndata.content.data = va_arg (args, data_t);
            break;
        case OPERATOR:
            node->ndata.kind = OPERATOR;
            node->ndata.content.token = va_arg (args, int);
            break;
        case FUNCTION:
            node->ndata.kind = FUNCTION;
            node->ndata.content.token = va_arg (args, int);
            break;
        default:
            printf ("Some errors on %d\n", __LINE__);
            return NULL;
    }

    return node;
}

node_t * Build_Syntax_Tree (lexem_array_t *lexus)
{
    assert (lexus);
    node_t *top = NULL;

    //  индексирование в массиве лексем будет происходить по переменной size  //
    lexus->capacity = lexus->current;
    lexus->current  = 0;
    //  индексирование в массиве лексем будет происходить по переменной size  //

    top = Parce_Expr (lexus);

    return (top == NULL) ? NULL : top;
}

node_t *Parce_Expr (lexem_array_t *lexus)
{
    assert (lexus);
    int operator = 0;
    node_t *lbranch = Parce_Mult (lexus);
    node_t *rbranch = NULL;
    node_t *cbranch = NULL;

    while (lexus->lexems[lexus->current].kind == OPERATOR &&
          (lexus->lexems[lexus->current].lexm.oper == ADD || lexus->lexems[lexus->current].lexm.oper == SUB))
    {
        operator = lexus->lexems[lexus->current].lexm.oper;
        lexus->current++;

        rbranch = Parce_Mult (lexus);
        if (rbranch == NULL)
            return NULL;

        cbranch        = NodeCtor (OPERATOR, operator);
        cbranch->left  = lbranch;
        cbranch->right = rbranch;
        lbranch        = cbranch;
    }

    return lbranch;
}

node_t *Parce_Mult (lexem_array_t *lexus)
{
    assert (lexus);
    int operator = 0;
    node_t *lbranch = Parce_Degr (lexus);
    node_t *rbranch = NULL;
    node_t *cbranch = NULL;

    while (lexus->lexems[lexus->current].kind == OPERATOR &&
          (lexus->lexems[lexus->current].lexm.oper == MUL || lexus->lexems[lexus->current].lexm.oper == DIV))
    {
        operator = lexus->lexems[lexus->current].lexm.oper;
        lexus->current++;

        rbranch = Parce_Degr (lexus);
        if (rbranch == NULL)
            return NULL;

        cbranch        = NodeCtor (OPERATOR, operator);
        cbranch->left  = lbranch;
        cbranch->right = rbranch;
        lbranch        = cbranch;
    }

    return lbranch;
}

node_t *Parce_Degr (lexem_array_t *lexus)
{
    assert (lexus);
    int operator = 0;
    node_t *lbranch = Parce_Term (lexus);
    node_t *rbranch = NULL;
    node_t *cbranch = NULL;

    while (lexus->lexems[lexus->current].kind == OPERATOR && lexus->lexems[lexus->current].lexm.oper == DEG)
    {
        operator = lexus->lexems[lexus->current].lexm.oper;
        lexus->current++;

        rbranch = Parce_Term (lexus);
        if (rbranch == NULL)
            return NULL;

        cbranch        = NodeCtor (OPERATOR, operator);
        cbranch->left  = lbranch;
        cbranch->right = rbranch;
        lbranch        = cbranch;
    }

    return lbranch;
}

node_t *Parce_Term (lexem_array_t *lexus)
{
    assert (lexus);

    if (lexus->lexems[lexus->current].kind == BRACKET &&
       (lexus->lexems[lexus->current].lexm.brac == LRBRACE || lexus->lexems[lexus->current].lexm.brac == LSBRACE))
    {
        lexus->current++;
        return ParceBrac (lexus);
    }
    if (lexus->lexems[lexus->current].kind == FUNCTION)
    {
        node_t *tbranch = NodeCtor (FUNCTION, lexus->lexems[lexus->current].lexm.func);
        lexus->current++;
        if (lexus->lexems[lexus->current].kind == BRACKET &&
           (lexus->lexems[lexus->current].lexm.brac == LRBRACE || lexus->lexems[lexus->current].lexm.brac == LSBRACE))
        {
            lexus->current++;
            tbranch->right = ParceBrac (lexus);
            return tbranch;
        }
    }
    if (lexus->lexems[lexus->current].kind == NUMBER)
    {
        return ParceNumb (lexus);
    }
    if (lexus->lexems[lexus->current].kind == VARIABLE)
    {
        return ParceVar (lexus);
    }
    return NULL;
}

node_t *ParceBrac (lexem_array_t *lexus)
{
    assert (lexus);
    node_t *tbranch = NULL;

    tbranch = Parce_Expr (lexus);
    if (lexus->lexems[lexus->current].lexm.brac == RRBRACE || lexus->lexems[lexus->current].lexm.brac == RSBRACE)
    {
        lexus->current++;
        return tbranch;
    }

    return NULL;
}

node_t *ParceNumb (lexem_array_t *lexus)
{
    assert (lexus);
    node_t *tbranch = NULL;
    data_t data = lexus->lexems[lexus->current].lexm.data;


    tbranch = NodeCtor (NUMBER, data);


    lexus->current++;
    return tbranch;
}

node_t *ParceVar (lexem_array_t *lexus)
{
    assert (lexus);
    node_t *tbranch = NULL;
    var_t var  = lexus->lexems[lexus->current].lexm.var;


    tbranch = NodeCtor (VARIABLE, var);


    lexus->current++;
    return tbranch;
}
