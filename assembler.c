/*
Author: Ornit Cohen Gindi

This project simulates an assembler.
This  program gets a file with extention '.as', written in assembly-like language and transltates it into machine-like launguage.
The output: if there are no errors, the output is 1 to 3 files:
1. '.ob' object-file-like - contains the machine-like translation.
2. '.ext' file - contains list of addresses in which an external symbol is reffered, depends if external symbols exist in input.
3. '.ent' file - contains a list of symbols that are assigned entry, and the addressed in which they are defined, depends if entry symbols exist in input.
The algorithm used is mainly like the one suggested in the MAMAN.
Data structures: linked lists.
Assumptions:
Maximum length of line is 80
No comments in middle of line.
Using the words 'symbol' and 'label' interchangably
No floating point allowed even if the operand is still a whole number

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asScanners.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"
#include "buildOutput.h"

/*
Assembler starts there
This function builds the system status and runs the
main scanner functions of the assembler
params:
FILE* inputFile - pointer to the current file to work on
char* fileName - the current fileName (to be used for output files names)
*/
void runAssembler(FILE* inputFile, char* fileName){
    STATUS stat;
    /*removeExtention(fileName);*/
    initStatus(&stat, fileName); /*to contain status details of current line*/
    firstScan(inputFile, &stat);
    fseek(inputFile,0,SEEK_SET);
    secondScan(inputFile, &stat);

    if (stat.errorExists == No)
        buildOutputFiles(&stat);

    freeMemory(&stat);
}



int main(int argc, char *argv[]) {

  int i;
  char* fileName;
  FILE* inputFile;

  if (argc == 1){
    printf("[Error] - no arguments found\n");
    exit(EXIT_FAILURE);
  }

  for(i= 1 ; i < argc ; i++){
    fileName = argv[i];
    printf("[DEBUG] The file  name  %s\n", fileName);
    inputFile = isValidFile(fileName, EXT_AS);  /*input file contains a pointer to the opened file*/
    if (inputFile != NULL) {
        runAssembler(inputFile, fileName);
    }
  }

  return 0;
}
