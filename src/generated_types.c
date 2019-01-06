#include "generated_types.h"
#include <stdio.h>
#include <stdlib.h>
#include "garbage_collector.h"
#include "gc_util.h"
#include "gc_constants.h"

XGENERATE_TYPES_BTREE(MAKE_BTREE_SRC)

#define SELECT_BTREE_INSERT(SUFFIX, TYPES, NUM)\
    if(used_type == GEN_BTREE_NUM(SUFFIX))\
        return btree ## SUFFIX ## _insert((GEN_BTREE_TYPE(SUFFIX)**)btree, value);

/**
 * Inserts value into a generated type btree based on the given btree type
 * @par btree pointer to generated btree structure
 * @par value inserted value
 * @par used_type used generated btree type
 */
int gen_btree_insert(void **btree, long value, uint64_t used_type)
{
    XGENERATE_TYPES_BTREE(SELECT_BTREE_INSERT)
    return 0;
}

XGENERATE_TYPES_GRAPH(MAKE_GRAPH_SRC)

#define SELECT_GRAPH_MAKE_COMPLETE_GRAPH(SUFFIX, TYPES, NUM)\
        if(used_type == GEN_GRAPH_NUM(SUFFIX))\
            return (void*)make_complete_graph ## SUFFIX (number_of_nodes);
            
/**
 * Creates complete graph of generated type
 * @par number_of_nodes size of created complete graph
 * @par used_type number of generated type
 * @returns pointer newly created complete graph of given type
 */
void *gen_graph_make_complete_graph(size_t number_of_nodes, uint64_t used_type)
{
    XGENERATE_TYPES_GRAPH(SELECT_GRAPH_MAKE_COMPLETE_GRAPH)
    return NULL;
}

#define INIT_CONST_BTREE(SUFFIX, TYPES, NUM) GEN_BTREE_NUM(SUFFIX) = NUM + GEN_BTREE_TYPES_NUM_OFFSET;
#define INIT_CONST_GRAPH(SUFFIX, TYPES, NUM) \
    GEN_GRAPH_EDGE_NUM(SUFFIX) = (3 * NUM) + GEN_GRAPH_TYPES_NUM_OFFSET; \
    GEN_GRAPH_NODE_NUM(SUFFIX) = (3 * NUM) + GEN_GRAPH_TYPES_NUM_OFFSET + 1; \
    GEN_GRAPH_NUM(SUFFIX) = (3 * NUM) + GEN_GRAPH_TYPES_NUM_OFFSET + 2; 

/**
 * Initializes type num constants for generated types
 */
int gen_init_generated_types_constants()
{
    XGENERATE_TYPES_BTREE(INIT_CONST_BTREE)
    XGENERATE_TYPES_GRAPH(INIT_CONST_GRAPH)
}
            
