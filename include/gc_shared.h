/**
 * @name gc_shared.h
 * @author Mgr. Radomir Skrabal
 * 
 * This file contains definitions of shared structures and functions among the garbage collectors
 */
#ifndef GC_SHARED_H
#define GC_SHARED_H

#include <stdlib.h>
#include <stdint.h>
#include "gc_types.h"

/**
 * Header for a logical memory block
 * @remark Convention size slot is available free space in the block.
 * Ergo size of the whole block is sizeof(block_t) + size bytes
 */
typedef struct
{
    uint64_t type;
    uint64_t size;
} block_t;

#define block_active(block) (block_has_forward(block) ? block_get_forward(block) : block)

/**
 * Getters
 * Quite primitive right now but will raise in importance during optimization phase
 */
// size_t block_get_size(block_t *block);
#define block_get_size_aux(block) (block_is_array(block) ? align_size(block_get_array_size(block) * block_get_info(block)->size) + 16 :                                  align_size(block_get_info(block)->size) + 8)
#define block_get_size(block) block_get_size_aux(block_active(block))

#define block_get_forward(block) ((void*)((block_t*)block)->size)
// void *block_get_forward(block_t *block);
#define block_get_type(block) (((block_t*)block)->type & ELEMENT_TYPE_BIT_MASK)
// uint64_t block_get_type(block_t *block);
#define BLOCK_GET_ARRAY_SIZE_AUX(block) (((block_t*)block)->size)
#define block_get_array_size(block) (BLOCK_GET_ARRAY_SIZE_AUX(block_active(block)))
// size_t block_get_array_size(block_t *block);
#define BLOCK_GET_INFO_AUX(block) (&type_table[block_get_type(block)])
#define block_get_info(block) (BLOCK_GET_INFO_AUX(block_active(block))) 
// type_info_t *block_get_info(block_t *block);
#define BLOCK_IS_ARRAY_AUX(block) ((((block_t*)block)->type & ARRAY_BIT_MASK) != 0)
#define block_is_array(block) (BLOCK_IS_ARRAY_AUX(block_active(block)))
// int block_is_array(block_t *block);

/**
 * Setters
 * Quite primitive right now but will raise in importance during optimization phase
 */
int block_set_size(block_t *block, size_t size);
int block_set_forward(block_t *block, void *forward);
int block_set_type(block_t *block, uint64_t type);
int block_set_array_size(block_t *block, size_t size);
int block_set_info(block_t *block, type_info_t *info);
int block_set_is_array(block_t *block, int is_array);

/**
 * Predicates whetter the block has a forwarding addr set
 * @par block a memory block
 * @return 1 if the address is set, 0 otherwise
 */
#define block_has_forward(block) (block_get_type(block) == TYPE_FORWARD)
// int block_has_forward(block_t *block);

/**
 * Copies the metadata of a block to other block
 * @par src source memory block
 * @par dst destination memory block
 * @return always 0
 * @remark Obsolete
 */
int copy_block_metadata(block_t *src, block_t *dst);

/**
 * Predicates whetter this block contains structures
 * @par block memory block
 * @return 1 if this block contains structures, 0 otherwise
 */
int block_is_struct_block(block_t *block);

/**
 * Gets next block of memory
 * @par block - pointer to the memory block
 * @return pointer to the next memory block
 */
#define next_block(block) (block_t*)((uint64_t)block + block_get_size(block))
// block_t *next_block(block_t *block);

/**
 * Call to system for get raw memory
 * @par size size of requested memory in bytes
 * @return addr to start of allocated memory or NULL
 */
void *get_memory_primitive(size_t size);

/**
 * Call to system for memory deallocation
 * @par ptr pointer to allocated memory
 * @return always 0
 */
int release_memory_primitive(void *ptr);

/**
 * Returns allocated memory from a block
 * @par block initialized block of memory
 * @return ptr to the begining of user usable memory
 */
#define get_data_start(block) (block_is_array(block) ? (void*)((uint64_t)block + 16) : (void*)((uint64_t)block + 8))
// void *get_data_start(block_t *block);

/**
 * Returns pointer just after the allocated memory of a block
 * @par block block of a memory
 * @return pointer after the allocated memory block
 */
#define get_data_end(block) ((void*)next_block(block))
// void *get_data_end(block_t *block);

/**
 * Alings integer to the nearest greater <TODO násobek> of the size
 * @par size original size to be aligned
 * @return aligned size
 */
// #define align_size(size) (size + (WORD_SIZE - (size & 0x7)))
size_t align_size(size_t size);

/**
 * Initializes a block of given size from a larger chunk of raw memory
 * @par chunk oroginal chunk of memory
 * @par size of the memory chunk
 * @return initialized block
 */
block_t *init_block_from_chunk(void *chunk, size_t size);

/**
 * Splits the block of memory
 * @par src original memory block (big)
 * @par size size of new memory block
 * @return new memory block of given size or NULL if failed
 * @remark reduces the size of src block
 */
block_t *split_block(block_t **src, size_t size);

/**
 * Predicates whetter the pointer points to the memory block
 * @par block block of a memory
 * @par ptr pointer
 * @return 1 if ptr points into block, 0 otherwise
 */
#define is_pointer_to(block, ptr) ((uint64_t)ptr >= (uint64_t)get_data_start(block) && (uint64_t)ptr < (uint64_t)get_data_end(block))
// int is_pointer_to(block_t *block, void *ptr);

/**
 * Bottom of a stack
 */
extern void *stack_bottom;
/**
 * Top of a stack
 */
extern void *stack_top;

/**
 * Start of the uninitialized data segment
 */
extern void *BBSstart;
/**
 * End of the uninitilized data segment
 */
extern void *BBSend;

/**
 * Macro for filling the stack_top variable with proper value
 * @remark Currently working only oon GNU Linux 
 */
#define REFRESH_STACK_TOP asm volatile ("mov %%rbp, %0" : "=r" (stack_top));

/**
 * Save current stack pointer as stack bottom
 * @remark Currently only for GNU Linux
 */ 
#define SET_STACK_BOTTOM asm volatile ("mov %%rbp, %0" : "=r" (stack_bottom));

/**
 * Returns ptr to the value of a structure slot givven by an offset
 * @par struct_ptr pointer to the allocated structure
 * @par offset offset to the slot from start of the structure
 */
void *get_ptr_to_slot(void *struct_ptr, unsigned long offset);

#endif

