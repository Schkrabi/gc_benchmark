/**
 * @name gc_cheney_base.c
 * @author Mgr. Radomir Skrabal
 * THis file contains implementations of shared functions and variables shared by garbage collectors based on Cheney'c GC
 */

#include "gc_cheney_base.h"
#include "garbage_collector.h"
#include "gc_util.h"
#include <syslog.h>

/**
 * from space heap (active heap)
 */
block_t *gc_cheney_base_from_space;

/**
 * to space heap (copy heap)
 */
block_t *gc_cheney_base_to_space;

/**
 * Pointer towards middle of the semispaces
 * Used in determining end of semispace
 */
void *gc_cheney_base_semispace_middle;

/**
 * Block containing remaining memory in active semispace
 */
block_t *gc_cheney_base_remaining_block;

/**
 * Ptr to the remaining portion fo to space during copy phase
 */
block_t *gc_cheney_base_remaining_to_space;

/**
 * Roots used for automatic garbage collection
 */
root_ptr *gc_cheney_base_roots;

/**
 * Number of roots in gc_roots
 */
size_t gc_cheney_base_roots_count;

/**
 * Returns pointer right after the end of semispace
 * @par semispace_ptr pointer to the start of a semispace
 * @return pointer right after end of semispace
 */
void* gc_cheney_base_semispace_end(void *semispace_ptr)
{
    return (void*)((art_ptr_t)semispace_ptr + SEMISPACE_SIZE);
}

/**
 * Allocates a block of given size
 * @par size size of the new block
 * @return pointer to the memory block
 */
block_t *gc_cheney_base_alloc_block_of_size(size_t size)
{
    block_t *block;
    
//     printf("Allocating on %p, size %u", gc_cheney_base_remaining_block, (unsigned)size + 16);
    
    block = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_block, size);
    if(block == NULL)
    {
        gc_collect();
        block = gc_cheney_base_get_mem((void**)&gc_cheney_base_remaining_block, size);
        if(block == NULL)
        {
//             printf("OUT OF MEMORY\n");
            gc_log(LOG_ERR, "Memory depleted");
        }
    }
//     printf(", Remaining block %p, semispace end %p\n", gc_cheney_base_remaining_block, gc_cheney_base_semispace_end(gc_cheney_base_from_space)); 
    return block;
}

/**
 * Return forwarding address for a given pointer
 * @par ptr original pointer
 * @par src memory block to which the ptr points to
 * @par dst block to which ptr should be forwared
 * @return forwarding pointer for a ptr
 */
void *gc_cheney_base_get_forwarding_addr(void *ptr, block_t* src, block_t *dst)
{
    long unsigned int ptr_ul, src_ul, dst_ul;
    ptr_ul = (long unsigned int) ptr;
    src_ul = (long unsigned int) src;
    dst_ul = (long unsigned int) dst;
    
    return (void*)(dst_ul + (ptr_ul - src_ul));
}

/**
 * Swiches semispaces
 * @return always 0
 */
int gc_cheney_base_swich_semispaces()
{
    block_t *tmp;
    
    tmp = gc_cheney_base_to_space;
    gc_cheney_base_to_space = gc_cheney_base_from_space;
    gc_cheney_base_from_space = tmp;
    gc_cheney_base_remaining_block = gc_cheney_base_remaining_to_space;
    gc_cheney_base_remaining_to_space = gc_cheney_base_to_space;
    
    block_set_type(gc_cheney_base_to_space, TYPE_UNDEFINED);
    block_set_is_array(gc_cheney_base_to_space, 1);
    block_set_array_size(gc_cheney_base_to_space, SEMISPACE_SIZE - sizeof(block_t));
    
    return 0; 
}

/**
 * Returns the remaining space in bytes that collector has available
 * @return space in bytes or -1 if collector is limmited only by system
 */
int64_t gc_cheney_base_remaining_space()
{
    return (uint64_t)gc_cheney_base_semispace_limit((void*)gc_cheney_base_remaining_block) - (uint64_t)gc_cheney_base_remaining_block;
}

/**
 * Splits the block of memory
 * @par src original memory block (big) OUT: 
 * @par size size of new memory block WITHOUT THE HEADER!!!
 * @return new memory block of given size or NULL if failed
 * @remark changes the address of src argument
 */
void* gc_cheney_base_get_mem(void **ptr, size_t size)
{
    void *tmp, *ret;
    
    tmp = (void*)((uint64_t)*ptr + align_size(size) + sizeof(block_t));
    
    if((uint64_t)tmp >= (uint64_t)gc_cheney_base_semispace_limit(*ptr))
    {
        return NULL;
    }
    
    ret = *ptr;
    *ptr = tmp;
    
    //printf("Moved gc_cheney_base_to_space from %p to %p\n", ret, *ptr);
    
    return ret;
}
