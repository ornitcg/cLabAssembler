/*
  group of the big functions that are related to the assembler
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asScanners.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"
#include "symbolFuncs.h"
#include "instructionFuncs.h"
#include "commandFuncs.h"
#include "buildOutput.h"



void runAssembler(FILE* inputFile, char* fileName){
    STATUS stat;
    initStatus(&stat, fileName); /*to contain status details of current line*/
    /*fprintf(stderr, "******** DEBUG - in runAssembler\n");*/
    firstPass(inputFile, &stat);
    fseek(inputFile,0,SEEK_SET);
    secondPass(inputFile, &stat);
    freeMemory(&stat);
}

/*
  Performs the first pass over the assembly file
  params: FILE* inputFile - pointer to the input assembly file
  returns: 1 if errors occured and 0 otherwise
*/
void firstPass(FILE* inputFile , STATUS* stat){
    /***********************************************DECLARATIONS***********************************************************/
    char line[MAX_LINE] = EMPTY_STRING;        /*line of assembly code*/
    char symbol[MAX_LABEL] = EMPTY_STRING;     /*to contain the label from line*/
    char instruction[MAX_INSTRUCTION]= EMPTY_STRING;         /*to contain the label from line*/
    char command[MAX_CMD_LEN]= EMPTY_STRING;                 /*to contain the label from line*/
    Info instType;                             /*stands for instruction type*/
    /*********************************************************************************************************/
    fprintf(stderr,"%d DEBUG  STARTING FIRST PASS\n", stat->lineNumber);

    while(fgets(line, MAX_LINE, inputFile) != NULL){ /*each iteration of this loop is on a whole line from input file*/
        fprintf(stderr,"\n\n");
        fprintf(stderr,"[1]%d DEBUG FIRST PASS before first trim line is: --|%s|--\n", stat->lineNumber, line);

        trimWhiteSpaces(line);    /*removes whitespaces from both ends and also the '\n' for each line read from file*/


        /*fprintf(stderr,"[2]%d DEBUG FIRST PASS after first trim line : --|%s|--\n", stat->lineNumber, line);*/

        if (!toIgnore(line) ){
            /***********************************SYMBOL CHECK**************************************/
            /*fprintf(stderr,"[2] DEBUG firstpass1 firstpass\n");*/

            /*fprintf(stderr,"[3]%d DEBUG FIRST PASS inside to ignore--|%s|--\n", stat->lineNumber, line);*/

            stat -> symbolFound = parseSymbol(symbol, line, stat);  /*Yes/No/Error*/
              /*relevant to lines that begin with label definition*/
              /*fprintf(stderr,"[3] DEBUG firstpass1 firstpass\n");*/
             /*fprintf(stderr,"[4]%d DEBUG FIRST PASS symbol found is--|%s|--\n", stat->lineNumber, symbol);*/

            if (strlen(symbol) > 0) {
                /*fprintf(stderr,"[5]%d DEBUG  FIRST PASS line before trim symbol--|%s|--\n", stat->lineNumber, line);*/

                /*fprintf(stderr,"[3] DEBUG firstpass1 - line# %d The symbol that was found is --|%s|--\n", stat-> lineNumber, symbol);*/
                trimNchars(line, strlen(symbol)); /*restart line from end of symbol, which is empty if*/
                trimWhiteSpaces(line);
                /*fprintf(stderr,"[6]%d DEBUG  FIRST PASS line after trim symbol--|%s|--\n", stat->lineNumber, line);*/

                /*restart line from end of label, "+1" stand for the colon in the end*/
            }

            /*fprintf(stderr,"[7]%d DEBUG FIRST PASS line before parse instruction--|%s|--\n", stat->lineNumber, line);*/

            /**********************************INSTRUCTION CHECK***********************************/
            instType = parseInstruction(instruction, line, stat); /*Yes/No/Error*/
            /*fprintf(stderr,"[8]%d DEBUG FIRST PASS line after parse instruction--|%s|--\n", stat->lineNumber, line);*/


            if ( instType != Error && instType != No ){ /*then need to cut off instruction and trim spaces from rest of line*/
                /*fprintf(stderr,"[9]%d DEBUG FIRST PASS line before trim instruction--|%s|--\n", stat->lineNumber, line);*/

                trimNchars(line, strlen(instruction)); /*restart line from end of instruction, which is empty if*/
                trimWhiteSpaces(line);

                /*fprintf(stderr,"[10]%d DEBUG FIRST PASS line after trim instruction--|%s|--\n", stat->lineNumber, line);*/
                /*fprintf(stderr,"[11]%d DEBUG FIRST PASS  instruction--|%s|--\n", stat->lineNumber, instruction);*/

                /*fprintf(stderr,"DEBUG firstpass2 - line# %d The Istruction that was found is --|%s|--\n", stat-> lineNumber, instruction);*/

            } /*else there was a problem in finding a command*/

            if  ( instType == Data || instType == String ){

                if (stat -> symbolFound == Yes){
                    /*fprintf(stderr,"[3.5] DEBUG firstpass - DATA/STRING preceeding symbol : --|%s|--\n", symbol);*/

                    addSymbol(stat -> symbolTable, stat-> DC , symbol, Data, Empty);
                    /*fprintf(stderr,"[3.5] DEBUG firstpass - DATA/STRING preceeding symbol : --|%s|--\n", symbol);*/

                    printList(stat -> symbolTable, 'T');  /*DEBUG*/
                }
                parseData(line, instType, stat);/*return 0 if data is wrong or None*/
            }
            if ( instType == Entry ){
                do_nothing();
            }/*end if ( instType == Extern )*/
            if ( instType == Extern ){
                if (parseExtern(line, stat) == Ok && (stat -> symbolFound == Yes )){
                    printf("[WARNING]  line#%d: Label definition before .extern instruction", stat -> lineNumber);/*NOT ERROR*/
                    /*addSymbol(stat -> symbolTable, 0 , line , Empty, Extern);*/ /*DEBUG -wrong code*/
                    printList(stat -> symbolTable, 'T');  /*DEBUG*/
                    /*at this point what's left of line is the operand that passed the test of parseExtern*/
                }
            }/*end if ( instType == Extern )*/
            /*********************************COMMAND CHECK**************************************/
            if ( instType == No ){/* meaning no instruction found- proceed to parsing command and operands*/
                /*fprintf(stderr," [6] DEBUG firstpass3 - line# %d the rest of line: --|%s|--\n", stat-> lineNumber, line);*/
                if (stat -> symbolFound == Yes){
                    addSymbol(stat -> symbolTable, stat->IC , symbol,  Code, Empty);
                /*fprintf(stderr," [7] DEBUG firstpass3 - line# %d the rest of line: --|%s|--\n", stat-> lineNumber, line);*/
                    printList(stat -> symbolTable, 'T');  /*DEBUG*/
                }
                if (parseCommand(command, line,  stat) == Ok){
                    /*fprintf(stderr," [8] DEBUG firstpass3 - line# %d the rest of line: --|%s|--\n", stat-> lineNumber, line);*/

                    trimWhiteSpaces(&line[strlen(command)]); /*restart line from end of command*/
                    /*fprintf(stderr," [9] DEBUG firstpass3 - line# %d the rest of line: --|%s|--\n", stat-> lineNumber, line);*/

                    parseCommandOperands(line, stat);
                }
                /*fprintf(stderr," [10] DEBUG firstpass3 - line# %d the rest of line: --|%s|--\n", stat-> lineNumber, line);*/

            }
        }/*end if not ignore*/
        resetStatStructForLine(stat); /*resets part of the fields, to use for next line's data*/
    }/*end while*/
    stat -> ICF = stat -> IC;
    stat ->  DCF = stat -> DC;
    updateSymbolTable(stat);

}/*end of firstPass*/


