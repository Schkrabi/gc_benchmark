/**
 * @name graph.h
 * @author Mgr. Radomir Skrabal
 * This file contains definitions and prototypes for benchmarking implementation of graphs
 */
#ifndef GRAPH_H
#define GRAPH_H

#include "gc_shared.h"
#include <stdio.h>

/**
 * Froward declaration
 */
struct graph_edge;

/**
 * Graph node structure
 */
typedef struct graph_node 
{
    /**
     * Id of node in graph
     */
    //uint64_t id;
    /**
     * Value of node
     */
    int value;
    /**
     * Number of outgoing edges
     */
    //size_t edge_count;
    /**
     * Outgoing edges
     */
    struct graph_edge *edges;
    /**
     * Next node in graph
     */
    struct graph_node *next;
} graph_node_t;

#define __graph_node_t_scan_struct\
    case TYPE_GRAPH_NODE_T:\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(sizeof(int), graph_edge_t)\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(sizeof(int) + sizeof(graph_edge_t*), graph_node_t)\
        break;

/**
 * Graph directed edge structure
 */
typedef struct graph_edge
{
    /**
     * Value of this edge
     */
    int value;
    /**
     * Originating node of this directed edge
     */
    struct graph_node *from,
    /**
     * Pointed node of this directed edge
     */
        *to;
        
    /**
     * Next outgoing node on edge
     */
    struct graph_edge *next;
} graph_edge_t;

#define __graph_edge_t_scan_struct\
    case TYPE_GRAPH_EDGE_T:\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(sizeof(int), graph_node_t)\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(sizeof(int) + sizeof(graph_node_t*), graph_node_t)\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(sizeof(int) + 2*sizeof(graph_node_t*), graph_edge_t)\
        break;

/**
 * Graph structure
 */
typedef struct graph
{
    /**
     * Last used id for nodes
     */
    uint64_t next_id;
    /**
     * Number of nodes in graph
     */
    size_t node_count;
    /**
     * Nodes of this graph
     */
    struct graph_node *nodes;
} graph_t;

#define __graph_t_scan_struct\
    case TYPE_GRAPH_T:\
        __GC_CUSTOM_SCAN_STRUCT_TYPE(sizeof(uint64_t) + sizeof(size_t), graph_node_t)\
        break;
/**
 * Makes descriptor for the graph node structure
 * @par info type_info_t structure
 * @returns Always 0
 */
int graph_node_make_descriptor(type_info_t *info);
/**
 * Makes descriptor for the graph edge structure
 * @par info type_info_t structure
 * @returns Always 0
 */
int graph_edge_make_descriptor(type_info_t *info);
/**
 * Makes descriptor for the graph struture
 * @par info type_info_t structure
 * @returns Always 0
 */
int graph_make_descriptor(type_info_t *info);

/**
 * Makes desriptors for the graph, graph_node and graph_edge structures
 * @par graph_info type_info_t structure
 * @par node_info type_info_t structure
 * @par edge_info type_info_t structure
 * @returns Always 0
 */
int graph_framework_make_descriptors(type_info_t *graph_info, type_info_t *node_info, type_info_t *edge_info);

/**
 * Makes a new instance of empty graph
 * @returns pointer to newly allocated graph_t structure
 */ 
graph_t* make_empty_graph();
/**
 * Adds new node to graph
 * @par graph graph_t structure to which node is added
 * @returns pointer to newly allocated graph_node_t structure
 */
graph_node_t* graph_add_node(graph_t *graph);
/**
 * removes node and all edges from and to it from the graph
 * @par graph graph_t structure where node is located
 * @par node pointer to removed node
 * @returns 1 if node was removed, 0 if node was not found in graph
 */
int graph_remove_node(graph_t* graph, graph_node_t *node);

/**
 * Finds a node in a graph
 * @par graph searched graph_t struture
 * @par id id of the searched node
 * @returns pointer to graph_node_t structure or NULL if node was not found
 */
graph_node_t* graph_find_node(graph_t* graph, uint64_t id);
/**
 * Finds a first node in a graph with specified value
 * @par graph searched graph_t struture
 * @par value value of the searched node
 * @returns pointer to graph_node_t structure or NULL if node was not found
 */
graph_node_t* graph_find_node_by_value(graph_t* graph, int value);

/**
 * Adds edge to node
 * @par from graph_node_t structure from which the edge will originate
 * @par to graph_node_t structure towards which the edge will point
 * @returns pointer to newly allocated graph_edge_t structure
 */
graph_edge_t* graph_node_add_edge(graph_node_t* from, graph_node_t *to);
/**
 * Removes edge from node
 * @par from graph_node_t structure from which the edge originates
 * @par edge pointer to edge to be removed
 * @returns 1 if edge was removed, 0 if edge was not found
 */
int graph_node_remove_edge(graph_node_t *from, graph_edge_t *edge);
/**
 * Removes edge from node
 * @par from graph_node_t structure from which the edge originates
 * @par to graph_node_t structure to which the edge points
 * @returns 1 if edge was removed, 0 if edge was not found
 */
int graph_node_remove_edge_by_to_node(graph_node_t* from, graph_node_t* to);

/**
 * Add edge to graph
 * @par graph graph_t structure where the node is added
 * @par from graph_node_t structure from which the edge will originate
 * @par to graph_node_t structure towards which the edge will point
 * @returns Pointer towards newly allocated graph_edge_t structure or NULL if from or to nodes are not part of graph
 */
graph_edge_t* graph_add_edge(graph_t* graph, graph_node_t *from, graph_node_t *to);
/**
 * Removes edge from graph
 * @par graph graph_t structure where the edge is located
 * @par from graph_node_t structure from which the edge will originate
 * @par to graph_node_t structure towards which the edge will point
 * @returns 1 if edge was removed, 0 if edge was not found
 */
int graph_remove_edge(graph_t* graph, graph_node_t *from, graph_node_t *to);

/**
 * Finds edge
 * @par from graph_node_t structure from which the edge originates
 * @par to graph_node_t structure towards which the edge points
 * @returns pointer to graph_edge_t structure or NULL if edge is not found
 */
graph_edge_t* graph_node_find_edge(graph_node_t* from, graph_node_t *to);
/**
 * Finds edge
 * @par from graph_node_t structure from which the edge originates
 * @par value value of the searched edge
 * @returns pointer to graph_edge_t structure or NULL if edge is not found
 */
graph_edge_t* graph_node_find_edge_by_value(graph_node_t* from, int value);

/**
 * Creates a complete graph
 * @par number_of_nodes number of nodes the complete graph will contain
 * @returns pointer towards newly allocated graph_t structure
 */
graph_t* make_complete_graph(size_t number_of_nodes);

/**
 * Returns 1 if node belong to the graph
 * @par graph graph_t structure
 * @par node graph_node_t structure
 * @returns 1 if node is part of graph, 0 otherwise
 */
int is_graph_node(graph_t *graph, graph_node_t *node);
/**
 * Returns 1 if edge belong to the graph node
 * @par node graph_node_t structure
 * @par edge graph_edge_t structure
 * @returns 1 if edge is part of graph, 0 otherwise
 */
int is_node_edge(graph_node_t *node, graph_edge_t *edge);

/**
 * Tests the graph framewor
 * @returns 1 on success, 0 otherwise
 */
int test_graphs();
#endif
