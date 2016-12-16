/**
 * @name benchmark.h
 * @author Mgr. Radomir Skrabal
 * This file contains code for benchmarking functions of gc_benchmark
 */

#include "benchmark.h"
#include "cyclic_list.h"
#include "binary_tree.h"
#include <limits.h>
#include "gc_cheney.h"

/**
 * Tests the behaviour of garbage collector with large amount of short lived objects that are imidiately discarted
 * @par test_size overall number of object created in the test
 * @par max_list_len maximal magnitude to which the one object can grow before discarted 
 * @warning If max_list_len is set too large it might lead to the overall memory depletion!
 */
int test_short_lived(size_t test_size, size_t max_list_len)
{
    size_t i;
    
    for(i = 0; i < test_size; i++)
    {
        clist_t *list;
        size_t j, len;
        
        list = NULL;
        clist_insert(&list, rand()%UINT_MAX);
        len = (rand()%max_list_len) - 1;
        
        for(j = 0; j < max_list_len; j++)
        {
            clist_insert(&list, rand()%UINT_MAX);
        }
    }
    
    return 1;
}

/**
 * Tests the behaviour of garbage collector with certain amount of long lived object
 * @par test_size overall number of objects created in the test
 * @par max_tree_size maximal magnitude to which the one object can grow
 * @par old_pool number of old object that prevails in the memory
 * @par chance_to_replace a chance for a new object to replace an old object in old_pool
 * @warning If max_tree_size is set too large it might lead to the overall memory depletion!
 */
int test_long_lived(size_t test_size, size_t max_tree_size, size_t old_pool, double chance_to_replace)
{
    size_t i;
    
    gc_roots_count = old_pool;
    gc_roots = (void**)malloc(gc_roots_count*sizeof(void*));
    
    for(i = 0; i < test_size; i++)
    {
        btree_t *btree;
        size_t size, j;
        
        btree = NULL;
        
        btree_insert(&btree, rand()%UINT_MAX);
        
        size = rand()%max_tree_size;
        j = 0; 
        while(j < size)
        {
            if(btree_insert(&btree, rand()%UINT_MAX) == 0)
            {
                continue;
            }
            j++;
        }
        
        if(i < old_pool)
        {
            gc_roots[i] = btree;
        }
        else
        {
            if(rand()%100 < chance_to_replace*100)
            {
                gc_roots[rand()%old_pool] = btree;
            }
        }
    }
}
