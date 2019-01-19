#ifndef GENERATED_TYPES_H
#define GENERATED_TYPES_H

#include "gc_shared.h"
#include "benchmark.h"
#include <stdio.h>

#define GEN_BTREE_TYPES_NUM 800
#define GEN_GRAPH_TYPES_NUM 0

///////////////////////////////////////////////////////////////////////////////
//                  GENERATED BINARY_TREES                                   //
///////////////////////////////////////////////////////////////////////////////
#define GEN_BTREE_TYPES_NUM_OFFSET 14

#define GEN_BTREE_NUM_MIN GEN_BTREE_TYPES_NUM_OFFSET
#define GEN_BTREE_NUM_MAX GEN_BTREE_TYPES_NUM_OFFSET + GEN_BTREE_TYPES_NUM - 1

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
#define GEN_BTREE_NUM(NUM) NUM + GEN_BTREE_TYPES_NUM_OFFSET

/**
 * Macro generating headers for btree types
 * @par SUFFIX suffix of btree type
 * @par FIELDS additional fields in btree type
 * @par NUM nomber constant for btree type
 */
#define MAKE_BTREE_HEADER(SUFFIX, FIELDS, NUM)                                                          \
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
    int btree ## SUFFIX ## _make_descriptor(type_info_t *info)                                                                  \
    {                                                                                                                           \
        GEN_BTREE_NODE_TYPE(SUFFIX) measure;                                                                                    \
        info->size = sizeof(GEN_BTREE_NODE_TYPE(SUFFIX));                                                                       \
        info->number_of_references = 2;                                                                                         \
        info->references = (ptr_info_t*)malloc(2 * sizeof(ptr_info_t));                                                         \
        info->references[0].offset = (uint64_t)((art_ptr_t)&measure.lchild - (art_ptr_t)&measure);                              \
        ptr_info_set_type(&info->references[0], GEN_BTREE_NUM(NUM));                                                         \
        info->references[1].offset = (uint64_t)((art_ptr_t)&measure.rchild - (art_ptr_t)&measure);                              \
        ptr_info_set_type(&info->references[1], GEN_BTREE_NUM(NUM));                                                         \
    }                                                                                                                           \
    int btree ## SUFFIX ## _insert(GEN_BTREE_TYPE(SUFFIX) **root, long value)                                                   \
    {                                                                                                                           \
        if(*root == NULL)                                                                                                       \
        {                                                                                                                       \
            *root = __gc_malloc(GEN_BTREE_NUM(NUM));                                                                                           \
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
    
#define GEN_BTREE_INIT_TYPE_TABLE(SUFFIX, TYPES, NUM)\
        btree ## SUFFIX ## _make_descriptor(&type_table[GEN_BTREE_NUM(NUM)]);
        
#define GEN_BTREE_FREE_TYPE_TABLE(SUFFIX, TYPES, NUM)\
        free(type_table[GEN_BTREE_NUM(NUM)].references);
    
#define __GC_CUSTOM_GEN_BTREE_SCAN_ARRAY_PTR_PER_TYPE(SUFFIX, TYPES, NUM) __GC_CUSTOM_SCAN_ARRAY_PTR_PER_TYPE(GEN_BTREE_NUM(NUM), GEN_BTREE_TYPE(SUFFIX))
#define __GC_CUSTOM_GEN_BTREE_SCAN_PTR_PER_TYPE(SUFFIX, TYPES, NUM)  __GC_CUSTOM_SCAN_PTR_PER_TYPE(GEN_BTREE_NUM(NUM), GEN_BTREE_TYPE(SUFFIX))

#define __GC_CUSTOM_GEN_BTREE_MAKE_GC_SCAN_STRUCT_PER_TYPE(SUFFIX, TYPES, NUM)\
    case GEN_BTREE_NUM(NUM):\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(8, GEN_BTREE_TYPE(SUFFIX))\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(16, GEN_BTREE_TYPE(SUFFIX))\
        break;

#define __GC_CUSTOM_GEN_BTREE_WALK_ARRAY_PER_TYPE(SUFFIX, TYPES, NUM) __GC_CUSTOM_WALK_ARRAY_PER_TYPE(GEN_BTREE_NUM(NUM), GEN_BTREE_TYPE(SUFFIX))

///////////////////////////////////////////////////////////////////////////////
//                                  GRAPHS                                   //
///////////////////////////////////////////////////////////////////////////////

#define GEN_GRAPH_TYPES_NUM_OFFSET GEN_BTREE_TYPES_NUM_OFFSET + GEN_BTREE_TYPES_NUM

#define GEN_GRAPH_NUM_MIN GEN_GRAPH_TYPES_NUM_OFFSET

#define GEN_GRAPH_NUM_MAX GEN_GRAPH_TYPES_NUM_OFFSET + (3*GEN_GRAPH_TYPES_NUM) - 1

#define GEN_GRAPH_EDGE_NAME(SUFFIX) graph ## SUFFIX ## _edge
#define GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) graph ## SUFFIX ## _edge_t
#define GEN_GRAPH_EDGE_NUM(NUM) (3 * NUM) + GEN_GRAPH_TYPES_NUM_OFFSET

#define GEN_GRAPH_NODE_NAME(SUFFIX) graph ## SUFFIX ## _node
#define GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) graph ## SUFFIX ## _node_t
#define GEN_GRAPH_NODE_NUM(NUM) (3 * NUM) + GEN_GRAPH_TYPES_NUM_OFFSET + 1

#define GEN_GRAPH_NAME(SUFFIX) graph ## SUFFIX
#define GEN_GRAPH_TYPE_NAME(SUFFIX) graph ## SUFFIX ## _t
#define GEN_GRAPH_NUM(NUM) (3 * NUM) + GEN_GRAPH_TYPES_NUM_OFFSET + 2

#define MAKE_GRAPH_HEADER(SUFFIX, FIELDS, NUM) \
    struct GEN_GRAPH_EDGE_NAME(SUFFIX); \
    typedef struct GEN_GRAPH_NODE_NAME(SUFFIX){\
        int value;\
        struct GEN_GRAPH_EDGE_NAME(SUFFIX) *edges;\
        struct GEN_GRAPH_NODE_NAME(SUFFIX) *next;\
        FIELDS \
    } GEN_GRAPH_NODE_TYPE_NAME(SUFFIX); \
    typedef struct GEN_GRAPH_EDGE_NAME(SUFFIX){\
        int value;\
        struct GEN_GRAPH_NODE_NAME(SUFFIX) *from, *to;\
        struct GEN_GRAPH_EDGE_NAME(SUFFIX) *next;\
        FIELDS\
    } GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX);\
    typedef struct GEN_GRAPH_NAME(SUFFIX){\
        uint64_t next_id;\
        size_t node_count;\
        struct GEN_GRAPH_NODE_NAME(SUFFIX) *nodes;\
        FIELDS\
    } GEN_GRAPH_TYPE_NAME(SUFFIX);\
    int graph ## SUFFIX ## _node_make_descriptor(type_info_t *info);\
    int graph ## SUFFIX ## _edge_make_descriptor(type_info_t *info);\
    int graph ## SUFFIX ## _make_descriptor(type_info_t *info);\
    GEN_GRAPH_TYPE_NAME(SUFFIX)* make_empty_graph ## SUFFIX ();\
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
    GEN_GRAPH_TYPE_NAME(SUFFIX)* make_complete_graph ## SUFFIX (size_t number_of_nodes);\
    int is_graph ## SUFFIX ## _node(GEN_GRAPH_TYPE_NAME(SUFFIX) *graph, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *node);\
    int is_node ## SUFFIX ## _edge(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *node, GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) *edge);
    
#define MAKE_GRAPH_SRC(SUFFIX, FIELDS, NUM) \
    int graph ## SUFFIX ## _node_make_descriptor(type_info_t *info){\
        GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) measure;\
        info->size = sizeof(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX));\
        info->number_of_references = 2;\
        info->references = (ptr_info_t*)malloc(2 * sizeof(ptr_info_t));\
        info->references[0].offset = (uint64_t)((art_ptr_t)&measure.edges - (art_ptr_t)&measure);\
        ptr_info_set_type(&info->references[0], GEN_GRAPH_EDGE_NUM(NUM));\
        info->references[1].offset = (uint64_t)((art_ptr_t)&measure.next - (art_ptr_t)&measure);\
        ptr_info_set_type(&info->references[1], GEN_GRAPH_NODE_NUM(NUM));\
    }\
    int graph ## SUFFIX ## _edge_make_descriptor(type_info_t *info){\
        GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) measure;\
        info->size = sizeof(GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX));\
        info->number_of_references = 2;\
        info->references = (ptr_info_t*)malloc(3 * sizeof(ptr_info_t));\
        info->references[0].offset = (uint64_t)((art_ptr_t)&measure.from - (art_ptr_t)&measure);\
        ptr_info_set_type(&info->references[0], GEN_GRAPH_NODE_NUM(NUM));\
        info->references[1].offset = (uint64_t)((art_ptr_t)&measure.to - (art_ptr_t)&measure);\
        ptr_info_set_type(&info->references[1], GEN_GRAPH_NODE_NUM(NUM));\
        info->references[2].offset = (uint64_t)((art_ptr_t)&measure.next - (art_ptr_t)&measure);\
        ptr_info_set_type(&info->references[2], GEN_GRAPH_EDGE_NUM(NUM));\
    }\
    int graph ## SUFFIX ## _make_descriptor(type_info_t *info){\
        GEN_GRAPH_TYPE_NAME(SUFFIX) measure;\
        info->size = sizeof(GEN_GRAPH_TYPE_NAME(SUFFIX));\
        info->number_of_references = 1;\
        info->references = (ptr_info_t*)malloc(2 * sizeof(ptr_info_t));\
        info->references[0].offset = (uint64_t)((art_ptr_t)&measure.nodes - (art_ptr_t)&measure);\
        ptr_info_set_type(&info->references[0], GEN_GRAPH_NODE_NUM(NUM));\
    }\
    GEN_GRAPH_TYPE_NAME(SUFFIX)* make_empty_graph ## SUFFIX (){\
        GEN_GRAPH_TYPE_NAME(SUFFIX) *graph;\
        graph = __gc_malloc(GEN_GRAPH_NUM(NUM));\
        graph->next_id = 0;\
        graph->node_count = 0;\
        graph->nodes = NULL;\
        return graph;\
    }\
    GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) **__graph ## SUFFIX ## _last_node(GEN_GRAPH_TYPE_NAME(SUFFIX) *graph){\
        GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *node;\
        if(graph->nodes == NULL)\
        {\
            return &graph->nodes;\
        }\
        node = graph->nodes;\
        while(node->next != NULL)\
        {\
            node = node->next;\
        }\
        return &node->next;\
    }\
    GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) **__node ## SUFFIX ## _last_edge(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *node){\
        GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) *edge;\
        if(node->edges == NULL)\
        {\
            return &node->edges;\
        }\
        edge = node->edges;\
        while(edge->next != NULL)\
        {\
            edge = edge->next;\
        }\
        return &edge->next;\
    }\
    GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _add_node(GEN_GRAPH_TYPE_NAME(SUFFIX) *graph){\
        GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *node, **insetion_point;\
        node = __gc_malloc(GEN_GRAPH_NODE_NUM(NUM));\
        node->value = 0;\
        node->edges = NULL;\
        node->next = NULL;\
        insetion_point = __graph ## SUFFIX ## _last_node(graph);\
        *insetion_point = node;\
        return node;\
    }\
    int graph ## SUFFIX ## _remove_node(GEN_GRAPH_TYPE_NAME(SUFFIX)* graph, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *node){\
        GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *prev, *tmp;\
        tmp = graph->nodes;\
        do\
        {\
            graph ## SUFFIX ## _node_remove_edge_by_to_node(tmp, node);\
            tmp = tmp->next;\
        }while(tmp != NULL);\
        if(graph->nodes == node)\
        {\
            graph->nodes = node->next;\
            return 1;\
        }\
        prev = graph->nodes;\
        while(prev->next != node)\
        {\
            if(prev->next = NULL)\
            {\
                return 0;\
            }\
            prev = prev->next;\
        }\
        prev->next = node->next;\
        return 1;\
    }\
    GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _find_node_by_value(GEN_GRAPH_TYPE_NAME(SUFFIX)* graph, int value){\
        GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *node;\
        node = graph->nodes;\
        while(  node != NULL\
            &&  node->value != value)\
        {\
            node = node->next;\
        }\
        return node;\
    }\
    GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _node_add_edge(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* from, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *to){\
        GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) *edge, **point_of_insert;\
        edge = __gc_malloc(GEN_GRAPH_EDGE_NUM(NUM));\
        edge->from = from;\
        edge->to = to;\
        edge->value = 0;\
        point_of_insert = __node ## SUFFIX ## _last_edge(from);\
        *point_of_insert = edge;\
        return edge;\
    }\
    int graph ## SUFFIX ## _node_remove_edge(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *from, GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) *edge){\
        GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) *prev;\
        if(edge == NULL)\
        {\
            return 0;\
        }\
        if(from->edges == edge)\
        {\
            from->edges = edge->next;\
            return 1;\
        }\
        prev = from->edges;\
        while(prev->next != edge)\
        {\
            if(prev->next == NULL)\
            {\
                return 0;\
            }\
            prev = prev->next;\
        }\
        prev->next = edge->next;\
        return 1;\
    }\
    int graph ## SUFFIX ## _node_remove_edge_by_to_node(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* from, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* to){\
        return graph ## SUFFIX ## _node_remove_edge(from, graph ## SUFFIX ## _node_find_edge(from, to));\
    }\
    GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _add_edge(GEN_GRAPH_TYPE_NAME(SUFFIX)* graph, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *from, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *to){\
        if(     !is_graph ## SUFFIX ## _node(graph, from)\
            ||  !is_graph ## SUFFIX ## _node(graph, to))\
        {\
            return NULL;\
        }\
        return graph ## SUFFIX ## _node_add_edge(from, to);\
    }\
    int graph ## SUFFIX ## _remove_edge(GEN_GRAPH_TYPE_NAME(SUFFIX)* graph, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *from, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *to){\
        if(     !is_graph ## SUFFIX ## _node(graph, from)\
            ||  !is_graph ## SUFFIX ## _node(graph, to))\
        {\
            return 0;\
        }\
        return graph ## SUFFIX ## _node_remove_edge_by_to_node(from, to);\
    }\
    GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _node_find_edge(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* from, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *to){\
        GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) *edge;\
        edge = from->edges;\
        while(  edge != NULL\
            &&  edge->to != to)\
        {\
            edge = edge->next;\
        }\
        return edge;\
    }\
    GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX)* graph ## SUFFIX ## _node_find_edge_by_value(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX)* from, int value){\
        GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) *edge;\
        edge = from->edges;\
        while(  edge != NULL\
            &&  edge->value != value)\
        {\
            edge = edge->next;\
        }\
        return edge;\
    }\
    GEN_GRAPH_TYPE_NAME(SUFFIX)* make_complete_graph ## SUFFIX (size_t number_of_nodes){\
        size_t i, j;\
        GEN_GRAPH_TYPE_NAME(SUFFIX) *graph;\
        graph = make_empty_graph ## SUFFIX ();\
        for(i = 0; i < number_of_nodes; i++)\
        {\
            graph ## SUFFIX ## _add_node(graph);\
        }\
        for(i = 0; i < graph->node_count; i++)\
        {\
            for(j = 0; j < graph->node_count; j++)\
            {\
                if(i != j)\
                {\
                    graph ## SUFFIX ## _add_edge(graph, &graph->nodes[i], &graph->nodes[j]);\
                }\
            }\
        }\
        return graph;\
    }\
    int is_graph ## SUFFIX ## _node(GEN_GRAPH_TYPE_NAME(SUFFIX) *graph, GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *node){\
        GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *tmp = graph->nodes;\
        while(tmp != NULL)\
        {\
            if(tmp == node)\
            {\
                return 1;\
            }\
            tmp = tmp->next;\
        } \
        return 0;\
    }\
    int is_node ## SUFFIX ## _edge(GEN_GRAPH_NODE_TYPE_NAME(SUFFIX) *node, GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) *edge){\
        GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX) *tmp = node->edges;\
        while(tmp != NULL)\
        {\
            if(tmp == edge)\
            {\
                return 1;\
            }\
            tmp = tmp->next;\
        }\
        return 0;\
    }\
    
