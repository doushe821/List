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
    NO_ERRORS = 0,
    NULL_LIST_POINTER = 1,
    LIST_OVERFLOW,
    PUSHING_AFTER_NONE,
    FILE_CREATION_ERROR,
    WRITING_ERROR,
    REALLOC_FAILURE,
};

struct List_t;
struct NodeInfo
{
    void* data;
    int err;
    size_t next;
    size_t prev;
};

const int REALLOC_COEFF = 2;

int ListInit  (List_t** list, size_t ListSize, size_t ElSize);
int ListDstr  (List_t* list);

int PushFront (List_t* list, void* value            );
int PushTail  (List_t* list, void* value            );
int PushInd   (List_t* list, void* value, size_t ind);

int RemInd    (List_t* list, void* dest,  size_t ind);
int RemFront  (List_t* list, void* dest             );
int RemTail   (List_t* list, void* dest             );

int GraphDump(List_t* list, void DumpFunc(const void* value, FILE* fp));

int ListVer(List_t* list);

NodeInfo LNodeInfo(List_t* list, size_t ind);

#endif