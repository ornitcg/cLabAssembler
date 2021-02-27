#include <stdio.h>
#include <string.h>
#include "as_funcs.h"

void firstPass(FILE* inputFile){
  char line[MAX_LINE];
  fprintf(stderr, "*****DEBUG : firstPass \n");

  while(fgets(line, MAX_LINE, inputFile) != NULL){
    fprintf(stderr, "firstPass  %s",line);
  }
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