#define GEN_GRAPH_INIT_TYPE_TABLE(SUFFIX, TYPES, NUM)\
        graph ## SUFFIX ## _node_make_descriptor(&type_table[GEN_GRAPH_NODE_NUM(NUM)]);\
        graph ## SUFFIX ## _edge_make_descriptor(&type_table[GEN_GRAPH_EDGE_NUM(NUM)]);\
        graph ## SUFFIX ## _make_descriptor(&type_table[GEN_GRAPH_NUM(NUM)]);


#define GEN_GRAPH_FREE_TYPE_TABLE(SUFFIX, TYPES, NUM)\
        free(type_table[GEN_GRAPH_EDGE_NUM(NUM)].references);\
        free(type_table[GEN_GRAPH_NODE_NUM(NUM)].references);\
        free(type_table[GEN_GRAPH_NUM(NUM)].references);\
    
#define __GC_CUSTOM_GEN_GRAPH_SCAN_ARRAY_PTR_PER_TYPE(SUFFIX, TYPES, NUM)\
    __GC_CUSTOM_SCAN_ARRAY_PTR_PER_TYPE(GEN_GRAPH_NUM(NUM), GEN_GRAPH_TYPE_NAME(SUFFIX))\
    __GC_CUSTOM_SCAN_ARRAY_PTR_PER_TYPE(GEN_GRAPH_EDGE_NUM(NUM), GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX))\
    __GC_CUSTOM_SCAN_ARRAY_PTR_PER_TYPE(GEN_GRAPH_NODE_NUM(NUM), GEN_GRAPH_NODE_TYPE_NAME(SUFFIX))
    
    
#define __GC_CUSTOM_GEN_GRAPH_SCAN_PTR_PER_TYPE(SUFFIX, TYPES, NUM)\
    __GC_CUSTOM_SCAN_PTR_PER_TYPE(GEN_GRAPH_NUM(NUM), GEN_GRAPH_TYPE_NAME(SUFFIX))\
    __GC_CUSTOM_SCAN_PTR_PER_TYPE(GEN_GRAPH_NODE_NUM(NUM), GEN_GRAPH_NODE_TYPE_NAME(SUFFIX))\
    __GC_CUSTOM_SCAN_PTR_PER_TYPE(GEN_GRAPH_EDGE_NUM(NUM), GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX))

#define __GC_CUSTOM_GEN_GRAPH_MAKE_GC_SCAN_STRUCT_PER_TYPE(SUFFIX, TYPES, NUM)\
    case GEN_GRAPH_NUM(NUM):\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(sizeof(uint64_t) + sizeof(size_t), GEN_GRAPH_NODE_TYPE_NAME(SUFFIX))\
        break;\
    case GEN_GRAPH_NODE_NUM(NUM):\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(sizeof(int), GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX))\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(sizeof(int) + sizeof(graph_edge_t*), GEN_GRAPH_NODE_TYPE_NAME(SUFFIX))\
        break;\
    case GEN_GRAPH_EDGE_NUM(NUM):\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(sizeof(int), GEN_GRAPH_NODE_TYPE_NAME(SUFFIX))\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(sizeof(int) + sizeof(graph_node_t*), GEN_GRAPH_NODE_TYPE_NAME(SUFFIX))\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(sizeof(int) + 2*sizeof(graph_node_t*), GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX))\
        break;

#define __GC_CUSTOM_GEN_GRAPH_WALK_ARRAY_PER_TYPE(SUFFIX, TYPES, NUM)\
    __GC_CUSTOM_WALK_ARRAY_PER_TYPE(GEN_GRAPH_NUM(NUM), GEN_GRAPH_TYPE_NAME(SUFFIX))\
    __GC_CUSTOM_WALK_ARRAY_PER_TYPE(GEN_GRAPH_NODE_NUM(NUM), GEN_GRAPH_NODE_TYPE_NAME(SUFFIX))\
    __GC_CUSTOM_WALK_ARRAY_PER_TYPE(GEN_GRAPH_EDGE_NUM(NUM), GEN_GRAPH_EDGE_TYPE_NAME(SUFFIX))

