/**
 * @name test.c
 * @author Mgr. Radomir Skrabal
 * This file contains code for the testing of subsystems of gc_benchmark
 */

#include "gc_types.h"
#include <stdio.h>
#include "garbage_collector.h"
#include "gc_util.h"
#include "gc_cheney.h"
#include "binary_tree.h"
#include "cyclic_list.h"
#include "cdouble_list.h"
#include "gc_cheney_base.h"

#define TEST_SIZE 20

/**
 * Testing of the basic garbage collection functionality
 */
int gc_test()
{
    type_info_t *struct_info;
    test_struct_t test_instance;
    void **ptr_src1, **ptr_src2;
    int *atom_test, *array_test;
    test_struct_t *struct_test, *struct_array_test, *ptr_src3, *ptr_src4;
    int i;
    
    printf("\nTESTING MEMORY ALLOCATION AND COLLECTION:\n\n");
    printf("Initial memory dump:\n");
    mem_dump(stdout);
    
    atom_test = gc_malloc(int);
    struct_test = gc_malloc(test_struct_t);
    array_test = gc_malloc_array(int, TEST_SIZE);
    struct_array_test = gc_malloc_array(test_struct_t, TEST_SIZE);
    
    ptr_src1 = (void**)gc_malloc(void*);
    *ptr_src1 = atom_test;
    
    ptr_src2 = (void**)gc_malloc_array(void*,TEST_SIZE);
    ptr_src2[0] = atom_test;
    ptr_src2[1] = struct_test;
    ptr_src2[2] = NULL;
    
    ptr_src3 = (test_struct_t*)gc_malloc(test_struct_t);
    ptr_src3->ptr1 = NULL;
    ptr_src3->ptr2 = struct_test;
    
    ptr_src4 = (test_struct_t*)gc_malloc_array(test_struct_t, TEST_SIZE);
    ptr_src4[0].ptr1 = array_test;
    ptr_src4[0].ptr2 = atom_test;
    ptr_src4[1].ptr1 = NULL;
    ptr_src4[1].ptr2 = atom_test;
    ptr_src4[2].ptr1 = struct_test;
    ptr_src4[2].ptr2 = NULL;
    
    printf("\nMemory dump after allocation:\n");
    mem_dump(stdout);
    
    gc_cheney_base_roots_count = 4;
    gc_cheney_base_roots = (void**)malloc(gc_cheney_base_roots_count*sizeof(void*));
    
    gc_cheney_base_roots[0] = ptr_src1;
    gc_cheney_base_roots[1] = ptr_src2;
    gc_cheney_base_roots[2] = ptr_src3;
    gc_cheney_base_roots[3] = ptr_src4;
    
    gc_collect();
    
    printf("\nMemory dump after collection:\n");
    mem_dump(stdout);
    
    printf("\n");
    
    return 0;
}

/**
 * Testing of type system of garbage collector
 */
int type_test()
{
    printf("\nTESTING TYPE SYSTEM:\n\n");
    printf("%s\n", typenum_to_string(type_num(int)));
    printf("%s\n", typenum_to_string(type_num(void*)));
    printf("%s\n", typenum_to_string(type_num(double)));
    printf("%s\n", typenum_to_string(type_num(test_struct_t)));
    printf("%s\n", typenum_to_string(type_num(btree_t)));
    printf("%s\n", typenum_to_string(type_num(clist_t)));
    printf("\n");
    
    return 0;
}

/**
 * Testing of the binary tree data structure
 */
int btree_test()
{
    btree_t *btree;
    int i;
    long n;
    
    btree = NULL;
    
    printf("\nTESTING BINARY TREES:\n\n");
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
    
    printf("\n");
}

/**
 * Testing of the cyclic list data structure
 */
int clist_test()
{
    clist_t *clist;
    int i;
    long n;

    clist = NULL;
    
    printf("\nTESTING CYCLIC LIST:\n\n");
    
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
    
    printf("\n");
    
    return 0;
}

/**
 * Testing of cyclic double linked list data structure
 */
int cdlist_test()
{
    cdlist_t *list;
    int i;
    long n;

    list = NULL;
    
    printf("\nTESTING CYCLIC DOUBLE LINKED LIST:\n\n");
    
    printf("\nInserting:\n");
    for(i = 0; i < TEST_SIZE; i++)
    {
        n = rand()%TEST_SIZE;
        printf("inserting %d, is_succes?: %d\n", (int)n, cdlist_insert(&list, n));
    }    
    
    printf("\nSearching:\n");
    for(i = 0; i < TEST_SIZE; i++)
    {
        n = rand()%TEST_SIZE;
        printf("searchig for %d, is succes?: %p\n", (int)n, cdlist_search(list, n));
    }
    
    printf("\nDeleting:\n");
    for(i = 0; i < TEST_SIZE; i++)
    {
        n = rand()%TEST_SIZE;
        printf("deleteing %d ", (int)n);
        printf("is success?: %d\n", cdlist_delete(&list, n));
    }
    
    printf("\n");
    
    return 0;
}
