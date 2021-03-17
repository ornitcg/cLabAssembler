/*
  Group of helper utils functions that are related to assembler
*/

#ifndef _UTILS_ASSEMBLER_
#define _UTILS_ASSEMBLER_

#include "asStructsAndMacros.h"
#define EXT_AS ".as"


void printEnumName(Info name);/*DEBUG*/

/*
Checks if the current input line is an empty line or a comment line.
params:
char* line - The input line to check
returns: int YES/NO macro - if the line is empty or comment , returned YES, otherwise NO

*/
int toIgnore(char* line);

/*
Prints an error/warning message with a unified format
params:
STATUS* stat - for easy access to status info
char* message - ths message string to print
*/
void printMessageWithLocation(Info type, STATUS* stat, char* message);


/*
Calculates the value of distance from currentPos (address) to symbol address
params:
char* label - The symbol to calculate distance to
short currentPos - The address to calculate the distance from
STATUS* stat - for easy access to symbol table
returns:
short - the value of the calculated distance
*/
short diatanceToLabel(char* label, short currentPos, STATUS* stat);


/*
Builds the first word from the four fields of the parameter struct
params: FIRST_WORD type of struct that contains 4 fields
returns: a short int output composed of the 4 fields with the right shifting
assuming the input is correct
*/
short int buildFirstWord(short opcode, short funct, short src, short dest);


/*
Check if a string can ba a valid Number
params:
char* string - the string to check
returns: Info - Yes/No if found valid or not valid
assuming input has no heading or tailing whitespaces
*/
Info isValidAsNumber(char* string);


/*
Calculates the word for operand that is not a label.
params:
char* operand - the operand to calculate word for
Info addressType - The type of operand's addresing
returns:
short - the calculated word, for the specific type of operand,
in the smallest data type that contains a 12 bits word
*/
short wordValueOfNoneLabelOperand();


/*
Checks if a given string is a reserved words (as register names, commands and instructions)
char* string - the string to check
returns: Info - Yes/No if string is a reserved word or not
assuming input has no heading or tailing whitespaces
*/
Info isReservedWord(char* string);



/*
Checks if a given number is within range of signed 12 bits.
params:
int num - the number to check.
returns:
int - (macro)  YES if it is and NO otherwise
*/
int validInWordRange(int num);



/*
Checks if string is similar to name of any of the registers.
params:
char* string - the string to check;
returns: int - The number of register, string is similar to or NOT_FOUND otherwise
*/
int lookupRegister(char* string);



/*
Checks if string is acceptable by assembler as valid string data.
params:
char* string - the string to check.
returns: Info type - Yes if valid and No, otherwise
*/
Info isValidString(char* string);





#endif
