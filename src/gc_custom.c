/**
 * @name gc_custom.c
 * @author Mgr. Radomir Skrabal
 * This file contains implemetations of Custom Cheneys copying garbage collector for gc_custom.h
 */

#include "gc_custom.h"
#include "gc_cheney_base.h"
#include "garbage_collector.h"
#include <unistd.h>
#include <string.h>
#include "gc_util.h"
#include <syslog.h>
#include "gc_cheney_base.h"
#include "graph.h"
#include "binary_tree.h"
#include "cyclic_list.h"
#include "cdouble_list.h"
#include "large_structure.h"
#include "tarray.h"
#include "generated_types.h"

#define __XGC_CUSTOM_TYPES(x)\
        x(TYPE_BTREE_T, btree_t)\
        x(TYPE_CLIST_T, clist_t)\
        x(TYPE_CDLIST_T, cdlist_t)\
        x(TYPE_TARRAY_T, tarray_t)\
        x(TYPE_LARGE_STRUCTURE_T, large_structure_t)\
        x(TYPE_GRAPH_NODE_T, graph_node_t)\
        x(TYPE_GRAPH_EDGE_T, graph_edge_t)\
        x(TYPE_GRAPH_T, graph_t)

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_custom_init()
{
    void *chunk;   

    chunk = get_memory_primitive(2*SEMISPACE_SIZE);
    if(chunk == NULL)
    {
        return 1;
    }
//     gc_cheney_base_to_space = init_block_from_chunk(chunk, (2*SEMISPACE_SIZE) - sizeof(block_t));
    gc_cheney_base_to_space = (block_t*)((uint64_t)chunk + SEMISPACE_SIZE);
    if(gc_cheney_base_to_space == NULL)
    {
        return 2;
    }
//     gc_cheney_base_from_space = gc_cheney_base_get_mem((void*)&gc_cheney_base_to_space, SEMISPACE_SIZE - sizeof(block_t));
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
int gc_custom_cleanup()
{
    void *ptr = gc_cheney_base_from_space > gc_cheney_base_to_space ? gc_cheney_base_to_space : gc_cheney_base_from_space;
    release_memory_primitive(ptr);
}

/**
 * Allocates memory for single (non-array) value
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *gc_custom_malloc(int type)
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
void *gc_custom_malloc_array(int type, size_t size)
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
 * Carries out the "sweep" part of the algorithm
 * @return 0 if everything went well, error code otherwise
 */
int gc_custom_collect()
{
   return gc_custom_collect_from_roots(gc_cheney_base_roots, gc_cheney_base_roots_count);
}

/**
 * Carries out the "sweep" part of the algorithm
 * @par roots Array of root elements for garbage collection
 * @par size size of a roots arraay
 * @return 0 if everything went well, error code otherwise
 */
int gc_custom_collect_from_roots(root_ptr roots[], size_t size)
{
    block_t *todo_ptr;
    int i;
    
    gc_cheney_base_remaining_to_space = gc_cheney_base_to_space;
    todo_ptr = gc_cheney_base_to_space;
    
    for(i = 0; i < size; i++)
    {
      roots[i].ptr = gc_custom_scan_ptr(roots[i].ptr, TYPE_PTR, roots[i].is_array);
    }                                                                   
    
    while(todo_ptr < gc_cheney_base_remaining_to_space)
    {
      gc_custom_walk_block(todo_ptr);
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
int gc_custom_walk_block(block_t *block)
{
    if(block_is_array(block))
    {
        return gc_custom_walk_array(block);
    }
    switch(block_get_type(block))
    {
        case TYPE_UNDEFINED:
        case TYPE_FORWARD:
        case TYPE_INT:
        case TYPE_DOUBLE:
            return 0;
        default:
            return gc_custom_walk_struct(block);
    }
}

/**
 * Scans the block of type MEM_TYPE_STRUCT
 * @par block block of memory of type MEM_TYPE_STRUCT
 * @return 0 if everything went well, error code otherwise
 */
int gc_custom_walk_struct(block_t *block)
{
    gc_custom_scan_struct(get_data_start(block), block_get_type(block));
}

/**
 * Returns the remaining space in bytes that collector has available
 * @return space in bytes or -1 if collector is limmited only by system
 */
int64_t gc_custom_remaining_space()
{
    return gc_cheney_base_remaining_space();
}

#define __GC_CUSTOM_SCAN_ARRAY_PTR_PER_TYPE(NUM, TYPE)\
    case NUM:\
        byte_size = block_get_array_size(block) * sizeof(TYPE);\
        dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, byte_size);\
        memcpy(dst, block, byte_size + 16);\
        break;

#define __GC_CUSTOM_SCAN_PTR_PER_TYPE(NUM, TYPE)\
    case NUM:\
        dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, sizeof(TYPE) - 8);\
        memcpy(dst, block, sizeof(TYPE) + 8);\
        break;

void *gc_custom_scan_ptr(void *ptr, uint64_t type, int is_array)
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
                        __GC_CUSTOM_SCAN_ARRAY_PTR_PER_TYPE(TYPE_PTR, void*)
                        __GC_CUSTOM_SCAN_ARRAY_PTR_PER_TYPE(TYPE_DOUBLE, double)
                        __GC_CUSTOM_SCAN_ARRAY_PTR_PER_TYPE(TYPE_INT, int)
                        
                        __XGC_CUSTOM_TYPES(__GC_CUSTOM_SCAN_ARRAY_PTR_PER_TYPE)
                        
                        XGENERATE_TYPES_BTREE(__GC_CUSTOM_GEN_BTREE_SCAN_ARRAY_PTR_PER_TYPE)
                        XGENERATE_TYPES_GRAPH(__GC_CUSTOM_GEN_GRAPH_SCAN_ARRAY_PTR_PER_TYPE)
						default:
                            fprintf(stderr, "Unrecognized type %u in gc_custom_scan_ptr\n", (unsigned)block_get_type(block));
						}
					}
					else
					{
						switch(block_get_type(block))
						{
						__GC_CUSTOM_SCAN_PTR_PER_TYPE(TYPE_PTR, void*)
                        __GC_CUSTOM_SCAN_PTR_PER_TYPE(TYPE_DOUBLE, double)
                        __GC_CUSTOM_SCAN_PTR_PER_TYPE(TYPE_INT, int)
                        
                        __XGC_CUSTOM_TYPES(__GC_CUSTOM_SCAN_PTR_PER_TYPE)
                        
                        XGENERATE_TYPES_BTREE(__GC_CUSTOM_GEN_BTREE_SCAN_PTR_PER_TYPE)
                        XGENERATE_TYPES_GRAPH(__GC_CUSTOM_GEN_GRAPH_SCAN_PTR_PER_TYPE)
						default:
                            fprintf(stderr, "Unrecognized type %u\n", (unsigned)block_get_type(block));
						}
					}
					block_set_forward(block, dst);
				}
			return gc_cheney_base_get_forwarding_addr(ptr, block, block_get_forward(block));
	}
	return NULL;
}

