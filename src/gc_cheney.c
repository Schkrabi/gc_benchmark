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
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_init()
{
    void *chunk;
#ifdef __gnu_linux__
    extern char __bss_start; // Provided by the linker.
#endif    

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

#ifdef __gnu_linux__        
    BBSstart = (void*)&__bss_start;
    
    BBSend = sbrk(0);
    if(BBSend == (void*)-1)
    {
        fprintf(stderr, "Unable to init BBSend\n");
        return 4;
    }
#else
    BBSstart = NULL;
    BBSend = NULL;
    stack_bottom = NULL;
#endif    
    
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
 * End user function for memory allocation
 * @par size memory to be allocated in bytes
 * @return pointer to allocated memory or NULL
 */
void *gc_malloc(size_t size)
{
    block_t *block;

    block = split_block(&remaining_block, size);

#ifdef DEBUG
    printf("malloc call: size: %d, allocated: size: %d, remaining block: size : %d\n", size, block->size, remaining_block->size);
#endif

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
 * @par src space from which data will be copied (from_space)
 * @par dst space to which memory blocks will be evacuated (to_space)
 * @return 0 if everything went well, error code otherwise
 */
int gc_collect()
{
    REFRESH_STACK_TOP
    remaining_to_space = to_space;
    gc_walk_chunk(stack_top, stack_bottom);
    gc_swich_semispaces();
    return 0;
}

/**
 * Recursive method for walking the memory chunks to carry out the copying collection
 * @par start start pointer of a chunk
 * @par end pointer after the end of a chunk
 * @return Always 0
 */
int gc_walk_chunk(void *start, void *end)
{
    void **i;
    
    for(i = start; i < (void**)end; i++)
    {
        void    *ptr;
        block_t *block;
        ptr = *i;
        for(block = from_space; block < (block_t*)semispace_end((void*)from_space); block = next_block(block))
        {
            if(is_pointer_to(block, ptr))
            {
                if(block->forward == NULL)
                {
                    block_t *dst;
                    dst = split_block(&remaining_to_space, block->size);
                    block->forward = dst;
                    memcpy(get_data_start(dst), get_data_start(block), block->size);
                    gc_walk_chunk(get_data_start(dst), get_data_end(dst));
                }
                else
                {
                    *i = get_forwarding_addr(ptr, block, (block_t*)block->forward);
                }
            }
        }
        
    }
    return 0;
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
