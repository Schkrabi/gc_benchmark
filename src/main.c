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
#include "gc_constants.h"

#define TEST_SIZE 3

typedef struct 
{
  int value;
  void *ptr1, *ptr2;
} test_struct_t;

int gc_test();
int btree_test();
int clist_test();
int init_type_table();
int free_type_table();
int type_test();
/**
 * Main executed function
 */
int sub_main(int argc, char *argv[]);

/**
 * Main entrypoint Garbage collector initial setup
 */
int main(int argc, char *argv[])
{
    int err_msg, rtrn;
    used_gc = CHENEY_GC;
    
    init_type_table();
    err_msg = gc_init();
    
    srand(time(0));
    
    if(err_msg != 0)
    {
        fprintf(stderr, "Initialization error, code %d\n", err_msg);
    }
    
    rtrn = sub_main(argc, argv);
    
    free_type_table();
    
    return rtrn;
}

/**
 * Main executed function
 */
int sub_main(int argc, char *argv[])
{
    gc_test();
    //btree_test();
    //clist_test();
    //type_test();
    //mem_dump(stdout);
    
    return 0;
}

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
    
    gc_roots_count = 4;
    gc_roots = (void**)malloc(gc_roots_count*sizeof(void*));
    
    gc_roots[0] = ptr_src1;
    gc_roots[1] = ptr_src2;
    gc_roots[2] = ptr_src3;
    gc_roots[3] = ptr_src4;
    
    gc_collect();
    
    printf("\nMemory dump after collection:\n");
    mem_dump(stdout);
    
    printf("\n");
    
    return 0;
}

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

int init_type_table()
{
    test_struct_t test_instance;
    
    type_table[TYPE_INT].size = sizeof(int);
    type_table[TYPE_INT].number_of_references = 0;
    type_table[TYPE_INT].offsets = NULL;
    
    type_table[TYPE_ARRAY].size = 0;
    type_table[TYPE_ARRAY].number_of_references = 0;
    type_table[TYPE_ARRAY].offsets = NULL;
    
    type_table[TYPE_STRUCT].size = 0;
    type_table[TYPE_STRUCT].number_of_references = 0;
    type_table[TYPE_STRUCT].offsets = NULL;
    
    type_table[TYPE_PTR].size = sizeof(void*);
    type_table[TYPE_PTR].number_of_references = 1;
    type_table[TYPE_PTR].offsets = (unsigned long*)malloc(1*sizeof(unsigned long));
    type_table[TYPE_PTR].offsets[0] = 0;
    
    type_table[TYPE_DOUBLE].size = sizeof(double);
    type_table[TYPE_DOUBLE].number_of_references = 0;
    type_table[TYPE_DOUBLE].offsets = NULL;
    
    type_table[TYPE_TEST_STRUCT_T].size = sizeof(test_struct_t);
    type_table[TYPE_TEST_STRUCT_T].number_of_references = 2;
    type_table[TYPE_TEST_STRUCT_T].offsets = malloc(2*sizeof(unsigned long));
    type_table[TYPE_TEST_STRUCT_T].offsets[0] = (unsigned long)&test_instance.ptr1 - (unsigned long)&test_instance;
    type_table[TYPE_TEST_STRUCT_T].offsets[1] = (unsigned long)&test_instance.ptr2 - (unsigned long)&test_instance;
    
    btree_make_descriptor(&type_table[TYPE_BTREE_T]);
    clist_make_descriptor(&type_table[TYPE_CLIST_T]);
    
    type_table[TYPE_CDLIST_T].size = 0;
    type_table[TYPE_CDLIST_T].number_of_references = 0;
    type_table[TYPE_CDLIST_T].offsets = NULL;
}

int free_type_table()
{
    free(type_table[TYPE_PTR].offsets);
    free(type_table[TYPE_TEST_STRUCT_T].offsets);
    free(type_table[TYPE_BTREE_T].offsets);
    free(type_table[TYPE_CLIST_T].offsets);
}