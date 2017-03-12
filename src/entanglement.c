/**
 * @name entanglement.c
 * @author Mgr. Radomir Skrabal
 * This file contains code for entanglement_t large data structure
 */

#include "entanglement.h"
#include "gc_util.h"
#include "garbage_collector.h"

#define ENT_PTR_NUMS(x) x(1) x(2) x(3) x(4) x(5) x(6) x(7) x(8) x(9) x(10) x(11) x(12) x(13) x(14) x(15) x(16) x(17) x(18) x(19) x(20)
#define ENT_MAKE_DESC(num) info->offsets[num - 1] = (unsigned long)((art_ptr_t)&measure.ref ## num - (art_ptr_t)&measure);

/**
 * Creates and sets the entanglement structure descriptor
 * @par info pointer to the initialized descriptor structure
 * @return 1 if everything went well 0 otherwise
 */
int entanglement_make_descriptor(type_info_t *info)
{
    //TODO Obsolete
    entanglement_t measure;
    
    info->size = sizeof(entanglement_t);
    info->number_of_references = 0;
    info->references = NULL;
//     info->number_of_references = 20;
//     info->offsets = (unsigned long*)malloc(20 * sizeof(unsigned long));
//     
//     ENT_PTR_NUMS(ENT_MAKE_DESC)
}

#define ENT_FILL_REFS(num) e->ref ## num = rand_ptr();

int entanglement_init(entanglement_t *e)
{
    ENT_PTR_NUMS(ENT_FILL_REFS)
}

void *rand_ptr()
{
    unsigned ticket;
    void *rtrn;
    
    ticket = rand()%100;
    
    if(ticket == 0)
    {
        rtrn = (void*)gc_malloc(entanglement_t);
        entanglement_init((entanglement_t*)rtrn);
    }
    else
    {
        if(ticket == 1)
        {
            unsigned size = rand()%3;
            unsigned i;
            entanglement_t *e;
            e = gc_malloc_array(entanglement_t, size);
            rtrn = (void*)e;
            for(i = 0; i < size; i++)
            {
//                 entanglement_init(&(e[i]));
            }
//            rtrn = NULL;
        }
        else 
        {
            if(ticket < 52)
            {
                unsigned size = 1 + rand()%29;
                rtrn = (void*)gc_malloc_array(int, size);
            }
            else
            {
                rtrn = (void*)gc_malloc(int);
            }
        }
    }
    
    return rtrn;
}
