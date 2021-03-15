/* Group of functions that deal with assembler data structures as init, add and lookup */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asStructsAndMacros.h"
#include "utilsAssembler.h"
#include "utilsGeneral.h"


/* searches through the symbol table for a given label (symbol)
if the labels is found, its other attributes are returnded with pointer
otherwise , returns NULL*/

SYMBOL* lookupSymbol(LinkedList* symbolTable, char* symbol){
    Node* cursor = symbolTable -> head;
    while (cursor != NULL) {
        if ( strcmp(cursor -> keyStr, symbol) == 0)
            return cursor -> body;
        cursor = cursor -> next;
    }
    return NULL;
}


void initStatus(STATUS* stat, char* fileName){
    stat -> IC = INIT_ADDRESS;    /*instructions counter*/
    stat -> DC = 0;      /*data counter*/
    stat -> ICF = 0;    /*instructions counter*/
    stat -> DCF = 0;
    stat -> lineNumber = 1;      /*line number in code*/
    stat -> errorExists = No;  /*errors flag*/
    stat -> symbolFound = No;  /*symbols flag*/
    strcpy(stat -> fileName , fileName);
    strcpy(stat -> label , EMPTY_STRING);
    stat -> commandNumber = -1;
    stat -> srcOpAddressType = Empty;
    stat -> destOpAddressType = Empty;
    stat -> symbolTable = linkedListInit(sizeof(SYMBOL));            /*starting a linked list of Symbols*/
    stat -> codeTable = linkedListInit(sizeof(CODE_IMG));            /*starting a linked list of code image*/
    stat -> dataTable = linkedListInit(sizeof(DATA_IMG));            /*starting a linked list of data image*/
}
Info activateErrorFlag(STATUS* stat){
    stat -> errorExists = Yes;
    return Error;
}

Info getAddressType(Info opType,STATUS* stat){
    if (opType == Source)
        return stat -> srcOpAddressType;
    if (opType == Dest)
        return stat -> destOpAddressType;
    return Error;/*this line was just necesary for compiler. this function assumes validity, thus no error messages*/
}

void resetStatStructForLine(STATUS* stat){
    stat -> lineNumber ++;
    stat -> symbolFound = No;
    strcpy(stat -> label , EMPTY_STRING);
    stat -> commandNumber = -1;
    stat -> srcOpAddressType = Empty;
    stat -> destOpAddressType = Empty;
}

void printList(LinkedList* ll, char type){
    Node* cursor = ll->head;
    SYMBOL* symbody =NULL;

    /*fprintf(stderr," [1] in printList\n");*/

    while (cursor != NULL){

        if (type == 'T'){
            symbody = getSymbolBody(cursor);
            fprintf(stderr, "DEBUG -in printList key is %s The value id %d attr2 =",cursor -> keyStr, symbody -> address);
            printEnumName(symbody->attr2);
            printf("\n");
        }
        if (type == 'N')
            fprintf(stderr, "DEBUG -in printList key is %04d\n",cursor -> keyNum);
        cursor = cursor -> next;
    }
}

void addSymbol(LinkedList* symbolTable, short address, char* symbol, Info attr1, Info attr2, STATUS* stat){
    SYMBOL body;
    if (lookupSymbol(symbolTable, symbol) != NULL){
        /*fprintf(stderr,"[1] DEBUG parse symbol \n");*/
        printf("[Error] line# %d: Label %s is already in symbol table\n", stat -> lineNumber, symbol);
        activateErrorFlag(stat);
    }
    /*fprintf(stderr," [1] DEBUG addSymbol - added --|%s|--\n", symbol);*/
    body.address = address;
    body.attr1 = attr1;
    body.attr2 = attr2;
    appendNode( 0 , symbol , &body, symbolTable);
}


SYMBOL* getSymbolBody (Node* cursor){
    return (SYMBOL*)(cursor -> body);
}

/* Updates The symbol table at the end of the first pass
wherever there is a symbol that is related to dataTableits
 address should be shifted with the size of ICF */
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




void addCode(LinkedList* codeTable, short address, int lineNumber, Info comment, char* label, short code, Info ARE){
    CODE_IMG body;
    strcpy(body.label, label);
    body.lineNumber = lineNumber;
    body.comment = comment;
    body.code = code;
    body.ARE = ARE;
    appendNode(address, EMPTY_STRING , &body, codeTable);
}

CODE_IMG* getCodeImageBody (Node* cursor){
    return (CODE_IMG*)(cursor -> body);
}

void addData(LinkedList* dataTable, short address, short data, Info ARE){
    DATA_IMG body;
    body.data = data;
    body.ARE = ARE;
    appendNode(address, EMPTY_STRING,  &body, dataTable);
}

DATA_IMG* getDataImageBody (Node* cursor){
    return (DATA_IMG*)(cursor -> body);
}


void freeMemory(STATUS* stat){
    killList(stat->symbolTable);
    killList(stat->codeTable);
    killList(stat->dataTable);
}
