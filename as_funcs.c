/*
  group of the big functions that are related to the assembler
*/
#include <stdio.h>
#include <string.h>
#include "as_funcs.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"


/*
  Performs the first pass over the assembly file
  params: FILE* inputFile - pointer to the input assembly file
  returns: 1 if errors occured and 0 otherwise
*/
int firstPass(FILE* inputFile){
    int lineCounter = 0;
    regArr regs = {"r0","r1","r2", "r3","r4","r5","r6","r7"};
    STATUS stat; /*to contain status details of current line*/
    int ICF = 0;      /*instructions counter - for final value*/
    int DCF = 0;      /*data counter - for final value*/
    char line[MAX_LINE];  /*line of assembly code*/
    char label[MAX_LABEL];       /*to contain the label from line*/
    char command[MAX_CMD_LEN];
    stat.IC = INIT_ADDRESS;    /*instructions counter*/
    stat.DC = 0;      /*data counter*/
    stat.errorExists = NO;  /*errors flag*/
    stat.symbolFound = NO;  /*symbols flag*/
    /*  SET_CMD_TABLE;*/
    fprintf(stderr, "*****DEBUG : firstPass \n");

    while(fgets(line, MAX_LINE, inputFile) != NULL){
        line = trimWhiteSpaces(line);     /*removes whitespaces from both end and also the '\n' for each line read from file*/
        if (toIgnore(line))
            lineCounter++;
        else {
            if (containsLabelDef(line)){       /*relevant to lines that begin with label definition*/
                strcpy(label,parseNext(line,LABEL_IDENTIFIER));
                if (label == NULL)
                    stat.errorExists = YES;
                else stat.symbolFound  = YES;
            }
            if (thereIsCommand(line))
                command = getCommand(line);
            if (isValid(command))
              if (storageCommand(command))
                addCommandToTable(command, stat.DC);
              /*else ( DO LINE 8)*/
            else {
              stat.errorExists = YES;
              fprintf(stderr, "Error - invalid Command  %s\n",command);
            }
            /*else if (isExternOrEntry())*/

            fprintf(stderr, "firstPass  %s\n",line);
            fprintf(stderr, "length  %d\n",strlen(line));
lineCounter++;
    }/*end while*/
    ICF = stat.IC;
    DCF = stat.DC;
    updateDataTable(ICF);
}

void secondPass(FILE* inputFile,char* fileName){
  char line[MAX_LINE];
  fprintf(stderr, "*****DEBUG : secondPass \n");

  while(fgets(line, MAX_LINE, inputFile) != NULL){
    fprintf(stderr, "secondPass  %s",line);
  }
}


void runAssembler(FILE* inputFile, char* fileName){
  fprintf(stderr, "******** DEBUG - in runAssembler\n");
  secondPass(inputFile,fileName);
  fseek(inputFile,0,SEEK_SET);
  firstPass(inputFile);


}

/*updates the data table so that every
data symbol is given a shift at the size of ICF
params: int ICF -  the size to add to data symbols
        data table - to be updated with ICF
 */
void updateDataTable(int ICF){

}
