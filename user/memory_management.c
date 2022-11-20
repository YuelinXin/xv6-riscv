// malloc and free

#include "kernel/types.h"
#include "user/user.h"
#include "user/memory_management.h"

// malloc
void *malloc(int size)
{
    // return NULL if size is 0
    if (size <= 0)
    {
        return NULL;
    }
    // allocate memory
    void *ptr = sbrk(size);
    // return NULL if allocation failed
    if (ptr == (void *)-1)
    {
        return NULL;
    }
    // return pointer to allocated memory
    return ptr;
}

// free
void free(void *p)
{
    // do nothing
}