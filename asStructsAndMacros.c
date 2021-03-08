#include <stdio.h>
#include <string.h>
#include <stdlib.h>

SYMBOL* lookupSymbol(LinkedList* symbolTable, char* symbol){
    Node* cursor = symbolTable -> head;
    while (cursor != NULL) {
        if ( strcmp(cursor.keyStr, symbol) == 0)
            return cursor -> body;
        cursor = cursor.next;
    }
    return NULL;
}


void initStatus(STATUS* stat, char* fileName){
    stat -> IC = INIT_ADDRESS;    /*instructions counter*/
    stat -> DC = 0;      /*data counter*/
    stat -> lineNumber = 1;      /*line number in code*/
    stat -> errorExists = NO;  /*errors flag*/
    stat -> symbolFound = NO;  /*symbols flag*/
    stat -> filename = fileName;
    stat ->  symbolTable = linkedListInit(sizeof(SYMBOL));            /*starting a linked list of Symbols*/
    stat ->  codeTable = linkedListInit(sizeof(CODE_IMG));            /*starting a linked list of code image*/
    stat ->  dataTable = linkedListInit(sizeof(DATA_IMG));            /*starting a linked list of data image*/
}

void addSymbol(LinkedList* symbolTable, short address, char* symbol, attribute attr1, attribute attr2){
    SYMBOL body;
    body.address = address;
    body.attr1 = attr1;
    body.attr2 = attr2;
    appendNode( 0 , symbol , &body, symbolTable);
}

void addCode(LinkedList* codeTable, short address, char* inputLine, short code, char ARE){
    CODE_IMG body;
    strcpy(body.inputLine, inputLine);
    body.code = code;
    body.ARE = ARE;
    appendNode(address,EMPTY_KEYSTR , &body, codeTable);
}

void addData(LinkedList* dataTable, short address, short data, char ARE){
    CODE_IMG body;
    body.data = code;
    body.ARE = ARE;
    appendNode(address, EMPTY_KEYSTR,  &body, dataTable);
}
