/*
Author: Ornit Cohen Gindi
This is a group of functions that deal with all the parsing that has to do with a command and its operands.
and building the words for the commands and getOperands
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asScanners.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"
#include "symbolFuncs.h"
#include "commandFuncs.h"


/* searches line for a valid command statement.
params:
char* command - to collect the command found. stays empty if none found.
char* line - the input string to search within.
STATUS* stat - to access status info, including the commandNumber field to update if a valid command is found.
returns:
Info type - Ok , if  a valid command found, and the parsed command is coyed into the parameter commands
, and returns Error if no command found (since at this stage a command must be found)
 */
Info parseCommand(char* command, char* line, STATUS* stat ){
    int ind;
    /* At this point , line is clean of heading whitespaces */
    int cursor;

    strcpy(command, EMPTY_STRING);                       /*initialization of command string*/

    if (isEmptyString(line) == YES){
        printMessageWithLocation(Error , stat, "Command expected" );
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
    if (ind == NOT_FOUND){
        printMessageWithLocation(Error, stat, "Invalid command");
        return activateErrorFlag(stat);
    }
    stat -> commandNumber = ind; /*add the commandNumber to STATUS to make access easier*/
    return Ok;
}




/*
Searches through line for valid command operands.
if anything is invalid, the error flag (in stat) is turned on
params:
char* line - The input line to search within
STATUS* stat - for easy access to current command and line info
*/
void parseCommandOperands(char* line, STATUS* stat){
    int opNumFound , opNumAllowed, cmdIndex = stat -> commandNumber;
    char opSrc[MAX_LINE] ;
    char opDest[MAX_LINE] ;
    Info errorStatus = Ok; /*instead of addressing*/
    SET_COMMAND_TABLE(cmd);
    opNumAllowed = cmd[cmdIndex].operands;          /*for readability*/

    if(externalCommas(line) || countChar(line, COMMA) > 1){ /*comma before first operand or after last operand are not allowed*/
        printMessageWithLocation(Error, stat,"Extra comma found");
        /*Allowed By Judy Issacs to generalize the type of Error*/
        activateErrorFlag(stat);
        return;
    }
    /*Searching for operands blindly, Errors are checked later*/

    opNumFound = getOperands(line, opSrc, opDest, stat); /*operands will be empty if not found*/

    /*saves the address types (Immediate/Direct/Relative/Register) found , for each operand, into status*/
    stat -> srcOpAddressType = operandAddressType(opSrc , stat);
    stat -> destOpAddressType = operandAddressType(opDest , stat);


    /*less operand than expected (assuming that if missing comma the whole string is received as one operand (most likely with spaces but I chose to look at it as a missing operand error))*/
    if (opNumFound < opNumAllowed ){
        printMessageWithLocation(Error, stat, "Missing Operand(s)");
        errorStatus = Error;
    }
    /*got more operands than expected*/
    else if ((opNumFound!=NOT_FOUND) && (opNumAllowed  < opNumFound)){
        printMessageWithLocation(Error, stat, "Found too many operands");
        /*Allowed By Judy to generalize the type of Error*/
        errorStatus = Error;
    }

    if (stat -> srcOpAddressType == Error || stat -> srcOpAddressType == Error)
        errorStatus = Error;

    if (errorStatus == Ok ){
        /*if (opNumFound == 0 && opNumAllowed == 0)*/ /*no operand expected, and 0 received*/
        addFirstWord(opNumFound, stat);
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
    
}



/*
Helper function called by parseCommandOperands only.
Parses 0/1/2 operands from parameter line
fits them into parameters opSrc and opDest regardless of their validity as operands
errors are checked by the calling functions
params:
char* line - the input line after removal of the command string
char* opSrc - a string to collect the source operand if found.
char* opDest - a string to collect the destination operand if found.
STATUS* stat - for easy access to line number
returns number of operand found or NO_FOUND-if commas errors are found
*/
int getOperands(char* line,char* opSrc,char* opDest, STATUS* stat){
    int posComma = firstPosOfChar(line,COMMA);
    strcpy(opSrc, EMPTY_STRING);    /*default*/
    strcpy(opDest, EMPTY_STRING); /*default*/

    if (strlen(line) == 0 ) /*empty input, no operands*/
        return 0;

    if (posComma == NOT_FOUND){
        /* no comma so line is considered as one operand, assingned for Dest operand, Error checks are done in the calling function*/
        strcpy(opDest,line);
        return 1;
    }
    else { /*there is a comma so more than one operand expected*/

        strncpy(opSrc,line,posComma);   /*insert first operand into opSrc*/
        opSrc[posComma] = '\0';
        trimNchars(line , posComma+1); /*remove the taken operand, from line*/
        trimWhiteSpaces(opSrc);  /*remove whitespaces around the operand found*/
        trimWhiteSpaces(line);    /*remove whitespaces around the rest of line*/

        if (firstPosOfChar(line, COMMA) != NOT_FOUND){ /*if any comma is found at this point this is an error*/
            /*printMessageWithLocation(Error, stat, "Found too many operands");*/
            activateErrorFlag(stat);
            return 3; /* error since 2  operands are expected*/
        }
        if (strlen(line) == 0) /*no comma but also nothing else is left in line */
            return 1; /* if error, it is  checked by the calling function*/

        strcpy(opDest,line); /*the rest of line is taken as Dest operand*/
        return 2;
    }
}



/*
Wrappper to buildFirstWord
Adds to code table, the first word that fits lines of commands
according to the number of operands.
params:
int opNum - the number of operands for command
STATUS* stat - for info about source and destination operand addresses, and linke to codTable
*/
void addFirstWord(int opNum,  STATUS* stat){
    int cmdIndex =  stat -> commandNumber;
    Info comment = Ok;
    short firstWord;
    char* label = stat-> label; /*if there is a label definition, it will be copied into the label field in the code table (for the first word)*/
    SET_COMMAND_TABLE(cmd);

    if (opNum == 0)
        firstWord = buildFirstWord(cmd[cmdIndex].opcode, cmd[cmdIndex].funct, 0, 0);
    else if (opNum == 1)
        firstWord = buildFirstWord(cmd[cmdIndex].opcode, cmd[cmdIndex].funct, 0, stat->destOpAddressType);
    else firstWord = buildFirstWord(cmd[cmdIndex].opcode, cmd[cmdIndex].funct, stat->srcOpAddressType, stat->destOpAddressType);
    addCode(stat -> codeTable, (stat -> IC)++ ,stat->lineNumber , comment, label , firstWord , A);
}




/*
Wrapper function - Checks the address type of a given operand and the addressing validity,
and calls the addCode with the matching parameters, that adds the right word to the code table.
params:
char* operand - the operand to build a word for
Info opType - the type of the given operand (Source or Destinatin)
STATUS* stat - for easy access to required status info
*/
void addOperandWord(char* operand, Info opType, STATUS* stat){ /*opType is source/Dest*/
    Info addressType;
    short opWord;

    if ( isValidAddressing(operand, opType,  stat)  == Yes){ /*stat here is updated with the addressType of the checked operand*/

        addressType = getAddressType(opType,stat); /* gets from stat, Immediate,Direct,Relative,Register*/

        if (addressType == Immediate || addressType == Relative) /*operand contains extra charachter to remove*/
            operand+=1; /*to eliminate the charachter # or %*/

        if ( addressType == Relative ||  addressType == Direct ) /*operand contains label*/
            addCode(stat -> codeTable, (stat -> IC)++ /*address value*/ ,stat->lineNumber, addressType /*Immediate...*/, operand /*label*/, 0/*word*/ , FillLater/*ARE*/);

        else if (addressType == Immediate || addressType == Register ){
            opWord = wordValueOfNoneLabelOperand(operand ,  addressType);
            addCode(stat -> codeTable, (stat -> IC)++/*address value*/, stat->lineNumber , Empty/*addressType not interesting*/ , EMPTY_STRING /*label*/ , opWord , A);

        /*if  stat -> addressType = Error nothing is added*/
        }
    }
}



/*
Gets an operand and its type as Source or Dest
and check its validity for the current calling command
according to the  command table
Returns Yes if valid, and Error if not
 */
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
        res = firstPosOfChar(cmd[cmdIndex].opDest, addressType); /*addressType represents the characters 0/1/2/3 */
    else if (opType == Source)
        res = firstPosOfChar(cmd[cmdIndex].opSrc, addressType);
    if (res == NOT_FOUND){
        printMessageWithLocation(Error, stat, "Invalid adressing type of operand");
        return activateErrorFlag(stat);;
    }
    return Yes;
}




/*gets an operand string and finds its addressing type
(immediate, direct, relative, register),
returns enum Addressing  type as characters.
returns 'Error' if operand is not one of any type
if not Error, the value of the operand as a word, is returned via opWord parameter*/
Info operandAddressType(char* operand,  STATUS* stat){
    SYMBOL* sym;
    if(strlen(operand) == 0)
        return Empty;

    if (lookupRegister(operand) != NOT_FOUND) /*check id operand is Register*/
        return Register;

    if (operand[0] == IMMEDIATE_IDENTIFIER){ /*check if operand has the # prefix*/
        operand++;
        if (isValidAsNumber(operand)){
            if (isValidInWordRange(atoi(operand)) == Yes)
                return Immediate;
            else {
                printMessageWithLocation(Error, stat, "Operand value is out of word range");
                return activateErrorFlag(stat);
            }
        }
        printMessageWithLocation(Error, stat, "Operand is invalid as Immediate");
        return activateErrorFlag(stat);
    }
    if (operand[0] == RELATIVE_IDENTIFIER){ /*check if operand is has the % prefix*/
        operand++;
        if (isValidAsSymbol(operand, stat) == Yes){
            sym = lookupSymbol(stat -> symbolTable, operand);
            if (sym!= NULL && (sym -> attr2)== Extern){
                    printMessageWithLocation(Error, stat,"Extern Label cannot be used in relative addressing");
                    return activateErrorFlag(stat);
            }
            else  return Relative;
        }
        return activateErrorFlag(stat); /* error message  is printed by the isValidAsSymbol*/
    }
    if (isValidAsNumber(operand) == Yes ){
        printMessageWithLocation(Error, stat,"Operand's addresing mode is undefined");
        return activateErrorFlag(stat);
    }
    if (isValidAsSymbol(operand, stat) == Yes)
        return Direct;
    else  return activateErrorFlag(stat);
}
