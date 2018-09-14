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

extern char *session_ident;

/**
 * Initializes unique session identifier for the logger
 * @par test_num nuber of test carried out by the main
 * @par gc_num number of used garbage collector
 * @returns always 0
 */
int init_session_ident(int test_num, int gc_num);
/**
 * Frees the session indentifier pointer
 * @returns always 0
 */
int cleanup_session_ident();
/**
 * Gets the unique session identifier
 * @returns ptr to the session identifier string
 */
const char* get_session_ident();

/**
 * Returns filepath to log file
 * @par buff buffer for filepath
 * @return Alwasy 0
 */
int get_logger_file_path(char *buff);

/**
 * Inits the sysloger
 * @par test_num nuber of test carried out by the main
 * @par gc_num number of used garbage collector
 * @returns always 0
 */
int init_logger(int test_num, int gc_num);

/**
 * Appends log buffer to log file
 * @return reseted position to log buffer
 */
int dump_log();

/**
 * Vrites timestamp into the buffer
 * @par dst destination buffer
 * @return length of the timestamp string
 */
size_t timestamp(char *dst);

/**
 * Logging interface
 * @par priority priotrity of the message
 * @par format format string for the message
 * @par ... arguments for the format string
 * @return always 0
 */
int gc_log(int priority, const char* format, ...);

/**
 * Cleans up the logger subsystems
 * @returns 0 if everything went well, errno otherwise
 */
int cleanup_logger();

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
 * Dumps block in given semispace
 * @par file output file
 * @par semispace_ptr pointer to the begining of semispace
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int dump_semispace(FILE *file, void* semispace_ptr);

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

/**
 * Removes spaces from string and replaces them with underscore '_'
 * @par str out mutated string
 * @return always 0
 */
int str_remove_spaces(char *str);
/**
 * Removes char from string and replaces them 
 * @par str out mutated string
 * @par c replaced char
 * @par replace replacing chars
 * @return always 0
 */
int str_replace_char(char *str, char c, char replace);

/**
 * Fast time measurement
 * @return time mesuring integer
 */
static inline unsigned long long int rdtsc();

/**
 * Prints each 8 bytes of memory as hexadecimal 64-bit number
 * @remark For debugging purposes
 * @par ptr pointer to memory
 * @par size length of memory to be printed in bytes
 * @returns always 0
 */
int lin_mem_dump(void *ptr, size_t size);

/**
 * Prints into a file each 8 bytes of memory as hexadecimal 64-bit number
 * @remark For debugging purposes
 * @par file target file
 * @par ptr pointer to memory
 * @par size length of memory to be printed in bytes
 * @returns nonnegative number on success, or EOF on error.
 */
int flin_mem_dump(FILE *file, void *ptr, size_t mem_size);

/**
 * Prints into a string each 8 bytes of memory as hexadecimal 64-bit number
 * @remark For debugging purposes
 * @par buff string to be printed into
 * @par buff_size max length of a buffer
 * @par ptr pointer to memory
 * @par size length of memory to be printed in bytes
 * @returns pointer to printed string
 */
char* slin_mem_dump(char* buff, size_t buff_size, void *ptr, size_t mem_size);

/**
 * Initializes block of memory with given values in the dump format
 * @par ptr pointer to part of memory
 * @par mem_size size of the initailized memory
 * @oar str string with the dump of memory to be initialized by
 * @returns Always 0
 */
int init_mem_from_string(void *ptr, size_t mem_size, char* str);
#endif
