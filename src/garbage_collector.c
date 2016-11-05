/**
 * @name garbage_collector.c
 * @author Mgr. Radomir Skrabal
 * This file contains implementation for unified API of benchmarked garbage collectors
 */

#include "gc_shared.h"
#include "gc_cheney.h"
#include <stdlib.h>

/**
* Macros definig used garbage collectors names
*/
#define NO_COLLECTOR 0
#define CHENEY_GC 1
 
 /**
 * Byte indicating which garbage collector is used
 */
char used_gc;

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_init()
{
  switch(used_gc)
  {
    case CHENEY_GC:
      return gc_cheney_init();
    case NO_COLLECTOR:
    default:
      return 0;  
  }
}

/**
* Allocate memory block for singe atomic value
* @par size size of the value in bytes
* @par is_pointer indicates whetter allocated value is pointer
* @return pointer to allocated memory or NULL
*/
void *gc_malloc_atom(size_t size, int is_pointer)
{
  switch(used_gc)
  {
    case CHENEY_GC:
      return gc_cheney_malloc_atom(size, is_pointer);
    case NO_COLLECTOR:
    default:
      return malloc(size);
  }
}

/**
 * Allocate memory block for single struct value
 * @par type pointer to the sturct type descriptor
 * @return pointer to allocated memory or NULL
 */
void *gc_malloc_struct(struct_info_t *type)
{
  switch(used_gc)
  {
    case CHENEY_GC:
      return gc_cheney_malloc_struct(type);
    case NO_COLLECTOR:
    default:
      return malloc(type->struct_size);
  }
}

/**
 * Allocates memory block for array of atomic values
 * @par number_of_elements number of elements in array
 * @par is_pointer flag that indicates whetter values in array are pointers
 * @return pointer to allocated memory or NULL
 */
void *gc_malloc_array_of_atoms(size_t number_of_elements, size_t atom_size, int is_pointer)
{
  switch(used_gc)
  {
    case CHENEY_GC:
      return gc_cheney_malloc_array_of_atoms(number_of_elements, atom_size, is_pointer);
    case NO_COLLECTOR:
    default:
      return malloc(number_of_elements * atom_size);
  }
}

/**
 * Allocates memory block for array of structures
 * @par number_of_elements number of elements in array
 * @par type pointer to the struct type descriptor
 * @return pointer to allocated memory or NULL
 */
void *gc_malloc_array_of_struct(size_t number_of_elements, struct_info_t *type)
{
  switch(used_gc)
  {
    case CHENEY_GC:
      return gc_cheney_malloc_array_of_struct(number_of_elements, type);
    case NO_COLLECTOR:
    default:
      return malloc(number_of_elements * type->struct_size);
  }
}

/**
 * Collects the memory
 * @return Always 0
 */
int gc_collect()
{
  switch(used_gc)
  {
    case CHENEY_GC:
      return gc_cheney_collect();;
    case NO_COLLECTOR:
    default:
      return 0;
  }
}