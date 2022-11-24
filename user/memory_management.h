#include "kernel/types.h"

#define PAGE_SIZE 4096 // the page size
#define SIZE_MAX 0xffffffff // the maximum size of a memory block
#define META_SIZE sizeof(Block) // the size of the metadata
#define BLOCK_ADDR(ptr) ((Block *)ptr - 1) // the address of the metadata

// a linked list of free memory blocks
// metadata for memory partitions
typedef struct memory_block {
    struct memory_block *next;  // next block in the list
    uint size;                  // size of the block
    uint free;                  // is the block free?
}Block;


/**
 * @brief Allocates a block of memory of the given size
 * 
 * @param size 
 * @return void* 
 */
void* _malloc(int size);

/**
 * @brief: free a block of memory returned by malloc
 *
 * @param ptr: pointer to the block to be freed
*/
void _free(void* ptr);

/**
 * @brief Find a free block to accommodate the requested size
 * 
 * @param last 
 * @param size 
 * @return Block* 
 */
Block *find_free_block(Block **last, int size);

/**
 * @brief Request space from the OS when no free block is available
 * 
 * @param last 
 * @param size 
 * @return Block* 
 */
Block *request_space(Block *last, int size);
