/**
 * @name gc_shared.c
 * @author Mgr. Radomir Skrabal
 * This file contains implementation of shared variables and functions from gc_shared.h
 */
#include "gc_shared.h"

#define DEBUG

/**
 * from space heap (active heap)
 */
block_t *from_space;

/**
 * to space heap (copy heap)
 */
block_t *to_space;

/**
 * Block containing remaining memory in active semispace
 */
block_t *remaining_block;

void *stack_top;
void *stack_bottom;
void *BBSstart;
void *BBSend;

/**
 * Gets next block of memory
 * @par block - pointer to the memory block
 * @return pointer to the next memory block
 */
block_t *next_block(block_t *block)
{
    void *ptr;
    
    ptr = block;
    
    return (block_t*)(ptr + sizeof(block_t) + block->size);
}

/**
 * Predicate for checking GC marks
 * @par block - pointer to the memory block
 * @return 1 if block is marked, 0 otherwise
 */
unsigned is_marked(block_t *block)
{
    return block != NULL && block->marked;
}

/**
 * Call to system for get raw memory
 * @par size size of requested memory in bytes
 * @return addr to start of allocated memory or NULL
 */
void *get_memory_primitive(size_t size)
{
    //Primitive call, should be a system call but not important now
    return malloc(size);
}

/**
 * Call to system for memory deallocation
 * @par ptr pointer to allocated memory
 * @return always 0
 */
int release_memory_primitive(void *ptr)
{
    //Same as get_memory_primitive, not important now
    free(ptr);
    return 0;
}

/**
 * Initializes a block of given size from a larger chunk of raw memory
 * @par chunk oroginal chunk of memory
 * @par size of the memory chunk
 * @return initialized block
 */
block_t *init_block_from_chunk(void *chunk, size_t size)
{
    block_t *block;
    
    block = chunk;
    block->size = size - sizeof(block_t);
    block->marked = 0;
    return block;
}

/** Returns allocated memory from a block
 * @par block initialized block of memory
 * @return ptr to the begining of user usable memory
 */
void *get_data_start(block_t *block)
{
    void *ptr = block;
    return ptr + sizeof(block_t);
}

/**
 * Returns pointer just after the allocated memory of a block
 * @par block block of a memory
 * @return pointer after the allocated memory block
 */
void *get_data_end(block_t *block)
{
    return (void*)next_block(block);
}

/**
 * Alings integer to the nearest greater <TODO násobek> of the size
 * @par size original size to be aligned
 * @return aligned size
 */
size_t align_size(size_t size)
{
    size_t mult;

    if(size % WORD_SIZE == 0)
    {
        return size;
    }
    
    mult = size / WORD_SIZE;
    
    return (mult + 1) * WORD_SIZE;
}

/**
 * Splits the block of memory
 * @par src original memory block (big) OUT: 
 * @par size size of new memory block
 * @return new memory block of given size or NULL if failed
 * @remark changes the address of src argument
 */
block_t *split_block(block_t **src, size_t size)
{
    void *ptr;
    size_t remaining, aligned_size;
    block_t *split;
    
    aligned_size = align_size(size);

    if(src == NULL || *src == NULL || aligned_size >= (*src)->size)
    {
        return NULL;
    }
    remaining = (*src)->size - aligned_size;

    ptr = get_data_start(*src);
    ptr = ptr + aligned_size;
            
    split = *src;
    *src = init_block_from_chunk(ptr, (*src)->size - aligned_size);
    split->size = aligned_size;
    split->marked = 0;
    
    return split;	
}

/**
 * Initializes the Garbage Collector objects
 * @return If everything went well 0, otherwise error code
 */
int gc_init()
{
    void *chunk;
#ifdef __gnu_linux__
    extern char __bss_start; // Provided by the linker.
#endif    

    chunk = get_memory_primitive(2*SEMISPACE_SIZE);
    if(chunk == NULL)
    {
        return 1;
    }
    to_space = init_block_from_chunk(chunk, 2*SEMISPACE_SIZE);
    if(to_space == NULL)
    {
        return 2;
    }
    from_space = split_block(&to_space, SEMISPACE_SIZE - sizeof(block_t));
    if(from_space == NULL)
    {
        return 3;
    }
    remaining_block = from_space;

#ifdef __gnu_linux__    
    stack_bottom = get_stack_bottom();
    
    BBSstart = (void*)&__bss_start;
    
    BBSend = sbrk(0);
    if(BBSend == (void*)-1)
    {
        fprintf(stderr, "Unable to init BBSend\n");
        return 4;
    }
#else
    BBSstart = NULL;
    BBSend = NULL;
    stack_bottom = NULL;
#endif    
    
    return 0;
}

/**
 * Returns pointer right after the end of semispace
 * @par semispace_ptr pointer to the start of a semispace
 * @return pointer right after end of semispace
 */
