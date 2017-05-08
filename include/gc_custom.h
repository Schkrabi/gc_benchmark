/**
 * @name gc_custom.h
 * @author Mgr. Radomir Skrabal
 * This file contains headers and definitions for custom garbage collector based on cheneys GC
 */
#ifndef GC_CUSTOM_H
#define GC_CUSTOM_H

#include "gc_shared.h"
#include "gc_types.h"
#include "gc_cheney_base.h"

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_custom_init();

/**
 * Cleans up the garbage collectors objext
 * @return If everything went well 0, otherwise error code
 */
int gc_custom_cleanup();

/**
 * Allocates memory for single (non-array) value
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *gc_custom_malloc(int type);

/**
 * Allocates memory for an array of values
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *gc_custom_malloc_array(int type, size_t size);

/**
 * Carries out the "sweep" part of the algorithm
 * @return 0 if everything went well, error code otherwise
 */
int gc_custom_collect();

/**
 * Carries out the "sweep" part of the algorithm
 * @par roots Array of root elements for garbage collection
 * @par size size of a roots arraay
 * @return 0 if everything went well, error code otherwise
 */
int gc_custom_collect_from_roots(root_ptr roots[], size_t size);

/**
 * Scans the (copied) block of memory and copies the references it points to to the to_space
 * @par block memory block to be scanned
 * @return if everything went well 0 otherwise error code
 */
int gc_custom_walk_block(block_t *block);

/**
 * Scans the block of type MEM_TYPE_STRUCT
 * @par block block of memory of type MEM_TYPE_STRUCT
 * @return 0 if everything went well, error code otherwise
 */
int gc_custom_walk_struct(block_t *block);

/**
 * Scans the block of type MEM_TYPE_ARRAY
 * @par block block of memory of type MEM_TYPE_ARRAY
 * @return 0 if everything went well, error code otherwise
 */
int gc_custom_walk_array(block_t *block);

/**
 * Returns the remaining space in bytes that collector has available
 * @return space in bytes or -1 if collector is limmited only by system
 */
int64_t gc_custom_remaining_space();

/**
 * Scans the structure in memory
 * @par ptr pointer to the structure
 * @par type if of the structure
 * @remark Code for this function is auto-generated
 */
int gc_custom_scan_struct(void *ptr, int type);

/**
 * Scans the pointer if it points towards any memory and evacuates if if so
 * @par ptr scanned pointer
 * @par type type to which the pointer is pointing
 * @par is_array indicates whetter the pointer is an array
 * @return 0 if pointer do not points anywhere, forwarding address otherwise
 * @remark Code for this function is auto-generated
 */
void *gc_custom_scan_ptr(void *ptr, uint64_t type, int is_array);

#endif
