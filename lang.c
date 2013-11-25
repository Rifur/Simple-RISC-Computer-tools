#include "lang.h"

OpCode op [] = {
	{"nop", nop},
	{"ld",	ld, TypeII},
	{"ldr",	ldr, TypeI},
	{"st",	st, TypeII},
	{"str",	str, TypeI},
	{"la",	la, TypeII},
	{"lar",	lar, TypeI},

	{"br", br, TypeIII},		// uncondition branch
	{"brl", brl, TypeIII},		// brl, save pc to ra when branch
	{"brzr", brzr, TypeIII},	// branch if rc == 0
	{"brnz", brnz, TypeIII},	// branch if rc != 0
	{"brpl", brpl, TypeIII},	// branch if rc >= 0
	{"brmi", brmi, TypeIII},	// branch if rc <  0

	{"add",	add, TypeIII},
	{"addi",addi, TypeII},
	{"sub",	sub, TypeIII},
	{"neg",	neg, TypeII},
	{"and",	and, TypeIII},
	{"andi",andi, TypeII},
	{"or",	or, TypeIII},
	{"ori",	ori, TypeII},
	{"not",	not, TypeIII},

	{"stop", stop, TypeIII},
	{".org", org, Pseudotype},
	{".equ", equ, Pseudotype},
	{".dc", dc, Pseudotype},
	{".dw", dw, Pseudotype},

	{"--", -1, TypeIII}
};