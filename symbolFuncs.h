/*
  Checks the possibility of the line to contain a label by searching for a ':'
  params:   char* line - the string to search for a label in
  returns:   1 if found, and 0 otherwise
*/
int containsLabelDef(char* line);
Info parseSymbol(char* symbol , char* line, STATUS* stat);
int validSymbolChars(char* symbol);
Info isValidAsSymbol(char* string, STATUS* stat);