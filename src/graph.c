/**
 * @name graph.c
 * @author Mgr. Radomir Skrabal
 * This file contains implementations for benchmarking graphs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "garbage_collector.h"
#include "gc_util.h"
#include "gc_constants.h"

/**
 * Makes descriptor for the graph node structure
 * @par info type_info_t structure
 * @returns Always 0
 */
int graph_node_make_descriptor(type_info_t *info)
{
    graph_node_t measure;
    
    info->size = sizeof(graph_node_t);
    info->number_of_references = 2;
    info->references = (ptr_info_t*)malloc(2 * sizeof(ptr_info_t));
    info->references[0].offset = (uint64_t)((art_ptr_t)&measure.edges - (art_ptr_t)&measure);
    ptr_info_set_type(&info->references[0], TYPE_GRAPH_EDGE_T);
    info->references[1].offset = (uint64_t)((art_ptr_t)&measure.next - (art_ptr_t)&measure);
    ptr_info_set_type(&info->references[1], TYPE_GRAPH_NODE_T);
}
/**
 * Makes descriptor for the graph edge structure
 * @par info type_info_t structure
 * @returns Always 0
 */
int graph_edge_make_descriptor(type_info_t *info)
{
    graph_edge_t measure;
    
    info->size = sizeof(graph_edge_t);
    info->number_of_references = 2;
    info->references = (ptr_info_t*)malloc(3 * sizeof(ptr_info_t));
    info->references[0].offset = (uint64_t)((art_ptr_t)&measure.from - (art_ptr_t)&measure);
    ptr_info_set_type(&info->references[0], TYPE_GRAPH_NODE_T);
    info->references[1].offset = (uint64_t)((art_ptr_t)&measure.to - (art_ptr_t)&measure);
    ptr_info_set_type(&info->references[1], TYPE_GRAPH_NODE_T);
    info->references[2].offset = (uint64_t)((art_ptr_t)&measure.next - (art_ptr_t)&measure);
    ptr_info_set_type(&info->references[2], TYPE_GRAPH_EDGE_T);
}
/**
 * Makes descriptor for the graph struture
 * @par info type_info_t structure
 * @returns Always 0
 */
int graph_make_descriptor(type_info_t *info)
{
    graph_t measure;
    
    info->size = sizeof(graph_t);
    info->number_of_references = 1;
    info->references = (ptr_info_t*)malloc(2 * sizeof(ptr_info_t));
    info->references[0].offset = (uint64_t)((art_ptr_t)&measure.nodes - (art_ptr_t)&measure);
    ptr_info_set_type(&info->references[0], TYPE_GRAPH_NODE_T);
}

/**
 * Makes desriptors for the graph, graph_node and graph_edge structures
 * @par graph_info type_info_t structure
 * @par node_info type_info_t structure
 * @par edge_info type_info_t structure
 * @returns Always 0
 */
int graph_framework_make_descriptors(type_info_t *graph_info, type_info_t *node_info, type_info_t *edge_info)
{
    graph_node_make_descriptor(node_info);
    graph_edge_make_descriptor(edge_info);
    graph_make_descriptor(graph_info);
    return 0;
}

/**
 * Makes a new instance of empty graph
 * @returns pointer to newly allocated graph_t structure
 */ 
graph_t* make_empty_graph()
{
    graph_t *graph;
    
    graph = gc_malloc(graph_t);
    graph->next_id = 0;
    graph->node_count = 0;
    graph->nodes = NULL;
    
    return graph;
}

/**
 * Gets pointer to the last empty node pointer in graph
 * @par graph searched graph
 * @returns pointer to graph_node_t pointer
 */
graph_node_t **__graph_last_node(graph_t *graph)
{
    graph_node_t *node;
    if(graph->nodes == NULL)
    {
        return &graph->nodes;
    }
    
    node = graph->nodes;
    while(node->next != NULL)
    {
        node = node->next;
    }
    
    return &node->next;
}

