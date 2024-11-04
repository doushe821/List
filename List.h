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

int ListInit  (List_t** list, size_t ListSize, size_t ElSize);
int ListDstr  (List_t* list);

int PushFront (List_t* list, void* value            );
int PushTail  (List_t* list, void* value            );
int PushInd   (List_t* list, void* value, size_t ind);

int RemInd    (List_t* list, void* dest,  size_t ind);
int RemFront  (List_t* list, void* dest             );
int RemTail   (List_t* list, void* dest             );

int Linearize(List_t* list);

#endif