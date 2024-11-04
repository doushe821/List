#include "List.h"
#include "ListStruct.h"
#include "Stack/Stack.h"

const size_t FREE = 0x1EF2B5AC988A01;
const size_t POISON = 0x192C4840EC;
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
    
    StackInit(&(*list)->FreeStk, ListSize - 1, sizeof(size_t));
    for(size_t i = 1; i < ListSize; i++)
    {
        memcpy((char*)(*list)->data + i * (*list)->elsize, &POISON, (*list)->elsize);
        (*list)->ind[i].next = i + 1;
        (*list)->ind[i].prev = FREE;

        size_t fre = ListSize - i;

        StackPush((*list)->FreeStk, &fre);
    }

    (*list)->ind[ListSize - 1].next = 0;
    (*list)->ind[0].next = 0;
    (*list)->ind[0].prev = 0;
    
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
    StackDtor(list->FreeStk);
    free(list);
    return 0;
}

int PushFront(List_t* list, void* value)
{
    size_t free = 0;
    if(StackPop(list->FreeStk, &free) == STACK_UNDERFLOW)
    {
        return LIST_OVERFLOW;
    }
    //fprintf(stderr, "frree: %zu\n", free);
    memcpy((char*)list->data + free * list->elsize, value, list->elsize);

    list->ind[list->ind[NULL_ELEM].prev].next = free;
    list->ind[free].prev = list->ind[NULL_ELEM].prev;
    list->ind[free].next = NULL_ELEM;

    list->ind[0].prev = free;

    return 0;
}

int PushTail(List_t* list, void* value)
{
    size_t free = 0;
    if(StackPop(list->FreeStk, &free) == STACK_UNDERFLOW)
    {
        return LIST_OVERFLOW;
    }

    memcpy((char*)list->data + free * list->elsize, value, list->elsize);

    list->ind[free].prev = NULL_ELEM;
    list->ind[free].next = list->ind[NULL_ELEM].next;

    list->ind[list->ind[NULL_ELEM].next].prev = free;

    list->ind[NULL_ELEM].next = free;

    return 0;
}

int PushInd(List_t* list, void* value, size_t ind)
{
    size_t free = 0;
    if(StackPop(list->FreeStk, &free) == STACK_UNDERFLOW)
    {
        return LIST_OVERFLOW;
    }

    memcpy((char*)list->data + free * list->elsize, value, list->elsize);

    list->ind[free].prev = ind;
    list->ind[free].next = list->ind[ind].next;
    
    list->ind[list->ind[ind].next].prev = free;
    list->ind[ind].next = free;

    return 0;
}

int RemInd(List_t* list, void* dest, size_t ind)
{
    memcpy(dest, (char*)list->data + ind * list->elsize, list->elsize);
    StackPush(list->FreeStk, &ind);
    memcpy((char*)list->data + ind * list->elsize, &POISON, list->elsize);
    
    list->ind[list->ind[ind].next].prev = list->ind[ind].prev;
    list->ind[list->ind[ind].prev].next = list->ind[ind].next;

    list->ind[ind].prev = FREE;
    list->ind[ind].next = FREE;

    return 0;
}

int RemFront(List_t* list, void* dest)
{
    memcpy(dest, (char*)list->data + list->ind[0].prev * list->elsize, list->elsize);
    StackPush(list->FreeStk, &list->ind[0].prev);
    memcpy((char*)list->data + list->ind[0].prev * list->elsize, &POISON, list->elsize);

    list->ind[0].prev = list->ind[list->ind[0].prev].prev;
    
    

    return 0;
}

int RemTail(List_t* list, void* dest)
{
    memcpy(dest, (char*)list->data + list->ind[0].next * list->elsize, list->elsize);
    StackPush(list->FreeStk, &list->ind[0].next);
    memcpy((char*)list->data + list->ind[0].next * list->elsize, &POISON, list->elsize);

    list->ind[0].next = list->ind[list->ind[0].next].next;

    return 0;
}

int Linearize(List_t* list)
{
    FILE* fp = NULL;
    if((fp = fopen("lin.dot", "w+b")) == NULL)
    {
        fprintf(stderr, "Can't create output file\n");
        return FILE_CREATION_ERROR;
    }

    fprintf(fp, 
    "digraph g {\n"
    "fontname=\"Helvetica,Arial,sans-serif\""
    "node [fontname=\"Helvetica,Arial,sans-serif\"]\n"
    "graph [\n"
    "rankdir = \"LR\"\n"
    "];\n"
    "node [\n"
    "fontisze = \"16\"\n"
    "shape = \"ellipse\"\n"
    "];\n"                                            );

    for(size_t i = 0; i < list->size; i++)
    {
        if(list->ind[i].prev == FREE)
        {
            fprintf(fp, "\"node%zu\" [\n", i);
            fprintf(fp, "label = \"<f0> %zu |<f1> DATA = %zu|<f2> NEXT = %zu|<f3>PREV = %zu\"\n", i, POISON, FREE, FREE);
            fprintf(fp, "shape = \"record\"\n");
            fprintf(fp, "];\n");
        }
        else
        {
            fprintf(fp, "\"node%zu\" [\n", i);
            fprintf(fp, "label = \"<f0> %zu |<f1> DATA = %d|<f2> NEXT = %zu|<f3>PREV = %zu\"\n", i, *(int*)((char*)list->data + i * list->elsize), list->ind[i].next, list->ind[i].prev);
            fprintf(fp, "shape = \"record\"\n");
            fprintf(fp, "];\n");
        }
    }

    fprintf(fp, "\"free\" [\n");

    size_t freeID = 0;
    StackLook(list->FreeStk, &freeID);

    fprintf(fp, "label = \"<f0> fre|<f1> %zu\"\n", freeID);
    fprintf(fp, "shape = \"record\"\n");
    fprintf(fp, "];\n");
    
    size_t CCounter = 0;

    for(size_t i = 0; i < list->size; i++)
    {
        if(list->ind[i].prev == FREE)
        {
            continue;
        }
        else
        {
            fprintf(fp, "\"node%zu\":f2 -> \"node%zu\":f0 [id = %zu];\n", i, list->ind[i].next, CCounter++);
            fprintf(fp, "\"node%zu\":f3 -> \"node%zu\":f0 [id = %zu];\n", i, list->ind[i].prev, CCounter++);
        }
    }

    fprintf(fp, "\"free\":f1 -> \"node%zu\"f0 [id = %zu];\n", freeID, CCounter++);

    fprintf(fp, "}");
    return 0;
}