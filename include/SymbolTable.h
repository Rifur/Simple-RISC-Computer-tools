#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__ 1

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "lang.h"
#include "stack.h"

#define SYMBOLTABLE_SPACE_UNIT	1024
#define NOT_FOUND	-1
#define DECLARED	-2
#define SEARCH_FULL_TABLE	-3
#define UPDATE_ADDR -4
#define UNDEFINED_ADDR -1


typedef struct {
	char *name;
	int	address;
} Label;

typedef struct 
{
	Label *labels;
	int     count;  // How many var names in this table.
	int     length; // Size of symbol table.
} LabelList;

void CreateLabelList(LabelList *sym);

int InsertLabel(LabelList *lblList, const char *name, int addr);

int SearchLabelByName(const char *name);

Label* SearchLabel(LabelList *lblList, int index);

void DumpLabelList(LabelList *lblList);

#endif