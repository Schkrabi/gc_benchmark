/**
 * @name gc_cheney_base.h
 * @author Mgr. Radomir Skrabal
 * This file contains headers and definitions for shared code of garbage collectors based on cheneys algorithm
 */

#include "gc_shared.h"
#include "gc_types.h"
#include "gc_constants.h"

/**
 * from space heap (active heap)
 */
extern block_t *gc_cheney_base_from_space;

/**
 * to space heap (copy heap)
 */
extern block_t *gc_cheney_base_to_space;

/**
 * Pointer towards middle of the semispaces
 * Used in determining end of semispace
 */
extern void *gc_cheney_base_semispace_middle;

/**
 * Block containing remaining memory in active semispace
 */
extern block_t *gc_cheney_base_remaining_block;

/**
 * Ptr to the remaining portion fo to space during copy phase
 */
extern block_t *gc_cheney_base_remaining_to_space;

/**
 * Roots used for automatic garbage collection
 */
extern void **gc_cheney_base_roots;

/**
 * Number of roots in gc_roots
 */
extern size_t gc_cheney_base_roots_count;

/**
 * Returns pointer right after the end of semispace
 * @par semispace_ptr pointer to the start of a semispace
 * @return pointer right after end of semispace
 */
void* gc_cheney_base_semispace_end(void *semispace_ptr);

/**
 * Returns pointer right after end of semispace
 * @par arbitary pointer from semispace
 * @return pointer right after end of semispace
 */
#define gc_cheney_base_semispace_limit(ptr) ((uint64_t)ptr < (uint64_t)gc_cheney_base_semispace_middle ? gc_cheney_base_semispace_middle : (void*)((uint64_t)gc_cheney_base_semispace_middle + SEMISPACE_SIZE))

/**
 * Allocates a block of given size
 * @par size size of the new block
 * @return pointer to the memory block
 */
block_t *gc_cheney_base_alloc_block_of_size(size_t size);

/**
 * Return forwarding address for a given pointer
 * @par ptr original pointer
 * @par src memory block to which the ptr points to
 * @par dst block to which ptr should be forwared
 * @return forwarding pointer for a ptr
 */
void *gc_cheney_base_get_forwarding_addr(void *ptr, block_t* src, block_t *dst);

/**
 * Swiches semispaces
 * @return always 0
 */
int gc_cheney_base_swich_semispaces();

/**
 * Returns the remaining space in bytes that collector has available
 * @return space in bytes or -1 if collector is limmited only by system
 */
int64_t gc_cheney_base_remaining_space();

/**
 * Splits the block of memory
 * @par src original memory block (big) OUT: 
 * @par size size of new memory block WITHOUT THE HEADER!!!
 * @return new memory block of given size or NULL if failed
 * @remark changes the address of src argument
 */
void* gc_cheney_base_get_mem(void **ptr, size_t size);