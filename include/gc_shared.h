/**
 * @name gc_shared.h
 * @author Mgr. Radomir Skrabal
 * 
 * This file contains definitions of shared structures and functions among the garbage collectors
 */
#ifndef GC_SHARED_H
#define GC_SHARED_H

#include <stdlib.h>

typedef char byte;

/**
 * Structure for keeping info about structures
 */
typedef struct
{
  /**
   * Number of references in the structure
   */
  size_t number_of_references;
  /**
   * Offsets from the structure pointer to the references
   */
  unsigned long *offsets;
} struct_info_t; 

/**
 * Header for a logical memory block
 * @remark Convention size slot is available free space in the block.
 * Ergo size of the whole block is sizeof(block_t) + size bytes
 */
typedef struct
{
    size_t size;
    /**
     * Forwarding address for copying algorthm
     */
    void *forward;
    /**
     * Type of an object in memory Atom/Structure/Array
     */
    byte type;
    /**
     * Pointer to the info about allocated structure
     * @remark for TYPE_ATOM memory is always NULL
     */
    struct_info_t *info;
} block_t;

/** 
 * Allocated memory types
 */
#define MEM_TYPE_ATOM 0
#define MEM_TYPE_STRUCT 1
#define MEM_TYPE_ARRAY 2

/**
 * Gets next block of memory
 * @par block - pointer to the memory block
 * @return pointer to the next memory block
 */
block_t *next_block(block_t *block);

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
 * Initializes a block of given size from a larger chunk of raw memory
 * @par pointer to the original chunk
 * @par size size of a new block
 * @return new start ptr of remaining raw memory chunk
 */
block_t *init_block_from_chunk(void *chunk, size_t size);

/**
 * Returns allocated memory from a block
 * @par block initialized block of memory
 * @return ptr to the begining of user usable memory
 */
void *get_data_start(block_t *block);

/**
 * Returns pointer just after the allocated memory of a block
 * @par block block of a memory
 * @return pointer after the allocated memory block
 */
void *get_data_end(block_t *block);

/**
 * Size of a word in the system
 */
#define WORD_SIZE sizeof(void*)

/**
 * Alings integer to the nearest greater <TODO násobek> of the size
 * @par size original size to be aligned
 * @return aligned size
 */
size_t align_size(size_t size);

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
int is_pointer_to(block_t *block, void *ptr);

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

