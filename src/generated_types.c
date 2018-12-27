#include "generated_types.h"
#include <stdio.h>
#include <stdlib.h>
#include "garbage_collector.h"
#include "gc_util.h"
#include "gc_constants.h"

XGENERATE_TYPES_BTREE(MAKE_BTREE_SRC)

#define SELET_BTREE_INSERT(SUFFIX, TYPES, NUM)          \
    case NUM:                                           \
        return btree ## SUFFIX ## _insert((GEN_BTREE_TYPE(SUFFIX)**)btree, value);

/**
 * Inserts value into a generated type btree based on the given btree type
 * @par btree pointer to generated btree structure
 * @par value inserted value
 * @par used_type used generated btree type
 */
int gen_btree_insert(void **btree, long value, uint64_t used_type)
{
    switch(used_type)
    {
        XGENERATE_TYPES_BTREE(SELET_BTREE_INSERT)
    }
    
    return 0;
}
