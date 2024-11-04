#ifndef STACK_STRUCT_INCLUDED
#define STACK_STRUCT_INCLUDED

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "hash.h"

const int STACK_NAME_MAX = 16;
const int FUNC_NAME_MAX= 32;

struct Stack_t
{
    void* data;
    size_t size;
    size_t capacity;
    size_t elSize;

    int initialised;

    int Error;
};

#endif