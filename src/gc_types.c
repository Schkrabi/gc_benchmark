/**
 * @name gc_types.c
 * @author Mgr. Radomir Skrabal
 * This file contains implementation for header gc_types.h
 */

#include "gc_types.h"
#include "gc_constants.h"
#include <string.h>
#include "binary_tree.h"
#include "cyclic_list.h"
#include "cdouble_list.h"

type_info_t type_table[TYPE_COUNT];

/**
 * Xmacro to create a type to int function
 */
#define __XTYPE_COMPARATOR(TYPE, NUM) if(strcmp(arg, #TYPE) == 0) return NUM;
/**
 * Xmacro to create a int to type string function
 */
#define __XTYPE_STRING(TYPE, NUM) if(arg == NUM) return #TYPE;
/**
 * Xmacro to get size of type represented by typenum
 */
//#define __XTYPE_SIZE(TYPE, NUM) if(arg == NUM) return sizeof(TYPE);

/**
 * Auxiliary function for type_num macro, translates type name as const char* into a int
 * @par arg constant string containing a type name
 * @return type integer (offset to type table) or -1 if type was not recognized
 */
int __type_num(const char *arg)
{
    XTYPE_TABLE(__XTYPE_COMPARATOR)
    return -1;
}

/**
 * Auxiliary function to translate type int into a type name string
 * @par arg type int (offset into a type table
 * @return a string containing a type name
 * @remark FOR TESTING AND INFO USE ONLY. Not precise!
 */
const char* typenum_to_string(int arg)
{
    XTYPE_TABLE(__XTYPE_STRING)
    return "";
} 

/**
 * Gets the size of type reperesented by typenum
 * @par arg type int (offset into a type table
 * @return size of coresponding c type in bytes
 */
//size_t typenum_size(int arg)
//{
//    XTYPE_TABLE(__XTYPE_SIZE)
//    return 0;
//}

/**
 * Initializes type table for the garbage collector
 * @return 0 if everything went well, errno otherwise
 */
int init_type_table()
{
    test_struct_t test_instance;
    
    type_table[TYPE_UNDEFINED].size = 1;
    type_table[TYPE_UNDEFINED].number_of_references = 0;
    type_table[TYPE_UNDEFINED].offsets = NULL;
    
    type_table[TYPE_INT].size = sizeof(int);
    type_table[TYPE_INT].number_of_references = 0;
    type_table[TYPE_INT].offsets = NULL;
    
    type_table[TYPE_ARRAY].size = 0;
    type_table[TYPE_ARRAY].number_of_references = 0;
    type_table[TYPE_ARRAY].offsets = NULL;
    
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
    cdlist_make_descriptor(&type_table[TYPE_CDLIST_T]);    
    
    return 0;
}

/**
 * Cleans-up type table for the garbage collector
 * @return 0 if everything went well, errno otherwise
 */
int cleanup_type_table()
{
    free(type_table[TYPE_PTR].offsets);
    free(type_table[TYPE_TEST_STRUCT_T].offsets);
    free(type_table[TYPE_BTREE_T].offsets);
    free(type_table[TYPE_CLIST_T].offsets);
    free(type_table[TYPE_CDLIST_T].offsets);
}
