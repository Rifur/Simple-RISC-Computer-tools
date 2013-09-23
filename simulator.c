#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "simulator.h"
#include "codegen.h"
#include "lang.h"

unsigned int *ins;
static int insSize;

static int ra, rb, rc, cx;
static unsigned int reg[32];
static int mem[1024];

static int RUN = 1;

void __nop__()
{
	// Do nothing.
}

void __ld__(int ra, int c2)
{
	printf("\tR[%d]=%d\t <- M[%d]\n", ra, reg[ra], c2, mem[c2]);
	reg[ra] = mem[c2];
	printf("\tR[%d]=%d\n", ra, reg[ra]);
}

void __st__(int ra, int c2)
{
	printf("\tM[%d]=%d\t <- R[%d]=%d\n", c2, mem[c2], ra, reg[ra]);
	mem[c2] = reg[ra];
	printf("\tM[%d]=%d\t\n", c2, mem[c2]);
}

void __la__(int ra, int c2)
{
	printf("\tR[%d]=%d\t <- %d\n", ra, reg[ra], c2);
	reg[ra] = mem[c2];
	printf("\tR[%d]=%d\n", ra, reg[ra]);
}

void __add__(int ra, int rb, int rc)
{
	printf("\tR[%d]=%d\t  R[%d]=%d   R[%d]=%d\n", ra, reg[ra], rb, reg[rb], rc, reg[rc]);
	reg[ra] = reg[rb] + reg[rc];
	printf("\tR[%d]=%d\t= R[%d]=%d + R[%d]=%d\n", ra, reg[ra], rb, reg[rb], rc, reg[rc]);
}

void __sub__()
{

}

void __and__()
{

}

void __or__()
{

}

void __not__()
{

}

void __neg__()
{

}

void __br__()
{

}

void __brl__()
{

}

void __brzr__()
{

}

void __brnz__()
{

}

void __brpl__()
{

}

void __brmi__()
{

}

void __stop__()
{
	RUN = 0;
}


void DumpProgram(unsigned int *ins, int insSize)
{
	int i;
	printf("==== MACHINE CODE ====\n");
	for(i=0; i<insSize; ++i) {
		printf("%08x\n", ins[i]);
	}
	printf("--\n");
}

int main(int argc, char *argv[])
{	
	unsigned int tmp;
	int idx = 0;
	int pc, j, opi;
	Symbol opc;

	FILE *fsrc = fopen(argv[1], "rb");
	assert(fsrc != NULL);

	// get program size
	fseek(fsrc, 0, SEEK_END);
	insSize = ftell(fsrc);
	ins = (unsigned int *)malloc(insSize);
	fseek(fsrc, 0, SEEK_SET);


	while(fread(&tmp, sizeof(unsigned int), 1, fsrc)) {
		ins[idx] = htonl(tmp);
		idx += 1;
	}

	DumpProgram(ins, idx);


	for(pc=0; pc<idx && RUN!=0; ++pc) {
		opc = decode_opcode(ins[pc]);

		for(j=0; op[j].symbol != opc && op[j].symbol != -1; ++j) {
			// Do nothing.
		}

		if(op[j].symbol == -1) {
			printf("error: %d: %08x\t", idx, ins[pc]);
			yyerror("unknown instruction.");
		}

		//printf("%s ", op[j].keyword);

		switch(op[j].type) {
			case TypeI:
				decode_typeI(ins[pc], &ra, &cx);
				//printf("r%d %d\n", ra, cx);
			break;

			case TypeII:
				decode_typeII(ins[pc], &ra, &rb, &cx);
				//printf("r%d r%d %d\n", ra, rb, cx);
			break;

			case TypeIII:
				decode_typeIII(ins[pc], &ra, &rb, &rc, &cx);
				//printf("r%d r%d r%d %d\n", ra, rb, rc, cx);
			break;
		}

		switch(op[j].symbol) {
			case nop:
				printf("<nop>\n");
				__nop__();
			break;

			case ld:
				printf("<ld>\n");
				__ld__(ra, cx);
			break;

			case st:
				printf("<st>\n");
				__st__(ra, cx);
			break;
			
			case la:
				printf("<la>\n");
				__la__(ra, cx);
			break;

			case add:
				printf("<add>\n");
				__add__(ra, rb, rc);
			break;

			case sub:
				printf("<sub>\t");
				__sub__();
			break;

			case br:
				printf("<br>\t");
				__br__();
			break;

			case brl:
				printf("<brl>\t");
				__brl__();
			break;

			case brzr:
				printf("<brzr>\t");
				__brzr__();
			break;

			case brnz:
				printf("<brnz>\t");
				__brnz__();
			break;

			case brpl:
				printf("<brpl>\t");
				__brpl__();
			break;

			case brmi:
				printf("<brmi>\t");
				__brmi__();
			break;

			case stop:
				printf("<stop>\n");
				__stop__();
			break;

		}
	}


	free(ins); ins = NULL;
	fclose(fsrc);

	return 0;
}