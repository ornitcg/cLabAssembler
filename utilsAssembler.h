#ifndef _UTILS_ASSEMBLER_
#define _UTILS_ASSEMBLER_

#include "asStructsAndMacros.h"
#define EXT_AS ".as"




/*
  Group of helper functions that are related to assembler
*/

short wordValueOfNoneLabelOperand();
Info isValidAsNumber(char* string);

int lookupRegister(char* string);
Info isReservedWord(char* string);
int validInWordRange(short num);
int toIgnore(char* line);

short int buildFirstWord(short opcode, short funct, short src, short dest);
void printEnumName(Info name);
Info isValidString(char* string);
void printErrorWithLocation(STATUS* stat, char* message);
#endif
