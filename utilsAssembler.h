#include "asStructsAndMacros.h"

/*
  Group of helper functions that are related to assembler
*/

void DO_SOMETHING();
int parseSymbol(char* symbol , char* line);
int parseInstruction(char* Instruction , char* line);

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

/*****************************************FIRST WORD STRUCTURE*********************************************/

struct FIRST_WORD{
  short dest;
  short src;
  short funct;
  short opcode;
};
typedef struct FIRST_WORD FIRST_WORD;
