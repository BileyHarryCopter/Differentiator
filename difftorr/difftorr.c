#include "difftorr.h"
#include "../service/service.h"
#include "../includes/parcinit.h"
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                //  REALIZATION OF DIFFIRINTIATOR FUNCTIONS  //
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//

                            //  напишем всё в одной большой функции по тупому,
                                    //  а потом разобъём ебало об маленькие =)
node_t *CopyBranch (node_t *top)
{
    node_t *copy_top = (node_t *) calloc (1, sizeof (node_t));
    copy_top = (node_t *) memmove (copy_top, top, sizeof (node_t));

    //  copy of variable
    if (top->ndata.kind == VARIABLE)
    {
        copy_top->ndata.kind = VARIABLE;
        var_t variable = (var_t) calloc (strlen (top->ndata.content.var), sizeof (char));
        variable       = (var_t) memmove (variable, top->ndata.content.var, strlen (top->ndata.content.var));
        copy_top->ndata.content.var = variable;
    }

    if (top->left)
        copy_top->left  = CopyBranch (top->left);
    if (top->right)
        copy_top->right = CopyBranch (top->right);

    return copy_top;
}

//  for simple algebraical expressions
int DiffAddSub (node_t *top)
{
    if (!top)
        return ERROR;

    top->left  = DiffTree (top->left);
    top->right = DiffTree (top->right);

    return NO_ERROR;
}
int DiffMul (node_t *top)
{
    if (!top)
        return ERROR;

    node_t *lbranch = NodeCtor (OPERATOR, MUL);
    node_t *rbranch = NodeCtor (OPERATOR, MUL);

    //  copy left branch
    node_t *copy_lbranch = CopyBranch (top->left);

    //  copy right branch
    node_t *copy_rbranch = CopyBranch (top->right);


    lbranch->left = DiffTree (top->left);
    lbranch->right = copy_rbranch;

    rbranch->left = copy_lbranch;
    rbranch->right = DiffTree (top->right);

    top->ndata.content.token = ADD;
    top->left  = lbranch;
    top->right = rbranch;

    return NO_ERROR;
}
int DiffDiv (node_t *top)
{
    if (!top)
        return ERROR;

    node_t *lbranch  = NodeCtor (OPERATOR, SUB);
    node_t *rbranch  = NodeCtor (OPERATOR, MUL);

    //  copy left branch
    node_t *copy_lbranch = CopyBranch (top->left);

    //  copy right branch
    node_t *copy_rbranch = CopyBranch (top->right);

    //  copy left branch for rbranch
    node_t *lcopy_rbranch = CopyBranch (top->right);

    //  copy left branch for rbranch
    node_t *rcopy_rbranch = CopyBranch (top->right);

    lbranch->left  = NodeCtor (OPERATOR, MUL);
    lbranch->right = NodeCtor (OPERATOR, MUL);


    lbranch->left->left   = DiffTree (top->left);
    lbranch->left->right  = copy_rbranch;


    lbranch->right->left  = copy_lbranch;
    lbranch->right->right = DiffTree (top->right);

    rbranch->left  = lcopy_rbranch;
    rbranch->right = rcopy_rbranch;


    top->ndata.content.token = DIV;
    top->left  = lbranch;
    top->right = rbranch;

    return NO_ERROR;
}

//  for simple trigonometryc functions
int DiffSin (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind     = OPERATOR;
    top->ndata.content.token = MUL;

    top->left = NodeCtor (FUNCTION, COS);
    top->left->right = CopyBranch (top->right);

    top->right = DiffTree (top->right);

    return NO_ERROR;
}
int DiffCos (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind = OPERATOR;
    top->ndata.content.token = MUL;

    top->left    = NodeCtor (NUMBER, -1.0);

    node_t *rmul = NodeCtor (OPERATOR, MUL);
    rmul->left   = NodeCtor (FUNCTION, SIN);
    rmul->left->right = CopyBranch (top->right);

    rmul->right = DiffTree (top->right);
    top->right  = rmul;

    return NO_ERROR;
}
int DiffTan (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind = OPERATOR;
    top->ndata.content.token = MUL;

    top->left                      = NodeCtor (OPERATOR, DIV);
    top->left->left                = NodeCtor (NUMBER, 1.0);
    top->left->right               = NodeCtor (OPERATOR, DEG);


    top->left->right->left         = NodeCtor (FUNCTION, SIN);
    top->left->right->left->right  = CopyBranch (top->right);
    top->left->right->right        = NodeCtor (NUMBER, 2.0);

    top->right                     = DiffTree (top->right);

    return NO_ERROR;
}
int DiffCot (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind = OPERATOR;
    top->ndata.content.token = MUL;

    top->left                      = NodeCtor (OPERATOR, DIV);
    top->left->left                = NodeCtor (NUMBER, -1.0);
    top->left->right               = NodeCtor (OPERATOR, DEG);


    top->left->right->left         = NodeCtor (FUNCTION, SIN);
    top->left->right->left->right  = CopyBranch (top->right);
    top->left->right->right        = NodeCtor (NUMBER, 2.0);

    top->right                     = DiffTree (top->right);

    return NO_ERROR;
}

