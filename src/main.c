/**
 * @name main.c
 * @author Mgr. Radomir Skrabal
 * Main entrypoint of the GC_Benchmark
 */
#include <stdio.h>
#include <stdlib.h>
#include "gc_shared.h"
#include "garbage_collector.h"
#include "gc_cheney.h"
#include <string.h>
#include "binary_tree.h"
#include <time.h>
#include "cyclic_list.h"
#include "gc_util.h"
#include "gc_constants.h"
#include "cdouble_list.h"
#include <limits.h>
#include <syslog.h>
#include "gc_types.h"
#include "benchmark.h"
#include <unistd.h>
#include "test.h"
#include "graph.h"
#include "gc_cheney_base.h"

#define GC_CLEANUP_ERR 0x1
#define TYPE_CLEANUP_ERR 0x2
#define LOG_CLEANUP_ERR 0x4

/**
 * Initializes subsystems
 * @par rand_seed seed used for random generation
 * @par test_num number of test to be carried out
 * @par gc_num garbage collector id 
 * @returns 0 if initializaion went Ok, otherwise error code
 */
int init(unsigned rand_seed, int test_num, int gc_num);

/**
 * Clenas up and frees subsystems
 * @returns 0 if cleanup went ok, error code otherwise
 */
int cleanup();
/**
 * Parses arguments of the program
 * @par argc argument count of the main
 * @par argv arguments of the main
 * @par seed output argument, passes random seed
 * @par test_num output argument passes the used test
 * @par gc output argument passes the garbage collector used for the run
 * @returns  if everything went well, error code otherwise
 */
int parse_args(int argc, char *argv[], unsigned *seed, unsigned *test_num, char *gc);

/**
 * Prints help string to stdout
 * @returns Number of characters printed
 */
int print_help_str();

/**
 * Main executed function
 * @par test_num test to be executed
 * @return protgram return code
 */
int sub_main(unsigned test_num, unsigned seed);

/**
 * Main entrypoint Garbage collector initial setup
 */
int main(int argc, char *argv[])
{
    int rtrn, 
        err_msg;
    unsigned    seed,
                test_num;
    
    //Default
    seed = time(0);
    test_num = TEST_SHORT_LIVED;
    used_gc = CHENEY_GC;
    
    //Set by arguments
    err_msg = parse_args(argc, argv, &seed, &test_num, &used_gc);
    if(err_msg != 0)
    {
        return err_msg;
    }
        
    err_msg = init(seed, test_num, used_gc);
    if(err_msg != 0)
    {
        cleanup();
        return err_msg;
    }
    
    printf("%s\n", get_session_ident());
    gc_log(LOG_INFO, "seed %u", seed); 
    
    rtrn = sub_main(test_num, seed);
    
    err_msg = cleanup();
    if(err_msg != 0)
    {
        return err_msg;
    }
    
    return rtrn;
}

/**
 * Initializes subsystems
 * @par rand_seed seed used for random generation
 * @par test_num number of test to be carried out
 * @par gc_num garbage collector id 
 * @returns 0 if initializaion went Ok, otherwise error code
 */
int init(unsigned rand_seed, int test_num, int gc_num)
{
    int err_msg;
    
    err_msg = init_logger(test_num, gc_num);
    
    if(err_msg != 0)
    {
        //Logger not initialuzed
        fprintf(stderr, "Logger initialization error %x", err_msg);
        return err_msg;
    }
    
    err_msg = init_type_table();
    if(err_msg != 0)
    {
        gc_log(LOG_ERR, "Type table init error %x", err_msg);
        return err_msg;
    }
    
    err_msg = gc_init();
    if(err_msg != 0)
    {
        gc_log(LOG_ERR, "Garbage collector init error %x", err_msg);
        return err_msg;
    }
    
    srand(rand_seed);
    
    return 0;
}

/**
 * Clenas up and frees subsystems
 * @returns 0 if cleanup went ok, error code otherwise
 */
int cleanup()
{
    int err_msg;
    char err_map = 0x0;
    
    err_msg = gc_cleanup();
    if(err_msg != 0)
    {
        gc_log(LOG_ERR, "Garbage collector cleanup error %x", err_msg);
        err_map = err_map | GC_CLEANUP_ERR;
    }
    
    err_msg = cleanup_type_table();
    if(err_msg != 0)
    {
        gc_log(LOG_ERR, "Type table cleanup error %x", err_msg);
        err_map = err_map | TYPE_CLEANUP_ERR;
    }
    
    err_msg = cleanup_logger();
    if(err_msg != 0)
    {
        //Logger error
        fprintf(stderr, "Logger cleanup error %x", err_msg);
        err_map = err_map | LOG_CLEANUP_ERR;
    }
    
    return (int)err_map;
}

/**
 * Parses arguments of the program
 * @par argc argument count of the main
 * @par argv arguments of the main
 * @par seed output argument, passes random seed
 * @par test_num output argument passes the used test
 * @par gc output argument passes the garbage collector used for the run
 * @returns  if everything went well, error code otherwise
 */
