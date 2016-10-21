/**
 * @name gc_util.h
 * @author Mgr. Radomir Skrabal
 * This file contains header for additional utility code 
 */

#ifndef GC_UTIL_H
#define GC_UTIL_H

#include <stdlib.h>
#include <stdio.h>

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
#endif
