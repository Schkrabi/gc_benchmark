/**
 * @name gc_cheney.c
 * @author Mgr. Radomir Skrabal
 * This file contains implemetations of Cheneys copying garbage collector for gc_cheney.h
 */

#include "gc_cheney.h"
#include "garbage_collector.h"
#include <unistd.h>
#include <string.h>
#include "gc_util.h"
#include <syslog.h>
#include "gc_cheney_base.h"

/**
 * Pointer for hookup of dynamicaly generated code
 */
int (*gc_generated_main)();

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_generated_init()
{
    void *chunk;   

    chunk = get_memory_primitive(2*SEMISPACE_SIZE);
    if(chunk == NULL)
    {
        return 1;
    }
    gc_cheney_base_to_space = (block_t*)((uint64_t)chunk + SEMISPACE_SIZE);
    if(gc_cheney_base_to_space == NULL)
    {
        return 2;
    }
    gc_cheney_base_from_space = chunk;
    
    gc_cheney_base_semispace_middle = gc_cheney_base_to_space;
    if(gc_cheney_base_from_space == NULL)
    {
        return 3;
    }
    gc_cheney_base_remaining_block = gc_cheney_base_from_space;
    gc_cheney_base_roots_count = 0;
    
    return 0;
}

/**
 * Cleans up the garbage collectors objext
 * @return If everything went well 0, otherwise error code
 */
int gc_generated_cleanup()
{
    void *ptr = gc_cheney_base_from_space > gc_cheney_base_to_space ? gc_cheney_base_to_space : gc_cheney_base_from_space;
    release_memory_primitive(ptr);
}

/**
 * Allocates memory for single (non-array) value
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *gc_generated_malloc(int type)
{
    block_t *block;
    
    if(type_table[type].size <= sizeof(uint64_t))
    {
        block = gc_cheney_base_alloc_block_of_size(0);
    }
    else
    {
        block = gc_cheney_base_alloc_block_of_size(type_table[type].size - sizeof(uint64_t));
    }
    
    if(block == NULL)
    {
        return block;
    }
    
    block_set_is_array(block, 0);
    block_set_type(block, type);
    block_set_array_size(block, 0);
    
    return get_data_start(block);
}

/**
 * Allocates memory for an array of values
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *gc_generated_malloc_array(int type, size_t size)
{
    block_t *block;
    
    block = gc_cheney_base_alloc_block_of_size(type_table[type].size * size);
    
    if(block == NULL)
    {
        return block;
    }
    
    block_set_is_array(block, 1);
    block_set_type(block, type);
    block_set_array_size(block, size);
    
    return get_data_start(block);
}

/**
 * Returns the remaining space in bytes that collector has available
 * @return space in bytes or -1 if collector is limmited only by system
 */
int64_t gc_generated_remaining_space()
{
    return gc_cheney_base_remaining_space();
}

/**
 * Carries out the "sweep" part of the algorithm
 * @return 0 if everything went well, error code otherwise
 */
int gc_generated_collect()
{
   return (*gc_generated_main)();
}