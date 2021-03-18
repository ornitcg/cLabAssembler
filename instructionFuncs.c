/*
Author: Ornit Cohen Gindi
This file contains function that deal with parsing instruction lines, and the following data if exists
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asScanners.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"
#include "symbolFuncs.h"
#include "instructionFuncs.h"


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
Info  parseInstruction(char* instruction , char* line, STATUS* stat){
    int cursor = 0;

    strcpy(instruction, EMPTY_STRING);
    /*line is supposed to be clean of heading whitespaces at this point*/
    cursor =  firstPosOfChar(line, WHITE_SPACE); /*find the first whitespace position*/
    if ((strlen(line) == 0) || (cursor == NOT_FOUND))
        strcpy(instruction, line); /*no space, probably an error, get the while line instruction and continue to look for error*/
    else{
        strncpy(instruction, line, cursor ); /*get the string part until the whitespace into instruction*/
        instruction[cursor] = '\0';
    }

    if (instruction[0] == INSTRUCTION_IDENTIFIER){
        if (strcmp(instruction, ".data") == 0) return Data;
        if (strcmp(instruction, ".string") == 0) return String;
        if (strcmp(instruction, ".extern") == 0) return Extern;
        if (strcmp(instruction, ".entry") == 0) return Entry;
        printMessageWithLocation(Error, stat,"Invalid instruction name");
        activateErrorFlag(stat);
        return Error;
    }
    /*if ther is no INSTRUCTION_IDENTIFIER, it can be a command so it's not necesarily an error*/
    return No;
}



/*
Parses the pat of line that comes after .extern instruction.
params:
char* line -string that contains the rest of line after
the extern instruction, trimmed from heading and tailing whitespaces
STATUS* stat - for easy access to status parameters and update them

assuming attr2 is used only for extern and entry attributes
*/
Info parseExtern(char* line, STATUS* stat){
    SYMBOL* symBody;
    if (isEmptyString(line) == Yes){
        printMessageWithLocation(Error, stat, "Missing parameter after .extern instruction");
        return activateErrorFlag(stat);
    }
    if (firstPosOfChar(line,WHITE_SPACE)!= NOT_FOUND){
        printMessageWithLocation(Error, stat, "Too many parameters after .extern instruction");
        return activateErrorFlag(stat);
    }
    if (isValidAsSymbol(line, stat) == Error){
        /* isValidAsSymbol prints out a message*/
        return activateErrorFlag(stat);
    }
    symBody = lookupSymbol(stat->symbolTable , line);

    if (symBody != NULL){ /*Found in symbol table*/
        if (symBody -> attr2 == Entry ){ /*found in symbol table, checking the attr2 that is assumed to be used only for extern and entry attributes*/
            printMessageWithLocation(Error, stat,"Symbol can't be assigned both extern and entry");
            return activateErrorFlag(stat);
        }
        symBody -> attr2 = Extern;
    }
    else addSymbol(stat-> symbolTable, 0 /*address value*/,  line/*label*/, Empty /*attr1*/, Extern /*attr2*/,  stat);

    return Yes; /*multiple external symbols are acceptable as non error, but there is no need to add to  symbol table*/
}



/*
Searches for valid parameter (symbol) after .entry instruction.
Prints out error messages if rest of text after entry, is invalid.
params:
char* line -string that contains the rest of line after
the entry instruction, trimmed from heading and tailing whitespaces
STATUS* stat - for easy access to status parameters and update them
returns:
Info type as Error if found any, or Yes, if entry instruction line is all valid.
*/
Info parseEntry(char* line, STATUS* stat){
    SYMBOL* symBody; /* To hold the body of lookedup the symbol */

    symBody = lookupSymbol(stat->symbolTable , line);

    if(symBody == NULL){
        printMessageWithLocation(Error, stat, "Entry symbol not defined");
        return activateErrorFlag(stat);
    }
    else if (symBody -> attr2 == Extern){
    /*found in symbol table, checking the attr2 that is assumed to be used only for extern and entry attributes*/
            printMessageWithLocation(Error, stat, "Symbol can't be both extern and entry");
            return activateErrorFlag(stat);
        }
    symBody -> attr2 = Entry;
    return Yes; /*multiple external symbols are acceptable as non error, but there is no need to add to  symbol table*/

}



