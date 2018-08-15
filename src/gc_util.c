 /**
  * @name gc_util.c
  * @author Mgr. Radomir Skrabal
  * This file contains implementation for the gc_util.h
  */
 
#include "gc_util.h"
#include <string.h>
#include <time.h>
#include <syslog.h>
#include <stdarg.h>
#include "gc_types.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "gc_constants.h"
#include "gc_cheney_base.h"
#include "benchmark.h"
#include "garbage_collector.h"
#include <inttypes.h>

#define LOG_DIR "../log/"
#define LOG_BUFF_SIZE 40485760 //1 MB
#define FILESIZE (LOG_BUFF_SIZE * sizeof(char))

#define LOG_NAME "log"

/**
 * Info about memory mapped logging file
 */
struct stat sb;

/**
 * Memory buffer for log messages
 */
char *log_buffer;
/**
 * memory mapped logging file
 */
char *log_file_mapping;
/**
 * Session identification string for logging
 */
char *session_ident = NULL;
/**
 * Position of write in memory mapped logging file
 */
uint64_t log_pos = 0;

/**
 * Initializes unique session identifier for the logger
 * @par test_num nuber of test carried out by the main
 * @par gc_num number of used garbage collector
 * @returns always 0
 */
int init_session_ident(int test_num, int gc_num)
{
    time_t t;
    char *tstr, *buff;
    const char *teststr, *gcstr; 
    const char *name = LOG_NAME;
    size_t len;
    
    t = time(&t);
    tstr = asctime(gmtime(&t));
    teststr = testnum_to_str(test_num);
    gcstr = gc_to_str(gc_num);
    
    buff = (char*)malloc(1000 * sizeof(char));
    
    len = sprintf(buff, "%s_%s_%s_%s", name, teststr, gcstr, tstr);
    
    str_remove_spaces(buff);
    str_replace_char(buff, ':', '_');
    str_replace_char(buff, '\n', '\0');
    
    buff[len] = '\0';
    
    session_ident = buff;
    
    return 0;
}
const char* get_session_ident()
{    
    return session_ident;
}
int cleanup_session_ident()
{
    if(session_ident != NULL)
    {
        free(session_ident);
    }
}

/**
 * Returns filepath to log file
 * @par buff buffer for filepath
 * @return Alwasy 0
 */
int get_logger_file_path(char *buff)
{
    sprintf(buff, "%s%s", LOG_DIR, get_session_ident());
    return 0;
}

/**
 * Inits the sysloger
 * @par test_num nuber of test carried out by the main
 * @par gc_num number of used garbage collector
 * @returns always 0
 */
int init_logger(int test_num, int gc_num)
{
//     int fd,
//         result;
//     char *filename;
    
    init_session_ident(test_num, gc_num);
    
    log_buffer = (char*)malloc(sizeof(char) * LOG_BUFF_SIZE);
    
//     filename = (char*)malloc(1000*sizeof(char));
//     filename[0] = '\0';
//     strcpy(filename, LOG_DIR);
//     strcat(filename, get_session_ident());
//     
//     fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
//     free(filename);
//     
//     if(fd == -1)
//     {
//         perror("open");
//         return 1;
//     }
//     
//     //Stretch the file size to the size of the (mmapped) array of ints
//     result = lseek(fd, FILESIZE-1, SEEK_SET);
//     if (result == -1) {
// 	close(fd);
// 	perror("Error calling lseek() to 'stretch' the file");
// 	exit(EXIT_FAILURE);
//     }
//     
//     /* Something needs to be written at the end of the file to
//      * have the file actually have the new size.
//      */
//     result = write(fd, "", 1);
//     if (result != 1) {
// 	close(fd);
// 	perror("Error writing last byte of the file");
// 	exit(EXIT_FAILURE);
//     }
//     
//     if(fstat(fd, &sb) == -1)
//     {
//         close(fd);
//         perror("fstat");
//         return 1;
//     }
//     if(!S_ISREG(sb.st_mode))
//     {
//         close(fd);
//         fprintf(stderr, "Not a file\n");
//         return 1;
//     }
//     
//     log_file_mapping = mmap(0, sb.st_size, PROT_WRITE, MAP_SHARED, fd, 0);
//     
//     if(log_file_mapping== MAP_FAILED)
//     {
//         close(fd);
//         perror("mmap");
//         return 1;
//     }
//     
//     close(fd);
    
    return 0;
}

/**
 * Reverses the string in place
 * @par str reversed string
 * @par len length of the string
 * @return length of the reversed string
 */
