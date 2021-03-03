/*
  Group of helper functions that are related to assembler
*/

#include <stdio.h>
#include <string.h>
#include "as_funcs.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "structsAndMacros.h"

void DO_SOMETHING(){
  /*for place holding*/
}

int detectedCommand(char* str){
    return NO;
}
void initStatus(STATUS* stat){
    (*stat).IC = INIT_ADDRESS;    /*instructions counter*/
    (*stat).DC = 0;      /*data counter*/
    (*stat).errorExists = NO;  /*errors flag*/
    (*stat).symbolFound = NO;  /*symbols flag*/
}
char* parseLabel(char* line,STATUS* stat){

    return NULL;
}
char* parseNext(char* line, char* delimiter){
    return NULL;
}

/*
  Checks the possibility of the line to contain a label by searching for a ':'
  params:   char* line - the string to search for a label in
  returns:   1 if found, and 0 otherwise
*/
int containsLabelDef(char* line){
  if (strchr(line, LABEL_IDENTIFIER_INT)!= NULL)
    return YES;
  return NO;
}
/*checks validity of label*/
int labelIsValid(char* label){

    return YES;
  return NO;
}

int toIgnore(char* line){
    if (strlen(line) == 0 || line[0] == COMMENT_IDENTIFIER_INT)
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

/*updates the data table so that every
data symbol is given a shift at the size of ICF
params: int ICF -  the size to add to data symbols
        data table - to be updated with ICF
 */
void updateDataTable(int ICF){

}
