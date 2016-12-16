/**
 * @name gc_shared.c
 * @author Mgr. Radomir Skrabal
 * This file contains implementation of shared variables and functions from gc_shared.h
 */
#include "gc_shared.h"
#include "gc_util.h"
#include <stdio.h>
#include <stdlib.h>
#include "gc_constants.h"
#include <syslog.h>

void *stack_top;
void *stack_bottom;
void *BBSstart;
void *BBSend;

#define ARRAY_BIT_MASK 0x8000000000000000
#define ELEMENT_TYPE_BIT_MASK 0x7FFFFFFFFFFFFFFF

/**
 * Getters
 * Quite primitive right now but will raise in importance during optimization phase
 */
size_t block_get_size(block_t *block)
{
    type_info_t *descriptor;
    int type;
    
    if(block_has_forward(block))
    {
        return block_get_size(block_get_forward(block));
    }
    
    descriptor = block_get_info(block);
    
    if(block_is_array(block))
    {
        return align_size(block_get_array_size(block) * descriptor->size) + sizeof(block_t);
    }
    
    return align_size(descriptor->size) + sizeof(uint64_t);
}
void *block_get_forward(block_t *block)
{
    if(block_get_type(block) == TYPE_FORWARD)
    {
        return (void*)block->size;
    }
    return NULL;
}
uint64_t block_get_type(block_t *block)
{    
    return block->type & ELEMENT_TYPE_BIT_MASK;
}
size_t block_get_array_size(block_t *block)
{
    if(block_has_forward(block))
    {
        return block_get_array_size(block_get_forward(block));
    }
    
    return block->size;
}
type_info_t *block_get_info(block_t *block)
{
    if(block_has_forward(block))
    {
        return block_get_info(block_get_forward(block));
    }
    
    return &type_table[block_get_type(block)];
}
int block_is_array(block_t *block)
{
    if(block_has_forward(block))
    {
        return block_is_array(block_get_forward(block));
    }
    
    return (block->type & ARRAY_BIT_MASK) > 0;
}

/**
 * Setters
 * Quite primitive right now but will raise in importance during optimization phase
 */
int block_set_size(block_t *block, size_t size)
{
    if(block == NULL)
    {
        gc_log(LOG_ERR, "error: Call block_set_size(NULL, %u)", (unsigned int)size);
        return 0;
    }
    
    gc_log(LOG_ERR, "Error: Obsolete function block_set_size called");
    return 0;
}
int block_set_forward(block_t *block, void *forward)
{
    if(block == NULL)
    {
        gc_log(LOG_ERR, "error: Call block_set_forward(NULL, %p)", forward);
        return 0;
    }
    
    block->type = TYPE_FORWARD;
    block->size = (uint64_t)forward;
    
    return 0;
}
int block_set_type(block_t *block, uint64_t type)
{
    if(block == NULL)
    {
        gc_log(LOG_ERR, "error: Call block_set_type(NULL, %x)", (unsigned)type);
        return 0;
    }

    block->type &= ~ELEMENT_TYPE_BIT_MASK;
    block->type |= (type & ELEMENT_TYPE_BIT_MASK);
   
    return 0;    
}
int block_set_array_size(block_t *block, size_t size)
{
    if(block == NULL)
    {
        gc_log(LOG_ERR, "error: Call block_set_array_size(NULL, %u)", (unsigned int)size);
        return 0;
    }
    block->size = size;
    return 0;
}
int block_set_info(block_t *block, type_info_t *info)
{
    if(block == NULL)
    {
        gc_log(LOG_ERR, "error: Call block_set_info(NULL, %p)", info);
        return 0;
    }
    
    gc_log(LOG_ERR, "Error: Obsolete function block_set_info used");    
    return 0;
}
int block_set_is_array(block_t *block, int is_array)
{
    if(block == NULL)
    {
        gc_log(LOG_ERR, "error: Call block_set_is_array(NULL, %d)", (int)is_array);
        return 0;
    }
    
    if(is_array)
    {
        block->type |= ARRAY_BIT_MASK;
    }
    else
    {
        block->type &= ~ARRAY_BIT_MASK;
    }
    
    return 0;
}

