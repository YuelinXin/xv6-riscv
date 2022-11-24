// malloc and free

#include "kernel/types.h"
#include "user/user.h"
#include "user/memory_management.h"

void *global_base = 0;

// malloc
void *_malloc(int size)
{
    Block *block;
    // TODO: align size?

    if (size <= 0) // invalid size?
    {
        return 0;
    }

    if (size > SIZE_MAX / 2) // too big?
    {
        return 0;
    }

    if (!global_base)
    { // First call.
        block = request_space(0, size);
        if (!block)
        {
            return 0;
        }
        global_base = block;
    }
    else
    {
        Block *last = global_base;
        block = find_free_block(&last, size);
        if (!block)
        { // Failed to find free block.
            block = request_space(last, size);
            if (!block)
            {
                return 0;
            }
        }
        else
        { // Found free block
            // TODO: consider splitting block here.
            block->free = 0;
        }
    }

    return (block + 1);
}

// free
void _free(void *ptr)
{
    if (!ptr)
    {
        return;
    }

    // TODO: consider merging blocks once splitting blocks is implemented.
    Block *block_ptr = BLOCK_ADDR(ptr);
    if (block_ptr->free == 0)
    {
        block_ptr->free = 1;
        return;
    }
    return;
}

Block *find_free_block(Block **last, int size)
{
    Block *current = global_base;
    while (current && !(current->free && current->size >= size))
    {
        *last = current;
        current = current->next;
    }
    return current;
}

Block *request_space(Block *last, int size)
{
    Block *block;
    block = (Block *)sbrk(0); // get the top of the heap
    void *request = sbrk(size + META_SIZE);
    if ((void *)block == request) // Not thread safe.
    {
        return 0; // undefined behavior
    }
    if (request == (void *)-1)
    {
        return 0; // sbrk failed.
    }
    if (last)
    { // 0 on first request.
        last->next = block;
    }
    block->size = size;
    block->next = 0;
    block->free = 0;
    return block;
}
