/*
  Group of helper functions that are related to assembler
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asScanners.h"
#include "linkedList.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"

void DO_SOMETHING(){
  /*for place holding*/
}

int toIgnore(char* line){
    if (strlen(line) == 0 || line[0] == COMMENT_IDENTIFIER_INT)
        return YES;
    return NO;
}

/*Calculates the value of distance from current address to label address*/
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
  short  res;
  src = src << 2;
  funct = funct << 4;
  opcode = opcode << 8 ;
  res =  dest | src | funct | opcode ;
  return res;
}



Info isReservedWord(char* string){
    int i=0;
    SET_RESERVED_ARR(resv);
    /*printf("reserved word %s\n", resv[4]) ;*/
    while ( strcmp(resv[i], "NULL") != 0 ){ /*end of reserved words list*/
        if (strcmp( resv[i], string) == 0)  /*casefound use of reserved word*/
            return Yes;
        i++;
    }
    return No;
}

short wordValueOfNoneLabelOperand(char* operand, Info addressType){
    short word;
    if (addressType == Immediate)
        word =  (short)atoi(++operand);
    if (addressType == Register)
        word = (short)lookupRegister(operand);
    return word;
}

int validInWordRange(short num){
    if(num >= 2048 && num <= 2047)
        return YES;
    return NO;
 }

int lookupRegister(char* string){
 SET_REG_ARR(regs);
 int i;
 for(i = 0; i < REGISTERS_NUMBER ; i++)
     if(strcmp(regs[i],string) ==0 )
         return i;
 return NOT_FOUND;
}
