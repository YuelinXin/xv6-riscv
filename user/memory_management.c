// malloc and free

#include "kernel/types.h"
#include "user/user.h"
#include "user/memory_management.h"

// malloc
void *malloc(uint size)
{
    void *p;
    p = sbrk(size);
    if ((int)p == -1)
        return 0;
    return p;
}

// free
void free(void *p)
{
    // do nothing
}