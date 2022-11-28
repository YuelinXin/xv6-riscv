// malloc and free

/*
    something to do in the future:
    1. make the malloc thread safe by using a thread lock
    2. try to merge and split the blocks to increase the memory efficiency
*/

#include "kernel/types.h"
#include "user/user.h"
#include "user/memory_management.h"

// since we do not implement our own heap
// we will need a global variable to keep 
// track of the top of the heap
void *heap_top = 0;

// malloc
void *_malloc(int size)
{
    Block *block;

    if (size <= 0) { // invalid size
        return 0;
    }

    if (size > SIZE_MAX / 2) { // too big
        return 0;
    }

    // align size
    if (size % ALIGN != 0) {
        size += ALIGN - (size % ALIGN);
    }

    if (!heap_top) { // first call to malloc, allocate the heap
        block = _sbrk_wrapper(heap_top, size);
        if (!block) { // request failed
            return 0;
        }
        heap_top = block;
    }
    else {
        Block *end = heap_top; // the end block of the heap
        int mem_ava = _memory_availability(); // free memory available
        if (mem_ava / size > 4) { // if memory is enough, use first fit
            block = _first_fit(&end, size);
        }
        else { // if memory is not enough, use worst fit
            block = _worst_fit(&end, size);
        }
        if (!block) { // no free block found
            block = _sbrk_wrapper(end, size);
            if (!block) {
                return 0;
            }
        }
        else { // make it unavailable
            block->free = 0;
        }
    }

    return (block + 1);
}

// free
void _free(void *ptr)
{
    if (!ptr) { // invalid pointer
        return;
    }

    // check if the pointer is a valid block returned by malloc
    // if not, return
    Block *block = BLOCK_ADDR(ptr);
    if (!block->size) {
        return;
    }

    // free the block if the block has not been freed
    if (!block->free) {
        block->free = 1;
        return;
    }
    return;
}

// first fit
Block *_first_fit(Block **end, int size)
{
    Block *current = heap_top;
    // find the first free block that fits the requested size
    // fast but not memory efficient
    while (current && !(current->free && current->size >= size)) {
        *end = current;
        current = current->next;
    }
    return current;
}

// worst fit
Block *_worst_fit(Block **end, int size)
{
    Block *current = heap_top;
    Block *worst = 0;
    // find the biggest free block that fits the requested size
    // this is to reduce the small block fragments
    // slow but more memory efficient than first fit
    while (current) {
        if (current->free && current->size >= size) {
            if (!worst || current->size > worst->size) {
                worst = current;
            }
        }
        *end = current;
        current = current->next;
    }
    return worst;
}

// request space
Block *_sbrk_wrapper(Block *end, int size)
{
    Block *block = (Block *)sbrk(0); // get the top of the heap
    void *request = sbrk(size + META_SIZE); // expand the heap
    if (request == (void *) - 1) {
        return 0; // sbrk failed
    }
    if ((void *)block == request) { 
        return block;
    }
    if (end) { // this should always point to the heap top
        end->next = block;
    }
    block->size = size;
    block->next = 0;
    block->free = 0;
    return block;
}

// calculate memory availability
int _memory_availability()
{
    Block *current = heap_top;
    int free = 0;
    while (current) {
        if (current->free) {
            free += current->size;
        }
        current = current->next;
    }
    return free;
}
