/* Author: Ornit Cohen Gindi
Group of functions that deal with assembler data structures as init, add and lookup
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asStructsAndMacros.h"
#include "utilsAssembler.h"
#include "utilsGeneral.h"





/*
Searches through the symbol table for a given label (symbol)
params:
LinkedList* symbolTable - the able to search within.
char* symbol - the sembol to lookup.
returns: SYMBOL* - pointer to SYMBOL structure that has all the relevant dat fields of the  required symbol.
if none found , returns NULL
*/
SYMBOL* lookupSymbol(LinkedList* symbolTable, char* symbol){
    Node* cursor = symbolTable -> head;
    while (cursor != NULL) {
        if ( strcmp(cursor -> keyStr, symbol) == 0)
            return cursor -> body;
        cursor = cursor -> next;
    }
    return NULL;
}



/*
Initializes the STATUS structure for the current fileName
params:
STATUS* stat - pointer to the status structure to be initiallized
char* fileName - string of the current file name without extention
*/
void initStatus(STATUS* stat, char* fileName){
    stat -> IC = INIT_ADDRESS;    /*instructions counter*/
    stat -> DC = 0;      /*data counter*/
    stat -> ICF = 0;    /*instructions counter*/
    stat -> DCF = 0;
    stat -> lineNumber = 1;      /*line number in code*/
    stat -> errorForLine = No;
    stat -> errorExists = No;  /*errors flag*/
    stat -> symbolFound = No;  /*symbols flag*/
    strcpy(stat -> fileName , fileName);
    strcpy(stat -> label , EMPTY_STRING); /*to make it easier to fill in the missing details after the first pass*/
    stat -> commandNumber = -1;  /**/
    stat -> srcOpAddressType = Empty;
    stat -> destOpAddressType = Empty;
    stat -> symbolTable = linkedListInit(sizeof(SYMBOL));            /*starting a linked list of Symbols*/
    stat -> codeTable = linkedListInit(sizeof(CODE_IMG));            /*starting a linked list of code image*/
    stat -> dataTable = linkedListInit(sizeof(DATA_IMG));            /*starting a linked list of data image*/
}




/*
Turns the errorExists flag in STATUS  to Yes, once an error is Found
params:
STATUS* stat - in which the flag is a field
returns:
Info type Error value
*/
Info activateErrorFlag(STATUS* stat){
    stat -> errorExists = Yes;
    stat -> errorForLine = Yes;
    return Error;
}





/*
Gets the address type of the given type of operand.
params:
Info opType - source (Source) operand or destination (Dest) operand
STATUS* stat - in which the current operands address types are fields
returns:
Info type - address type values of operands: Immediate/Direct/Relative/Register
*/
Info getAddressType(Info opType,STATUS* stat){
    if (opType == Source)
        return stat -> srcOpAddressType;
    if (opType == Dest)
        return stat -> destOpAddressType;
    return Error;/*this line was just necessary for compiler. this function assumes validity, thus no error messages*/
}


/* Resets part of the field in status struct, for every line of the input file*/
void resetStatStructForLine(STATUS* stat){
    stat -> lineNumber ++;
    stat -> errorForLine = No;
    stat -> symbolFound = No;
    strcpy(stat -> label , EMPTY_STRING);
    stat -> commandNumber = -1;
    stat -> srcOpAddressType = Empty;
    stat -> destOpAddressType = Empty;
}


/*
Resets lineNumber to 1
*/
void resetforSecondScan(STATUS* stat){
    stat -> lineNumber =1;
}


/*
Adds a symbol to symbolTable
params: symbolTable - LinkedList pointer
short address - the address of the symbol
char* symbol - the label itself
Info attr1 - attributes Data/Code
Info attr2 - attributes Extern/Entry
STATUS* stat - pointer to statust structure, for easy access to current line number in input
*/
void addSymbol(LinkedList* symbolTable, short address, char* symbol, Info attr1, Info attr2, STATUS* stat){
    SYMBOL body;
    if (lookupSymbol(symbolTable, symbol) != NULL){
        printMessageWithLocation(Error, stat, "Label is already in symbol table");
        activateErrorFlag(stat);
    }
    body.address = address;
    body.attr1 = attr1;
    body.attr2 = attr2;
    appendNode( 0 , symbol , &body, symbolTable);
}


/*
Gets the bunch of fields of a SYMBOL structure
params:
Node* cursor -  pointer to head of symbolTable linkedList
returns:
SYMBOL* - pointer to the SYMBOL structure which is a field in Node
*/
SYMBOL* getSymbolBody (Node* cursor){
    return (SYMBOL*)(cursor -> body);
}


/*
Updates The symbol table at the end of the first scan
wherever there is a symbol that is related to dataTable ,its
 address should be shifted with the size of ICF
 params: STATUS* stat - to access symbolTable
  */
void updateSymbolTable(STATUS* stat){
    Node* cursor = stat -> symbolTable -> head;
    SYMBOL* body;

    while (cursor != NULL){

        body = getSymbolBody(cursor);

        if (  body -> attr1 == Data)
            (body -> address) += stat -> ICF;

        cursor = cursor -> next;
    }

}


/*
    Adds the data into the matching structure and linkedList.
    params:
    LinkedList* codeTable
    short address - the address of the current word, to be printed into the object file
    int lineNumber - the number of relevent line in the input file
    Info comment - other info that helps filling ithis table in the second scan
    char* label - if the current word is for a label, then this parameter contains the label string and stays empty otherwise
    short code - the word required in the object file
    Info ARE - the charachter 'A','E','E'  as required in the output object file , or FillLater if the character is not known yet -
 */
void addCode(LinkedList* codeTable, short address, int lineNumber, Info comment, char* label, short code, Info ARE){
    CODE_IMG body;
    strcpy(body.label, label);
    body.lineNumber = lineNumber;
    body.comment = comment;
    body.code = code;
    body.ARE = ARE;
    appendNode(address, EMPTY_STRING , &body, codeTable);
}


/*
Gets the bunch of fields of a CODE_IMG structure
params:
Node* cursor -  pointer to head of codeTable linkedList
returns:
CODE_IMG* - pointer to the CODE_IMG structure which is a field in Node
*/
CODE_IMG* getCodeImageBody (Node* cursor){
    return (CODE_IMG*)(cursor -> body);
}



/*
    Adds the data into the matching structure and linkedList.
    params:
    LinkedList* dataTable
    short address - the address of the current word, to be printed into the object file
    short data - the word required in the object file
 */
void addData(LinkedList* dataTable, short address, short data/*, Info ARE*/){
    DATA_IMG body = data;
    appendNode(address, EMPTY_STRING,  &body, dataTable);
}



/*
Gets the bunch of fields of a DATA_IMG structure
params:
Node* cursor -  pointer to head of dataTable linkedList
returns:
DATA_IMG* - pointer to the DATA_IMG structure which is a field in Node
*/
DATA_IMG* getDataImageBody (Node* cursor){
    return (DATA_IMG*)(cursor -> body);
}



/*
Frees all the mallocs of the linked lists of the three tables that stat holds
params:
STATUS* stat - pointer to STATUS that holds the malloced linked lists
*/
void freeMemory(STATUS* stat){
    killList(stat->symbolTable);
    killList(stat->codeTable);
    killList(stat->dataTable);
}
