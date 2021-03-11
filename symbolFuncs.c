#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asScanners.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"
#include "symbolFuncs.h"


/*
  Checks the possibility of the line to contain a label by searching for a ':'
  params:   char* line - the string to search for a label in
  returns:   1 if found, and 0 otherwise
*/
int containsLabelDef(char* line){
  if (strchr(line, LABEL_IDENTIFIER)!= NULL)
    return YES;
  return NO;
}

/* returns Yes/No/Error*/
Info parseSymbol(char* symbol , char* line, STATUS* stat){
    int cursor = 0;
    char tmpSymbol[MAX_LINE];
    Info valid;
    /*line is supposed to be clean of heading whitespaces at this point*/
    cursor =  firstPosOfChar(line, LABEL_IDENTIFIER); /*find the first colon position*/
    if ((strlen(line) == 0) || (cursor == NOT_FOUND))
        return No;

    strncpy(tmpSymbol, line, cursor ); /*get the string part until the colon sign*/
    tmpSymbol[cursor] = '\0';

    valid = isValidAsSymbol(tmpSymbol, stat);
    if (lookupSymbol(stat-> symbolTable, tmpSymbol) != NULL){
        printf("line# %d: Error - Label %s is already in symbol table\n", stat -> lineNumber, tmpSymbol);
        return Error;
    }
    if (valid == Yes){
        strcpy(symbol, tmpSymbol ); /*get the string part until the colon sign*/
        return Yes ;
    }
    return valid;
}


int validSymbolChars(char* symbol){ /*OK*/
    int cursor = 0;
    char c;
    if (strlen(symbol) == 0)
        return NO;
    c = symbol[0];
    if (c < 65 || (c > 90 && c < 97) || c > 122)
        return NO;

    while ( c != '\0'){
        if (c < 48 || (c>57 && c < 65) || (c > 90 && c < 97) || c > 122)
            return NO;
        c = symbol[++cursor];
    }
    return YES;
}


Info isValidAsSymbol(char* string, STATUS* stat){
    if (strlen(string) > MAX_LABEL){
        printf("line# %d: Error - Label is too long: %s  \n",  stat -> lineNumber,string);
        return Error;
    }
    if (!validSymbolChars(string)){
        printf("line# %d: Error - Invalid charachters in label %s \n",  stat -> lineNumber, string);
        return Error;
    }
    if (isReservedWord(string) == Yes){
        printf("line# %d: Error - Used reserved word for label \n",  stat -> lineNumber);
        return Error;
    }
    return Yes;
}
