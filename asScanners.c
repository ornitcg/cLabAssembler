/*
  group of the big functions that are related to the assembler
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asScanners.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"
#include "symbolFuncs.h"
#include "instructionFuncs.h"
#include "commandFuncs.h"
#include "buildOutput.h"



void runAssembler(FILE* inputFile, char* fileName){
    STATUS stat;
    /*fprintf(stderr,"%s\n",fileName);*/
    removeExtention(fileName);
    initStatus(&stat, fileName); /*to contain status details of current line*/
    /*fprintf(stderr, "******** DEBUG - in runAssembler\n");*/
    firstPass(inputFile, &stat);
    fseek(inputFile,0,SEEK_SET);
    secondPass(inputFile, &stat);

    if (stat.errorExists == No)
        buildOutputFiles(&stat);

    freeMemory(&stat);
}

/*
  Performs the first pass over the assembly file
  params: FILE* inputFile - pointer to the input assembly file
  returns: 1 if errors occured and 0 otherwise
*/
void firstPass(FILE* inputFile , STATUS* stat){
    /***********************************************DECLARATIONS***********************************************************/
    char line[MAX_LINE] = EMPTY_STRING;        /*line of assembly code*/
    char symbol[MAX_LABEL] = EMPTY_STRING;     /*to contain the label from line*/
    char instruction[MAX_INSTRUCTION]= EMPTY_STRING;         /*to contain the label from line*/
    char command[MAX_CMD_LEN]= EMPTY_STRING;                 /*to contain the label from line*/
    Info instType;                             /*stands for instruction type*/
    /*********************************************************************************************************/
    fprintf(stderr,"%d DEBUG  STARTING FIRST PASS\n", stat->lineNumber);

    while(fgets(line, MAX_LINE, inputFile) != NULL){ /*each iteration of this loop is on a whole line from input file*/
        trimWhiteSpaces(line);    /*removes whitespaces from both ends and also the '\n' for each line read from file*/
        fprintf(stderr," [DEBUG] line#%d  line string: --|%s|--\n", stat->lineNumber,line);
        /*fprintf(stderr, "DEBUG IC %d, DC %d\n", stat->IC, stat->DC);*/

        if (!toIgnore(line) ){
            /***********************************SYMBOL CHECK**************************************/
            stat -> symbolFound = parseSymbol(symbol, line, stat);  /*Yes/No/Error*/
              /*relevant to lines that begin with label definition*/
            if (strlen(symbol) > 0) {
                trimNchars(line, strlen(symbol)+1); /*restart line from end of symbol, which is empty if*/
                trimWhiteSpaces(line);
                /*restart line from end of label, "+1" stand for the colon in the end*/
            }
            /**********************************INSTRUCTION CHECK***********************************/
            instType = parseInstruction(instruction, line, stat); /*Yes/No/Error*/

            if ( instType != Error && instType != No ){ /*then need to cut off instruction and trim spaces from rest of line*/
                trimNchars(line, strlen(instruction)); /*restart line from end of instruction, which is empty if*/
                trimWhiteSpaces(line);
            } /*else there was a problem in finding a command*/
            if  ( instType == Data || instType == String ){
                if (stat -> symbolFound == Yes){
                    addSymbol(stat -> symbolTable, stat-> DC , symbol, Data, Empty, stat);
                    /*printList(stat -> symbolTable, 'T'); */ /*DEBUG*/
                }
                parseData(line, instType, stat);/*return 0 if data is wrong or None*/
            }
            if ( instType == Entry ){
                do_nothing();
            }/*end if ( instType == Extern )*/
            if ( instType == Extern ){
                /*fprintf(stderr,"[EXTERN] line after extern: --|%s|--\n",line);*/
                if (parseExtern(line, stat) == Ok && (stat -> symbolFound == Yes )){
                    printf("[WARNING]  line#%d: Label definition before .extern instruction", stat -> lineNumber);/*NOT ERROR*/
                    /*printList(stat -> symbolTable, 'T'); */ /*DEBUG*/
                    /*at this point what's left of line is the operand that passed the test of parseExtern*/
                }
            }/*end if ( instType == Extern )*/
            /*********************************COMMAND CHECK**************************************/
            if ( instType == No ){/* meaning no instruction found- proceed to parsing command and operands*/

                if (stat -> symbolFound == Yes){
                    addSymbol(stat -> symbolTable, stat->IC , symbol,  Code, Empty, stat);
                    /*printList(stat -> symbolTable, 'T'); */ /*DEBUG*/
                }
                if (parseCommand(command, line,  stat) == Ok){
                    trimNchars(line, strlen(command)); /*restart line from end of instruction, which is empty if*/
                    trimWhiteSpaces(line);

                    parseCommandOperands(line, stat);
                }
                /*fprintf(stderr," [10] DEBUG firstpass3 - line# %d the rest of line: --|%s|--\n", stat-> lineNumber, line);*/

            }
        }/*end if not ignore*/
        fprintf(stderr," [DEBUG] line#%d  src: %c dest %c\n", stat -> lineNumber, stat-> srcOpAddressType,stat-> destOpAddressType );

        resetStatStructForLine(stat); /*resets part of the fields, to use for next line's data*/
    }/*end while*/
    stat -> ICF = stat -> IC;
    stat ->  DCF = stat -> DC;
    updateSymbolTable(stat);

}/*end of firstPass*/

