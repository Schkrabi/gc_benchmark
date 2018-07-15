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
#include "jitlib.h"

#define R_PTR		(R(0))
#define R_TYPE		(R(1))
#define R_IS_ARRAY	(R(2))
#define R_BLOCK		(R(3))
#define R_V1		(R(4))
#define R_LOOP          (R(5))
#define R_IF            (R(6))

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

/**
 * Generates assembly code for walking array of structures per type
 * @par p jit compiler
 * @par info structure carrying information about the inspected type
 * @par type_num number of the type
 * @par current jit label for the current switch case
 * @par next jit label for the next switch case
 * @par end jit label for the end of the switch
 * @returns Always 0
 */
int make_gc_walk_array_per_type(struct jit *p, type_info_t *info, int type_num, jit_op *current, jit_op **next, jit_op **end);

/**
 * Generates assembly code for waling arrayy of structurea
 * @par type_table types of structured to be insepcted
 * @par type_count count of types of structures
 * @returns Always 0
 **/
int make_gc_walk_array(type_info_t type_table[], size_t type_count);

#define JIT_BLOCK_GET_TYPE(p, R_TARGET, R_SRC) \
    jit_ldr(p, R_TARGET, R_SRC, sizeof(uint64_t));\
    jit_andi(p, R_TARGET, R_TARGET, ELEMENT_TYPE_BIT_MASK)
    
#define JIT_BLOCK_HAS_FORWARD(p, R_DST, R_SRC) \
    JIT_BLOCK_GET_TYPE(p, R_DST, R_BLOCK); \
    jit_eqi(p, R_DST, R_DST, TYPE_FORWARD)
    
#define JIT_BLOCK_GET_FORWARD(p, R_WRK)\
    jit_addi(p, R_WRK, R_WRK, sizeof(uint64_t));\
    jit_ldr(p, R_WRK, R_WRK, sizeof(uint64_t))
    
//TODO prototype & move
int jit_block_active(struct jit *p)
{
    jit_op *after;
    JIT_BLOCK_HAS_FORWARD(p, R_IF, R_BLOCK);
    after = jit_beqi(p, JIT_FORWARD, R_IF, 0);
    JIT_BLOCK_GET_FORWARD(p, R_BLOCK);
    jit_movr(p, R_BLOCK, R_BLOCK); //Useless op. need it for label
    jit_patch(p, after);
    return 0;
}

#define JIT_BLOCK_IS_ARRAY(p, R_DST, R_SRC)\
    jit_block_active(p);\
    JIT_BLOCK_GET_TYPE(p, R_DST, R_SRC);\
    jit_andi(p, R_DST, R_DST, ARRAY_BIT_MASK);\
    jit_eqi(p, R_DST, R_DST, 0)
    
int jit_get_data_start(struct jit *p)
{
    jit_op *array, *end;
    
    JIT_BLOCK_IS_ARRAY(p, R_IS_ARRAY, R_BLOCK);
        array = jit_beqi(p, JIT_FORWARD, R_IS_ARRAY, 0);
    jit_addi(p, R_PTR, R_BLOCK, sizeof(uint64_t));
    end = jit_jmpi(p, JIT_FORWARD);
    jit_addi(p, R_PTR, R_BLOCK, 2*sizeof(uint64_t));
    jit_patch(p, array);
    jit_movr(p, R_PTR, R_PTR); //Useles op, need for label
    jit_patch(p, end);
}

///////////////////////////////////////////////////////////////////////////////
//                  HOOKUP POINTERS FOR GENERADED CODE                       //
///////////////////////////////////////////////////////////////////////////////
typedef int (* walk_array_ftype)(block_t *);
walk_array_ftype gc_generated_walk_array;

/////////////////////////
//PLACEHOLDER CODE HERE//
/////////////////////////
int gc_generated_collect_from_roots(root_ptr roots[], size_t size);
int gc_generated_walk_block(block_t *block);
int gc_generated_walk_struct(block_t *block);
int64_t gc_generated_remaining_space();
void *gc_generated_scan_ptr(void *ptr, uint64_t type, int is_array);
int gc_generated_scan_struct(void *ptr, int type);
//int gc_generated_walk_array(block_t *block);

#endif