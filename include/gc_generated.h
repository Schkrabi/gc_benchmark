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
#define R_ACTIVE	(R(4))
#define R_LOOP          (R(5))
#define R_IF            (R(6))

/**
 * Jit compilers
 */
extern struct jit *jit_gc_walk_array;
extern struct jit *jit_gc_scan_ptr;
extern struct jit *jit_gc_scan_struct;

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
 * Auxiliary function to compute size of atom allocation during dynamic code generation
 * @par info info structure about type
 * @ret size of the allocation in bytes
 */
size_t atom_alloc_size(type_info_t *info);

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
int make_gc_walk_array(struct jit *p, type_info_t type_table[], size_t type_count);

/**
 * Generates assembly code for gc_generated_scan_ptr per atom of type
 * @par p jit compiler
 * @par info pointer to the structure with type info
 * @par type_num number of the type in system
 * @par current jit label to patch for switch code
 * @par next pointer to the jit label for next part of the switch
 * @par pointer to jit label for the end of the switch
 */
int make_gc_scan_ptr_per_type_atom(struct jit *p, type_info_t *info, int type_num, jit_op *current, jit_op **next, jit_op **end);

/**
 * Generates assembly code for gc_generated_scan_ptr per array of type
 * @par p jit compiler
 * @par info pointer to the structure with type info
 * @par type_num number of the type in system
 * @par current jit label to patch for switch code
 * @par next pointer to the jit label for next part of the switch
 * @par pointer to jit label for the end of the switch
 */
int make_gc_scan_ptr_per_type_array(struct jit *p, type_info_t *info, int type_num, jit_op *current, jit_op **next, jit_op **end);

/**
 * Generates assembly code for gc_generated_scan_ptr 
 * @par p jit compiler
 * @par type_table table with type descriptors
 * @par type_count number of types in system
 */
int make_gc_scan_ptr(struct jit *p, type_info_t type_table[], size_t type_count);

/**
 * Genrates assembly code for gc_generated_scan_struct
 * @par p jit compiler
 * @par type_table table with type descriptors
 * @par type_count number of types in system
 */
int make_gc_scan_struct(struct jit *p, type_info_t type_table[], size_t type_count);

/**
 * Generates assembly code for gc_generated_struct per type
 * @par p jit compiler
 * @par info pointer to the structure with type info
 * @par type_num number of the type in system
 * @par current jit label to patch for switch code
 * @par next pointer to the jit label for next part of the switch
 * @par pointer to jit label for the end of the switch
 */
int make_gc_scan_struct_per_type(struct jit *p, type_info_t *info, int type_num, jit_op *current, jit_op **next, jit_op **end);

//global ptr to hookup the 	jit_op's in macros
 extern jit_op *__jit_op_macro_after1,
        *__jit_op_macro_after2,
        *__jit_op_macro_after3;

//global size variable for memcpy macro        
extern size_t __jit_memcpy_aux_size;

#define JIT_BLOCK_GET_TYPE(p, R_TARGET, R_SRC) \
    jit_ldr(p, R_TARGET, R_SRC, sizeof(uint64_t));\
    jit_andi(p, R_TARGET, R_TARGET, ELEMENT_TYPE_BIT_MASK)
    
#define JIT_JMP_BLOCK_HAS_FORWARD(p, jmp_label, R_DST, R_SRC) \
    JIT_BLOCK_GET_TYPE(p, R_DST, R_SRC); \
    jmp_label = jit_beqi(p, R_DST, R_DST, TYPE_FORWARD)
    
#define JIT_BLOCK_GET_FORWARD(p, R_DST, R_SRC)\
    jit_addi(p, R_DST, R_SRC, sizeof(uint64_t));\
    jit_ldr(p, R_DST, R_DST, sizeof(uint64_t))

#define JIT_BLOCK_ACTIVE(p, R_DST, R_SRC)\
	JIT_JMP_BLOCK_HAS_FORWARD(p, __jit_op_macro_after1, R_DST, R_SRC);\
	jit_movr(p, R_DST, R_SRC);\
	__jit_op_macro_after2 = jit_jmpi(p, JIT_FORWARD);\
	jit_patch(p, __jit_op_macro_after1);\
	JIT_BLOCK_GET_FORWARD(p, R_DST, R_SRC);\
	jit_patch(p, __jit_op_macro_after2)
	
