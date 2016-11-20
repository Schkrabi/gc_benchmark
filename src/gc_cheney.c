/**
 * @name gc_cheney.c
 * @author Mgr. Radomir Skrabal
 * This file contains implemetations of Cheneys copying garbage collector for gc_cheney.h
 */

#include "gc_cheney.h"
#include <unistd.h>
#include <string.h>

/**
 * from space heap (active heap)
 */
block_t *from_space;

/**
 * to space heap (copy heap)
 */
block_t *to_space;

/**
 * Block containing remaining memory in active semispace
 */
block_t *remaining_block;

/**
 * Ptr to the remaining portion fo to space during copy phase
 */
block_t *remaining_to_space;

/**
 * Roots used for automatic garbage collection
 */
void **gc_roots;

/**
 * Number of roots in gc_roots
 */
size_t gc_roots_count;

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_cheney_init()
{
    void *chunk;   

    chunk = get_memory_primitive(2*SEMISPACE_SIZE);
    if(chunk == NULL)
    {
        return 1;
    }
    to_space = init_block_from_chunk(chunk, 2*SEMISPACE_SIZE);
    if(to_space == NULL)
    {
        return 2;
    }
    from_space = split_block(&to_space, SEMISPACE_SIZE - sizeof(block_t));
    if(from_space == NULL)
    {
        return 3;
    }
    remaining_block = from_space;
    gc_roots_count = 0;
    
    return 0;
}

/**
 * Returns pointer right after the end of semispace
 * @par semispace_ptr pointer to the start of a semispace
 * @return pointer right after end of semispace
 */
void* semispace_end(void *semispace_ptr)
{
    return semispace_ptr + SEMISPACE_SIZE;
}

/**
 * Allocates a block of given size
 * @par size size of the new block
 * @return pointer to the memory block
 */
block_t *alloc_block_of_size(size_t size)
{
    block_t *block;
    
    block = split_block(&remaining_block, size);
    if(block == NULL)
    {
        gc_cheney_collect();
        block = split_block(&remaining_block, size);
        if(block == NULL)
        {
            fprintf(stderr, "Out of memory!");
        }
    }
    return block;
}

/**
 * Allocates memory for single (non-array) value
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *gc_cheney_malloc(int type)
{
    block_t *block;
    
    block = alloc_block_of_size(type_table[type].size);
    
    if(block == NULL)
    {
        return block;
    }
    
    block_set_type(block, type);
    block_set_element_type(block, type);
    block_set_array_size(block, 0);
    block_set_forward(block, NULL);
    
    return get_data_start(block);
}

/**
 * Allocates memory for an array of values
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *gc_cheney_malloc_array(int type, size_t size)
{
    block_t *block;
    
    block = alloc_block_of_size(type_table[type].size * size);
    
    if(block == NULL)
    {
        return block;
    }
    
    block_set_type(block, TYPE_ARRAY);
    block_set_element_type(block, type);
    block_set_array_size(block, size);
    block_set_forward(block, NULL);
    
    return get_data_start(block);
}


/**
 * Return forwarding address for a given pointer
 * @par ptr original pointer
 * @par src memory block to which the ptr points to
 * @par dst block to which ptr should be forwared
 * @return forwarding pointer for a ptr
 */
void *get_forwarding_addr(void *ptr, block_t* src, block_t *dst)
{
    long unsigned int ptr_ul, src_ul, dst_ul;
    ptr_ul = (long unsigned int) ptr;
    src_ul = (long unsigned int) src;
    dst_ul = (long unsigned int) dst;
    
    return (void*)(dst_ul + (ptr_ul - src_ul));
}

/**
 * Carries out the "sweep" part of the algorithm
 * @return 0 if everything went well, error code otherwise
 */
int gc_cheney_collect()
{
   return gc_collect_from_roots(gc_roots, gc_roots_count);
}

/**
 * Carries out the "sweep" part of the algorithm
 * @par roots Array of root elements for garbage collection
 * @par size size of a roots arraay
 * @return 0 if everything went well, error code otherwise
 */
int gc_collect_from_roots(void *roots[], size_t size)
{
    block_t *todo_ptr;
    int i;
    
    remaining_to_space = to_space;
    todo_ptr = to_space;
    
    for(i = 0; i < size; i++)
    {
      gc_scan_ptr(roots[i]);
    }                                                                   
    
    while(todo_ptr < remaining_to_space)
    {
      gc_walk_block(todo_ptr);
      todo_ptr = next_block(todo_ptr);
    }
    
    gc_swich_semispaces();
    return 0;
}

