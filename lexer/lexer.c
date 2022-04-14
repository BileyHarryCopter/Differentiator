#include "lexer.h"
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
lexem_array_t *LexsInit (unsigned capacity)
{
    lexem_array_t *lexus = (lexem_array_t *) calloc (1, sizeof (lexem_array_t));
    lexus->lexems        = (lexem_t *) calloc (capacity, sizeof (lexem_t));
    lexus->capacity = capacity;
    lexus->size     = 0;
    return lexus;
}

int LexsResz (lexem_array_t *lexus)
{
    assert (lexus);
    assert (lexus->lexems);
    lexus->capacity = ENCR_KF * lexus->capacity;
    lexus->lexems   = (lexem_t *) realloc (lexus->lexems, lexus->capacity * sizeof (lexem_t));
    return NO_ERROR;
}

int LexsDelete (lexem_array_t *lexus)
{
    assert (lexus);
    assert (lexus->lexems);

    for (int i = 0; i < lexus->size; i++)
    {
        if (lexus->lexems[i].kind == VARIABLE)
            free (lexus->lexems[i].lexm.var);
    }
    free   (lexus->lexems);
    free   (lexus);
    return NO_ERROR;
}

int LexsInsert (lexem_array_t *lexus, kind_t kind, ...)
{
    va_list args;
    data_t data;
    int token;
    var_t var = NULL;

    va_start (args, kind);
    if (kind == VARIABLE)
    {
        var = va_arg (args, var_t);
        assert (var);
    }
    else if (kind == NUMBER)
        data = va_arg (args, data_t);
    else
        token = va_arg (args, int);

    assert (lexus);
    if (lexus->size > KRIT_KF * lexus->capacity)
        LexsResz (lexus);
    assert (lexus->lexems);

    lexus->lexems[lexus->size].kind = kind;
    switch (kind)
    {
        case VARIABLE:
            lexus->lexems[lexus->size].lexm.var = (char *) calloc (strlen(var), sizeof (char));
            lexus->lexems[lexus->size].lexm.var = memmove (lexus->lexems[lexus->size].lexm.var, var, strlen(var));
            break;
        case NUMBER:
            lexus->lexems[lexus->size].lexm.data = data;
            break;
        case BRACKET:
            lexus->lexems[lexus->size].lexm.brac = token;
            break;
        case OPERATOR:
            lexus->lexems[lexus->size].lexm.oper = token;
            break;
        case FUNCTION:
            lexus->lexems[lexus->size].lexm.func = token;
            break;
        default:
            return ERROR;
    }
    lexus->size++;

    va_end (args);

    return NO_ERROR;
}

int LexsPrint (lexem_array_t *lexus, const char * output_name)
{
    assert (lexus);
    assert (output_name);
    int token = 0;
    int kind  = 0;
    FILE * file = FileOpen (output_name, "w");


    for (unsigned pos = 0; pos < lexus->size - 1; pos++)
    {
        kind = lexus->lexems[pos].kind;
        switch (kind)
        {
            case VARIABLE:
                fprintf (file, "VARIABLE: %s\n", lexus->lexems[pos].lexm.var);
                break;
            case NUMBER:
                fprintf (file, "NUMBER: %lf\n", lexus->lexems[pos].lexm.data);
                break;
            case BRACKET:
                token = lexus->lexems[pos].lexm.brac;
                if (token == LRBRACE || token == LSBRACE)
                    fprintf (file, "LBRACE\n");
                else if (token == RRBRACE || token == RSBRACE)
                    fprintf (file, "RBRACE\n");
                break;
            case OPERATOR:
                token = lexus->lexems[pos].lexm.oper;
                fprintf (file, "OPERATOR: %c\n", token);
                break;
            case FUNCTION:
                token = lexus->lexems[pos].lexm.func;
                fprintf (file, "FUNCTION: %s\n", function_names[token]);
            default:
                return ERROR;
        }
    }

    FileClose (file);

    return NO_ERROR;
}

//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//

FILE * FileOpen (const char *file_name, const char *mode)
{
    assert (file_name && mode);
    FILE *file = fopen (file_name, mode);
    assert (file);
    return file;
}

int FileClose (FILE *file)
{
    assert (file);
    fclose (file);
    return NO_ERROR;
}

char * StrCtor (char first, FILE *file)
{
    assert (file);
    int length = 0;
    char *str = (char *) calloc (STR_INIT, sizeof (char));
    ungetc (first, file);

    while (first != EOF && first != '\n')
    {
        first = getc (file);
        if (first == LRBRACE || first == RRBRACE || first == LSBRACE || first == RSBRACE
        ||  first == ADD || first == SUB || first == MUL || first == DIV || first == DEG)
        {
            ungetc (first, file);
            return str;
        }
        str[length] = first;
        length++;
    }
    //  delete an indent
    if (length > 1)
        str[length - 1] = 0;

    assert (str);
    return str;
}

int StrDelete (char *str)
{
    assert (str);
    free (str);
    return NO_ERROR;
}

int IstrCmp (char *str1, const char *str2)
{
    assert (str1);
    assert (str2);
    int pos = 0;

    for (pos = 0; str2[pos] != 0; pos++)
    {
        if (toupper (str1[pos]) != str2[pos])
            return toupper (str1[pos]) - str2[pos];
    }

    return (toupper(str1[pos] < str2[pos])) ? -1 : 1;
}

int IsFunc (char *str)
{
    assert (str);

    for (int i = 0; i < FUNC_NUMB; i++)
    {
        if (IstrCmp (str, function_names[i]) == 0)
            return i;
    }

    return -1;
}

int LexsFill (lexem_array_t *lexus, const char *file_name)
{
    int token = 0;
    char symb = 0;
    data_t data = 0.0;
    unsigned pos = 0;
    char *str = NULL;
    FILE *file = NULL;

    file = FileOpen (file_name, "r");
    fseek (file, 0L, SEEK_SET);

    while (symb != EOF)
    {
        symb = getc (file);

        if (isspace(symb))
            continue;

        else if (isdigit (symb))
        {
            ungetc (symb, file);
            fscanf (file, "%lf", &data);
            LexsInsert (lexus, NUMBER, data);
        }
        else if (symb == LRBRACE || symb == RRBRACE || symb == LSBRACE || symb == RSBRACE)
        {
            LexsInsert (lexus, BRACKET, symb);
        }
        else if (symb == ADD || symb == SUB || symb == MUL || symb == DIV || symb == DEG)
        {
            LexsInsert (lexus, OPERATOR, symb);
        }
        else
        {
            str = StrCtor (symb, file);
            if (IsFunc (str) != -1)
            {
                token = IsFunc (str);
                LexsInsert (lexus, FUNCTION, token);
            }
            else
                LexsInsert (lexus, VARIABLE, str);
            StrDelete (str);
        }
    }

    FileClose (file);
    return NO_ERROR;
}
