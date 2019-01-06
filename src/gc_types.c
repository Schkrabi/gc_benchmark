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
#include "tarray.h"
#include "large_structure.h"
#include "graph.h"
#include "generated_types.h"

type_info_t type_table[3000];

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
    //type_table = (type_info_t*)malloc(TYPE_COUNT * sizeof(type_info_t));
    
    test_struct_t test_instance;
    
    type_table[TYPE_UNDEFINED].size = 1;
    type_table[TYPE_UNDEFINED].number_of_references = 0;
    type_table[TYPE_UNDEFINED].references = NULL;
    
    type_table[TYPE_INT].size = sizeof(int);
    type_table[TYPE_INT].number_of_references = 0;
    type_table[TYPE_INT].references = NULL;
    
    type_table[TYPE_PTR].size = sizeof(void*);
    type_table[TYPE_PTR].number_of_references = 1;
    type_table[TYPE_PTR].references = (ptr_info_t*)malloc(1*sizeof(ptr_info_t));
    type_table[TYPE_PTR].references[0].offset = 0;
    ptr_info_set_type(&type_table[TYPE_PTR].references[0], TYPE_PTR);
    
    type_table[TYPE_DOUBLE].size = sizeof(double);
    type_table[TYPE_DOUBLE].number_of_references = 0;
    type_table[TYPE_DOUBLE].references = NULL;
    
    type_table[TYPE_TEST_STRUCT_T].size = sizeof(test_struct_t);
    type_table[TYPE_TEST_STRUCT_T].number_of_references = 2;
    type_table[TYPE_TEST_STRUCT_T].references = malloc(2*sizeof(ptr_info_t));
    type_table[TYPE_TEST_STRUCT_T].references[0].offset = (unsigned long)&test_instance.ptr1 - (unsigned long)&test_instance;
    ptr_info_set_type(&type_table[TYPE_TEST_STRUCT_T].references[0], TYPE_PTR);
    type_table[TYPE_TEST_STRUCT_T].references[1].offset = (unsigned long)&test_instance.ptr2 - (unsigned long)&test_instance;
    ptr_info_set_type(&type_table[TYPE_TEST_STRUCT_T].references[1], TYPE_PTR);
    
    btree_make_descriptor(&type_table[TYPE_BTREE_T]);
    clist_make_descriptor(&type_table[TYPE_CLIST_T]);
    cdlist_make_descriptor(&type_table[TYPE_CDLIST_T]);  
    tarray_make_descriptor(&type_table[TYPE_TARRAY_T]);
    large_structure_make_descriptor(&type_table[TYPE_LARGE_STRUCTURE_T]);
    graph_framework_make_descriptors(&type_table[TYPE_GRAPH_T], &type_table[TYPE_GRAPH_NODE_T], &type_table[TYPE_GRAPH_EDGE_T]);
    
    //Initialization of generated types
    gen_init_generated_types_constants();    
    XGENERATE_TYPES_BTREE(GEN_BTREE_INIT_TYPE_TABLE)
    XGENERATE_TYPES_GRAPH(GEN_GRAPH_INIT_TYPE_TABLE)
    
    return 0;
}

/**
 * Cleans-up type table for the garbage collector
 * @return 0 if everything went well, errno otherwise
 */
int cleanup_type_table()
{
    free(type_table[TYPE_PTR].references);
    free(type_table[TYPE_TEST_STRUCT_T].references);
    free(type_table[TYPE_BTREE_T].references);
    free(type_table[TYPE_CLIST_T].references);
    free(type_table[TYPE_CDLIST_T].references);
    free(type_table[TYPE_TARRAY_T].references);
    free(type_table[TYPE_LARGE_STRUCTURE_T].references);
    free(type_table[TYPE_GRAPH_T].references);
    free(type_table[TYPE_GRAPH_NODE_T].references);
    free(type_table[TYPE_GRAPH_EDGE_T].references);
    
    XGENERATE_TYPES_BTREE(GEN_BTREE_FREE_TYPE_TABLE)
    XGENERATE_TYPES_GRAPH(GEN_GRAPH_FREE_TYPE_TABLE)
    
    //free(type_table);
    return 0;
}

/**
 * Sets the type for ptr descriptor
 * @par info A pointer to ptr_info_t structure
 * @par type A type to be set
 */
int ptr_info_set_type(ptr_info_t *info, int type)
{
    info->type = (info->type & ARRAY_BIT_MASK) | type;
}

/**
 * Sets whetter the pointer is pointing to an array
 * @par info A pointer to ptr_info_t structure
 * @par is_array A boolean value indicating whetter the pointer should point towards an array
 */
int ptr_info_set_is_array(ptr_info_t *info, int is_array)
{
    uint64_t tmp;
    
    tmp = ptr_info_get_type(info);
    
    info->type = is_array ? (ARRAY_BIT_MASK | tmp) : tmp;
}
