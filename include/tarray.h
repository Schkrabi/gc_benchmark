/**
 * @name tarray.h
 * @author Mgr. Radomir Skrabal
 * This file contains headers and definitions for testing array struct
 */ 

#ifndef TARRAY_H
#define TARRAY_H

#include "gc_shared.h"
 
/**
 * Testing array struct
 */
typedef struct tarray
{
    size_t size;
    int *data;
} tarray_t;

/**
 * Makes and sets the tarray_t structure descriptor to the given descriptor pointer
 * @par info pointer to the initialized descriptor structure
 * @return 1 if everything went well 0 otherwise
 */
int tarray_make_descriptor(type_info_t *info);

/**
 * Initialize the tarray
 * @par size a size of the array
 * @returns Pointer to newly allocated tarray_t
 */
tarray_t *tarray_init(size_t size);

/**
 * Gets the item on index position
 * @par a - pointer to tarray_t struct
 * @par index - index of item to be retrieved
 * @returns A value on index or 0 if index is out of range
 * @note I know this is stupid convention, but it is for testing purposes only anyway.
 */
int tarray_item(tarray_t *a, size_t index);

/**
 * Sets the item on position
 * @par a - pointer to tarray_t struct
 * @par index - index of item to be set
 * @par item - a value to be set on index position
 * @returns True if succesfull, false otherwise (index out of range)
 */
int tarray_set(tarray_t *a, size_t index, int item);

#endif


