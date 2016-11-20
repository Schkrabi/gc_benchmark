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

void *stack_top;
void *stack_bottom;
void *BBSstart;
void *BBSend;

/**
 * Getters
 * Quite primitive right now but will raise in importance during optimization phase
 */
size_t block_get_size(block_t *block)
{
    type_info_t *descriptor;
    int type;
    
    type = block_get_type(block);
    
    if(type == TYPE_UNDEFINED)
    {
        return block_get_array_size(block);
    }
    
    descriptor = block_get_info(block);
    
    if(type == TYPE_ARRAY)
    {
        return align_size(block_get_array_size(block) * descriptor->size);
    }
    
    return align_size(descriptor->size);
}
void *block_get_forward(block_t *block)
{
    return block->forward;
}
byte block_get_type(block_t *block)
{
    return block->type;
}
size_t block_get_array_size(block_t *block)
{
    return block->size;
}
type_info_t *block_get_info(block_t *block)
{
    return &type_table[block_get_element_type(block)];
}
int block_atom_is_ptr(block_t *block)
{
    return block_get_element_type(block) == TYPE_PTR;
}
int block_get_element_type(block_t *block)
{
    return block->element_type;
}

/**
 * Setters
 * Quite primitive right now but will raise in importance during optimization phase
 */
int block_set_size(block_t *block, size_t size)
{
    if(block == NULL)
    {
        fprintf(stderr, "\nerror: Call block_set_size(NULL, %u)\n\n", (unsigned int)size);
        return 0;
    }
    
    fprintf(stderr, "\n\nError: Obsolete method block_set_size called\n\n");
    return 0;
}
int block_set_forward(block_t *block, void *forward)
{
    if(block == NULL)
    {
        fprintf(stderr, "\nerror: Call block_set_forward(NULL, %p)\n\n", forward);
        return 0;
    }
    
    block->forward = forward;
    return 0;
}
int block_set_type(block_t *block, byte type)
{
    if(block == NULL)
    {
        fprintf(stderr, "\nerror: Call block_set_type(NULL, %x)\n\n", type);
        return 0;
    }
    block->type = type;
    return 0;    
}
int block_set_array_size(block_t *block, size_t size)
{
    if(block == NULL)
    {
        fprintf(stderr, "\nerror: Call block_set_array_size(NULL, %u)\n\n", (unsigned int)size);
        return 0;
    }
    block->size = size;
    return 0;
}
int block_set_info(block_t *block, type_info_t *info)
{
    if(block == NULL)
    {
        fprintf(stderr, "\nerror: Call block_set_info(NULL, %p)\n\n", info);
        return 0;
    }
    
    fprintf(stderr, "\n\nError: Obsolete function block_set_info used\n\n");
    
    return 0;
}
int block_set_atom_is_ptr(block_t *block, int is_ptr)
{
    if(block == NULL)
    {
        fprintf(stderr, "\nerror: Call block_set_atom_is_ptr(NULL, %d)\n\n", is_ptr);
        return 0;
    }
    
    fprintf(stderr, "\n\nError: Obsolete function block_set_atom_is_ptr used\n\n");
    
    return 0;
}
int block_set_element_type(block_t *block, int type)
{
    if(block == NULL)
    {
        fprintf(stderr, "\nerror: Call block_set_element_type(NULL, %d)\n\n", type);
        return 0;
    }
    block->element_type = type;
    return 0;
}

/**
 * Predicates whetter the block has a forwarding addr set
 * @par block a memory block
 * @return 1 if the address is set, 0 otherwise
 */
int block_has_forward(block_t *block)
{
    return block->forward != NULL;
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
    block_set_array_size(dst, block_get_array_size(src));
    block_set_element_type(dst, block_get_element_type(src));
}

/**
 * Predicates whetter this block contains structures
 * @par block memory block
 * @return 1 if this block contains structures, 0 otherwise
 */
int block_is_struct_block(block_t *block)
{
    int type = block_get_element_type(block);
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
    
    return (block_t*)(ptr + sizeof(block_t) + block_get_size(block));
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
 * Initializes a block of given size from a larger chunk of raw memory
 * @par chunk oroginal chunk of memory
 * @par size of the memory chunk
 * @return initialized block
 */
block_t *init_block_from_chunk(void *chunk, size_t size)
{
    block_t *block;
    
    block = chunk;
    block_set_type(block, TYPE_UNDEFINED);
    block_set_array_size(block, size - sizeof(block_t));
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
        fprintf(stderr, "\n\nerror: split_block called with non-raw block: split_block(%p, %u)\n\n", src, (unsigned)size);
        return NULL;
    }
    
    aligned_size = align_size(size);

    if(src == NULL || *src == NULL || aligned_size >= (*src)->size)
    {
        return NULL;
    }
    remaining = block_get_size(*src) - aligned_size;

    ptr = get_data_start(*src);
    ptr = ptr + aligned_size;
            
    split = *src;
    *src = init_block_from_chunk(ptr, (*src)->size - aligned_size);
    block_set_type(split, TYPE_UNDEFINED);
    block_set_element_type(split, TYPE_UNDEFINED);
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

