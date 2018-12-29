#ifndef GENERATED_TYPES_H
#define GENERATED_TYPES_H

#include "gc_shared.h"
#include <stdio.h>

/**
 * Macro generating btree struct name from suffix
 */
#define GEN_BTREE_NAME(SUFFIX) btree ## SUFFIX
/**
 * Macro generating btree type name from suffix
 */
#define GEN_BTREE_TYPE(SUFFIX) btree ## SUFFIX ## _t
/**
 * Macro generating btree node type name from suffix
 */
#define GEN_BTREE_NODE_TYPE(SUFFIX) btree ## SUFFIX ## _node_t
/**
 * Macro generating btree type constant from suffix
 */
#define GEN_BTREE_NUM(SUFFIX) TYPE_BTREE_ ## SUFFIX ## _T

/**
 * Macro generating headers for btree types
 * @par SUFFIX suffix of btree type
 * @par FIELDS additional fields in btree type
 * @par NUM nomber constant for btree type
 */
#define MAKE_BTREE_HEADER(SUFFIX, FIELDS, NUM)                                                          \
    extern uint64_t GEN_BTREE_NUM(SUFFIX);                                                              \
    typedef struct GEN_BTREE_NAME(SUFFIX) {                                                             \
        long value;                                                                                     \
        struct GEN_BTREE_NAME(SUFFIX) *lchild, *rchild;                                                 \
        FIELDS                                                                                          \
    } GEN_BTREE_NODE_TYPE(SUFFIX), GEN_BTREE_TYPE(SUFFIX);                                              \
    int btree ## SUFFIX ## _make_descriptor(type_info_t *info);                                         \
    int btree ## SUFFIX ## _insert(GEN_BTREE_TYPE(SUFFIX) **root, long value);                          \
    GEN_BTREE_NODE_TYPE(SUFFIX) *btree ## SUFFIX ## _search(GEN_BTREE_TYPE(SUFFIX) *root, long value);  \
    int btree ## SUFFIX ## _delete(GEN_BTREE_TYPE(SUFFIX) **root, long value);                          \
    GEN_BTREE_NODE_TYPE(SUFFIX) **btree ## SUFFIX ## _most_right_node(GEN_BTREE_TYPE(SUFFIX) **root);
   
/**
 * Macro generating code for btree types
 * @par SUFFIX suffix of btree type
 * @par FIELDS additional fields in btree type
 * @par NUM nomber constant for btree type
 */
