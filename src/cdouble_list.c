/**
 * @name cdouble_list.c
 * @author Mgr. Radomir Skrabal
 * This file contains code and implementations for cyclic double linked list structure
 */

#include <stdlib.h>
#include "cdouble_list.h"
#include "garbage_collector.h"
#include "gc_util.h"

/**
 * Makes and sets the cyclic double linked list descriptor to the given descriptor structure pointer
 * @par info pointer to the allocated descriptor structure
 * @return 1 if everything went well, 0 otherwise
 */
int cdlist_make_descriptor(type_info_t *info)
{
    cdlist_t pattern;
    
    info->size = sizeof(pattern);
    info->number_of_references = 2;
    info->offsets = (unsigned long*)malloc(2*sizeof(unsigned long));
    info->offsets[0] = (unsigned long)((art_ptr_t)&pattern.prev - (art_ptr_t)&pattern);
    info->offsets[1] = (unsigned long)((art_ptr_t)&pattern.next - (art_ptr_t)&pattern);
}

/**
 * Inserts the value into the cyclic double linked list
 * @par list pointer to the list structure pointer
 * @par value value to be inserted
 * @return 1 if everything went well, 0 otherwise
 */
int cdlist_insert(cdlist_t **list, long value)
{
    cdlist_node_t *prev, *node;
    
    if(list == NULL)
    {
        return 0    ;
    }
    
    node = gc_malloc(cdlist_node_t);
    node->value = value;
    
    if(*list == NULL)
    {
        *list = node;
        node->next = node;
        node->prev = node;
        return 1;
    }
    
    prev = (*list)->prev;
    prev->next = node;
    node->prev = prev;
    (*list)->prev = node;
    node->next = *list;
    return 1;
}

/**
 * Searches the cyclic double linked list for a given value
 * @par list pointer to the list structure
 * @par value searched value
 * @return pointer to the node containing the value or NULL if value was not found
 */
cdlist_node_t *cdlist_search(cdlist_t *list, long value)
{
    cdlist_node_t *node;
    
    if(list == NULL)
    {
        return NULL;
    }
    
    node = list;
    do
    {
        if(node->value == value)
        {
            return node;
        }
        node = node->next;
    }while(node != list);
    
    return NULL;
}

/**
 * Deletes value from cyclic double linked list
 * @par list pointer to the list structure pointer
 * @par value deleted value
 * @return 1 if value was deleted, 0 if value was not find in list
 */
int cdlist_delete(cdlist_t **list, long value)
{
    cdlist_node_t *node;
    
    if(list == NULL || *list == NULL)
    {
        return 0;
    }
    
    node = *list;
    do
    {
        if(node->value == value)
        {
            if(     node->next == node
                &&  node->prev == node)
            {
                *list = NULL;
            }
            else
            {
                cdlist_node_t *prev, *next;
                
                prev = node->prev;
                next = node->next;
                
                prev->next = next;
                next->prev = prev;
                
                if(node == *list)
                {
                    *list = next;
                }
            }
            
            return 1;
        }
        node = node->next;
    }while(node != *list);
    
    return 0;
}
