/*
Author: Ornit Cohen Gindi

Assumptions:
maximum length of line is 80
no comments in middle of line.
using the words 'symbol' and 'label' interchangably for labelIsValid
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "as_funcs.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"




int main(int argc, char *argv[]) {

  int i;
  char* fileName;
  FILE* inputFile;

  if (argc < 2){
    fprintf(stderr,"Error - no arguments found\n");
    exit(EXIT_FAILURE);
  }
  for(i= 1 ; i < argc ; i++){
    fileName = argv[i];
    inputFile = validFile(fileName);  /*input file contains a pointer to the opened file*/
    if (inputFile != NULL) {
        fprintf(stderr, "******** DEBUG - starting runAssembler\n");

        runAssembler(inputFile, fileName);
        fprintf(stderr, "******** DEBUG - done runAssembler\n");

        fclose(inputFile);
    }
  }


  return 0;
}