/**
 * Gets pointer to the last empty edge pointer in node
 * @par node searched node
 * @returns pointer to graph_edge_t pointer
 */
graph_edge_t **__node_last_edge(graph_node_t *node)
{
    graph_edge_t *edge;
    if(node->edges == NULL)
    {
        return &node->edges;
    }
    
    edge = node->edges;
    while(edge->next != NULL)
    {
        edge = edge->next;
    }
    
    return &edge->next;
}

/**
 * Adds new node to graph
 * @par graph graph_t structure to which node is added
 * @returns pointer to newly allocated graph_node_t structure
 */
graph_node_t* graph_add_node(graph_t *graph)
{
    graph_node_t *node, **insetion_point;
    node = gc_malloc(graph_node_t);
    node->value = 0;
    node->edges = NULL;
    node->next = NULL;    
    
    insetion_point = __graph_last_node(graph);
    
    *insetion_point = node;
    
    return node;
}

/**
 * removes node and all edges from and to it from the graph
 * @par graph graph_t structure where node is located
 * @par node pointer to removed node
 * @returns 1 if node was removed, 0 if node was not found in graph
 */
int graph_remove_node(graph_t* graph, graph_node_t *node)
{
    graph_node_t *prev, *tmp;
    
    tmp = graph->nodes;
    do
    {
        graph_node_remove_edge_by_to_node(tmp, node);
        tmp = tmp->next;
    }while(tmp != NULL);
    
    if(graph->nodes == node)
    {
        graph->nodes = node->next;
        return 1;
    }
    prev = graph->nodes;
    while(prev->next != node)
    {
        if(prev->next = NULL)
        {
            return 0;
        }
        prev = prev->next;
    }
    
    prev->next = node->next;
    return 1; 
}

/**
 * Finds a first node in a graph with specified value
 * @par graph searched graph_t struture
 * @par value value of the searched node
 * @returns pointer to graph_node_t structure or NULL if node was not found
 */
graph_node_t* graph_find_node_by_value(graph_t* graph, int value)
{
    graph_node_t *node;
    node = graph->nodes;
    while(  node != NULL
        &&  node->value != value)
    {
        node = node->next;
    }
    return node;
}

/**
 * Adds edge to node
 * @par from graph_node_t structure from which the edge will originate
 * @par to graph_node_t structure towards which the edge will point
 * @returns pointer to newly allocated graph_edge_t structure
 */
graph_edge_t* graph_node_add_edge(graph_node_t* from, graph_node_t *to)
{
    graph_edge_t *edge, **point_of_insert;
    edge = gc_malloc(graph_edge_t);
    edge->from = from;
    edge->to = to;
    edge->value = 0;
    
    point_of_insert = __node_last_edge(from);
    *point_of_insert = edge;
    return edge;
}
/**
 * Removes edge from node
 * @par from graph_node_t structure from which the edge originates
 * @par edge pointer to edge to be removed
 * @returns 1 if edge was removed, 0 if edge was not found
 */
int graph_node_remove_edge(graph_node_t *from, graph_edge_t *edge)
{
    graph_edge_t *prev;
    
    if(edge == NULL)
    {
        return 0;
    }
    
    if(from->edges == edge)
    {
        from->edges = edge->next;
        return 1;
    }
    
    prev = from->edges;
    while(prev->next != edge)
    {
        if(prev->next == NULL)
        {
            return 0;
        }
        prev = prev->next;
    }
    prev->next = edge->next;
    return 1;
}
/**
 * Removes edge from node
 * @par from graph_node_t structure from which the edge originates
 * @par to graph_node_t structure to which the edge points
 * @returns 1 if edge was removed, 0 if edge was not found
 */
int graph_node_remove_edge_by_to_node(graph_node_t* from, graph_node_t* to)
{
    return graph_node_remove_edge(from, graph_node_find_edge(from, to));
}

/**
 * Add edge to graph
 * @par graph graph_t structure where the node is added
 * @par from graph_node_t structure from which the edge will originate
 * @par to graph_node_t structure towards which the edge will point
 * @returns Pointer towards newly allocated graph_edge_t structure or NULL if from or to nodes are not part of graph
 */
