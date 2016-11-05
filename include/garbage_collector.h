/**
 * @name garbage_collector.h
 * @author Mgr. Radomir Skrabal
 * This file contains definitions for using multiple garbage collectors and provide unified API for allocating memory
 */
 
#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

/**
 * Byte indicating which garbage collector is used
 */
extern char used_gc;

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_init();

/**
* Allocate memory block for singe atomic value
* @par size size of the value in bytes
* @par is_pointer indicates whetter allocated value is pointer
* @return pointer to allocated memory or NULL
*/
void *gc_malloc_atom(size_t size, int is_pointer);

/**
 * Allocate memory block for single struct value
 * @par type pointer to the sturct type descriptor
 * @return pointer to allocated memory or NULL
 */
void *gc_malloc_struct(struct_info_t *type);

/**
 * Allocates memory block for array of atomic values
 * @par number_of_elements number of elements in array
 * @par is_pointer flag that indicates whetter values in array are pointers
 * @return pointer to allocated memory or NULL
 */
void *gc_malloc_array_of_atoms(size_t number_of_elements, size_t atom_size, int is_pointer);
/**
 * Allocates memory block for array of structures
 * @par number_of_elements number of elements in array
 * @par type pointer to the struct type descriptor
 * @return pointer to allocated memory or NULL
 */
void *gc_malloc_array_of_struct(size_t number_of_elements, struct_info_t *type);

/**
 * Collects the memory
 * @return Always 0
 */
int gc_collect();

#endif