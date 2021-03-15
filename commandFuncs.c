/*Parse functions that deal with the step of command and its operands*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asScanners.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"
#include "symbolFuncs.h"
#include "commandFuncs.h"

/* searches line for a valid command statement
If none found, returns Error for at this stage
the only valid option is to find a command
if a valid command is found , the parsed command
name is returned via the command parameter */
Info parseCommand(char* command, char* line, STATUS* stat ){
    int ind;
    /* At this point , line is clean of heading whitespaces */
    int cursor;
    strcpy(command, EMPTY_STRING);                       /*initialization of command string*/

    if (isEmptyString(line) == YES){
        printf("[Error] line#%d : Command expected\n", stat -> lineNumber);
        return activateErrorFlag(stat);
    }

    cursor= firstPosOfChar(line, WHITE_SPACE);              /*find the first whitespace position*/
    if (cursor == NOT_FOUND)                               /* no whitespace found in rest of line*/
        strcpy(command, line);                            /* some commands dont require operands so this case may be ok*/
    else {
        strncpy(command, line, cursor);                 /*copy into commant charchters up until the first whitespace*/
        command[cursor] = '\0';
    }
    ind = lookupCommand(command);                           /*find index of command in commands table*/
    /*fprintf(stderr, "[1][DEBUG] - in parsecommand: index found %d for command --|%s|--\n",ind, command);*/
    if (ind == NOT_FOUND){
        printf("[Error] line#%d: Invalid command\n", stat -> lineNumber);
        return activateErrorFlag(stat);
    }
    stat -> commandNumber = ind; /*add the commandNumber to STATUS to make access easier*/
    return Ok;
}

/* Searches through line for valid command getOperands
returns 'Error' if input not valid
and 'Ok' if found valid */
void parseCommandOperands(char* line, STATUS* stat){
    int opNumFound , opNumAllowed, cmdIndex = stat -> commandNumber;
    char* opSrc = (char*)malloc(sizeof(line));
    char* opDest = (char*)malloc(sizeof(line));
    Info errorStatus = Ok; /*instead of addressing*/
    SET_COMMAND_TABLE(cmd);
    opNumAllowed = cmd[cmdIndex].operands;          /*for readability*/

    /*Searching for operands blindly, Errors are checked later*/
    opNumFound = getOperands(line, opSrc, opDest, stat); /*operands will be empty if not found*/

    stat -> srcOpAddressType = operandAddressType(opSrc , stat);
    stat -> destOpAddressType = operandAddressType(opDest , stat);
    /*fprintf(stderr," [DEBUG] in parseCommandOperands line#%d  src: %c dest %c\n", stat -> lineNumber, stat-> srcOpAddressType,stat-> destOpAddressType );*/

    if(externalCommas(line)){ /*comma before first operand or after last operand are not allowed*/
        printf("[Error] line#%d: Leading/tailing comma are not allowed\n", stat -> lineNumber);
        /*Allowed By Judy Issacs to generalize the type of Error*/
        errorStatus = Error;
    }
    /*less operand than expected (assuming that if missing comma the whole string is received as one operand (most likely with spaces but I chose to look at it as a missing operand error))*/
    if ((opNumFound < opNumAllowed )){
        printf("[Error] line#%d: Missing Operand(s) \n", stat -> lineNumber);
        errorStatus = Error;
    }
    /*got more operands than expected*/
    if ((opNumAllowed  < opNumFound)){
        printf("[Error] line#%d: Too many operands\n", stat -> lineNumber);
        /*Allowed By Judy to generalize the type of Error*/
        errorStatus = Error;
    }
    if (stat -> srcOpAddressType == Error || stat -> srcOpAddressType == Error)
        errorStatus = Error;

    if (errorStatus == Ok ){

        /*if (opNumFound == 0 && opNumAllowed == 0)*/ /*no operand expected, and 0 received*/
        addFirstWord(opNumFound, opSrc, opDest, stat);
        if(opNumFound > 0 && opNumFound == opNumAllowed){
                if (opNumAllowed == 1){
                    addOperandWord(opDest, Dest, stat);
                }
                else { /*2 allowed*/
                    addOperandWord(opSrc, Source , stat);
                    addOperandWord(opDest, Dest , stat);
                }
        }
    }
    else activateErrorFlag(stat);

    free(opSrc);
    free(opDest);
}

