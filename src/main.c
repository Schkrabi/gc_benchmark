/**
 * @name main.c
 * @author Mgr. Radomir Skrabal
 * Main entrypoint of the GC_Benchmark
 */
#include <stdio.h>
#include <stdlib.h>
#include "gc_shared.h"
#include "garbage_collector.h"
#include "gc_cheney.h"
#include <string.h>
#include "binary_tree.h"
#include <time.h>
#include "cyclic_list.h"
#include "gc_util.h"

#define TEST_SIZE 20
#define NO_COLLECTOR 0
#define CHENEY_GC 1

typedef struct 
{
  int value;
  void *ptr1, *ptr2;
} test_struct_t;

int gc_test();
int btree_test();
int clist_test();
/**
 * Main executed function
 */
int sub_main(int argc, char *argv[]);

/**
 * Main entrypoint Garbage collector initial setup
 */
int main(int argc, char *argv[])
{
    int err_msg;
    struct_info_t btree_info, clist_info;
    
    SET_STACK_BOTTOM
    used_gc = CHENEY_GC;
    
    btree_make_descriptor(&btree_info);
    clist_make_descriptor(&clist_info);
    
    err_msg = gc_init();
    
    srand(time(0));
    
    if(err_msg != 0)
    {
            fprintf(stderr, "Initialization error, code %d\n", err_msg);
    }
    
    return sub_main(argc, argv);
}

/**
 * Main executed function
 */
int sub_main(int argc, char *argv[])
{
    gc_test();
    btree_test();
    clist_test();
    mem_dump(stdout);
    return 0;
}

int gc_test()
{
    struct_info_t *struct_info;
    test_struct_t test_instance;
    void **ptr_src1, **ptr_src2;
    int *atom_test, *array_test;
    test_struct_t *struct_test, *struct_array_test, *ptr_src3, *ptr_src4;
    int i;
    
    struct_info = (struct_info_t*)malloc(sizeof(struct_info_t));
    
    struct_info[0].number_of_references = 2;
    struct_info[0].offsets = malloc(2*sizeof(unsigned long));
    
    struct_info[0].offsets[0] = (unsigned long)&test_instance.ptr1 - (unsigned long)&test_instance;
    struct_info[0].offsets[1] = (unsigned long)&test_instance.ptr2 - (unsigned long)&test_instance;
    struct_info[0].struct_size = sizeof(test_struct_t);
    
    printf("Initial memory dump:\n");
    mem_dump(stdout);
    
    atom_test = gc_malloc_atom(sizeof(int), 0);
    struct_test = gc_malloc_struct(struct_info);
    array_test = gc_malloc_array_of_atoms(TEST_SIZE, sizeof(int), 0);
    struct_array_test = gc_malloc_array_of_struct(TEST_SIZE, struct_info);
    
    ptr_src1 = (void**)gc_malloc_atom(sizeof(void*), 1);
    *ptr_src1 = atom_test;
    
    ptr_src2 = (void**)gc_malloc_array_of_atoms(TEST_SIZE, sizeof(void*), 1);
    ptr_src2[0] = atom_test;
    ptr_src2[1] = struct_test;
    ptr_src2[2] = NULL;
    
    ptr_src3 = (test_struct_t*)gc_malloc_struct(struct_info);
    ptr_src3->ptr1 = NULL;
    ptr_src3->ptr2 = struct_test;
    
    ptr_src4 = (test_struct_t*)gc_malloc_array_of_struct(TEST_SIZE, struct_info);
    ptr_src4[0].ptr1 = array_test;
    ptr_src4[0].ptr2 = atom_test;
    ptr_src4[1].ptr1 = NULL;
    ptr_src4[1].ptr2 = atom_test;
    ptr_src4[2].ptr1 = struct_test;
    ptr_src4[2].ptr2 = NULL;
    
    printf("\nMemory dump after allocation:\n");
    mem_dump(stdout);
    
    gc_roots_count = 4;
    gc_roots = (void**)malloc(gc_roots_count*sizeof(void*));
    
    gc_roots[0] = ptr_src1;
    gc_roots[1] = ptr_src2;
    gc_roots[2] = ptr_src3;
    gc_roots[3] = ptr_src4;
    
    gc_collect();
    
    printf("\nMemory dump after collection:\n");
    mem_dump(stdout);
}

int btree_test()
{
    btree_t *btree;
    int i;
    long n;
    
    btree = NULL;
    
    printf("\nInserting:\n");
    for(i = 0; i < TEST_SIZE; i++)
    {
        n = rand()%TEST_SIZE;
        printf("inserting %d, is_succes?: %d\n", (int)n, btree_insert(&btree, n));
    }    
    
    printf("\nSearching:\n");
    for(i = 0; i < TEST_SIZE; i++)
    {
        n = rand()%TEST_SIZE;
        printf("searchig for %d, is succes?: %p\n", (int)n, btree_search(btree, n));
    }
    
    printf("\nDeleting:\n");
    for(i = 0; i < TEST_SIZE; i++)
    {
        n = rand()%TEST_SIZE;
        printf("deleteing %d, is success?: %d\n", (int)n, btree_delete(&btree, n));
    }
}

int clist_test()
{
    clist_t *clist;
    int i;
    long n;

    clist = NULL;
    
    printf("\nInserting:\n");
    for(i = 0; i < TEST_SIZE; i++)
    {
        n = rand()%TEST_SIZE;
        printf("inserting %d, is_succes?: %d\n", (int)n, clist_insert(&clist, n));
    }    
    
    printf("\nSearching:\n");
    for(i = 0; i < TEST_SIZE; i++)
    {
        n = rand()%TEST_SIZE;
        printf("searchig for %d, is succes?: %p\n", (int)n, clist_search(clist, n));
    }
    
    printf("\nDeleting:\n");
    for(i = 0; i < TEST_SIZE; i++)
    {
        n = rand()%TEST_SIZE;
        printf("deleteing %d, is success?: %d\n", (int)n, clist_delete(&clist, n));
    }
    
    return 0;
}