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
    int cursor= firstPosOfChar(line, WHITE_SPACE);       /*find the first whitespace position*/

    if (strlen(line) == 0){
        printf("line#%d : Error - Command expected\n", stat -> lineNumber);
        return Error;
    }
    if (cursor == NOT_FOUND)
        strcpy(command, line); /* some commands dont require operands so this case may be ok*/
    else {
        strncpy(command, line, cursor);
        command[cursor] = '\0';
    }
    ind = lookupCommand(command);
    fprintf(stderr, "DEBUG - in parsecommand: index found %d for command %s\n",ind, command);
    if (ind == NOT_FOUND){
        printf("line#%d: Error - Invalid command\n", stat -> lineNumber);
        return Error;
    }
    stat -> commandNumber = ind; /*adding the commandNumber to STATUS to make access easier*/
    return Ok;
}

/* Searches through line for valid command getOperands
returns 'Error' if input not valid
and 'Ok' if found valid */
Info parseCommandOperands(char* line, STATUS* stat){
    int opNumFound , opNumAllowed, cmdIndex = stat -> commandNumber;
    char* opSrc = (char*)malloc(sizeof(line));
    char* opTarget = (char*)malloc(sizeof(line));
    short firstWord, opWord;
    Info opType; /*instead of addressing*/
    Info opRes;
    SET_COMMAND_TABLE(cmd);

    opNumAllowed = cmd[cmdIndex].operands;          /*for readability*/
    opNumFound = getOperands(line, opSrc, opTarget); /*operands will be empty if not found*/

    /* Lets look at the bright side first... and then at errors. it helps with freeing allocated memory on time*/
    if (opNumFound == 0 && opNumAllowed == 0){ /*no operand expected, and 0 received*/
        firstWord = buildFirstWord(cmd[opNumFound].opcode, cmd[opNumAllowed].funct, 0, 0);
        addData(stat -> dataTable, (stat -> IC)++ , firstWord, 'A');
    }
    else if( opNumFound == opNumAllowed){
            if (opNumAllowed == 1 )
                if ( isValidAddressing(opTarget, Target , stat)  == Yes){
                    opWord = wordValueOfOperand(opTarget);
                    addData(stat -> dataTable, (stat -> IC)++ , firstWord, 'A');

                }

            else {


            }
            opRes = isValidAddressing(opSrc, Source , stat) /* Addressing type is allowed for this combination of command and operand */
            if (opRes == Yes){
                firstWord = buildFirstWord(cmd[opNumAllowed].opcode, cmd[opNumAllowed].funct, 0, something);
                addData(stat -> dataTable, (stat -> IC)++ , firstWord, 'A');
            }

        }


    free(opSrc);
    free(opTarget);

    /* Look at some general errors: */
    if(externalCommas(line)){ /*comma before first operand or after last operand are not allowed*/
        printf("line#%d : Error - leading/tailing comma are not allowed\n", stat -> lineNumber);
        /*Allowed By Judy Issacs to generalize the type of Error*/
        return Error;
    }
    /*less operand than expected (assuming that if missing comma the whole string is received as one operand (most likely with spaces but I chose to look at it as a missing operand error))*/
    if ((opNumFound < opNumAllowed )){
        printf("line#%d : Error - missing Operand(s) \n", stat -> lineNumber);
        return Error;
    }
    /*got more operands than expected*/
    if ((opNumAllowed  < opNumFound)){
        printf("line#%d : Error - Too many operands\n", stat -> lineNumber);
        /*Allowed By Judy to generalize the type of Error*/
        return Error;
    }




    fprintf(stderr, "DEBUG in parseOperands , commandNumber %d operators: %d line: --|%s|--\n",cmdIndex, opNumAllowed , line);


}

