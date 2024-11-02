#include "DBTools.h"

#include <stdarg.h>

static int mew(FILE* buffer, const char* format, va_list vlist);

int meow(FILE* buffer, const char* format, ...)
{
    va_list vlist;
    va_start(vlist, format);

    mew(buffer, format, vlist);
    va_end(vlist);
    return 0;
}

static int mew(FILE* buffer, const char* format, va_list vlist)
{
    vfprintf(buffer, format, vlist);
    return 0;
}