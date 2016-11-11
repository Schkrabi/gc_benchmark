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
#define SEMISPACE_SIZE 1048576 

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
 * Roots used for automatic garbage collection
 */
extern void **gc_roots;

/**
 * Number of roots in gc_roots
 */
extern size_t gc_roots_count;

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_cheney_init();

/**
 * Returns pointer right after the end of semispace
 * @par semispace_ptr pointer to the start of a semispace
 * @return pointer right after end of semispace
 */
void* semispace_end(void *semispace_ptr);

/**
 * Allocates a block of given size
 * @par size size of the new block
 * @return pointer to the memory block
 */
block_t *alloc_block_of_size(size_t size);

/**
 * Allocate memory block for singe atomic value
 * @par size size of the value in bytes
 * @par is_pointer indicates whetter allocated value is pointer
 * @return pointer to allocated memory or NULL
 */
void *gc_cheney_malloc_atom(size_t size, int is_pointer);

/**
 * Allocate memory block for single struct value
 * @par type pointer to the sturct type descriptor
 * @return pointer to allocated memory or NULL
 */
void *gc_cheney_malloc_struct(struct_info_t *type);

/**
 * Allocates memory block for array of atomic values
 * @par number_of_elements number of elements in array
 * @par is_pointer flag that indicates whetter values in array are pointers
 * @return pointer to allocated memory or NULL
 */
void *gc_cheney_malloc_array_of_atoms(size_t number_of_elements, size_t atom_size, int is_pointer);
/**
 * Allocates memory block for array of structures
 * @par number_of_elements number of elements in array
 * @par type pointer to the struct type descriptor
 * @return pointer to allocated memory or NULL
 */
void *gc_cheney_malloc_array_of_struct(size_t number_of_elements, struct_info_t *type);

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
int gc_collect_from_roots(void *roots[], size_t size);
 
/**
 * Return forwarding address for a given pointer
 * @par ptr original pointer
 * @par src memory block to which the ptr points to
 * @par dst block to which ptr should be forwared
 * @return forwarding pointer for a ptr
 */
void *get_forwarding_addr(void *ptr, block_t* src, block_t *dst);

/**
 * Scans the pointer if it points towards any memory and evacuates if if so
 * @par ptr scanned pointer
 * @return 0 if pointer do not points anywhere, forwarding address otherwise
 */
void *gc_scan_ptr(void *ptr);

/**
 * Scans the chunk of memory containign pointers
 * @par start pointer to the start of memory chunk
 * @par end pointer behind the end of memory chunk
 * @return 0 if everything went well, error code otherwise
 */
int gc_scan_chunk(void *start, void *end);

/**
 * Scans the structure in memory
 * @par ptr pointer to the structure
 * @par info descriptor of the structure
 */
int gc_scan_struct(void *ptr, struct_info_t *info);

/**
 * Scans the (copied) block of memory and copies the references it points to to the to_space
 * @par block memory block to be scanned
 * @return if everything went well 0 otherwise error code
 */
int gc_walk_block(block_t *block);

/**
 * Scans the block of type MEM_TYPE_ATOM
 * @par block block of memory of type MEM_TYPE_ATOM
 * @return 0 if everything went well, error code otherwise
 */
int gc_walk_atom(block_t *block);
/**
 * Scans the block of type MEM_TYPE_STRUCT
 * @par block block of memory of type MEM_TYPE_STRUCT
 * @return 0 if everything went well, error code otherwise
 */
int gc_walk_struct(block_t *block);
/**
 * Scans the block of type MEM_TYPE_ARRAY
 * @par block block of memory of type MEM_TYPE_ARRAY
 * @return 0 if everything went well, error code otherwise
 */
int gc_walk_array(block_t *block);
 
/**
 * Swiches semispaces
 * @return always 0
 */
int gc_swich_semispaces();
#endif