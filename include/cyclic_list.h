 /**
  * @name cyclic_list.h
  * @author Mgr. Radomir Skrabal
  * This file contains headers and definitions for cyclic list structure used in project gc_benchmark
  */
 
#ifndef CYCLIC_LIST_H
#define CYCLIC_LIST_H

#include "gc_shared.h"

typedef struct {
    long value;
    void *next;
} clist_node_t, clist_t;

/**
 * Makes and sets the cyclic list structure descriptor to the given descriptor pointer
 * @par info pointer to the initialized descriptor structure
 * @return 1 if everything went well 0 otherwise
 */
int clist_make_descriptor(type_info_t *info);

/**
 * Inserts value into a cyclic list
 * @par list cyclic list structure
 * @par value inserted value
 * @return 1 if the value was inserted, 0 otherwise
 */
int clist_insert(clist_t **list, long value);

/**
 * Searches the cyclic list for the first occurence of a value
 * @par list cyclic list structure
 * @par value searched value
 * @return pointer to the node containing value, or NULL if value was not found
 */
clist_node_t *clist_search(clist_t *list, long value);

/**
 * Removes the first occurence of a value in a cyclic list
 * @par list cyclic list structure
 * @par value deleted value
 * @return 1 if value was deleted, 0 if value was not present in the list
 */
int clist_delete(clist_t **list, long value);

/**
 * Gets the previous node in the cyclic list
 * @par node start node for search
 * @return pointer to the cyclic list node or NULL if list is empty
 */
clist_node_t *cnode_get_previous(clist_node_t *node);

#endif
