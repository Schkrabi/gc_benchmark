 /**
  * @name gc_util.c
  * @author Mgr. Radomir Skrabal
  * This file contains implementation for the gc_util.h
  */
 
#include "gc_util.h"
#include <string.h>
 
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
 * Dumps information of allocated memory to the given file
 * @par file output file
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int mem_dump(FILE *file)
{
    block_t *block;
    int sum, tmp;
    
    fprintf(file, "Dumping all active memory blocks\n");
    for(block = from_space; block < (block_t*)semispace_end((void*)from_space); block = next_block(block))
    {
        tmp = dump_block(file, block);
        if(tmp < 0)
        {
            break;
        }
        sum += tmp;
    }
    return sum;
}

/**
 * Dumps information about block of memory to the given file
 * @par file output file
 * @par block a memory block
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int dump_block(FILE *file, block_t *block)
{
    switch(block_get_type(block))
    {
        case MEM_TYPE_ATOM:
            return dump_block_atom(file, block);
        case MEM_TYPE_STRUCT:
            return dump_block_struct(file, block);
        case MEM_TYPE_ARRAY:
            return dump_block_array(file, block);
    }
}

/**
 * Dumps a block of memory containign atomic value into a given file
 * @par file output file
 * @par block memory block of type MEM_TYPE_ATOM
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int dump_block_atom(FILE *file, block_t *block)
{
    return fprintf(file, "Block ATOM %p, size %u, is pointer %d\n", block, (unsigned int)block_get_size(block), block_atom_is_ptr(block));
}

/**
 * Dumps a block of memory containign strucutre into a given file
 * @par file output file
 * @par block memory block of type MEM_TYPE_STRUCT
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int dump_block_struct(FILE *file, block_t *block)
{
    char *buff;
    int rtrn;
    
    struct_info_to_string(block_get_info(block), &buff);
    rtrn = fprintf(file, "Block STRUCT %p, size %u, struct info: %s\n", block, (unsigned int)block_get_size(block), buff);
    
    free(buff);
    return rtrn;
}

/**
 * Dumps a block of memory containign an array into a file
 * @par file output file
 * @par block memory block of type MEM_TYPE_ARRAY
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int dump_block_array(FILE *file, block_t *block)
{
    int rtrn;
    if(!block_is_struct_block(block))
    {
        rtrn = fprintf( file, 
                        "Block ARRAY ATOM %p, size %u, array size %u, is pointer %d\n", 
                        block, 
                        (unsigned int)block_get_size(block), 
                        (unsigned int)block_get_array_size(block), 
                        block_atom_is_ptr(block));
    }
    else
    {
        char *buff;
        struct_info_to_string(block->info, &buff);
        
        rtrn = fprintf( file,
                        "Block ARRAY STRUCT %p, size %u, array size %u, struct info %s\n",
                        block,
                        (unsigned int)block_get_size(block),
                        (unsigned int)block_get_array_size(block),
                        buff);
        free(buff);
    }
    return rtrn;
}

/**
 * Creates a cstring containing readable information about struct_info_t structure
 * @par info descriptor structure
 * @par buf::out output parameter for the string
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int struct_info_to_string(struct_info_t *info, char **buff)
{
    char *aux, *tmp;
    int i, rtrn;
    
    *buff = malloc(5000 * sizeof(char));
    aux = malloc(1000 * sizeof(char));
    tmp = malloc(10 * sizeof(char));
    
    aux[0] = '\0';
    for(i = 0; i < info->number_of_references; i++)
    {
        sprintf(tmp, "%x ", (unsigned int)info->offsets[i]);
        
        strcat(aux, tmp);
    }
    
    rtrn = sprintf(*buff, "Struct Info %p, number_of_references %u, offsets: %s", info, (unsigned int)info->number_of_references, aux);
    free(aux);
    free(tmp);
    return rtrn;
}
