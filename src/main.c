/**
 * @name main.c
 * @author Mgr. Radomir Skrabal
 * Main entrypoint of the GC_Benchmark
 */
#include <stdio.h>
#include <stdlib.h>
#include "gc_shared.h"
#include "gc_cheney.h"

#define TEST_SIZE 3

/**
 * Main executed function
 */
int sub_main(int argc, char *argv[])
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
    
    not_marked = NULL;
    
    gc_collect();
    
    for(i = 0; i < TEST_SIZE; i++)
    {
        printf("%p\n", parray[i]);
    }
    
    printf("Dumping all active memory blocks\n");
    for(block = from_space; block < (block_t*)semispace_end((void*)from_space); block = next_block(block))
    {
        printf("block %p, size %u\n", block, (unsigned int)block->size);
    }

    printf("\n");  
    printf("%p\n", not_marked);
    
    return 0;
}

/**
 * Main entrypoint Garbage collector initial setup
 */
int main(int argc, char *argv[])
{
    SET_STACK_BOTTOM
    gc_init();
    
    return sub_main(argc, argv);
}