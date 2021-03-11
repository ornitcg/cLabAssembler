#include "utilsAssembler.h"
Info  parseInstruction(char* instruction , char* line, STATUS* stat);
Info parseData(char* line, Info type, STATUS* stat);
Info parseStringData(char* string, STATUS* stat);
Info parseNumbersData(char* line, STATUS* stat);
Info parseExtern(char* line, STATUS* stat);
