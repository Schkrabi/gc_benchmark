/**
 * @name cdouble_list.h
 * @author Mgr. Radomir Skrabal
 * This file contains headers and definitions for cyclic double-linked list structure
 */

#ifndef CDOUBLE_LIST_H
#define CDOUBLE_LIST_H

#include "gc_shared.h"

typedef struct
{
    long value;
    void *prev;
    void *next;
} cdlist_node_t, cdlist_t;

/**
 * Makes and sets the cyclic double linked list descriptor to the given descriptor structure pointer
 * @par info pointer to the allocated descriptor structure
 * @return 1 if everything went well, 0 otherwise
 */
int cdlist_make_descriptor(type_info_t *info);

/**
 * Inserts the value into the cyclic double linked list
 * @par list pointer to the list structure pointer
 * @par value value to be inserted
 * @return 1 if everything went well, 0 otherwise
 */
int cdlist_insert(cdlist_t **list, long value);

/**
 * Searches the cyclic double linked list for a given value
 * @par list pointer to the list structure
 * @par value searched value
 * @return pointer to the node containing the value or NULL if value was not found
 */
cdlist_node_t *cdlist_search(cdlist_t *list, long value);

/**
 * Deletes value from cyclic double linked list
 * @par list pointer to the list structure pointer
 * @par value deleted value
 * @return 1 if value was deleted, 0 if value was not find in list
 */
int cdlist_delete(cdlist_t **list, long value);

#endif
