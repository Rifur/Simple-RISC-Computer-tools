#include <stdio.h>
#include <stdlib.h>
#include "decode.h"

unsigned int htonl(unsigned int x)
{				
	unsigned int y = x;

	return (unsigned int)	\
		((y & 0xff) << 24 |	\
		(y & 0xff00) << 8 |				\
		(y & 0xff0000) >> 8 |			\
		(y & 0xff000000) >> 24);		
}

void yyerror(char *msg)
{
	printf(msg);
	exit(0);
}

Symbol decode_opcode(unsigned int code)
{
	int op = code >> 27;
	int cond = code & 0x7;
	if(op == 8) {
		switch(cond) {
			case 0:
				return brnv;
			break;

			case 1:
				return br;
			break;

			case 2:
				return brzr;
			break;

			case 3:
				return brnz;
			break;

			case 4:
				return brpl;
			break;

			case 5:
				return brmi;
			break;
		}
	}

	return op;
}

void decode_typeI(unsigned int code, int *ra, int *c1)
{
	code = code & 0x7FFFFFF;
	*ra = code >> 22;
	*c1 = code & 0x3FFFFF;
}

void decode_typeII(unsigned int code, int *ra, int *rb, int *c2)
{
	code = code & 0x7FFFFFF;
	*ra = code >> 22;
	*rb = (code >> 17) & 0xF;
	*c2 = code & 0x1FFFF;
}

void decode_typeIII(unsigned int code, int *ra, int *rb, int *rc, int *c3)
{
	code = code & 0x7FFFFFF;
	*ra = code >> 22;
	*rb = (code >> 17) & 0xF;
	*rc = (code >> 12) & 0xF;
	*c3 = code & 0xFFF;
}