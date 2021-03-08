#include "asStructsAndMacros.h"

/*
  Group of helper functions that are related to assembler
*/

void DO_SOMETHING();
info parseSymbol(char* symbol , char* line, STATUS* stat);
info parseData(char* line, info type, STATUS* stat);
info parseNumbersData(line, &stat);
info parseStringData(char* string, STATUS* stat);
info  parseInstruction(char* instruction , char* line, STATUS* stat);
/*
  Checks the possibility of the line to contain a label by searching for a ':'
  params:   char* line - the string to search for a label in
  returns:   1 if found, and 0 otherwise
*/
int containsLabelDef(char* line);
int validSymbolChars(char* symbol); /*OK*/
int validInWordRange(short num);
int lookupCommand(char* string);
int lookupRegister(char* string);
info isReservedWord(char* string);
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
