#include "includes/standarts.h"
#include "includes/constant.h"
#include "lexer/lexer.h"
#include "parcer/parcer.h"

int main (void)
{
    node_t *tree = NULL;
    lexem_array_t *lexus = LexsInit (CAP_INIT);

    if (LexsFill (lexus, "logs/input.txt") == NO_ERROR)
    {
        LexsPrint (lexus, "logs/output.txt");
        tree = Build_Syntax_Tree (lexus);
        TreePrint (tree, "logs/outtree.txt");
    }

    TreeDelete (tree);
    LexsDelete (lexus);
    return 0;
}
