#include "difftorr.h"
#include "service/service.h"
#include "includes/parcinit.h"
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
        //  правила дифференцирования для операторов

        //  for addition
        if (top->ndata.content.token == ADD || top->ndata.content.token == SUB)
        {
            top->left  = DiffTree (top->left);
            top->right = DiffTree (top->right);
        }
        //  for multiplate
        if (top->ndata.content.token == MUL)
        {
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
        }

        //  for division
        if (top->ndata.content.token == DIV)
        {
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
        }

    }
    if (top->ndata.kind == FUNCTION && top-> left && top->right)
    {
        //  правила дифференцирования для разных функций + дифференцирование сложной функции

        //  the loopa
    }

    return top;
}
