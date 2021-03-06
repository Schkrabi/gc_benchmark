/**
 * @name cyclic_list.c
 * @author Mgr. Radomir Skrabal
 * This file contatins code and implementation of a cycllic list structure
 */

#include "cyclic_list.h"
#include "garbage_collector.h"
#include "gc_util.h"
#include <stdlib.h>
#include "gc_constants.h"

type_info_t *clist_descriptor;

/**
 * Makes and sets the cyclic list structure descriptor to the given descriptor pointer
 * Also fills variable clist_descriptor necessary for algorithms
 * @par info pointer to the initialized descriptor structure
 * @return 1 if everything went well 0 otherwise
 */
int clist_make_descriptor(type_info_t *info)
{
    clist_node_t pattern;
    clist_descriptor = info;
    info->size = sizeof(clist_node_t);
    info->number_of_references = 1;
    info->references = (ptr_info_t*)malloc(1*sizeof(ptr_info_t));
    info->references[0].offset = (uint64_t)((art_ptr_t)&pattern.next - (art_ptr_t)&pattern);
    ptr_info_set_type(&info->references[0], TYPE_CLIST_T);
}

/**
 * Inserts value into a cyclic list
 * @par list cyclic list structure
 * @par value inserted value
 * @return 1 if the value was inserted, 0 otherwise
 */
int clist_insert(clist_t **list, long value)
{
    clist_node_t *node;
    
    node = gc_malloc(clist_node_t);
    node->value = value;
    
    if(*list == NULL)
    {
        node->next = node;
        *list = node;
    }
    else
    {
        clist_node_t *prev;
        
        prev = cnode_get_previous(*list);
        prev->next = node;
        node->next = *list;
        *list = node;
    }
    return 1;
}

/**
 * Searches the cyclic list for the first occurence of a value
 * @par list cyclic list structure
 * @par value searched value
 * @return pointer to the node containing value, or NULL if value was not found
 */
clist_node_t *clist_search(clist_t *list, long value)
{
    clist_node_t *node;
    
    if(list == NULL)
    {
        return NULL;
    }
    if(list->value == value)
    {
        return list;
    }
    
    node = list;
    do
    {
        node = node->next;
        if(node->value == value)
        {
            return node;
        }
    }while(node != list);
    
    return NULL;
}

/**
 * Removes the first occurence of a value in a cyclic list
 * @par list cyclic list structure
 * @par value deleted value
 * @return 1 if value was deleted, 0 if value was not present in the list
 */
int clist_delete(clist_t **list, long value)
{
    clist_node_t *node, *prev;
    
    if(*list == NULL)
    {
        return 0;
    }
    
    node = *list;
    prev = NULL;
    
    if((*list)->value == value)
    {
        prev = cnode_get_previous(*list);
        // Change the first element of the list
        *list = node->next;
    }
    else
    {
        do
        {
            prev = node;
            node = node->next;
            if(node->value == value)
            {
                break;
            }
        }while(node != *list);
        
        if(node == *list)
        {
            return 0;
        }
    }
    
    if(prev == NULL)
    {    
        return 0;
    }
    
    prev->next = node->next;
    return 1;
}

/**
 * Gets the previous node in the cyclic list
 * @par node start node for search
 * @return pointer to the cyclic list node or NULL if list is empty
 */
clist_node_t *cnode_get_previous(clist_node_t *node)
{
   clist_node_t *n;
   
   if(node == NULL)
   {
       return NULL;
   }
   
   n = node;
   while(n->next != node)
   {
       n = n->next;
   }
   
   return n;
}
