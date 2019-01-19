#ifndef GENERATED_TYPES_H
#define GENERATED_TYPES_H

#include "gc_shared.h"
#include "benchmark.h"
#include <stdio.h>

#define GEN_BTREE_TYPES_NUM 0
#define GEN_GRAPH_TYPES_NUM 250

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
#define XGENERATE_TYPES_BTREE(X)
    
/**
 * xmacro for graph type definition
 */
#define XGENERATE_TYPES_GRAPH(X)\
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
    X(IUL, int a; uint64_t aa; long aaa;, 249)

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
