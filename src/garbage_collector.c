/**
 * @name garbage_collector.c
 * @author Mgr. Radomir Skrabal
 * This file contains implementation for unified API of benchmarked garbage collectors
 */

#include "gc_shared.h"
#include "gc_cheney.h"
#include <stdlib.h>
#include "gc_constants.h"

 /**
 * Byte indicating which garbage collector is used
 */
char used_gc;

/**
 * Xmacros for generating switches
 */
#define __XCOLLECTOR_INIT(collector, num) case num : return gc_ ## collector ## _init();
#define __XCOLLECTOR_MALLOC(collector, num) case num : return gc_ ## collector ## _malloc(type);
#define __XCOLLECTOR_MALLOC_ARRAY(collector, num) case num : return gc_ ## collector ## _malloc_array(type, size);
#define __XCOLLECTOR_COLLECT(collector, num) case num : return gc_ ## collector ## _collect();

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_init()
{
  switch(used_gc)
  {
    XCOLLECTOR_TABLE(__XCOLLECTOR_INIT)
    case NO_COLLECTOR:
    default:
      return 0;  
  }
}

/**
 * Allocates memory for single (non-array) value
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *__gc_malloc(int type)
{
    switch(used_gc)
    {
        XCOLLECTOR_TABLE(__XCOLLECTOR_MALLOC)
        case NO_COLLECTOR:
        default:
            return NULL;
    }
}

/**
 * Allocates memory for an array of values
 * @par type type number
 * @return pointer to allocated memory or NULL
 */
void *__gc_malloc_array(int type, size_t size)
{
    switch(used_gc)
    {
        XCOLLECTOR_TABLE(__XCOLLECTOR_MALLOC_ARRAY)
        case NO_COLLECTOR:
        default:
            return NULL;
    }
}

/**
 * Collects the memory
 * @return Always 0
 */
int gc_collect()
{
  switch(used_gc)
  {
    XCOLLECTOR_TABLE(__XCOLLECTOR_COLLECT)
    case NO_COLLECTOR:
    default:
      return 0;
  }
}