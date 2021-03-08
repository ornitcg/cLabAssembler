/*
  group of the big functions that are related to the assembler
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "as_funcs.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"


/*
  Performs the first pass over the assembly file
  params: FILE* inputFile - pointer to the input assembly file
  returns: 1 if errors occured and 0 otherwise
*/
int firstPass(FILE* inputFile , Status* stat){
    /***********************************************DECLARATIONS**********************************************************/
    /*regArr regs = {"r0","r1","r2", "r3","r4","r5","r6","r7"};*/

    /*int ICF = 0;*/      /*instructions counter - for final value*/
    /*int DCF = 0;*/      /*data counter - for final value*/
    char line[MAX_LINE];                       /*line of assembly code*/
    char symbol[MAX_LABEL];                    /*to contain the label from line*/
    char instruction[MAX_INSTRUCTION];         /*to contain the label from line*/
    char command[MAX_CMD];                     /*to contain the label from line*/
    info instType;                        /*stands for instruction type*/
    info res;
    /*********************************************************************************************************/
    SET_COMMAND_TABLE(cmdTable);      /*implemented by array of structures, all set by macro*/

    while(fgets(line, MAX_LINE, inputFile) != NULL){ /*each iteration of this loop is on a whole line from input file*/
        strcpy(line, trimWhiteSpaces(line));        /*removes whitespaces from both ends and also the '\n' for each line read from file*/

        if (!toIgnore(line) ){

            res = parseSymbol(symbol, line, &stat);
            stat -> symbolFound = res;  /*Yes/No/Error*/
            if (res == Error)      /*relevant to lines that begin with label definition*/
                stat -> errorExists = Yes;
            else
                strcpy(line, trimWhiteSpaces(&line[strlen(symbol)+1])); /*restart line from end of label*/

            instType = parseInstruction(instruction, line, &stat); /*Yes/No/Error*/
            if (instType == Error)
                stat -> errorExists = Yes;
            else {
                strcpy(line, trimWhiteSpaces(&line[strlen(instruction)+1])); /*restart line from end of instruction*/
                if  ( instType == Data || instType == String ){
                    if (stat -> symbolFound == Yes)
                        addSymbol(symbolTable, (stat-> DC)+(stat->IC) , symbol, Data, Empty);
                    res = parseData(line, instType, &stat);/*return 0 if data is wrong or None*/
                    if (res == Error)
                        stat -> errorExists = Yes;
                    }
                }else if  ( instType == Extern ){
                    res = parseExtern(line, &stat);
                    if (res == Ok)
                        addSymbol(symbolTable, 0 , symbol, Extern, Empty);
                    else if (res == Error)
                        stat -> errorExists = Yes;
                    }
                }else /*no instruction - proceed to parsing command and operands*/
            }

        }/*end if not ignore*/
            /*fprintf(stderr, "firstPass  %s\n",line);*/
            /*fprintf(stderr, "length  %d\n",strlen(line));*/
        (stat -> lineNumber)++;
    }/*end while*/
    /*ICF = stat.IC;
    DCF = stat.DC;*/
    /*updateDataTable(ICF);*/
    return SOMETHING;
}/*end of firstPass*/

void secondPass(FILE* inputFile, STATUS* stat){
}


void runAssembler(FILE* inputFile, char* fileName){
    STATUS* stat;
    initStat(&stat, fileName); /*to contain status details of current line*/
    fprintf(stderr, "******** DEBUG - in runAssembler\n");
    firstPass(inputFile, stat);
    fseek(inputFile,0,SEEK_SET);
    secondPass(inputFile, stat);

}
