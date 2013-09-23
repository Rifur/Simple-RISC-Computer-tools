#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__ 1

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "codegen.h"
#include "SymbolTable.h"
#include "lang.h"
#include "stack.h"

#define SYMBOLTABLE_SPACE_UNIT	1024
#define NOT_FOUND	-1
#define DECLARED	-2
#define SEARCH_FULL_TABLE	-3

#define NO_LABEL -1

/*	Instruction	*/
typedef struct {
	Symbol opcode;
	unsigned int machineCode;
	InstructionType type;
	int labelIdx;
} Instruction;



typedef struct {
	Instruction *ins;
	int     count;  // How many var names in this table.
	int     length; // Size of symbol table.
} InstructionProgram;

void CreateInstructionProgram(InstructionProgram *sym);

int InsertInstruction(InstructionProgram *sym, Symbol op, unsigned int code, int lblIdx, InstructionType type);

void ResolveSymbolicName(InstructionProgram *sym, LabelList *lbl);

void DumpInstructionProgram(InstructionProgram *sym);

void OutputInstructionProgram(FILE *fdst, InstructionProgram *sym);

#endif