/**
 * @name main.c
 * @author Mgr. Radomir Skrabal
 * Main entrypoint of the GC_Benchmark
 */
#include <stdio.h>
#include <stdlib.h>
#include "gc_shared.h"

/*
 * Plan is as following:
 *  1. Upon init allocate big chunk of memory and divide it into the from_space and to_space ptrs	OK
 *  2. Initialize one big block on from_space								OK
 *  3. Upon gc_malloc call reduce the required size from the chunk and offset the big block ptr		TODO add collection phase
 *  4. Implementing mark_from_chunk which marks blocks in heap from a memory chunk                OK
 *  5. Implementing gc_mark which proceeds mark phase                                             TODO check marking from stack && BSS
 *  6. Implementing gc_copy which copies semispaces
 *  7. Implementing gc_collect which calls gc_mark, gc_copy and swiches semispaces
 *  - not yet solving problem of using up all memory
 *  - ignoring static pointers in roots
 *  - allocating in advance, do not take into account programs that do not allocate at all
 *  - iterating throught the heap via size, ie calling next_block until complete size overflows
 */

#define TEST_SIZE 3

int main(int argc, char *argv[])
{
    int err_msg, i;
    void **parray;
    int *not_marked;
    block_t *block;
    
    err_msg = gc_init();
    if(err_msg != 0)
    {
            printf("Initialization error, code %d\n", err_msg);
    }
    
    parray = gc_malloc(TEST_SIZE*sizeof(void*));
    not_marked = gc_malloc(TEST_SIZE*sizeof(int));
    
    for(i = 0; i < TEST_SIZE; i++)
    {
        parray[i] = gc_malloc(sizeof(int));
    }
    
    gc_mark();
    
    for(i = 0; i < TEST_SIZE; i++)
    {
        printf("%p\n", parray[i]);
    }
    
    printf("Dumping all active memory blocks\n");
    for(block = from_space; block < (block_t*)semispace_end((void*)from_space); block = next_block(block))
    {
        printf("block %p, size %u, marked %d\n", block, block->size, block->marked);
    }

    printf("\n");    

    return 0;
}