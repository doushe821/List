#include <stdio.h>
#include <stdlib.h>
#include "List.h"
void IntDump(const void* value, FILE* fp);
int main()
{
    List_t* list = {};
    ListInit(&list, 4, 4);
    int ar[52] = {};
    for(int i = 0; i < 52; i++)
    {
        ar[i] = rand()%100;
    }
    
    for(int i = 0; i < 10; i++)
    {
        PushFront(list, &ar[i]);
        GraphDump(list, IntDump);
    }
    PushTail(list, &ar[11]);
    size_t pencil = 0;
    RemInd(list, &pencil, 7);
    GraphDump(list, IntDump);

    NodeInfo ni = {};
    size_t next = 1;
    int d = 0;
    for(int i = 0; i < 10; i++)
    {
        ni = LNodeInfo(list, next);
        memcpy(&d, ni.data, 4);
        printf("%d\n", d);
        next = ni.next;
        MEOW("NEXT = %zu\n", next);
    }

    ListDstr(list);
    return 0;

}

void IntDump(const void* value, FILE* fp)
{
    int a = 0;
    memcpy(&a, value, 4);
    fprintf(fp, "%d", a);
}