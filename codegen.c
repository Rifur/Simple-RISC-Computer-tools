#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"

void yyerror(char *msg)
{
	printf(msg);
	exit(0);
}

unsigned int codegen_typeI(unsigned int code, int opcode, int ra, int c1)
{
	return code | (opcode << 27) | ((ra & 0xF) << 22) | ((c1 & 0x3FFFFF));
}

unsigned int codegen_typeII(unsigned int code, int opcode, int ra, int rb, int c2)
{
	return code | (opcode << 27) | ((ra & 0xF) << 22) | ((rb & 0xF) << 17) | ((c2 & 0x1FFFF));
}

unsigned int codegen_typeIII(unsigned int code, int opcode, int ra, int rb, int rc, int c3)
{
	return code | (opcode << 27) | ((ra & 0xF) << 22) | ((rb & 0xF) << 17) | ((rc & 0xF) << 12) | ((c3 & 0xFFF));
}

unsigned int htonl(unsigned int x)
{				
	unsigned int y = x;

	return (unsigned int)	\
		((y & 0xff) << 24 |	\
		(y & 0xff00) << 8 |				\
		(y & 0xff0000) >> 8 |			\
		(y & 0xff000000) >> 24);		
}