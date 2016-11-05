/**
 * @name binary_tree.c
 * @author Mgr. Radomir Skrabal
 * This  file contains implementation for benchmarking binary trees
 */

#include <stdio.h>
#include <stdlib.h>
#include "binary_tree.h" 
#include "garbage_collector.h"

struct_info_t *btree_descriptor;
 
/**
 * Makes and sets the binary tree structure descriptor to the given descriptor pointer
 * Allso fills variable btree_descriptor necessary for algorythms
 * @par info pointer to the initialized descriptor structure
 * @return 1 if everything went well 0 otherwise
 */
int btree_make_descriptor(struct_info_t *info)
{
    btree_node_t measure;
    
    btree_descriptor = info;
    info->struct_size = sizeof(btree_node_t);
    info->number_of_references = 2;
    info->offsets = (unsigned long*)malloc(2 * sizeof(unsigned long));
    info->offsets[0] = (unsigned long)&measure.lchild - (unsigned long)&measure;
    info->offsets[1] = (unsigned long)&measure.rchild - (unsigned long)&measure;
}

/**
* Inserts value into the binary tree
* @par root pointer to the root tree pointer
* @par value value to be inserted
* @return 1 if value was inserted, 0 if it was already present in the tree
*/
int btree_insert(btree_t **root, long value)
{
  if(*root == NULL)
  {
    *root = gc_malloc_struct(btree_descriptor);
    (*root)->value = value;
    (*root)->lchild = NULL;
    (*root)->rchild = NULL;
    
    return 1;
  }
  
  if((*root)->value == value)
  {
    return 0;
  }
  
  return btree_insert((btree_t**)((*root)->value > value ? &(*root)->lchild : &(*root)->rchild), value);
}

/**
* Searches the tree for a given value
* @par root pointer to the tree root
* @par value searched value
* @return pointer to the tree node that contains the value, NULL if value is not present in the tree
*/
btree_node_t *btree_search(btree_t *root, long value)
{
  if(root == NULL)
  {
    return NULL;
  }
  if(root->value == value)
  {
    return root;
  }
    
  return btree_search((btree_t*)(root->value > value ? root->lchild : root->rchild), value);
}

/**
* Deletes the value from tree
* @par root pointer to the pointer of the tree root
* @par value value to be deleted 
* @return 1 if the value was deleted, 0 otherwise (value was noot present in a tree)
* @remark This version should be probably more memory-friendly as it only copies a value and persists the pointer hierarchy
*/
int btree_delete(btree_t **root, long value)
{
  btree_node_t *btree;
  
  if(*root == NULL)
  {
    return 0;
  }
  
  btree = *root;
  
  if(btree->value == value)
  {
    if(btree->lchild == NULL)
    {
      *root = btree->rchild;
    }
    if(btree->rchild == NULL)
    {
      *root = btree->lchild;
    }
    
    btree->value = ((btree_node_t*)btree->lchild)->value;
    
    return btree_delete((btree_t**)(&btree->lchild), btree->value);
  }
  
  return btree_delete((btree_t**)(btree->value > value ? &btree->lchild : &btree->rchild), value);
}