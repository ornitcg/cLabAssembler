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
    stat -> lineNumber = 1;      /*line number in code*/
    stat -> errorExists = NO;  /*errors flag*/
    stat -> symbolFound = NO;  /*symbols flag*/
    strcpy(stat -> fileName , fileName);
    strcpy(stat -> label , EMPTY_STRING);
    stat -> commandNumber = -1;
    stat -> srcOpAddressType = Empty;
    stat -> targetOpAddressType = Empty;
    stat -> symbolTable = linkedListInit(sizeof(SYMBOL));            /*starting a linked list of Symbols*/
    stat -> codeTable = linkedListInit(sizeof(CODE_IMG));            /*starting a linked list of code image*/
    stat -> dataTable = linkedListInit(sizeof(DATA_IMG));            /*starting a linked list of data image*/
}

void addSymbol(LinkedList* symbolTable, short address, char* symbol, Info attr1, Info attr2){
    SYMBOL body;
    body.address = address;
    body.attr1 = attr1;
    body.attr2 = attr2;
    appendNode( 0 , symbol , &body, symbolTable);
}

void addCode(LinkedList* codeTable, short address, Info comment, char* label, short code, Info ARE){
    CODE_IMG body;
    strcpy(body.label, label);
    body.comment = comment;
    body.code = code;
    body.ARE = ARE;
    appendNode(address, EMPTY_STRING , &body, codeTable);
}

void addData(LinkedList* dataTable, short address, short code, Info ARE){
    CODE_IMG body;
    body.code = code;
    body.ARE = ARE;
    appendNode(address, EMPTY_STRING,  &body, dataTable);
}
