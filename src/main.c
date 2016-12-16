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

#define TEST_SUBSYSTEM 0
#define TEST_SHORD_LIVED 1
#define TEST_LONG_LIVED 2

#define GC_CLEANUP_ERR 0x1
#define TYPE_CLEANUP_ERR 0x2
#define LOG_CLEANUP_ERR 0x4

/**
 * Initializes subsystems
 * @par rand_seed seed used for random generation
 * @returns 0 if initializaion went Ok, otherwise error code
 */
int init(unsigned rand_seed);
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
    test_num = TEST_LONG_LIVED; //TODO
    used_gc = CHENEY_GC;
    
    //Set by arguments
    err_msg = parse_args(argc, argv, &seed, &test_num, &used_gc);
    if(err_msg != 0)
    {
        return err_msg;
    }
        
    err_msg = init(seed);
    if(err_msg != 0)
    {
        return err_msg;
    }
    
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
 * @returns 0 if initializaion went Ok, otherwise error code
 */
int init(unsigned rand_seed)
{
    int err_msg;
    
    err_msg = init_logger();
    
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
    
    err_msg = cleanup_session_ident();
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
    char *cvalue = NULL;
    int index;
    int c;
    
    opterr = 0;

    while ((c = getopt (argc, argv, "s:t:g:h")) != -1)
    {
        switch (c)
        {
        case 's':
            *seed = atoi(optarg);
            break;
        case 't':
            //TODO
            break;
        case 'g':
            //TODO
            break;
        case 'h':
            //TODO
            break;
        case '?':
          /*  if (optopt == 's')
                fprintf (stderr, "Option -%s requires an argument.\n", optopt);
            else if (optopt == 't')
                fprintf (stderr, "Option -%t requires an argument.\n", optopt);
            else if (optopt == 'g')
                fprintf (stderr, "Option -%g requires an argument.\n", optopt);            
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);*/
            return 1;
        default:
            abort ();
        }
    }
    return 0;
}

/**
 * Main executed function
 * @par test_num test to be executed
 * @return protgram return code
 */
int sub_main(unsigned test_num, unsigned seed)
{
    switch(test_num)
    {
        case TEST_SUBSYSTEM:
            gc_test();
            btree_test();
            clist_test();
            type_test();
            cdlist_test();
            mem_dump(stdout);
            break;
        case TEST_SHORD_LIVED:
            test_short_lived(10000, 100);
            break;
        case TEST_LONG_LIVED:
            test_long_lived(10000, 100, 250, 0.01);
            break;
    }
    
    printf("Log signature:\n\n%s\n", get_session_ident());
    gc_log(LOG_INFO, "r: %d", seed);
    
    return 0;
}