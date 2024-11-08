#include "List.h"
#include "ListStruct.h"

const size_t FREE = 0x1EF2B5AC988A01;
const size_t POISON = 0x192C4840EC;
const size_t NULL_ELEM = 0;

static int MemCheck  (List_t* list);
static int ListResize(List_t* list);

int ListInit(List_t** list, size_t ListSize, size_t ElSize)
{
    if(list == NULL)
    {
        return NULL_LIST_POINTER;
    }
    
    *list = (List_t*)calloc(1, sizeof(List_t));
    (*list)->elsize = ElSize;
    
    (*list)->data = calloc(ListSize + 1, ElSize);
    
    (*list)->ref = (NodeRefferences*)calloc(ListSize, sizeof(NodeRefferences));

    (*list)->size = ListSize;
    
    for(size_t i = 1; i < ListSize; i++)
    {
        memcpy((char*)(*list)->data + i * (*list)->elsize, &POISON, (*list)->elsize);
        (*list)->ref[i].next = i + 1;
        (*list)->ref[i].prev = FREE;
    }
    (*list)->ref[ListSize - 1].next = NULL_ELEM;
    (*list)->free = 1;
    (*list)->ref[NULL_ELEM].next = NULL_ELEM;
    (*list)->ref[NULL_ELEM].prev = NULL_ELEM;

    (*list)->dumpNum = 0;
    
    return NO_ERRORS;
}

int ListDstr(List_t* list)
{
    if(list == NULL)
    {
        return NULL_LIST_POINTER;
    }

    free(list->data);
    free(list->ref);
    free(list);
    return NO_ERRORS;
}

int PushFront(List_t* list, void* value)
{
    if(list == NULL)
    {
        return NULL_LIST_POINTER;
    }
    if(MemCheck(list) != 0)
    {
        return REALLOC_FAILURE;
    }
    size_t prevFree = list->free;
    list->free = list->ref[prevFree].next;

    memcpy((char*)list->data + prevFree * list->elsize, value, list->elsize);

    list->ref[list->ref[NULL_ELEM].prev].next = prevFree;
    list->ref[prevFree].prev = list->ref[NULL_ELEM].prev;
    list->ref[prevFree].next = NULL_ELEM;

    list->ref[0].prev = prevFree;
    
    return NO_ERRORS;
}

int PushTail(List_t* list, void* value)
{
    if(list == NULL)
    {
        return NULL_LIST_POINTER;
    }
    if(MemCheck(list) != 0)
    {
        return REALLOC_FAILURE;
    }
    size_t prevFree = list->free;
    list->free = list->ref[prevFree].next;

    memcpy((char*)list->data + prevFree * list->elsize, value, list->elsize);

    list->ref[prevFree].prev = NULL_ELEM;
    list->ref[prevFree].next = list->ref[NULL_ELEM].next;

    list->ref[list->ref[NULL_ELEM].next].prev = prevFree;

    list->ref[NULL_ELEM].next = prevFree;

    return NO_ERRORS;
}

int PushInd(List_t* list, void* value, size_t ind)
{
    if(list == NULL)
    {
        return NULL_LIST_POINTER;
    }
    if(MemCheck(list) != 0)
    {
        return REALLOC_FAILURE;
    }
    size_t prevFree = list->free;
    list->free = list->ref[prevFree].next;

    memcpy((char*)list->data + prevFree * list->elsize, value, list->elsize);

    list->ref[prevFree].prev = ind;
    list->ref[prevFree].next = list->ref[ind].next;
    
    list->ref[list->ref[ind].next].prev = prevFree;
    list->ref[ind].next = prevFree;

    return NO_ERRORS; 
}

NodeInfo LNodeInfo(List_t* list, size_t ind)
{    
    NodeInfo info = {};
    if(list == NULL)
    {
        info.err = NULL_LIST_POINTER;
        return info;
    }
    info.err = 0;
    info.next = list->ref[ind].next;
    info.prev = list->ref[ind].prev;
    info.data = (char*)list->data + ind * list->elsize;
    return info;
}

int RemInd(List_t* list, void* dest, size_t ind)
{
    size_t prevFree = list->free;
    list->free = ind;

    memcpy(dest, (char*)list->data + ind * list->elsize, list->elsize);
    memcpy((char*)list->data + ind * list->elsize, &POISON, list->elsize);
    
    list->ref[list->ref[ind].next].prev = list->ref[ind].prev;
    list->ref[list->ref[ind].prev].next = list->ref[ind].next;

    list->ref[ind].prev = FREE;
    list->ref[ind].next = prevFree;

    return NO_ERRORS;
}

