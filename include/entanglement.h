/**
 * @name entanglement.h
 * @author Mgr. Radomir Skrabal
 * This file contains headers and definitions for entanglement large data structure 
 */

#ifndef ENTANGLEMENT_H
#define ENTANGLEMENT_H
#include <stdint.h>
#include "gc_types.h"

#define ENTANGLEMENT_SLOTS(x)   x(uint64_t, val1) \
                                x(void*, ref1) \
                                x(void*, ref2) \
                                x(void*, ref3) \
                                x(uint64_t, val2) \
                                x(void*, ref4) \
                                x(uint64_t, val3) \
                                x(void*, ref5) \
                                x(void*, ref6) \
                                x(void*, ref7) \
                                x(void*, ref8) \
                                x(void*, ref9) \
                                x(uint64_t, val4) \
                                x(uint64_t, val5) \
                                x(uint64_t, val6) \
                                x(void*, ref10) \
                                x(void*, ref11) \
                                x(void*, ref12) \
                                x(uint64_t, val7) \
                                x(void*, ref13) \
                                x(uint64_t, val8) \
                                x(void*, ref14) \
                                x(uint64_t, val9) \
                                x(void*, ref15) \
                                x(void*, ref16) \
                                x(void*, ref17) \
                                x(uint64_t, val10) \
                                x(void*, ref18) \
                                x(void*, ref19) \
                                x(void*, ref20)
                                
#define ENTANGLEMENT_STRUCT(type, name) type name;

typedef struct
{
    ENTANGLEMENT_SLOTS(ENTANGLEMENT_STRUCT)
} entanglement_t;

int entanglement_make_descriptor(type_info_t *info);

int entanglement_init(entanglement_t *e);
void *rand_ptr();

#endif