/**
 * Predicates whetter the block has a forwarding addr set
 * @par block a memory block
 * @return 1 if the address is set, 0 otherwise
 */
int block_has_forward(block_t *block)
{
    return block_get_type(block) == TYPE_FORWARD;
}

/**
 * Copies the metadata of a block to other block
 * @par src source memory block
 * @par dst destination memory block
 * @return always 0
 */
int copy_block_metadata(block_t *src, block_t *dst)
{
    block_set_type(dst, block_get_type(src));
    if(block_is_array(src))
    {
        block_set_array_size(dst, block_get_array_size(src));
        block_set_is_array(dst, 1);
    }
    else
    {
        block_set_is_array(dst, 0);
    }
}

/**
 * Predicates whetter this block contains structures
 * @par block memory block
 * @return 1 if this block contains structures, 0 otherwise
 */
int block_is_struct_block(block_t *block)
{
    int type = block_get_type(block);
    return      type != TYPE_UNDEFINED
            &&  type != TYPE_INT
            &&  type != TYPE_PTR
            &&  type != TYPE_DOUBLE;
}
/**
 * Gets next block of memory
 * @par block - pointer to the memory block
 * @return pointer to the next memory block
 */
block_t *next_block(block_t *block)
{
    void *ptr;
    
    ptr = block;
    
    return (block_t*)(ptr + block_get_size(block));
}

block_t *next_nth_block(block_t *block, size_t n)
{
    if(n == 1 || n == 0)
    {
        return next_block(block);
    }
    return next_block(next_nth_block(block, n-1));
}

/**
 * Call to system for get raw memory
 * @par size size of requested memory in bytes
 * @return addr to start of allocated memory or NULL
 */
void *get_memory_primitive(size_t size)
{
    //Primitive call, should be a system call but not important now
//#ifndef DEBUG
//    return malloc(size);
//#else
    return calloc(size, 1);
//#endif
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

/** Returns allocated memory from a block
 * @par block initialized block of memory
 * @return ptr to the begining of user usable memory
 */
void *get_data_start(block_t *block)
{
    void *ptr = block;
    
    if(block_is_array(block))
    {    
        return ptr + sizeof(block_t);
    }
    
    return ptr + sizeof(uint64_t);
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
 * Initializes a block of given size from a larger chunk of raw memory
 * @par chunk oroginal chunk of memory
 * @par size of the memory chunk
 * @return initialized block
 */
block_t *init_block_from_chunk(void *chunk, size_t size)
{
    block_t *block;
    
    block = chunk;
    block_set_is_array(block, 1);
    block_set_type(block, TYPE_UNDEFINED);
    block_set_array_size(block, size);
    return block;
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
    
    if(block_get_type(*src) != TYPE_UNDEFINED)
    {
        gc_log(LOG_ERR, "error: split_block called with non-raw block: split_block(%p, %u)", src, (unsigned)size);
        return NULL;
    }
    
    aligned_size = align_size(size);

    if(src == NULL || *src == NULL)
    {
        return NULL;
    }
    
    if(aligned_size + sizeof(block_t) >= block_get_size(*src) - sizeof(block_t))
    {
        return NULL;
    }
    
    remaining = (block_get_size(*src) - sizeof(block_t)) - (aligned_size + sizeof(block_t));

    ptr = (void*)((uint64_t)*src + (uint64_t)sizeof(block_t) + (uint64_t)aligned_size);
            
    split = *src;
    *src = ptr;

    block_set_is_array(ptr, 1);
    block_set_type(ptr, TYPE_UNDEFINED);
    block_set_array_size(ptr, remaining);
    
    block_set_is_array(split, 1);
    block_set_type(split, TYPE_UNDEFINED);
    block_set_array_size(split, aligned_size);
    
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

/**
 * Returns ptr to the value of a structure slot givven by an offset
 * @par struct_ptr pointer to the allocated structure            
 * @par offset offset to the slot from start of the structure
 */
void *get_ptr_to_slot(void *struct_ptr, unsigned long offset)
{
  return (void*)((art_ptr_t)struct_ptr + (art_ptr_t)offset);
}

