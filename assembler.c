#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <libgen.h>
#include "strtok.h"
#include "SymbolTable.h"
#include "assembler.h"
#include "lang.h"
#include "Instruction.h"
#include "codegen.h"

#define LINE_MAX 256
#define TOKEN_MAX 8

static char line[LINE_MAX];
static char token[TOKEN_MAX][LINE_MAX];
static int	idxToken = 0;
static char *ptoken = NULL;
static char *ptr = NULL;
static int	lineNum = 0;

extern InstructionProgram insProgram;
extern LabelList lblList;

void __nop(void)
{
	unsigned int code = 0;
	printf("nop\n");
	code = codegen_typeI(0, nop, 0, 0);
	InsertInstruction(&insProgram, nop, code, NO_LABEL, TypeI);
}

void __ld(char *sra, char *soffset)
{
	int ra = 0;
	int offset = 0;

	unsigned int code;

	if(sra[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow ld instruction.");
	}

	if(sscanf(sra, "r%d", &ra) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number.");
	} else if (sscanf(soffset, "%d", &offset) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need immediate value.");
	}

	printf("r%d %d\n", ra, offset);

	code = codegen_typeII(0, ld, ra, 0, offset);
	InsertInstruction(&insProgram, ld, code, NO_LABEL, TypeII);
}

void __ld_indirect(char *sra, char *soffset, char *srb)
{
	int ra = 0;
	int rb = 0;
	int offset = 0;
	
	unsigned int code;

	if(sra[0] != 'r' || srb[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow add instruction.");
	}

	if(sscanf(sra, "r%d", &ra) != 1 || sscanf(srb, "r%d", &rb) != 1 || sscanf(soffset, "%d", &offset) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number or immediate value.");
	}

	printf("r%d %d (r%d)\n", ra, offset, rb);


	code = codegen_typeII(0, ld, ra, rb, offset);
	InsertInstruction(&insProgram, la, code, NO_LABEL, TypeII);
}

void __st(char *sra, char *sc2)
{
	int ra = 0;
	int c2 = 0;
	int withLabel = NO_LABEL;
	
	unsigned int code;

	if(sra[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow st instruction.");
	}

	if(sscanf(sra, "r%d", &ra) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number or immediate value.");
	}

	if(sscanf(sc2, "%d", &c2) != 1) {
		withLabel = InsertLabel(&lblList, sc2, UNDEFINED_ADDR);
	}

	printf("r%d %s\n", ra, sc2);

	code = codegen_typeII(0, st, ra, 0, c2);
	InsertInstruction(&insProgram, st, code, withLabel, TypeII);
}

void __la(char *sra, char *sc2)
{
	int ra = 0;
	int c2 = 0;
	int withLabel = NO_LABEL;
	
	unsigned int code;

	if(sra[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow la instruction.");
	}

	if(sscanf(sra, "r%d", &ra) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number or immediate value.");
	}

	if(sscanf(sc2, "%d", &c2) != 1) {
		withLabel = InsertLabel(&lblList, sc2, UNDEFINED_ADDR);
	}

	printf("r%d %s\n", ra, sc2);

	code = codegen_typeII(0, la, ra, 0, c2);
	InsertInstruction(&insProgram, la, code, withLabel, TypeII);
}

void __la_indirect(char *sra, char *sc2, char *srb)
{
	int ra = 0;
	int rb = 0;
	int c2 = 0;

	unsigned int code;

	if(sra[0] != 'r' || srb[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow add instruction.");
	}

	if(sscanf(sra, "r%d", &ra) != 1 || sscanf(srb, "r%d", &rb) != 1 || sscanf(sc2, "%d", &c2) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number or immediate value.");
	}

	printf("r%d %d (r%d)\n", ra, c2, rb);

	code = codegen_typeII(0, la, ra, rb, c2);
	InsertInstruction(&insProgram, la, code, NO_LABEL, TypeII);
}

void __add(char *sra, char *srb, char *src)
{
	int ra = 0;
	int rb = 0;
	int rc = 0;

	unsigned int code;

	if(sra[0] != 'r' || srb[0] != 'r' || src[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow add instruction.");
	}

	if(sscanf(sra, "r%d", &ra) != 1 || sscanf(srb, "r%d", &rb) != 1 || sscanf(src, "r%d", &rc) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number");
	}

	printf("r%d r%d r%d\n", ra, rb, rc);

	code = codegen_typeIII(0, add, ra, rb, rc, 0);
	InsertInstruction(&insProgram, add, code, NO_LABEL, TypeIII);
}

void __sub(char *sra, char *srb, char *src)
{
	int ra = 0;
	int rb = 0;
	int rc = 0;

	unsigned int code;

	if(sra[0] != 'r' || srb[0] != 'r' || src[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow sub instruction.");
	}

	if(sscanf(sra, "r%d", &ra) != 1 || sscanf(srb, "r%d", &rb) != 1 || sscanf(src, "r%d", &rc) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number");
	}

	printf("r%d r%d r%d\n", ra, rb, rc);

	code = codegen_typeIII(0, sub, ra, rb, rc, 0);
	InsertInstruction(&insProgram, sub, code, NO_LABEL, TypeIII);
}

void __br(char *srb)
{
	int rb = 0;

	unsigned int code;

	if(srb[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow brzr instruction.");
	}

	if(sscanf(srb, "r%d", &rb) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number");
	}

	printf("r%d\n", rb);

	code = codegen_typeIII(0, br, 0, rb, 0, 1);
	InsertInstruction(&insProgram, br, code, NO_LABEL, TypeIII);
}

void __brl(char *sra, char *srb)
{
	int ra = 0;
	int rb = 0;

	unsigned int code;

	if(sra[0] != 'r' || srb[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow brl instruction.");
	}

	if(sscanf(sra, "r%d", &ra) != 1 || sscanf(srb, "r%d", &rb) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number");
	}

	printf("r%d r%d\n", ra, rb);

	code = codegen_typeIII(0, brl, ra, rb, 0, 1);
	InsertInstruction(&insProgram, brl, code, NO_LABEL, TypeIII);
}

void __brzr(char *srb, char *src)
{
	int rb = 0;
	int rc = 0;

	unsigned int code;

	if(srb[0] != 'r' || src[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow brzr instruction.");
	}

	if(sscanf(srb, "r%d", &rb) != 1 || sscanf(src, "r%d", &rc) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number");
	}

	printf("r%d r%d\n", rb, rc);

	code = codegen_typeIII(0, br, 0, rb, rc, 2);
	InsertInstruction(&insProgram, brzr, code, NO_LABEL, TypeIII);
}

void __brnz(char *srb, char *src)
{
	int rb = 0;
	int rc = 0;

	unsigned int code;

	if(srb[0] != 'r' || src[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow brnz instruction.");
	}

	if(sscanf(srb, "r%d", &rb) != 1 || sscanf(src, "r%d", &rc) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number");
	}

	printf("r%d r%d\n", rb, rc);

	code = codegen_typeIII(0, br, 0, rb, rc, 3);
	InsertInstruction(&insProgram, brzr, code, NO_LABEL, TypeIII);
}

void __brpl(char *srb, char *src)
{
	int rb = 0;
	int rc = 0;

	unsigned int code;

	if(srb[0] != 'r' || src[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow brpl instruction.");
	}

	if(sscanf(srb, "r%d", &rb) != 1 || sscanf(src, "r%d", &rc) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number");
	}

	printf("r%d r%d\n", rb, rc);

	code = codegen_typeIII(0, br, 0, rb, rc, 4);
	InsertInstruction(&insProgram, brzr, code, NO_LABEL, TypeIII);
}

void __brmi(char *srb, char *src)
{
	int rb = 0;
	int rc = 0;

	unsigned int code;

	if(srb[0] != 'r' || src[0] != 'r') {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: register shall follow brmi instruction.");
	}

	if(sscanf(srb, "r%d", &rb) != 1 || sscanf(src, "r%d", &rc) != 1) {
		printf("line: %d: ", lineNum);
		yyerror("syntax error: need register number");
	}

	printf("r%d r%d\n", rb, rc);

	code = codegen_typeIII(0, br, 0, rb, rc, 5);
	InsertInstruction(&insProgram, brzr, code, NO_LABEL, TypeIII);
}

void __stop()
{
	unsigned int code;
	code = codegen_typeIII(0, stop, 0, 0, 0, 0);
	InsertInstruction(&insProgram, stop, code, NO_LABEL, TypeIII);
}

void statement(char token[TOKEN_MAX][LINE_MAX], int length)
{
	int i;
	for(i=0; op[i].symbol != -1; ++i) {
		if(strcmp(token[0], op[i].keyword) == 0) {
			break;
		}
	}

	if(op[i].symbol == -1) {
		printf("line: %d: %s\t", lineNum, token[0]);
		yyerror("undefined symbol");
	}

	switch(op[i].symbol) {
		case nop:
			__nop();
		break;
		case ld:
			printf("<ld>\t");
			(length < 4) ? 
				__ld(token[1], token[2]) : 
				__ld_indirect(token[1], token[2], token[3]);

		break;
		case st:
			printf("<st>\t");
			__st(token[1], token[2]);
		break;
		case la:
			printf("<la>\t");
			(length < 4) ? 
				__la(token[1], token[2]) : 
				__la_indirect(token[1], token[2], token[3]);

		break;
		case add:
			printf("<add>\t");
			__add(token[1], token[2], token[3]);
		break;

		case sub:
			printf("<sub>\t");
			__sub(token[1], token[2], token[3]);
		break;

		case br:
			printf("<br>\t");
			__br(token[1]);
		break;

		case brl:
			printf("<brl>\t");
			__brl(token[1], token[2]);
		break;

		case brzr:
			printf("<brzr>\t");
			(length > 2) ?
				__brzr(token[1], token[2]) :
				yyerror("syntax error");
		break;

		case brnz:
			printf("<brnz>\t");
			(length > 2) ?
				__brnz(token[1], token[2]) :
				yyerror("syntax error");
		break;

		case brpl:
			printf("<brpl>\t");
			(length > 2) ?
				__brpl(token[1], token[2]) :
				yyerror("syntax error");
		break;

		case brmi:
			printf("<brmi>\t");
			(length > 2) ?
				__brmi(token[1], token[2]) :
				yyerror("syntax error");
		break;

		case stop:
			printf("<stop>\n");
			__stop();
		break;

	}
}


int main(int argc, char *argv[])
{
	int	i;
	char *pline;
	char fname[512];

	FILE *fsrc, *fdst;
	fsrc = fopen(argv[1], "r");
	assert(fsrc != NULL);

	sprintf(fname, "%s/%s.obj", dirname(argv[1]), basename(argv[1]));
	fdst = fopen(fname, "wb");

	CreateInstructionProgram(&insProgram);
	CreateLabelList(&lblList);

	while(fgets(line, LINE_MAX, fsrc) != NULL) {

		idxToken = 0;
		lineNum += 1;
		pline = line;
		ptr = NULL;

		while((ptoken = strtok_r(pline, " ,\n\t()", &ptr)) != NULL) {

			// ignore comment
			if(ptr!= NULL && ptr[0] == ';') {
				ptr[0] = '\0';
			}

			// record label
			i = strlen(ptoken);
			if(ptoken[i-1] == ':') {
				ptoken[i-1] = '\0';
				InsertLabel(&lblList, ptoken, insProgram.count*4);
			} else {
			// assembly token
				strcpy(token[idxToken++], ptoken);	
			}

			pline = NULL;
		}

		if(idxToken > 0) {
			statement(token, idxToken);
		}

	}

	ResolveSymbolicName(&insProgram, &lblList);
	DumpLabelList(&lblList);
	DumpInstructionProgram(&insProgram);

	OutputInstructionProgram(fdst, &insProgram);

	fclose(fsrc);
	fclose(fdst);

	return 0;
}
