/**
 * @name gc_shared.c
 * @author Mgr. Radomir Skrabal
 * This file contains implementation of shared variables and functions from gc_shared.h
 */
#include "gc_shared.h"
#include <stdlib.h>

void *stack_top;
void *stack_bottom;
void *BBSstart;
void *BBSend;

/**
 * Gets next block of memory
 * @par block - pointer to the memory block
 * @return pointer to the next memory block
 */
block_t *next_block(block_t *block)
{
    void *ptr;
    
    ptr = block;
    
    return (block_t*)(ptr + sizeof(block_t) + block->size);
}

/**
 * Call to system for get raw memory
 * @par size size of requested memory in bytes
 * @return addr to start of allocated memory or NULL
 */
void *get_memory_primitive(size_t size)
{
    //Primitive call, should be a system call but not important now
#ifndef DEBUG
    return malloc(size);
#else
    return calloc(size, 1);
#endif
}

/**
 * Call to system for memory deallocation
 * @par ptr pointer to allocated memory
 * @return always 0
 */
int release_memory_primitive(void *ptr)
{
    //Same as get_memory_primitive, not important now
    free(ptr);
    return 0;
}

/**
 * Initializes a block of given size from a larger chunk of raw memory
 * @par chunk oroginal chunk of memory
 * @par size of the memory chunk
 * @return initialized block
 */
block_t *init_block_from_chunk(void *chunk, size_t size)
{
    block_t *block;
    
    block = chunk;
    block->size = size - sizeof(block_t);
    block->forward = NULL;
    return block;
}

/** Returns allocated memory from a block
 * @par block initialized block of memory
 * @return ptr to the begining of user usable memory
 */
void *get_data_start(block_t *block)
{
    void *ptr = block;
    return ptr + sizeof(block_t);
}

/**
 * Returns pointer just after the allocated memory of a block
 * @par block block of a memory
 * @return pointer after the allocated memory block
 */
void *get_data_end(block_t *block)
{
    return (void*)next_block(block);
}

/**
 * Alings integer to the nearest greater <TODO násobek> of the size
 * @par size original size to be aligned
 * @return aligned size
 */
size_t align_size(size_t size)
{
    size_t mult;

    if(size % WORD_SIZE == 0)
    {
        return size;
    }
    
    mult = size / WORD_SIZE;
    
    return (mult + 1) * WORD_SIZE;
}

/**
 * Splits the block of memory
 * @par src original memory block (big) OUT: 
 * @par size size of new memory block
 * @return new memory block of given size or NULL if failed
 * @remark changes the address of src argument
 */
block_t *split_block(block_t **src, size_t size)
{
    void *ptr;
    size_t remaining, aligned_size;
    block_t *split;
    
    aligned_size = align_size(size);

    if(src == NULL || *src == NULL || aligned_size >= (*src)->size)
    {
        return NULL;
    }
    remaining = (*src)->size - aligned_size;

    ptr = get_data_start(*src);
    ptr = ptr + aligned_size;
            
    split = *src;
    *src = init_block_from_chunk(ptr, (*src)->size - aligned_size);
    split->size = aligned_size;
    split->forward = NULL;
    
    return split;	
}

/**
 * Predicates whetter the pointer points to the memory block
 * @par block block of a memory
 * @par ptr pointer
 * @return 1 if ptr points into block, 0 otherwise
 */
int is_pointer_to(block_t *block, void *ptr)
{
    void *start, *end;

    start = get_data_start(block);
    end = get_data_end(block);
    
    return ptr >= start && ptr < end;
}

#ifdef __gnu_linux__
#define ART_PTR_T unsigned long long
#else
#define ART_PTR_T unsigned long
#endif                                                 

/**
 * Returns ptr to the value of a structure slot givven by an offset
 * @par struct_ptr pointer to the allocated structure            
 * @par offset offset to the slot from start of the structure
 */
void *get_ptr_to_slot(void *struct_ptr, unsigned long offset)
{
  return (void*)((ART_PTR)struct_ptr + (ART_PTR)offset);
}

