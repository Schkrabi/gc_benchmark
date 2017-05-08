/**
 * @name binary_tree.h
 * @author Mgr. Radomir Skrabal
 * This file contaains structures and pprototypes for benchmarkking impoolementation of binary trees
 */
 
#ifndef BINARY_TREE_H
#define BINARY_TREE_H
 
#include "gc_shared.h"
#include <stdio.h>
 
 /**
  * Structure for binary tree and binary tree node
  */
 typedef struct
 {
    long value;
    void *lchild, *rchild;
 } btree_node_t, btree_t;
 
//TODO obsolete remove
extern type_info_t *btree_descriptor;
 
/**
 * Makes and sets the binary tree structure descriptor to the given descriptor pointer
 * Also fills variable btree_descriptor necessary for algorithms
 * @par info pointer to the initialized descriptor structure
 * @return 1 if everything went well 0 otherwise
 */
int btree_make_descriptor(type_info_t *info);
 
 /**
  * Inserts value into the binary tree
  * @par root pointer to the root tree pointer
  * @par value value to be inserted
  * @return 1 if value was inserted, 0 if it was already present in the tree
  */
 int btree_insert(btree_t **root, long value);
 
 /**
  * Searches the tree for a given value
  * @par root pointer to the tree root
  * @par value searched value
  * @return pointer to the tree node that contains the value, NULL if value is not present in the tree
  */
 btree_node_t *btree_search(btree_t *root, long value);
 
 /**
  * Deletes the value from tree
  * @par root pointer to the pointer of the tree root
  * @par value value to be deleted 
  * @return 1 if the value was deleted, 0 otherwise (value was noot present in a tree)
  */
 int btree_delete(btree_t **root, long value);
 
 /**
  * Returs rightmost node of the tree
  * @par root searched tree
  * @return pointer to the rightmost node pointer or root if tree has height of 1
  */ 
 btree_node_t **most_right_node(btree_t **root);
 
 /**
 * Prints the btree into a stream
 * @par tree printed tree structure
 * @par out stream to print into
 * @par indent auxiliary parameter for right subtree indentation
 * @return number of characters printed
 * @remark for testing purposes.
 **/
int btree_print(btree_t *tree, FILE *out, size_t indent);
 
 #endif