size_t strrev(char *str, size_t len)
{
    size_t  b = 0,
            e = len - 1;
    char tmp;
    
    while(b < e)
    {
        tmp = str[b];
        str[b] = str[e];
        str[e] = tmp;
        b++;
        e--;
    }
    
    return len;
}

/**
 * Converts the unsigned number into a string
 * @par num converted number
 * @par radix radix by which the number is converted
 * @par output dst allocated buffer to which the string is saved
 * @return length of the string
 */
size_t utoa(uint64_t num, uint64_t radix, char *dst)
{
    unsigned i = 0;
    uint64_t n = num;
    
    do
    {
        dst[i] = '0' + (n % radix);
        n /= radix;
        i++;
    }while(n > 0);
    dst[i] = '\0';
    
    return strrev(dst, i);;
}

/**
 * Converts the unsigned number into a decimal string
 * @par num converted number
 * @par dst allocated buffer to which the string is saved
 * @return length of the string
 */
size_t utoa10(uint64_t num, char *dst)
{
    return utoa(num, 10, dst);
}

/**
 * Converts the unsigned number into a hexadecimal string
 * @par num converted number
 * @par dst allocated buffer to which the string is saved
 * @return length of the string
 */
size_t utoa16(uint64_t num, char *dst)
{
    return utoa(num, 16, dst);
}

/**
 * Appends log buffer to log file
 * @return reseted position to log buffer
 */
int dump_log()
{
    FILE *file;
    char filename[1000];
    
    get_logger_file_path(filename);
    
    file = fopen(filename, "a");
    
    fprintf(file, "%s", log_buffer);
    
    fclose(file);
    
    log_pos = 0;
    
    return log_pos;
}

/**
 * Vrites timestamp into the buffer
 * @par dst destination buffer
 * @return length of the timestamp string
 */
size_t timestamp(char *dst)
{
//     size_t len, pos = 0;
//     struct timespec time;
//     char buff[1000];
//     
//     clock_gettime(CLOCK_MONOTONIC, &time);
//     
//     len = utoa10((uint64_t)time.tv_sec, &dst[pos]);
//     pos += len;    
//     dst[pos] = ' ';
//     pos++;
//     
//     len = utoa10((uint64_t)time.tv_nsec, &dst[pos]);
//     pos += len;
//     dst[pos] = '\0';
//     return pos;
    
    return sprintf(dst, "%u", (unsigned)rdtsc());    
}

/**
 * Logging interface
 * @par priority priotrity of the message
 * @par format format string for the message
 * @par ... arguments for the format string
 * @return always 0
 */
int gc_log(int priority, const char* format, ...)
{
    va_list args;
    char buff[1000];
    size_t len;

    va_start(args, format);    
    
    len = timestamp(buff);
    if(log_pos + len + 1 >= LOG_BUFF_SIZE)
    {
        dump_log();
    }
    strcpy(&log_buffer[log_pos], buff);
    log_pos += len;
    log_buffer[log_pos] = ' ';
    log_pos++;
    
    len = vsprintf(buff, format, args);
    if(log_pos + len + 1 >= LOG_BUFF_SIZE)
    {
        dump_log();
    }
    strcpy(&log_buffer[log_pos], buff);
    log_pos += len;
    
    log_buffer[log_pos] = '\n';
    log_pos++;
    
    return 0;
}

/**
 * Cleans up the logger subsystems
 * @returns 0 if everything went well, errno otherwise
 */