void secondPass(FILE* inputFile, STATUS* stat){
    char line[MAX_LINE] = EMPTY_STRING;        /*line of assembly code*/
    char symbol[MAX_LABEL] = EMPTY_STRING;     /*to contain the label from line*/
    char instruction[MAX_INSTRUCTION] = EMPTY_STRING;         /*to contain the label from line*/
    /*char command[MAX_CMD_LEN] = EMPTY_STRING; */                /*to contain the label from line*/
    Info instType;                             /*stands for instruction type*/

    /*********************************************************************************************************/
    fprintf(stderr," \n\n[DEBUG] SECOND PASS \n\n");

    while(fgets(line, MAX_LINE, inputFile) != NULL){ /*each iteration of this loop is on a whole line from input file*/

        trimWhiteSpaces(line);        /*removes whitespaces from both ends and also the '\n' for each line read from file*/
        /*fprintf(stderr,"\n\n");*/
        if (!toIgnore(line) ){
            /***********************************SYMBOL CHECK**************************************/
            stat -> symbolFound = parseSymbol(symbol, line, stat);  /*Yes/No/Error*/
              /*relevant to lines that begin with label definition*/
            if (strlen(symbol) > 0){
                /*fprintf(stderr,"DEBUG secondPass - line# %d The symbol that was found is --|%s|--\n", stat-> lineNumber, symbol);*/
                trimWhiteSpaces(&line[strlen(symbol)+1]);
                /*restart line from end of label, "+1" stand for the colon in the end*/
            }
            /**********************************INSTRUCTION CHECK***********************************/
            instType = parseInstruction(instruction, line, stat); /*Yes/No/Error*/

            if ( instType != Error && instType != No ){
                /*then need to cut off instruction and trim spaces from rest of line*/
                trimWhiteSpaces(&line[strlen(instruction)]);
                 /*restart line from end of instruction, which is empty if*/
                /*fprintf(stderr,"DEBUG firstpass2 - line# %d The Istruction that was found is --|%s|--\n", stat-> lineNumber, instruction);*/
            } /*there was a problen in parsing*/
            if ( instType == Entry ){
                if (parseEntry(line, stat) == Ok && (stat -> symbolFound == Yes ))
                    printf("[WARNING]  line#%d: Label definition before .extern instruction", stat -> lineNumber);/*NOT ERROR*/
                    /*at this point what's left of line is the operand that passed the test of parseEntry*/
            }/*end if ( instType == Extern )*/
            /*********************************COMMAND CHECK**************************************/
            if ( instType == No ){/* meaning no instruction found- proceed to parsing command and operands*/
                fillMissingDetailsInCodeTable(stat);
            }
        }/*end if not ignore*/
        resetStatStructForLine(stat); /*resets part of the fields, to use for next line's data*/

        if (stat -> errorExists == No){
            buildObjectFile(stat);
            buildEntriesFile(stat);
            buildExternalsFile(stat);
        }
    }/*end while*/

}

void fillMissingDetailsInCodeTable(STATUS* stat){
    /*At this point the Symbol table is full, the data*/
    Node* cursor = stat -> codeTable -> head;
    int currentAddress;
    CODE_IMG* body = NULL;
    SYMBOL* symbol = NULL;
    while (cursor != NULL){
        currentAddress = cursor -> keyNum;
        body = getCodeImageBody(cursor); /*The bunch of fields of code image linked list  at this cursor locaion*/
        symbol = lookupSymbol(stat -> symbolTable, body-> label); /*symbol points at the body of the symbol table link thatbelongs to the symbol at cursor location,if this location does not relate to a symbol, Null will be returned*/
        if ( body-> label != NULL && symbol == NULL ){ /*any operand symbol should be found in the symbol table, thus error*/
            stat -> errorExists = Yes;
            return;
        }
        if ( body -> comment == Direct)
            body -> code = symbol -> address;

        if ( body -> comment == Relative)
            body -> code = (symbol-> address) - currentAddress;

        cursor = cursor -> next;
    }
}
