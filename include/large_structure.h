/**
 * @name large_structure.h
 * @author Mgr. Radomir Skrabal
 *
 * This file contains headers and definitions for large structure and associated tests
 */

#ifndef LARGE_STRUCTURE_H
#define LARGE_STRUCTURE_H

#include <stdint.h>
#include "gc_shared.h"

typedef struct large_structure
{
    uint64_t int1, int2, int3, int4, int5, int6, int7, int8, int9, int10, int11, int12, int13, int14, int15, int16, int17;
    struct large_structure *ptr1, *ptr2, *ptr3, *ptr4, *ptr5, *ptr6;
    double double1, double2, double3, double4, double5, double6, double7;
    struct large_structure *ptr7, *ptr8, *ptr9, *ptr10;
    char str[53];
    struct large_structure *ptr11, *ptr12, *ptr13;
} large_structure_t;

/**
 * Makes and sets the large structure descriptor to the given descriptor pointer
 * @par info pointer to the initialized descriptor structure
 * @return 1 if everything went well 0 otherwise
 */
int large_structure_make_descriptor(type_info_t *info);

/**
 * Initializes the pointer of large structure to reference pointers in pool
 * @par q initialized structure
 * @par pool array of large_structure_t pointers
 * @par pool_size size of the pool array
 * @return 1 if everything went well, 0 otherwise
 * @see CHANCE_FOR_NULL, large_structure_roll_ptr, large_structure_roll_uniform_ptr
 */
int large_structure_entangle(large_structure_t *q, large_structure_t **pool, size_t pool_size);

/**
 * Chooses pointer from pool at random with chance of chosing NULL ptr
 * @par pool array of large_structure_t pointers
 * @par pool_size size of the pool array
 * @return Pointer from pool or NULL
 * @see CHANCE_FOR_NULL, large_structure_roll_ptr, large_structure_roll_uniform_ptr
 **/ 
large_structure_t* large_structure_roll_ptr(large_structure_t **pool, size_t pool_size);

/**
 * Chooses uniformly at random pointer from pool
 * @par pool array of large_structure_t pointers
 * @par pool_size size of the pool array
 * @return Pointer from pool
 * @see CHANCE_FOR_NULL, large_structure_roll_ptr, large_structure_roll_uniform_ptr
 **/ 
large_structure_t* large_structure_roll_uniform_ptr(large_structure_t **pool, size_t pool_size);

#endif
