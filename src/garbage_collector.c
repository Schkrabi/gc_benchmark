/**
 * @name garbage_collector.c
 * @author Mgr. Radomir Skrabal
 * This file contains implementation for unified API of benchmarked garbage collectors
 */

#include "garbage_collector.h"
#include "gc_shared.h"
#include "gc_cheney.h"
#include "gc_custom.h"
#include <stdlib.h>
#include "gc_constants.h"
#include <time.h>
#include "gc_util.h"
#include <syslog.h>
#include <string.h>

/**
 * Logging macros uncomment to log specific functionality
 */
//#define LOG_ALLOCATION
#define LOG_COLLECTION

 /**
 * Byte indicating which garbage collector is used
 */
char used_gc;

#ifdef LOG_COLLECTION
size_t collection_no = 0;
#endif

/**
 * Xmacros for generating switches
 */
#define __XCOLLECTOR_INIT(collector, num) case num : return gc_ ## collector ## _init();
#define __XCOLLECTOR_MALLOC(collector, num) case num : return gc_ ## collector ## _malloc(type);
#define __XCOLLECTOR_MALLOC_ARRAY(collector, num) case num : return gc_ ## collector ## _malloc_array(type, size);
#define __XCOLLECTOR_COLLECT(collector, num) case num : rtrno = gc_ ## collector ## _collect();
#define __XCOLLECTOR_CLEANUP(collector, num) case num : return gc_ ## collector ## _cleanup();
#define __XCOLLECTOR_PARSE_ARG(collector, num) if(strcmp(arg, #collector) == 0) return num;
#define __XCOLLECTOR_REMAINING_SPACE(collector, num) case num: return gc_ ## collector ## _remaining_space();
#define __XCOLLECTOR_NUM_TO_STR(collector, num) case num: return #collector;

/**
 * Parses argument given by -g in command line
 * @par arg -g argument
 * @return garbage collector number or INVALID_GC
 */
int parse_gc_arg(const char *arg)
{
    XCOLLECTOR_TABLE(__XCOLLECTOR_PARSE_ARG)
    return INVALID_GC;
}

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
#ifdef LOG_ALLOCATION
    gc_log(LOG_INFO, "A AT %d", type);
#endif
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
#ifdef LOG_ALLOCATION
    gc_log(LOG_INFO, "A AR %d, %u", type, (unsigned)size);
#endif
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
    
#ifdef LOG_COLLECTION
    //Logging start of the collection, collection id, tick time, available free memory, 
    gc_log(LOG_INFO, "CS %u %u %u", (unsigned)collection_no, (unsigned)clock(), (unsigned)gc_remaining_space()); 
#endif
    switch(used_gc)
    {
        XCOLLECTOR_TABLE(__XCOLLECTOR_COLLECT)
        case NO_COLLECTOR:
        default:
        rtrno = 0;
    }
#ifdef LOG_COLLECTION
    //Logging end of the collection, collection id, tick time, available free memory, 
    gc_log(LOG_INFO, "CE %u %u %u", (unsigned) collection_no, (unsigned)clock(), (unsigned)gc_remaining_space());
    collection_no++;
#endif
    return rtrno;
}

/**
 * Returns the remaining space in bytes that collector has available
 * @return space in bytes or -1 if collector is limmited only by system
 */
int64_t gc_remaining_space()
{
    switch(used_gc)
    {
        XCOLLECTOR_TABLE(__XCOLLECTOR_REMAINING_SPACE)
        case NO_COLLECTOR:
        default:
            return -1;
    }
}

/**
 * Takes collector constant and returns constant strig with collector name
 * @par gc_num number of the garbage collector
 * @return constant string with the collector name or empty string
 */
const char* gc_to_str(int gc_num)
{
    switch(gc_num)
    {
        XCOLLECTOR_TABLE(__XCOLLECTOR_NUM_TO_STR)
        default:
            gc_log(LOG_ERR, "Invalid collector number %d", gc_num);
            return "";
    }
}