#define MAKE_BTREE_SRC(SUFFIX, FIELDS, NUM)                                                                                     \
    uint64_t GEN_BTREE_NUM(SUFFIX) = NUM;                                                                                       \
    int btree ## SUFFIX ## _make_descriptor(type_info_t *info)                                                                  \
    {                                                                                                                           \
        GEN_BTREE_NODE_TYPE(SUFFIX) measure;                                                                                    \
        info->size = sizeof(GEN_BTREE_NODE_TYPE(SUFFIX));                                                                       \
        info->number_of_references = 2;                                                                                         \
        info->references = (ptr_info_t*)malloc(2 * sizeof(ptr_info_t));                                                         \
        info->references[0].offset = (uint64_t)((art_ptr_t)&measure.lchild - (art_ptr_t)&measure);                              \
        ptr_info_set_type(&info->references[0], GEN_BTREE_NUM(SUFFIX));                                                         \
        info->references[1].offset = (uint64_t)((art_ptr_t)&measure.rchild - (art_ptr_t)&measure);                              \
        ptr_info_set_type(&info->references[1], GEN_BTREE_NUM(SUFFIX));                                                         \
    }                                                                                                                           \
    int btree ## SUFFIX ## _insert(GEN_BTREE_TYPE(SUFFIX) **root, long value)                                                   \
    {                                                                                                                           \
        if(*root == NULL)                                                                                                       \
        {                                                                                                                       \
            *root = __gc_malloc(NUM);                                                                                           \
            (*root)->value = value;                                                                                             \
            (*root)->lchild = NULL;                                                                                             \
            (*root)->rchild = NULL;                                                                                             \
            return 1;                                                                                                           \
        }                                                                                                                       \
        if((*root)->value == value)                                                                                             \
        {                                                                                                                       \
            return 0;                                                                                                           \
        }                                                                                                                       \
        return btree ## SUFFIX ## _insert((GEN_BTREE_TYPE(SUFFIX)**)((*root)->value > value ?                                   \
                                                                        &(*root)->lchild : &(*root)->rchild), value);           \
    }                                                                                                                           \
    GEN_BTREE_NODE_TYPE(SUFFIX) *btree ## SUFFIX ## _search(GEN_BTREE_TYPE(SUFFIX) *root, long value)                           \
    {                                                                                                                           \
        if(root == NULL)                                                                                                        \
        {                                                                                                                       \
            return NULL;                                                                                                        \
        }                                                                                                                       \
        if(root->value == value)                                                                                                \
        {                                                                                                                       \
            return root;                                                                                                        \
        }                                                                                                                       \
        return btree ## SUFFIX ## _search((GEN_BTREE_TYPE(SUFFIX)*)(root->value > value ?                                       \
                                                                        root->lchild : root->rchild), value);                   \
    }                                                                                                                           \
    int btree ## SUFFIX ## _delete(GEN_BTREE_TYPE(SUFFIX) **root, long value)                                                   \
    {                                                                                                                           \
        GEN_BTREE_NODE_TYPE(SUFFIX) *btree;                                                                                     \
        if(*root == NULL)                                                                                                       \
        {                                                                                                                       \
            return 0;                                                                                                           \
        }                                                                                                                       \
        btree = *root;                                                                                                          \
        if(btree->value == value)                                                                                               \
        {                                                                                                                       \
            GEN_BTREE_NODE_TYPE(SUFFIX) **replacement;                                                                          \
            if(btree->lchild == NULL)                                                                                           \
            {                                                                                                                   \
                *root = btree->rchild;                                                                                          \
                return 1;                                                                                                       \
            }                                                                                                                   \
            if(btree->rchild == NULL)                                                                                           \
            {                                                                                                                   \
                *root = btree->lchild;                                                                                          \
                return 1;                                                                                                       \
            }                                                                                                                   \
            replacement = btree ## SUFFIX ## _most_right_node((GEN_BTREE_TYPE(SUFFIX)**)&btree->lchild);                        \
            (*replacement)->lchild = btree->lchild;                                                                             \
            (*replacement)->rchild = btree->rchild;                                                                             \
            *root = *replacement;                                                                                               \
            *replacement = NULL;                                                                                                \
            if(*root == (*root)->lchild)                                                                                        \
            {                                                                                                                   \
                (*root)->lchild = NULL;                                                                                         \
            }                                                                                                                   \
            return 1;                                                                                                           \
        }                                                                                                                       \
        return btree ## SUFFIX ## _delete((GEN_BTREE_TYPE(SUFFIX)**)(btree->value > value ?                                     \
                                                                        &btree->lchild : &btree->rchild), value);               \
    }                                                                                                                           \
    GEN_BTREE_NODE_TYPE(SUFFIX) **btree ## SUFFIX ## _most_right_node(GEN_BTREE_TYPE(SUFFIX) **root)                            \
    {                                                                                                                           \
        GEN_BTREE_NODE_TYPE(SUFFIX) *btree;                                                                                     \
        if(    root == NULL                                                                                                     \
            ||  *root == NULL)                                                                                                  \
        {                                                                                                                       \
            return NULL;                                                                                                        \
        }                                                                                                                       \
        btree = *root;                                                                                                          \
        if(btree->rchild == NULL)                                                                                               \
        {                                                                                                                       \
            return root;                                                                                                        \
        }                                                                                                                       \
        return btree ## SUFFIX ## _most_right_node((GEN_BTREE_TYPE(SUFFIX)**)&btree->rchild);                                   \
    }
    
#define GEN_INIT_TYPE_TABLE(SUFFIX, TYPES, NUM) btree ## SUFFIX ## _make_descriptor(&type_table[NUM]);
#define GEN_FREE_TYPE_TABLE(SUFFIX, TYPES, NUM) free(type_table[NUM].references);

#define GEN_BTREE_NUM_MIN 14
#define GEN_BTREE_NUM_MAX 15

/**
 * xmacro for btree type definition
 */
#define XGENERATE_TYPES_BTREE(X) \
    X(IF, int a; float aa;, 14) \
    X(CD, char aaa; double aaaa;, 15)
    
XGENERATE_TYPES_BTREE(MAKE_BTREE_HEADER)

/**
 * Inserts value into a generated type btree based on the given btree type
 * @par btree pointer to generated btree structure
 * @par value inserted value
 * @par used_type used generated btree type
 */
int gen_btree_insert(void **btree, long value, uint64_t used_type);