void addFirstWord(int opNum, char* opSrc, char* opDest, STATUS* stat){
    int cmdIndex =  stat -> commandNumber;
    Info comment = Ok;
    short firstWord;
    char* label = stat-> label;
    SET_COMMAND_TABLE(cmd);

    if (opNum == 0)
        firstWord = buildFirstWord(cmd[cmdIndex].opcode, cmd[cmdIndex].funct, 0, 0);
    else if (opNum == 1)
        firstWord = buildFirstWord(cmd[cmdIndex].opcode, cmd[cmdIndex].funct, 0, stat->destOpAddressType);
    else firstWord = buildFirstWord(cmd[cmdIndex].opcode, cmd[cmdIndex].funct, stat->srcOpAddressType, stat->destOpAddressType);
    addCode(stat -> codeTable, (stat -> IC)++ ,stat->lineNumber , comment, label , firstWord , A);
}

void addOperandWord(char* operand, Info opType, STATUS* stat){ /*opType is source/Dest*/
    Info addressType;
    short opWord;

    if ( isValidAddressing(operand, opType,  stat)  == Yes){ /*stat here is updated with the addressType of the checked operand*/

        addressType = getAddressType(opType,stat); /*Immediate,Direct,Relative,Register*/

        if (addressType == Immediate || addressType == Relative) /*operand contains extra charachter to remove*/
            operand+=1; /*to eliminate the charachter # or %*/

        if ( addressType == Relative ||  addressType == Direct ) /*operand contains label*/
            addCode(stat -> codeTable, (stat -> IC)++ /*address value*/ ,stat->lineNumber, addressType /*Immediate...*/, operand /*label*/, 0/*word*/ , FillLater/*ARE*/);

        else if (addressType == Immediate || addressType == Register ){
            /*fprintf(stderr,"[DEBUG]in addOperandWord the Immediate or register operand is: %s\n",operand);*/
            opWord = wordValueOfNoneLabelOperand(operand ,  addressType);
            addCode(stat -> codeTable, (stat -> IC)++/*address value*/, stat->lineNumber , Empty/*addressType not interesting*/ , EMPTY_STRING /*label*/ , opWord , A);

        /*if  stat -> addressType = Error nothing is added*/
        }
    }
}


/* Helper function called by parseCommandOperands only.
Parses 0/1/2 operands from parameter line
fits them into parameters opSrc and opDest
regardless of their validity as operands
returns number of operand found*/
int getOperands(char* line,char* opSrc,char* opDest, STATUS* stat){
    int posComma = firstPosOfChar(line,COMMA);
    strcpy(opSrc, EMPTY_STRING);    /*default*/
    strcpy(opDest, EMPTY_STRING); /*default*/

    if (strlen(line) == 0 ) /*empty input, no operands*/
        return 0;

    if (posComma == NOT_FOUND){ /* line is considered as one operand, assingned for Dest operand*/
        strcpy(opDest,line);
        return 1;
    }
    else { /*more than one operand expected*/
        strncpy(opSrc,line,posComma);   /*insert first operand into opSrc*/
        line += (posComma+1);           /*cut the operand away from line, ***including the comma****/
        opSrc[posComma] = '\0';
        trimWhiteSpaces(opSrc);  /*remove whitespaces around the operand found*/
        trimWhiteSpaces(line);    /*remove whitespaces around the rest of line*/
        if (firstPosOfChar(line, COMMA) != NOT_FOUND){ /*if any comma is found at this point this is an error*/
            printf("[Error -d ] line# d: Too many commas\n"/*, stat -> lineNumber*/);
            activateErrorFlag(stat);
            return 1; /* error since 2  operands are expected*/
        }
        if (strlen(line) == 0){ /*no comma but also nothing else is left in line */
            printf("[Error -d ]line# d:  missing operand\n"/*, stat -> lineNumber*/);
            activateErrorFlag(stat);
            return 1; /* error since 2 operands are expected*/
        }
        strcpy(opDest,line); /*the rest of line is taken as Dest operand*/
        return 2;
    }
}