graph_edge_t* graph_add_edge(graph_t* graph, graph_node_t *from, graph_node_t *to)
{
    if(     !is_graph_node(graph, from)
        ||  !is_graph_node(graph, to))
    {
        return NULL;
    }
    
    return graph_node_add_edge(from, to);
}
/**
 * Removes edge from graph
 * @par graph graph_t structure where the edge is located
 * @par from graph_node_t structure from which the edge will originate
 * @par to graph_node_t structure towards which the edge will point
 * @returns 1 if edge was removed, 0 if edge was not found
 */
int graph_remove_edge(graph_t* graph, graph_node_t *from, graph_node_t *to)
{
    if(     !is_graph_node(graph, from)
        ||  !is_graph_node(graph, to))
    {
        return 0;
    }
    
    return graph_node_remove_edge_by_to_node(from, to);
}

/**
 * Finds edge
 * @par from graph_node_t structure from which the edge originates
 * @par to graph_node_t structure towards which the edge points
 * @returns pointer to graph_edge_t structure or NULL if edge is not found
 */
graph_edge_t* graph_node_find_edge(graph_node_t* from, graph_node_t *to)
{
    graph_edge_t *edge;
    edge = from->edges;
    while(  edge != NULL
        &&  edge->to != to)
    {
        edge = edge->next;
    }
    return edge;
}
/**
 * Finds edge
 * @par from graph_node_t structure from which the edge originates
 * @par value value of the searched edge
 * @returns pointer to graph_edge_t structure or NULL if edge is not found
 */
graph_edge_t* graph_node_find_edge_by_value(graph_node_t* from, int value)
{
    graph_edge_t *edge;
    edge = from->edges;
    while(  edge != NULL
        &&  edge->value != value)
    {
        edge = edge->next;
    }
    return edge;
}

/**
 * Creates a complete graph
 * @par number_of_nodes number of nodes the complete graph will contain
 * @returns pointer towards newly allocated graph_t structure
 */
graph_t* make_complete_graph(size_t number_of_nodes)
{
    size_t i, j;
    graph_t *graph;
    
    graph = make_empty_graph();
    
    for(i = 0; i < number_of_nodes; i++)
    {
        graph_add_node(graph);
    }
    
    for(i = 0; i < graph->node_count; i++)
    {
        for(j = 0; j < graph->node_count; j++)
        {
            if(i != j)
            {
                graph_add_edge(graph, &graph->nodes[i], &graph->nodes[j]);
            }
        }
    }
    
    return graph;
}

/**
 * Returns 1 if node belong to the graph
 * @par graph graph_t structure
 * @par node graph_node_t structure
 * @returns 1 if node is part of graph, 0 otherwise
 */
int is_graph_node(graph_t *graph, graph_node_t *node)
{
    graph_node_t *tmp = graph->nodes;
    while(tmp != NULL)
    {
        if(tmp == node)
        {
            return 1;
        }
        tmp = tmp->next;
    }
    
    return 0;
}
/**
 * Returns 1 if edge belong to the graph node
 * @par node graph_node_t structure
 * @par edge graph_edge_t structure
 * @returns 1 if edge is part of graph, 0 otherwise
 */
