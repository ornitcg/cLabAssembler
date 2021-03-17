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


/*
  Performs the first scan over the assembly file
  params:
  FILE* inputFile - pointer to the input assembly file
  STATUS* stat - ths status information of the current scan
  The algorithm goes like suggested by the university
*/
void firstScan(FILE* inputFile , STATUS* stat){
    /*********************************************** DECLARATIONS ***********************************************************/
    char line[MAX_LINE] = EMPTY_STRING;                       /*line of assembly code*/
    /*char symbol[MAX_LABEL] = EMPTY_STRING; */                  /*to contain the label from line*/
    char instruction[MAX_INSTRUCTION]= EMPTY_STRING;         /*to contain the label from line*/
    char command[MAX_CMD_LEN]= EMPTY_STRING;                 /*to contain the label from line*/
    Info instType;                                           /*stands for instruction type*/
    /*********************************************************************************************************/
    while(fgets(line, MAX_LINE, inputFile) != NULL){ /*each iteration of this loop is on a whole line from input file*/
        trimWhiteSpaces(line);    /*removes whitespaces from both ends and also the '\n' for each line read from file*/
        fprintf(stderr,"\n\n [DEBUG] firstScan line#%d  line string: --|%s|--\n", stat->lineNumber,line);
        /*fprintf(stderr, "DEBUG IC %d, DC %d\n", stat->IC, stat->DC);*/

        if (!toIgnore(line) ){
            /*********************************** SYMBOL CHECK **************************************/
            parseSymbol( line, stat);  /*Yes/No/Error*/

              /*relevant to lines that begin with label definition*/
            if (stat -> symbolFound == Yes) {
                trimNchars(line, strlen(stat -> label)+1); /*restart line from end of symbol, which is empty if*/
                trimWhiteSpaces(line);
                /*restart line from end of label, "+1" stand for the colon in the end*/
            }
            /********************************** INSTRUCTION CHECK ***********************************/
            fprintf(stderr,"[DEBUG] in first scan error for line = %d, error exists = %d \n", stat -> errorForLine, stat-> errorExists);

            if (stat -> errorForLine == No){
                instType = parseInstruction(instruction, line, stat); /* value returned into instType= Yes/No/Error*/

                if ( instType != Error && instType != No ){
                /*then need to cut off instruction and trim spaces from rest of line*/
                    trimNchars(line, strlen(instruction)); /*restart line from end of instruction, which is empty if*/
                    trimWhiteSpaces(line);
                } /*else there was a problem in finding a command*/
                if  ( instType == Data || instType == String ){
                    if (stat -> symbolFound == Yes){
                        addSymbol(stat -> symbolTable, stat-> DC , stat -> label , Data, Empty, stat);
                    }
                    parseData(line, instType, stat);/*return 0 if data is wrong or None*/
                }
                    /*if  instType == Entry do nothing*/
                if ( instType == Extern ){
                    if (parseExtern(line, stat) == Ok && (stat -> symbolFound == Yes ))
                        printMessageWithLocation(Warning, stat, "Label definition before .extern instruction");/*NOT ERROR*/
                        /*at this point what's left of line is the operand that passed the test of parseExtern*/
                }
                if ( instType == Entry ){
                    if (checkEntrySyntax(line, stat) == Ok && (stat -> symbolFound == Yes )) /* will perform only syntax validity check*/
                        printMessageWithLocation(Warning, stat, "Label definition before .extern instruction");/*NOT ERROR*/
                        /*at this point what's left of line is the operand that passed the test of parseEntry*/
                }
                /*********************************COMMAND CHECK**************************************/
                if ( stat -> errorForLine == No && instType == No ){
                    /* meaning no instruction found- proceed to parsing command and operands*/

                    if (stat -> symbolFound == Yes)
                        addSymbol(stat -> symbolTable, stat->IC , stat -> label,  Code, Empty, stat);

                    if (parseCommand(command, line,  stat) == Ok){
                        trimNchars(line, strlen(command)); /*restart line from end of instruction, which is empty if*/
                        trimWhiteSpaces(line);
                        parseCommandOperands(line, stat);
                    }
                    }/*end if (instType == No) */
            }
        }/*end if not ignore*/
        resetStatStructForLine(stat); /*resets part of the fields, to use for next line's data*/
    }/*end while*/
    stat -> ICF = stat -> IC;
    stat ->  DCF = stat -> DC;
    updateSymbolTable(stat);

}/*end of firstScan*/



