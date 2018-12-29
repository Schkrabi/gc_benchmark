/**
 * @name large_structure.c
 * @author Mgr. Radomir Skrabal
 * 
 * This file contains code for large structure type
 */
#include <stddef.h>
#include "large_structure.h"
#include "gc_constants.h"

#define XPTR_NUMS(x) x(1) x(2) x(3) x(4) x(5) x(6) x(7) x(8) x(9) x(10) x(11) x(12) x(13)

#define SET_OFFSET(NUM) info->references[NUM - 1].offset = offsetof(large_structure_t, ptr ## NUM); \
                        ptr_info_set_type(&info->references[NUM-1], TYPE_LARGE_STRUCTURE_T);

/**
 * Makes and sets the large structure descriptor to the given descriptor pointer
 * @par info pointer to the initialized descriptor structure
 * @return 1 if everything went well 0 otherwise
 */
int large_structure_make_descriptor(type_info_t *info)
{
    info->size = sizeof(large_structure_t);
    info->number_of_references = 13;
    info->references = (ptr_info_t*)malloc(13 * sizeof(ptr_info_t));
    
    XPTR_NUMS(SET_OFFSET)
    
    return 1;
}

#define FILL_PTR(NUM) q->ptr ## NUM = large_structure_roll_ptr(pool, pool_size);
/**
 * Initializes the pointer of large structure to reference pointers in pool
 * @par q initialized structure
 * @par pool array of large_structure_t pointers
 * @par pool_size size of the pool array
 * @return 1 if everything went well, 0 otherwise
 * @see CHANCE_FOR_NULL, large_structure_roll_ptr, large_structure_roll_uniform_ptr
 */
int large_structure_entangle(large_structure_t *q, large_structure_t **pool, size_t pool_size)
{
    XPTR_NUMS(FILL_PTR)
    
    return 1;
}

/**
 * Defines chance for pointer in large structure to be initialized to NULL
 */
#define CHANCE_FOR_NULL 0.5

/**
 * Chooses pointer from pool at random with chance of chosing NULL ptr
 * @par pool array of large_structure_t pointers
 * @par pool_size size of the pool array
 * @return Pointer from pool or NULL
 * @see CHANCE_FOR_NULL, large_structure_roll_ptr, large_structure_roll_uniform_ptr
 **/ 
large_structure_t* large_structure_roll_ptr(large_structure_t **pool, size_t pool_size)
{
    size_t roll;
    
    roll = rand()%100;
    
    if(roll < CHANCE_FOR_NULL * 100)
    {
        return NULL;
    }
    
    return large_structure_roll_uniform_ptr(pool, pool_size);
}

/**
 * Chooses uniformly at random pointer from pool
 * @par pool array of large_structure_t pointers
 * @par pool_size size of the pool array
 * @return Pointer from pool
 * @see CHANCE_FOR_NULL, large_structure_roll_ptr, large_structure_roll_uniform_ptr
 **/ 
large_structure_t* large_structure_roll_uniform_ptr(large_structure_t **pool, size_t pool_size)
{
    return pool[rand()%pool_size];
} 