/*Gets an operand and its opType as source or Dest
and check its validity for the current command
(info held by stat parameter) according to the  command table
Returns Yes if valid, and Error if not */
Info isValidAddressing(char* operand, Info opType, STATUS* stat){
    int cmdIndex = stat -> commandNumber;
    int res, opNumAllowed ;
    Info addressType ;
    SET_COMMAND_TABLE(cmd);
    opNumAllowed = cmd[cmdIndex].operands;

    if (opType == Source)
        addressType = (stat -> srcOpAddressType);
    else if (opType == Dest)
        addressType = (stat -> destOpAddressType);

    if (addressType == Error) /*Address enum data type, no addressing type was detected, */
        return activateErrorFlag(stat);; /*Info enum data type*/

    /*now check if op type is allowed for use at this location in command*/
    if (opNumAllowed == 1 || opType == Dest)
        res = firstPosOfChar(cmd[cmdIndex].opDest, addressType); /*addressType represents the charachters 0/1/2/3 */
    else if (opType == Source)
        res = firstPosOfChar(cmd[cmdIndex].opSrc, addressType);
    if (res == NOT_FOUND){
        printf("[Error] line#%d: Invalid adressing type op operand\n", stat -> lineNumber);
        return activateErrorFlag(stat);;
    }
    return Yes;
}

/*gets an operand string and finds its addressing type
(immediate, direct, relative, register),
returns enum Addressing  type as charachters.
returns 'Error' if operand is not one of any type
if not Error, the value of the operand as a word, is returned via opWord parameter*/
Info operandAddressType(char* operand,  STATUS* stat){
    SYMBOL* sym;
    if(strlen(operand) == 0)
        return Empty;

    if (lookupRegister(operand) != NOT_FOUND) /*check id operand is Register*/
        return Register;

    if (operand[0] == IMMEDIATE_IDENTIFIER){ /*check if operand is Immediate*/
        operand++;
        if (isValidAsNumber(operand))
            return Immediate;
        printf("[Error] line#%d: Operand is invalid as Immediate\n", stat -> lineNumber);
        return activateErrorFlag(stat);
    }
    if (operand[0] == RELATIVE_IDENTIFIER){ /*check if operand is relative*/
        operand++;
        if (isValidAsSymbol(operand, stat)){
            sym = lookupSymbol(stat -> symbolTable, operand);
            if (sym!= NULL && (sym -> attr2)== Extern){
                    printf("[Error] line#%d: Extern Label cannot be used in relative addressing\n", stat -> lineNumber);
                    return activateErrorFlag(stat);
            }
            else  return Relative;
        }
        printf("[Error] line#%d: Operand is an invalid symbol\n", stat -> lineNumber);
        return activateErrorFlag(stat);
    }
    if (isValidAsSymbol(operand, stat))
        return Direct;
    else  printf("[Error] line#%d: Operand is an invalid symbol\n", stat -> lineNumber);
    return activateErrorFlag(stat);
}

int lookupCommand(char* string){
    int i;
    SET_COMMAND_TABLE(cmdTable);

    for(i = 0; i < COMMANDS_NUMBER ; i++)
        if(strcmp(cmdTable[i].command,string) == 0 )
            return i;
    return NOT_FOUND;
}
