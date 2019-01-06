/**
 * @name benchmark.h
 * @author Mgr. Radomir Skrabal
 * This file contains headers and definitions for benchmarking functions of gc_benchmark
 */

#ifndef BENCHMARK_H
#define BENCHMARK_H
#include <stdlib.h>

/**
 * Default value for __test_size
 */
#define __DEFAULT_TEST_SIZE 10000
/**
 * Default value for __max_structure_size
 */
#define __DEFAULT_MAX_STRUCTURE_SIZE 10
/**
 * Default value form __old_pool_size
 */
#define __DEFAULT_OLD_POOL_SIZE 15
/**
 * Default value for __chance_to_replace
 */
#define __DEFAULT_CHANCE_TO_REPLACE 0.01
/**
 * Default value for __entanglement_buff_size
 */
#define __DEFAULT_ENTANGLEMENT_BUFF_SIZE 100
/**
 * Default value for __binary_tree_types
 */
#define __DEFAULT_BINARY_TREE_TYPES 0
/**
 * Default value for __graph_types
 */
#define __DEFAULT_GRAPH_TYPES 0

/**
 * User specified arugment for test size
 */
extern size_t __test_size;
/**
 * User specified arugment for maximal structure size (where applicable)
 */
extern size_t __max_structure_size;
/**
 * User specified arugment for size of old pool (where applicable)
 */
extern size_t __old_pool_size;
/**
 * User specified arugment for chance to replace structure in old pool (where applicable)
 */
extern double __chance_to_replace;
/**
 * User specified argument for number of generated binary tree types
 */
extern int __binary_tree_types;
/**
 * User specified argument for number of generated graph types
 */
extern int __graph_types;

/**
  * Parses argument specified from the command line
  * @par arg argument string specified from command line
  * @return Test number or TEST_INVALID if test number was not recognized
  */
int parse_test_arg(const char *arg);

/**
 * Returns name of the test for given test_num
 * @par test_num test number
 * @return constant string containing test name
 */
const char* testnum_to_str(int test_num);

/**
 * Tests the behaviour of garbage collector with large amount of short lived objects that are imidiately discarted
 * @par test_size overall number of object created in the test
 * @par max_list_len maximal magnitude to which the one object can grow before discarted 
 * @warning If max_list_len is set too large it might lead to the overall memory depletion!
 */
int test_list(size_t test_size, size_t max_list_len);

/**
 * Tests the behaviour of garbage collector with certain amount of long lived object
 * @par test_size overall number of objects created in the test
 * @par max_tree_size maximal magnitude to which the one object can grow
 * @par old_pool number of old object that prevails in the memory
 * @par chance_to_replace a chance for a new object to replace an old object in old_pool
 * @warning If max_tree_size is set too large it might lead to the overall memory depletion!
 */
int test_binary_tree(size_t test_size, size_t max_tree_size, size_t old_pool, double chance_to_replace);

/**
 * Test behaviour of garbage collector with large long living objects entangled in interconected graph
 * @par test_size Overall number of memeory object that will be allocated
 * @par old_pool number of roots for garbage collection
 * @par chance_tp_replace a chace (between 0.0 and 1.0) for a newly allocated object to become root instead of old one
 * @par entanglement_buff_size maximal number of objects to choose for entangling the structures
 */
int test_large_structure(size_t test_size, size_t old_pool, double chance_to_replace, size_t entanglement_buff_size);

/**
 * Tests behaviour of grabage collector with complete graph long living objects
 * @par test_size overall number of objects created in the test
 * @par max_graph_size maximal number of nodes that single complete graph structure can have
 * @par old_pool number of roots for garbage collection
 * @par chance_to_replace a chace (between 0.0 and 1.0) for a newly allocated object to become root instead of old one
 */ 
int test_complete_graphs(size_t test_size, size_t max_graph_size, size_t old_pool, double chance_to_replace);

/**
 * Tests behaviour of garbage collector with large number of binary tree types
 * @par test_size overall number of objects created in the test
 * @par max_graph_size maximal number of nodes that single complete graph structure can have
 * @par old_pool number of roots for garbage collection
 * @par chance_to_replace a chace (between 0.0 and 1.0) for a newly allocated object to become root instead of old one
 */
int test_binary_tree_multitype(size_t test_size, size_t max_tree_size, size_t old_pool, double chance_to_replace);

/**
 * Tests behaviour of grabage collector with complete graph long living objects
 * @par test_size overall number of objects created in the test
 * @par max_graph_size maximal number of nodes that single complete graph structure can have
 * @par old_pool number of roots for garbage collection
 * @par chance_to_replace a chace (between 0.0 and 1.0) for a newly allocated object to become root instead of old one
 */ 
int test_complete_graphs_multitype(size_t test_size, size_t max_graph_size, size_t old_pool, double chance_to_replace);
#endif
