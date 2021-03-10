/*
  group of the big functions that are related to the assembler
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "as_funcs.h"
#include "utilsGeneral.h"
#include "utilsAssembler.h"
#include "asStructsAndMacros.h"


/*
  Performs the first pass over the assembly file
  params: FILE* inputFile - pointer to the input assembly file
  returns: 1 if errors occured and 0 otherwise
*/
void firstPass(FILE* inputFile , STATUS* stat){
    /***********************************************DECLARATIONS***********************************************************/
    /*int ICF = 0;*/      /*instructions counter - for final value*/
    /*int DCF = 0;*/      /*data counter - for final value*/
    char line[MAX_LINE];                       /*line of assembly code*/
    char symbol[MAX_LABEL] = "";                    /*to contain the label from line*/
    char instruction[MAX_INSTRUCTION];         /*to contain the label from line*/
    char command[MAX_CMD_LEN];                     /*to contain the label from line*/
    info instType;                        /*stands for instruction type*/
    info res;
    /*********************************************************************************************************/
    SET_COMMAND_TABLE(cmdTable);      /*implemented by array of structures, all set by macro*/

    while(fgets(line, MAX_LINE, inputFile) != NULL){ /*each iteration of this loop is on a whole line from input file*/
        strcpy(line, trimWhiteSpaces(line));        /*removes whitespaces from both ends and also the '\n' for each line read from file*/
        fprintf(stderr,"\n\n");
        if (!toIgnore(line) ){
            emptyString(symbol);
            emptyString(instruction);
            emptyString(command);
            stat -> symbolFound = No; /*reset for this new line*/

            /***********************************SYMBOL**************************************/
            res = parseSymbol(symbol, line, stat);
            stat -> symbolFound = res;  /*Yes/No/Error*/
            if (res == Error)      /*relevant to lines that begin with label definition*/
                stat -> errorExists = Yes;
            if (res == Yes){
                fprintf(stderr,"DEBUG firstpass1 - line# %d The symbol that was found is --|%s|--\n", stat-> lineNumber, symbol);
                strcpy(line, trimWhiteSpaces(&line[strlen(symbol)+1])); /*restart line from end of label "+1" stand for the colon in the end*/
            }
            /**********************************INSTRUCTION***********************************/
            instType = parseInstruction(instruction, line, stat); /*Yes/No/Error*/

            if (instType != Error && instType != No){ /*then need to cut off instruction and trim spaces from rest of line*/
                strcpy(line, trimWhiteSpaces(&line[strlen(instruction)])); /*restart line from end of instruction, which is empty if*/
                fprintf(stderr,"DEBUG firstpass2 - line# %d The Istruction that was found is --|%s|--\n", stat-> lineNumber, instruction);

            } /*there was a problen in parsing*/
            if (instType == Error)
                stat -> errorExists = Yes;
            else if  ( instType == Data || instType == String ){

                    if (stat -> symbolFound == Yes){
                        addSymbol(stat -> symbolTable, (stat-> DC)+(stat->IC) , symbol, Data, Empty);
                    }
                    res = parseData(line, instType, stat);/*return 0 if data is wrong or None*/
                    if (res == Error)
                        stat -> errorExists = Yes;
                    }
            else if ( instType == Extern ){
                    res = parseExtern(line, stat);
                    if (res == Ok){
                        if (stat -> symbolFound == Yes )
                            printf("line#%d: WARNING -  exists symbol before .extern instruction", stat -> lineNumber);/* NOT ERROR*/
                        addSymbol(stat -> symbolTable, 0 , line , Empty, Extern); /*at this point what's left of line is the operand that passed the test of parseExtern*/
                    }
                    else if (res == Error)
                        stat -> errorExists = Yes;
            }/*end if ( instType == Extern )*/
            /*********************************COMMAND**************************************/
            if (instType == No){/*no instruction - proceed to parsing command and operands*/
                fprintf(stderr,"DEBUG firstpass3 - line# %d the rest of line: --|%s|--\n", stat-> lineNumber, line);

                if (stat -> symbolFound == Yes)
                    addSymbol(stat -> symbolTable, stat->IC , symbol,  Code, Empty);
                res = parseCommand(command, line,  stat );
                if (res == Error)
                    stat -> errorExists = Yes;
                else if (res == Ok){
                    strcpy(line, trimWhiteSpaces(&line[strlen(command)])); /*restart line from end of command*/
                    parseCommandOperands(line, stat);
                }
            }/*end (instType == No)*/
        }/*end if not ignore*/
            /*fprintf(stderr, "firstPass  %s\n",line);*/
            /*fprintf(stderr, "length  %d\n",strlen(line));*/
        (stat -> lineNumber)++;
    }/*end while*/
    /*ICF = stat.IC;
      DCF = stat.DC;*/
    /*updateDataTable(ICF);*/
}/*end of firstPass*/

void secondPass(FILE* inputFile, STATUS* stat){
}


void runAssembler(FILE* inputFile, char* fileName){
    STATUS stat;
    initStatus(&stat, fileName); /*to contain status details of current line*/
    /*fprintf(stderr, "******** DEBUG - in runAssembler\n");*/
    firstPass(inputFile, &stat);
    fseek(inputFile,0,SEEK_SET);
    secondPass(inputFile, &stat);

}
