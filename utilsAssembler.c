/*
Author: Ornit Cohen Gindi
Group of helper utils functions that are related to assembler
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asScanners.h"
#include "linkedList.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"


/*
Checks if the current input line is an empty line or a comment line.
params:
char* line - The input line to check
returns: int YES/NO macro - if the line is empty or comment , returned YES, otherwise NO

*/
int toIgnore(char* line){
    if (strlen(line) == 0 || line[0] == COMMENT_IDENTIFIER_INT)
        return YES;
    return NO;
}



/*
Prints an error/warning message with a unified format
params:
STATUS* stat - for easy access to status info
char* message - ths message string to print
*/
void printMessageWithLocation( Info type ,STATUS*  stat , char* message ){
    if (type == Error)
        printf("[ERROR][File:%s Line:%d] %s\n", stat -> fileName , stat->lineNumber, message);
    else if (type == Warning)
        printf("[WARNING][File:%s Line:%d] %s\n", stat -> fileName , stat->lineNumber, message);


}




/*
Calculates the value of distance from currentPos (address) to symbol address
params:
char* label - The symbol to calculate distance to
short currentPos - The address to calculate the distance from
STATUS* stat - for easy access to symbol table
returns:
short - the value of the calculated distance
*/
short diatanceToLabel(char* label, short currentPos, STATUS* stat){
    SYMBOL* sym = lookupSymbol(stat -> symbolTable,  label);
    short symPos = sym -> address;
    return symPos - currentPos;
}



/*
Builds the first word from the four fields of the parameter struct
params: FIRST_WORD type of struct that contains 4 fields
returns: a short int output composed of the 4 fields with the right shifting
assuming the input is correct
*/
short  buildFirstWord(short opcode, short funct, short src, short dest){
  signed short  res;
  if (src >0 ) src -=48; /*if not zero; received as char*/
  if (dest >0 )dest -=48; /*if not zero; received as char*/

  src = src << 2;
  funct = funct << 4;
  opcode = opcode << 8 ;
  res =  opcode | funct | src | dest ;
  return res;
}




/*
Check if a string can ba a valid Number
params:
char* string - the string to check
returns: Info - Yes/No if found valid or not valid
assuming input has no heading or tailing whitespaces
*/
Info isValidAsNumber(char* string){
    int cursor = 0;
    if ( strlen(string)==0 )
        return No;
    if ( string[0] == '-' || string[0]=='+' )
        cursor++;
    while(string[cursor] != '\0'){
        if ( string[cursor] < 48 || string[cursor] > 57 )
            return No;
        cursor++;
    }
    return Yes;
}



/*
Checks if a given string is a reserved words (as register names, commands and instructions)
char* string - the string to check
returns: Info - Yes/No if string is a reserved word or not
assuming input has no heading or tailing whitespaces
*/
Info isReservedWord(char* string){
    int i = 0 ;
    SET_RESERVED_ARR(resv);

    while ( strcmp(resv[i], "NULL") != 0 ){ /*end of reserved words list*/
        if (strcmp( resv[i], string) == 0)  /*case found use of reserved word*/
            return Yes;
        i++;
    }
    if (lookupRegister(string) != NOT_FOUND)
        return Yes;
    return No;
}



/*
Calculates the word for operand that is not a label.
params:
char* operand - the operand to calculate word for
Info addressType - The type of operand's addresing
returns:
short - the calculated word, for the specific type of operand,
in the smallest data type that contains a 12 bits word
*/
short wordValueOfNoneLabelOperand(char* operand, Info addressType){
    short word;
    if (addressType == Immediate){
        word =  (short)atoi(operand);
    }
    if (addressType == Register)
        word = 1 << (lookupRegister(operand));
    return word;
}



/*
Checks if a given number is within range of a signed 12 bits word.
params:
int num - the number to check.
returns:
int - (macro)  YES if it is and NO otherwise
*/
int validInWordRange(int num){
    if(num >= -2048 && num <= 2047)
        return YES;
    return NO;
 }



/*
Checks if string is similar to name of any of the registers.
params:
char* string - the string to check;
returns: int - The number of register, string is similar to or NOT_FOUND otherwise
*/
int lookupRegister(char* string){
 SET_REG_ARR(regs);
 int i;
 for(i = 0; i < REGISTERS_NUMBER ; i++)
     if(strcmp(regs[i],string) ==0 )
         return i;
 return NOT_FOUND;
}



/*
Checks if string is acceptable by assembler as valid string data.
params:
char* string - the string to check.
returns: Info type - Yes if valid and No, otherwise
*/
Info isValidString(char* string){
    if (string[0] == QUOTE && string[strlen(string)-1]== QUOTE)
        return Yes;
    return No;
}