#define JIT_JMP_BLOCK_IS_ARRAY_ACTIVE_BLOCK(p, jmp_label, R_DST, R_SRC)\
	jit_ldr(p, R_DST, R_SRC, sizeof(uint64_t));\
	jit_andi(p, R_DST, R_DST, ARRAY_BIT_MASK);\
	jmp_label = jit_bnei(p, JIT_FORWARD, R_DST, 0)
	
#define JIT_JMP_BLOCK_IS_ARRAY(p, jmp_label, R_DST, R_SRC)\
	JIT_BLOCK_ACTIVE(p, R_DST, R_SRC);\
	JIT_JMP_BLOCK_IS_ARRAY_ACTIVE_BLOCK(p, jmp_label, R_DST, R_DST)

#define JIT_GET_DATA_START(p, R_DST, R_SRC)\
	JIT_JMP_BLOCK_IS_ARRAY(p, __jit_op_macro_after1, R_DST, R_SRC);\
	jit_addi(p, R_DST, R_SRC, sizeof(uint64_t));\
	__jit_op_macro_after2 = jit_jmpi(p, JIT_FORWARD);\
	jit_patch(p, __jit_op_macro_after1);\
	jit_addi(p, R_DST, R_SRC, 2*sizeof(uint64_t));\
	jit_patch(p, __jit_op_macro_after2)
	
#define JIT_BLOCK_GET_INFO_ACTIVE_BLOCK(p, R_DST, R_SRC)\
	JIT_BLOCK_GET_TYPE(p, R_DST, R_SRC);\
	jit_muli(p, R_DST, R_DST, sizeof(type_info_t));\
	jit_addi(p, R_DST, R_DST, (art_ptr_t)&type_table)
	
#define JIT_BLOCK_GET_INFO(p, R_DST, R_SRC)\
	JIT_BLOCK_ACTIVE(p, R_DST, R_SRC);\
	JIT_BLOCK_GET_INFO_ACTIVE_BLOCK(p, R_DST, R_DST)
	
#define JIT_GET_TYPE_SIZE(p, R_DST, R_SRC)\
	jit_ldr(p, R_DST, R_SRC, sizeof(size_t))
	
#define JIT_GET_ARRAY_SIZE_ACTIVE_BLOCK(p, R_DST, R_SRC)\
	jit_movr(p, R_DST, R_SRC);\
	jit_addi(p, R_DST, R_DST, sizeof(uint64_t));\
	jit_ldr(p, R_DST, R_DST, sizeof(uint64_t))
	
#define JIT_GET_ARRAY_SIZE(p, R_DST, R_SRC)\
	JIT_BLOCK_ACTIVE(p, R_DST, R_SRC);\
	JIT_GET_ARRAY_SIZE_ACTIVE_BLOCK(p, R_DST, R_DST)
	
#define JIT_BLOCK_GET_SIZE_ACTIVE_BLOCK(p, R_DST, R_AUX, R_SRC)\
	JIT_BLOCK_GET_INFO_ACTIVE_BLOCK(p, R_AUX, R_SRC);\
	JIT_GET_TYPE_SIZE(p, R_DST, R_AUX);\
	JIT_JMP_BLOCK_IS_ARRAY_ACTIVE_BLOCK(p, __jit_op_macro_after1, R_AUX, R_SRC);\
	jit_movi(p, R_AUX, 8);\
	__jit_op_macro_after3 = jit_jmpi(p, JIT_FORWARD);\
	jit_patch(p, __jit_op_macro_after1);\
	JIT_GET_ARRAY_SIZE(p, R_AUX, R_SRC);\
	jit_mulr(p, R_DST, R_DST, R_AUX);\
	jit_movi(p, R_AUX, 16);\
	jit_patch(p, __jit_op_macro_after3);\
	jit_prepare(p);\
	jit_putargr(p, R_DST);\
	jit_call(p, align_size);\
	jit_retval(p, R_DST);\
	jit_addr(p, R_DST, R_DST, R_AUX)	
	