int cleanup_logger()
{
    /*if(msync(log_file_mapping, strlen("Some random message\n") + 1, MS_ASYNC) == -1)
    {
        perror("msync");
    }
    
    if(munmap(log_file_mapping, sb.st_size) == -1) 
    {
        perror("munmap");
        return 1;
    }     */ 
    
    dump_log();
    free(log_buffer);
    cleanup_session_ident();
    
    return 0;
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
 * Dumps information of allocated memory to the given file
 * @par file output file
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int mem_dump(FILE *file)
{
    block_t *block;
    int sum, tmp;
    
    fprintf(file, "Dumping all active memory blocks\n");
    for(block = gc_cheney_base_from_space; block < gc_cheney_base_remaining_block; block = next_block(block))
    {
        tmp = dump_block(file, block);
        if(tmp < 0)
        {
            break;
        }
        sum += tmp;
    }
    
    sum += fprintf(file, "Free space: %u bytes\n", (unsigned)gc_cheney_base_remaining_space());
    
    return sum;
}

/**
 * Dumps block in given semispace
 * @par file output file
 * @par semispace_ptr pointer to the begining of semispace
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int dump_semispace(FILE *file, void* semispace_ptr)
{
    block_t *block;
    int sum, tmp;
    
    fprintf(file, "Dumping semispace %p\n", semispace_ptr);
    for(block = semispace_ptr; block < (block_t*)gc_cheney_base_semispace_limit(semispace_ptr); block = next_block(block))
    {
        tmp = dump_block(file, block);
        if(tmp < 0)
        {
            break;
        }
        sum += tmp;
    }
    
    //sum += fprintf(file, "Free space: %u bytes\n", (unsigned)gc_cheney_base_remaining_space());
    
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
    if(block_is_array(block))
    {
        return dump_block_array(file, block);
    }
    switch(block_get_type(block))
    {
        case TYPE_UNDEFINED:
        case TYPE_INT:
        case TYPE_PTR:
        case TYPE_DOUBLE:
            return dump_block_atom(file, block);
        default:
            dump_block_struct(file, block);
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
    return fprintf(file, "Block ATOM %p, size %u, type %s\n", block, (unsigned int)block_get_size(block), typenum_to_string((int)block_get_type(block)));
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
    
    type_info_to_string(block_get_info(block), &buff);
    rtrn = fprintf(file, 
                   "Block STRUCT %p, size %u, type %s, struct info: %s\n", 
                   block, 
                   (unsigned int)block_get_size(block), 
                   typenum_to_string((int)block_get_type(block)),  
                   buff);
    
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
                        "Block ARRAY ATOM %p, size %u, array size %u, type %s\n", 
                        block, 
                        (unsigned int)block_get_size(block), 
                        (unsigned int)block_get_array_size(block), 
                        typenum_to_string((int)block_get_type(block)));
    }
    else
    {
        char *buff;
        type_info_to_string(block_get_info(block), &buff);
        
        rtrn = fprintf( file,
                        "Block ARRAY STRUCT %p, size %u, array size %u, type %s, struct info %s\n",
                        block,
                        (unsigned int)block_get_size(block),
                        (unsigned int)block_get_array_size(block),
                        typenum_to_string((int)block_get_type(block)),
                        buff);
        free(buff);
    }
    return rtrn;
}

/**
 * Creates a cstring containing readable information about type_info_t structure
 * @par info descriptor structure
 * @par buf::out output parameter for the string
 * @return If successful, the total number of characters written is returned otherwise, a negative number is returned.
 */
int type_info_to_string(type_info_t *info, char **buff)
{
    char *aux, *tmp;
    int i, rtrn;
    
    *buff = malloc(5000 * sizeof(char));
    aux = malloc(1000 * sizeof(char));
    tmp = malloc(10 * sizeof(char));
    
    aux[0] = '\0';
    for(i = 0; i < info->number_of_references; i++)
    {
//         sprintf(tmp, "%x(%s) ", (unsigned int)info->references[i].offset, typenum_to_string(info->references[i].type));
        sprintf(tmp, "%x ", (unsigned int)info->references[i].offset);
        
        strcat(aux, tmp);
    }
    
    rtrn = sprintf(*buff, "Struct Info %p, number_of_references %u, offsets: %s", info, (unsigned int)info->number_of_references, aux);
    free(aux);
    free(tmp);
    return rtrn;
}

/**
 * Removes spaces from string and replaces them with underscore '_'
 * @par str out mutated string
 * @return always 0
 */
int str_remove_spaces(char *str)
{
    return str_replace_char(str, ' ', '_');
}

/**
 * Removes char from string and replaces them 
 * @par str out mutated string
 * @par c replaced char
 * @par replace replacing chars
 * @return always 0
 */
int str_replace_char(char *str, char c, char replace)
{
    char *ptr;
    size_t len;
    
    ptr = str;
    len = strlen(str);
    
    while(1)
    {
        ptr = strchr(ptr, c);
        if(ptr == NULL || ptr > str + len)
            break;
        *ptr = replace;
        ptr++;
    }
    
    return 0;
}

/**
 * Fast time measurement
 * @return time mesuring integer
 */
static inline unsigned long long int rdtsc()
{
        unsigned long long int x;
        __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
        return x;
} 

/**
 * Prints each 8 bytes of memory as hexadecimal 64-bit number
 * @remark For debugging purposes
 * @par ptr pointer to memory
 * @par size length of memory to be printed in bytes
 * @returns always 0
 */
int lin_mem_dump(void *ptr, size_t size)
{
	size_t i;
	
	for(i = 0; i < size; i += sizeof(uint64_t))
	{
		printf("| %" PRIx64 " ", *((uint64_t*)((art_ptr_t)ptr + i)));
	}
	printf("\n");
	return 0;	
}
