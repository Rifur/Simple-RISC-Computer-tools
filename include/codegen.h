#ifndef __CODEGEN_H__
#define __CODEGEN_H__ 1

void yyerror(char *msg);
unsigned int codegen_typeI(unsigned int code, int opcode, int ra, int c1);
unsigned int codegen_typeII(unsigned int code, int opcode, int ra, int rb, int c2);
unsigned int codegen_typeIII(unsigned int code, int opcode, int ra, int rb, int rc, int c3);
unsigned int htonl(unsigned int x);


#endif