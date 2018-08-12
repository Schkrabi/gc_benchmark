/**
 * @name test.c
 * @author Mgr. Radomir Skrabal
 * This file contains code for the testing of subsystems of gc_benchmark
 */

#include "gc_types.h"
#include <stdio.h>
#include <inttypes.h>
#include "garbage_collector.h"
#include "gc_util.h"
#include "gc_cheney.h"
#include "binary_tree.h"
#include "cyclic_list.h"
#include "cdouble_list.h"
#include "gc_cheney_base.h"
#include "tarray.h"
#include "large_structure.h"

#define TEST_SIZE 20

/**
 * Testing of the basic garbage collection functionality
 */
int gc_test()
{
	btree_t
		*evacuated_array,
		*evacuated_referenced_atom,
		*not_evacuated_array,
		*not_evacuated_referenced_atom;

	int 	*evacuated_referenced_array,
		*not_evacuated_referenced_array,
		i,
		checkOk;

	tarray_t 
		*evacuated_atom,
		*not_evacuated_atom;

	int allocation_checklist[8];
	int collection_checklist[8];
	block_t *block;

	for(i = 0; i < 8; i++)
	{
		allocation_checklist[i] = 0;
		collection_checklist[i] = 0;
	}

	printf("\nTESTING MEMORY ALLOCATION\n");

//Evacuated
	evacuated_atom = gc_malloc(tarray_t);
	evacuated_atom->size = 0x0;

	evacuated_array = gc_malloc_array(btree_t, TEST_SIZE);
	for(i = 0; i < TEST_SIZE; i++)
	{
		evacuated_array[i].value = 0x1;
		evacuated_array[i].lchild = NULL;
		evacuated_array[i].rchild = NULL;
	}

	evacuated_referenced_atom = gc_malloc(btree_t);
	evacuated_referenced_atom->value = 0x2;
	evacuated_referenced_atom->lchild = NULL;
	evacuated_referenced_atom->rchild = NULL;
	//Set reference
	evacuated_array[0].lchild = evacuated_referenced_atom;

	evacuated_referenced_array = gc_malloc_array(int, TEST_SIZE);
	for(i = 0; i < TEST_SIZE; i++)
	{
		evacuated_referenced_array[i] = 0x3;
	}
	//Set reference 
	evacuated_atom->data = evacuated_referenced_array;

//Not Evacuated
	not_evacuated_atom = gc_malloc(tarray_t);
	not_evacuated_atom->size = 0x4;

	not_evacuated_array = gc_malloc_array(btree_t, TEST_SIZE);
	for(i = 0; i < TEST_SIZE; i++)
	{
		not_evacuated_array[i].value = 0x5;
		not_evacuated_array[i].lchild = NULL;
		not_evacuated_array[i].rchild = NULL;
	}

	not_evacuated_referenced_atom = gc_malloc(btree_t);
	not_evacuated_referenced_atom->value = 0x6;
	not_evacuated_referenced_atom->lchild = NULL;
	not_evacuated_referenced_atom->rchild = NULL;
	//Set reference
	not_evacuated_array[0].rchild = not_evacuated_referenced_atom;

	not_evacuated_referenced_array = gc_malloc_array(int, TEST_SIZE);
	for(i = 0; i < TEST_SIZE; i++)
	{
		not_evacuated_referenced_array[i] = 0x7;
	}
	//Set reference
	not_evacuated_atom->data = not_evacuated_referenced_array;

//Check allocation
	for(block = gc_cheney_base_from_space; block < gc_cheney_base_remaining_block; block = next_block(block))
    	{
		switch(block_get_type(block))
		{
			case TYPE_BTREE_T:
			{
				btree_t *b = get_data_start(block);
				allocation_checklist[b->value]++;
				break;
			}
			case TYPE_TARRAY_T:
			{
				tarray_t *t = get_data_start(block);
				allocation_checklist[t->size]++;
				break;
			}
			case TYPE_INT:
			{
				int *j = get_data_start(block);
				allocation_checklist[*j]++;
				break;
			}
			default:
				printf("Allocation error block type %"PRIu64" found\n", block_get_type(block));
		}
	}

	checkOk = 1;
	if(allocation_checklist[0] != 1)
	{
		printf("Error allocating atom\n");
		checkOk = 0;
	}
	if(allocation_checklist[1] != 1)
	{
		printf("Error allocating array\n");
		checkOk = 0;	}
	if(allocation_checklist[2] != 1)
	{
		printf("Error allocating referenced atom\n");
		checkOk = 0;
	}
	if(allocation_checklist[3] != 1)
	{
		printf("Error allocating referenced array\n");
		checkOk = 0;
	}
	if(allocation_checklist[4] != 1)
	{

		printf("Error allocating not evacuated atom\n");
		checkOk = 0;
	}
	if(allocation_checklist[5] != 1)
	{
		printf("Error allocating not evacuated array\n");
		checkOk = 0;
	}
	if(allocation_checklist[6] != 1)
	{
		printf("Error allocating not evacuated referenced atom\n");
		checkOk = 0;
	}
	if(allocation_checklist[7] != 1)
	{
		printf("Error allocating not evacuated referenced array\n");
		checkOk = 0;	
	}
	
	if(checkOk)
	{
		printf("SUCCESS\n");
	}
	else
	{
		printf("FAILED\n");
		printf("\nMemory dump\n");
		mem_dump(stdout);
	}

//Collection
	printf("\nTESTING MEMORY COLLECTION\n");
	gc_cheney_base_roots_count = 2;
   	gc_cheney_base_roots = (root_ptr*)malloc(gc_cheney_base_roots_count*sizeof(root_ptr));

	//Direct reference from roots to atom
	gc_cheney_base_roots[0].ptr = evacuated_atom;
	gc_cheney_base_roots[0].is_array = 0;

	//Direct reference from roots to array
	gc_cheney_base_roots[1].ptr = evacuated_array;
	gc_cheney_base_roots[1].is_array = 1;
	
	gc_collect();

	checkOk = 1;
	for(block = gc_cheney_base_from_space; block < gc_cheney_base_remaining_block; block = next_block(block))
    	{
		switch(block_get_type(block))
		{
			case TYPE_BTREE_T:
			{
				btree_t *b = get_data_start(block);
				collection_checklist[b->value]++;
				break;
			}
			case TYPE_TARRAY_T:
			{
				tarray_t *t = get_data_start(block);
				collection_checklist[t->size]++;
				break;
			}
			case TYPE_INT:
			{
				int *j = get_data_start(block);
				collection_checklist[*j]++;
				break;
			}
			default:
			{
				printf("Allocation error block type %"PRIu64" found\n", block_get_type(block));
				checkOk = 0;
			}
		}
	}

	if(collection_checklist[0] != 1)
	{
		printf("Error atom should have not been collected\n");
		checkOk = 0;
	}
	if(collection_checklist[1] != 1)
	{
		printf("Error array should have not been collected\n");
		checkOk = 0;	}
	if(collection_checklist[2] != 1)
	{
		printf("Error referenced atom should have not been collected\n");
		checkOk = 0;
	}
	if(collection_checklist[3] != 1)
	{
		printf("Error referenced array should have not been collected\n");
		checkOk = 0;
	}
	if(collection_checklist[4] != 0)
	{

		printf("Error collecting not evacuated atom\n");
		checkOk = 0;
	}
	if(collection_checklist[5] != 0)
	{
		printf("Error collecting not evacuated array\n");
		checkOk = 0;
	}
	if(collection_checklist[6] != 0)
	{
		printf("Error collecting not evacuated referenced atom\n");
		checkOk = 0;
	}
	if(collection_checklist[7] != 0)
	{
		printf("Error collecting not evacuated referenced array\n");
		checkOk = 0;	
	}
	
	if(checkOk)
	{
		printf("SUCCESS\n");
	}
	else
	{
		printf("FAILED\n");
		printf("\nMemory dump\n");
		mem_dump(stdout);
	}

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

#define TEST_TARRAY_LEN 50

/**
 * Testing test array data structure
 */
int tarray_test()
{
    tarray_t    *tarray;
    size_t      i, index;
    int         n, j;
    
    tarray = NULL;
    printf("\nTESTING TARRAY:\n\n");
    
    tarray = tarray_init(TEST_TARRAY_LEN);
    
    printf("\nInserting:\n");
    for(i = 0; i < TEST_TARRAY_LEN; i++)
    {
            n = rand()%TEST_SIZE;
            printf("inserting %d, is_success?: %d\n", n, tarray_set(tarray, i, n));
    }
    
    printf("\nSelecting:\n");
    for(i = 0; i < TEST_SIZE; i++)
    {
        j = rand()%TEST_TARRAY_LEN;
        printf("item on index %d is %d\n", j, tarray_item(tarray, j));
    }
    
    printf("\nSelecting - error prone:\n");
    for(i = 0; i < TEST_SIZE; i++)
    {
        j = (rand()%(3*TEST_TARRAY_LEN)) - TEST_TARRAY_LEN;
        printf("item on index %d is %d\n", j, tarray_item(tarray, j));
    }
    
    printf("\n");
    return 0;
}

#define LARGE_STRUCTURE_TEST_SIZE 10

/**
 * Testing large data structure
 */
int large_structure_test()
{
    large_structure_t *q, **pool;
    int i;
    
    //Clean the memory
    gc_collect();
    
    printf("Memory dump before Large structure test:\n");
    mem_dump(stdout);
    
    pool = (large_structure_t**)calloc(2*LARGE_STRUCTURE_TEST_SIZE, sizeof(large_structure_t*));
    
    for(i = 0; i < 2*LARGE_STRUCTURE_TEST_SIZE; i++)
    {
        pool[i] = (large_structure_t*)gc_malloc(large_structure_t);
    }
    
    for(i = 0; i < 2*LARGE_STRUCTURE_TEST_SIZE; i++)
    {
        large_structure_entangle(pool[i], pool, 2*LARGE_STRUCTURE_TEST_SIZE);
    }
    
    gc_cheney_base_roots_count = LARGE_STRUCTURE_TEST_SIZE;
    gc_cheney_base_roots = (root_ptr*)malloc(gc_cheney_base_roots_count*sizeof(root_ptr));
    
    for(i = 0; i < LARGE_STRUCTURE_TEST_SIZE; i++)
    {
        gc_cheney_base_roots[i].ptr = pool[i*2];
        gc_cheney_base_roots[i].is_array = 0;
    }    
    
    printf("Memory dump after large structure allocation:\n");
    mem_dump(stdout);
    
    gc_collect();
    
    printf("Memory dump after large structure collection:\n");
    mem_dump(stdout);
}