int is_node_edge(graph_node_t *node, graph_edge_t *edge)
{
    graph_edge_t *tmp = node->edges;
    while(tmp != NULL)
    {
        if(tmp == edge)
        {
            return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}

#define REPORT_VALUE(val, print_symbol) printf("\t " #val " = " #print_symbol "\n", val)

/**
 * Tests the graph framewor
 * @returns 1 on success, 0 otherwise
 */
int test_graphs()
{
    int rslt, r;
    size_t i;
    graph_t *graph, *graph2;
    graph_node_t *node, *node2, *node3, *node4;
    graph_edge_t *edge, *edge2, *edge3, *edge4;
    
    rslt = 1;
    
    graph = make_empty_graph();
    if(graph == NULL)
    {
        rslt = 0;
        printf("Error: make_empty_graph() failed\n");
        printf("\n");
    }
    
    //ADD NODE
    node = graph_add_node(graph);
    
    if(     node == NULL
        ||  graph->nodes == NULL
        ||  graph->nodes != node)
    {
        rslt = 0;
        printf("Error: add_node(graph) failed\n");
        REPORT_VALUE(node, %p);
        REPORT_VALUE(graph->nodes, %p);
        printf("\n");
    }
    
    node2 = graph_add_node(graph);
    
    if(     node2 == NULL
        ||  graph->nodes == NULL
        ||  graph->nodes->next != node2)
    {
        rslt = 0;
        printf("Error: add_node(graph) (2) failed\n");
        REPORT_VALUE(node2, %p);
        REPORT_VALUE(graph->nodes, %p);
        REPORT_VALUE(graph->nodes->next, %p);
        printf("\n");
    }
    
    //ADD EDGE
    node3 = graph_add_node(graph);    
    edge = graph_node_add_edge(node, node2);
    if(     edge == NULL
        ||  node->edges == NULL
        ||  node->edges->from != node
        ||  node->edges->to != node2)
    {
        rslt = 0;
        printf("Error: graph_node_add_edge(node, node2);\n");
        REPORT_VALUE(edge, %p);
        REPORT_VALUE(node->edges, %p);
        REPORT_VALUE(node->edges->from, %p);
        REPORT_VALUE(node, %p);
        REPORT_VALUE(node->edges->to, %p);
        REPORT_VALUE(node2, %p);
        printf("\n");
    }
    
    edge = graph_add_edge(graph, node2, node3);
    if(     edge == NULL
        ||  node2->edges == NULL
        ||  node2->edges->from != node2
        ||  node2->edges->to != node3)
    {
        rslt = 0;
        printf("Error: graph_add_edge(graph, node2, node3);\n");
        REPORT_VALUE(edge, %p);
        REPORT_VALUE(node2->edges->from, %p);
        REPORT_VALUE(node2, %p);
        REPORT_VALUE(node2->edges->to, %p);
        REPORT_VALUE(node3, %p);
        printf("\n");
    }
    
    //IS GRAPH NODE
    graph2 = make_empty_graph();
    node4 = graph_add_node(graph2);
    
    r = is_graph_node(graph, node2);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: is_graph_node(graph, node2) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(graph->nodes, %p);
        REPORT_VALUE(node2, %p);
        printf("\n");
    }
    
    r = is_graph_node(graph, node4);
    if(r != 0)
    {
        rslt = 0;
        printf("Error: is_graph_node(graph, node4) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(graph->nodes, %p);
        REPORT_VALUE(node4, %p);
        printf("\n");
    }
    
    //IS NODE EDGE
    edge = node->edges;
    edge2 = node->edges->next;
    edge3 = graph_add_edge(graph2, graph2->nodes, graph2->nodes->next);
    edge4 = graph_add_edge(graph2, graph2->nodes->next, graph2->nodes);
    
    r = is_node_edge(node, edge);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: is_node_edge(node, edge) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(node, %p);
        REPORT_VALUE(node->edges, %p);
        REPORT_VALUE(edge, %p);
        printf("\n");
    }
    
    r = is_node_edge(node, edge3);
    if(r != 0)
    {
        rslt = 0;
        printf("Error: is_node_edge(node, edge3) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(node, %p);
        REPORT_VALUE(node->edges, %p);
        REPORT_VALUE(edge3, %p);
        printf("\n");
    }
    
    //FIND NODE
    node->value = 42;
    node2 = graph_find_node_by_value(graph, -1); //Nonexistant value
    if(node2 != NULL)
    {
        rslt = 0;
        printf("Error: graph_find_node_by_value(graph, -1) failed\n");
        printf("\n");
    }
    
    node2 = graph_find_node_by_value(graph, 42);
    if(node2 != node)
    {
        rslt = 0;
        printf("Error: graph_find_node_by_value(graph, 42) failed\n");
        printf("\n");
    }
    node2 = graph->nodes->next;
    
    //FIND EDGE
    edge2 = graph_node_find_edge(node, node); //Nonexistant edge
    if(edge2 != NULL)
    {
        rslt = 0;
        printf("Error: graph_node_find_edge(node, node) failed\n");
        printf("\n");
    }
    
    edge2 = graph_node_find_edge_by_value(node, -1); //Nonexistant value
    if(edge2 != NULL)
    {
        rslt = 0;
        printf("Error: graph_node_find_edge_by_value(node, -1) failed\n");
        printf("\n");
    }
    
    edge2 = graph_node_find_edge(node, node2);
    if(edge2 != edge)
    {
        rslt = 0;
        printf("Error: graph_node_find_edge(node, node2) failed\n");
        printf("\n");
    }
    
    graph->nodes->edges->value = 42;
    edge2 = graph_node_find_edge_by_value(node, 42);
    if(edge2 != graph->nodes->edges)
    {
        rslt = 0;
        printf("Error: graph_node_find_edge_by_value(node, 42) failed\n");
        printf("\n");
    }
    edge2 = node->edges->next;
    
    //REMOVE EDGE
    edge3 = graph_add_edge(graph, node2, node);
    
    r = graph_node_remove_edge(node, edge3);
    if(r != 0)
    {
        rslt = 0;
        printf("Error: graph_node_remove_edge(node, edge3) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(edge, %p);
        REPORT_VALUE(node->edges, %p);
        printf("\n");
    }
    
    r = graph_node_remove_edge_by_to_node(node, node3);
    if(r != 0)
    {
        rslt = 0;
        printf("Error: graph_node_remove_edge_by_to_node(node, node3) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(node, %p);
        REPORT_VALUE(node3, %p);
        REPORT_VALUE(node->edges, %p);
        printf("\n");
    }
    
    r = graph_remove_edge(graph, node, node3);
    if(r != 0)
    {
        rslt = 0;
        printf("Error: graph_remove_edge(graph, node, node3) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(graph, %p);
        REPORT_VALUE(node, %p);
        REPORT_VALUE(node3, %p);
        printf("\n");
    }
    
    r = graph_node_remove_edge(node, edge);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: graph_node_remove_edge(node, edge) failed\n");
        printf("\n");
    }
    
    r = graph_node_remove_edge_by_to_node(node2, node);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: graph_node_remove_edge_by_to_node(node2, node) failed\n");
        printf("\n");
    }
    
    r = graph_remove_edge(graph2, node3, node4);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: graph_remove_edge(graph2, node3, node4) failed\n");
        printf("\n");
    }
    
    //REMOVE NODE
    graph_node_add_edge(node, node2);
    node3 = node2->next;
    graph_add_edge(graph, node2, node3);
    graph_add_edge(graph, node, node3);
    graph_add_edge(graph, node2, node);
    
    r = graph_remove_node(graph, node);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: graph_remove_node(graph, node) failed\n");
        REPORT_VALUE(r, %d);
        printf("\n");
    }
    
    r = graph_remove_node(graph, node2);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: graph_remove_node(graph, node2) failed\n");
        REPORT_VALUE(r, %d);
        printf("\n");
    }
    
    //MAKE COMPLETE GRAPH
    graph = make_complete_graph(25);
    
    r = 1;
    if(graph->node_count != 25)
    {
        r = 0;
    }
    else
    {
        for(i = 0; i < graph->node_count; i++)
        {
            size_t j;
            for(j = 0; j < graph->node_count; j++)
            {
                if(i != j)
                {
                    if(graph_node_find_edge(&graph->nodes[i], &graph->nodes[j]) == NULL)
                    {
                        r = 0;
                        break;
                    }
                }
            }
        }
    }
    
    if(!r)
    {
        rslt = 0;
        printf("Error: make_complete_graph(12) failed\n");
        printf("\n");
    }
    
    return rslt;
}
