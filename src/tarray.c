/**
 * @name tarray.c
 * @author Mgr. Radomir Skrabal
 * This file contains code and implementation for testing array struct.
 */

#include <stdio.h>
#include <stdlib.h>
#include "tarray.h" 
#include "garbage_collector.h"
#include "gc_util.h"
#include "gc_constants.h"

/**
 * Makes and sets the tarray_t structure descriptor to the given descriptor pointer
 * @par info pointer to the initialized descriptor structure
 * @return 1 if everything went well 0 otherwise
 */
int tarray_make_descriptor(type_info_t *info)
{
    tarray_t sample;
    
    info->size = sizeof(tarray_t);
    info->number_of_references = 1;
    info->references = (ptr_info_t*)malloc(sizeof(ptr_info_t));
    info->references[0].offset = (uint64_t)((art_ptr_t)&sample.data - (art_ptr_t)&sample);
    ptr_info_set_type(&info->references[0], TYPE_INT);
    ptr_info_set_is_array(&info->references[0], 1);
}

/**
 * Initialize the tarray
 * @par size a size of the array
 * @returns Pointer to newly allocated tarray_t
 */
tarray_t *tarray_init(size_t size)
{
    tarray_t *tarray = gc_malloc(tarray_t);
    tarray->size = size;
    tarray->data = gc_malloc_array(int, size);
    return tarray;
}

/**
 * Gets the item on index position
 * @par a - pointer to tarray_t struct
 * @par index - index of item to be retrieved
 * @returns A value on index or 0 if index is out of range
 * @note I know this is stupid convention, but it is for testing purposes only anyway.
 */
int tarray_item(tarray_t *a, size_t index)
{
    if(index < 0 || index >= a->size)
        return 0;
    return a->data[index];
}

/**
 * Sets the item on position
 * @par a - pointer to tarray_t struct
 * @par index - index of item to be set
 * @par item - a value to be set on index position
 * @returns True if succesfull, false otherwise (index out of range)
 */
int tarray_set(tarray_t *a, size_t index, int item)
{
    if(index < 0 || index >= a->size)
        return 0;
    a->data[index] = item;
}