//  for simple reversed trigonometryc functions
int DiffArcSin (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind = OPERATOR;
    top->ndata.content.token = MUL;

    top->left                            = NodeCtor (OPERATOR, DIV);
    top->left->left                      = NodeCtor (NUMBER, 1.0);
    top->left->right                     = NodeCtor (OPERATOR, DEG);

    top->left->right->left               = NodeCtor (OPERATOR, SUB);
    top->left->right->left->left         = NodeCtor (NUMBER, 1.0);
    top->left->right->left->right        = NodeCtor (OPERATOR, DEG);
    top->left->right->left->right->left  = CopyBranch (top->right);
    top->left->right->left->right->right = NodeCtor (NUMBER, 2.0);

    top->left->right->right              = NodeCtor (NUMBER, 0.5);

    top->right                           = DiffTree (top->right);

    return NO_ERROR;
}
int DiffArcCos (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind = OPERATOR;
    top->ndata.content.token = MUL;

    top->left                            = NodeCtor (OPERATOR, DIV);
    top->left->left                      = NodeCtor (NUMBER, -1.0);
    top->left->right                     = NodeCtor (OPERATOR, DEG);

    top->left->right->left               = NodeCtor (OPERATOR, SUB);
    top->left->right->left->left         = NodeCtor (NUMBER, 1.0);
    top->left->right->left->right        = NodeCtor (OPERATOR, DEG);
    top->left->right->left->right->left  = CopyBranch (top->right);
    top->left->right->left->right->right = NodeCtor (NUMBER, 2.0);

    top->left->right->right              = NodeCtor (NUMBER, 0.5);

    top->right                           = DiffTree (top->right);

    return NO_ERROR;
}
int DiffArcTan (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind = OPERATOR;
    top->ndata.content.token = MUL;

    top->left                      = NodeCtor (OPERATOR, DIV);
    top->left->left                = NodeCtor (NUMBER, 1.0);
    top->left->right               = NodeCtor (OPERATOR, ADD);
    top->left->right->left         = NodeCtor (NUMBER, 1.0);
    top->left->right->right        = NodeCtor (OPERATOR, DEG);
    top->left->right->right->left  = CopyBranch (top->right);
    top->left->right->right->right = NodeCtor (NUMBER, 2.0);

    top->right                     = DiffTree (top->right);

    return NO_ERROR;
}
int DiffArcCot (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind = OPERATOR;
    top->ndata.content.token = MUL;

    top->left                      = NodeCtor (OPERATOR, DIV);
    top->left->left                = NodeCtor (NUMBER, -1.0);
    top->left->right               = NodeCtor (OPERATOR, ADD);
    top->left->right->left         = NodeCtor (NUMBER, 1.0);
    top->left->right->right        = NodeCtor (OPERATOR, DEG);
    top->left->right->right->left  = CopyBranch (top->right);
    top->left->right->right->right = NodeCtor (NUMBER, 2.0);

    top->right                     = DiffTree (top->right);

    return NO_ERROR;
}

