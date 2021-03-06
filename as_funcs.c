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
int firstPass(FILE* inputFile , CMD_TABLE* cmd, SYMBOL_TABLE** symbolTable,DATA_IMG** dataImg, CODE_IMG** codeImg){
    /***********************************************DECLARATIONS**********************************************************/
    /*regArr regs = {"r0","r1","r2", "r3","r4","r5","r6","r7"};*/
    STATUS stat; /*to contain status details of current line*/
    /*int ICF = 0;*/      /*instructions counter - for final value*/
    /*int DCF = 0;*/      /*data counter - for final value*/
    char line[MAX_LINE];  /*line of assembly code*/
    char symbol[MAX_LABEL];       /*to contain the label from line*/
    char instruction[MAX_INSTRUCTION];       /*to contain the label from line*/
    char command[MAX_CMD];       /*to contain the label from line*/
    enum inst instType;
    /*********************************************************************************************************/

    initStatus(&stat);

    while(fgets(line, MAX_LINE, inputFile) != NULL){
        strcpy(line, trimWhiteSpaces(line));     /*removes whitespaces from both end and also the '\n' for each line read from file*/

        if (toIgnore(line)){
            fprintf(stderr, "DEBUG line to ignore  %s\n",line);
            lineCounter++;
        }
        else {
            if (parseSymbol(symbol, line, &stat));{      /*relevant to lines that begin with label definition*/
                    stat.symbolFound = YES;
                    strcpy(line, trimWhiteSpaces(&line[strlen(symbol)+1])); /*restart line from end of label*/
            }
            instType = parseInstruction(instruction, line, &stat);
            if ( instType == Data ){/*enum type*/
                if (stat.symbolFound) addSymbol(symbolTable, symbol, stat.DC, /*enum*/instType);
                strcpy(line, trimWhiteSpaces(&line[strlen(instruction)+1])); /*restart line from end of instruction*/
                stat.DC += parseData(line, dataImg ,stat.DC);/*return 0 if data is wrong or None*/
            }
            /*else if ( instType == String )
            else parseCommand(){

            }
        }/*end else*/

            /*fprintf(stderr, "firstPass  %s\n",line);*/
            /*fprintf(stderr, "length  %d\n",strlen(line));*/
            stat.lineNumber++;
    }/*end while*/
    /*ICF = stat.IC;
    DCF = stat.DC;*/
    /*updateDataTable(ICF);*/
    return SOMETHING;
}/*end of firstPass*/

void secondPass(FILE* inputFile,char* fileName){
}


void runAssembler(FILE* inputFile, char* fileName){

    CODE_IMG* codeImg = NULL;            /*starting a linked list*/
    DATA_IMG* dataImg = NULL;            /*starting a linked list*/
    SYMBOL_TABLE* symbolTable = NULL;    /*starting a linked list*/
    SET_CMD_TABLE(cmd);             /*implemented by array of structures, all set by macro*/

    fprintf(stderr, "******** DEBUG - in runAssembler\n");
    firstPass(inputFile, cmd, &symbolTable, &dataImg, &codeImg);
    fseek(inputFile,0,SEEK_SET);
    secondPass(inputFile,fileName);

}
