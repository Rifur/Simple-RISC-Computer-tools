#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "simulator.h"
#include "codegen.h"
#include "lang.h"


static int ra, rb, rc, cx;
static unsigned int reg[32];
static int insSize;
static unsigned int *mem;

static int RUN = 0;
static int STEP = 0;
static int pc = 0;

static FILE *fsrc = NULL;
static int breakpoint [32] = {[0 ... 31] = -1};
static int bp_p = 0;



void __nop__()
{
	// Do nothing.
}

void __ld__(int ra, int rb, int c2)
{
	int tmp = c2 + reg[rb];
	printf("R[%d]=%d\t-> %d <- M[%d] + R[%d]=%d\n", ra, reg[ra], tmp, c2, rb, reg[rb]);
	reg[ra] = mem[tmp/4];
}

void __st__(int ra, int rb, int c2)
{
	int tmp = c2+reg[rb];
	printf("M[%d + R[%d]=%d]=%d\t-> %d\n", c2, rb, reg[rb], mem[tmp], reg[ra]);
	mem[tmp/4] = reg[ra];
}

void __la__(int ra, int c2)
{
	printf("R[%d]=%d\t -> %d\n", ra, reg[ra], c2);
	reg[ra] = c2;
}

void __lar__(int ra, int c1)
{
	int tmp = pc*4 + c1;
	printf("R[%d]=%d\t -> %d\n", ra, reg[ra], tmp);
	reg[ra] = tmp;
}

void __add__(int ra, int rb, int rc)
{
	int tmp = reg[rb] + reg[rc];
	printf("R[%d]=%d\t-> %d <- R[%d]=%d + R[%d]=%d\n", ra, reg[ra], tmp, rb, reg[rb], rc, reg[rc]);
	reg[ra] = tmp;
}