/* Helper function called by parseCommandOperands only.
Parses 0/1/2 operands from parameter line
fits them into parameters opSrc and opTarget
regardless of their validity as operands
returns number of operand found*/
int getOperands(char* line,char* opSrc,char* opTarget){
    int posComma = firstPosOfChar(line,COMMA);
    strcpy(opSrc, EMPTY_STRING);    /*default*/
    strcpy(opTarget, EMPTY_STRING); /*default*/

    if (strlen(line) == 0 ) /*empty input, no operands*/
        return 0;

    if (posComma == NOT_FOUND){ /* line is considered as one operand, assingned for target operand*/
        strcpy(opTarget,line);
        return 1;
    }
    else { /*more than one operand expected*/
        strncpy(opSrc,line,posComma);   /*insert first operand into opSrc*/
        line += (posComma+1);           /*cut the operand away from line, ***including the comma****/
        opSrc[posComma] = '\0';
        strcpy(opSrc, trimWhiteSpaces(opSrc));  /*remove whitespaces around the operand found*/
        strcpy(line, trimWhiteSpaces(line));    /*remove whitespaces around the rest of line*/
        if (firstPosOfChar(line, COMMA) != NOT_FOUND){ /*if any comma is found at this point this is an error*/
            printf("line# d: Error - Too many commas\n"/*, stat -> lineNumber*/);
            return 1; /* error since 2  operands are expected*/
        }
        if (strlen(line) == 0){ /*no comma but also nothing else is left in line */
            printf("line# d : Error - missing operand\n"/*, stat -> lineNumber*/);
            return 1; /* error since 2 operands are expected*/
        }
        strcpy(opTarget,line); /*the rest of line is taken as target operand*/
        return 2;
    }
}

/*Gets an operand and its opType as source or target
and check its validity for the current command
(info held by stat parameter) according to the  command table
Returns Yes if valid, and Error if not */
Info isValidAddressing(char* operand, Info opType, STATUS* stat){
    int cmdIndex = stat -> commandNumber;
    int res, opNumAllowed ;
    Info opAddress = operandAddressType(operand, stat);
    SET_COMMAND_TABLE(cmd);
    opNumAllowed = cmd[cmdIndex].operands;

    if (opAddress == Error) /*Address enum data type, no addressing type was detected, */
        return Error; /*Info enum data type*/
    if (opNumAllowed == 1 && opType == Source ){ /**/
        printf("line#%d : Error - Too many operands (in isValidAddressing)\n", stat -> lineNumber);
        return Error;
    }/*now check if op type is allowed at this location in command*/
    if (opNumAllowed == 1 || opType == Target)
        res = firstPosOfChar(cmd[cmdIndex].opTarget, opAddress);
    else if (opType == Source)
        res = firstPosOfChar(cmd[cmdIndex].opSrc, opAddress);
    if (res == NOT_FOUND){
        printf("line#%d : Error - Invalid adressing type op operand\n", stat -> lineNumber);
        return Error;
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
    if (lookupRegister(operand) != NOT_FOUND) /*check id operand is Register*/
        return Register;

    if (operand[0] == IMMEDIATE_IDENTIFIER){ /*check if operand is Immediate*/
        operand++;
        if (isValidAsNumber(operand))
            return Immediate;
        printf("line#%d : Error - Operand is invalid as Immediate\n", stat -> lineNumber);
        return Error;
    }
    if (operand[0] == RELATIVE_IDENTIFIER){ /*check if operand is relative*/
        operand++;
        if (isValidAsSymbol(operand, stat)){
            sym = lookupSymbol(stat -> symbolTable, operand);
            if (sym!= NULL && (sym -> attr2)== Extern){
                    printf("line#%d : Error - Extern Label cannot be used in relative addressing\n", stat -> lineNumber);
                    return Error;
            }
            else  return Relative;
        }
        printf("line#%d : Error - Operand is an invalid symbol\n", stat -> lineNumber);
        return Error;
    }
    if (isValidAsSymbol(operand, stat))
        return Direct;
    else  printf("line#%d : Error - Operand is an invalid symbol\n", stat -> lineNumber);
    return Error;
}

int lookupCommand(char* string){
    int i;
    SET_COMMAND_TABLE(cmdTable);

    for(i = 0; i < COMMANDS_NUMBER ; i++)
        if(strcmp(cmdTable[i].command,string) == 0 )
            return i;
    return NOT_FOUND;
}
