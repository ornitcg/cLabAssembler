#ifndef _UTILS_ASSEMBLER_
#define _UTILS_ASSEMBLER_

#include "asStructsAndMacros.h"
#define EXT_AS ".as"

/*Info contains a bunch of word that are helpfull for readability*/
enum Info {A = 'A' , R = 'R' ,E = 'E' , Immediate = '0', Direct ='1', Relative ='2' , Register ='3' ,Error = -1, No =0, Yes=1, Ok=2, Source=3, Dest=4, Code =5, Data=6, String=7,  Entry=8 , Extern=9 , Empty = '_'  , FillLater ='?' };

typedef enum Info Info;


/*
  Group of helper functions that are related to assembler
*/
void DO_SOMETHING();
short wordValueOfNoneLabelOperand();
Info isValidAsNumber(char* string);

int lookupRegister(char* string);
Info isReservedWord(char* string);
int validInWordRange(short num);
int toIgnore(char* line);

short int buildFirstWord(short opcode, short funct, short src, short dest);
void printEnumName(Info name);
Info isValidString(char* string);

#endif
