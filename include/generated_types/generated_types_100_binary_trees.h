    #ifndef GENERATED_TYPES_H
#define GENERATED_TYPES_H

#include "gc_shared.h"
#include "benchmark.h"
#include <stdio.h>

#define GEN_BTREE_TYPES_NUM 100
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
        ptr_info_set_type(&info->references[0], GEN_BTREE_NUM(NUM));                                                            \
        info->references[1].offset = (uint64_t)((art_ptr_t)&measure.rchild - (art_ptr_t)&measure);                              \
        ptr_info_set_type(&info->references[1], GEN_BTREE_NUM(NUM));                                                            \
    }                                                                                                                           \
    int btree ## SUFFIX ## _insert(GEN_BTREE_TYPE(SUFFIX) **root, long value)                                                   \
    {                                                                                                                           \
        if(*root == NULL)                                                                                                       \
        {                                                                                                                       \
            *root = __gc_malloc(GEN_BTREE_NUM(NUM));                                                                            \
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
    X(CUD, char a; uint64_t aa; double aaa; char padding[15];, 99)


    
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
