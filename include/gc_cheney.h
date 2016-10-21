/**
 * @name gc_cheney.h
 * @author Mgr. Radomir Skrabal
 * This file contains header for the functions used for implementation of Cheney copying garbage collector
 */ 

#ifndef GC_CHENEY_H
#define GC_CHENEY_H

#include "gc_shared.h"
#include <stdio.h>

/**
 * Size of the semispace in bytes
 */
#define SEMISPACE_SIZE 1024

/**
 * from space heap (active heap)
 */
extern block_t *from_space;
/**
 * to space heap (copy heap)
 */
extern block_t *to_space;

/**
 * Block containing remaining memory in active semispace
 */
extern block_t *remaining_block;

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_init();

/**
 * Returns pointer right after the end of semispace
 * @par semispace_ptr pointer to the start of a semispace
 * @return pointer right after end of semispace
 */
void* semispace_end(void *semispace_ptr);

/**
 * End user function for memory allocation
 * @par size memory to be allocated in bytes
 * @return pointer to allocated memory or NULL
 */
void *gc_malloc(size_t size);

/**
 * Collects the memory
 * @return Always 0
 */
 int gc_collect();
 
/**
 * Return forwarding address for a given pointer
 * @par ptr original pointer
 * @par src memory block to which the ptr points to
 * @par dst block to which ptr should be forwared
 * @return forwarding pointer for a ptr
 */
void *get_forwarding_addr(void *ptr, block_t* src, block_t *dst);
 
/**
 * Recursive method for walking the memory chunks to carry out the copying collection
 * @par start start pointer of a chunk
 * @par end pointer after the end of a chunk
 * @return Always 0
 */
int gc_walk_chunk(void *start, void *end);
 
/**
 * Swiches semispaces
 * @return always 0
 */
int gc_swich_semispaces();
#endif