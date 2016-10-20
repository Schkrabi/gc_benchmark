/**
 * @name gc_shared.h
 * @author Mgr. Radomir Skrabal
 * 
 * This file contains definitions of shared structures and functions among the garbage collectors
 * 
 * TODO Currently kind of mess in here. Mixed with copying collector. Will move the code later (probably when will be building custom collector)
 */
#ifndef GC_SHARED_H
#define GC_SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Header for a logical memory block
 * @remark Convention size slot is available free space in the block.
 * Ergo size of the whole block is sizeof(block_t) + size bytes
 */
typedef struct
{
    size_t size;
    unsigned marked; //Wasting space, but should not be important for this implementation
    /**
     * Forwarding address for copying algorthm
     */
    void *forward;
} block_t;

/**
 * Gets next block of memory
 * @par block - pointer to the memory block
 * @return pointer to the next memory block
 */
block_t *next_block(block_t *block);

/**
 * Predicate for checking GC marks
 * @par block - pointer to the memory block
 * @return 1 if block is marked, 0 otherwise
 */
unsigned is_marked(block_t *block);

/**
 * Call to system for get raw memory
 * @par size size of requested memory in bytes
 * @return addr to start of allocated memory or NULL
 */
void *get_memory_primitive(size_t size);

/**
 * Call to system for memory deallocation
 * @par ptr pointer to allocated memory
 * @return always 0
 */
int release_memory_primitive(void *ptr);

/**
 * Initializes a block of given size from a larger chunk of raw memory
 * @par pointer to the original chunk
 * @par size size of a new block
 * @return new start ptr of remaining raw memory chunk
 */
block_t *init_block_from_chunk(void *chunk, size_t size);

/**
 * Returns allocated memory from a block
 * @par block initialized block of memory
 * @return ptr to the begining of user usable memory
 */
void *get_data_start(block_t *block);

/**
 * Returns pointer just after the allocated memory of a block
 * @par block block of a memory
 * @return pointer after the allocated memory block
 */
void *get_data_end(block_t *block);

/**
 * Size of a word in the system
 */
#define WORD_SIZE sizeof(void*)

/**
 * Alings integer to the nearest greater <TODO násobek> of the size
 * @par size original size to be aligned
 * @return aligned size
 */
size_t align_size(size_t size);

/**
 * Splits the block of memory
 * @par src original memory block (big)
 * @par size size of new memory block
 * @return new memory block of given size or NULL if failed
 * @remark reduces the size of src block
 */
block_t *split_block(block_t **src, size_t size);

/**
 * Size of the semispace
 */
#define SEMISPACE_SIZE 1024

/**
 * from space heap (active heap)
 */
extern block_t *from_space;
/**
 * to space heap (copy heap)
 */
extern block_t *to_space;

/**
 * Block containing remaining memory in active semispace
 */
extern block_t *remaining_block;

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_init();

/**
 * Returns pointer right after the end of semispace
 * @par semispace_ptr pointer to the start of a semispace
 * @return pointer right after end of semispace
 */
void* semispace_end(void *semispace_ptr);

/**
 * End user function for memory allocation
 * @par size memory to be allocated in bytes
 * @return pointer to allocated memory or NULL
 */
void *gc_malloc(size_t size);

/**
 * Predicates whetter the pointer points to the memory block
 * @par block block of a memory
 * @par ptr pointer
 * @return 1 if ptr points into block, 0 otherwise
 */
int is_pointer_to(block_t *block, void *ptr);

/**
 * Marks memory block in the heap by pointers from given chunk
 * @par start start of a memory chunk (must be aligned)
 * @par end end of a memory chunk
 * @return 0 if everything went well, otherwise error code
 */
int mark_from_chunk(void *start, void *end);

/**
 * Reads one line from opened file
 * @param file read file
 * @param buffer buffer where read characters are stored
 * @param max_site size of buffer
 * @returns number of read characters or -1 on error
 */
size_t read_line(FILE *file, char* buffer, size_t max_size);

/**
 * Opens the file /proc/self/maps and retrieves the line containing stack mapping
 * @param buffer buffer where the line is stored
 * @param max_size size of buffer
 * @returns length of the returned line
 */
int get_stack_line(char *buffer, size_t max_size);

/**
 * Gets the bottom of the stack from /proc/self/maps file
 * @returns pointer to the bottom of the stack
 */
void *get_stack_bottom();

/**
 * Carries out memory marking phase of Garbage Collector
 * @return 0 if everything went well, otherwise error code
 */
int gc_mark();

/**
 * Bottom of a stack
 */
extern void *stack_bottom;
/**
 * Top of a stack
 */
extern void *stack_top;

/**
 * Start of the uninitialized data segment
 */
extern void *BBSstart;
/**
 * End of the uninitilized data segment
 */
extern void *BBSend;

/**
 * Macro for filling the stack_top variable with proper value
 * @remark Currently working only oon GNU Linux 
 */
#ifdef __gnu_linux__
#define REFRESH_STACK_TOP asm volatile ("mov %%rbp, %0" : "=r" (stack_top));
#else
#define REFRESH_STACK_TOP
#endif

/**
 * Save current stack pointer as stack bottom
 * @remark Currently only for GNU Linux
 */ 
#ifdef __gnu_linux__
#define SET_STACK_BOTTOM asm volatile ("mov %%rbp, %0" : "=r" (stack_bottom));
#else
#define SAVE_STACK_BOTTOM
#endif

/**
 * Carries out the "sweep" part of the algorithm
 * @par src space from which data will be copied (from_space)
 * @par dst space to which memory blocks will be evacuated (to_space)
 * @return 0 if everything went well, error code otherwise
 */
 int gc_collect();
 
 void *get_forwarding_addr(void *ptr, block_t* src, block_t *dst);
 int gc_copy_chunk(void *start, void *end);
 int gc_swich_semispaces();

#endif

