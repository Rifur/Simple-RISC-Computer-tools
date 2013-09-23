#ifndef __DECODE_H__
#define __DECODE_H__ 1

#include "lang.h"

Symbol decode_opcode(unsigned int code);

void decode_typeI(unsigned int code, int *ra, int *c1);
void decode_typeII(unsigned int code, int *ra, int *rb, int *c2);
void decode_typeIII(unsigned int code, int *ra, int *rb, int *rc, int *c3);

#endif