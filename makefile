simulator:
	gcc -O3 simulator.c decode.c lang.c -o simulator.exe -Iinclude

all:
	gcc -O3 assembler.c codegen.c lang.c stack.c SymbolTable.c strtok.c Instruction.c -o assembler.exe -Iinclude