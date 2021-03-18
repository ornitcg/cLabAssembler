/*
Author: Ornit Cohen Gindi
This group of functions deal with parsing a symbol and checkig its validity
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asScanners.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"
#include "symbolFuncs.h"



/*
Finds a definition of symbol at beggining of line.
params:

char* line - the line to search within.
STATUS* stat - for easy access to status info at error messages
returns - Info (enum) Yes if a valid symbol was found, the defined symbol then in added to status
                     No - if no symbol was found
                     Error - if error found
*/
Info parseSymbol( char* line, STATUS* stat){
    int cursor = 0;
    char tmpSymbol[MAX_LINE] = EMPTY_STRING;

    /*line is supposed to be clean of heading whitespaces at this point*/
    cursor =  firstPosOfChar(line, LABEL_IDENTIFIER); /*find the first colon position*/
    if ((strlen(line) == 0) || (cursor == NOT_FOUND))
        return No;
    strncpy(tmpSymbol, line, cursor ); /*get the string part until the colon sign*/
    tmpSymbol[cursor] = '\0';
    if (isEmptyString(tmpSymbol) == Yes){
        printMessageWithLocation(Error,stat,"Expected label to define");
        return activateErrorFlag(stat);
    }

    if (isValidAsSymbol(tmpSymbol, stat) == Yes){
        strcpy(stat ->label, tmpSymbol);
        stat -> symbolFound =  Yes;
        return Yes ;
    }
    else
        return activateErrorFlag(stat);

}

/*
Checks if symbol characters are all valid.
params: char* symbol - the symbol to check
returns: int - Yes if symbol contains invalid symbol
and No otherwise.
a No in not necessarily an error , thus No
*/
int validSymbolChars(char* symbol){ /*OK*/
    int cursor = 0;
    char c;
    if (isEmptyString(symbol) == YES)
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




/*
Checks if a given string is valid as a symbol.
params:
char* string - the string to check if valid as symbol
STATUS* stat - for easy access to status info used in error messages
returns: Info type - Yes if string is valid as symbol, and Error otherwise
assumes string is a whole symbol without whitespaces. if there are any it's an error
*/
Info isValidAsSymbol(char* string, STATUS* stat){

    if (strlen(string) > MAX_LABEL){
        printMessageWithLocation(Error, stat, "Label is too long");
        return activateErrorFlag(stat);
    }
    if (!validSymbolChars(string)){
        printMessageWithLocation(Error, stat, "Invalid characters in label");
        return activateErrorFlag(stat);
    }
    if (isReservedWord(string) == Yes){
        printMessageWithLocation(Error, stat, "Used reserved word for label");
        return activateErrorFlag(stat);
    }
    return Yes;
}
