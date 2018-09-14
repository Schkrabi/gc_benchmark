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
    info->number_of_references = 1;
    info->references = (ptr_info_t*)malloc(sizeof(ptr_info_t));
    info->references[0].offset = (uint64_t)((art_ptr_t)&measure.edges - (art_ptr_t)&measure);
    ptr_info_set_type(&info->references[0], TYPE_GRAPH_EDGE_T);
    ptr_info_set_is_array(&info->references[0], 1);
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
    info->references = (ptr_info_t*)malloc(2 * sizeof(ptr_info_t));
    info->references[0].offset = (uint64_t)((art_ptr_t)&measure.from - (art_ptr_t)&measure);
    ptr_info_set_type(&info->references[0], TYPE_GRAPH_NODE_T);
    info->references[1].offset = (uint64_t)((art_ptr_t)&measure.to - (art_ptr_t)&measure);
    ptr_info_set_type(&info->references[1], TYPE_GRAPH_EDGE_T);
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
    info->references = (ptr_info_t*)malloc(sizeof(ptr_info_t));
    info->references[0].offset = (uint64_t)((art_ptr_t)&measure.nodes - (art_ptr_t)&measure);
    ptr_info_set_type(&info->references[0], TYPE_GRAPH_NODE_T);
    ptr_info_set_is_array(&info->references[0], 1);
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
 * Fixes the edges in the nodes so all will be pointing inside the array
 * @par nodes array of graph_node_t structures
 * @par size size of the array
 * @returns pointer to nodes
 */
graph_node_t* fix_edges(graph_node_t* nodes, size_t size)
{
    size_t i, j, k;
    
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < nodes[i].edge_count; j++)
        {
            uint64_t id;
            
            nodes[i].edges[j].from = &nodes[i];
            id = nodes[i].edges[j].to->id;
            
            for(k = 0; k < size; k++)
            {
                if(nodes[k].id == id)
                {
                    nodes[i].edges[j].to = &nodes[k];
                    break;
                }
            }
        }
    }
    
    return nodes;
}

/**
 * Moves one array of graph_nodes_t to other. The original array is invalidated (destroyed) in the process
 * @par src source array
 * @par dst destination array
 * @par array_size number of nodes that will be moved
 * @returns dst pointer
 */
graph_node_t* graph_node_array_deep_move(graph_node_t *src, graph_node_t *dst, size_t array_size)
{
    size_t i, j, k;
    memcpy(dst, src, array_size * sizeof(graph_node_t));
    
    //Fix edges
    fix_edges(dst, array_size);
    
    return dst;
}

/**
 * Adds new node to graph
 * @par graph graph_t structure to which node is added
 * @returns pointer to newly allocated graph_node_t structure
 */
graph_node_t* graph_add_node(graph_t *graph)
{
    size_t 
        new_node_count,
        new_node_index;
    graph_node_t* nodes;
    
    new_node_count = graph->node_count + 1;
    nodes = gc_malloc_array(graph_node_t, graph->node_count + 1);
    
    if(graph->nodes != NULL)
    {
        graph_node_array_deep_move(graph->nodes, nodes, graph->node_count);
    }
    
    new_node_index = new_node_count - 1;
    
    nodes[new_node_index].id = graph->next_id;
    graph->next_id++;
    nodes[new_node_index].value = 0;
    nodes[new_node_index].edge_count = 0;
    nodes[new_node_index].edges = NULL;
    
    graph->node_count = new_node_count;
    graph->nodes = nodes;
    
    return &nodes[new_node_index];
}
/**
 * removes node and all edges from and to it from the graph
 * @par graph graph_t structure where node is located
 * @par id id of the removed node
 * @returns 1 if node was removed, 0 if node was not found in graph
 */
int graph_remove_node_by_id(graph_t* graph, uint64_t id)
{
    return graph_remove_node(graph, graph_find_node(graph, id));
}
/**
 * removes node and all edges from and to it from the graph
 * @par graph graph_t structure where node is located
 * @par node pointer to removed node
 * @returns 1 if node was removed, 0 if node was not found in graph
 */
