 /**
  * @name test.h
  * @author Mgr. Radomir Skrabal
  * This file contains headers and definitions for testing of gc_benchmark subsystems
  */

/******************************************************************************
 *                          BASIC SUBSYSTEMS                                  *
 ******************************************************************************/
/**
 * Testing of the basic garbage collection functionality
 */ 
int gc_test();
/**
 * Testing of type system of garbage collector
 */
int type_test();

/******************************************************************************
 *                          DATA STRUCTURES                                   *
 ******************************************************************************/
/**
 * Testing of the binary tree data structure
 */
int btree_test();
/**
 * Testing of the cyclic list data structure
 */
int clist_test();
/**
 * Testing of cyclic double linked list data structure
 */
int cdlist_test();
/**
 * Testing test array data structure
 */
int tarray_test();
/**
 * Testing large data structure
 */
int large_structure_test();
