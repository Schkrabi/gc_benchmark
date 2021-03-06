/**
 * @name gc_cheney.h
 * @author Mgr. Radomir Skrabal
 * This file contains header for the functions used for implementation of Cheney copying garbage collector
 */ 

#ifndef GC_CHENEY_H
#define GC_CHENEY_H

#include "gc_shared.h"
#include "gc_types.h"
#include "gc_cheney_base.h"

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_cheney_init();

/**
 * Cleans up the garbage collectors objext
 * @return If everything went well 0, otherwise error code
 */
int gc_cheney_cleanup();

/**
 * Allocates memory for single (non-array) value
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *gc_cheney_malloc(int type);

/**
 * Allocates memory for an array of values
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *gc_cheney_malloc_array(int type, size_t size);

/**
 * Collects the memory
 * @return Always 0
 */
 int gc_cheney_collect();
 
 /**
 * Carries out the "sweep" part of the algorithm
 * @par roots Array of root elements for garbage collection
 * @par size size of a roots arraay
 * @return 0 if everything went well, error code otherwise
 */
int gc_cheney_collect_from_roots(root_ptr roots[], size_t size);
 
//TODO remove
/**
 * Return forwarding address for a given pointer
 * @par ptr original pointer
 * @par src memory block to which the ptr points to
 * @par dst block to which ptr should be forwared
 * @return forwarding pointer for a ptr
 */
// void *get_forwarding_addr(void *ptr, block_t* src, block_t *dst);

/**
 * Scans the pointer if it points towards any memory and evacuates if if so
 * @par ptr scanned pointer
 * @par type type to which the pointer is pointing
 * @par is_array indicates whetter the pointer is an array
 * @return 0 if pointer do not points anywhere, forwarding address otherwise
 */
void *gc_cheney_scan_ptr(void *ptr, uint64_t type, int is_array);

//TODO remove
/**
 * Scans the chunk of memory containign pointers
 * @par start pointer to the start of memory chunk
 * @par end pointer behind the end of memory chunk
 * @return 0 if everything went well, error code otherwise
 */
// int gc_cheney_scan_chunk(void *start, void *end);

/**
 * Scans the structure in memory
 * @par ptr pointer to the structure
 * @par info descriptor of the structure
 */
int gc_cheney_scan_struct(void *ptr, type_info_t *info);

/**
 * Scans the (copied) block of memory and copies the references it points to to the to_space
 * @par block memory block to be scanned
 * @return if everything went well 0 otherwise error code
 */
int gc_cheney_walk_block(block_t *block);

//TODO remove
/**
 * Scans the block of type MEM_TYPE_ATOM
 * @par block block of memory of type MEM_TYPE_ATOM
 * @return 0 if everything went well, error code otherwise
 */
// int gc_cheney_walk_atom(block_t *block);
/**
 * Scans the block of type MEM_TYPE_STRUCT
 * @par block block of memory of type MEM_TYPE_STRUCT
 * @return 0 if everything went well, error code otherwise
 */
int gc_cheney_walk_struct(block_t *block);
/**
 * Scans the block of type MEM_TYPE_ARRAY
 * @par block block of memory of type MEM_TYPE_ARRAY
 * @return 0 if everything went well, error code otherwise
 */
int gc_cheney_walk_array(block_t *block);
 
//TODO remove
/**
 * Swiches semispaces
 * @return always 0
 */
// int gc_swich_semispaces();

/**
 * Returns the remaining space in bytes that collector has available
 * @return space in bytes or -1 if collector is limmited only by system
 */
int64_t gc_cheney_remaining_space();
#endif