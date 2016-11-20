/**
 * @name gc_types.c
 * @author Mgr. Radomir Skrabal
 * This file contains implementation for header gc_types.h
 */

#include "gc_types.h"
#include "gc_constants.h"
#include <string.h>

type_info_t type_table[TYPE_COUNT];

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