int RemFront(List_t* list, void* dest)
{
    size_t prevFree = list->free;
    list->free = list->ref[NULL_ELEM].prev;
    list->ref[list->ref[NULL_ELEM].prev].next = prevFree;

    memcpy(dest, (char*)list->data + list->ref[0].prev * list->elsize, list->elsize);
    memcpy((char*)list->data + list->ref[0].prev * list->elsize, &POISON, list->elsize);

    list->ref[0].prev = list->ref[list->ref[0].prev].prev;
    list->ref[list->ref[NULL_ELEM].prev].prev = FREE;

    return NO_ERRORS;
}

int RemTail(List_t* list, void* dest)
{
    size_t prevFree = list->free;
    list->free = list->ref[NULL_ELEM].next;
    list->ref[list->ref[NULL_ELEM].next].next = prevFree;

    memcpy(dest, (char*)list->data + list->ref[0].next * list->elsize, list->elsize);

    memcpy((char*)list->data + list->ref[0].next * list->elsize, &POISON, list->elsize);

    list->ref[0].next = list->ref[list->ref[0].next].next;
    list->ref[list->ref[NULL_ELEM].next].prev = FREE;

    return NO_ERRORS;
}

static int MemCheck(List_t* list)
{
    if(list->ref[list->free].next == NULL_ELEM)
    {
        list->ref[list->free].next = list->size;
        if(ListResize(list) != 0)
        {
            return REALLOC_FAILURE;
        }
        for(size_t i = list->ref[list->free].next; i < list->size; i++)
        {
            memcpy((char*)list->data + i * list->elsize, &POISON, list->elsize);
            list->ref[i].next = i + 1;
            list->ref[i].prev = FREE;
        }
        list->ref[list->size - 1].next = 0;
    }
    return NO_ERRORS;
}

static int ListResize(List_t* list)
{
    void* ptr = realloc(list->data, list->size * list->elsize * REALLOC_COEFF);
    if(ptr == NULL)
    {
        return REALLOC_FAILURE;
    }
    list->data = ptr;

    ptr = realloc(list->ref, list->size * sizeof(NodeRefferences) * REALLOC_COEFF);
    if(ptr == NULL)
    {
        return REALLOC_FAILURE;
    }
    list->ref = (NodeRefferences*)ptr;

    list->size *= REALLOC_COEFF;
    return NO_ERRORS;
}

int GraphDump(List_t* list, void DumpFunc(const void* value, FILE* fp))
{
    FILE* fp = NULL;
    if(list->dumpNum == 0)
    {
        system("mkdir ListLog");
    }

    if((fp = fopen("ListLog/lin.dot", "w+b")) == NULL)
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
        if(list->ref[i].prev == FREE)
        {
            if(i == list->free)
            {
                fprintf(fp, "\"node%zu\" [\n", i);
                fprintf(fp, "label = \"<f0> %zu |<f1> DATA = %zu|<f2> NEXT = %zu|<f3>PREV = %zu\"\n", i, POISON, list->ref[list->free].next, FREE);
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
            fprintf(fp, "label = \"<f0> %zu |<f1> DATA = ", i);
            DumpFunc((char*)list->data + i * list->elsize, fp);
            fprintf(fp, "|<f2> NEXT = %zu|<f3>PREV = %zu\"\n", list->ref[i].next, list->ref[i].prev);
            fprintf(fp, "shape = \"record\"\n");
            fprintf(fp, "];\n");
        }
    }

    fprintf(fp, "\"free\" [\n");
    fprintf(fp, "label = \"<f0> fre|<f1> %zu\"\n", list->free);
    fprintf(fp, "shape = \"record\"\n");
    fprintf(fp, "];\n");

    for(size_t i = 0; i < list->size; i++)
    {
        if(list->ref[i].prev == FREE)
        {
            continue;
        }
        else
        {
            fprintf(fp, "\"node%zu\":f2 -> \"node%zu\"[color=\"blue\"];\n", i, list->ref[i].next);
            fprintf(fp, "\"node%zu\":f3 -> \"node%zu\"[color=\"red\"];\n", i, list->ref[i].prev);
        }
        fprintf(fp, "\"node%zu\"-> \"node%zu\" [weight=69, color=white]", i, i + 1);
    }

    fprintf(fp, "\"free\":f1 -> \"node%zu\"f0;\n", list->free);

    fprintf(fp, "}");
    fclose(fp);

    char Dump[FILENAME_MAX] = {};
    sprintf(Dump, "cd ListLog\ndot -Tpdf lin.dot -o dump%zu.pdf\n", list->dumpNum);
    system(Dump);
    list->dumpNum++;

    return NO_ERRORS;
}