void* semispace_end(void *semispace_ptr)
{
    return semispace_ptr + SEMISPACE_SIZE;
}

/**
 * End user function for memory allocation
 * @par size memory to be allocated in bytes
 * @return pointer to allocated memory or NULL
 */
void *gc_malloc(size_t size)
{
    block_t *block;

    block = split_block(&remaining_block, size);

#ifdef DEBUG
    printf("malloc call: size: %d, allocated: size: %d, remaining block: size : %d\n", size, block->size, remaining_block->size);
#endif

    return get_data_start(block);	
}

/**
 * Predicates whetter the pointer points to the memory block
 * @par block block of a memory
 * @par ptr pointer
 * @return 1 if ptr points into block, 0 otherwise
 */
int is_pointer_to(block_t *block, void *ptr)
{
    void *start, *end;

    start = get_data_start(block);
    end = get_data_end(block);
    
    return ptr >= start && ptr < end;
}

/**
 * Marks memory block in the heap by pointers from given chunk
 * @par start start of a memory chunk (must be aligned)
 * @par end end of a memory chunk
 * @return 0 if everything went well, otherwise error code
 */
int mark_from_chunk(void *start, void *end)
{
    void *ptr, **it;
    block_t *block;

    for(it = start; it < (void**)end; it++)
    {
        ptr = *it;
#ifdef DEBUG
        printf("scanning ptr: %p\n", ptr);
#endif     
        for(block = from_space; block < (block_t*)semispace_end((void*)from_space); block = next_block(block))
        {
#ifdef DEBUG
            printf("scanning in block %p - %p\n", block, next_block(block));
#endif
            if(is_pointer_to(block, ptr))
            {
#ifdef DEBUG
                printf("Pointer %p found in block %p - %p\n", ptr, block, next_block(block));
#endif
                block->marked = 1;
            }
        }
    }
}

/**
 * Reads one line from opened file
 * @param file read file
 * @param buffer buffer where read characters are stored
 * @param max_site size of buffer
 * @returns number of read characters or -1 on error
 */
size_t read_line(FILE *file, char* buffer, size_t max_size)
{
    size_t buff_pos = 0;
    
    if(buffer == NULL || max_size == 0)
        return -1;
    
    do
    {
        if(feof(file))
            break;
        
        buffer[buff_pos] = fgetc(file);
        buff_pos++;
    }while( buff_pos < max_size 
            && buffer[buff_pos - 1] != '\n');
            
    if(buff_pos > 0 && buffer[buff_pos - 1] != '\0')
    {
        if(buff_pos >= max_size)
            return -1;
        buffer[buff_pos] = '\0';
        buff_pos++;
    }
    
    return buff_pos;
}

#define BUFF_SIZE 10000
#define STACK_WORD_LEN 9

/**
 * Opens the file /proc/self/maps and retrieves the line containing stack mapping
 * @param buffer buffer where the line is stored
 * @param max_size size of buffer
 * @returns length of the returned line
 */
int get_stack_line(char *buffer, size_t max_size)
{
    FILE *maps;
    size_t line_len;
    int    found = 0;
    
    maps = fopen("/proc/self/maps", "r");
    
    while(!feof(maps))
    {
        line_len = read_line(maps, buffer, max_size);
        if(line_len > STACK_WORD_LEN)
        {
            char *end = buffer + (line_len - STACK_WORD_LEN); 
            if(strcmp(end, "[stack]\n") == 0)
            {
                found = 1;
                break;
            }
        }
    }
    
    fclose(maps);
    
    if(!found)
        return -1;
    
    return line_len;
}

/**
 * Gets the bottom of the stack from /proc/self/maps file
 * @returns pointer to the bottom of the stack
 */
void *get_stack_bottom()
{
    char buffer[BUFF_SIZE];
    void *bottom;
    
    get_stack_line(buffer, BUFF_SIZE);
    
    sscanf(buffer, "%p", &bottom);
    
    return bottom;
}

/**
 * Carries out memory marking phase of Garbage Collector
 * @return 0 if everything went well, otherwise error code
 */
int gc_mark()
{
    block_t *block;
    
    //Mark from heap
    for(block = from_space; block < (block_t*)semispace_end((void*)from_space); block = next_block(block))
    {
        mark_from_chunk(get_data_start(block), get_data_end(block));
    }
    
    //Mark from stack
    REFRESH_STACK_TOP
    mark_from_chunk(stack_bottom, stack_top);
    
    //Mark from BSS
    mark_from_chunk(BBSstart, BBSend);
}

/**
 * Carries out the "sweep" part of the algorithm
 * @par src space from which data will be copied (from_space)
 * @par dst space to which memory blocks will be evacuated (to_space)
 * @return 0 if everything went well, error code otherwise
 */
int gc_copy(block_t *src, block_t *dst)
{
  
}