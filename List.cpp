#include "List.h"
#include "ListStruct.h"

const size_t FREE = 0x1EF2B5AC988A01;
const size_t NULL_ELEM = 0;

int ListInit(List_t** list, size_t ListSize, size_t ElSize)
{
    if(list == NULL)
    {
        return NULL_LIST_POINTER;
    }
    
    *list = (List_t*)calloc(1, sizeof(List_t));
    (*list)->elsize = ElSize;
    
    
    (*list)->data = calloc(ListSize + 1, ElSize);
    
    (*list)->ind = (Navigation*)calloc(ListSize, sizeof(Navigation));

    (*list)->size = ListSize;
    
    for(size_t i = 1; i < ListSize - 1; i++)
    {
        (*list)->ind[i].next = i + 1;
        (*list)->ind[i].prev = FREE;
    }
    
    (*list)->ind[ListSize - 1].next = 0;
    (*list)->ind[0].next = 0;
    (*list)->ind[0].prev = 0;
    (*list)->free = 1;
    
    return 0;
}

int ListDstr(List_t* list)
{
    if(list == NULL)
    {
        return NULL_LIST_POINTER;
    }

    free(list->data);
    free(list->ind);
    free(list);
    return 0;
}

int PushFront(List_t* list, void* value)
{
    memcpy((char*)list->data + list->free * list->elsize, value, list->elsize);

    list->ind[list->free].prev = list->ind[0].prev;
    list->ind[list->free].next = NULL_ELEM;

    list->ind[0].next = list->free;
    fprintf(stderr, "ADDED: %d\n", *((int*)((char*)list->data + list->free * list->elsize)));
    
    list->free++;
    return 0;
}

int RemInd(List_t* list, void* dest, size_t ind)
{
    memcpy(dest, list->data + ind * list->elsize, list->elsize);
    list->free = ind;
    
    list->ind[list->ind[ind].next].prev = list->ind[ind].prev;
    list->ind[list->ind[ind].prev].next = list->ind[ind].next;

    return 0;
}

int PushInd(List_t* list, void* value, size_t ind)
{
    memcpy(list->data + list->free * list->elsize, value, list->elsize);
    size_t oldFree = list->free;

    list->ind[oldFree].next = list->ind[ind].next;
    list->ind[oldFree].prev = ind;
    list->ind[ind].next = oldFree;
    list->ind[list->ind[ind].next].prev = oldFree;
    list->free++;

    return 0;
}