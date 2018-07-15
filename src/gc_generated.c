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
#include "gc_generated.h"
#include "gc_constants.h"

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
    make_gc_walk_array(&type_table, TYPE_COUNT);
    
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
   //return (*gc_generated_main)();
    return gc_generated_collect_from_roots(gc_cheney_base_roots, gc_cheney_base_roots_count);
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
        jit_patch(p, current);
        *next = jit_bnei(p, JIT_FORWARD, R_TYPE, type_num);
        
        //Call for get_data_start(block)
        
        
        //jit_prepare(p);
        //jit_putargr(p, R_BLOCK); 
        //jit_call(p, get_data_start); //NOT A FUNCTION A MACRO!!
        //jit_retval(p, R_PTR);
        
        //Call for get_data_end(block)
        jit_prepare(p);
        jit_putargr(p, R_BLOCK);
        jit_call(p, get_data_end); //NOT A FUNCTION A MACRO!!
        jit_retval(p, R_LOOP);
        
        //for cyclle
        jit_label *for_label = jit_get_label(p);
        jit_op *loop_end = jit_bger_u(p, JIT_FORWARD, R_PTR, R_LOOP);
        
            jit_prepare(p);
            jit_putargi(p, type_num);
            jit_putargr(p, R_PTR);
            jit_call(p, gc_generated_scan_struct);
        
        jit_addi(p, R_PTR, R_PTR, info->size);
        jit_jmpi(p, for_label);
        jit_patch(p, loop_end);
        
        //Declare another switch end label
        *end = jit_jmpi(p, JIT_FORWARD);
    }
    return 0;
}

/**
 * Generates assembly code for waling arrayy of structurea
 * @par type_table types of structured to be insepcted
 * @par type_count count of types of structures
 * @returns Always 0
 **/
int make_gc_walk_array(type_info_t type_table[], size_t type_count)
{
    int i;
    struct jit * p;
    
    jit_op *current, *next, **ends;
    
    p = jit_init();
    jit_prolog(p, &gc_generated_walk_array);
    
    jit_prepare(p);
    jit_putargr(p, R_BLOCK);
    jit_call(p, block_is_struct_block);
    jit_retval(p, R_IF);
    
    jit_op *if_end = jit_beqi(p, JIT_FORWARD, R_IF, 0);
        
        JIT_BLOCK_GET_TYPE(p, R_TYPE, R_BLOCK);
        
        ends = (jit_op**)malloc(sizeof(jit_op*) * type_count);
        current = jit_blti_u(p, JIT_FORWARD, R_BLOCK, 0); //Necessary? how to initialize jit_op without calling jump?
        
        for(i = 0; i < type_count; i++)
        {
            make_gc_walk_array_per_type(p, &type_table[i], i, current, &next, &(ends[i]));
            current = next;            
        }
        jit_patch(p, current);
        
        for(i = 0; i < type_count; i++)
        {
            jit_patch(p, ends[i]);
        }
    
    jit_patch(p, if_end);
    
    jit_generate_code(p);
    //TODO Debug
    jit_dump_ops(p, JIT_DEBUG_CODE);
    jit_free(p);
    
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

void *gc_generated_scan_ptr(void *ptr, uint64_t type, int is_array)
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
}

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

/*
int gc_generated_walk_array(block_t *block)
{
	if(block_is_struct_block(block))
	{
		void *ptr;
		uint64_t type;
		type = block_get_type(block);
		switch(type)
		{
		case 4:
			for(ptr = get_data_start(block); ptr < get_data_end(block); ptr += 8)
				gc_generated_scan_struct(ptr, 4);
			break;
		case 7:
			for(ptr = get_data_start(block); ptr < get_data_end(block); ptr += 24)
				gc_generated_scan_struct(ptr, 7);
			break;
		case 8:
			for(ptr = get_data_start(block); ptr < get_data_end(block); ptr += 16)
				gc_generated_scan_struct(ptr, 8);
			break;
		case 9:
			for(ptr = get_data_start(block); ptr < get_data_end(block); ptr += 24)
				gc_generated_scan_struct(ptr, 9);
			break;
		case 6:
			for(ptr = get_data_start(block); ptr < get_data_end(block); ptr += 24)
				gc_generated_scan_struct(ptr, 6);
			break;
		case 10:
			for(ptr = get_data_start(block); ptr < get_data_end(block); ptr += 16)
				gc_generated_scan_struct(ptr, 10);
			break;
		case 11:
			for(ptr = get_data_start(block); ptr < get_data_end(block); ptr += 352)
				gc_generated_scan_struct(ptr, 11);
			break;
		}
	}
}*/
