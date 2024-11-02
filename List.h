#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef NDEBUG

#include "DBTools.h"

#endif

enum Errors
{
    NULL_LIST_POINTER = 1,
    LIST_OVERFLOW,
    PUSHING_AFTER_NONE,
};

struct List_t;

int ListInit(List_t** list, size_t ListSize, size_t ElSize);
int ListDstr(List_t* list);
int PushFront(List_t* list, void* vlaue);
int PushInd(List_t* list, void* value, size_t ind);
int PushTail(List_t* list, void* value);

#ifndef NDEBUG
int QDump(List_t* list);
#endif

#endif