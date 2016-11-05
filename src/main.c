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

#define TEST_SIZE 3
#define NO_COLLECTOR 0
#define CHENEY_GC 1

int mem_dump(FILE *file);
int dump_block(FILE *file, block_t *block);
int dump_block_atom(FILE *file, block_t *block);
int dump_block_struct(FILE *file, block_t *block);
int dump_block_array(FILE *file, block_t *block);
int struct_info_to_string(struct_info_t *info, char **buff);

int mem_dump(FILE *file)
{
    block_t *block;
    
    fprintf(file, "Dumping all active memory blocks\n");
    for(block = from_space; block < (block_t*)semispace_end((void*)from_space); block = next_block(block))
    {
        dump_block(file, block);
    }
}

int dump_block(FILE *file, block_t *block)
{
    switch(block_get_type(block))
    {
        case MEM_TYPE_ATOM:
            dump_block_atom(file, block);
            break;
        case MEM_TYPE_STRUCT:
            dump_block_struct(file, block);
            break;
        case MEM_TYPE_ARRAY:
            dump_block_array(file, block);
            break;
    }
}

int dump_block_atom(FILE *file, block_t *block)
{
    fprintf(file, "Block ATOM %p, size %u, is pointer %d\n", block, (unsigned int)block_get_size(block), block_atom_is_ptr(block));
}

int dump_block_struct(FILE *file, block_t *block)
{
    char *buff;
    
    struct_info_to_string(block_get_info(block), &buff);
    fprintf(file, "Block STRUCT %p, size %u, struct info: %s\n", block, (unsigned int)block_get_size(block), buff);
    
    free(buff);
}

int dump_block_array(FILE *file, block_t *block)
{
    if(!block_is_struct_block(block))
    {
        fprintf(    file, 
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
        
        fprintf(    file,
                    "Block ARRAY STRUCT %p, size %u, array size %u, struct info %s\n",
                    block,
                    (unsigned int)block_get_size(block),
                    (unsigned int)block_get_array_size(block),
                    buff);
        free(buff);
    }
}

int struct_info_to_string(struct_info_t *info, char **buff)
{
    char *aux, *tmp;
    int i;
    
    *buff = malloc(5000 * sizeof(char));
    aux = malloc(1000 * sizeof(char));
    tmp = malloc(10 * sizeof(char));
    
    aux[0] = '\0';
    for(i = 0; i < info->number_of_references; i++)
    {
        sprintf(tmp, "%x ", (unsigned int)info->offsets[i]);
        
        strcat(aux, tmp);
    }
    
    sprintf(*buff, "Struct Info %p, number_of_references %u, offsets: %s", info, (unsigned int)info->number_of_references, aux);
    free(aux);
    free(tmp);
    return 0;
}

typedef struct 
{
  int value;
  void *ptr1, *ptr2;
} test_struct_t;

/**
 * Main executed function
 */
int sub_main(int argc, char *argv[])
{
    struct_info_t *struct_info;
    test_struct_t test_instance;
    void **ptr_src1, **ptr_src2;
    int *atom_test, *array_test;
    test_struct_t *struct_test, *struct_array_test, *ptr_src3, *ptr_src4;
    int i;
    
    struct_info = (struct_info_t*)malloc(sizeof(struct_info_t));
    
    struct_info[0].number_of_references = 2;
    struct_info[0].offsets = malloc(2*sizeof(unsigned long));
    
    struct_info[0].offsets[0] = (unsigned long)&test_instance.ptr1 - (unsigned long)&test_instance;
    struct_info[0].offsets[1] = (unsigned long)&test_instance.ptr2 - (unsigned long)&test_instance;
    struct_info[0].struct_size = sizeof(test_struct_t);
    
    printf("Initial memory dump:\n");
    mem_dump(stdout);
    
    atom_test = gc_malloc_atom(sizeof(int), 0);
    struct_test = gc_malloc_struct(struct_info);
    array_test = gc_malloc_array_of_atoms(TEST_SIZE, sizeof(int), 0);
    struct_array_test = gc_malloc_array_of_struct(TEST_SIZE, struct_info);
    
    ptr_src1 = (void**)gc_malloc_atom(sizeof(void*), 1);
    *ptr_src1 = atom_test;
    
    ptr_src2 = (void**)gc_malloc_array_of_atoms(TEST_SIZE, sizeof(void*), 1);
    ptr_src2[0] = atom_test;
    ptr_src2[1] = struct_test;
    ptr_src2[2] = NULL;
    
    ptr_src3 = (test_struct_t*)gc_malloc_struct(struct_info);
    ptr_src3->ptr1 = NULL;
    ptr_src3->ptr2 = struct_test;
    
    ptr_src4 = (test_struct_t*)gc_malloc_array_of_struct(TEST_SIZE, struct_info);
    ptr_src4[0].ptr1 = array_test;
    ptr_src4[0].ptr2 = atom_test;
    ptr_src4[1].ptr1 = NULL;
    ptr_src4[1].ptr2 = atom_test;
    ptr_src4[2].ptr1 = struct_test;
    ptr_src4[2].ptr2 = NULL;
    
    printf("\nMemory dump after allocation:\n");
    mem_dump(stdout);
    
    gc_roots_count = 4;
    gc_roots = (void**)malloc(gc_roots_count*sizeof(void*));
    
    gc_roots[0] = ptr_src1;
    gc_roots[1] = ptr_src2;
    gc_roots[2] = ptr_src3;
    gc_roots[3] = ptr_src4;
    
    gc_collect();
    
    printf("\nMemory dump after collection:\n");
    mem_dump(stdout);
    
    return 0;
}

/**
 * Main entrypoint Garbage collector initial setup
 */
int main(int argc, char *argv[])
{
    int err_msg;
    
    SET_STACK_BOTTOM
    used_gc = CHENEY_GC;
    
    err_msg = gc_init();
    if(err_msg != 0)
    {
            printf("Initialization error, code %d\n", err_msg);
    }
    
    return sub_main(argc, argv);
}