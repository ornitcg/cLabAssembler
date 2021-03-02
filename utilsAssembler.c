/*
  Group of helper functions that are related to assembler
*/

#include <stdio.h>
#include <string.h>
#include "as_funcs.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"

char* parseNext(char* line, char* delimiter){
  return NULL;
}

/*
  Checks the possibility of the line to contain a label by searching for a ':'
  params:   char* line - the string to search for a label in
  returns:   1 if found, and 0 otherwise
*/
int containsLabelDef(char* line){
  if (strchr(line, LABEL_IDENTIFIER)!= NULL)
    return YES;
  return NO;
}
/*checks validity of label*/
int labelIsValid(char* label){

    return YES;
  return NO;
}

int toIgnore(char* line){
    if (strlen(line) == 0 || line[0] == COMMENT_IDENTIFIER)
        return YES;
    return NO;
}

/*
Builds the first worb from the four fields of the parameter struct
params: FIRST_WORD type of struct that contains 4 fields
returns: a short int output composed of the 4 fields with the right shifting
assuming the input is correct
*/
short int buildFirstWord(FIRST_WORD w){
  short int res;
  w.src = w.src << 2;
  w.funct = w.funct << 4;
  w.opcode = w.opcode << 8 ;
  res =  w.dest | w.src |w.funct | w.opcode ;
  return res;
}
