/**
 * @name gc_types.h
 * @author Mgr. Radomir Skrabal
 * THis file contains definitions and headers relating types, type systems and utilities for garbage collector
 */

#ifndef GC_TYPES_H
#define GC_TYPES_H

#include <stdlib.h>

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
    * Offsets from the structure pointer to the references
    */
    unsigned long *offsets;
} type_info_t; 

/**
 * Type table used by garbage collector
 */
extern type_info_t type_table[];

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
 * Gets the size of type reperesented by typenum
 * @par arg type int (offset into a type table
 * @return size of coresponding c type in bytes
 */
//size_t typenum_size(int arg);

#endif