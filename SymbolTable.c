#include "SymbolTable.h"

LabelList lblList;

//----------------------------------------------------------------------
// PRIVATE Function
/**
 * To expand label list
 */ 
static void ExpandLabelList(LabelList *tb)
{
	int     i;
	Label *newCapacity;
	newCapacity = (Label*)malloc(sizeof(Label)*(tb->length + SYMBOLTABLE_SPACE_UNIT));

	memcpy(newCapacity, tb->labels, sizeof(Label)*tb->length);
	free(tb->labels);

	tb->labels = newCapacity;
	tb->length += SYMBOLTABLE_SPACE_UNIT;
}



//----------------------------------------------------------------------
// Public Function

/**
 * Create a new symbol table. 
 */
void CreateLabelList(LabelList *sym)
{
	sym->count = 0;
	sym->length = SYMBOLTABLE_SPACE_UNIT;
	sym->labels = (Label *)malloc(sizeof(Label)*SYMBOLTABLE_SPACE_UNIT);
}

//----------------------------------------------------------------------

/**
 * Insert a Label info label list.
 */  
int InsertLabel(LabelList *lblList, const char *name, int addr)
{
	LabelList *tb = lblList;
	int length = strlen(name);
	int i;
	
	if((i = SearchLabelByName(name)) != NOT_FOUND) {
		if(tb->labels[i].address == UNDEFINED_ADDR && addr != UNDEFINED_ADDR) {
			tb->labels[i].address = addr;
			return i;
		} else if(tb->labels[i].address != UNDEFINED_ADDR && addr != UNDEFINED_ADDR) {
			printf("error: label %s: ", name);
			yyerror("Duplicate definition.\n");	
		} else {
			return i;
		}
	}
	
	// if havn't enough space, call ExpandLabelList()
	if(tb->count+1 == tb->length) {
		ExpandLabelList(tb);
	}
	
	Label *obj = &tb->labels[tb->count];
	obj->name = (char *)malloc(sizeof(char)*(length+1));
	sprintf(obj->name, "%s", name);
	obj->address = addr;
	
	return tb->count++;
}


/**
 * Search a label by name in label list.
 */
int SearchLabelByName(const char *name)
{
	LabelList *tb = &lblList;
	int i;
	
	for(i=tb->count-1; i>=0; --i) {
		if(!strcmp(tb->labels[i].name, name)) {
			return i;
		}
	}
	
	return NOT_FOUND;
}


/**
 * Search a label by index in label list.
 */
Label* SearchLabel(LabelList *lblList, int index)
{
	LabelList *tb = lblList;
	if(index > tb->count) {
		return NULL;
	} else {
		return &(tb->labels[index]);
	}
}

/**
 *	Dump label list.
 */
 void DumpLabelList(LabelList *lblList)
 {
 	LabelList *tb = lblList;
 	int i;
 	
 	printf("--\n==== LABELS Symbolic name ====\n");
 	for(i=0; i<tb->count; ++i) {
 		printf("#%d\tlabel: %s\t", i, tb->labels[i].name);
 		if(tb->labels[i].address == UNDEFINED_ADDR) {
 			printf("addr: UNDEFINED ADDRESS");
 		} else {
 			printf("addr: %06d (dec)", tb->labels[i].address);
 		}
 		printf("\n");
 	}
 }