#include "asStructsAndMacros.h"


/*
  Performs the first scan over the assembly file
  params:
  FILE* inputFile - pointer to the input assembly file
  STATUS* stat - ths status information of the current scan
  The algorithm goes like suggested by the university
*/
void firstScan(FILE* inputFile , STATUS* stat);

/*
  Performs the second scan over the assembly file, and the codes table (code map)
  and completes data on symbols
  params:
  FILE* inputFile - pointer to the input assembly file, reset to beginning of file
  STATUS* stat - ths status information of the current file.
  The algorithm goes like suggested by the university
*/
void secondScan(FILE* inputFile, STATUS* stat);




/*
Scans the codeTable for missing data and fills in where valid.
prints out error messages for errors found that are relevant to second pass
params:
STATUS* stat - pointer to status, for easy access to all info
*/
void fillMissingDetailsInCodeTable(STATUS* stat);
