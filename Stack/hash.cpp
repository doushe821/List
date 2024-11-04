#include "hash.h"
#include <stdio.h>

hash_t hash(const void* data, size_t size)
{
    hash_t hash = 5381;
    for(hash_t i = 0; i < size; i++)
    {
        hash = hash*33^((hash_t)(*((char*)data + i)));
    }
    return hash;
}