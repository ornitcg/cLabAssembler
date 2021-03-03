#include "structsAndMacros.h"

/*
  Group of helper functions that are related to assembler
*/

void DO_SOMETHING();
char* parseLabel(char* line,STATUS* stat);
/*
  Checks the possibility of the line to contain a label by searching for a ':'
  params:   char* line - the string to search for a label in
  returns:   1 if found, and 0 otherwise
*/
int containsLabelDef(char* line);

/*
  Gets the label from line
  params:   char* line - the string to get the label from
  returns:   string of the label if label is valid, and NULL otherwise
*/
/*char* getLabel(line);*/

char* parseNext(char* line, char* delimiter);
int toIgnore(char* line);
void initStatus(STATUS* stat);
int detectedCommand(char* str);