int graph_remove_node(graph_t* graph, graph_node_t *node)
{
    size_t i, new_node_count, deleted_index;
    graph_node_t *nodes;
    
    if(!is_graph_node(graph, node))
    {
        return 0;
    }
    
    if(graph->node_count == 1)
    {
        graph->node_count = 0;
        graph->nodes = NULL;
        return 1;
    }
    
    deleted_index = (size_t)(((art_ptr_t)node - (art_ptr_t)graph->nodes)/sizeof(graph_node_t));
    
    //Remove invalid edges
    for(i = 0; i < graph->node_count; i++)
    {
        graph_node_remove_edge_by_to_node(&graph->nodes[i], node);
    }
    
    //Remove the node
    new_node_count = graph->node_count - 1;
    nodes = gc_malloc_array(graph_node_t, new_node_count);
    
    memcpy(nodes, graph->nodes, (size_t)((art_ptr_t)node - (art_ptr_t)graph->nodes));
    memcpy(&nodes[deleted_index], &graph->nodes[deleted_index + 1], (graph->node_count - deleted_index - 1) * sizeof(graph_node_t));
    
    fix_edges(nodes, new_node_count);
    graph->nodes = nodes;
    graph->node_count = new_node_count;
    
    return 1;
}

/**
 * Finds a node in a graph
 * @par graph searched graph_t struture
 * @par id id of the searched node
 * @returns pointer to graph_node_t structure or NULL if node was not found
 */
graph_node_t* graph_find_node(graph_t* graph, uint64_t id)
{
    size_t i;
    for(i = 0; i < graph->node_count; i++)
    {
        if(graph->nodes[i].id == id)
        {
            return &graph->nodes[i];
        }
    }
    return NULL;
}
/**
 * Finds a first node in a graph with specified value
 * @par graph searched graph_t struture
 * @par value value of the searched node
 * @returns pointer to graph_node_t structure or NULL if node was not found
 */
graph_node_t* graph_find_node_by_value(graph_t* graph, int value)
{
    size_t i;
    for(i = 0; i < graph->node_count; i++)
    {
        if(graph->nodes[i].value == value)
        {
            return &graph->nodes[i];
        }
    }
    return NULL;
}

/**
 * Adds edge to node
 * @par from graph_node_t structure from which the edge will originate
 * @par to graph_node_t structure towards which the edge will point
 * @returns pointer to newly allocated graph_edge_t structure
 */
graph_edge_t* graph_node_add_edge(graph_node_t* from, graph_node_t *to)
{
    size_t new_edge_count, new_edge_index;
    graph_edge_t *edges;
    
    new_edge_count = from->edge_count + 1;
    edges = gc_malloc_array(graph_edge_t, new_edge_count);
    memcpy(edges, from->edges, from->edge_count * sizeof(graph_edge_t));
    
    new_edge_index = new_edge_count - 1;
    edges[new_edge_index].value = 0;
    edges[new_edge_index].from = from;
    edges[new_edge_index].to = to;
    
    from->edges = edges;
    from->edge_count = new_edge_count;
    
    return &edges[new_edge_index];
}
/**
 * Removes edge from node
 * @par from graph_node_t structure from which the edge originates
 * @par edge pointer to edge to be removed
 * @returns 1 if edge was removed, 0 if edge was not found
 */
