/**
 * @name garbage_collector.h
 * @author Mgr. Radomir Skrabal
 * This file contains definitions for using multiple garbage collectors and provide unified API for allocating memory
 */
 
#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include "gc_types.h"

/**
 * Byte indicating which garbage collector is used
 */
extern char used_gc;

/**
 * Parses argument given by -g in command line
 * @par arg -g argument
 * @return garbage collector number or INVALID_INVALID
 */
int parse_gc_arg(const char *arg);

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_init();

/**
 * Cleans up the garbage collector objext
 * @return If everything went well 0, otherwise error code
 */
int gc_cleanup();

/**
 * Allocates memory for single (non-array) value
 * @par type c type
 * @return pointer to allocated memory or NULL
 */
#define gc_malloc(type) __gc_malloc(type_num(type))

/**
 * Allocates memory for single (non-array) value
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *__gc_malloc(int type);

/**
 * Allocates memory for an array of values
 * @par type c type
 * @return pointer to allocated memory or NULL
 */
#define gc_malloc_array(type, size) (type*) __gc_malloc_array(type_num(type), size)

/**
 * Allocates memory for an array of values
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *__gc_malloc_array(int type, size_t size);

/**
 * Collects the memory
 * @return Always 0
 */
int gc_collect();

#endif