/*
  Performs the second scan over the assembly file, and the codes table (code map)
  and completes data on symbols
  params:
  FILE* inputFile - pointer to the input assembly file, reset to beginning of file
  STATUS* stat - ths status information of the current file.
  The algorithm goes like suggested by the university
*/
void secondScan(FILE* inputFile, STATUS* stat){
    char line[MAX_LINE] = EMPTY_STRING;                            /*line of assembly code*/
    /*char symbol[MAX_LABEL] = EMPTY_STRING;              */          /*to contain the label from line*/
    char instruction[MAX_INSTRUCTION] = EMPTY_STRING;            /*to contain the label from line*/
    Info instType;                             /*stands for instruction type*/

    /*********************************************************************************************************/
    resetforSecondScan(stat); /* resets the input line count in status, to 1*/

    while(fgets(line, MAX_LINE, inputFile) != NULL){ /*each iteration of this loop is on a whole line from input file*/

        trimWhiteSpaces(line);  /*removes whitespaces from both ends and also the '\n' for each line read from file*/
        if (!toIgnore(line) ){
            /***********************************SYMBOL CHECK**************************************/
            parseSymbol( line, stat);  /*Yes/No/Error*/
              /*relevant to lines that begin with label definition*/
            if (stat -> symbolFound == Yes){
                trimNchars(line, strlen(stat -> label)+1); /*restart line from end of label, "+1" stand for the colon in the end*/
                trimWhiteSpaces(line);
            }
            /**********************************INSTRUCTION CHECK***********************************/
            instType = parseInstruction(instruction, line, stat); /*Yes/No/Error*/

            if ( instType != Error && instType != No ){ /*then need to cut off instruction and trim spaces from rest of line*/
                trimNchars(line, strlen(instruction)); /*restart line from end of symbol, which is empty if*/
                trimWhiteSpaces(line);                 /*restart line from end of instruction, which is empty if*/
            } /*there was a problen in parsing*/
            if ( instType == Entry )
                parseEntry(line, stat); /* now enters the actual entry check for second scan*/
        }/*end if not ignore*/
        resetStatStructForLine(stat); /*resets part of the fields, to use for next line's data*/
    }/*end while*/
    /* I chose to  fill the missing info in another loop over the code image
    since it doesn't chande the complexity, and does improve readability*/

    fillMissingDetailsInCodeTable(stat);

}/* end of secondPass*/



/*
Scans the codeTable for missing data and fills in where valid.
prints out error messages for errors found that are relevant to second pass
params:
STATUS* stat - pointer to status, for easy access to all info
*/
void fillMissingDetailsInCodeTable(STATUS* stat){
    /*At this point the Symbol table is full*/
    int currentAddress;
    CODE_IMG* codeBody = NULL;
    SYMBOL* symbol = NULL;
    Node* cursor = stat -> codeTable -> head;

    while (cursor != NULL){/*Iterating on code image rows*/
        currentAddress = cursor -> keyNum;
        codeBody = getCodeImageBody(cursor); /*The bunch of fields of code image linked list  at this cursor locaion*/

        stat -> lineNumber = codeBody -> lineNumber;
        /* recycling this field of STATUS.lineNumber for easy use at error messages, since this field is redundant now and stat is being called anyway by the printMessageWithLocationesage function*/

        symbol = lookupSymbol(stat -> symbolTable, codeBody -> label);
        /*symbol points at the body of the symbol table link thatbelongs to the symbol
        at cursor location,if this location does not relate to a symbol, Null will be returned*/

        if ( strcmp(codeBody-> label,EMPTY_STRING)!= 0  && symbol == NULL ){
            /*any operand symbol should be found in the symbol table, thus error*/
            printMessageWithLocation(Error, stat, "Symbol not defined \n");
            activateErrorFlag(stat);
        }
        if ( symbol!= NULL && codeBody -> ARE == FillLater){ /* Find the relevant places to fill in*/

            if ( codeBody -> comment == Relative){

                if(symbol-> attr2==Extern){
                    printMessageWithLocation(Error, stat, "Extern symbol cannot be assigned as relative address.\n");
                    activateErrorFlag(stat);
                }
                else codeBody -> code = (symbol-> address) - currentAddress;
                /* The calculation value  of distance for relative addressing operand*/
            }
            else if ( codeBody -> comment == Direct)
                codeBody -> code = symbol -> address;   /* insert value of direct operand */

            if (symbol -> attr2 == Extern )
                codeBody -> ARE = E;
            else codeBody -> ARE = R;
        }
            cursor = cursor -> next;
    }
}
