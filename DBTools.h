#ifndef DEBUG_TOOLS_INCLUDED
#define DEBUG_TOOLS_INCLUDED

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int meow(FILE* buffer, const char* format, ...);


#ifndef NDEBUG
#define $ fprintf(stderr, "%s:%d\n", __FILE__, __LINE__)
#define MEOW(message, ...) meow(stderr, message, __VA_ARGS__)
#else
#define $
#define MEOW(message)
#endif

#endif