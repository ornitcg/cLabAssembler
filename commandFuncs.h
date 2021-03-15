/* searches line for a valid command statement
If none found, returns Error for at this stage
the only valid option is to find a command
if a valid command is found , the parsed command
name is returned via the command parameter */
Info parseCommand(char* command, char* line, STATUS* stat );

/* Searches through line for valid command getOperands
returns 'Error' if input not valid
and 'Ok' if found valid */
void parseCommandOperands(char* line, STATUS* stat);
int getOperands(char* line,char* opSrc,char* opDest,STATUS* stat);
Info operandAddressType(char* operand, STATUS* stat);
Info isValidAddressing(char* operand, Info opNumber, STATUS* stat);
int lookupCommand(char* string);
void addOperandWord(char* opDest, Info addressType, STATUS* stat);
void addFirstWord(int opNumFound, char* opSrc, char* opDest, STATUS* stat);
