#ifndef SERVICE
#define SERVICE

#include "../includes/standarts.h"
#include "../includes/constant.h"

FILE * FileOpen (const char *file_name, const char *mode);
int FileClose (FILE *file);

#endif
