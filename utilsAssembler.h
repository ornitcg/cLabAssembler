#ifndef _UTILS_ASSEMBLER_
#define _UTILS_ASSEMBLER_

#include "asStructsAndMacros.h"
#define EXT_AS ".as"

enum Info {No, Yes, Ok, Source, Target, Code , Data, String,  Entry, Extern  , Empty ,  Error = -1 , Immediate = '0', Direct ='1', Relative ='2' , Register ='3'};
typedef enum Info Info;

/*enum Addressing {Immediate = '0', Direct ='1', Relative ='2' , Register ='3' , Null = 'E'};
typedef enum Addressing Addressing;*/

/*
  Group of helper functions that are related to assembler
*/
void DO_SOMETHING();
short wordValueOfOperand();

int lookupRegister(char* string);
Info isReservedWord(char* string);
int validInWordRange(short num);
int toIgnore(char* line);
short int buildFirstWord(short opcode, short funct, short src, short dest);

#endif
