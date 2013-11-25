#ifndef __LANG_H__
#define __LANG_H__ 1

typedef enum {
	nop=0,
	ld=1,
	ldr,
	st,
	str,
	la,
	lar,

	br=8,
	brl=9,
	brzr=1000,	// those instruction's opcode are denote by br or brl, and
	brnz,		// using Cond c3<2..0> to distinguish it. 
	brpl,		// For the parsing convenient, just set from 1000 (greater number), not magic.
	brmi,
	brnv,

	add=12,
	addi,
	sub,
	neg,
	and=20,
	andi,
	or,
	ori,
	not,

	stop=31,
	org,
	equ,
	dc,
	dw

} Symbol;


typedef enum {
	TypeI,
	TypeII,
	TypeIII,
	Pseudotype
} InstructionType;

/*	OpCode	*/
typedef struct {
	char	*keyword;
	Symbol	symbol;
	InstructionType type;
} OpCode;


extern OpCode op [];


#endif