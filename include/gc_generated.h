/**
 * @name gc_cheney.h
 * @author Mgr. Radomir Skrabal
 * This file contains header for the functions used for implementation of Generated copying garbage collector
 */ 

#ifndef GC_GENERATED_H
#define GC_GENERATED_H

#include "gc_shared.h"
#include "gc_types.h"
#include "gc_cheney_base.h"

/**
 * Pointer for hookup of dynamicaly generated code
 */
extern int (*gc_generated_main)();

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_generated_init();

/**
 * Cleans up the garbage collectors objext
 * @return If everything went well 0, otherwise error code
 */
int gc_generated_cleanup();

/**
 * Allocates memory for single (non-array) value
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *gc_generated_malloc(int type);

/**
 * Allocates memory for an array of values
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *gc_generated_malloc_array(int type, size_t size);

/**
 * Collects the memory
 * @return Always 0
 */
int gc_generated_collect();
 
 /**
 * Returns the remaining space in bytes that collector has available
 * @return space in bytes or -1 if collector is limmited only by system
 */
int64_t gc_generated_remaining_space();
#endif