/**
 * Scans the pointer if it points towards any memory and evacuates if if so
 * @par ptr scanned pointer
 * @return 0 if pointer do not points anywhere, forwarding address otherwise
 */
void *gc_scan_ptr(void *ptr)
{
    block_t *block;
    
    for(block = from_space; block < (block_t*)semispace_end((void*)from_space); block = next_block(block))
    {
        if(is_pointer_to(block, ptr))
        {
            if(!block_has_forward(block))
            {
                block_t *dst;
                dst = split_block(&remaining_to_space, block_get_size(block));
                copy_block_metadata(block, dst);
                
                block_set_forward(block, dst);
                memcpy(get_data_start(dst), get_data_start(block), block_get_size(block));
                
                return dst;
            }
            else
            {
                return get_forwarding_addr(ptr, block, block_get_forward(block));
            }
        }
    }
    return NULL;
}

/**
 * Scans the chunk of memory containign pointers
 * @par start pointer to the start of memory chunk
 * @par end pointer behind the end of memory chunk
 * @return 0 if everything went well, error code otherwise
 */
int gc_scan_chunk(void *start, void *end)
{
    void **i;
    
    for(i = start; i < (void**)end; i++)
    {
        void *fwd;
        fwd = gc_scan_ptr(*i);
        
        if(fwd != NULL)
        {
            *i = fwd;
        }
    }
    return 0;
}

/**
 * Scans the structure in memory
 * @par ptr pointer to the structure
 * @par info descriptor of the structure
 */
int gc_scan_struct(void *ptr, type_info_t *info)
{
    int i;
    
    for(i = 0; i < info->number_of_references; i++)
    {
        void **slot_ptr, *fwd;
        slot_ptr = (void**)(ptr + info->offsets[i]);
        
        fwd = gc_scan_ptr(*slot_ptr);
        
        if(fwd != NULL)
        {
            *slot_ptr = fwd;
        }
    }
    return 0;
}

/**
 * Scans the (copied) block of memory and copies the references it points to to the to_space
 * @par block memory block to be scanned
 * @return if everything went well 0 otherwise error code
 */
int gc_walk_block(block_t *block)
{
    switch(block_get_type(block))
    {
        case TYPE_UNDEFINED:
        case TYPE_INT:
        case TYPE_DOUBLE:
            return 0;
        case TYPE_ARRAY:
            return gc_walk_array(block);
        default:
            return gc_walk_struct(block);
    }
}

/**
 * Scans the block of type MEM_TYPE_ATOM
 * @par block block of memory of type MEM_TYPE_ATOM
 * @return 0 if everything went well, error code otherwise
 */
int gc_walk_atom(block_t *block)
{
    if(block_atom_is_ptr(block))
    {
        return gc_scan_chunk(get_data_start(block), get_data_end(block));
    }
}
/**
 * Scans the block of type MEM_TYPE_STRUCT
 * @par block block of memory of type MEM_TYPE_STRUCT
 * @return 0 if everything went well, error code otherwise
 */
int gc_walk_struct(block_t *block)
{
    gc_scan_struct(get_data_start(block), block_get_info(block));
}
/**
 * Scans the block of type MEM_TYPE_ARRAY
 * @par block block of memory of type MEM_TYPE_ARRAY
 * @return 0 if everything went well, error code otherwise
 */
int gc_walk_array(block_t *block)
{
    if(block_is_struct_block(block))
    {
        void *ptr;
        type_info_t *info;
        
        info = block_get_info(block);
        for(ptr = get_data_start(block); ptr < get_data_end(block); ptr += info->size)
        {
            gc_scan_struct(ptr, info);
        }
    }
    else if(block_atom_is_ptr(block))
    {
        gc_scan_chunk(get_data_start(block), get_data_end(block));
    }
}

/**
 * Swiches semispaces
 * @return always 0
 */
int gc_swich_semispaces()
{
    block_t *tmp;
    tmp = to_space;
    to_space = from_space;
    from_space = tmp;
    remaining_block = remaining_to_space;
    remaining_to_space = to_space;
    return 0;
}
