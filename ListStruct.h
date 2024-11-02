#ifndef LIST_STRUCT_H_INCLUDED
#define LIST_STRUCT_H_INLUDED

struct Navigation
{
    size_t next;
    size_t prev;
};

struct List_t
{
    void* data;
    Navigation* ind;
    size_t size;
    size_t elsize;
    size_t free;
};


#endif