#define JIT_NEXT_BLOCK(p, R_DST, R_AUX, R_AUX2, R_SRC)\
        JIT_BLOCK_ACTIVE(p, R_AUX, R_SRC);\
        JIT_BLOCK_GET_SIZE_ACTIVE_BLOCK(p, R_DST, R_AUX2, R_AUX);\
        jit_addr(p, R_DST, R_DST, R_SRC)
        
#define JIT_GET_DATA_END(p, R_DST, R_AUX, R_AUX2, R_SRC)\
        JIT_NEXT_BLOCK(p, R_DST, R_AUX, R_AUX2, R_SRC)

#define JIT_SEMISPACE_LIMIT(p, R_DST, R_SRC)\
	jit_movi(p, R_DST, (uint64_t)gc_cheney_base_semispace_middle);\
	__jit_op_macro_after1 = jit_blti(p, JIT_FORWARD, R_SRC, (uint64_t)gc_cheney_base_semispace_middle);\
	jit_addi(p, R_DST, R_DST, SEMISPACE_SIZE);\
	jit_patch(p, __jit_op_macro_after1)

#define JIT_JMP_IS_OLD_MEM(p, label, R_DST, R_SRC)\
    jit_ldi(p, R_DST, &gc_cheney_base_from_space, sizeof(uint64_t));\
    __jit_op_macro_after1 = jit_bltr(p, JIT_FORWARD, R_SRC, R_DST);\
    jit_addi(p, R_DST, R_DST, SEMISPACE_SIZE);\
    label = jit_bltr(p, JIT_FORWARD, R_SRC, R_DST);\
    jit_patch(p, __jit_op_macro_after1)
    
#define JIT_MEMCPY_CONST_SIZE(p, R_DST_PTR, R_SRC_PTR, R_TMP, size)\
    for(__jit_memcpy_aux_size = 0; __jit_memcpy_aux_size < size; __jit_memcpy_aux_size += sizeof(uint64_t))\
    {\
        jit_ldxi_u(p, R_TMP, R_SRC_PTR, __jit_memcpy_aux_size, sizeof(uint64_t));\
        jit_stxi(p, __jit_memcpy_aux_size, R_DST_PTR, R_TMP, sizeof(uint64_t));\
    }
    
#define JIT_MEMCPY_DYNAMIC_SIZE(p, R_DST_PTR, R_SRC_PTR, R_TMP, R_SIZE)\
    __jit_op_macro_after1 = jit_get_label(p);\
    __jit_op_macro_after2 = jit_blei(p, JIT_FORWARD, R_SIZE, 0);\
        jit_subi(p, R_SIZE, R_SIZE, sizeof(uint64_t));\
        jit_ldxr_u(p, R_TMP, R_SRC_PTR, R_SIZE, sizeof(uint64_t));\
        jit_stxr(p, R_SIZE, R_DST_PTR, R_TMP, sizeof(uint64_t));\
        jit_jmpi(p, __jit_op_macro_after1);\
    jit_patch(p, __jit_op_macro_after2)

///////////////////////////////////////////////////////////////////////////////
//                  HOOKUP POINTERS FOR GENERADED CODE                       //
///////////////////////////////////////////////////////////////////////////////
typedef int (* walk_array_ftype)(block_t *);
extern walk_array_ftype gc_generated_walk_array;

typedef int (* scan_ptr_ftype)(void *, uint64_t, int);
extern scan_ptr_ftype gc_generated_scan_ptr;

typedef int (* scan_struct_ftype)(void*,int);
extern scan_struct_ftype gc_generated_scan_struct;

/////////////////////////
//PLACEHOLDER CODE HERE//
/////////////////////////
int gc_generated_collect_from_roots(root_ptr roots[], size_t size);
int gc_generated_walk_block(block_t *block);
int gc_generated_walk_struct(block_t *block);
int64_t gc_generated_remaining_space();
//void *gc_generated_scan_ptr(void *ptr, uint64_t type, int is_array);
//int gc_generated_scan_struct(void *ptr, int type);
//int gc_generated_walk_array(block_t *block);

#endif
