/*
  Group of helper functions that are related to assembler
*/
#include <stdio.h>
#include <string.h>
#include "as_funcs.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"

void DO_SOMETHING(){
  /*for place holding*/
}

int detectedCommand(char* str){
    return NO;
}
void initStatus(STATUS* stat){
    (*stat).IC = INIT_ADDRESS;    /*instructions counter*/
    (*stat).DC = 0;      /*data counter*/
    (*stat).lineNumber = 1;      /*line number in code*/
    (*stat).errorExists = NO;  /*errors flag*/
    (*stat).symbolFound = NO;  /*symbols flag*/
}

int parseData(char* line, DATA_IMG** dataImg, int statDC){
    int dc = 0;
    int cursor = 0;
    short data;
    char dataString[MAX_LINE];

    if (strlen(line) == 0 ){
        printf("line#%d: Error - Missing data");
        stat.errorExists = YES;
        return dc; /*returns 0 number of data arguments*/
    }
    while (line[0] != '\0'){
        if (externalCommas(line)){
            printf("line#%d: Error - Invalid commas");
            stat.errorExists = YES;
            return dc;
        }
        cursor =  firstPosOfChar(line, COMMA); /*find the first colon position*/
        if (cursor > -1) { /*no commas in data */
            strncopy(dataString, line ,cursor);
            strcpy(line, trimWhiteSpaces(&line[strlen(dataString)+1])); /*remove the data string piece from line, and trim whitespaces*/
            strcpy(dataString, trimWhiteSpaces(dataString)); /*trim whitespaces from the piece of datastring*/
        }
        else {
                strcopy(dataString, line);
                line[0] = '\0';
            }
        /*now we have a dataString that is supposed to be a data item*/
        if (validAsNumber(dataString)){ /*check if all charachters are numbers (plus optional sign at the beginning)*/
            data = atoi(dataString); /*invert string to number*/
            updateDataImage(data, statDC, dataImg); /*add data to data image*/
            statDC ++;
        }
        else{
            printf("line#%d: Error - Invalid data");
            stat.errorExists = YES;
            return statDC;
        }
    }/*end while*/
    return dc; /*number of data items found*/
}



/*need to add the line numbers*/
enum inst parseSymbol(char* symbol , char* line){
    int cursor = 0;
    fprintf(stderr, "DEBUG in parseLabel for line: --%s--\n",line);
    /*line is supposed to be clean of heading whitespaces at this point*/
    cursor =  firstPosOfChar(line, LABEL_IDENTIFIER); /*find the first colon position*/

    if ((strlen(line) == 0) || (cursor == -1)){
        return NO;
    }
    strncpy(symbol, line, cursor ); /*get the string part until the colon sign*/
    symbol[cursor] = '\0';
    if (cursor >= MAX_LABEL){
        fprintf(stderr,"line#: Label is too long: %s  \n", symbol);
        return NO;
    }
    if (!validSymbolChars(symbol)){
        fprintf(stderr, "line# : Invalid charachters in label %s \n", symbol);
        return NO;
    }
    return YES ;
}


/*need to add the line numbers*/
int  parseLabel(char* instruction , char* line, STATUS* stat){
    int cursor = 0;
    fprintf(stderr, "DEBUG in parseLabel for line: --%s--\n",line);
    /*line is supposed to be clean of heading whitespaces at this point*/
    cursor =  firstPosOfChar(line, WHITE_SPACE); /*find the first colon position*/
    if ((strlen(line) == 0) || (cursor == -1)){
        return NO;
    }
    strncpy(instruction, line, cursor ); /*get the string part until the colon sign*/
    instruction[cursor] = '\0';
    if (instruction[0] == INSTRUCTION_IDENTIFIER){
        if (strcmp(instruction, ".data") == 0) return Data;
        if (strcmp(instruction, ".string") == 0) return String;
        if (strcmp(instruction, ".extern") == 0) return Extern;
        if (strcmp(instruction, ".entry") == 0) return Entry;

    }
    return YES ;
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



/*
  Checks the possibility of the line to contain a label by searching for a ':'
  params:   char* line - the string to search for a label in
  returns:   1 if found, and 0 otherwise
*/
int containsLabelDef(char* line){
  if (strchr(line, LABEL_IDENTIFIER_INT)!= NULL)
    return YES;
  return NO;
}


int toIgnore(char* line){
    if (strlen(line) == 0 || line[0] == COMMENT_IDENTIFIER_INT)
        return YES;
    return NO;
}

/*
Builds the first worb from the four fields of the parameter struct
params: FIRST_WORD type of struct that contains 4 fields
returns: a short int output composed of the 4 fields with the right shifting
assuming the input is correct
*/
short int buildFirstWord(FIRST_WORD w){
  short int res;
  w.src = w.src << 2;
  w.funct = w.funct << 4;
  w.opcode = w.opcode << 8 ;
  res =  w.dest | w.src |w.funct | w.opcode ;
  return res;
}
