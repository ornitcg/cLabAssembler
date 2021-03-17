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
int getOperands(char* line,char* opSrc,char* opDest,STATUS* stat);
Info operandAddressType(char* operand, STATUS* stat);
Info isValidAddressing(char* operand, Info opNumber, STATUS* stat);
int lookupCommand(char* string);


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
