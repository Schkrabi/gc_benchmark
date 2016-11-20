/**
 * @name gc_util.h
 * @author Mgr. Radomir Skrabal
 * This file contains header for additional utility code 
 */

#ifndef GC_UTIL_H
#define GC_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include "gc_shared.h"
#include "gc_cheney.h"
#include "gc_types.h"

#ifdef __gnu_linux__
typedef unsigned long long art_ptr_t;
#else
typedef unsigned long art_ptr_t;
#endif 

/**
 * Reads one line from opened file
 * @param file read file
 * @param buffer buffer where read characters are stored
 * @param max_site size of buffer
 * @returns number of read characters or -1 on error
 */
size_t read_line(FILE *file, char* buffer, size_t max_size);

/**
 * Opens the file /proc/self/maps and retrieves the line containing stack mapping
 * @param buffer buffer where the line is stored
 * @param max_size size of buffer
 * @returns length of the returned line
 */
int get_stack_line(char *buffer, size_t max_size);

/**
 * Gets the bottom of the stack from /proc/self/maps file
 * @returns pointer to the bottom of the stack
 */
void *get_stack_bottom();

/**
 * Dumps information of allocated memory to the given file
 * @par file output file
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int mem_dump(FILE *file);
/**
 * Dumps information about block of memory to the given file
 * @par file output file
 * @par block a memory block
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int dump_block(FILE *file, block_t *block);
/**
 * Dumps a block of memory containign atomic value into a given file
 * @par file output file
 * @par block memory block of type MEM_TYPE_ATOM
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int dump_block_atom(FILE *file, block_t *block);
/**
 * Dumps a block of memory containign strucutre into a given file
 * @par file output file
 * @par block memory block of type MEM_TYPE_STRUCT
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int dump_block_struct(FILE *file, block_t *block);
/**
 * Dumps a block of memory containign an array into a file
 * @par file output file
 * @par block memory block of type MEM_TYPE_ARRAY
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int dump_block_array(FILE *file, block_t *block);
/**
 * Creates a cstring containing readable information about type_info_t structure
 * @par info descriptor structure
 * @par buf::out output parameter for the string
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int type_info_to_string(type_info_t *info, char **buff);
#endif
