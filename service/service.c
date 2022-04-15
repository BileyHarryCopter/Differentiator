#include "service.h"

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
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
