/**
 * @name binary_tree.c
 * @author Mgr. Radomir Skrabal
 * This  file contains implementation for benchmarking binary trees
 */

#include <stdio.h>
#include <stdlib.h>
#include "binary_tree.h" 
#include "garbage_collector.h"
#include "gc_util.h"

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
    info->offsets[0] = (unsigned long)((art_ptr_t)&measure.lchild - (art_ptr_t)&measure);
    info->offsets[1] = (unsigned long)((art_ptr_t)&measure.rchild - (art_ptr_t)&measure);
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
        btree_node_t **replacement;  
        
        if(btree->lchild == NULL)
        {
            *root = btree->rchild;
            return 1;
        }
        if(btree->rchild == NULL)
        {
            *root = btree->lchild;
            return 1;
        }
        
        replacement = most_right_node((btree_t**)&btree->lchild);
        
        (*replacement)->lchild = btree->lchild;
        (*replacement)->rchild = btree->rchild;
        *root = *replacement;
        *replacement = NULL;
        
        //In case that left subtree had height of 1, reference to the lchild might be circular
        if(*root == (*root)->lchild)
        {
            (*root)->lchild = NULL;
        }
        
        return 1;
    }
    
    return btree_delete((btree_t**)(btree->value > value ? &btree->lchild : &btree->rchild), value);
}

/**
  * Returs rightmost node of the tree
  * @par root searched tree
  * @return pointer to the rightmost node pointer or root if tree has height of 1
  */ 
 btree_node_t **most_right_node(btree_t **root)
 {
     btree_node_t *btree;
     
     if(    root == NULL
        ||  *root == NULL)
     {
         return NULL;
     }
     
     btree = *root;
     
     if(btree->rchild == NULL)
     {
         return root;
     }
     
     return most_right_node((btree_t**)&btree->rchild);
 }