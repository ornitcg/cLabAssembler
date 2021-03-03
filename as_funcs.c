/*
  group of the big functions that are related to the assembler
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "as_funcs.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "structsAndMacros.h"


/*
  Performs the first pass over the assembly file
  params: FILE* inputFile - pointer to the input assembly file
  returns: 1 if errors occured and 0 otherwise
*/
int firstPass(FILE* inputFile){
    /***********************************************DECLARATIONS**********************************************************/
    int lineCounter = 0;
    /*regArr regs = {"r0","r1","r2", "r3","r4","r5","r6","r7"};*/
    STATUS stat; /*to contain status details of current line*/
    /*int ICF = 0;*/      /*instructions counter - for final value*/
    /*int DCF = 0;*/      /*data counter - for final value*/
    char line[MAX_LINE];  /*line of assembly code*/
    char label[MAX_LABEL];       /*to contain the label from line*/
    /*char command[MAX_CMD_LEN];*/
    /*********************************************************************************************************/

    SET_SYMBOL_TABLE(symbolTable);
    SET_CMD_TABLE(cmd);
    initStatus(&stat);

    /*  SET_CMD_TABLE;*/
    fprintf(stderr, "*****DEBUG : firstPass \n");

    while(fgets(line, MAX_LINE, inputFile) != NULL){
        fprintf(stderr, "DEBUG firstPass  --%s--\n",line);
        strcpy(line, trimWhiteSpaces(line));     /*removes whitespaces from both end and also the '\n' for each line read from file*/
        fprintf(stderr, "DEBUG trimmed  --%s--\n",line);

        if (toIgnore(line)){
            fprintf(stderr, "DEBUG line to ignore  %s\n",line);
            lineCounter++;
        }
        else {
            DO_SOMETHING();
            strcpy(label,parseLabel(line, &stat));      /*relevant to lines that begin with label definition*/

        }/*end else*/

            /*fprintf(stderr, "firstPass  %s\n",line);*/
            /*fprintf(stderr, "length  %d\n",strlen(line));*/
            lineCounter++;
    }/*end while*/
    /*ICF = stat.IC;
    DCF = stat.DC;*/
    /*updateDataTable(ICF);*/
    return SOMETHING;
}/*end of firstPass*/

void secondPass(FILE* inputFile,char* fileName){
}


void runAssembler(FILE* inputFile, char* fileName){
  fprintf(stderr, "******** DEBUG - in runAssembler\n");
  secondPass(inputFile,fileName);
  fseek(inputFile,0,SEEK_SET);
  firstPass(inputFile);
}
