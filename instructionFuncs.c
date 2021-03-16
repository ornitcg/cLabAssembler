/*need to add the line numbers*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asScanners.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"
#include "symbolFuncs.h"
#include "instructionFuncs.h"

Info  parseInstruction(char* instruction , char* line, STATUS* stat){
    int cursor = 0;

    strcpy(instruction, EMPTY_STRING);
    /*line is supposed to be clean of heading whitespaces at this point*/
    cursor =  firstPosOfChar(line, WHITE_SPACE); /*find the first whitespace position*/
    if ((strlen(line) == 0) || (cursor == NOT_FOUND))
        return No;

    strncpy(instruction, line, cursor ); /*get the string part until the whitespace into instruction*/
    instruction[cursor] = '\0';
    if (instruction[0] == INSTRUCTION_IDENTIFIER){
        if (strcmp(instruction, ".data") == 0) return Data;
        if (strcmp(instruction, ".string") == 0) return String;
        if (strcmp(instruction, ".extern") == 0) return Extern;
        if (strcmp(instruction, ".entry") == 0) return Entry;
        printErrorWithLocation(stat,"Invalid instruction name");
        activateErrorFlag(stat);
        return Error;
    }
    /*if ther is no INSTRUCTION_IDENTIFIER, it can be a command so it's not necesarily an error*/
    return No;
}



void parseData(char* line, Info type, STATUS* stat){
    if (strlen(line) == 0 ){
        printErrorWithLocation(stat, "Missing data");
        stat -> errorExists = Yes;
        return;
    }
    if (type == String)
        parseStringData(line, stat);
    else
        parseNumbersData(line, stat);
}

Info parseStringData(char* string, STATUS* stat){
    int strEndInd;
    if (strlen(string) ==0){
        printErrorWithLocation(stat, "Missing string parameter");
        return activateErrorFlag(stat);
    }

    strEndInd = strlen(string)-1;

    if ( isValidString(string) == Yes){
        string[strEndInd]= '\0';
        string++;

        while(string[0]!= '\0'){
            addData(stat -> dataTable, stat -> DC , string[0] ,'A');
            (stat -> DC)++;
            string++;
        }
        addData(stat -> dataTable, stat -> DC , '\0' ,'A');
        (stat -> DC)++;
        return String; /*reminder: String is of info type. empty string is acceptable*/
    }
    printErrorWithLocation(stat, "Invalid string");
    return activateErrorFlag(stat);

}


/*assuming attr2 is used only for extern and entry attributes*/
Info parseExtern(char* line, STATUS* stat){
    SYMBOL* symBody;

    if (isEmptyString(line)){
        printErrorWithLocation(stat, "Missing parameter after .extern instruction");
        return activateErrorFlag(stat);
    }
    if (firstPosOfChar(line,WHITE_SPACE)!= NOT_FOUND){
        printErrorWithLocation(stat, "Too many parameters after .extern instruction");
        return activateErrorFlag(stat);
    }
    if (isValidAsSymbol(line, stat) == Error){
        printErrorWithLocation(stat, "Invalid symbol after .extern instruction");
        return activateErrorFlag(stat);
    }
    symBody = lookupSymbol(stat->symbolTable , line);

    if (symBody != NULL){ /*Found in symbol table*/
        if (symBody -> attr2 == Entry ){ /*found in symbol table, checking the attr2 that is assumed to be used only for extern and entry attributes*/
            printErrorWithLocation(stat,"Symbol can't be assigned both extern and entry");
            return activateErrorFlag(stat);
        }
        symBody -> attr2 = Extern; /*DEBUG???*/
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
STATUS* stat - for easy access to status parameters as lineNumber and symbolTable
returns:
Info type as Error if found any, or Yes, if entry instruction line is all valid.
*/
Info parseEntry(char* line, STATUS* stat){
    SYMBOL* symBody; /* To hold the body of lookedup the symbol */

    if (isEmptyString(line)){
        printErrorWithLocation(stat,"Missing parameter after .entry instruction");
        return activateErrorFlag(stat);
    }
    if (firstPosOfChar(line,WHITE_SPACE)!= NOT_FOUND){
        printErrorWithLocation(stat, "Too many parameters after .entry instruction");
        return activateErrorFlag(stat);
    }
    if (isValidAsSymbol(line, stat) == Error){
        printErrorWithLocation(stat, "Invalid symbol after .entry instruction");
        return activateErrorFlag(stat);
    }
    symBody = lookupSymbol(stat->symbolTable , line);

    if(symBody == NULL){
        printErrorWithLocation(stat, "Entry symbol not defined");
        return activateErrorFlag(stat);
    }
    else if (symBody -> attr2 == Extern){
    /*found in symbol table, checking the attr2 that is assumed to be used only for extern and entry attributes*/
            printErrorWithLocation(stat, "Symbol can't be both extern and entry");
            return activateErrorFlag(stat);
        }
    symBody -> attr2 = Entry;
    return Yes; /*multiple external symbols are acceptable as non error, but there is no need to add to  symbol table*/
}


/*


*/
Info parseNumbersData(char* line, STATUS* stat){
    int cursor = 0;
    short data;
    char dataString[MAX_LINE];

    while (line[0] != '\0'){
        trimWhiteSpaces(line); /*trim whitespaces from the what's left of line*/

        if (externalCommas(line)){
            printErrorWithLocation(stat, "Invalid commas");
            return Error;
        }
        cursor =  firstPosOfChar(line, COMMA); /*find the first colon position returns -1 if no comma found*/
        if (cursor != NOT_FOUND) { /*case comma found in data */
            strncpy(dataString, line ,cursor); /*take a piece of string up to comma, into dataString*/
            dataString[cursor] = '\0';
            trimWhiteSpaces(dataString); /*trim whitespaces from the piece of datastring*/
            trimNchars(line, cursor+1); /*remove the data string piece from line*/
        }
        else {
                strcpy(dataString, line);
                line[0] = '\0';
            }
        /*now we have a dataString that is supposed to be a data item*/
        if (isValidAsNumber(dataString) ){ /*check if all charachters are numbers (plus optional sign at the beginning)*/
            if (firstPosOfChar(dataString, DECIMAL_POINT) != NOT_FOUND){
                printErrorWithLocation(stat,"Fractions anf Floating point are not supported");
                return activateErrorFlag(stat);
            }
            data = atoi(dataString); /*invert string to number*/
            if (validInWordRange(data) == YES){
                addData(stat -> dataTable ,stat -> DC , data, 'A'); /*add data to data image*/
                (stat -> DC)++;
            }
            else{
                printErrorWithLocation(stat, "Data out of range");
                return activateErrorFlag(stat);
            }
        }
        else{/*not valid number*/
            printErrorWithLocation(stat, "Invalid data");
            stat -> errorExists = Yes;
            return Error;
        }
    }/*end while*/
    return Data;
}
