/* searches line for a valid command statement.
params:
char* command - to collect the command found. stays empty if none found.
char* line - the input string to search within.
STATUS* stat - to access status info, including the commandNumber field to update if a valid command is found.
returns:
Info type - Ok , if  a valid command found, and the parsed command is coyed into the parameter commands
, and returns Error if no command found (since at this stage a command must be found)
 */
Info parseCommand(char* command, char* line, STATUS* stat );

/* Searches through line for valid command getOperands
returns 'Error' if input not valid
and 'Ok' if found valid */
void parseCommandOperands(char* line, STATUS* stat);



/*
Helper function called by parseCommandOperands only.
Parses 0/1/2 operands from parameter line
fits them into parameters opSrc and opDest regardless of their validity as operands
errors are checked by the calling functions
params:
char* line - the input line after removal of the command string
char* opSrc - a string to collect the source operand if found.
char* opDest - a string to collect the destination operand if found.
STATUS* stat - for easy access to line number
returns number of operand found
*/
int getOperands(char* line,char* opSrc,char* opDest,STATUS* stat);




/*
Wrappper to buildFirstWord
Adds to code table, the first word that fits lines of commands
according to the number of operands.
params:
int opNum - the number of operands for command
STATUS* stat - for info about source and destination operand addresses, and linke to codTable
*/
Info operandAddressType(char* operand, STATUS* stat);


/*
Gets an operand and its type as Source or Dest
and check its validity for the current calling command
according to the  command table
Returns Yes if valid, and Error if not
 */
Info isValidAddressing(char* operand, Info opNumber, STATUS* stat);




/*
Wrapper function - Checks the address type of a given operand and the addressing validity,
and calls the addCode with the matching parameters, that adds the right word to the code table.
params:
char* operand - the operand to build a word for
Info opType - the type of the given operand (Source or Destinatin)
STATUS* stat - for easy access to required status info
*/
void addOperandWord(char* opDest, Info addressType, STATUS* stat);


/*
Wrappper to buildFirstWord
Adds to code table, the first word that fits lines of commands
according to the number of operands.
params:
int opNum - the number of operands for command
STATUS* stat - for info about source and destination operand addresses, and linke to codTable
*/
void addFirstWord(int opNumFound, STATUS* stat);
