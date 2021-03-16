#include "asStructsAndMacros.h"


/*
  Performs the first pass over the assembly file
  params: FILE* inputFile - pointer to the input assembly file
  returns: 1 if errors occured and 0 otherwise
*/
void firstScan(FILE* inputFile , STATUS* stat);
void secondScan(FILE* inputFile, STATUS* stat);
void runAssembler(FILE* inputFile, char* fileName);






/*
Scans the codeTable for missing data and fills in where valid.
prints out error messages for errors found that are relevant to second pass
params:
STATUS* stat - pointer to status, for easy access to all info
*/
void fillMissingDetailsInCodeTable(STATUS* stat);