int parse_args(int argc, char *argv[], unsigned *seed, unsigned *test_num, char *gc)
{
    int index;
    int c;
    char *arg, *err_ptr;
    
    opterr = 0;

    while ((c = getopt (argc, argv, "s:t:g:h:q:m:o:a:c:e:")) != -1)
    {
        arg = NULL;
        err_ptr = NULL;
        
        switch (c)
        {
        case 's':
            arg = optarg;
            *seed = strtol(arg, &err_ptr, 0);
            if(arg == err_ptr)
            {
                fprintf(stderr, "Option -s requires a integer argument (decimal, hexadecimal or octal)\n");
                return 1;
            }
            break;
        case 't':
            *test_num = parse_test_arg(optarg);
            if(*test_num == TEST_INVALID)
            {
                fprintf(stderr, "Argument for option -t was not recognized\n");
                return 1;
            }
            break;
        case 'g':
            *gc = parse_gc_arg(optarg);
            if(*gc == INVALID_GC)
            {
                fprintf(stderr, "Argument for option -g was not recognized\n");
                return 1;
            }
            break;
        case 'q':
            arg = optarg;
            __test_size = strtol(arg, &err_ptr, 0);
            if(arg == err_ptr)
            {
                fprintf(stderr, "Option -q requires an integer argument (decimal, hexadecimal or octal)\n");
                return 1;
            }
            break;
        case 'a':
            arg = optarg;
            __max_structure_size = strtol(arg, &err_ptr, 0);
            if(arg == err_ptr)
            {
                fprintf(stderr, "Option -a requires an integer argument (decimal, hexadecimal or octal)\n");
                return 1;
            }
            break;
        case 'c':
            arg = optarg;
            __chance_to_replace = strtod(arg, &err_ptr);
            if(arg == err_ptr)
            {
                fprintf(stderr, "Option -c requires an integer argument\n");
                return 1;
            }
            break;
        case 'm':
            arg = optarg;
            __semispace_size = strtol(arg, &err_ptr, 0);
            if(arg == err_ptr)
            {
                fprintf(stderr, "Option -m requires a integer argument (decimal, hexadecimal or octal)\n");
                return 1;
            }
            break;
        case 'o':
            arg = optarg;
            __old_pool_size = strtol(arg, &err_ptr, 0);
            if(arg == err_ptr)
            {
                fprintf(stderr, "Option -o requires a integer argument (decimal, hexadecimal or octal)\n");
                return 1;
            }
            break;
        case 'e':
            arg = optarg;
            __entanglement_buff_size = strtol(arg, &err_ptr, 0);
            if(arg == err_ptr)
            {
                fprintf(stderr, "Option -e requires a integer argument (decimal, hexadecimal or octal)\n");
                return 1;
            }
            break;
        case 'h':
            print_help_str();
            return 1;
            break;
        case '?':
            if (    optopt == 's'
                ||  optopt == 't'
                ||  optopt == 'g'
                ||  optopt == 'q'
                ||  optopt == 'a'
                ||  optopt == 'c'
                ||  optopt == 'm'
                ||  optopt == 'a'
                ||  optopt == 'o'
                ||  optopt == 'e')
            {
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            }
            else
            {
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            }
            return 1;
        default:
            abort ();
        }
    }
    return 0;
}

#define NUMINTS  (1000)
#define FILESIZE (NUMINTS * sizeof(int))

/**
 * Main executed function
 * @par test_num test to be executed
 * @return protgram return code
 */
int sub_main(unsigned test_num, unsigned seed)
{
    gc_log(LOG_INFO, "test start %u", (unsigned)clock());
    switch(test_num)
    {
        case TEST_SUBSYSTEM:
            gc_test();
            btree_test();
            clist_test();
            type_test();
            cdlist_test();
            tarray_test();
            large_structure_test();
            test_graphs();
            mem_dump(stdout);
            break;
        case TEST_SHORT_LIVED:
            //test_short_lived(__test_size, 100);
            test_short_lived(__test_size, __max_structure_size);
            break;
        case TEST_LONG_LIVED:
            //Testing on 8192 bytes of memory, worst case of 15 root trees of size 10 will take only 4800 bytes 
            //enough space for additional junk and replacements
            //test_long_lived(TEST_SIZE, 10, 15, 0.01); 
            test_long_lived(__test_size, __max_structure_size, __old_pool_size, __chance_to_replace); 
            break;
        //case TEST_LONG_LIVED_ALMOST_FULL:
            //Testing on 8192 bytes of memory, worst case of 25 root trees of size 10 will take maximum of 8000 bytes 
            //only little space for additional garbage - more collections
            //test_long_lived(TEST_SIZE, 10, 25, 0.01);
            //break;
        //case TEST_LONG_LIVED_NO_REPLACE:
            //After generating the root set will stay constant
            //test_long_lived(TEST_SIZE, 10, 15, 0.0);
            //break;
        case TEST_LARGE_STRUCTURE:
            //test_large_structure(TEST_SIZE, 10, 0.01, 100);
            test_large_structure(__test_size, __max_structure_size, __chance_to_replace, __entanglement_buff_size);
            break;
        case TEST_GRAPH:
            test_complete_graphs(__test_size, __max_structure_size, __old_pool_size, __chance_to_replace);
            break;
    }
    gc_log(LOG_INFO, "test end %u", (unsigned)clock());
    
    return 0;
}

#define __XAGG_CONCAT(name, value) #name " "
#define __XCONCAT_OPTIONS(xmacro) xmacro(__XAGG_CONCAT)

/**
 * Prints help string to stdout
 * @returns Number of characters printed
 */
int print_help_str()
{
    //printf("Usage: gc_benchmark [options]\nOptions:\n\t-s <number>\tSets the <number> as random number generator seed\n\t-t <name>\tSets the test to run. Available options are subsystem, short_lived, long_lived\n\t-g <name>\tSets used garbage collector. Available options are cheney\n");
    printf("Usage: gc_benchmark [options]\nOptions:\n");
    printf("\t-s <number>\tSets the <number> as random number generator seed\n");
    printf("\t-t <name>\tSets the test to run. Available options are: " __XCONCAT_OPTIONS(XTEST_TABLE) "\n");
    printf("\t-g <name>\tSets used garbage collector. Available options are: %s\n", __XCONCAT_OPTIONS(XCOLLECTOR_TABLE));
}