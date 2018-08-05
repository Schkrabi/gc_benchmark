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
#include <inttypes.h>
#include "gc_cheney_base.h"
#include "gc_generated.h"
#include "gc_constants.h"

/**
 * Jit compilers
 */
struct jit *jit_gc_walk_array;
struct jit *jit_gc_scan_ptr;

 jit_op 	*__jit_op_macro_after1,
        *__jit_op_macro_after2,
        *__jit_op_macro_after3;

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
    
    //Generate assembly code
    jit_gc_walk_array = jit_init();
    make_gc_walk_array(jit_gc_walk_array, type_table, TYPE_COUNT);

    jit_gc_scan_ptr = jit_init();
    make_gc_scan_ptr(jit_gc_scan_ptr, type_table, TYPE_COUNT);
    
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
	
    //Release generated assembly code
    jit_free(jit_gc_walk_array);
    jit_free(jit_gc_scan_ptr);
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
   //return (*gc_generated_main)();
    return gc_generated_collect_from_roots(gc_cheney_base_roots, gc_cheney_base_roots_count);
}

/**
 * Auxiliary function to compute size of atom allocation during dynamic code generation
 * @par info info structure about type
 * @ret size of the allocation in bytes
 */
size_t atom_alloc_size(type_info_t *info)
{
    if(info->size < sizeof(uint64_t))
    {
        return 0;
    }
    return info->size - sizeof(uint64_t);
}

/**
 * Generates assembly code for walking array of structures
 * @par p jit compiler
 * @par info structure carrying information about the inspected type
 * @par type_num number of the type
 * @par current jit label for the current switch case
 * @par next jit label for the next switch case
 * @par end jit label for the end of the switch
 * @returns Always 0
 */
int make_gc_walk_array_per_type(struct jit *p, type_info_t *info, int type_num, jit_op *current, jit_op **next, jit_op **end)
{
    if(info->number_of_references > 0)
    {        
        //Patch the current switch label and forwared declare the next        
        if(current != NULL)
        {
            jit_patch(p, current);
        }
	*next = jit_bnei(p, JIT_FORWARD, R_TYPE, type_num);
        
        //Call for get_data_start(block)
        JIT_GET_DATA_START(p, R_PTR, R_BLOCK);
        
        //Call for get_data_end(block)
        JIT_GET_DATA_END(p, R_LOOP, R_IF, R_IS_ARRAY, R_BLOCK); //TODO AUX registers
        
        //for cyclle
        jit_label *for_label = jit_get_label(p);
        jit_op *loop_end = jit_bger_u(p, JIT_FORWARD, R_PTR, R_LOOP);
        
            //jit_msg(p, "loop");
            jit_prepare(p);
            jit_putargr(p, R_PTR);
            jit_putargi(p, type_num);
            jit_call(p, gc_generated_scan_struct);
        
        jit_addi(p, R_PTR, R_PTR, info->size);
        jit_jmpi(p, for_label);
        
        //jit_msg(p, "loop finished");
        
        jit_patch(p, loop_end);
        //Declare another switch end label
        *end = jit_jmpi(p, JIT_FORWARD);
    }
    return 0;
}

/**
 * Generates assembly code for waling arrayy of structurea
 * @par type_table types of structured to be inspected
 * @par type_count count of types of structures
 * @returns Always 0
 **/
