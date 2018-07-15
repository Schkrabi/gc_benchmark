/**
 * @name gc_types.h
 * @author Mgr. Radomir Skrabal
 * THis file contains definitions and headers relating types, type systems and utilities for garbage collector
 */

#ifndef GC_TYPES_H
#define GC_TYPES_H

#include <stdlib.h>
#include <stdint.h>

#define ARRAY_BIT_MASK 0x8000000000000000
#define ELEMENT_TYPE_BIT_MASK 0x7FFFFFFFFFFFFFFF

//Obsolete, TODO remove
typedef struct 
{
  int value;
  void *ptr1, *ptr2;
} test_struct_t;

/**
 * Structure for storing information about pointers
 */
typedef struct 
{
    uint64_t offset;
    uint64_t type;
} ptr_info_t;

/**
 * Structure for keeping info about structures
 */
typedef struct
{
    /**
     * Size of the structure in bytes
     */
    size_t size;
    /**
    * Number of references in the structure
    */
    size_t number_of_references;
    /**
     * Info about structure pointers
     */
    ptr_info_t *references;
} type_info_t; 

/**
 * Type table used by garbage collector
 */
extern type_info_t type_table[];

/**
 * Type to int macro
 * @par type a c type (void*, double, struct etc...)
 * @example type_num(void*) ==> TYPE_PTR, type_num(int) ==> TYPE_INT
 */
#define type_num(type) __type_num(#type)

/**
 * Auxiliary function for type_num macro, translates type name as const char* into a int
 * @par arg constant string containing a type name
 * @return type integer (offset to type table) or -1 if type was not recognized
 */
int __type_num(const char *arg);

/**
 * Auxiliary function to translate type int into a type name string
 * @par arg type int (offset into a type table
 * @return a string containing a type name
 * @remark FOR TESTING AND INFO USE ONLY. Not precise!
 */
const char* typenum_to_string(int arg);

/**
 * Initializes type table for the garbage collector
 * @return 0 if everything went well, errno otherwise
 */
int init_type_table();
/**
 * Cleans-up type table for the garbage collector
 * @return 0 if everything went well, errno otherwise
 */
int cleanup_type_table();

/**
 * Gets the typo of a pointer
 * @par ptr_info A pointer to ptr_info_t structure
 * @return type of the pointer
 */
#define ptr_info_get_type(ptr_info) ((ptr_info)->type & ELEMENT_TYPE_BIT_MASK)
/**
 * Indicates whetter this pointer points to array
 * @par ptr_info A pointer to ptr_info_t structure
 * @return true or false
 */
#define ptr_info_is_array(ptr_info) (((ptr_info)->type & ARRAY_BIT_MASK) != 0x0)

/**
 * Sets the type for ptr descriptor
 * @par info A pointer to ptr_info_t structure
 * @par type A type to be set
 */
int ptr_info_set_type(ptr_info_t *info, int type);

/**
 * Sets whetter the pointer is pointing to an array
 * @par info A pointer to ptr_info_t structure
 * @par is_array A boolean value indicating whetter the pointer should point towards an array
 */
int ptr_info_set_is_array(ptr_info_t *info, int is_array);

#endif