#define __GC_CUSTOM_SCAN_STRUCT_TYPE(OFFSET, TYPE)\
    scanned = (void**)((art_ptr_t)ptr + OFFSET);\
    if(gc_cheney_base_is_old_mem(*scanned)){\
        block = (block_t*)((art_ptr_t)*scanned - 8);\
        if(block_has_forward(block)){\
            *scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));\
        }\
        else{\
            dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, sizeof(TYPE) - 8);\
            memcpy(dst, block, sizeof(TYPE) + 8);\
            block_set_forward(block, dst);\
            *scanned = dst + 8;\
        }\
    }
    
#define __GC_CUSTOM_SCAN_STRUCT_ARRAY_TYPE(OFFSET, TYPE)\
    scanned = (void**)((art_ptr_t)ptr + OFFSET);\
    if(gc_cheney_base_is_old_mem(*scanned)) {\
        block = (block_t*)((art_ptr_t)*scanned - 16);\
        if(block_has_forward(block)){\
            *scanned = gc_cheney_base_get_forwarding_addr(*scanned, block, block_get_forward(block));\
        }\
        else{\
            size_t byte_size = block_get_array_size(block) * sizeof(TYPE);\
            dst = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_to_space, byte_size);\
            memcpy(dst, block, byte_size + 16);\
            block_set_forward(block, dst);\
            *scanned = dst + 16;\
        }\
    }\
    break;
    
#define __void_scan_struct\
    case TYPE_PTR:\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(0, void*)\
        break;\
        
#define __MAKE_GC_CUSTOM_SCAN_STRUCT_PER_TYPE(NUM, TYPE) __ ## TYPE ## _scan_struct

int gc_custom_scan_struct(void *ptr, int type)
{
	block_t *block;
	void *dst, **scanned;
	switch(type)
	{
	__void_scan_struct
	
	__XGC_CUSTOM_TYPES(__MAKE_GC_CUSTOM_SCAN_STRUCT_PER_TYPE)
	
	XGENERATE_TYPES_BTREE(__GC_CUSTOM_GEN_BTREE_MAKE_GC_SCAN_STRUCT_PER_TYPE)
    XGENERATE_TYPES_GRAPH(__GC_CUSTOM_GEN_GRAPH_MAKE_GC_SCAN_STRUCT_PER_TYPE)
    default:
        fprintf(stderr, "Unrecognized type %u in gc_custom_scan_struct\n", (unsigned)block_get_type(block));
            
	}
	return 0;
}

//Use X macro?
#define __GC_CUSTOM_WALK_ARRAY_PER_TYPE(NUM, TYPE)\
    case NUM:\
        for(ptr = get_data_start(block); ptr < get_data_end(block); ptr += sizeof(TYPE))\
            gc_custom_scan_struct(ptr, NUM);\
        break;

int gc_custom_walk_array(block_t *block)
{
	if(block_is_struct_block(block))
	{
		void *ptr;
		uint64_t type;
		type = block_get_type(block);
		switch(type)
		{
        __GC_CUSTOM_WALK_ARRAY_PER_TYPE(TYPE_PTR, void*)
        
        __XGC_CUSTOM_TYPES(__GC_CUSTOM_WALK_ARRAY_PER_TYPE)
        
        XGENERATE_TYPES_BTREE(__GC_CUSTOM_GEN_BTREE_WALK_ARRAY_PER_TYPE)
        XGENERATE_TYPES_GRAPH(__GC_CUSTOM_GEN_GRAPH_WALK_ARRAY_PER_TYPE)
        default:
            fprintf(stderr, "Unrecognized type %u in gc_custom_walk_array\n", (unsigned)block_get_type(block));
		}
	}
	return 0;
}
