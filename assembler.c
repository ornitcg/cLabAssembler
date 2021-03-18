/*
Author: Ornit Cohen Gindi

This project simulates an assembler.
The input: a file with extention '.as', written in assembly-like language and translates it into machine-like launguage.
The output: if there are no errors, the output is 1 to 3 files:
1. '.ob' object-file-like - contains the machine-like translation.
2. '.ext' file - contains list of addresses in which an external symbol is reffered, depends if external symbols exist in input.
3. '.ent' file - contains a list of symbols that are assigned entry, and the addressed in which they are defined, depends if entry symbols exist in input.

This program reads multiple input files, and builds output files for each of which has no errors .

The algorithm used is as the one suggested in the MAMAN.
Used data structures: linked lists for symbol table, code image  and data image.
also used a STATUS structure - that contains  all the needed system information including links to all linkedlists.
this structure goes as parameter to functions, as an easy way to pass complex info in one parameter, also makes a nicer code

Assumptions:
- Maximum length of line is 80 characters excluding '\n'
- Nothing but whitespaces can come before comment.
- Using the words 'symbol' and 'label' interchangably for the same thing
- Allowing no whitespaces between the colon of a label definition and the next thing (command/Instruction),
since it was up to me - approved by JUDY ISSACS
- No floating point allowed even if the operand is still a whole number
- Ignoring the assumptin of 4096 words in RAM - approved by JUDY ISSACS
- Not allowing data numbers that  the range of signed 12 bits.
- If any error is found in the first scan, second scan will not occure.

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
    initStatus(&stat, fileName); /*to contain status details of current line*/
    firstScan(inputFile, &stat);
    fseek(inputFile,0,SEEK_SET);
    /*fprintf(stderr, "[DEBUG] after first scan\n");*/

    if (stat.errorExists == No){
        secondScan(inputFile, &stat);
    }
    /*fprintf(stderr, "[DEBUG] after second scan\n");*/

    if (stat.errorExists == No)
        buildOutputFiles(&stat);

    freeMemory(&stat);
}


/* MAIN */
int main(int argc, char *argv[]) {

  int i;
  char* fileName;
  FILE* inputFile;

  checkNumOfCommandLineArgs(argc);

  for(i= 1 ; i < argc ; i++){
    fileName = argv[i];
    inputFile = isValidFile(fileName, EXT_AS);  /*input file contains a pointer to the opened file*/
    if (inputFile != NULL)
        runAssembler(inputFile, fileName);
  }
  return 1;
}
