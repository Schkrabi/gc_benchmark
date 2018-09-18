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
#include <string.h>
#include "gc_constants.h"
#include "gc_cheney_base.h"
#include "gc_util.h"
#include <syslog.h>
#include <cdouble_list.h>
#include "garbage_collector.h"
#include "large_structure.h"
#include "graph.h"

/**
 * User specified arugment for test size
 */
size_t __test_size = __DEFAULT_TEST_SIZE;
/**
 * User specified arugment for maximal structure size (where applicable)
 */
size_t __max_structure_size = __DEFAULT_MAX_STRUCTURE_SIZE;
/**
 * User specified arugment for size of old pool (where applicable)
 */
size_t __old_pool_size = __DEFAULT_OLD_POOL_SIZE;
/**
 * User specified arugment for chance to replace structure in old pool (where applicable)
 */
double __chance_to_replace = __DEFAULT_CHANCE_TO_REPLACE;
/**
 * User specified arugment for entanglement buffer size (test large structure only)
 */
size_t __entanglement_buff_size = __DEFAULT_ENTANGLEMENT_BUFF_SIZE;

#define XTEST_COMPARE(name, num) if(strcmp(arg, #name) == 0) return num;
#define XTEST_TOSTR(name, num) case num: return #name;

 /**
  * Parses argument specified from the command line
  * @par arg argument string specified from command line
  * @return Test number or TEST_INVALID if test number was not recognized
  */
int parse_test_arg(const char *arg)
{
    XTEST_TABLE(XTEST_COMPARE)
    
    return TEST_INVALID;
}

/**
 * Returns name of the test for given test_num
 * @par test_num test number
 * @return constant string containing test name
 */
const char* testnum_to_str(int test_num)
{
    switch(test_num)
    {
        XTEST_TABLE(XTEST_TOSTR)
        default:
            gc_log(LOG_ERR, "Invalid test number %d", test_num);
            return "";
    }
}

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
    
    gc_cheney_base_roots_count = old_pool;
    gc_cheney_base_roots = (root_ptr*)malloc(gc_cheney_base_roots_count*sizeof(root_ptr));
    
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
            gc_cheney_base_roots[i].ptr = btree;
            gc_cheney_base_roots[i].is_array = 0;
        }
        else
        {
            if(rand()%100 < chance_to_replace*100)
            {
                gc_cheney_base_roots[rand()%old_pool].ptr = btree;
            }
        }
    }
}

/**
 * Searches throuht the memory and finds large_strucutre_t and find first at most max_size objects
 * @par array allocated array where pointers to large_structure_t will be stored
 * @par max_size maximal number of large_structure_t object that will be retrieved. Must be smaller or equal that the size of array
 * @returns number of objects retrieved
 */
size_t get_allocated_large_structure_array(large_structure_t** array, size_t max_size)
{
    block_t *block;
    size_t i = 0;
    
    for(block = gc_cheney_base_from_space; block < gc_cheney_base_remaining_block; block = next_block(block))
    {
        if(i >= max_size)
            break;
        
        if(     !block_is_array(block)
            &&  block_get_type(block) == TYPE_LARGE_STRUCTURE_T)
        {
            array[i] = (large_structure_t*)get_data_start(block);
            i++;
        }
    }
    
    return i;
}

/**
 * Test behaviour of garbage collector with large long living objects entangled in interconected graph
 * @par test_size Overall number of memeory object that will be allocated
 * @par old_pool number of roots for garbage collection
 * @par chance_tp_replace a chace (between 0.0 and 1.0) for a newly allocated object to become root instead of old one
 * @par entanglement_buff_size maximal number of objects to choose for entangling the structures
 */
int test_large_structure(size_t test_size, size_t old_pool, double chance_to_replace, size_t entanglement_buff_size)
{
    large_structure_t **entanglement_buff, *current;
    size_t i, so_far;
    
    gc_cheney_base_roots_count = old_pool;
    gc_cheney_base_roots = (root_ptr*)calloc(gc_cheney_base_roots_count, sizeof(root_ptr));
    
    entanglement_buff = (large_structure_t**)calloc(entanglement_buff_size, sizeof(large_structure_t*));
    
    so_far = 0;
    for(i = 0; i < test_size; i++)
    {
        size_t found = 0;
        
        current = (large_structure_t*)gc_malloc(large_structure_t);
        
        if(current == NULL) //The memory was completely filled and no objects can be allcoated.
        {
            break;
        }
        
        found = get_allocated_large_structure_array(entanglement_buff, entanglement_buff_size);
        
        large_structure_entangle(current, entanglement_buff, found);
        
        if(i < old_pool)
        {
            gc_cheney_base_roots[i].ptr = (void*)current;
            gc_cheney_base_roots[i].is_array = 0;
        }
        else
        {
            if(rand()%100 < chance_to_replace*100)
            {
                gc_cheney_base_roots[rand()%old_pool].ptr = (void*)current;
            }
        }
    }
    
    return 1;
}

/**
 * Tests behaviour of grabage collector with complete graph long living objects
 * @par test_size overall number of objects created in the test
 * @par max_graph_size maximal number of nodes that single complete graph structure can have
 * @par old_pool number of roots for garbage collection
 * @par chance_to_replace a chace (between 0.0 and 1.0) for a newly allocated object to become root instead of old one
 */ 
int test_complete_graphs(size_t test_size, size_t max_graph_size, size_t old_pool, double chance_to_replace)
{
    size_t i;
    
    gc_cheney_base_roots_count = old_pool;
    gc_cheney_base_roots = (root_ptr*)malloc(gc_cheney_base_roots_count*sizeof(root_ptr));
    
    for(i = 0; i < test_size; i++)
    {
        graph_t *graph;
        size_t size, j;
        
        graph = NULL;
        
        size = rand()%max_graph_size;
        
        graph = make_complete_graph(size);        
        
        if(i < old_pool)
        {
            gc_cheney_base_roots[i].ptr = graph;
            gc_cheney_base_roots[i].is_array = 0;
        }
        else
        {
            if(rand()%100 < chance_to_replace*100)
            {
                gc_cheney_base_roots[rand()%old_pool].ptr = graph;
            }
        }
    }
}