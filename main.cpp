#include <stdio.h>
#include <stdlib.h>
#include "List.h"

int main()
{
    List_t* list = {};
    $;
    ListInit(&list, 52, 4);
    $;
    int ar[52] = {};
    printf("Start ar:\n");
    for(int i = 0; i < 10; i++)
    {
        ar[i] = rand()%100;
        printf("%d\n", ar[i]);
    }
    printf("\nList1:\n");
    for(int i = 0; i < 10; i++)
    {
        PushFront(list, &ar[i]);
    }
    int x = 777;
    size_t ind = 7;
    PushInd(list, &x, ind);
    PushTail(list, &x);
    QDump(list);
    ListDstr(list);
    return 0;
}