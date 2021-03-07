#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void addSymbol(LinkedList* symbolTable, short address, char* symbol, attribute attr1, attribute attr2){
    SYMBOL body;
    strcpy(body.symbol, symbol);
    body.attr1 = attr1;
    body.attr2 = attr2;
    appendNode(address, &body, symbolTable);
}

void addCode(CODE_IMG* codeTable, short address, char* inputLine, short code, char ARE){
    CODE_IMG body;
    strcpy(body.inputLine, inputLine);
    body.code = code;
    body.ARE = ARE;
    appendNode(address, &body, codeTable);
}

void addData(CODE_IMG* dataTable, short address, short data, char ARE){
    CODE_IMG body;
    body.data = code;
    body.ARE = ARE;
    appendNode(address, &body, dataTable);
}