void __addi__(int ra, int rb, int c2)
{
	int tmp = reg[rb] + c2;
	printf("R[%d]=%d\t-> %d <- R[%d]=%d + %d\n", ra, reg[ra], tmp, rb, reg[rb], c2);
	reg[ra] = tmp;
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

void __brnz__(int rb, int rc)
{
	if(reg[rc] != 0) {
		printf("branch to R[%d]=%d\n", rb, reg[rb]);
		pc = reg[rb] / 4;
	} else {
		printf("cont.\n");
	}
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
	pc = 0;
}


void DumpProgram(unsigned int *addr, unsigned int *ins, int insSize)
{
	int i;
	printf("==== MACHINE CODE ====\n");
	for(i=0; i<insSize; ++i) {
		printf("%08d\t%08x\n", addr[i], ins[i]);
	}
	printf("--\n");
}

static int 
Load(char *fname)
{
	unsigned int tmpIns = 0;
	unsigned int tmpAddr = 0;

	fsrc = fopen(fname, "rb");
	if(fsrc != NULL) {
		// load binary file
		while(fscanf(fsrc, "%d%x", &tmpAddr, &tmpIns) == 2) {
			mem[tmpAddr/4] = tmpIns;
		}
		return 1;
	} else {
		return 0;
	}
}

static void 
Help()
{
	printf("load <file-name> -- Load obj file into debugger.\n\n"
			"breakpoint <address> -- Set breakpoint at the address.\n"
			"\tbreakpoint = b\n\n"
			"clear <address> -- Clear breakpoint at the address\n\n"
			"show R<#reg> -- Show register's content.\n"
			"\te.g. show r10\n\n"
			"show reg -- Show all register's content.\n\n"
			"show M<#mem> -- Show memory content from #mem. unit: 32*4 bytes\n"
			"\te.g. show m200 will display from address 200 to 328.\n\n"
			"help: Show the help messages.\n");
}

static void
Clear_Breakpoint(int i)
{
	int j;
	for(j=0; j<bp_p; ++j) {
		if(breakpoint[j] == i) {
			printf("Clear breakpoint at %d, address %d\n", j, breakpoint[j]);
			for( ; j<bp_p - 1 ; ++j) {
				breakpoint[j] = breakpoint[j+1];
			}
			bp_p -= 1;
			breakpoint[bp_p] = -1;
		}
	}
}

static void
Show_Reg(int i)
{
	if(i >= 0 && i < 32) {
		printf("R[%d] = %d\n", i, reg[i]);
	} else if(i == 32) {
		for(i=0; i<32; ++i) {
			printf("R[%d]=%d\t", i, reg[i]);
		}
		printf("\n");
	}
}

static void
Show_Mem(int i)
{
	int j;
	for(j=i; j<i+32; ++j) {
		printf("%010d\t%02X %02X %02X %02X\n", j*4, mem[j]>>24, (mem[j]>>16)&0xFF, (mem[j]>>8)&0xFF, mem[j]&0xFF);
	}
}

static void
UnknownCommand()
{
	printf("Unknown command. If you need manual, please type 'help'.\n");
}

int main(int argc, char *argv[])
{	
	unsigned int tmpIns = 0;
	unsigned int tmpAddr = 0;
	int idx = 0;
	int opi = 0;
	Symbol opc;

	int i = 0;
	int j = 0;

	char command [128] = {0};

	mem = (unsigned int *)calloc(MEM_SIZE, sizeof(int));

	if(argc > 1) {
		if(Load(argv[1])) {
			printf("Load %s\n", argv[1]);
		}
	}

while(1) {
		printf("(sim)$ ");
		scanf("%s", command);
		if(!strcmp(command, "load")) {
			scanf("%s", command);
			if(Load(command)) {
				printf("Load %s\n", command);
			} else {

			}
			RUN = 0;
			STEP = 0;
			pc = 0;
			continue;
		} else if(!strcmp(command, "help")) {
			Help();
			continue;
		} else if(!strcmp(command, "breakpoint") || !strcmp(command, "b")) {
			if(scanf("%d", &i)) {
				breakpoint[bp_p] = i;
				bp_p += 1;

				printf("Breakpoint at %010d\n", i);
			}
			continue;
		} else if(!strcmp(command, "clear")) {
			if(scanf("%d", &i)) {
				Clear_Breakpoint(i);
			}
			continue;
		} else if(!strcmp(command, "run")) {
			RUN = 1;
			STEP = 0;
		} else if(!strcmp(command, "step")) {
			RUN = 1;
			STEP = 1;
		} else if(!strcmp(command, "show")) {
			if(scanf("%s", command)) {
				if(command[0] == 'R') {
					sscanf(&command[1], "%d", &i);
					Show_Reg(i);
				} else if(command[0] == 'M') {
					sscanf(&command[1], "%d", &i);
					Show_Mem(i);
				} else if(!strcmp(command, "reg")) {
					Show_Reg(32);
				}
			}
			continue;
		}
		else {
			UnknownCommand();
			continue;
		}
	


	// get program size
	//fseek(fsrc, 0, SEEK_END);
	//insSize = ftell(fsrc);
	//fseek(fsrc, 0, SEEK_SET);

	if(RUN == 0) {
		continue;
	}

	while(RUN != 0) {
		opc = decode_opcode(mem[pc]);

		// find opcode
		for(j=0; op[j].symbol != opc && op[j].symbol != -1; ++j) {
			
		}

		if(op[j].symbol == -1) {
			printf("error: %d: %08x\t", idx, mem[pc]);
			yyerror("unknown instruction.");
		}


		switch(op[j].type) {
			case TypeI:
				decode_typeI(mem[pc], &ra, &cx);
				//printf("r%d %d\n", ra, cx);
			break;

			case TypeII:
				decode_typeII(mem[pc], &ra, &rb, &cx);
				//printf("r%d r%d %d\n", ra, rb, cx);
			break;

			case TypeIII:
				decode_typeIII(mem[pc], &ra, &rb, &rc, &cx);
				//printf("r%d r%d r%d %d\n", ra, rb, rc, cx);
			break;
		}

		// Breakpoint
		for(i=0; i<bp_p; ++i) {
			if(breakpoint[i] == pc*4) {
				printf("Breakpoint %d at address %010d:\n", i, pc*4);
				break;
			}
		}

		// stop at breakpoint
		if(i != bp_p) {
			RUN = 0;
			STEP = 1;
			//continue;
		}

		// Step running
		if(STEP) {
			RUN = 0;
		}


		pc = pc + 1;

		switch(op[j].symbol) {
			case nop:
				//printf("<nop>\n");
				//__nop__();
				continue;
			break;

			case ld:
				printf("<ld>\t");
				__ld__(ra, rb, cx);
			break;

			case st:
				printf("<st>\t");
				__st__(ra, rb, cx);
			break;
			
			case la:
				printf("<la>\t");
				__la__(ra, cx);
			break;

			case lar:
				printf("<lar>\t");
				__lar__(ra, cx);
			break;

			case add:
				printf("<add>\t");
				__add__(ra, rb, rc);
			break;

			case addi:
				printf("<addi>\t");
				__addi__(ra, rb, cx);
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
				__brnz__(rb, rc);
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

}

	free(mem); mem = NULL;

	fclose(fsrc);

	return 0;
}