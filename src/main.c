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

int mem_dump()
{
    block_t *block;
    
    printf("Dumping all active memory blocks\n");
    for(block = from_space; block < (block_t*)semispace_end((void*)from_space); block = next_block(block))
    {
        printf("block %p, size %u\n", block, (unsigned int)block->size);
    }
}

typedef struct 
{
  int value;
  void *ptr1, *ptr2;
} test_struct_t;

/**
 * Main executed function
 */
int sub_main(int argc, char *argv[])
{
    /*block_t *roots[1];
    void **tmp1, **tmp2;
    int i;
    
    roots[0] = (block_t*)malloc(sizeof(block_t) + 2*sizeof(void*));
    
    roots[0]->size = 2*sizeof(void*);
    roots[0]->forward = NULL;
    
    tmp1 = (void*)(roots[0] + 1);
    tmp2 = tmp1 + 1;
    
    *tmp1 = gc_malloc(TEST_SIZE * sizeof(int*));
    *tmp2 = gc_malloc(TEST_SIZE * sizeof(int));
    *tmp2 = NULL;
    
    //for(i = 0; i < TEST_SIZE; i++)
    //{
    //  (*tmp1)[i] = gc_malloc(sizeof(int));
    //  *((*tmp1)[i]) = i;
    //}
    
    mem_dump();
    
    gc_collect_from_roots(roots, 1);
    
    
    mem_dump();*/
    
    struct_info_t *struct_info;
    test_struct_t test_instance;
    int i;
    
    struct_info = (struct_info_t*)malloc(sizeof(struct_info_t));
    
    struct_info[0].number_of_references = 2;
    struct_info[0].offsets = malloc(2*sizeof(unsigned long));
    
    struct_info[0].offsets[0] = (unsigned long)&test_instance.ptr1 - (unsigned long)&test_instance;
    struct_info[0].offsets[1] = (unsigned long)&test_instance.ptr2 - (unsigned long)&test_instance;
    
    printf("no of refs: %d\n", struct_info[0].number_of_references);
    printf("offsets:\n");
    for(i = 0; i < struct_info[0].number_of_references; i++)
    {
      printf("%p\n", struct_info[0].offsets[i]);
    }   
    
    return 0;
}

/**
 * Main entrypoint Garbage collector initial setup
 */
int main(int argc, char *argv[])
{
    int err_msg;
    
    SET_STACK_BOTTOM
    err_msg = gc_init();
    if(err_msg != 0)
    {
            printf("Initialization error, code %d\n", err_msg);
    }
    
    return sub_main(argc, argv);
}