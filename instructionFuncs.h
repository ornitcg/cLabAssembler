#include "utilsAssembler.h"



/*
Finds if there is an instruction in the input line.
if found it is parsed into the parameter 'instruction'
params:
char* instruction - to collect the string of instruction name.
char line - the input line o parse.
STATUS* stat - to access/updae status info
returns: Info - The ype of instruction if foune (Data/String/Extern/entry),
No for no instruction found,
and Error if error found
*/
Info  parseInstruction(char* instruction , char* line, STATUS* stat);


/*
wrapper function that calls the requred function according
to the type of data following the instruction
params:
char* line - the rest of input line, what's left after the instruction.
Info instruction - for String or Data (enum Info type)
STATUS* stat - for other status info and update
*/
void parseData(char* line, Info type, STATUS* stat);



/*
Parses the data string that come after the instuction .string
params:
char* line - the part of input line that comed after the data instruction.
STATUS* stat - to access status info.
returns: Info type - String if ok and Error if any found
*/
Info parseStringData(char* string, STATUS* stat);



/*
Parses the data of numbers that come after the instuction .data
params:
char* line - the part of input line that comed after the data instruction.
STATUS* stat - to access status info.
returns: Info type - Data if ok and Error if any found
*/
Info parseNumbersData(char* line, STATUS* stat);


/*
Parses the pat of line that comes after .extern instruction.
params:
char* line -string that contains the rest of line after
the extern instruction, trimmed from heading and tailing whitespaces
STATUS* stat - for easy access to status parameters and update them

assuming attr2 is used only for extern and entry attributes
*/
Info parseExtern(char* line, STATUS* stat);





/*
Checks if entry instruction is written corrrect
relevant to first scan.
params:
char* line - the rest of line after instruction, to check.
STSTUS* stat - fo error flag.
returns: Ok if no error found, and Error otherwise
*/
Info checkEntrySyntax(char* line, STATUS* stat);



/*
Searches for valid parameter (symbol) after .entry instruction.
Prints out error messages if rest of text after entry, is invalid.
params:
char* line -string that contains the rest of line after
the entry instruction, trimmed from heading and tailing whitespaces
STATUS* stat - for easy access to status parameters as lineNumber and symbolTable
returns:
Info type as Error if found any, or Yes, if entry instruction line is all valid.
*/
Info parseEntry(char* line, STATUS* stat);
