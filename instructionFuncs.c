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
        printf("line#%d: Error - Invalid instruction name\n", stat->lineNumber);
        activateErrorFlag(stat);
        return Error;
    }
    /*if ther is no INSTRUCTION_IDENTIFIER, it can be a command so it's not necesarily an error*/
    return No;
}



void parseData(char* line, Info type, STATUS* stat){
    if (strlen(line) == 0 ){
        printf("[Error] line#%d: Missing data\n", stat -> lineNumber);
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
        printf("[Error] line#%d: Missing string operand\n", stat -> lineNumber);
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
    printf("[Error] line#%d: Invalid string\n", stat -> lineNumber);
    return activateErrorFlag(stat);

}


/*assuming attr2 is used only for extern and entry attributes*/
Info parseExtern(char* line, STATUS* stat){
    SYMBOL* symBody;

    if (isEmptyString(line)){
        printf("[Error] line#%d: Missing operand after .extern instruction\n", stat -> lineNumber);
        return activateErrorFlag(stat);
    }
    if (firstPosOfChar(line,WHITE_SPACE)!= NOT_FOUND){
        printf("[Error] line#%d: Too many operands after .extern instruction\n", stat -> lineNumber);
        return activateErrorFlag(stat);
    }
    if (isValidAsSymbol(line, stat) == Error){
        printf("[Error] line#%d: Invalid symbol after .extern instruction\n", stat -> lineNumber);
        return activateErrorFlag(stat);
    }
    symBody = lookupSymbol(stat->symbolTable , line);

    if (symBody != NULL){ /*Found in symbol table*/
        if (symBody -> attr2 == Entry ){ /*found in symbol table, checking the attr2 that is assumed to be used only for extern and entry attributes*/
            printf("[Error] line#%d: Symbol can't be both extern and entry\n", stat -> lineNumber);
            return activateErrorFlag(stat);
        }
        symBody -> attr2 = Extern; /*DEBUG???*/
    }
    else addSymbol(stat-> symbolTable, 0 /*address value*/,  line/*label*/, Empty /*attr1*/, Extern /*attr2*/,  stat);

    return Yes; /*multiple external symbols are acceptable as non error, but there is no need to add to  symbol table*/
}

Info parseEntry(char* line, STATUS* stat){
    SYMBOL* symBody;

    if (isEmptyString(line)){
        printf("[Error] line#%d: Missing operand after .entry instruction\n", stat -> lineNumber);
        return activateErrorFlag(stat);
    }
    if (firstPosOfChar(line,WHITE_SPACE)!= NOT_FOUND){
        printf("[Error] line#%d: Too many operands after .entry instruction\n", stat -> lineNumber);
        return activateErrorFlag(stat);
    }
    if (isValidAsSymbol(line, stat) == Error){
        /*printf("[6][DEBUG] in ParseEntry changing attribute to Entry --|%s|--\n", line);*/

        printf("[Error] line#%d: Invalid symbol after .entry instruction\n", stat -> lineNumber);
        return activateErrorFlag(stat);
    }
    symBody = lookupSymbol(stat->symbolTable , line);
    /*printf("[7][DEBUG] in ParseEntry looking for label --|%s|--\n", line);*/

    if(symBody == NULL){
        /*printf("[8][DEBUG] in ParseEntry changing attribute to Entry --|%s|--\n", line);*/

        printf("[Error] line#%d: Entry symbol not defined\n", stat -> lineNumber);
        return activateErrorFlag(stat);
    }
    else if (symBody -> attr2 == Extern){ /*found in symbol table, checking the attr2 that is assumed to be used only for extern and entry attributes*/
        /*printf("[9][DEBUG] in ParseEntry changing attribute to Entry --|%s|--\n", line);*/

            printf("[Error] line#%d: Symbol can't be both extern and entry\n", stat -> lineNumber);
            return activateErrorFlag(stat);
        }
    /*printf("[10][DEBUG] in ParseEntry changing attribute to Entry --|%s|--\n", line);*/

    symBody -> attr2 = Entry;
    /*printf("[11][DEBUG] in ParseEntry changing attribute to Entry --|%s|--\n", line);*/

    return Yes; /*multiple external symbols are acceptable as non error, but there is no need to add to  symbol table*/
}



Info parseNumbersData(char* line, STATUS* stat){
    int cursor = 0;
    short data;
    char dataString[MAX_LINE];

    while (line[0] != '\0'){
        trimWhiteSpaces(line); /*trim whitespaces from the what's left of line*/

        if (externalCommas(line)){
            printf("[Error] line#%d: Invalid commas\n", stat -> lineNumber);
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
                printf("[Error] line#%d: Fractions are not supported\n", stat->lineNumber);
                return activateErrorFlag(stat);
            }
            data = atoi(dataString); /*invert string to number*/
            if (validInWordRange(data) == YES){
                addData(stat -> dataTable ,stat -> DC , data, 'A'); /*add data to data image*/
                /*fprintf(stderr,"[DEBUG] in parse string data printing list after addition of data %d at DC %d\n", data,stat -> DC);*/

                (stat -> DC)++;
            }
            else{
                printf("[Error] line#%d: Data out of range\n", stat->lineNumber);
                return activateErrorFlag(stat);
            }
        }
        else{/*not valid number*/
            printf("[Error] line#%d: Invalid data\n", stat->lineNumber);
            stat -> errorExists = Yes;
            return Error;
        }
    }/*end while*/
    return Data;
}
