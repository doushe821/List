#ifndef LIST_STRUCT_H_INCLUDED
#define LIST_STRUCT_H_INLUDED

struct NodeRefferences
{
    size_t next;
    size_t prev;
};

struct List_t
{
    void* data;
    NodeRefferences* ref;
    size_t size;
    size_t elsize;
    size_t free;
    size_t dumpNum;
};


#endif