/*
  Performs the Second pass over the assembly file, and completes data on symbols
  params: FILE* inputFile - pointer to the input assembly file
  returns: 1 if errors occured and 0 otherwise
*/
void secondPass(FILE* inputFile, STATUS* stat){
    char line[MAX_LINE] = EMPTY_STRING;        /*line of assembly code*/
    char symbol[MAX_LABEL] = EMPTY_STRING;     /*to contain the label from line*/
    char instruction[MAX_INSTRUCTION] = EMPTY_STRING;         /*to contain the label from line*/
    /*char command[MAX_CMD_LEN] = EMPTY_STRING; */                /*to contain the label from line*/
    Info instType;                             /*stands for instruction type*/

    /*********************************************************************************************************/
    fprintf(stderr," \n\n[DEBUG] SECOND PASS \n\n");

    while(fgets(line, MAX_LINE, inputFile) != NULL){ /*each iteration of this loop is on a whole line from input file*/

        trimWhiteSpaces(line);        /*removes whitespaces from both ends and also the '\n' for each line read from file*/
        /*fprintf(stderr,"\n\n");*/
        if (!toIgnore(line) ){
            /***********************************SYMBOL CHECK**************************************/
            stat -> symbolFound = parseSymbol(symbol, line, stat);  /*Yes/No/Error*/
              /*relevant to lines that begin with label definition*/
            if (strlen(symbol) > 0){
                trimNchars(line, strlen(symbol)+1); /*restart line from end of symbol, which is empty if*/
                trimWhiteSpaces(line);
                /*restart line from end of label, "+1" stand for the colon in the end*/
            }
            /**********************************INSTRUCTION CHECK***********************************/
            instType = parseInstruction(instruction, line, stat); /*Yes/No/Error*/

            if ( instType != Error && instType != No ){
                /*then need to cut off instruction and trim spaces from rest of line*/
                trimNchars(line, strlen(instruction)); /*restart line from end of symbol, which is empty if*/
                trimWhiteSpaces(line);
                 /*restart line from end of instruction, which is empty if*/
            } /*there was a problen in parsing*/
            if ( instType == Entry ){
                if (parseEntry(line, stat) == Ok && (stat -> symbolFound == Yes ))
                    printf("[WARNING]  line#%d: Label definition before .extern instruction", stat -> lineNumber);/*NOT ERROR*/
                    /*at this point what's left of line is the operand that passed the test of parseEntry*/
            }/*end if ( instType == Extern )*/
            /*********************************COMMAND CHECK**************************************/

        }/*end if not ignore*/
        resetStatStructForLine(stat); /*resets part of the fields, to use for next line's data*/

    }/*end while*/
    /*I chose to do fill the missing info in another loop over the code image since it doesn't chande the complexity, and does improve readability*/
    fillMissingDetailsInCodeTable(stat);

    printf("SYMBOL TABLE\n");
    printList(stat -> symbolTable, 'T');

    printf("CODE TABLE\n");
    printList(stat -> codeTable , 'N');

    printf("DATA TABLE\n");
    printList(stat -> dataTable , 'N');

}

void fillMissingDetailsInCodeTable(STATUS* stat){
    /*At this point the Symbol table is full, the data*/
    Node* cursor = stat -> codeTable -> head;
    int currentAddress;
    CODE_IMG* codeBody = NULL;
    SYMBOL* symbol = NULL;
    while (cursor != NULL){/*Iterating on code image rows*/
        currentAddress = cursor -> keyNum;
        codeBody = getCodeImageBody(cursor); /*The bunch of fields of code image linked list  at this cursor locaion*/
        /*printf("[1][DEBUG] before fillMissing in codeImage  lineNumber: %d ,%d ,%s , %d ,%c \n",  codeBody -> lineNumber,cursor-> keyNum, codeBody-> label, codeBody -> code,codeBody ->ARE);*/

        symbol = lookupSymbol(stat -> symbolTable, codeBody -> label);/*symbol points at the body of the symbol table link thatbelongs to the symbol at cursor location,if this location does not relate to a symbol, Null will be returned*/

        if ( strcmp(codeBody-> label,EMPTY_STRING)!= 0  && symbol == NULL ){ /*any operand symbol should be found in the symbol table, thus error*/
            printf("[Error] line#%d: Symbol not defined --|%s|--\n", codeBody -> lineNumber, codeBody -> label);
            activateErrorFlag(stat);
        }

        if ( codeBody -> ARE == FillLater){
            if ( codeBody -> comment == Relative)
                codeBody -> code = (symbol-> address) - currentAddress;
            else if ( codeBody -> comment == Direct)
                codeBody -> code = symbol -> address;

            if (symbol -> attr2 == Extern )
                codeBody -> ARE = E;
            else codeBody -> ARE = R;
        }
        /*printf("[2][DEBUG] after fillMissing in codeImage  lineNumber: %d ,%d ,%s , %d ,%c \n",  codeBody -> lineNumber,cursor-> keyNum, codeBody-> label, codeBody -> code,codeBody ->ARE);*/

        cursor = cursor -> next;
    }
}
