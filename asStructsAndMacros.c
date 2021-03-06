#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
void addSymbol(node** head, int dcStat, char* symbol, int attr1, int attr2){
    SYMBOL_TABLE* newNode;
    newNode.dcStat = dcStat;
    strcpy(newNode.symbol, symbol);
    newNode.attr1 = attr1;
    newNode.attr2 = attr2;
    appendNode( head,  newNode );
}*/
void updateDataImage(){

    
}

void addSymbol(SYMBOL* symbolTable, int dcStat, char* symbol, int attr1, int attr2){
    SYMBOL* newNode = (SYMBOL*)malloc(sizeof(SYMBOL));
    SYMBOL* cursor = symbolTable;
    (*newNode).dcStat = dcStat;
    strcpy((*newNode).symbol, symbol);
    (*newNode).attr1 = attr1;
    (*newNode).attr2 = attr2;
    (*newNode).next = NULL;
    if (symbolTable = NULL)
        symbolTable = newNode;
    else {
        while (cursor.next != NULL)
            cursor = cursor.next;
        cursor.next = newNode;
    }
}

void addCode(CODE_IMG* codeImg, int icStat, char* inputLine, short data, short code){
    CODE_IMG* newNode = (CODE_IMG*)malloc(sizeof(CODE_IMG));
    CODE_IMG* cursor = codeImg;
    int icStat;
    char inputLine[MAX_LINE];
    short data;
    short code;
    (*newNode).next = NULL;
    if (codeImg = NULL){
        codeImg = newNode;
        newNode.prev = codeImg;
    }
    else {
        while (cursor.next != NULL)
            cursor = cursor.next;
        cursor.next = newNode;
        newNode.prev = cursor
    }
}

void addData(DATA_IMG* dataImg, short data){
    DATA_IMG* newNode = (DATA_IMG*)malloc(sizeof(DATA_IMG));
    DATA_IMG* cursor = dataImg;
    (*newNode).data = data;
    (*newNode).next = NULL;
    if (dataImg = NULL){
        dataImg = newNode;
        newNode.prev = dataImg;
    }
    else {
        while (cursor.next != NULL)
            cursor = cursor.next;
        cursor.next = newNode;
        newNode.prev = cursor;

    }
}
