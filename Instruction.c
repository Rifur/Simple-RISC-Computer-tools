#include "Instruction.h"

InstructionProgram insProgram;


//----------------------------------------------------------------------
// PRIVATE Function
/**
 * To expand label list
 */ 
static void ExpandInstructionProgram(InstructionProgram *tb)
{
	int     i;
	Instruction *newCapacity;
	newCapacity = (Instruction*)malloc(sizeof(Instruction)*(tb->length + SYMBOLTABLE_SPACE_UNIT));

	memcpy(newCapacity, tb->ins, sizeof(Instruction)*tb->length);
	free(tb->ins);

	tb->ins = newCapacity;
	tb->length += SYMBOLTABLE_SPACE_UNIT;
}



//----------------------------------------------------------------------
// Public Function

/**
 * Create a new instruction program. 
 */
void CreateInstructionProgram(InstructionProgram *sym)
{
	sym->count = 0;
	sym->length = SYMBOLTABLE_SPACE_UNIT;
	sym->ins = (Instruction *)malloc(sizeof(Instruction)*SYMBOLTABLE_SPACE_UNIT);
}

//----------------------------------------------------------------------

/**
 * Insert a Label info label list.
 */  
int InsertInstruction(InstructionProgram *sym, Symbol op, unsigned int code, int lblIdx, InstructionType type)
{
	InstructionProgram *tb = sym;
	int i;
	
	// if havn't enough space, call ExpandLabelList()
	if(tb->count+1 >= tb->length) {
		ExpandInstructionProgram(tb);
	}
	
	Instruction *obj = &tb->ins[tb->count];
	obj->opcode = op;
	obj->machineCode = code;
	obj->labelIdx = lblIdx;
	obj->type = type;
	
	return tb->count++;
}

//----------------------------------------------------------------------
/**
 *	Resovle symbolic name for memory locations.
 */
void ResolveSymbolicName(InstructionProgram *sym, LabelList *lbl)
{
	int i;
	Label *b;
	unsigned int code;

	for(i=0; i<sym->count; ++i) {
		if(sym->ins[i].labelIdx == NO_LABEL) {
			continue;
		}

		b = SearchLabel(lbl, sym->ins[i].labelIdx);
		if(b == NULL) {
			yyerror("error: Unknown label.");
		}

		if(b->address == UNDEFINED_ADDR) {
			printf("error: No address label: %s", b->name);
			yyerror("\n");
		}

		code = sym->ins[i].machineCode;
		switch(sym->ins[i].type) {
			case TypeI:
				code = codegen_typeI(sym->ins[i].machineCode, 0, 0, b->address);
			break;

			case TypeII:
				code = codegen_typeII(sym->ins[i].machineCode, 0, 0, 0, b->address);
			break;

			case TypeIII:
				code = codegen_typeIII(sym->ins[i].machineCode, 0, 0, 0, 0, b->address);
			break;
		}
		sym->ins[i].machineCode = code;

	}
}

//----------------------------------------------------------------------

/**
 *	Dump Instruction Program.
 */
void DumpInstructionProgram(InstructionProgram *sym)
{
	int i;
	printf("--\n==== DUMP MACHINE CODE ====\n");
	for(i=0; i<sym->count; ++i) {
		printf("%10X type %d", sym->ins[i].machineCode, sym->ins[i].type);
		if(sym->ins[i].labelIdx != NO_LABEL) {
			printf("\tLabel index: #%d", sym->ins[i].labelIdx);
		}
		printf("\n");
	}
}

//----------------------------------------------------------------------
/**
 *	Output Instruction Program.
 */
void OutputInstructionProgram(FILE *fdst, InstructionProgram *sym)
{
	int i;
	unsigned tmp;

	if(fdst == NULL) {
		return;
	}

	for(i=0; i<sym->count; ++i) {
		tmp = htonl(sym->ins[i].machineCode);
		fwrite(&tmp, sizeof(unsigned int), 1, fdst);
	}
}