/*
Checks if entry instruction is written corrrect
relevant to first scan.
params:
char* line - the rest of line after instruction, to check.
STSTUS* stat - fo error flag.
returns: Ok if no error found, and Error otherwise
*/
Info checkEntrySyntax(char* line, STATUS* stat){

    if (isEmptyString(line) == YES){
        printMessageWithLocation(Error, stat,"Missing parameter after .entry instruction");
        return activateErrorFlag(stat);
    }
    if (isValidAsSymbol(line, stat) == Error){
        /* isValidAsSymbol prints out a message*/
        return activateErrorFlag(stat);
    }
    if (firstPosOfChar(line,WHITE_SPACE)!= NOT_FOUND){
        printMessageWithLocation(Error, stat, "Too many parameters after .entry instruction");
        return activateErrorFlag(stat);
    }

    return Ok;

}



/*
wrapper function that calls the required data parsing function according
to the type of data following the instruction
params:
char* line - the rest of input line, what's left after the instruction name.
Info instruction - for String or Data (enum Info type)
STATUS* stat - for other status info and update
*/
void parseData(char* line, Info instruction, STATUS* stat){
    if (strlen(line) == 0 ){
        printMessageWithLocation(Error, stat, "Missing data");
        activateErrorFlag(stat);
        return;
    }
    if (instruction == String)
        parseStringData(line, stat);
    else /* instruction == Data */
        parseNumbersData(line, stat);
}



/*
Parses the data of numbers that come after the instuction .data
params:
char* line - the part of input line that comed after the data instruction.
STATUS* stat - to access status info.
returns: Info type - Data if ok and Error if any found
*/
Info parseNumbersData(char* line, STATUS* stat){
    int cursor = 0;
    short data;
    char dataString[MAX_LINE];

    trimWhiteSpaces(line); /*trim whitespaces from the what's left of line*/
    if (externalCommas(line)){
        printMessageWithLocation(Error, stat, "External commas are not allowed");
        return activateErrorFlag(stat);
    }
    while (line[0] != '\0'){

        if (externalCommas(line)){ /*useful after first trim of data*/
            printMessageWithLocation(Error, stat, "Invalid commas found");
            return activateErrorFlag(stat);
        }
        cursor =  firstPosOfChar(line, COMMA); /*find the first comma position returns -1 if no comma found*/
        if (cursor != NOT_FOUND) { /*case comma found in data */
            strncpy(dataString, line ,cursor); /*take a piece of string up to comma, into dataString*/
            dataString[cursor] = '\0';
            trimWhiteSpaces(dataString); /*trim whitespaces from the piece of datastring*/
            trimNchars(line, cursor+1); /*remove the data string piece from line*/
            trimWhiteSpaces(line); /*trim whitespaces from the what's left of line*/
        }
        else if (isEmptyString(line) == Yes){ /*case nothing in rest of line*/
            printMessageWithLocation(Error, stat, "Missing paramerer(s) for data");
            return activateErrorFlag(stat);
        }
        else { /*case no commas in rest of line*/
                strcpy(dataString, line);
                emptyString(line);
        }

        /*now we have a dataString that is supposed to be a data item*/
        if (isValidAsNumber(dataString) ){ /*check if all charachters are numbers (plus optional sign at the beginning)*/
            if (firstPosOfChar(dataString, FLOATING_POINT) != NOT_FOUND){ /* floating point is not allowed*/
                printMessageWithLocation(Error, stat,"Fractions anf Floating point are not allowed");
                return activateErrorFlag(stat);
            }
            data = atoi(dataString); /*invert string to number*/
            if (isValidInWordRange(data) == YES){
                addData(stat -> dataTable ,stat -> DC , data); /*add data to data image*/
                (stat -> DC)++;
            }
            else{
                printMessageWithLocation(Error, stat, "Data out of range");
                return activateErrorFlag(stat);
            }
        }
        else{/*not valid number*/
            printMessageWithLocation(Error, stat, "Invalid data");
            return activateErrorFlag(stat);
        }
    }/*end while*/
    return Data;
}


/*
Parses the data string that come after the instuction .string
params:
char* line - the part of input line that comed after the data instruction.
STATUS* stat - to access status info.
returns: Info type - String if ok and Error if any found
*/
Info parseStringData(char* string, STATUS* stat){
    int strEndInd;
    if (strlen(string) ==0){
        printMessageWithLocation(Error, stat, "Missing string parameter");
        return activateErrorFlag(stat);
    }

    strEndInd = strlen(string)-1;

    if ( isValidString(string) == Yes){
        string[strEndInd]= '\0';
        string++;

        while(string[0]!= '\0'){
            addData(stat -> dataTable, stat -> DC , string[0] );
            (stat -> DC)++;
            string++;
        }
        addData(stat -> dataTable, stat -> DC , '\0');
        (stat -> DC)++;
        return String; /*reminder: String is of info type. empty string is acceptable*/
    }
    printMessageWithLocation(Error, stat, "Invalid string");
    return activateErrorFlag(stat);

}