#define GEN_GRAPH_EDGE_NAME(SUFFIX) graph ## SUFFIX ## _edge
#define GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) graph ## SUFFIX ## _edge_t
#define GEN_GRAPH_EDGE_NUM(SUFFIX) TYPE_GRAPH_EDGE_ ## SUFFIX ## _T

#define GEN_GRAPH_NODE_NAME(SUFFIX) graph ## SUFFIX ## _node
#define GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) graph ## SUFFIX ## _node_t
#define GEN_GRAPH_NODE_NUM(SUFFIX) TYPE_GRAPH_NODE_ ## SUFFIX ## _T

#define GEN_GRAPH_NAME(SUFFIX) graph ## SUFFIX
#define GEN_GRAPH_TYPE_NAME(SUFFIX) graph ## SUFFIX ## _t
#define GEN_GRAPH_NUM(SUFFIX) TYPE_GRAPH_ ## SUFFIX ## _T

#define MAKE_GRAPH_HEADER(SUFFIX, FIELDS, NUM) \
    extern uint64_t GEN_GRAPH_EDGE_NUM(SUFFIX); \
    extern uint64_t GEN_GRAPH_NODE_NUM(SUFFIX); \
    extern uint64_t GEN_GRAPH_NUM(SUFFIX); \
    struct GEN_GRAPH_EDGE_NAME(SUFFIX); \
    typedef struct GRAPH_NODE_NAME(SUFFIX) \
    {\
        int value;\
        struct GEN_GRAPH_EDGE_NAME(SUFFIX) *edges;\
        struct GRAPH_NODE_NAME(SUFFIX) *next;\
        FIELDS \
    } GEN_GRAPH_NODE_TYPE_NAME(SUFFIX); \
    typedef struct GRAPH_EDGE_NAME(SUFFIX) \
    {\
        int value;\
        struct GRAPH_NODE_NAME(SUFFIX) *from, *to;\
        struct GEN_GRAPH_EDGE_NAME(SUFFIX) *next;\
        FIELDS\
    } GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX);\
    typedef struct GEN_GRAPH_NAME(SUFFIX) \
    {\
        uint64_t next_id;\
        size_t node_count;\
        struct GRAPH_NODE_NAME(SUFFIX) *nodes;\
        FIELDS\
    } GEN_GRAPH_TYPE_NAME(SUFFIX);\
    int graph ## SUFFIX ## _node_make_descriptor(type_info_t *info);\
    int graph ## SUFFIX ## _edge_make_descriptor(type_info_t *info);\
    int graph ## SUFFIX ## _make_descriptor(type_info_t *info);\
    GEN_GRAPH_TYPE_NAME(SUFFIX)* make_empty_graph ## SUFFIX ## ();\
    GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _add_node(GEN_GRAPH_TYPE_NAME(SUFFIX) *graph);\
    int graph ## SUFFIX ## _remove_node(GEN_GRAPH_TYPE_NAME(SUFFIX)* graph, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *node);\
    GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _find_node(GEN_GRAPH_TYPE_NAME(SUFFIX)* graph, uint64_t id);\
    GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _find_node_by_value(GEN_GRAPH_TYPE_NAME(SUFFIX)* graph, int value);\
    GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _node_add_edge(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* from, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *to);\
    int graph ## SUFFIX ## _node_remove_edge(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *from, GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) *edge);\
    int graph ## SUFFIX ## _node_remove_edge_by_to_node(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* from, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* to);\
    GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _add_edge(GEN_GRAPH_TYPE_NAME(SUFFIX)* graph, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *from, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *to);\
    int graph ## SUFFIX ## _remove_edge(GEN_GRAPH_TYPE_NAME(SUFFIX)* graph, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *from, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *to);\
    GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _node_find_edge(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* from, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *to);\
    GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _node_find_edge_by_value(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* from, int value);\
    GEN_GRAPH_TYPE_NAME(SUFFIX)* make_complete_graph ## SUFFIX ## (size_t number_of_nodes);\
    int is_graph ## SUFFIX ## _node(GEN_GRAPH_TYPE_NAME(SUFFIX) *graph, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *node);\
    int is_node ## SUFFIX ## _edge(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *node, GEN_GRAPH_EDGE_TYPE_NAME(SUFFX) *edge);

/**
 * xmacro for graph type definition
 */
#define XGENERATE_TYPES_GRAPH(X) \
    X(IF, int a; float aa;, 16)  \
    X(CD, char aaa; double aaaa;, 17)
    
XGENERATE_TYPES_GRAPH(MAKE_GRAPH_HEADER)

#endif