/**
 * xmacro for btree type definition
 */
#define XGENERATE_TYPES_BTREE(X)\
    X(C, char a;, 0)\
    X(D, double a;, 1)\
    X(F, float a;, 2)\
    X(I, int a;, 3)\
    X(L, long a;, 4)\
    X(S, size_t a;, 5)\
    X(U, uint64_t a;, 6)\
    X(CC, char a; char aa;, 7)\
    X(CD, char a; double aa;, 8)\
    X(CF, char a; float aa;, 9)\
    X(CI, char a; int aa;, 10)\
    X(CL, char a; long aa;, 11)\
    X(CS, char a; size_t aa;, 12)\
    X(CU, char a; uint64_t aa;, 13)\
    X(DC, double a; char aa;, 14)\
    X(DD, double a; double aa;, 15)\
    X(DF, double a; float aa;, 16)\
    X(DI, double a; int aa;, 17)\
    X(DL, double a; long aa;, 18)\
    X(DS, double a; size_t aa;, 19)\
    X(DU, double a; uint64_t aa;, 20)\
    X(FC, float a; char aa;, 21)\
    X(FD, float a; double aa;, 22)\
    X(FF, float a; float aa;, 23)\
    X(FI, float a; int aa;, 24)\
    X(FL, float a; long aa;, 25)\
    X(FS, float a; size_t aa;, 26)\
    X(FU, float a; uint64_t aa;, 27)\
    X(IC, int a; char aa;, 28)\
    X(ID, int a; double aa;, 29)\
    X(IF, int a; float aa;, 30)\
    X(II, int a; int aa;, 31)\
    X(IL, int a; long aa;, 32)\
    X(IS, int a; size_t aa;, 33)\
    X(IU, int a; uint64_t aa;, 34)\
    X(LC, long a; char aa;, 35)\
    X(LD, long a; double aa;, 36)\
    X(LF, long a; float aa;, 37)\
    X(LI, long a; int aa;, 38)\
    X(LL, long a; long aa;, 39)\
    X(LS, long a; size_t aa;, 40)\
    X(LU, long a; uint64_t aa;, 41)\
    X(SC, size_t a; char aa;, 42)\
    X(SD, size_t a; double aa;, 43)\
    X(SF, size_t a; float aa;, 44)\
    X(SI, size_t a; int aa;, 45)\
    X(SL, size_t a; long aa;, 46)\
    X(SS, size_t a; size_t aa;, 47)\
    X(SU, size_t a; uint64_t aa;, 48)\
    X(UC, uint64_t a; char aa;, 49)\
    X(UD, uint64_t a; double aa;, 50)\
    X(UF, uint64_t a; float aa;, 51)\
    X(UI, uint64_t a; int aa;, 52)\
    X(UL, uint64_t a; long aa;, 53)\
    X(US, uint64_t a; size_t aa;, 54)\
    X(UU, uint64_t a; uint64_t aa;, 55)\
    X(CCC, char a; char aa; char aaa;, 56)\
    X(CCD, char a; char aa; double aaa;, 57)\
    X(CCF, char a; char aa; float aaa;, 58)\
    X(CCI, char a; char aa; int aaa;, 59)\
    X(CCL, char a; char aa; long aaa;, 60)\
    X(CCS, char a; char aa; size_t aaa;, 61)\
    X(CCU, char a; char aa; uint64_t aaa;, 62)\
    X(CDC, char a; double aa; char aaa;, 63)\
    X(CDD, char a; double aa; double aaa;, 64)\
    X(CDF, char a; double aa; float aaa;, 65)\
    X(CDI, char a; double aa; int aaa;, 66)\
    X(CDL, char a; double aa; long aaa;, 67)\
    X(CDS, char a; double aa; size_t aaa;, 68)\
    X(CDU, char a; double aa; uint64_t aaa;, 69)\
    X(CFC, char a; float aa; char aaa;, 70)\
    X(CFD, char a; float aa; double aaa;, 71)\
    X(CFF, char a; float aa; float aaa;, 72)\
    X(CFI, char a; float aa; int aaa;, 73)\
    X(CFL, char a; float aa; long aaa;, 74)\
    X(CFS, char a; float aa; size_t aaa;, 75)\
    X(CFU, char a; float aa; uint64_t aaa;, 76)\
    X(CIC, char a; int aa; char aaa;, 77)\
    X(CID, char a; int aa; double aaa;, 78)\
    X(CIF, char a; int aa; float aaa;, 79)\
    X(CII, char a; int aa; int aaa;, 80)\
    X(CIL, char a; int aa; long aaa;, 81)\
    X(CIS, char a; int aa; size_t aaa;, 82)\
    X(CIU, char a; int aa; uint64_t aaa;, 83)\
    X(CLC, char a; long aa; char aaa;, 84)\
    X(CLD, char a; long aa; double aaa;, 85)\
    X(CLF, char a; long aa; float aaa;, 86)\
    X(CLI, char a; long aa; int aaa;, 87)\
    X(CLL, char a; long aa; long aaa;, 88)\
    X(CLS, char a; long aa; size_t aaa;, 89)\
    X(CLU, char a; long aa; uint64_t aaa;, 90)\
    X(CSC, char a; size_t aa; char aaa;, 91)\
    X(CSD, char a; size_t aa; double aaa;, 92)\
    X(CSF, char a; size_t aa; float aaa;, 93)\
    X(CSI, char a; size_t aa; int aaa;, 94)\
    X(CSL, char a; size_t aa; long aaa;, 95)\
    X(CSS, char a; size_t aa; size_t aaa;, 96)\
    X(CSU, char a; size_t aa; uint64_t aaa;, 97)\
    X(CUC, char a; uint64_t aa; char aaa;, 98)\
    X(CUD, char a; uint64_t aa; double aaa;, 99)\
    X(CUF, char a; uint64_t aa; float aaa;, 100)\
    X(CUI, char a; uint64_t aa; int aaa;, 101)\
    X(CUL, char a; uint64_t aa; long aaa;, 102)\
    X(CUS, char a; uint64_t aa; size_t aaa;, 103)\
    X(CUU, char a; uint64_t aa; uint64_t aaa;, 104)\
    X(DCC, double a; char aa; char aaa;, 105)\
    X(DCD, double a; char aa; double aaa;, 106)\
    X(DCF, double a; char aa; float aaa;, 107)\
    X(DCI, double a; char aa; int aaa;, 108)\
    X(DCL, double a; char aa; long aaa;, 109)\
    X(DCS, double a; char aa; size_t aaa;, 110)\
    X(DCU, double a; char aa; uint64_t aaa;, 111)\
    X(DDC, double a; double aa; char aaa;, 112)\
    X(DDD, double a; double aa; double aaa;, 113)\
    X(DDF, double a; double aa; float aaa;, 114)\
    X(DDI, double a; double aa; int aaa;, 115)\
    X(DDL, double a; double aa; long aaa;, 116)\
    X(DDS, double a; double aa; size_t aaa;, 117)\
    X(DDU, double a; double aa; uint64_t aaa;, 118)\
    X(DFC, double a; float aa; char aaa;, 119)\
    X(DFD, double a; float aa; double aaa;, 120)\
    X(DFF, double a; float aa; float aaa;, 121)\
    X(DFI, double a; float aa; int aaa;, 122)\
    X(DFL, double a; float aa; long aaa;, 123)\
    X(DFS, double a; float aa; size_t aaa;, 124)\
    X(DFU, double a; float aa; uint64_t aaa;, 125)\
    X(DIC, double a; int aa; char aaa;, 126)\
    X(DID, double a; int aa; double aaa;, 127)\
    X(DIF, double a; int aa; float aaa;, 128)\
    X(DII, double a; int aa; int aaa;, 129)\
    X(DIL, double a; int aa; long aaa;, 130)\
    X(DIS, double a; int aa; size_t aaa;, 131)\
    X(DIU, double a; int aa; uint64_t aaa;, 132)\
    X(DLC, double a; long aa; char aaa;, 133)\
    X(DLD, double a; long aa; double aaa;, 134)\
    X(DLF, double a; long aa; float aaa;, 135)\
    X(DLI, double a; long aa; int aaa;, 136)\
    X(DLL, double a; long aa; long aaa;, 137)\
    X(DLS, double a; long aa; size_t aaa;, 138)\
    X(DLU, double a; long aa; uint64_t aaa;, 139)\
    X(DSC, double a; size_t aa; char aaa;, 140)\
    X(DSD, double a; size_t aa; double aaa;, 141)\
    X(DSF, double a; size_t aa; float aaa;, 142)\
    X(DSI, double a; size_t aa; int aaa;, 143)\
    X(DSL, double a; size_t aa; long aaa;, 144)\
    X(DSS, double a; size_t aa; size_t aaa;, 145)\
    X(DSU, double a; size_t aa; uint64_t aaa;, 146)\
    X(DUC, double a; uint64_t aa; char aaa;, 147)\
    X(DUD, double a; uint64_t aa; double aaa;, 148)\
    X(DUF, double a; uint64_t aa; float aaa;, 149)\
    X(DUI, double a; uint64_t aa; int aaa;, 150)\
    X(DUL, double a; uint64_t aa; long aaa;, 151)\
    X(DUS, double a; uint64_t aa; size_t aaa;, 152)\
    X(DUU, double a; uint64_t aa; uint64_t aaa;, 153)\
    X(FCC, float a; char aa; char aaa;, 154)\
    X(FCD, float a; char aa; double aaa;, 155)\
    X(FCF, float a; char aa; float aaa;, 156)\
    X(FCI, float a; char aa; int aaa;, 157)\
    X(FCL, float a; char aa; long aaa;, 158)\
    X(FCS, float a; char aa; size_t aaa;, 159)\
    X(FCU, float a; char aa; uint64_t aaa;, 160)\
    X(FDC, float a; double aa; char aaa;, 161)\
    X(FDD, float a; double aa; double aaa;, 162)\
    X(FDF, float a; double aa; float aaa;, 163)\
    X(FDI, float a; double aa; int aaa;, 164)\
    X(FDL, float a; double aa; long aaa;, 165)\
    X(FDS, float a; double aa; size_t aaa;, 166)\
    X(FDU, float a; double aa; uint64_t aaa;, 167)\
    X(FFC, float a; float aa; char aaa;, 168)\
    X(FFD, float a; float aa; double aaa;, 169)\
    X(FFF, float a; float aa; float aaa;, 170)\
    X(FFI, float a; float aa; int aaa;, 171)\
    X(FFL, float a; float aa; long aaa;, 172)\
    X(FFS, float a; float aa; size_t aaa;, 173)\
    X(FFU, float a; float aa; uint64_t aaa;, 174)\
    X(FIC, float a; int aa; char aaa;, 175)\
    X(FID, float a; int aa; double aaa;, 176)\
    X(FIF, float a; int aa; float aaa;, 177)\
    X(FII, float a; int aa; int aaa;, 178)\
    X(FIL, float a; int aa; long aaa;, 179)\
    X(FIS, float a; int aa; size_t aaa;, 180)\
    X(FIU, float a; int aa; uint64_t aaa;, 181)\
    X(FLC, float a; long aa; char aaa;, 182)\
    X(FLD, float a; long aa; double aaa;, 183)\
    X(FLF, float a; long aa; float aaa;, 184)\
    X(FLI, float a; long aa; int aaa;, 185)\
    X(FLL, float a; long aa; long aaa;, 186)\
    X(FLS, float a; long aa; size_t aaa;, 187)\
    X(FLU, float a; long aa; uint64_t aaa;, 188)\
    X(FSC, float a; size_t aa; char aaa;, 189)\
    X(FSD, float a; size_t aa; double aaa;, 190)\
    X(FSF, float a; size_t aa; float aaa;, 191)\
    X(FSI, float a; size_t aa; int aaa;, 192)\
    X(FSL, float a; size_t aa; long aaa;, 193)\
    X(FSS, float a; size_t aa; size_t aaa;, 194)\
    X(FSU, float a; size_t aa; uint64_t aaa;, 195)\
    X(FUC, float a; uint64_t aa; char aaa;, 196)\
    X(FUD, float a; uint64_t aa; double aaa;, 197)\
    X(FUF, float a; uint64_t aa; float aaa;, 198)\
    X(FUI, float a; uint64_t aa; int aaa;, 199)\
    X(FUL, float a; uint64_t aa; long aaa;, 200)\
    X(FUS, float a; uint64_t aa; size_t aaa;, 201)\
    X(FUU, float a; uint64_t aa; uint64_t aaa;, 202)\
    X(ICC, int a; char aa; char aaa;, 203)\
    X(ICD, int a; char aa; double aaa;, 204)\
    X(ICF, int a; char aa; float aaa;, 205)\
    X(ICI, int a; char aa; int aaa;, 206)\
    X(ICL, int a; char aa; long aaa;, 207)\
    X(ICS, int a; char aa; size_t aaa;, 208)\
    X(ICU, int a; char aa; uint64_t aaa;, 209)\
    X(IDC, int a; double aa; char aaa;, 210)\
    X(IDD, int a; double aa; double aaa;, 211)\
    X(IDF, int a; double aa; float aaa;, 212)\
    X(IDI, int a; double aa; int aaa;, 213)\
    X(IDL, int a; double aa; long aaa;, 214)\
    X(IDS, int a; double aa; size_t aaa;, 215)\
    X(IDU, int a; double aa; uint64_t aaa;, 216)\
    X(IFC, int a; float aa; char aaa;, 217)\
    X(IFD, int a; float aa; double aaa;, 218)\
    X(IFF, int a; float aa; float aaa;, 219)\
    X(IFI, int a; float aa; int aaa;, 220)\
    X(IFL, int a; float aa; long aaa;, 221)\
    X(IFS, int a; float aa; size_t aaa;, 222)\
    X(IFU, int a; float aa; uint64_t aaa;, 223)\
    X(IIC, int a; int aa; char aaa;, 224)\
    X(IID, int a; int aa; double aaa;, 225)\
    X(IIF, int a; int aa; float aaa;, 226)\
    X(III, int a; int aa; int aaa;, 227)\
    X(IIL, int a; int aa; long aaa;, 228)\
    X(IIS, int a; int aa; size_t aaa;, 229)\
    X(IIU, int a; int aa; uint64_t aaa;, 230)\
    X(ILC, int a; long aa; char aaa;, 231)\
    X(ILD, int a; long aa; double aaa;, 232)\
    X(ILF, int a; long aa; float aaa;, 233)\
    X(ILI, int a; long aa; int aaa;, 234)\
    X(ILL, int a; long aa; long aaa;, 235)\
    X(ILS, int a; long aa; size_t aaa;, 236)\
    X(ILU, int a; long aa; uint64_t aaa;, 237)\
    X(ISC, int a; size_t aa; char aaa;, 238)\
    X(ISD, int a; size_t aa; double aaa;, 239)\
    X(ISF, int a; size_t aa; float aaa;, 240)\
    X(ISI, int a; size_t aa; int aaa;, 241)\
    X(ISL, int a; size_t aa; long aaa;, 242)\
    X(ISS, int a; size_t aa; size_t aaa;, 243)\
    X(ISU, int a; size_t aa; uint64_t aaa;, 244)\
    X(IUC, int a; uint64_t aa; char aaa;, 245)\
    X(IUD, int a; uint64_t aa; double aaa;, 246)\
    X(IUF, int a; uint64_t aa; float aaa;, 247)\
    X(IUI, int a; uint64_t aa; int aaa;, 248)\
    X(IUL, int a; uint64_t aa; long aaa;, 249)\
    X(IUS, int a; uint64_t aa; size_t aaa;, 250)\
    X(IUU, int a; uint64_t aa; uint64_t aaa;, 251)\
    X(LCC, long a; char aa; char aaa;, 252)\
    X(LCD, long a; char aa; double aaa;, 253)\
    X(LCF, long a; char aa; float aaa;, 254)\
    X(LCI, long a; char aa; int aaa;, 255)\
    X(LCL, long a; char aa; long aaa;, 256)\
    X(LCS, long a; char aa; size_t aaa;, 257)\
    X(LCU, long a; char aa; uint64_t aaa;, 258)\
    X(LDC, long a; double aa; char aaa;, 259)\
    X(LDD, long a; double aa; double aaa;, 260)\
    X(LDF, long a; double aa; float aaa;, 261)\
    X(LDI, long a; double aa; int aaa;, 262)\
    X(LDL, long a; double aa; long aaa;, 263)\
    X(LDS, long a; double aa; size_t aaa;, 264)\
    X(LDU, long a; double aa; uint64_t aaa;, 265)\
    X(LFC, long a; float aa; char aaa;, 266)\
    X(LFD, long a; float aa; double aaa;, 267)\
    X(LFF, long a; float aa; float aaa;, 268)\
    X(LFI, long a; float aa; int aaa;, 269)\
    X(LFL, long a; float aa; long aaa;, 270)\
    X(LFS, long a; float aa; size_t aaa;, 271)\
    X(LFU, long a; float aa; uint64_t aaa;, 272)\
    X(LIC, long a; int aa; char aaa;, 273)\
    X(LID, long a; int aa; double aaa;, 274)\
    X(LIF, long a; int aa; float aaa;, 275)\
    X(LII, long a; int aa; int aaa;, 276)\
    X(LIL, long a; int aa; long aaa;, 277)\
    X(LIS, long a; int aa; size_t aaa;, 278)\
    X(LIU, long a; int aa; uint64_t aaa;, 279)\
    X(LLC, long a; long aa; char aaa;, 280)\
    X(LLD, long a; long aa; double aaa;, 281)\
    X(LLF, long a; long aa; float aaa;, 282)\
    X(LLI, long a; long aa; int aaa;, 283)\
    X(LLL, long a; long aa; long aaa;, 284)\
    X(LLS, long a; long aa; size_t aaa;, 285)\
    X(LLU, long a; long aa; uint64_t aaa;, 286)\
    X(LSC, long a; size_t aa; char aaa;, 287)\
    X(LSD, long a; size_t aa; double aaa;, 288)\
    X(LSF, long a; size_t aa; float aaa;, 289)\
    X(LSI, long a; size_t aa; int aaa;, 290)\
    X(LSL, long a; size_t aa; long aaa;, 291)\
    X(LSS, long a; size_t aa; size_t aaa;, 292)\
    X(LSU, long a; size_t aa; uint64_t aaa;, 293)\
    X(LUC, long a; uint64_t aa; char aaa;, 294)\
    X(LUD, long a; uint64_t aa; double aaa;, 295)\
    X(LUF, long a; uint64_t aa; float aaa;, 296)\
    X(LUI, long a; uint64_t aa; int aaa;, 297)\
    X(LUL, long a; uint64_t aa; long aaa;, 298)\
    X(LUS, long a; uint64_t aa; size_t aaa;, 299)\
    X(LUU, long a; uint64_t aa; uint64_t aaa;, 300)\
    X(SCC, size_t a; char aa; char aaa;, 301)\
    X(SCD, size_t a; char aa; double aaa;, 302)\
    X(SCF, size_t a; char aa; float aaa;, 303)\
    X(SCI, size_t a; char aa; int aaa;, 304)\
    X(SCL, size_t a; char aa; long aaa;, 305)\
    X(SCS, size_t a; char aa; size_t aaa;, 306)\
    X(SCU, size_t a; char aa; uint64_t aaa;, 307)\
    X(SDC, size_t a; double aa; char aaa;, 308)\
    X(SDD, size_t a; double aa; double aaa;, 309)\
    X(SDF, size_t a; double aa; float aaa;, 310)\
    X(SDI, size_t a; double aa; int aaa;, 311)\
    X(SDL, size_t a; double aa; long aaa;, 312)\
    X(SDS, size_t a; double aa; size_t aaa;, 313)\
    X(SDU, size_t a; double aa; uint64_t aaa;, 314)\
    X(SFC, size_t a; float aa; char aaa;, 315)\
    X(SFD, size_t a; float aa; double aaa;, 316)\
    X(SFF, size_t a; float aa; float aaa;, 317)\
    X(SFI, size_t a; float aa; int aaa;, 318)\
    X(SFL, size_t a; float aa; long aaa;, 319)\
    X(SFS, size_t a; float aa; size_t aaa;, 320)\
    X(SFU, size_t a; float aa; uint64_t aaa;, 321)\
    X(SIC, size_t a; int aa; char aaa;, 322)\
    X(SID, size_t a; int aa; double aaa;, 323)\
    X(SIF, size_t a; int aa; float aaa;, 324)\
    X(SII, size_t a; int aa; int aaa;, 325)\
    X(SIL, size_t a; int aa; long aaa;, 326)\
    X(SIS, size_t a; int aa; size_t aaa;, 327)\
    X(SIU, size_t a; int aa; uint64_t aaa;, 328)\
    X(SLC, size_t a; long aa; char aaa;, 329)\
    X(SLD, size_t a; long aa; double aaa;, 330)\
    X(SLF, size_t a; long aa; float aaa;, 331)\
    X(SLI, size_t a; long aa; int aaa;, 332)\
    X(SLL, size_t a; long aa; long aaa;, 333)\
    X(SLS, size_t a; long aa; size_t aaa;, 334)\
    X(SLU, size_t a; long aa; uint64_t aaa;, 335)\
    X(SSC, size_t a; size_t aa; char aaa;, 336)\
    X(SSD, size_t a; size_t aa; double aaa;, 337)\
    X(SSF, size_t a; size_t aa; float aaa;, 338)\
    X(SSI, size_t a; size_t aa; int aaa;, 339)\
    X(SSL, size_t a; size_t aa; long aaa;, 340)\
    X(SSS, size_t a; size_t aa; size_t aaa;, 341)\
    X(SSU, size_t a; size_t aa; uint64_t aaa;, 342)\
    X(SUC, size_t a; uint64_t aa; char aaa;, 343)\
    X(SUD, size_t a; uint64_t aa; double aaa;, 344)\
    X(SUF, size_t a; uint64_t aa; float aaa;, 345)\
    X(SUI, size_t a; uint64_t aa; int aaa;, 346)\
    X(SUL, size_t a; uint64_t aa; long aaa;, 347)\
    X(SUS, size_t a; uint64_t aa; size_t aaa;, 348)\
    X(SUU, size_t a; uint64_t aa; uint64_t aaa;, 349)\
    X(UCC, uint64_t a; char aa; char aaa;, 350)\
    X(UCD, uint64_t a; char aa; double aaa;, 351)\
    X(UCF, uint64_t a; char aa; float aaa;, 352)\
    X(UCI, uint64_t a; char aa; int aaa;, 353)\
    X(UCL, uint64_t a; char aa; long aaa;, 354)\
    X(UCS, uint64_t a; char aa; size_t aaa;, 355)\
    X(UCU, uint64_t a; char aa; uint64_t aaa;, 356)\
    X(UDC, uint64_t a; double aa; char aaa;, 357)\
    X(UDD, uint64_t a; double aa; double aaa;, 358)\
    X(UDF, uint64_t a; double aa; float aaa;, 359)\
    X(UDI, uint64_t a; double aa; int aaa;, 360)\
    X(UDL, uint64_t a; double aa; long aaa;, 361)\
    X(UDS, uint64_t a; double aa; size_t aaa;, 362)\
    X(UDU, uint64_t a; double aa; uint64_t aaa;, 363)\
    X(UFC, uint64_t a; float aa; char aaa;, 364)\
    X(UFD, uint64_t a; float aa; double aaa;, 365)\
    X(UFF, uint64_t a; float aa; float aaa;, 366)\
    X(UFI, uint64_t a; float aa; int aaa;, 367)\
    X(UFL, uint64_t a; float aa; long aaa;, 368)\
    X(UFS, uint64_t a; float aa; size_t aaa;, 369)\
    X(UFU, uint64_t a; float aa; uint64_t aaa;, 370)\
    X(UIC, uint64_t a; int aa; char aaa;, 371)\
    X(UID, uint64_t a; int aa; double aaa;, 372)\
    X(UIF, uint64_t a; int aa; float aaa;, 373)\
    X(UII, uint64_t a; int aa; int aaa;, 374)\
    X(UIL, uint64_t a; int aa; long aaa;, 375)\
    X(UIS, uint64_t a; int aa; size_t aaa;, 376)\
    X(UIU, uint64_t a; int aa; uint64_t aaa;, 377)\
    X(ULC, uint64_t a; long aa; char aaa;, 378)\
    X(ULD, uint64_t a; long aa; double aaa;, 379)\
    X(ULF, uint64_t a; long aa; float aaa;, 380)\
    X(ULI, uint64_t a; long aa; int aaa;, 381)\
    X(ULL, uint64_t a; long aa; long aaa;, 382)\
    X(ULS, uint64_t a; long aa; size_t aaa;, 383)\
    X(ULU, uint64_t a; long aa; uint64_t aaa;, 384)\
    X(USC, uint64_t a; size_t aa; char aaa;, 385)\
    X(USD, uint64_t a; size_t aa; double aaa;, 386)\
    X(USF, uint64_t a; size_t aa; float aaa;, 387)\
    X(USI, uint64_t a; size_t aa; int aaa;, 388)\
    X(USL, uint64_t a; size_t aa; long aaa;, 389)\
    X(USS, uint64_t a; size_t aa; size_t aaa;, 390)\
    X(USU, uint64_t a; size_t aa; uint64_t aaa;, 391)\
    X(UUC, uint64_t a; uint64_t aa; char aaa;, 392)\
    X(UUD, uint64_t a; uint64_t aa; double aaa;, 393)\
    X(UUF, uint64_t a; uint64_t aa; float aaa;, 394)\
    X(UUI, uint64_t a; uint64_t aa; int aaa;, 395)\
    X(UUL, uint64_t a; uint64_t aa; long aaa;, 396)\
    X(UUS, uint64_t a; uint64_t aa; size_t aaa;, 397)\
    X(UUU, uint64_t a; uint64_t aa; uint64_t aaa;, 398)\
    X(CCCC, char a; char aa; char aaa; char aaaa;, 399)\
    X(CCCD, char a; char aa; char aaa; double aaaa;, 400)\
    X(CCCF, char a; char aa; char aaa; float aaaa;, 401)\
    X(CCCI, char a; char aa; char aaa; int aaaa;, 402)\
    X(CCCL, char a; char aa; char aaa; long aaaa;, 403)\
    X(CCCS, char a; char aa; char aaa; size_t aaaa;, 404)\
    X(CCCU, char a; char aa; char aaa; uint64_t aaaa;, 405)\
    X(CCDC, char a; char aa; double aaa; char aaaa;, 406)\
    X(CCDD, char a; char aa; double aaa; double aaaa;, 407)\
    X(CCDF, char a; char aa; double aaa; float aaaa;, 408)\
    X(CCDI, char a; char aa; double aaa; int aaaa;, 409)\
    X(CCDL, char a; char aa; double aaa; long aaaa;, 410)\
    X(CCDS, char a; char aa; double aaa; size_t aaaa;, 411)\
    X(CCDU, char a; char aa; double aaa; uint64_t aaaa;, 412)\
    X(CCFC, char a; char aa; float aaa; char aaaa;, 413)\
    X(CCFD, char a; char aa; float aaa; double aaaa;, 414)\
    X(CCFF, char a; char aa; float aaa; float aaaa;, 415)\
    X(CCFI, char a; char aa; float aaa; int aaaa;, 416)\
    X(CCFL, char a; char aa; float aaa; long aaaa;, 417)\
    X(CCFS, char a; char aa; float aaa; size_t aaaa;, 418)\
    X(CCFU, char a; char aa; float aaa; uint64_t aaaa;, 419)\
    X(CCIC, char a; char aa; int aaa; char aaaa;, 420)\
    X(CCID, char a; char aa; int aaa; double aaaa;, 421)\
    X(CCIF, char a; char aa; int aaa; float aaaa;, 422)\
    X(CCII, char a; char aa; int aaa; int aaaa;, 423)\
    X(CCIL, char a; char aa; int aaa; long aaaa;, 424)\
    X(CCIS, char a; char aa; int aaa; size_t aaaa;, 425)\
    X(CCIU, char a; char aa; int aaa; uint64_t aaaa;, 426)\
    X(CCLC, char a; char aa; long aaa; char aaaa;, 427)\
    X(CCLD, char a; char aa; long aaa; double aaaa;, 428)\
    X(CCLF, char a; char aa; long aaa; float aaaa;, 429)\
    X(CCLI, char a; char aa; long aaa; int aaaa;, 430)\
    X(CCLL, char a; char aa; long aaa; long aaaa;, 431)\
    X(CCLS, char a; char aa; long aaa; size_t aaaa;, 432)\
    X(CCLU, char a; char aa; long aaa; uint64_t aaaa;, 433)\
    X(CCSC, char a; char aa; size_t aaa; char aaaa;, 434)\
    X(CCSD, char a; char aa; size_t aaa; double aaaa;, 435)\
    X(CCSF, char a; char aa; size_t aaa; float aaaa;, 436)\
    X(CCSI, char a; char aa; size_t aaa; int aaaa;, 437)\
    X(CCSL, char a; char aa; size_t aaa; long aaaa;, 438)\
    X(CCSS, char a; char aa; size_t aaa; size_t aaaa;, 439)\
    X(CCSU, char a; char aa; size_t aaa; uint64_t aaaa;, 440)\
    X(CCUC, char a; char aa; uint64_t aaa; char aaaa;, 441)\
    X(CCUD, char a; char aa; uint64_t aaa; double aaaa;, 442)\
    X(CCUF, char a; char aa; uint64_t aaa; float aaaa;, 443)\
    X(CCUI, char a; char aa; uint64_t aaa; int aaaa;, 444)\
    X(CCUL, char a; char aa; uint64_t aaa; long aaaa;, 445)\
    X(CCUS, char a; char aa; uint64_t aaa; size_t aaaa;, 446)\
    X(CCUU, char a; char aa; uint64_t aaa; uint64_t aaaa;, 447)\
    X(CDCC, char a; double aa; char aaa; char aaaa;, 448)\
    X(CDCD, char a; double aa; char aaa; double aaaa;, 449)\
    X(CDCF, char a; double aa; char aaa; float aaaa;, 450)\
    X(CDCI, char a; double aa; char aaa; int aaaa;, 451)\
    X(CDCL, char a; double aa; char aaa; long aaaa;, 452)\
    X(CDCS, char a; double aa; char aaa; size_t aaaa;, 453)\
    X(CDCU, char a; double aa; char aaa; uint64_t aaaa;, 454)\
    X(CDDC, char a; double aa; double aaa; char aaaa;, 455)\
    X(CDDD, char a; double aa; double aaa; double aaaa;, 456)\
    X(CDDF, char a; double aa; double aaa; float aaaa;, 457)\
    X(CDDI, char a; double aa; double aaa; int aaaa;, 458)\
    X(CDDL, char a; double aa; double aaa; long aaaa;, 459)\
    X(CDDS, char a; double aa; double aaa; size_t aaaa;, 460)\
    X(CDDU, char a; double aa; double aaa; uint64_t aaaa;, 461)\
    X(CDFC, char a; double aa; float aaa; char aaaa;, 462)\
    X(CDFD, char a; double aa; float aaa; double aaaa;, 463)\
    X(CDFF, char a; double aa; float aaa; float aaaa;, 464)\
    X(CDFI, char a; double aa; float aaa; int aaaa;, 465)\
    X(CDFL, char a; double aa; float aaa; long aaaa;, 466)\
    X(CDFS, char a; double aa; float aaa; size_t aaaa;, 467)\
    X(CDFU, char a; double aa; float aaa; uint64_t aaaa;, 468)\
    X(CDIC, char a; double aa; int aaa; char aaaa;, 469)\
    X(CDID, char a; double aa; int aaa; double aaaa;, 470)\
    X(CDIF, char a; double aa; int aaa; float aaaa;, 471)\
    X(CDII, char a; double aa; int aaa; int aaaa;, 472)\
    X(CDIL, char a; double aa; int aaa; long aaaa;, 473)\
    X(CDIS, char a; double aa; int aaa; size_t aaaa;, 474)\
    X(CDIU, char a; double aa; int aaa; uint64_t aaaa;, 475)\
    X(CDLC, char a; double aa; long aaa; char aaaa;, 476)\
    X(CDLD, char a; double aa; long aaa; double aaaa;, 477)\
    X(CDLF, char a; double aa; long aaa; float aaaa;, 478)\
    X(CDLI, char a; double aa; long aaa; int aaaa;, 479)\
    X(CDLL, char a; double aa; long aaa; long aaaa;, 480)\
    X(CDLS, char a; double aa; long aaa; size_t aaaa;, 481)\
    X(CDLU, char a; double aa; long aaa; uint64_t aaaa;, 482)\
    X(CDSC, char a; double aa; size_t aaa; char aaaa;, 483)\
    X(CDSD, char a; double aa; size_t aaa; double aaaa;, 484)\
    X(CDSF, char a; double aa; size_t aaa; float aaaa;, 485)\
    X(CDSI, char a; double aa; size_t aaa; int aaaa;, 486)\
    X(CDSL, char a; double aa; size_t aaa; long aaaa;, 487)\
    X(CDSS, char a; double aa; size_t aaa; size_t aaaa;, 488)\
    X(CDSU, char a; double aa; size_t aaa; uint64_t aaaa;, 489)\
    X(CDUC, char a; double aa; uint64_t aaa; char aaaa;, 490)\
    X(CDUD, char a; double aa; uint64_t aaa; double aaaa;, 491)\
    X(CDUF, char a; double aa; uint64_t aaa; float aaaa;, 492)\
    X(CDUI, char a; double aa; uint64_t aaa; int aaaa;, 493)\
    X(CDUL, char a; double aa; uint64_t aaa; long aaaa;, 494)\
    X(CDUS, char a; double aa; uint64_t aaa; size_t aaaa;, 495)\
    X(CDUU, char a; double aa; uint64_t aaa; uint64_t aaaa;, 496)\
    X(CFCC, char a; float aa; char aaa; char aaaa;, 497)\
    X(CFCD, char a; float aa; char aaa; double aaaa;, 498)\
    X(CFCF, char a; float aa; char aaa; float aaaa;, 499)\
    X(CFCI, char a; float aa; char aaa; int aaaa;, 500)\
    X(CFCL, char a; float aa; char aaa; long aaaa;, 501)\
    X(CFCS, char a; float aa; char aaa; size_t aaaa;, 502)\
    X(CFCU, char a; float aa; char aaa; uint64_t aaaa;, 503)\
    X(CFDC, char a; float aa; double aaa; char aaaa;, 504)\
    X(CFDD, char a; float aa; double aaa; double aaaa;, 505)\
    X(CFDF, char a; float aa; double aaa; float aaaa;, 506)\
    X(CFDI, char a; float aa; double aaa; int aaaa;, 507)\
    X(CFDL, char a; float aa; double aaa; long aaaa;, 508)\
    X(CFDS, char a; float aa; double aaa; size_t aaaa;, 509)\
    X(CFDU, char a; float aa; double aaa; uint64_t aaaa;, 510)\
    X(CFFC, char a; float aa; float aaa; char aaaa;, 511)\
    X(CFFD, char a; float aa; float aaa; double aaaa;, 512)\
    X(CFFF, char a; float aa; float aaa; float aaaa;, 513)\
    X(CFFI, char a; float aa; float aaa; int aaaa;, 514)\
    X(CFFL, char a; float aa; float aaa; long aaaa;, 515)\
    X(CFFS, char a; float aa; float aaa; size_t aaaa;, 516)\
    X(CFFU, char a; float aa; float aaa; uint64_t aaaa;, 517)\
    X(CFIC, char a; float aa; int aaa; char aaaa;, 518)\
    X(CFID, char a; float aa; int aaa; double aaaa;, 519)\
    X(CFIF, char a; float aa; int aaa; float aaaa;, 520)\
    X(CFII, char a; float aa; int aaa; int aaaa;, 521)\
    X(CFIL, char a; float aa; int aaa; long aaaa;, 522)\
    X(CFIS, char a; float aa; int aaa; size_t aaaa;, 523)\
    X(CFIU, char a; float aa; int aaa; uint64_t aaaa;, 524)\
    X(CFLC, char a; float aa; long aaa; char aaaa;, 525)\
    X(CFLD, char a; float aa; long aaa; double aaaa;, 526)\
    X(CFLF, char a; float aa; long aaa; float aaaa;, 527)\
    X(CFLI, char a; float aa; long aaa; int aaaa;, 528)\
    X(CFLL, char a; float aa; long aaa; long aaaa;, 529)\
    X(CFLS, char a; float aa; long aaa; size_t aaaa;, 530)\
    X(CFLU, char a; float aa; long aaa; uint64_t aaaa;, 531)\
    X(CFSC, char a; float aa; size_t aaa; char aaaa;, 532)\
    X(CFSD, char a; float aa; size_t aaa; double aaaa;, 533)\
    X(CFSF, char a; float aa; size_t aaa; float aaaa;, 534)\
    X(CFSI, char a; float aa; size_t aaa; int aaaa;, 535)\
    X(CFSL, char a; float aa; size_t aaa; long aaaa;, 536)\
    X(CFSS, char a; float aa; size_t aaa; size_t aaaa;, 537)\
    X(CFSU, char a; float aa; size_t aaa; uint64_t aaaa;, 538)\
    X(CFUC, char a; float aa; uint64_t aaa; char aaaa;, 539)\
    X(CFUD, char a; float aa; uint64_t aaa; double aaaa;, 540)\
    X(CFUF, char a; float aa; uint64_t aaa; float aaaa;, 541)\
    X(CFUI, char a; float aa; uint64_t aaa; int aaaa;, 542)\
    X(CFUL, char a; float aa; uint64_t aaa; long aaaa;, 543)\
    X(CFUS, char a; float aa; uint64_t aaa; size_t aaaa;, 544)\
    X(CFUU, char a; float aa; uint64_t aaa; uint64_t aaaa;, 545)\
    X(CICC, char a; int aa; char aaa; char aaaa;, 546)\
    X(CICD, char a; int aa; char aaa; double aaaa;, 547)\
    X(CICF, char a; int aa; char aaa; float aaaa;, 548)\
    X(CICI, char a; int aa; char aaa; int aaaa;, 549)\
    X(CICL, char a; int aa; char aaa; long aaaa;, 550)\
    X(CICS, char a; int aa; char aaa; size_t aaaa;, 551)\
    X(CICU, char a; int aa; char aaa; uint64_t aaaa;, 552)\
    X(CIDC, char a; int aa; double aaa; char aaaa;, 553)\
    X(CIDD, char a; int aa; double aaa; double aaaa;, 554)\
    X(CIDF, char a; int aa; double aaa; float aaaa;, 555)\
    X(CIDI, char a; int aa; double aaa; int aaaa;, 556)\
    X(CIDL, char a; int aa; double aaa; long aaaa;, 557)\
    X(CIDS, char a; int aa; double aaa; size_t aaaa;, 558)\
    X(CIDU, char a; int aa; double aaa; uint64_t aaaa;, 559)\
    X(CIFC, char a; int aa; float aaa; char aaaa;, 560)\
    X(CIFD, char a; int aa; float aaa; double aaaa;, 561)\
    X(CIFF, char a; int aa; float aaa; float aaaa;, 562)\
    X(CIFI, char a; int aa; float aaa; int aaaa;, 563)\
    X(CIFL, char a; int aa; float aaa; long aaaa;, 564)\
    X(CIFS, char a; int aa; float aaa; size_t aaaa;, 565)\
    X(CIFU, char a; int aa; float aaa; uint64_t aaaa;, 566)\
    X(CIIC, char a; int aa; int aaa; char aaaa;, 567)\
    X(CIID, char a; int aa; int aaa; double aaaa;, 568)\
    X(CIIF, char a; int aa; int aaa; float aaaa;, 569)\
    X(CIII, char a; int aa; int aaa; int aaaa;, 570)\
    X(CIIL, char a; int aa; int aaa; long aaaa;, 571)\
    X(CIIS, char a; int aa; int aaa; size_t aaaa;, 572)\
    X(CIIU, char a; int aa; int aaa; uint64_t aaaa;, 573)\
    X(CILC, char a; int aa; long aaa; char aaaa;, 574)\
    X(CILD, char a; int aa; long aaa; double aaaa;, 575)\
    X(CILF, char a; int aa; long aaa; float aaaa;, 576)\
    X(CILI, char a; int aa; long aaa; int aaaa;, 577)\
    X(CILL, char a; int aa; long aaa; long aaaa;, 578)\
    X(CILS, char a; int aa; long aaa; size_t aaaa;, 579)\
    X(CILU, char a; int aa; long aaa; uint64_t aaaa;, 580)\
    X(CISC, char a; int aa; size_t aaa; char aaaa;, 581)\
    X(CISD, char a; int aa; size_t aaa; double aaaa;, 582)\
    X(CISF, char a; int aa; size_t aaa; float aaaa;, 583)\
    X(CISI, char a; int aa; size_t aaa; int aaaa;, 584)\
    X(CISL, char a; int aa; size_t aaa; long aaaa;, 585)\
    X(CISS, char a; int aa; size_t aaa; size_t aaaa;, 586)\
    X(CISU, char a; int aa; size_t aaa; uint64_t aaaa;, 587)\
    X(CIUC, char a; int aa; uint64_t aaa; char aaaa;, 588)\
    X(CIUD, char a; int aa; uint64_t aaa; double aaaa;, 589)\
    X(CIUF, char a; int aa; uint64_t aaa; float aaaa;, 590)\
    X(CIUI, char a; int aa; uint64_t aaa; int aaaa;, 591)\
    X(CIUL, char a; int aa; uint64_t aaa; long aaaa;, 592)\
    X(CIUS, char a; int aa; uint64_t aaa; size_t aaaa;, 593)\
    X(CIUU, char a; int aa; uint64_t aaa; uint64_t aaaa;, 594)\
    X(CLCC, char a; long aa; char aaa; char aaaa;, 595)\
    X(CLCD, char a; long aa; char aaa; double aaaa;, 596)\
    X(CLCF, char a; long aa; char aaa; float aaaa;, 597)\
    X(CLCI, char a; long aa; char aaa; int aaaa;, 598)\
    X(CLCL, char a; long aa; char aaa; long aaaa;, 599)\
    X(CLCS, char a; long aa; char aaa; size_t aaaa;, 600)\
    X(CLCU, char a; long aa; char aaa; uint64_t aaaa;, 601)\
    X(CLDC, char a; long aa; double aaa; char aaaa;, 602)\
    X(CLDD, char a; long aa; double aaa; double aaaa;, 603)\
    X(CLDF, char a; long aa; double aaa; float aaaa;, 604)\
    X(CLDI, char a; long aa; double aaa; int aaaa;, 605)\
    X(CLDL, char a; long aa; double aaa; long aaaa;, 606)\
    X(CLDS, char a; long aa; double aaa; size_t aaaa;, 607)\
    X(CLDU, char a; long aa; double aaa; uint64_t aaaa;, 608)\
    X(CLFC, char a; long aa; float aaa; char aaaa;, 609)\
    X(CLFD, char a; long aa; float aaa; double aaaa;, 610)\
    X(CLFF, char a; long aa; float aaa; float aaaa;, 611)\
    X(CLFI, char a; long aa; float aaa; int aaaa;, 612)\
    X(CLFL, char a; long aa; float aaa; long aaaa;, 613)\
    X(CLFS, char a; long aa; float aaa; size_t aaaa;, 614)\
    X(CLFU, char a; long aa; float aaa; uint64_t aaaa;, 615)\
    X(CLIC, char a; long aa; int aaa; char aaaa;, 616)\
    X(CLID, char a; long aa; int aaa; double aaaa;, 617)\
    X(CLIF, char a; long aa; int aaa; float aaaa;, 618)\
    X(CLII, char a; long aa; int aaa; int aaaa;, 619)\
    X(CLIL, char a; long aa; int aaa; long aaaa;, 620)\
    X(CLIS, char a; long aa; int aaa; size_t aaaa;, 621)\
    X(CLIU, char a; long aa; int aaa; uint64_t aaaa;, 622)\
    X(CLLC, char a; long aa; long aaa; char aaaa;, 623)\
    X(CLLD, char a; long aa; long aaa; double aaaa;, 624)\
    X(CLLF, char a; long aa; long aaa; float aaaa;, 625)\
    X(CLLI, char a; long aa; long aaa; int aaaa;, 626)\
    X(CLLL, char a; long aa; long aaa; long aaaa;, 627)\
    X(CLLS, char a; long aa; long aaa; size_t aaaa;, 628)\
    X(CLLU, char a; long aa; long aaa; uint64_t aaaa;, 629)\
    X(CLSC, char a; long aa; size_t aaa; char aaaa;, 630)\
    X(CLSD, char a; long aa; size_t aaa; double aaaa;, 631)\
    X(CLSF, char a; long aa; size_t aaa; float aaaa;, 632)\
    X(CLSI, char a; long aa; size_t aaa; int aaaa;, 633)\
    X(CLSL, char a; long aa; size_t aaa; long aaaa;, 634)\
    X(CLSS, char a; long aa; size_t aaa; size_t aaaa;, 635)\
    X(CLSU, char a; long aa; size_t aaa; uint64_t aaaa;, 636)\
    X(CLUC, char a; long aa; uint64_t aaa; char aaaa;, 637)\
    X(CLUD, char a; long aa; uint64_t aaa; double aaaa;, 638)\
    X(CLUF, char a; long aa; uint64_t aaa; float aaaa;, 639)\
    X(CLUI, char a; long aa; uint64_t aaa; int aaaa;, 640)\
    X(CLUL, char a; long aa; uint64_t aaa; long aaaa;, 641)\
    X(CLUS, char a; long aa; uint64_t aaa; size_t aaaa;, 642)\
    X(CLUU, char a; long aa; uint64_t aaa; uint64_t aaaa;, 643)\
    X(CSCC, char a; size_t aa; char aaa; char aaaa;, 644)\
    X(CSCD, char a; size_t aa; char aaa; double aaaa;, 645)\
    X(CSCF, char a; size_t aa; char aaa; float aaaa;, 646)\
    X(CSCI, char a; size_t aa; char aaa; int aaaa;, 647)\
    X(CSCL, char a; size_t aa; char aaa; long aaaa;, 648)\
    X(CSCS, char a; size_t aa; char aaa; size_t aaaa;, 649)\
    X(CSCU, char a; size_t aa; char aaa; uint64_t aaaa;, 650)\
    X(CSDC, char a; size_t aa; double aaa; char aaaa;, 651)\
    X(CSDD, char a; size_t aa; double aaa; double aaaa;, 652)\
    X(CSDF, char a; size_t aa; double aaa; float aaaa;, 653)\
    X(CSDI, char a; size_t aa; double aaa; int aaaa;, 654)\
    X(CSDL, char a; size_t aa; double aaa; long aaaa;, 655)\
    X(CSDS, char a; size_t aa; double aaa; size_t aaaa;, 656)\
    X(CSDU, char a; size_t aa; double aaa; uint64_t aaaa;, 657)\
    X(CSFC, char a; size_t aa; float aaa; char aaaa;, 658)\
    X(CSFD, char a; size_t aa; float aaa; double aaaa;, 659)\
    X(CSFF, char a; size_t aa; float aaa; float aaaa;, 660)\
    X(CSFI, char a; size_t aa; float aaa; int aaaa;, 661)\
    X(CSFL, char a; size_t aa; float aaa; long aaaa;, 662)\
    X(CSFS, char a; size_t aa; float aaa; size_t aaaa;, 663)\
    X(CSFU, char a; size_t aa; float aaa; uint64_t aaaa;, 664)\
    X(CSIC, char a; size_t aa; int aaa; char aaaa;, 665)\
    X(CSID, char a; size_t aa; int aaa; double aaaa;, 666)\
    X(CSIF, char a; size_t aa; int aaa; float aaaa;, 667)\
    X(CSII, char a; size_t aa; int aaa; int aaaa;, 668)\
    X(CSIL, char a; size_t aa; int aaa; long aaaa;, 669)\
    X(CSIS, char a; size_t aa; int aaa; size_t aaaa;, 670)\
    X(CSIU, char a; size_t aa; int aaa; uint64_t aaaa;, 671)\
    X(CSLC, char a; size_t aa; long aaa; char aaaa;, 672)\
    X(CSLD, char a; size_t aa; long aaa; double aaaa;, 673)\
    X(CSLF, char a; size_t aa; long aaa; float aaaa;, 674)\
    X(CSLI, char a; size_t aa; long aaa; int aaaa;, 675)\
    X(CSLL, char a; size_t aa; long aaa; long aaaa;, 676)\
    X(CSLS, char a; size_t aa; long aaa; size_t aaaa;, 677)\
    X(CSLU, char a; size_t aa; long aaa; uint64_t aaaa;, 678)\
    X(CSSC, char a; size_t aa; size_t aaa; char aaaa;, 679)\
    X(CSSD, char a; size_t aa; size_t aaa; double aaaa;, 680)\
    X(CSSF, char a; size_t aa; size_t aaa; float aaaa;, 681)\
    X(CSSI, char a; size_t aa; size_t aaa; int aaaa;, 682)\
    X(CSSL, char a; size_t aa; size_t aaa; long aaaa;, 683)\
    X(CSSS, char a; size_t aa; size_t aaa; size_t aaaa;, 684)\
    X(CSSU, char a; size_t aa; size_t aaa; uint64_t aaaa;, 685)\
    X(CSUC, char a; size_t aa; uint64_t aaa; char aaaa;, 686)\
    X(CSUD, char a; size_t aa; uint64_t aaa; double aaaa;, 687)\
    X(CSUF, char a; size_t aa; uint64_t aaa; float aaaa;, 688)\
    X(CSUI, char a; size_t aa; uint64_t aaa; int aaaa;, 689)\
    X(CSUL, char a; size_t aa; uint64_t aaa; long aaaa;, 690)\
    X(CSUS, char a; size_t aa; uint64_t aaa; size_t aaaa;, 691)\
    X(CSUU, char a; size_t aa; uint64_t aaa; uint64_t aaaa;, 692)\
    X(CUCC, char a; uint64_t aa; char aaa; char aaaa;, 693)\
    X(CUCD, char a; uint64_t aa; char aaa; double aaaa;, 694)\
    X(CUCF, char a; uint64_t aa; char aaa; float aaaa;, 695)\
    X(CUCI, char a; uint64_t aa; char aaa; int aaaa;, 696)\
    X(CUCL, char a; uint64_t aa; char aaa; long aaaa;, 697)\
    X(CUCS, char a; uint64_t aa; char aaa; size_t aaaa;, 698)\
    X(CUCU, char a; uint64_t aa; char aaa; uint64_t aaaa;, 699)\
    X(CUDC, char a; uint64_t aa; double aaa; char aaaa;, 700)\
    X(CUDD, char a; uint64_t aa; double aaa; double aaaa;, 701)\
    X(CUDF, char a; uint64_t aa; double aaa; float aaaa;, 702)\
    X(CUDI, char a; uint64_t aa; double aaa; int aaaa;, 703)\
    X(CUDL, char a; uint64_t aa; double aaa; long aaaa;, 704)\
    X(CUDS, char a; uint64_t aa; double aaa; size_t aaaa;, 705)\
    X(CUDU, char a; uint64_t aa; double aaa; uint64_t aaaa;, 706)\
    X(CUFC, char a; uint64_t aa; float aaa; char aaaa;, 707)\
    X(CUFD, char a; uint64_t aa; float aaa; double aaaa;, 708)\
    X(CUFF, char a; uint64_t aa; float aaa; float aaaa;, 709)\
    X(CUFI, char a; uint64_t aa; float aaa; int aaaa;, 710)\
    X(CUFL, char a; uint64_t aa; float aaa; long aaaa;, 711)\
    X(CUFS, char a; uint64_t aa; float aaa; size_t aaaa;, 712)\
    X(CUFU, char a; uint64_t aa; float aaa; uint64_t aaaa;, 713)\
    X(CUIC, char a; uint64_t aa; int aaa; char aaaa;, 714)\
    X(CUID, char a; uint64_t aa; int aaa; double aaaa;, 715)\
    X(CUIF, char a; uint64_t aa; int aaa; float aaaa;, 716)\
    X(CUII, char a; uint64_t aa; int aaa; int aaaa;, 717)\
    X(CUIL, char a; uint64_t aa; int aaa; long aaaa;, 718)\
    X(CUIS, char a; uint64_t aa; int aaa; size_t aaaa;, 719)\
    X(CUIU, char a; uint64_t aa; int aaa; uint64_t aaaa;, 720)\
    X(CULC, char a; uint64_t aa; long aaa; char aaaa;, 721)\
    X(CULD, char a; uint64_t aa; long aaa; double aaaa;, 722)\
    X(CULF, char a; uint64_t aa; long aaa; float aaaa;, 723)\
    X(CULI, char a; uint64_t aa; long aaa; int aaaa;, 724)\
    X(CULL, char a; uint64_t aa; long aaa; long aaaa;, 725)\
    X(CULS, char a; uint64_t aa; long aaa; size_t aaaa;, 726)\
    X(CULU, char a; uint64_t aa; long aaa; uint64_t aaaa;, 727)\
    X(CUSC, char a; uint64_t aa; size_t aaa; char aaaa;, 728)\
    X(CUSD, char a; uint64_t aa; size_t aaa; double aaaa;, 729)\
    X(CUSF, char a; uint64_t aa; size_t aaa; float aaaa;, 730)\
    X(CUSI, char a; uint64_t aa; size_t aaa; int aaaa;, 731)\
    X(CUSL, char a; uint64_t aa; size_t aaa; long aaaa;, 732)\
    X(CUSS, char a; uint64_t aa; size_t aaa; size_t aaaa;, 733)\
    X(CUSU, char a; uint64_t aa; size_t aaa; uint64_t aaaa;, 734)\
    X(CUUC, char a; uint64_t aa; uint64_t aaa; char aaaa;, 735)\
    X(CUUD, char a; uint64_t aa; uint64_t aaa; double aaaa;, 736)\
    X(CUUF, char a; uint64_t aa; uint64_t aaa; float aaaa;, 737)\
    X(CUUI, char a; uint64_t aa; uint64_t aaa; int aaaa;, 738)\
    X(CUUL, char a; uint64_t aa; uint64_t aaa; long aaaa;, 739)\
    X(CUUS, char a; uint64_t aa; uint64_t aaa; size_t aaaa;, 740)\
    X(CUUU, char a; uint64_t aa; uint64_t aaa; uint64_t aaaa;, 741)\
    X(DCCC, double a; char aa; char aaa; char aaaa;, 742)\
    X(DCCD, double a; char aa; char aaa; double aaaa;, 743)\
    X(DCCF, double a; char aa; char aaa; float aaaa;, 744)\
    X(DCCI, double a; char aa; char aaa; int aaaa;, 745)\
    X(DCCL, double a; char aa; char aaa; long aaaa;, 746)\
    X(DCCS, double a; char aa; char aaa; size_t aaaa;, 747)\
    X(DCCU, double a; char aa; char aaa; uint64_t aaaa;, 748)\
    X(DCDC, double a; char aa; double aaa; char aaaa;, 749)\
    X(DCDD, double a; char aa; double aaa; double aaaa;, 750)\
    X(DCDF, double a; char aa; double aaa; float aaaa;, 751)\
    X(DCDI, double a; char aa; double aaa; int aaaa;, 752)\
    X(DCDL, double a; char aa; double aaa; long aaaa;, 753)\
    X(DCDS, double a; char aa; double aaa; size_t aaaa;, 754)\
    X(DCDU, double a; char aa; double aaa; uint64_t aaaa;, 755)\
    X(DCFC, double a; char aa; float aaa; char aaaa;, 756)\
    X(DCFD, double a; char aa; float aaa; double aaaa;, 757)\
    X(DCFF, double a; char aa; float aaa; float aaaa;, 758)\
    X(DCFI, double a; char aa; float aaa; int aaaa;, 759)\
    X(DCFL, double a; char aa; float aaa; long aaaa;, 760)\
    X(DCFS, double a; char aa; float aaa; size_t aaaa;, 761)\
    X(DCFU, double a; char aa; float aaa; uint64_t aaaa;, 762)\
    X(DCIC, double a; char aa; int aaa; char aaaa;, 763)\
    X(DCID, double a; char aa; int aaa; double aaaa;, 764)\
    X(DCIF, double a; char aa; int aaa; float aaaa;, 765)\
    X(DCII, double a; char aa; int aaa; int aaaa;, 766)\
    X(DCIL, double a; char aa; int aaa; long aaaa;, 767)\
    X(DCIS, double a; char aa; int aaa; size_t aaaa;, 768)\
    X(DCIU, double a; char aa; int aaa; uint64_t aaaa;, 769)\
    X(DCLC, double a; char aa; long aaa; char aaaa;, 770)\
    X(DCLD, double a; char aa; long aaa; double aaaa;, 771)\
    X(DCLF, double a; char aa; long aaa; float aaaa;, 772)\
    X(DCLI, double a; char aa; long aaa; int aaaa;, 773)\
    X(DCLL, double a; char aa; long aaa; long aaaa;, 774)\
    X(DCLS, double a; char aa; long aaa; size_t aaaa;, 775)\
    X(DCLU, double a; char aa; long aaa; uint64_t aaaa;, 776)\
    X(DCSC, double a; char aa; size_t aaa; char aaaa;, 777)\
    X(DCSD, double a; char aa; size_t aaa; double aaaa;, 778)\
    X(DCSF, double a; char aa; size_t aaa; float aaaa;, 779)\
    X(DCSI, double a; char aa; size_t aaa; int aaaa;, 780)\
    X(DCSL, double a; char aa; size_t aaa; long aaaa;, 781)\
    X(DCSS, double a; char aa; size_t aaa; size_t aaaa;, 782)\
    X(DCSU, double a; char aa; size_t aaa; uint64_t aaaa;, 783)\
    X(DCUC, double a; char aa; uint64_t aaa; char aaaa;, 784)\
    X(DCUD, double a; char aa; uint64_t aaa; double aaaa;, 785)\
    X(DCUF, double a; char aa; uint64_t aaa; float aaaa;, 786)\
    X(DCUI, double a; char aa; uint64_t aaa; int aaaa;, 787)\
    X(DCUL, double a; char aa; uint64_t aaa; long aaaa;, 788)\
    X(DCUS, double a; char aa; uint64_t aaa; size_t aaaa;, 789)\
    X(DCUU, double a; char aa; uint64_t aaa; uint64_t aaaa;, 790)\
    X(DDCC, double a; double aa; char aaa; char aaaa;, 791)\
    X(DDCD, double a; double aa; char aaa; double aaaa;, 792)\
    X(DDCF, double a; double aa; char aaa; float aaaa;, 793)\
    X(DDCI, double a; double aa; char aaa; int aaaa;, 794)\
    X(DDCL, double a; double aa; char aaa; long aaaa;, 795)\
    X(DDCS, double a; double aa; char aaa; size_t aaaa;, 796)\
    X(DDCU, double a; double aa; char aaa; uint64_t aaaa;, 797)\
    X(DDDC, double a; double aa; double aaa; char aaaa;, 798)\
    X(DDDD, double a; double aa; double aaa; double aaaa;, 799)
    
/**
 * xmacro for graph type definition
 */
#define XGENERATE_TYPES_GRAPH(X)

XGENERATE_TYPES_BTREE(MAKE_BTREE_HEADER)
XGENERATE_TYPES_GRAPH(MAKE_GRAPH_HEADER)

/**
 * Inserts value into a generated type btree based on the given btree type
 * @par btree pointer to generated btree structure
 * @par value inserted value
 * @par used_type used generated btree type
 */
int gen_btree_insert(void **btree, long value, uint64_t used_type);

/**
 * Creates complete graph of generated type
 * @par number_of_nodes size of created complete graph
 * @par used_type number of generated type
 * @returns pointer newly created complete graph of given type
 */
void *gen_graph_make_complete_graph(size_t number_of_nodes, uint64_t used_type);
#endif
