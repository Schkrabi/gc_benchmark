/**
 * @name gc_constants.h
 * @author Mgr. Radomir Skrabal
 * This file contains shared defined constants for project gc_benchmark
 */

#ifndef GC_CONSTANTS_H
#define GC_CONSTANTS_H

/******************************************************************************
 *                              GENERAL CONSTANTS                             *
 ******************************************************************************/
/**
 * Size of a word in the system
 * @remark Currently only for GNU\Linux systems
 */
#define WORD_SIZE sizeof(void*)

/**
 * Size of the semispace in bytes if applicable
 */
#define SEMISPACE_SIZE 8192//1048576 //1MB

/******************************************************************************
 *                              GARBAGE COLLECTORS                            *
 ******************************************************************************/
/**
 * Error value for argument parsing
 */
#define INVALID_GC -1
/**
 * Coinstant for garbage collector deactivation
 * @see gabage_collector.h: used_gc
 */
#define NO_COLLECTOR 0
/**
 * Constant for Cheneys gabgae collector
 * @see gabage_collector.h: used_gc
 */
#define CHENEY_GC 1
#define CUSTOM_GC 2

#define XCOLLECTOR_TABLE(x) x(cheney, CHENEY_GC) \
                            x(custom, CUSTOM_GC)

/******************************************************************************
 *                                  TYPES                                     *
 ******************************************************************************/
/**
 * Overall number of types recognized by GC
 */
#define TYPE_COUNT 10

/**
 * Type offsets into a type table
 * @see gc_types.h: type_table
 */
#define TYPE_UNDEFINED 0
#define TYPE_FORWARD 1
#define TYPE_INT 2
#define TYPE_ARRAY 3 //TODO Obsolete remove
#define TYPE_PTR 4
#define TYPE_DOUBLE 5
#define TYPE_TEST_STRUCT_T 6
#define TYPE_BTREE_T 7
#define TYPE_CLIST_T 8
#define TYPE_CDLIST_T 9

/**
 * xmacro for mapping a real c types into a GC types
 */
#define XTYPE_TABLE(x)  x(int, TYPE_INT) \
                        x(void*, TYPE_PTR) \
                        x(int*, TYPE_PTR) \
                        x(double*, TYPE_PTR) \
                        x(double, TYPE_DOUBLE) \
                        x(test_struct_t, TYPE_TEST_STRUCT_T) \
                        x(test_struct_t, TYPE_PTR) \
                        x(btree_t, TYPE_BTREE_T) \
                        x(btree_node_t, TYPE_BTREE_T) \
                        x(btree_t*, TYPE_PTR) \
                        x(btree_node_t*, TYPE_PTR) \
                        x(clist_t, TYPE_CLIST_T) \
                        x(clist_node_t, TYPE_CLIST_T) \
                        x(clist_t*, TYPE_PTR) \
                        x(clist_node_t*, TYPE_PTR) \
                        x(cdlist_t, TYPE_CDLIST_T) \
                        x(cdlist_node_t, TYPE_CDLIST_T) \
                        x(cdlist_t*, TYPE_PTR) \
                        x(cdlist_node_t*, TYPE_PTR)
                        
/** 
 * Allocated memory types
 * @remark OBSOLETE
 * TODO Remove
 */
//#define MEM_TYPE_ATOM 0
//#define MEM_TYPE_STRUCT 1
//#define MEM_TYPE_ARRAY 2

/******************************************************************************
 *                                  TESTS                                     *
 ******************************************************************************/
#define TEST_INVALID -1
#define TEST_SUBSYSTEM 0
#define TEST_SHORT_LIVED 1
#define TEST_LONG_LIVED 2

#define TEST_DIFF1 3
#define TEST_DIFF2 4
#define TEST_DIFF3 5
#define TEST_DIFF4 6

#define XTEST_TABLE(x)  x(subsystem, TEST_SUBSYSTEM) \
                        x(short_lived, TEST_SHORT_LIVED) \
                        x(long_lived, TEST_LONG_LIVED)  \
                        x(diff1, TEST_DIFF1)    \
                        x(diff2, TEST_DIFF2)    \
                        x(diff3, TEST_DIFF3)    \
                        x(diff4, TEST_DIFF4)

#endif
