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
#include "asScanners.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"

int main(int argc, char *argv[]) {

  int i;
  char* fileName;
  FILE* inputFile;

  if (argc < 2){
    fprintf(stderr,"[Error] - no arguments found\n");
    exit(EXIT_FAILURE);
  }

  for(i= 1 ; i < argc ; i++){
      fileName = argv[i];
  }
  fprintf(stderr,"DEBUG\n\n");

  for(i= 1 ; i < argc ; i++){
    fileName = argv[i];
    /*fprintf(stderr,"[DEBUG] - in assembler  fileName %s\n", fileName);*/

    inputFile = validFile(fileName, EXT_AS);  /*input file contains a pointer to the opened file*/
    if (inputFile != NULL) {
        fprintf(stderr, "******** DEBUG - starting runAssembler for file %s\n\n",fileName);

        runAssembler(inputFile, fileName);

        fclose(inputFile);
    }
    fprintf(stderr, "******** DEBUG - done main\n");

  }
  fprintf(stderr,"DEBUG\n\n");

  for(i= 1 ; i < argc ; i++){
      fileName = argv[i];
      fprintf(stderr,"[DEBUG] - in assembler  fileName %s  %s\n", argv[i], fileName);
  }

  return 0;
}
