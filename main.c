#include "includes/standarts.h"
#include "includes/constant.h"
#include "lexer/lexer.h"
#include "parcer/parcer.h"
#include "dump/dump.h"
#include "difftorr/difftorr.h"

int main (void)
{
    node_t *tree = NULL;
    lexem_array_t *lexus = LexsInit (CAP_INIT);

    if (LexsFill (lexus, "logs/input.txt") == NO_ERROR)
    {
        LexsPrint (lexus, "logs/output.txt");

        tree = Build_Syntax_Tree (lexus);
        //  GraphDump (tree, JUST);
        //  TreePrint (tree, "logs/outtree1.txt");

        tree = DiffTree (tree);
        GraphDump (tree, DEBUG);
        GraphDump (tree, JUST);
        TreePrint (tree, "logs/outtree2.txt");
    }

    TreeDelete (tree);
    LexsDelete (lexus);
    return 0;
}