//  for simple hyperbolic functions
int DiffSinh (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind = OPERATOR;
    top->ndata.content.token = MUL;

    top->left = NodeCtor (FUNCTION, COSH);
    top->left->right = CopyBranch (top->right);

    top->right = DiffTree (top->right);

    return NO_ERROR;
}
int DiffCosh (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind = OPERATOR;
    top->ndata.content.token = MUL;

    top->left = NodeCtor (FUNCTION, SINH);
    top->left->right = CopyBranch (top->right);

    top->right = DiffTree (top->right);

    return NO_ERROR;
}
int DiffTanh (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind = OPERATOR;
    top->ndata.content.token = MUL;

    top->left                      = NodeCtor (OPERATOR, DIV);
    top->left->left                = NodeCtor (NUMBER, 1.0);
    top->left->right               = NodeCtor (OPERATOR, DEG);


    top->left->right->left         = NodeCtor (FUNCTION, COSH);
    top->left->right->left->right  = CopyBranch (top->right);
    top->left->right->right        = NodeCtor (NUMBER, 2.0);

    top->right                     = DiffTree (top->right);

    return NO_ERROR;
}
int DiffCoth (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind = OPERATOR;
    top->ndata.content.token = MUL;

    top->left                      = NodeCtor (OPERATOR, DIV);
    top->left->left                = NodeCtor (NUMBER, -1.0);
    top->left->right               = NodeCtor (OPERATOR, DEG);


    top->left->right->left         = NodeCtor (FUNCTION, SINH);
    top->left->right->left->right  = CopyBranch (top->right);
    top->left->right->right        = NodeCtor (NUMBER, 2.0);

    top->right                     = DiffTree (top->right);

    return NO_ERROR;
}

//  for exponential
int DiffLn (node_t *top)
{
    if (!top)
        return ERROR;

    top->ndata.kind = OPERATOR;
    top->ndata.content.token = MUL;

    top->left = NodeCtor (OPERATOR, DIV);
    top->left->left = NodeCtor (NUMBER, 1.0);
    top->left->right = CopyBranch (top->right);

    top->right = DiffTree (top->right);

    return NO_ERROR;
}

//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
node_t *DiffTree (node_t *top)
{
    if (!top->left && !top->right)
    {
        //  cases for: kind = VARIABLE || kind = NUMBER
        if (top->ndata.kind == NUMBER)
            top->ndata.content.data = 0.0;
        if (top->ndata.kind == VARIABLE)
        {
            top->ndata.kind = NUMBER;
            free (top->ndata.content.var);
            top->ndata.content.data = 1.0;
        }
    }
    if (top->ndata.kind == OPERATOR && top-> left && top->right)
    {
        //  for addition or subtraction
        if (top->ndata.content.token == ADD || top->ndata.content.token == SUB)
        {
            if (DiffAddSub (top) != NO_ERROR)
                return NULL;
        }
        //  for multiplication
        if (top->ndata.content.token == MUL)
        {
            if (DiffMul (top) != NO_ERROR)
                return NULL;
        }

        //  for division
        if (top->ndata.content.token == DIV)
        {
            if (DiffDiv (top) != NO_ERROR)
                return NULL;
        }

    }
    //  правила дифференцирования для разных функций + дифференцирование сложной функции
    if (top->ndata.kind == FUNCTION && top->right)
    {
        if (top->ndata.content.token == SIN)
        {
            if (DiffSin (top) != NO_ERROR)
                return NULL;
        }
        else if (top->ndata.content.token == COS)
        {
            if (DiffCos (top) != NO_ERROR)
                return NULL;
        }
        else if (top->ndata.content.token == TAN)
        {
            if (DiffTan (top) != NO_ERROR)
                return NULL;
        }
        else if (top->ndata.content.token == COT)
        {
            if (DiffCot (top) != NO_ERROR)
                return NULL;
        }
        else if (top->ndata.content.token == ARCSIN)
        {
            if (DiffArcSin (top) != NO_ERROR)
                return NULL;
        }
        else if (top->ndata.content.token == ARCCOS)
        {
            if (DiffArcCos (top) != NO_ERROR)
                return NULL;
        }
        else if (top->ndata.content.token == ARCTAN)
        {
            if (DiffArcTan (top) != NO_ERROR)
                return NULL;
        }
        else if (top->ndata.content.token == ARCCOT)
        {
            if (DiffArcTan (top) != NO_ERROR)
                return NULL;
        }
        else if (top->ndata.content.token == SINH)
        {
            if (DiffSinh (top) != NO_ERROR)
                return NULL;
        }
        else if (top->ndata.content.token == COSH)
        {
            if (DiffCosh (top) != NO_ERROR)
                return NULL;
        }
        else if (top->ndata.content.token == TANH)
        {
            if (DiffTanh (top) != NO_ERROR)
                return NULL;
        }
        else if (top->ndata.content.token == COTH)
        {
            if (DiffCoth (top) != NO_ERROR)
                return NULL;
        }
        else if (top->ndata.content.token == LN)
        {
            if (DiffLn (top) != NO_ERROR)
                return NULL;
        }
    }

    return top;
}
