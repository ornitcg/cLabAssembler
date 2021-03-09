/*
  Group of helper functions that are related to assembler
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "as_funcs.h"
#include "linkedList.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"

void DO_SOMETHING(){
  /*for place holding*/
}

info parseCommandOperands(char* line, STATUS* stat){

    return Ok;
}


info parseCommand(char* command, char* line, STATUS* stat ){
    int ind;
    /*at this point , line is supposed to be clean of heading whitespaces */
    int cursor= firstPosOfChar(line, WHITE_SPACE);       /*find the first whitespace position*/

    /*printf( "DEBUG in parseCommand for line: --%d--\n",stat-> lineNumber);*/
    if (strlen(line) == 0){
        printf("line#%d : Error - Command expected\n", stat -> lineNumber);
        return Error;
    }
    if (cursor == NOT_FOUND)
        strcpy(command, line);
    else strncpy(command, line, cursor);
    ind = lookupCommand(command);
    if (ind == NOT_FOUND){
        printf("line#%d: Error - Invalid command\n", stat -> lineNumber);
        return Error;
    }
    stat -> commandNumber = ind; /*adding the commandNumber to STATUS to make access easier*/
    return Ok;
}



/*assuming attr2 is used only for extern and entry attributes*/
info parseExtern(char* line, STATUS* stat){
    SYMBOL* symBody;
    symBody = lookupSymbol(stat->symbolTable , line);
    if (symBody == NULL) /*not Found in symbol table*/
        return Ok;
    if (symBody -> attr2 == Extern ||symBody -> attr2 == Entry ) /*found in symbol table, checking the attr2 that is assumed to be used only for extern and entry attributes*/
        return Error;
    return No; /*multiple external symbols are acceptable as non error, but there is no need to add to  symbol table*/
}


info parseStringData(char* string, STATUS* stat){
    int strEndInd;
    if (strlen(string) ==0){
        printf("line#%d: Error - Missing string operand\n", stat -> lineNumber);
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
    return Error;

}

info parseNumbersData(char* line, STATUS* stat){
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
        if (validAsNumber(dataString) ){ /*check if all charachters are numbers (plus optional sign at the beginning)*/
            data = atoi(dataString); /*invert string to number*/
            if (validInWordRange(data) == YES){
                addData(stat -> dataTable ,stat -> DC , data, 'A'); /*add data to data image*/
                (stat -> DC)++;
            }
            else{
                printf("line#%d: Error - Data out of range\n", stat->lineNumber);
                return Error;
            }
        }
        else{
            printf("line#%d: Error - Invalid data\n", stat->lineNumber);
            return Error;
        }
    }/*end while*/
    return Data;
}


info parseData(char* line, info type, STATUS* stat){
    if (strlen(line) == 0 ){
        printf("line#%d: Error - Missing data\n", stat -> lineNumber);
        return Error;
    }
    if (type == String)
        return parseStringData(line, stat);
    else
        return parseNumbersData(line, stat);
}

/* returns Yes/No/Error*/
info parseSymbol(char* symbol , char* line, STATUS* stat){
    int cursor = 0;
    char tmpSymbol[MAX_LINE];
    info valid;
    /*printf( "DEBUG in parseLabel for line: --%s--\n",line);*/
    /*line is supposed to be clean of heading whitespaces at this point*/
    cursor =  firstPosOfChar(line, LABEL_IDENTIFIER); /*find the first colon position*/
    if ((strlen(line) == 0) || (cursor == NOT_FOUND)){
        return No;
    }
    strncpy(tmpSymbol, line, cursor ); /*get the string part until the colon sign*/
    tmpSymbol[cursor] = '\0';
    valid = validAsSymbol(tmpSymbol, stat);
    if (lookupSymbol(stat-> symbolTable, tmpSymbol) != NULL){
        printf("line# %d: Error - Label %s is already in symbol table\n", stat -> lineNumber, tmpSymbol);
        return Error;
    }
    if (valid == Yes){
        strncpy(symbol, line, cursor ); /*get the string part until the colon sign*/
        return Yes ;
    }
    return valid;
}

info validAsSymbol(char* string, STATUS* stat){
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

info isReservedWord(char* string){
    SET_RESERVED_ARR(resv);
    /*printf("reserved word %s\n", resv[4]) ;*/

        return Yes;
    return No;
}

int lookupRegister(char* string){
    SET_REG_ARR(regs);
    int i;
    for(i = 0; i < REGISTERS_NUMBER ; i++)
        if(strcmp(regs[i],string) ==0 )
            return i;
    return NOT_FOUND;
}

int lookupCommand(char* string){
    int i;
    SET_COMMAND_TABLE(cmdTable);

    for(i = 0; i < COMMANDS_NUMBER ; i++)
        if(strcmp(cmdTable[i].command,string) == 0 )
            return i;
    return NOT_FOUND;
}

/*need to add the line numbers*/
info  parseInstruction(char* instruction , char* line, STATUS* stat){
    int cursor = 0;
    /*fprintf(stderr, "DEBUG in parseLabel for line: --%s--\n",line);*/
    /*line is supposed to be clean of heading whitespaces at this point*/
    cursor =  firstPosOfChar(line, WHITE_SPACE); /*find the first colon position*/
    if ((strlen(line) == 0) || (cursor == NOT_FOUND)){
        instruction = EMPTY_STRING;
        return No;
    }
    strncpy(instruction, line, cursor ); /*get the string part until the colon sign*/
    instruction[cursor] = '\0';
    if (instruction[0] == INSTRUCTION_IDENTIFIER){
        if (strcmp(instruction, ".data") == 0) return Data;
        if (strcmp(instruction, ".string") == 0) return String;
        if (strcmp(instruction, ".extern") == 0) return Extern;
        if (strcmp(instruction, ".entry") == 0) return Entry;
        printf("line#%d: Error - Invalid instruction name\n", stat->lineNumber);
        instruction = EMPTY_STRING;
        return Error;
    }
    instruction = EMPTY_STRING;
    return No;
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
  if (strchr(line, LABEL_IDENTIFIER)!= NULL)
    return YES;
  return NO;
}

int validInWordRange(short num){
    if(num >= 2048 && num <= 2047)
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