int graph_node_remove_edge(graph_node_t *from, graph_edge_t *edge)
{
    size_t deleted_index, new_edge_count;
    graph_edge_t* edges;
    if(!is_node_edge(from, edge))
    {
        return 0;
    }
    if(from->edge_count == 1)
    {
        from->edge_count = 0;
        from->edges = NULL;
        return 1;
    }
    
    deleted_index = (size_t)(((art_ptr_t)edge - (art_ptr_t)from->edges)/sizeof(graph_edge_t));
    new_edge_count = from->edge_count - 1;
    edges = gc_malloc_array(graph_edge_t, new_edge_count);
    
    memcpy(edges, from->edges, (size_t)((art_ptr_t)edge - (art_ptr_t)from->edges));
    memcpy(&edges[deleted_index], &from->edges[deleted_index+1], (from->edge_count - deleted_index - 1) * sizeof(graph_edge_t));
    
    from->edges = edges;
    from->edge_count = new_edge_count;
    
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
 * Add edge to graph
 * @par graph graph_t structure where the node is added
 * @par from_id id of node from which the edge will originate
 * @par to_id id of node towards which the edge will point
 * @returns Pointer towards newly allocated graph_edge_t structure or NULL if from_id or to_id nodes are not part of graph
 */
graph_edge_t* graph_add_edge_by_id(graph_t* graph, uint64_t from_id, uint64_t to_id)
{
    return graph_add_edge(graph, graph_find_node(graph, from_id), graph_find_node(graph, to_id));
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
 * Removes edge from graph
 * @par graph graph_t structure where the edge is located
 * @par from_id id of node from which the edge will originate
 * @par to_id id of node towards which the edge will point
 * @returns 1 if edge was removed, 0 if edge was not found
 */
int graph_remove_edge_by_id(graph_t* graph, uint64_t from_id, uint64_t to_id)
{
    return graph_remove_edge(graph, graph_find_node(graph, from_id), graph_find_node(graph, to_id));
}

/**
 * Finds edge
 * @par from graph_node_t structure from which the edge originates
 * @par to graph_node_t structure towards which the edge points
 * @returns pointer to graph_edge_t structure or NULL if edge is not found
 */
graph_edge_t* graph_node_find_edge(graph_node_t* from, graph_node_t *to)
{
    size_t i;
    for(i = 0; i < from->edge_count; i++)
    {
        if(from->edges[i].to == to)
        {
            return &from->edges[i];
        }
    }
    
    return NULL;
}
/**
 * Finds edge
 * @par from graph_node_t structure from which the edge originates
 * @par value value of the searched edge
 * @returns pointer to graph_edge_t structure or NULL if edge is not found
 */
graph_edge_t* graph_node_find_edge_by_value(graph_node_t* from, int value)
{
    size_t i;
    for(i = 0; i < from->edge_count; i++)
    {
        if(from->edges[i].value == value)
        {
            return &from->edges[i];
        }
    }
    
    return NULL;
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
    return node > graph->nodes && (node < (graph_node_t*)((art_ptr_t)graph->nodes + (graph->node_count * sizeof(graph_node_t))));
}
/**
 * Returns 1 if edge belong to the graph node
 * @par node graph_node_t structure
 * @par edge graph_edge_t structure
 * @returns 1 if edge is part of graph, 0 otherwise
 */
int is_node_edge(graph_node_t *node, graph_edge_t *edge)
{
    return edge > node->edges && (edge < (graph_edge_t*)((art_ptr_t)node->edges + (node->edge_count * sizeof(graph_edge_t))));
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
    graph_node_t *node, *node2;
    graph_edge_t *edge;
    
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
        ||  graph->node_count != 1
        ||  &graph->nodes[0] != node)
    {
        rslt = 0;
        printf("Error: add_node(graph) failed\n");
        REPORT_VALUE(node, %p);
        REPORT_VALUE(graph->nodes, %p);
        REPORT_VALUE(graph->node_count, %zu);
        REPORT_VALUE(&graph->nodes[0], %p);
        printf("\n");
    }
    
    node2 = graph_add_node(graph);
    
    if(     node2 == NULL
        ||  graph->nodes == NULL
        ||  graph->node_count != 2
        ||  &graph->nodes[1] != node2)
    {
        rslt = 0;
        printf("Error: add_node(graph) (2) failed\n");
        REPORT_VALUE(node2, %p);
        REPORT_VALUE(graph->nodes, %p);
        REPORT_VALUE(graph->node_count, %zu);
        REPORT_VALUE(&graph->nodes[1], %p);
        printf("\n");
    }
    
    //ADD EDGE
    graph_add_node(graph);    
    edge = graph_node_add_edge(&graph->nodes[0], &graph->nodes[1]);
    if(     edge == NULL
        ||  graph->nodes[0].edges == NULL
        ||  graph->nodes[0].edges[0].to != &graph->nodes[1]
        ||  graph->nodes[0].edges[0].from != &graph->nodes[0])
    {
        rslt = 0;
        printf("Eroor: add_edge(&graph->nodes[0], &graph->nodes[1]);\n");
        REPORT_VALUE(edge, %p);
        REPORT_VALUE(graph->nodes[0].edges, %p);
        REPORT_VALUE(graph->nodes[0].edges[0].to, %p);
        REPORT_VALUE(&graph->nodes[1], %p);
        REPORT_VALUE(graph->nodes[0].edges[0].from, %p);
        REPORT_VALUE(&graph->nodes[0], %p);
        printf("\n");
    }
    
    edge = graph_add_edge(graph, &graph->nodes[1], &graph->nodes[2]);
    if(     edge == NULL
        ||  graph->nodes[1].edges == NULL
        ||  graph->nodes[1].edges[0].to != &graph->nodes[2]
        ||  graph->nodes[1].edges[0].from != &graph->nodes[1])
    {
        rslt = 0;
        printf("Error: add_edge((graph, &graph->nodes[1], &graph->nodes[2]);\n");
        REPORT_VALUE(edge, %p);
        REPORT_VALUE(graph->nodes[1].edges, %p);
        REPORT_VALUE(graph->nodes[1].edges[0].to, %p);
        REPORT_VALUE(&graph->nodes[2], %p);
        REPORT_VALUE(graph->nodes[1].edges[0].from, %p);
        REPORT_VALUE(&graph->nodes[1], %p);
        printf("\n");
    }
    
    edge = graph_add_edge_by_id(graph, graph->nodes[0].id, graph->nodes[2].id);
    if(     edge == NULL
        ||  graph->nodes[0].edges == NULL
        ||  graph->nodes[0].edges[1].to != &graph->nodes[2]
        ||  graph->nodes[0].edges[1].from != &graph->nodes[0])
    {
        rslt = 0;
        printf("Error: add_edge(graph, &graph->node[0], graph->nodes[2].id);\n");
        REPORT_VALUE(edge, %p);
        REPORT_VALUE(graph->nodes[0].edges, %p);
        REPORT_VALUE(graph->nodes[0].edges[1].to, %p);
        REPORT_VALUE(&graph->nodes[2], %p);
        REPORT_VALUE(graph->nodes[0].edges[1].from, %p);
        REPORT_VALUE(&graph->nodes[0], %p);
        printf("\n");
    }
    
    //IS GRAPH NODE
    graph2 = make_empty_graph();
    graph_add_node(graph2);
    graph_add_node(graph2);
    
    r = is_graph_node(graph, &graph->nodes[1]);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: is_graph_node(graph, &graph->node[1]) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(graph->nodes, %p);
        REPORT_VALUE((graph_node_t*)((art_ptr_t)graph->nodes + (graph->node_count * sizeof(graph_node_t))), %p);
        REPORT_VALUE(&graph->nodes[1], %p);
        printf("\n");
    }
    
    r = is_graph_node(graph, &graph2->nodes[1]);
    if(r != 0)
    {
        rslt = 0;
        printf("Error: is_graph_node(graph, &graph2->nodes[1]); failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(graph->nodes, %p);
        REPORT_VALUE((graph_node_t*)((art_ptr_t)graph->nodes + (graph->node_count * sizeof(graph_node_t))), %p);
        REPORT_VALUE(&graph2->nodes[1], %p);
        printf("\n");
    }
    
    //IS GRAPH NODE
    graph_add_edge(graph2, &graph2->nodes[0], &graph2->nodes[1]);
    graph_add_edge(graph2, &graph2->nodes[1], &graph2->nodes[0]);
    
    r = is_node_edge(&graph->nodes[0], &graph->nodes[0].edges[0]);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: is_node_edge(&graph->nodes[0], &graph->nodes[0].edges[0]) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(&graph->nodes[0].edges[0], %p);
        REPORT_VALUE(graph->nodes[0].edges, %p);
        REPORT_VALUE((graph_edge_t*)((art_ptr_t)graph->nodes[0].edges + (graph->nodes[0].edge_count * sizeof(graph_edge_t))), %p);
        printf("\n");
    }
    
    r = is_node_edge(&graph->nodes[0], &graph2->nodes[0].edges[0]);
    if(r != 0)
    {
        rslt = 0;
        printf("Error: is_node_edge(&graph->nodes[0], &graph2->nodes[0].edges[0]) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(&graph2->nodes[0].edges[0], %p);
        REPORT_VALUE(graph->nodes[0].edges, %p);
        REPORT_VALUE((graph_edge_t*)((art_ptr_t)graph->nodes[0].edges + (graph->nodes[0].edge_count * sizeof(graph_edge_t))), %p);
        printf("\n");
    }
    
    //FIND NODE
    node = graph_find_node(graph, (uint64_t)50); //Nonexistant id
    if(node != NULL)
    {
        rslt = 0;
        printf("Error: find_node(graph, (uint64_t)50) failed\n");
        printf("\n");
    }
    
    node = graph_find_node_by_value(graph, (int)50); //Nonexistant value
    if(node != NULL)
    {
        rslt = 0;
        printf("Error: find_node_by_value(graph, (int)50) failed\n");
        printf("\n");
    }
    
    node = graph_find_node(graph, graph->nodes[1].id);
    if(node != &graph->nodes[1])
    {
        rslt = 0;
        printf("Error: find_node(graph, graph->nodes[1].id) failed\n");
        printf("\n");
    }
    
    graph->nodes[1].value = 42;
    node = graph_find_node_by_value(graph, graph->nodes[1].value);
    if(node != &graph->nodes[1])
    {
        rslt = 0;
        printf("Error: find_node_by_value(graph, graph->nodes[1].value) failed\n");
        printf("\n");
    }
    
    //FIND EDGE
    edge = graph_node_find_edge(&graph->nodes[0], &graph->nodes[0]); //Nonexistant edge
    if(edge != NULL)
    {
        rslt = 0;
        printf("Error: find_edge(&graph->nodes[0], &graph->nodes[0]) failed\n");
        printf("\n");
    }
    
    edge = graph_node_find_edge_by_value(&graph->nodes[0], 50); //Nonexistant value
    if(edge != NULL)
    {
        rslt = 0;
        printf("Error: find_edge_by_value(&graph->nodes[0], 50) failed\n");
        printf("\n");
    }
    
    edge = graph_node_find_edge(&graph->nodes[0], &graph->nodes[1]);
    if(edge != &graph->nodes[0].edges[0])
    {
        rslt = 0;
        printf("Error: find_edge(&graph->nodes[0], &graph->nodes[1]) failed\n");
        printf("\n");
    }
    
    graph->nodes[0].edges[1].value = 42;    
    edge = graph_node_find_edge_by_value(&graph->nodes[0], graph->nodes[0].edges[1].value);
    if(edge != &graph->nodes[0].edges[1])
    {
        rslt = 0;
        printf("Error: find_edge_by_value(&graph->nodes[0], graph->nodes[0].edges[1].value) failed\n");
        printf("\n");
    }
    
    //REMOVE EDGE
    graph_add_edge(graph, &graph->nodes[1], &graph->nodes[0]);
    
    r = graph_node_remove_edge(&graph->nodes[0], &graph2->nodes[0].edges[0]);
    if(r != 0)
    {
        rslt = 0;
        printf("Error: remove_edge(&graph->nodes[0], &graph2->nodes[0].edges[0]) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(&graph2->nodes[0].edges[0], %p);
        REPORT_VALUE(graph->nodes[0].edges, %p);
        REPORT_VALUE((graph_node_t*)((art_ptr_t)graph->nodes[0].edges + (graph->nodes[0].edge_count * sizeof(graph_edge_t))), %p);
        printf("\n");
    }
    
    r = graph_node_remove_edge_by_to_node(&graph->nodes[0], &graph2->nodes[0]);
    if(r != 0)
    {
        rslt = 0;
        printf("Error: remove_edge(&graph->node[0], &graph2->nodes[0]) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(&graph2->nodes[0], %p);
        REPORT_VALUE(&graph->nodes[0].edges, %p);
        REPORT_VALUE((graph_node_t*)((art_ptr_t)graph->nodes[0].edges + (graph->nodes[0].edge_count * sizeof(graph_edge_t))), %p);
        printf("\n");
    }
    
    r = graph_remove_edge(graph, &graph->nodes[0], &graph2->nodes[0]);
    if(r != 0)
    {
        rslt = 0;
        printf("Error: remove_edge(graph, @graph->nodes[0], graph2->nodes[0]) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(&graph2->nodes[0], %p);
        REPORT_VALUE(&graph->nodes, %p);
        REPORT_VALUE((graph_node_t*)((art_ptr_t)&graph->nodes + (graph->node_count * sizeof(graph_node_t))), %p);
        printf("\n");
    }
    
    r = graph_remove_edge_by_id(graph, graph->nodes[0].id, 50); //Nonexistant id
    if(r != 0)
    {
        rslt = 0;
        printf("Error: remove_edge(graph, graph->nodes[0].id, 50) failed\n");
        REPORT_VALUE(r, %d);
        printf("\n");
    }
    
    r = graph_node_remove_edge(&graph->nodes[0], &graph->nodes[0].edges[1]);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: remove_edge(&graph->nodes[0], &graph->nodes[0].edges[1]) failed\n");
        printf("\n");
    }
    
    r = graph_node_remove_edge_by_to_node(&graph->nodes[0], &graph->nodes[1]);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: remove_edge(&graph->nodes[0], &graph->nodes[1]) failed\n");
        printf("\n");
    }
    
    r = graph_remove_edge(graph2, &graph2->nodes[0], &graph2->nodes[1]);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: remove_edge(graph2, &graph2->nodes[0], &graph2->nodes[1]) failed\n");
        printf("\n");
    }
    
    r = graph_remove_edge_by_id(graph2, graph->nodes[1].id, graph->nodes[0].id);
    if(r != 1)
    {
        rslt = 0;
        printf("Error: remove_edge(graph2, &graph->nodes[1].id, &graph->nodes[0].id) failed\n");
        printf("\n");
    }
    
    //REMOVE NODE
    graph_node_add_edge(&graph->nodes[0], &graph->nodes[1]);
    graph_add_edge(graph, &graph->nodes[1], &graph->nodes[2]);
    graph_add_edge_by_id(graph, graph->nodes[0].id, graph->nodes[2].id);
    graph_add_edge(graph, &graph->nodes[1], &graph->nodes[0]);
    r = graph_remove_node_by_id(graph, 50); //Nonexistant ID
    if(r != 0)
    {
        rslt = 0;
        printf("Error: remove_node(graph, 50) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(graph->nodes, %p);
        printf("\n");
    }
    
    r = graph_remove_node(graph, &graph->nodes[0]);
    if(     r != 1
        ||  graph->node_count != 1)
    {
        rslt = 0;
        printf("Error: remove_node(graph, &graph->node[0]); failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(graph->nodes, %p);
        REPORT_VALUE(graph->nodes[0].id, %zu);
        printf("\n");
    }
    
    r = graph_remove_node_by_id(graph, graph->nodes[1].id);
    if(     r != 1
        ||  graph->node_count != 1)
    {
        rslt = 0;
        printf("Error: remove_node(graph, graph->nodes[0].id) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(graph->nodes, %p);
        REPORT_VALUE(graph->node_count, %zu);
        printf("\n");
    }
    
    r = graph_remove_node(graph, &graph->nodes[0]);
    if(     r != 1
        ||  graph->node_count != 0
        ||  graph->nodes != NULL)
    {
        rslt = 0;
        printf("Error: remove_node(graph, &graph->nodes[0]); (2) failed\n");
        REPORT_VALUE(r, %d);
        REPORT_VALUE(graph->nodes, %p);
        REPORT_VALUE(graph->node_count, %zu);
        printf("\n");
    }
    
    //MAKE COMPLETE GRAPH
    graph = make_complete_graph(12);
    
    r = 1;
    if(graph->node_count != 12)
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