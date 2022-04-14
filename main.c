#include "lexer/lexer.h"
#include "includes/standarts.h"

int main (void)
{
    lexem_array_t *lexus = LexsInit (CAP_INIT);
    if (LexsFill (lexus, "input.txt") == NO_ERROR)
    {
        LexsPrint (lexus, "output.txt");
    }
    LexsDelete (lexus);
    return 0;
}