int make_gc_walk_array(struct jit *p, type_info_t type_table[], size_t type_count)
{
	int i;
	jit_op 	*current, 
		*next, 
		**ends;

	jit_prolog(p, &gc_generated_walk_array);

	jit_declare_arg(p, JIT_PTR, sizeof(void*));
	jit_getarg(p, R_BLOCK, 0);

	jit_prepare(p);
	jit_putargr(p, R_BLOCK);
	jit_call(p, block_is_struct_block);
	jit_retval(p, R_IF);

	jit_op *if_end = jit_beqi(p, JIT_FORWARD, R_IF, 0x0);

	JIT_BLOCK_GET_TYPE(p, R_TYPE, R_BLOCK);

	ends = (jit_op**)calloc(type_count, sizeof(jit_op*));
	current = NULL;
	next = NULL;

	for(i = 0; i < type_count; i++)
	{
	    make_gc_walk_array_per_type(p, &type_table[i], i, current, &next, &(ends[i]));
	    if(next != NULL)
	    {
		current = next;
		next = NULL;            
	    }
	}
	jit_patch(p, current);

	for(i = 0; i < type_count; i++)
	{
	    if(ends[i] != NULL)
	    {
		jit_patch(p, ends[i]);
	    }
	}
	free(ends); 

jit_patch(p, if_end);

	jit_reti(p, 0x0);

	//jit_check_code(p, JIT_WARN_ALL); //TODO remove
	jit_generate_code(p);
	//jit_dump_ops(p, JIT_DEBUG_OPS); //TODO remove
   
	return 0;    
}

/**
 * Generates assembly code for gc_generated_scan_ptr per atom of type
 * @par p jit compiler
 * @par info pointer to the structure with type info
 * @par type_num number of the type in system
 * @par current jit label to patch for switch code
 * @par next pointer to the jit label for next part of the switch
 * @par pointer to jit label for the end of the switch
 */
int make_gc_scan_ptr_per_type_atom(struct jit *p, type_info_t *info, int type_num, jit_op *current, jit_op **next, jit_op **end)
{
	//INPUT
	// R_BLOCK - ptr to block
	// R_TYPE - type num

	//USED R_IF

	//OUTPUT
	// R_IF - pointer where data was copied to
	
	//Patch the current switch label and forwared declare the next        
	if(current != NULL)
	{
	    jit_patch(p, current);
	}

	*next = jit_bnei(p, JIT_FORWARD, R_TYPE, type_num);

	//call gc_cheney_base_get_mem	
	jit_prepare(p);
	jit_putargi(p, &gc_cheney_base_remaining_to_space);
	jit_putargi(p, atom_alloc_size(info));
	jit_call(p, gc_cheney_base_get_mem);
	jit_retval(p, R_IF); 

	//call memcpy
	jit_prepare(p);
	jit_putargr(p, R_IF);
	jit_putargr(p, R_BLOCK);
	jit_putargi(p, (uint64_t)(atom_alloc_size(info) + sizeof(block_t)));
	jit_call(p, memcpy);

	//Declare another switch end label
	*end = jit_jmpi(p, JIT_FORWARD);

	return 0;
}

/**
 * Generates assembly code for gc_generated_scan_ptr per array of type
 * @par p jit compiler
 * @par info pointer to the structure with type info
 * @par type_num number of the type in system
 * @par current jit label to patch for switch code
 * @par next pointer to the jit label for next part of the switch
 * @par pointer to jit label for the end of the switch
 */
int make_gc_scan_ptr_per_type_array(struct jit *p, type_info_t *info, int type_num, jit_op *current, jit_op **next, jit_op **end)
{
	//INPUT
	// R_BLOCK - ptr to block
	// R_TYPE - type num
	
	//USED
	// R_IF
	// R_LOOP
	//OUTPUT
	// R_IF - pointer where data was copied to

	//Patch the current switch label and forwared declare the next        
	if(current != NULL)
	{
	    jit_patch(p, current);
	}
	*next = jit_bnei(p, JIT_FORWARD, R_TYPE, type_num);

	JIT_GET_ARRAY_SIZE_ACTIVE_BLOCK(p, R_LOOP, R_BLOCK);
	
	jit_muli(p, R_LOOP, R_LOOP, (uint64_t)(info->size));
	
	jit_prepare(p);
	jit_putargi(p, (void**)&gc_cheney_base_remaining_to_space);
	jit_putargr(p, R_LOOP);
	jit_call(p, gc_cheney_base_get_mem);
	jit_retval(p, R_IF);
	
	jit_addi(p, R_LOOP, R_LOOP, (uint64_t)sizeof(block_t));

	jit_prepare(p);
	jit_putargr(p, R_IF);
	jit_putargr(p, R_BLOCK);
	jit_putargr(p, R_LOOP);
	jit_call(p, memcpy);

	//Declare another switch end label
	*end = jit_jmpi(p, JIT_FORWARD);
	
	return 0;
}

