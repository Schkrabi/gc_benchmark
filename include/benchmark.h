/**
 * @name benchmark.h
 * @author Mgr. Radomir Skrabal
 * This file contains headers and definitions for benchmarking functions of gc_benchmark
 */

#include <stdlib.h>

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
int test_short_lived(size_t test_size, size_t max_list_len);

/**
 * Tests the behaviour of garbage collector with certain amount of long lived object
 * @par test_size overall number of objects created in the test
 * @par max_tree_size maximal magnitude to which the one object can grow
 * @par old_pool number of old object that prevails in the memory
 * @par chance_to_replace a chance for a new object to replace an old object in old_pool
 * @warning If max_tree_size is set too large it might lead to the overall memory depletion!
 */
int test_long_lived(size_t test_size, size_t max_tree_size, size_t old_pool, double chance_to_replace);

int test_large_structure(size_t test_size, size_t old_pool, double chance_to_replace);
