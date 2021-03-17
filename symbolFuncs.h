


/*
Finds a definition of symbol at beggining of line.
params:

char* line - the line to search within.
STATUS* stat - for easy access to status info at error messages
returns - Info (enum) Yes if a valid symbol was found, the defined symbol then in added to status
                     No - if no symbol was found
                     Error - if error found
*/
Info parseSymbol(char* line, STATUS* stat);


/*
Checks if symbol charachters are all valid.
params: char* symbol - the symbol to check
returns: int - Yes if symbol contains invalid symbol
and No otherwise
*/
int validSymbolChars(char* symbol);





/*
Checks if a given string is valid as a symbol.
params:
char* string - the string to check if valid as symbol
STATUS* stat - for easy access to status info used in error messages
returns: Info type - Yes if string is valid as symbol, and Error otherwise
assumes string is a whole symbol without whitespaces. if there are any it's an error
*/
Info isValidAsSymbol(char* string, STATUS* stat);
