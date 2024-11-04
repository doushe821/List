#include <stdio.h>
#include <stdlib.h>
#include "List.h"

int main()
{
    List_t* list = {};
    ListInit(&list, 52, 4);
    
    int ar[52] = {};
    for(int i = 0; i < 52; i++)
    {
        ar[i] = rand()%100;
    }
    
    for(int i = 0; i < 10; i++)
    {
        PushFront(list, &ar[i]);
    }
    PushTail(list, &ar[11]);
    size_t pencil = 0;
    RemInd(list, &pencil, 7);
    Linearize(list);
    ListDstr(list);
    return 0;

}