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
    instruction = EMPTY_STRING;
    /*fprintf(stderr, "DEBUG in parseLabel for line: --%s--\n",line);*/
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
        stat -> errorExists = Yes;
        instruction = EMPTY_STRING;
        return Error;
    }
    /*if ther is no INSTRUCTION_IDENTIFIER, it can be a command so it's not necesarily an error*/
    return No;
}



void parseData(char* line, Info type, STATUS* stat){
    if (strlen(line) == 0 ){
        printf("line#%d: Error - Missing data\n", stat -> lineNumber);
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
        printf("line#%d: Error - Missing string operand\n", stat -> lineNumber);
        stat -> errorExists = Yes;
        return Error;
    }
    strEndInd = strlen(string)-1;
    if(string[0] == QUOTE && string[strEndInd]== QUOTE){
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
    printf("line#%d: Error - Invalid string\n", stat -> lineNumber);
    stat -> errorExists = Yes;
    return Error;

}


/*assuming attr2 is used only for extern and entry attributes*/
Info parseExtern(char* line, STATUS* stat){
    SYMBOL* symBody;
    symBody = lookupSymbol(stat->symbolTable , line);
    if (symBody == NULL) /*not Found in symbol table*/
        return Ok;
    if (symBody -> attr2 == Extern ||symBody -> attr2 == Entry ){ /*found in symbol table, checking the attr2 that is assumed to be used only for extern and entry attributes*/
        stat -> errorExists = Yes;
        return Error;
    }
    return No; /*multiple external symbols are acceptable as non error, but there is no need to add to  symbol table*/
}

Info parseNumbersData(char* line, STATUS* stat){
    int cursor = 0;
    short data;
    char dataString[MAX_LINE];
    while (line[0] != '\0'){
        if (externalCommas(line)){
            printf("line#%d: Error - Invalid commas\n", stat -> lineNumber);
            return Error;
        }
        cursor =  firstPosOfChar(line, COMMA); /*find the first colon position returns -1 if no comma found*/
        if (cursor > -1) { /*case comma found in data */
            strncpy(dataString, line ,cursor); /*take a piece of string up to comma, into dataString*/
            strcpy(line, trimWhiteSpaces(&line[strlen(dataString)+1])); /*remove the data string piece from line, and trim whitespaces*/
            strcpy(dataString, trimWhiteSpaces(dataString)); /*trim whitespaces from the piece of datastring*/
        }
        else {
                strcpy(dataString, line);
                line[0] = '\0';
            }
        /*now we have a dataString that is supposed to be a data item*/
        if (isValidAsNumber(dataString) ){ /*check if all charachters are numbers (plus optional sign at the beginning)*/
            data = atoi(dataString); /*invert string to number*/
            if (validInWordRange(data) == YES){
                addData(stat -> dataTable ,stat -> DC , data, 'A'); /*add data to data image*/
                (stat -> DC)++;
            }
            else{
                printf("line#%d: Error - Data out of range\n", stat->lineNumber);
                stat -> errorExists = Yes;
                return Error;
            }
        }
        else{
            printf("line#%d: Error - Invalid data\n", stat->lineNumber);
            stat -> errorExists = Yes;
            return Error;
        }
    }/*end while*/
    return Data;
}
