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
    Info res;                                  /*stands for result*/
    /*********************************************************************************************************/
    SET_COMMAND_TABLE(cmdTable);      /*implemented by array of structures, all set by macro*/

    while(fgets(line, MAX_LINE, inputFile) != NULL){ /*each iteration of this loop is on a whole line from input file*/
        strcpy(line, trimWhiteSpaces(line));        /*removes whitespaces from both ends and also the '\n' for each line read from file*/
        fprintf(stderr,"\n\n");
        if (!toIgnore(line) ){
            /***********************************SYMBOL CHECK**************************************/
            res = parseSymbol(symbol, line, stat);
            stat -> symbolFound = res;  /*Yes/No/Error*/
              /*relevant to lines that begin with label definition*/
            if (res == Yes){
                fprintf(stderr,"DEBUG firstpass1 - line# %d The symbol that was found is --|%s|--\n", stat-> lineNumber, symbol);
                strcpy(line, trimWhiteSpaces(&line[strlen(symbol)+1]));
                /*restart line from end of label, "+1" stand for the colon in the end*/
            }
            /**********************************INSTRUCTION CHECK***********************************/
            instType = parseInstruction(instruction, line, stat); /*Yes/No/Error*/

            if ( instType != Error && instType != No ){ /*then need to cut off instruction and trim spaces from rest of line*/
                strcpy(line, trimWhiteSpaces(&line[strlen(instruction)])); /*restart line from end of instruction, which is empty if*/
                fprintf(stderr,"DEBUG firstpass2 - line# %d The Istruction that was found is --|%s|--\n", stat-> lineNumber, instruction);
            } /*there was a problen in parsing*/
            if  ( instType == Data || instType == String ){
                if (stat -> symbolFound == Yes)
                    addSymbol(stat -> symbolTable, stat-> DC , symbol, Data, Empty);
                parseData(line, instType, stat);/*return 0 if data is wrong or None*/
            }
            if ( instType == Extern ){
                if (parseExtern(line, stat) == Ok && (stat -> symbolFound == Yes )){
                    printf("[WARNING]  line#%d: Label definition before .extern instruction", stat -> lineNumber);/*NOT ERROR*/
                    addSymbol(stat -> symbolTable, 0 , line , Empty, Extern);
                    /*at this point what's left of line is the operand that passed the test of parseExtern*/
                }
            }/*end if ( instType == Extern )*/
            /*********************************COMMAND CHECK**************************************/
            if ( instType == No ){/* meaning no instruction found- proceed to parsing command and operands*/
                fprintf(stderr,"DEBUG firstpass3 - line# %d the rest of line: --|%s|--\n", stat-> lineNumber, line);
                if (stat -> symbolFound == Yes)
                    addSymbol(stat -> symbolTable, stat->IC , symbol,  Code, Empty);

                if (parseCommand(command, line,  stat) == Ok){
                    strcpy(line, trimWhiteSpaces(&line[strlen(command)])); /*restart line from end of command*/
                    parseCommandOperands(line, stat);
                }
            }
        }/*end if not ignore*/
        resetStatStructForLine(stat); /*resets part of the fields, to use for next line's data*/
    }/*end while*/
    stat -> ICF = stat -> IC;
    stat ->  DCF = stat -> DC;

}/*end of firstPass*/

void secondPass(FILE* inputFile, STATUS* stat){




}


void runAssembler(FILE* inputFile, char* fileName){
    STATUS stat;
    initStatus(&stat, fileName); /*to contain status details of current line*/
    /*fprintf(stderr, "******** DEBUG - in runAssembler\n");*/
    firstPass(inputFile, &stat);
    fseek(inputFile,0,SEEK_SET);
    secondPass(inputFile, &stat);

}
