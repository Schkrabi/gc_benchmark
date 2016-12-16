/**
 * @name garbage_collector.c
 * @author Mgr. Radomir Skrabal
 * This file contains implementation for unified API of benchmarked garbage collectors
 */

#include "gc_shared.h"
#include "gc_cheney.h"
#include <stdlib.h>
#include "gc_constants.h"
#include <time.h>
#include "gc_util.h"
#include <syslog.h>

 /**
 * Byte indicating which garbage collector is used
 */
char used_gc;

size_t collection_no = 0;

/**
 * Xmacros for generating switches
 */
#define __XCOLLECTOR_INIT(collector, num) case num : return gc_ ## collector ## _init();
#define __XCOLLECTOR_MALLOC(collector, num) case num : return gc_ ## collector ## _malloc(type);
#define __XCOLLECTOR_MALLOC_ARRAY(collector, num) case num : return gc_ ## collector ## _malloc_array(type, size);
#define __XCOLLECTOR_COLLECT(collector, num) case num : rtrno = gc_ ## collector ## _collect();
#define __XCOLLECTOR_CLEANUP(collector, num) case num : return gc_ ## collector ## _cleanup();

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
 * Cleans up the garbage collector objext
 * @return If everything went well 0, otherwise error code
 */
int gc_cleanup()
{
    switch(used_gc)
    {
        XCOLLECTOR_TABLE(__XCOLLECTOR_CLEANUP)
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
    gc_log(LOG_INFO, "A AT %d", type);
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
    gc_log(LOG_INFO, "A AR %d, %u", type, (unsigned)size);
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
    int rtrno;
    
    //Logging start of the collection, collection id, tick time, available free memory, 
    gc_log(LOG_INFO, "CS %u, %u, %u", (unsigned)collection_no, (unsigned)clock(), (unsigned)block_get_size(remaining_block)); 
    switch(used_gc)
    {
        XCOLLECTOR_TABLE(__XCOLLECTOR_COLLECT)
        case NO_COLLECTOR:
        default:
        rtrno = 0;
    }
    //Logging end of the collection, collection id, tick time, available free memory, 
    gc_log(LOG_INFO, "CE %u, %u, %u", (unsigned) collection_no, (unsigned)clock(), (unsigned)block_get_size(remaining_block));
    collection_no++;
    return rtrno;
}