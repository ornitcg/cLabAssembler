/*
Author: Ornit Cohen Gindi

Assumptions:
maximum length of line is 80
no comments in middle of line.

*/


#include <stdio.h>
#include <stdlib.h>
#include "utilsGeneral.h"
#include "as_funcs.h"



int main(int argc, char *argv[]) {

  int i;
  char* fileName;
  FILE* inputFile;
/*
  if (argc < 2){
    fprintf(stderr,"Error - no arguments found\n");
    exit(EXIT_FAILURE);
  }
  for(i= 1 ; i < argc ; i++){
    fileName = argv[i];
    inputFile = validFile(fileName);  /*input file contains a pointer to the opened file
    if (inputFile != NULL) {
        /*fprintf(stderr, "******** DEBUG - starting runAssembler\n");

        runAssembler(inputFile, fileName);
        fclose(inputFile);
    }
  }
  */

  return 0;
}
