#ifndef _UTILS_ASSEMBLER_
#define _UTILS_ASSEMBLER_

#include "asStructsAndMacros.h"
#define EXT_AS ".as"

/*
  Group of helper functions that are related to assembler
*/
void DO_SOMETHING();
info parseSymbol(char* symbol , char* line, STATUS* stat);
info parseData(char* line, info type, STATUS* stat);
info parseNumbersData(char* line, STATUS* stat);
info parseStringData(char* string, STATUS* stat);
info parseInstruction(char* instruction , char* line, STATUS* stat);
info parseCommand(char* command, char* line, STATUS* stat );
info parseExtern(char* line, STATUS* stat);
info parseCommandOperands(char* line, STATUS* stat);
/*
  Checks the possibility of the line to contain a label by searching for a ':'
  params:   char* line - the string to search for a label in
  returns:   1 if found, and 0 otherwise
*/
int containsLabelDef(char* line);

int lookupCommand(char* string);
int lookupRegister(char* string);
info isReservedWord(char* string);
info isValidAsSymbol(char* string, STATUS* stat);
int validSymbolChars(char* symbol); /*OK*/
int validInWordRange(short num);
Addressing isValidOperand(char* operand, STATUS* stat);
char* parseNext(char* line, char* delimiter);
int toIgnore(char* line);
int detectedCommand(char* str);
short int buildFirstWord(short opcode, short funct, short src, short dest);
#endif
