#ifndef GENERATED_TYPES_H
#define GENERATED_TYPES_H

#include "gc_shared.h"
#include "benchmark.h"
#include <stdio.h>

#define GEN_BTREE_TYPES_NUM 400
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
    X(C, char a; char padding[31];, 0)\
    X(D, double a; char padding[24];, 1)\
    X(F, float a; char padding[28];, 2)\
    X(I, int a; char padding[28];, 3)\
    X(L, long a; char padding[24];, 4)\
    X(S, size_t a; char padding[24];, 5)\
    X(U, uint64_t a; char padding[24];, 6)\
    X(CC, char a; char aa; char padding[30];, 7)\
    X(CD, char a; double aa; char padding[23];, 8)\
    X(CF, char a; float aa; char padding[27];, 9)\
    X(CI, char a; int aa; char padding[27];, 10)\
    X(CL, char a; long aa; char padding[23];, 11)\
    X(CS, char a; size_t aa; char padding[23];, 12)\
    X(CU, char a; uint64_t aa; char padding[23];, 13)\
    X(DC, double a; char aa; char padding[23];, 14)\
    X(DD, double a; double aa; char padding[16];, 15)\
    X(DF, double a; float aa; char padding[20];, 16)\
    X(DI, double a; int aa; char padding[20];, 17)\
    X(DL, double a; long aa; char padding[16];, 18)\
    X(DS, double a; size_t aa; char padding[16];, 19)\
    X(DU, double a; uint64_t aa; char padding[16];, 20)\
    X(FC, float a; char aa; char padding[27];, 21)\
    X(FD, float a; double aa; char padding[20];, 22)\
    X(FF, float a; float aa; char padding[24];, 23)\
    X(FI, float a; int aa; char padding[24];, 24)\
    X(FL, float a; long aa; char padding[20];, 25)\
    X(FS, float a; size_t aa; char padding[20];, 26)\
    X(FU, float a; uint64_t aa; char padding[20];, 27)\
    X(IC, int a; char aa; char padding[27];, 28)\
    X(ID, int a; double aa; char padding[20];, 29)\
    X(IF, int a; float aa; char padding[24];, 30)\
    X(II, int a; int aa; char padding[24];, 31)\
    X(IL, int a; long aa; char padding[20];, 32)\
    X(IS, int a; size_t aa; char padding[20];, 33)\
    X(IU, int a; uint64_t aa; char padding[20];, 34)\
    X(LC, long a; char aa; char padding[23];, 35)\
    X(LD, long a; double aa; char padding[16];, 36)\
    X(LF, long a; float aa; char padding[20];, 37)\
    X(LI, long a; int aa; char padding[20];, 38)\
    X(LL, long a; long aa; char padding[16];, 39)\
    X(LS, long a; size_t aa; char padding[16];, 40)\
    X(LU, long a; uint64_t aa; char padding[16];, 41)\
    X(SC, size_t a; char aa; char padding[23];, 42)\
    X(SD, size_t a; double aa; char padding[16];, 43)\
    X(SF, size_t a; float aa; char padding[20];, 44)\
    X(SI, size_t a; int aa; char padding[20];, 45)\
    X(SL, size_t a; long aa; char padding[16];, 46)\
    X(SS, size_t a; size_t aa; char padding[16];, 47)\
    X(SU, size_t a; uint64_t aa; char padding[16];, 48)\
    X(UC, uint64_t a; char aa; char padding[23];, 49)\
    X(UD, uint64_t a; double aa; char padding[16];, 50)\
    X(UF, uint64_t a; float aa; char padding[20];, 51)\
    X(UI, uint64_t a; int aa; char padding[20];, 52)\
    X(UL, uint64_t a; long aa; char padding[16];, 53)\
    X(US, uint64_t a; size_t aa; char padding[16];, 54)\
    X(UU, uint64_t a; uint64_t aa; char padding[16];, 55)\
    X(CCC, char a; char aa; char aaa; char padding[29];, 56)\
    X(CCD, char a; char aa; double aaa; char padding[22];, 57)\
    X(CCF, char a; char aa; float aaa; char padding[26];, 58)\
    X(CCI, char a; char aa; int aaa; char padding[26];, 59)\
    X(CCL, char a; char aa; long aaa; char padding[22];, 60)\
    X(CCS, char a; char aa; size_t aaa; char padding[22];, 61)\
    X(CCU, char a; char aa; uint64_t aaa; char padding[22];, 62)\
    X(CDC, char a; double aa; char aaa; char padding[22];, 63)\
    X(CDD, char a; double aa; double aaa; char padding[15];, 64)\
    X(CDF, char a; double aa; float aaa; char padding[19];, 65)\
    X(CDI, char a; double aa; int aaa; char padding[19];, 66)\
    X(CDL, char a; double aa; long aaa; char padding[15];, 67)\
    X(CDS, char a; double aa; size_t aaa; char padding[15];, 68)\
    X(CDU, char a; double aa; uint64_t aaa; char padding[15];, 69)\
    X(CFC, char a; float aa; char aaa; char padding[26];, 70)\
    X(CFD, char a; float aa; double aaa; char padding[19];, 71)\
    X(CFF, char a; float aa; float aaa; char padding[23];, 72)\
    X(CFI, char a; float aa; int aaa; char padding[23];, 73)\
    X(CFL, char a; float aa; long aaa; char padding[19];, 74)\
    X(CFS, char a; float aa; size_t aaa; char padding[19];, 75)\
    X(CFU, char a; float aa; uint64_t aaa; char padding[19];, 76)\
    X(CIC, char a; int aa; char aaa; char padding[26];, 77)\
    X(CID, char a; int aa; double aaa; char padding[19];, 78)\
    X(CIF, char a; int aa; float aaa; char padding[23];, 79)\
    X(CII, char a; int aa; int aaa; char padding[23];, 80)\
    X(CIL, char a; int aa; long aaa; char padding[19];, 81)\
    X(CIS, char a; int aa; size_t aaa; char padding[19];, 82)\
    X(CIU, char a; int aa; uint64_t aaa; char padding[19];, 83)\
    X(CLC, char a; long aa; char aaa; char padding[22];, 84)\
    X(CLD, char a; long aa; double aaa; char padding[15];, 85)\
    X(CLF, char a; long aa; float aaa; char padding[19];, 86)\
    X(CLI, char a; long aa; int aaa; char padding[19];, 87)\
    X(CLL, char a; long aa; long aaa; char padding[15];, 88)\
    X(CLS, char a; long aa; size_t aaa; char padding[15];, 89)\
    X(CLU, char a; long aa; uint64_t aaa; char padding[15];, 90)\
    X(CSC, char a; size_t aa; char aaa; char padding[22];, 91)\
    X(CSD, char a; size_t aa; double aaa; char padding[15];, 92)\
    X(CSF, char a; size_t aa; float aaa; char padding[19];, 93)\
    X(CSI, char a; size_t aa; int aaa; char padding[19];, 94)\
    X(CSL, char a; size_t aa; long aaa; char padding[15];, 95)\
    X(CSS, char a; size_t aa; size_t aaa; char padding[15];, 96)\
    X(CSU, char a; size_t aa; uint64_t aaa; char padding[15];, 97)\
    X(CUC, char a; uint64_t aa; char aaa; char padding[22];, 98)\
    X(CUD, char a; uint64_t aa; double aaa; char padding[15];, 99)\
    X(CUF, char a; uint64_t aa; float aaa; char padding[19];, 100)\
    X(CUI, char a; uint64_t aa; int aaa; char padding[19];, 101)\
    X(CUL, char a; uint64_t aa; long aaa; char padding[15];, 102)\
    X(CUS, char a; uint64_t aa; size_t aaa; char padding[15];, 103)\
    X(CUU, char a; uint64_t aa; uint64_t aaa; char padding[15];, 104)\
    X(DCC, double a; char aa; char aaa; char padding[22];, 105)\
    X(DCD, double a; char aa; double aaa; char padding[15];, 106)\
    X(DCF, double a; char aa; float aaa; char padding[19];, 107)\
    X(DCI, double a; char aa; int aaa; char padding[19];, 108)\
    X(DCL, double a; char aa; long aaa; char padding[15];, 109)\
    X(DCS, double a; char aa; size_t aaa; char padding[15];, 110)\
    X(DCU, double a; char aa; uint64_t aaa; char padding[15];, 111)\
    X(DDC, double a; double aa; char aaa; char padding[15];, 112)\
    X(DDD, double a; double aa; double aaa; char padding[8];, 113)\
    X(DDF, double a; double aa; float aaa; char padding[12];, 114)\
    X(DDI, double a; double aa; int aaa; char padding[12];, 115)\
    X(DDL, double a; double aa; long aaa; char padding[8];, 116)\
    X(DDS, double a; double aa; size_t aaa; char padding[8];, 117)\
    X(DDU, double a; double aa; uint64_t aaa; char padding[8];, 118)\
    X(DFC, double a; float aa; char aaa; char padding[19];, 119)\
    X(DFD, double a; float aa; double aaa; char padding[12];, 120)\
    X(DFF, double a; float aa; float aaa; char padding[16];, 121)\
    X(DFI, double a; float aa; int aaa; char padding[16];, 122)\
    X(DFL, double a; float aa; long aaa; char padding[12];, 123)\
    X(DFS, double a; float aa; size_t aaa; char padding[12];, 124)\
    X(DFU, double a; float aa; uint64_t aaa; char padding[12];, 125)\
    X(DIC, double a; int aa; char aaa; char padding[19];, 126)\
    X(DID, double a; int aa; double aaa; char padding[12];, 127)\
    X(DIF, double a; int aa; float aaa; char padding[16];, 128)\
    X(DII, double a; int aa; int aaa; char padding[16];, 129)\
    X(DIL, double a; int aa; long aaa; char padding[12];, 130)\
    X(DIS, double a; int aa; size_t aaa; char padding[12];, 131)\
    X(DIU, double a; int aa; uint64_t aaa; char padding[12];, 132)\
    X(DLC, double a; long aa; char aaa; char padding[15];, 133)\
    X(DLD, double a; long aa; double aaa; char padding[8];, 134)\
    X(DLF, double a; long aa; float aaa; char padding[12];, 135)\
    X(DLI, double a; long aa; int aaa; char padding[12];, 136)\
    X(DLL, double a; long aa; long aaa; char padding[8];, 137)\
    X(DLS, double a; long aa; size_t aaa; char padding[8];, 138)\
    X(DLU, double a; long aa; uint64_t aaa; char padding[8];, 139)\
    X(DSC, double a; size_t aa; char aaa; char padding[15];, 140)\
    X(DSD, double a; size_t aa; double aaa; char padding[8];, 141)\
    X(DSF, double a; size_t aa; float aaa; char padding[12];, 142)\
    X(DSI, double a; size_t aa; int aaa; char padding[12];, 143)\
    X(DSL, double a; size_t aa; long aaa; char padding[8];, 144)\
    X(DSS, double a; size_t aa; size_t aaa; char padding[8];, 145)\
    X(DSU, double a; size_t aa; uint64_t aaa; char padding[8];, 146)\
    X(DUC, double a; uint64_t aa; char aaa; char padding[15];, 147)\
    X(DUD, double a; uint64_t aa; double aaa; char padding[8];, 148)\
    X(DUF, double a; uint64_t aa; float aaa; char padding[12];, 149)\
    X(DUI, double a; uint64_t aa; int aaa; char padding[12];, 150)\
    X(DUL, double a; uint64_t aa; long aaa; char padding[8];, 151)\
    X(DUS, double a; uint64_t aa; size_t aaa; char padding[8];, 152)\
    X(DUU, double a; uint64_t aa; uint64_t aaa; char padding[8];, 153)\
    X(FCC, float a; char aa; char aaa; char padding[26];, 154)\
    X(FCD, float a; char aa; double aaa; char padding[19];, 155)\
    X(FCF, float a; char aa; float aaa; char padding[23];, 156)\
    X(FCI, float a; char aa; int aaa; char padding[23];, 157)\
    X(FCL, float a; char aa; long aaa; char padding[19];, 158)\
    X(FCS, float a; char aa; size_t aaa; char padding[19];, 159)\
    X(FCU, float a; char aa; uint64_t aaa; char padding[19];, 160)\
    X(FDC, float a; double aa; char aaa; char padding[19];, 161)\
    X(FDD, float a; double aa; double aaa; char padding[12];, 162)\
    X(FDF, float a; double aa; float aaa; char padding[16];, 163)\
    X(FDI, float a; double aa; int aaa; char padding[16];, 164)\
    X(FDL, float a; double aa; long aaa; char padding[12];, 165)\
    X(FDS, float a; double aa; size_t aaa; char padding[12];, 166)\
    X(FDU, float a; double aa; uint64_t aaa; char padding[12];, 167)\
    X(FFC, float a; float aa; char aaa; char padding[23];, 168)\
    X(FFD, float a; float aa; double aaa; char padding[16];, 169)\
    X(FFF, float a; float aa; float aaa; char padding[20];, 170)\
    X(FFI, float a; float aa; int aaa; char padding[20];, 171)\
    X(FFL, float a; float aa; long aaa; char padding[16];, 172)\
    X(FFS, float a; float aa; size_t aaa; char padding[16];, 173)\
    X(FFU, float a; float aa; uint64_t aaa; char padding[16];, 174)\
    X(FIC, float a; int aa; char aaa; char padding[23];, 175)\
    X(FID, float a; int aa; double aaa; char padding[16];, 176)\
    X(FIF, float a; int aa; float aaa; char padding[20];, 177)\
    X(FII, float a; int aa; int aaa; char padding[20];, 178)\
    X(FIL, float a; int aa; long aaa; char padding[16];, 179)\
    X(FIS, float a; int aa; size_t aaa; char padding[16];, 180)\
    X(FIU, float a; int aa; uint64_t aaa; char padding[16];, 181)\
    X(FLC, float a; long aa; char aaa; char padding[19];, 182)\
    X(FLD, float a; long aa; double aaa; char padding[12];, 183)\
    X(FLF, float a; long aa; float aaa; char padding[16];, 184)\
    X(FLI, float a; long aa; int aaa; char padding[16];, 185)\
    X(FLL, float a; long aa; long aaa; char padding[12];, 186)\
    X(FLS, float a; long aa; size_t aaa; char padding[12];, 187)\
    X(FLU, float a; long aa; uint64_t aaa; char padding[12];, 188)\
    X(FSC, float a; size_t aa; char aaa; char padding[19];, 189)\
    X(FSD, float a; size_t aa; double aaa; char padding[12];, 190)\
    X(FSF, float a; size_t aa; float aaa; char padding[16];, 191)\
    X(FSI, float a; size_t aa; int aaa; char padding[16];, 192)\
    X(FSL, float a; size_t aa; long aaa; char padding[12];, 193)\
    X(FSS, float a; size_t aa; size_t aaa; char padding[12];, 194)\
    X(FSU, float a; size_t aa; uint64_t aaa; char padding[12];, 195)\
    X(FUC, float a; uint64_t aa; char aaa; char padding[19];, 196)\
    X(FUD, float a; uint64_t aa; double aaa; char padding[12];, 197)\
    X(FUF, float a; uint64_t aa; float aaa; char padding[16];, 198)\
    X(FUI, float a; uint64_t aa; int aaa; char padding[16];, 199)\
    X(FUL, float a; uint64_t aa; long aaa; char padding[12];, 200)\
    X(FUS, float a; uint64_t aa; size_t aaa; char padding[12];, 201)\
    X(FUU, float a; uint64_t aa; uint64_t aaa; char padding[12];, 202)\
    X(ICC, int a; char aa; char aaa; char padding[26];, 203)\
    X(ICD, int a; char aa; double aaa; char padding[19];, 204)\
    X(ICF, int a; char aa; float aaa; char padding[23];, 205)\
    X(ICI, int a; char aa; int aaa; char padding[23];, 206)\
    X(ICL, int a; char aa; long aaa; char padding[19];, 207)\
    X(ICS, int a; char aa; size_t aaa; char padding[19];, 208)\
    X(ICU, int a; char aa; uint64_t aaa; char padding[19];, 209)\
    X(IDC, int a; double aa; char aaa; char padding[19];, 210)\
    X(IDD, int a; double aa; double aaa; char padding[12];, 211)\
    X(IDF, int a; double aa; float aaa; char padding[16];, 212)\
    X(IDI, int a; double aa; int aaa; char padding[16];, 213)\
    X(IDL, int a; double aa; long aaa; char padding[12];, 214)\
    X(IDS, int a; double aa; size_t aaa; char padding[12];, 215)\
    X(IDU, int a; double aa; uint64_t aaa; char padding[12];, 216)\
    X(IFC, int a; float aa; char aaa; char padding[23];, 217)\
    X(IFD, int a; float aa; double aaa; char padding[16];, 218)\
    X(IFF, int a; float aa; float aaa; char padding[20];, 219)\
    X(IFI, int a; float aa; int aaa; char padding[20];, 220)\
    X(IFL, int a; float aa; long aaa; char padding[16];, 221)\
    X(IFS, int a; float aa; size_t aaa; char padding[16];, 222)\
    X(IFU, int a; float aa; uint64_t aaa; char padding[16];, 223)\
    X(IIC, int a; int aa; char aaa; char padding[23];, 224)\
    X(IID, int a; int aa; double aaa; char padding[16];, 225)\
    X(IIF, int a; int aa; float aaa; char padding[20];, 226)\
    X(III, int a; int aa; int aaa; char padding[20];, 227)\
    X(IIL, int a; int aa; long aaa; char padding[16];, 228)\
    X(IIS, int a; int aa; size_t aaa; char padding[16];, 229)\
    X(IIU, int a; int aa; uint64_t aaa; char padding[16];, 230)\
    X(ILC, int a; long aa; char aaa; char padding[19];, 231)\
    X(ILD, int a; long aa; double aaa; char padding[12];, 232)\
    X(ILF, int a; long aa; float aaa; char padding[16];, 233)\
    X(ILI, int a; long aa; int aaa; char padding[16];, 234)\
    X(ILL, int a; long aa; long aaa; char padding[12];, 235)\
    X(ILS, int a; long aa; size_t aaa; char padding[12];, 236)\
    X(ILU, int a; long aa; uint64_t aaa; char padding[12];, 237)\
    X(ISC, int a; size_t aa; char aaa; char padding[19];, 238)\
    X(ISD, int a; size_t aa; double aaa; char padding[12];, 239)\
    X(ISF, int a; size_t aa; float aaa; char padding[16];, 240)\
    X(ISI, int a; size_t aa; int aaa; char padding[16];, 241)\
    X(ISL, int a; size_t aa; long aaa; char padding[12];, 242)\
    X(ISS, int a; size_t aa; size_t aaa; char padding[12];, 243)\
    X(ISU, int a; size_t aa; uint64_t aaa; char padding[12];, 244)\
    X(IUC, int a; uint64_t aa; char aaa; char padding[19];, 245)\
    X(IUD, int a; uint64_t aa; double aaa; char padding[12];, 246)\
    X(IUF, int a; uint64_t aa; float aaa; char padding[16];, 247)\
    X(IUI, int a; uint64_t aa; int aaa; char padding[16];, 248)\
    X(IUL, int a; uint64_t aa; long aaa; char padding[12];, 249)\
    X(IUS, int a; uint64_t aa; size_t aaa; char padding[12];, 250)\
    X(IUU, int a; uint64_t aa; uint64_t aaa; char padding[12];, 251)\
    X(LCC, long a; char aa; char aaa; char padding[22];, 252)\
    X(LCD, long a; char aa; double aaa; char padding[15];, 253)\
    X(LCF, long a; char aa; float aaa; char padding[19];, 254)\
    X(LCI, long a; char aa; int aaa; char padding[19];, 255)\
    X(LCL, long a; char aa; long aaa; char padding[15];, 256)\
    X(LCS, long a; char aa; size_t aaa; char padding[15];, 257)\
    X(LCU, long a; char aa; uint64_t aaa; char padding[15];, 258)\
    X(LDC, long a; double aa; char aaa; char padding[15];, 259)\
    X(LDD, long a; double aa; double aaa; char padding[8];, 260)\
    X(LDF, long a; double aa; float aaa; char padding[12];, 261)\
    X(LDI, long a; double aa; int aaa; char padding[12];, 262)\
    X(LDL, long a; double aa; long aaa; char padding[8];, 263)\
    X(LDS, long a; double aa; size_t aaa; char padding[8];, 264)\
    X(LDU, long a; double aa; uint64_t aaa; char padding[8];, 265)\
    X(LFC, long a; float aa; char aaa; char padding[19];, 266)\
    X(LFD, long a; float aa; double aaa; char padding[12];, 267)\
    X(LFF, long a; float aa; float aaa; char padding[16];, 268)\
    X(LFI, long a; float aa; int aaa; char padding[16];, 269)\
    X(LFL, long a; float aa; long aaa; char padding[12];, 270)\
    X(LFS, long a; float aa; size_t aaa; char padding[12];, 271)\
    X(LFU, long a; float aa; uint64_t aaa; char padding[12];, 272)\
    X(LIC, long a; int aa; char aaa; char padding[19];, 273)\
    X(LID, long a; int aa; double aaa; char padding[12];, 274)\
    X(LIF, long a; int aa; float aaa; char padding[16];, 275)\
    X(LII, long a; int aa; int aaa; char padding[16];, 276)\
    X(LIL, long a; int aa; long aaa; char padding[12];, 277)\
    X(LIS, long a; int aa; size_t aaa; char padding[12];, 278)\
    X(LIU, long a; int aa; uint64_t aaa; char padding[12];, 279)\
    X(LLC, long a; long aa; char aaa; char padding[15];, 280)\
    X(LLD, long a; long aa; double aaa; char padding[8];, 281)\
    X(LLF, long a; long aa; float aaa; char padding[12];, 282)\
    X(LLI, long a; long aa; int aaa; char padding[12];, 283)\
    X(LLL, long a; long aa; long aaa; char padding[8];, 284)\
    X(LLS, long a; long aa; size_t aaa; char padding[8];, 285)\
    X(LLU, long a; long aa; uint64_t aaa; char padding[8];, 286)\
    X(LSC, long a; size_t aa; char aaa; char padding[15];, 287)\
    X(LSD, long a; size_t aa; double aaa; char padding[8];, 288)\
    X(LSF, long a; size_t aa; float aaa; char padding[12];, 289)\
    X(LSI, long a; size_t aa; int aaa; char padding[12];, 290)\
    X(LSL, long a; size_t aa; long aaa; char padding[8];, 291)\
    X(LSS, long a; size_t aa; size_t aaa; char padding[8];, 292)\
    X(LSU, long a; size_t aa; uint64_t aaa; char padding[8];, 293)\
    X(LUC, long a; uint64_t aa; char aaa; char padding[15];, 294)\
    X(LUD, long a; uint64_t aa; double aaa; char padding[8];, 295)\
    X(LUF, long a; uint64_t aa; float aaa; char padding[12];, 296)\
    X(LUI, long a; uint64_t aa; int aaa; char padding[12];, 297)\
    X(LUL, long a; uint64_t aa; long aaa; char padding[8];, 298)\
    X(LUS, long a; uint64_t aa; size_t aaa; char padding[8];, 299)\
    X(LUU, long a; uint64_t aa; uint64_t aaa; char padding[8];, 300)\
    X(SCC, size_t a; char aa; char aaa; char padding[22];, 301)\
    X(SCD, size_t a; char aa; double aaa; char padding[15];, 302)\
    X(SCF, size_t a; char aa; float aaa; char padding[19];, 303)\
    X(SCI, size_t a; char aa; int aaa; char padding[19];, 304)\
    X(SCL, size_t a; char aa; long aaa; char padding[15];, 305)\
    X(SCS, size_t a; char aa; size_t aaa; char padding[15];, 306)\
    X(SCU, size_t a; char aa; uint64_t aaa; char padding[15];, 307)\
    X(SDC, size_t a; double aa; char aaa; char padding[15];, 308)\
    X(SDD, size_t a; double aa; double aaa; char padding[8];, 309)\
    X(SDF, size_t a; double aa; float aaa; char padding[12];, 310)\
    X(SDI, size_t a; double aa; int aaa; char padding[12];, 311)\
    X(SDL, size_t a; double aa; long aaa; char padding[8];, 312)\
    X(SDS, size_t a; double aa; size_t aaa; char padding[8];, 313)\
    X(SDU, size_t a; double aa; uint64_t aaa; char padding[8];, 314)\
    X(SFC, size_t a; float aa; char aaa; char padding[19];, 315)\
    X(SFD, size_t a; float aa; double aaa; char padding[12];, 316)\
    X(SFF, size_t a; float aa; float aaa; char padding[16];, 317)\
    X(SFI, size_t a; float aa; int aaa; char padding[16];, 318)\
    X(SFL, size_t a; float aa; long aaa; char padding[12];, 319)\
    X(SFS, size_t a; float aa; size_t aaa; char padding[12];, 320)\
    X(SFU, size_t a; float aa; uint64_t aaa; char padding[12];, 321)\
    X(SIC, size_t a; int aa; char aaa; char padding[19];, 322)\
    X(SID, size_t a; int aa; double aaa; char padding[12];, 323)\
    X(SIF, size_t a; int aa; float aaa; char padding[16];, 324)\
    X(SII, size_t a; int aa; int aaa; char padding[16];, 325)\
    X(SIL, size_t a; int aa; long aaa; char padding[12];, 326)\
    X(SIS, size_t a; int aa; size_t aaa; char padding[12];, 327)\
    X(SIU, size_t a; int aa; uint64_t aaa; char padding[12];, 328)\
    X(SLC, size_t a; long aa; char aaa; char padding[15];, 329)\
    X(SLD, size_t a; long aa; double aaa; char padding[8];, 330)\
    X(SLF, size_t a; long aa; float aaa; char padding[12];, 331)\
    X(SLI, size_t a; long aa; int aaa; char padding[12];, 332)\
    X(SLL, size_t a; long aa; long aaa; char padding[8];, 333)\
    X(SLS, size_t a; long aa; size_t aaa; char padding[8];, 334)\
    X(SLU, size_t a; long aa; uint64_t aaa; char padding[8];, 335)\
    X(SSC, size_t a; size_t aa; char aaa; char padding[15];, 336)\
    X(SSD, size_t a; size_t aa; double aaa; char padding[8];, 337)\
    X(SSF, size_t a; size_t aa; float aaa; char padding[12];, 338)\
    X(SSI, size_t a; size_t aa; int aaa; char padding[12];, 339)\
    X(SSL, size_t a; size_t aa; long aaa; char padding[8];, 340)\
    X(SSS, size_t a; size_t aa; size_t aaa; char padding[8];, 341)\
    X(SSU, size_t a; size_t aa; uint64_t aaa; char padding[8];, 342)\
    X(SUC, size_t a; uint64_t aa; char aaa; char padding[15];, 343)\
    X(SUD, size_t a; uint64_t aa; double aaa; char padding[8];, 344)\
    X(SUF, size_t a; uint64_t aa; float aaa; char padding[12];, 345)\
    X(SUI, size_t a; uint64_t aa; int aaa; char padding[12];, 346)\
    X(SUL, size_t a; uint64_t aa; long aaa; char padding[8];, 347)\
    X(SUS, size_t a; uint64_t aa; size_t aaa; char padding[8];, 348)\
    X(SUU, size_t a; uint64_t aa; uint64_t aaa; char padding[8];, 349)\
    X(UCC, uint64_t a; char aa; char aaa; char padding[22];, 350)\
    X(UCD, uint64_t a; char aa; double aaa; char padding[15];, 351)\
    X(UCF, uint64_t a; char aa; float aaa; char padding[19];, 352)\
    X(UCI, uint64_t a; char aa; int aaa; char padding[19];, 353)\
    X(UCL, uint64_t a; char aa; long aaa; char padding[15];, 354)\
    X(UCS, uint64_t a; char aa; size_t aaa; char padding[15];, 355)\
    X(UCU, uint64_t a; char aa; uint64_t aaa; char padding[15];, 356)\
    X(UDC, uint64_t a; double aa; char aaa; char padding[15];, 357)\
    X(UDD, uint64_t a; double aa; double aaa; char padding[8];, 358)\
    X(UDF, uint64_t a; double aa; float aaa; char padding[12];, 359)\
    X(UDI, uint64_t a; double aa; int aaa; char padding[12];, 360)\
    X(UDL, uint64_t a; double aa; long aaa; char padding[8];, 361)\
    X(UDS, uint64_t a; double aa; size_t aaa; char padding[8];, 362)\
    X(UDU, uint64_t a; double aa; uint64_t aaa; char padding[8];, 363)\
    X(UFC, uint64_t a; float aa; char aaa; char padding[19];, 364)\
    X(UFD, uint64_t a; float aa; double aaa; char padding[12];, 365)\
    X(UFF, uint64_t a; float aa; float aaa; char padding[16];, 366)\
    X(UFI, uint64_t a; float aa; int aaa; char padding[16];, 367)\
    X(UFL, uint64_t a; float aa; long aaa; char padding[12];, 368)\
    X(UFS, uint64_t a; float aa; size_t aaa; char padding[12];, 369)\
    X(UFU, uint64_t a; float aa; uint64_t aaa; char padding[12];, 370)\
    X(UIC, uint64_t a; int aa; char aaa; char padding[19];, 371)\
    X(UID, uint64_t a; int aa; double aaa; char padding[12];, 372)\
    X(UIF, uint64_t a; int aa; float aaa; char padding[16];, 373)\
    X(UII, uint64_t a; int aa; int aaa; char padding[16];, 374)\
    X(UIL, uint64_t a; int aa; long aaa; char padding[12];, 375)\
    X(UIS, uint64_t a; int aa; size_t aaa; char padding[12];, 376)\
    X(UIU, uint64_t a; int aa; uint64_t aaa; char padding[12];, 377)\
    X(ULC, uint64_t a; long aa; char aaa; char padding[15];, 378)\
    X(ULD, uint64_t a; long aa; double aaa; char padding[8];, 379)\
    X(ULF, uint64_t a; long aa; float aaa; char padding[12];, 380)\
    X(ULI, uint64_t a; long aa; int aaa; char padding[12];, 381)\
    X(ULL, uint64_t a; long aa; long aaa; char padding[8];, 382)\
    X(ULS, uint64_t a; long aa; size_t aaa; char padding[8];, 383)\
    X(ULU, uint64_t a; long aa; uint64_t aaa; char padding[8];, 384)\
    X(USC, uint64_t a; size_t aa; char aaa; char padding[15];, 385)\
    X(USD, uint64_t a; size_t aa; double aaa; char padding[8];, 386)\
    X(USF, uint64_t a; size_t aa; float aaa; char padding[12];, 387)\
    X(USI, uint64_t a; size_t aa; int aaa; char padding[12];, 388)\
    X(USL, uint64_t a; size_t aa; long aaa; char padding[8];, 389)\
    X(USS, uint64_t a; size_t aa; size_t aaa; char padding[8];, 390)\
    X(USU, uint64_t a; size_t aa; uint64_t aaa; char padding[8];, 391)\
    X(UUC, uint64_t a; uint64_t aa; char aaa; char padding[15];, 392)\
    X(UUD, uint64_t a; uint64_t aa; double aaa; char padding[8];, 393)\
    X(UUF, uint64_t a; uint64_t aa; float aaa; char padding[12];, 394)\
    X(UUI, uint64_t a; uint64_t aa; int aaa; char padding[12];, 395)\
    X(UUL, uint64_t a; uint64_t aa; long aaa; char padding[8];, 396)\
    X(UUS, uint64_t a; uint64_t aa; size_t aaa; char padding[8];, 397)\
    X(UUU, uint64_t a; uint64_t aa; uint64_t aaa; char padding[8];, 398)\
    X(CCCC, char a; char aa; char aaa; char aaaa; char padding[28];, 399)
    
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