/**
 * Generates assembly code for gc_generated_scan_ptr 
 * @par p jit compiler
 * @par type_table table with type descriptors
 * @par type_count number of types in system
 */
int make_gc_scan_ptr(struct jit *p, type_info_t type_table[], size_t type_count)
{
	//USED
	// R_PTR
	// R_BLOCK
	// R_IS_ARRAY
	// R_TYPE
	// R_IF

	int i;
	jit_op 	*old_mem, 
		*has_forward,
		*array,
		*current,
		*next,
		**ends,
		*atom_set_forwarding,
		*array_block,
		*not_array_block;

	//////////////////////////////////////HEADER///////////////////////////
	jit_prolog(p, &gc_generated_scan_ptr);

	jit_declare_arg(p, JIT_PTR, sizeof(void*));
	jit_declare_arg(p, JIT_UNSIGNED_NUM, sizeof(uint64_t));
	jit_declare_arg(p, JIT_SIGNED_NUM, sizeof(int));

	jit_getarg(p, R_PTR, 0);
	jit_getarg(p, R_IS_ARRAY, 2);

	////////////////////////////////////HEADER END/////////////////////////

	JIT_JMP_IS_OLD_MEM(p, old_mem, R_IF, R_PTR);
	jit_reti(p, NULL);

jit_patch(p, old_mem);

	//Compute block ptr
	not_array_block = jit_beqi(p, JIT_FORWARD, R_IS_ARRAY, 0x0);
	jit_subi(p, R_BLOCK, R_PTR, sizeof(block_t));
	array_block = jit_jmpi(p, JIT_FORWARD);
	
jit_patch(p, not_array_block);
	jit_subi(p, R_BLOCK, R_PTR, sizeof(uint64_t));

jit_patch(p, array_block);
	//Is block forwarded?
	jit_ldr(p, R_IF, R_BLOCK, sizeof(uint64_t));
	has_forward = jit_beqi(p, JIT_FORWARD, R_IF, TYPE_FORWARD);

	//Get type of a block
	jit_ldr(p, R_TYPE, R_BLOCK, sizeof(uint64_t));
	jit_andi(p, R_TYPE, R_TYPE, ELEMENT_TYPE_BIT_MASK);

	//Is block array block?
	jit_andi(p, R_IF, R_IF, ARRAY_BIT_MASK); //Block->type already in R_IF
	array = jit_beqi(p, JIT_FORWARD, R_IF, 0x0);
	
	//ATOM
	current = NULL;
	next = NULL;
	ends = (jit_op**)calloc(type_count, sizeof(jit_op*));
	for(i = 0; i < type_count; i++)
	{
		make_gc_scan_ptr_per_type_array(p, &type_table[i], i, current, &next, &ends[i]);
		
		if(next != NULL)
		{
			current = next;
			next = NULL;
		}
	}
	jit_patch(p, current);

	for(i = 0; i < type_count; i++)
	{
		if(ends[i] != NULL)
		{
			jit_patch(p, ends[i]);
		}
	}
	free(ends);
	
	atom_set_forwarding = jit_jmpi(p, JIT_FORWARD);

jit_patch(p, array);
	//ARRAY
	current = NULL;
	next = NULL;
	ends = (jit_op**)calloc(type_count, sizeof(jit_op*));
	for(i = 0; i < type_count; i++)
	{
		make_gc_scan_ptr_per_type_atom(p, &type_table[i], i, current, &next, &ends[i]);
		
		if(next != NULL)
		{
			current = next;
			next = NULL;
		}
	}
	jit_patch(p, current);

	for(i = 0; i < type_count; i++)
	{
		if(ends[i] != NULL)
		{
			jit_patch(p, ends[i]);
		}
	}
	free(ends);

jit_patch(p, atom_set_forwarding);

	//Set forwarding address to evacuated block
	jit_prepare(p);
	jit_putargr(p, R_BLOCK);
	jit_putargr(p, R_IF);
	jit_call(p, block_set_forward);

jit_patch(p, has_forward);
	jit_movr(p, R_IF, R_BLOCK);
	jit_addi(p, R_IF, R_IF, sizeof(uint64_t));
	jit_ldr(p, R_IF, R_IF, sizeof(uint64_t));

	jit_prepare(p);
	jit_putargr(p, R_PTR);
	jit_putargr(p, R_BLOCK);
	jit_putargr(p, R_IF);
	jit_call(p, gc_cheney_base_get_forwarding_addr);
	jit_retval(p, R_IF);
	jit_retr(p, R_IF);

	//////////////////////////////////////FOOTER///////////////////////////
	//jit_disable_optimization(p, JIT_OPT_ALL);
	//jit_check_code(p, JIT_WARN_ALL);
	jit_generate_code(p);
	//jit_dump_ops(p, JIT_DEBUG_OPS);
	////////////////////////////////////FOOTER END/////////////////////////
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//            TODO GENERATE CODE FOR FOLLOWING FUNCTIONS                     //
///////////////////////////////////////////////////////////////////////////////

/**
 * Carries out the "sweep" part of the algorithm
 * @par roots Array of root elements for garbage collection
 * @par size size of a roots arraay
 * @return 0 if everything went well, error code otherwise
 */
int gc_generated_collect_from_roots(root_ptr roots[], size_t size)
{
    block_t *todo_ptr;
    int i;
    
    gc_cheney_base_remaining_to_space = gc_cheney_base_to_space;
    todo_ptr = gc_cheney_base_to_space;
    
    for(i = 0; i < size; i++)
    {
      roots[i].ptr = gc_generated_scan_ptr(roots[i].ptr, TYPE_PTR, roots[i].is_array);
    }                                                                   
    
    while(todo_ptr < gc_cheney_base_remaining_to_space)
    {
      gc_generated_walk_block(todo_ptr);
      todo_ptr = next_block(todo_ptr);
    }
    
    gc_cheney_base_swich_semispaces();
    return 0;
}

/**
 * Scans the (copied) block of memory and copies the references it points to to the to_space
 * @par block memory block to be scanned
 * @return if everything went well 0 otherwise error code
 */
int gc_generated_walk_block(block_t *block)
{
    if(block_is_array(block))
    {
        return gc_generated_walk_array(block);
    }
    switch(block_get_type(block))
    {
        case TYPE_UNDEFINED:
        case TYPE_INT:
        case TYPE_DOUBLE:
            return 0;
        default:
            return gc_generated_walk_struct(block);
    }
}

/**
 * Scans the block of type MEM_TYPE_STRUCT
 * @par block block of memory of type MEM_TYPE_STRUCT
 * @return 0 if everything went well, error code otherwise
 */
int gc_generated_walk_struct(block_t *block)
{
    gc_generated_scan_struct(get_data_start(block), block_get_type(block));
}

/*void *gc_generated_scan_ptr(void *ptr, uint64_t type, int is_array)
{
	block_t *block;
	block = (block_t*)((art_ptr_t)ptr - (is_array ? 16 : 8));
	if(gc_cheney_base_is_old_mem(block))
	{
				if(!block_has_forward(block))
				{
					block_t *dst;
					if(is_array)
					{
						size_t byte_size;
						switch(block_get_type(block))
						{
						case 4:
							byte_size = block_get_array_size(block) * 8;
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, byte_size);
							memcpy(dst, block, byte_size + 16);
							break;
						case 7:
							byte_size = block_get_array_size(block) * 24;
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, byte_size);
							memcpy(dst, block, byte_size + 16);
							break;
						case 8:
							byte_size = block_get_array_size(block) * 16;
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, byte_size);
							memcpy(dst, block, byte_size + 16);
							break;
						case 9:
							byte_size = block_get_array_size(block) * 24;
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, byte_size);
							memcpy(dst, block, byte_size + 16);
							break;
						case 5:
							byte_size = block_get_array_size(block) * 8;
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, byte_size);
							memcpy(dst, block, byte_size + 16);
							break;
						case 2:
							byte_size = block_get_array_size(block) * 4;
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, byte_size);
							memcpy(dst, block, byte_size + 16);
							break;
						case 6:
							byte_size = block_get_array_size(block) * 24;
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, byte_size);
							memcpy(dst, block, byte_size + 16);
							break;
						case 10:
							byte_size = block_get_array_size(block) * 16;
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, byte_size);
							memcpy(dst, block, byte_size + 16);
							break;
						case 11:
							byte_size = block_get_array_size(block) * 352;
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, byte_size);
							memcpy(dst, block, byte_size + 16);
							break;
						default:
						{
							size_t block_size = block_get_size(block);
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, block_size - sizeof(block_t));
							memcpy(dst, block, block_size);
						}
						}
					}
					else
					{
						switch(block_get_type(block))
						{
						case 4:
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 0);
							memcpy(dst, block, 16);
							break;
						case 7:
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 16);
							memcpy(dst, block, 32);
							break;
						case 8:
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 8);
							memcpy(dst, block, 24);
							break;
						case 9:
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 16);
							memcpy(dst, block, 32);
							break;
						case 5:
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 0);
							memcpy(dst, block, 16);
							break;
						case 2:
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 0);
							memcpy(dst, block, 16);
							break;
						case 6:
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 16);
							memcpy(dst, block, 32);
							break;
						case 10:
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 8);
							memcpy(dst, block, 24);
							break;
						case 11:
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
							memcpy(dst, block, 360);
							break;
						default:
						{
							size_t block_size = block_get_size(block);
							dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, block_size - sizeof(block_t));
							memcpy(dst, block, block_size);
						}
						}
					}
					block_set_forward(block, dst);
				}
			return gc_cheney_base_get_forwarding_addr(ptr, block, block_get_forward(block));
	}
	return NULL;
}*/

int gc_generated_scan_struct(void *ptr, int type)
{
	block_t *block;
	void *dst, **scanned;
	switch(type)
	{
	case 4:
		scanned = (void**)((art_ptr_t)ptr + 0);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 0);
				memcpy(dst, block, 16);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		break;
	case 7:
		scanned = (void**)((art_ptr_t)ptr + 8);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 16);
				memcpy(dst, block, 32);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 16);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 16);
				memcpy(dst, block, 32);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		break;
	case 8:
		scanned = (void**)((art_ptr_t)ptr + 8);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 8);
				memcpy(dst, block, 24);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		break;
	case 9:
		scanned = (void**)((art_ptr_t)ptr + 8);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 16);
				memcpy(dst, block, 32);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 16);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 16);
				memcpy(dst, block, 32);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		break;
	case 6:
		scanned = (void**)((art_ptr_t)ptr + 8);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 0);
				memcpy(dst, block, 16);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 16);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 0);
				memcpy(dst, block, 16);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		break;
	case 10:
		scanned = (void**)((art_ptr_t)ptr + 8);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 16);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				size_t byte_size = block_get_array_size(block) * 4;
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, byte_size);
				memcpy(dst, block, byte_size + 16);
				block_set_forward(block, dst);
				*scanned = dst + 16;
			}
		}
		break;
	case 11:
		scanned = (void**)((art_ptr_t)ptr + 136);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 144);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 152);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 160);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 168);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 176);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 240);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 248);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 256);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 264);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 328);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 336);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		scanned = (void**)((art_ptr_t)ptr + 344);
		if(gc_cheney_base_is_old_mem(*scanned))
		{
			block = (block_t*)((art_ptr_t)*scanned - 8);
			if(block_has_forward(block))
			{
				*scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));
			}
			else
			{
				dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, 344);
				memcpy(dst, block, 360);
				block_set_forward(block, dst);
				*scanned = dst + 8;
			}
		}
		break;
	}
	return 0;
}
