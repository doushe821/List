#include "List.h"
#include "ListStruct.h"

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
    
    for(size_t i = 1; i < ListSize; i++)
    {
        memcpy((char*)(*list)->data + i * (*list)->elsize, &POISON, (*list)->elsize);
        (*list)->ind[i].next = i + 1;
        (*list)->ind[i].prev = FREE;
    }

    (*list)->free = 1;
    (*list)->ind[0].next = 0;
    (*list)->ind[0].prev = 0;
    
    return NO_ERRORS;
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
    return NO_ERRORS;
}

int PushFront(List_t* list, void* value)
{
    size_t prevFree = list->free;
    list->free = list->ind[prevFree].next;

    memcpy((char*)list->data + prevFree * list->elsize, value, list->elsize);

    list->ind[list->ind[NULL_ELEM].prev].next = prevFree;
    list->ind[prevFree].prev = list->ind[NULL_ELEM].prev;
    list->ind[prevFree].next = NULL_ELEM;

    list->ind[0].prev = prevFree;
    
    return NO_ERRORS;
}

int PushTail(List_t* list, void* value)
{
    size_t prevFree = list->free;
    list->free = list->ind[prevFree].next;

    memcpy((char*)list->data + prevFree * list->elsize, value, list->elsize);

    list->ind[prevFree].prev = NULL_ELEM;
    list->ind[prevFree].next = list->ind[NULL_ELEM].next;

    list->ind[list->ind[NULL_ELEM].next].prev = prevFree;

    list->ind[NULL_ELEM].next = prevFree;

    return NO_ERRORS;
}

int PushInd(List_t* list, void* value, size_t ind)
{
    size_t prevFree = list->free;
    list->free = list->ind[prevFree].next;

    memcpy((char*)list->data + prevFree * list->elsize, value, list->elsize);

    list->ind[prevFree].prev = ind;
    list->ind[prevFree].next = list->ind[ind].next;
    
    list->ind[list->ind[ind].next].prev = prevFree;
    list->ind[ind].next = prevFree;

    return NO_ERRORS;
}

int RemInd(List_t* list, void* dest, size_t ind)
{
    size_t prevFree = list->free;
    list->free = ind;

    memcpy(dest, (char*)list->data + ind * list->elsize, list->elsize);
    memcpy((char*)list->data + ind * list->elsize, &POISON, list->elsize);
    
    list->ind[list->ind[ind].next].prev = list->ind[ind].prev;
    list->ind[list->ind[ind].prev].next = list->ind[ind].next;

    list->ind[ind].prev = FREE;
    list->ind[ind].next = prevFree;

    return NO_ERRORS;
}

int RemFront(List_t* list, void* dest)
{
    size_t prevFree = list->free;
    list->free = list->ind[NULL_ELEM].prev;
    list->ind[list->ind[NULL_ELEM].prev].next = prevFree;

    memcpy(dest, (char*)list->data + list->ind[0].prev * list->elsize, list->elsize);
    memcpy((char*)list->data + list->ind[0].prev * list->elsize, &POISON, list->elsize);

    list->ind[0].prev = list->ind[list->ind[0].prev].prev;
    list->ind[list->ind[NULL_ELEM].prev].prev = FREE;

    return NO_ERRORS;
}

int RemTail(List_t* list, void* dest)
{
    size_t prevFree = list->free;
    list->free = list->ind[NULL_ELEM].next;
    list->ind[list->ind[NULL_ELEM].next].next = prevFree;

    memcpy(dest, (char*)list->data + list->ind[0].next * list->elsize, list->elsize);

    memcpy((char*)list->data + list->ind[0].next * list->elsize, &POISON, list->elsize);

    list->ind[0].next = list->ind[list->ind[0].next].next;
    list->ind[list->ind[NULL_ELEM].next].prev = FREE;

    return NO_ERRORS;
}

int GraphDump(List_t* list)
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
            if(i == list->free)
            {
                fprintf(fp, "\"node%zu\" [\n", i);
                fprintf(fp, "label = \"<f0> %zu |<f1> DATA = %zu|<f2> NEXT = %zu|<f3>PREV = %zu\"\n", i, POISON, list->ind[list->free].next, FREE);
                fprintf(fp, "shape = \"record\"\n");
                fprintf(fp, "];\n");
            }
            else
            {
                continue;
            }
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
    fprintf(fp, "label = \"<f0> fre|<f1> %zu\"\n", list->free);
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
            fprintf(fp, "\"node%zu\":f2 -> \"node%zu\" :f0 [id = %zu, constraint=false, color=\"blue\"];\n", i, list->ind[i].next, CCounter++);
            fprintf(fp, "\"node%zu\":f3 -> \"node%zu\" :f0 [id = %zu, constraint=false, color=\"red\"];\n", i, list->ind[i].prev, CCounter++);
        }
        //fprintf(fp, "\"node%zu\":f0 -> \"node%zu\" : f0 [id = %zu, ]");
    }

    fprintf(fp, "\"free\":f1 -> \"node%zu\"f0 [id = %zu];\n", list->free, CCounter++);

    fprintf(fp, "}");
    fclose(fp);
    system("dot -Tpdf lin.dot -o lin.pdf\n"); 
    return NO_ERRORS;
}