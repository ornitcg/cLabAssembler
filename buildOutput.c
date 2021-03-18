/*
Author: Ornit Cohen Gindi
Here is a group of functions that deal with building the output files.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asStructsAndMacros.h"
#include "buildOutput.h"
#include "utilsGeneral.h"

#define OBJECT_LINE 80
#define EXT_LINE 40
#define ENT_LINE 40

/*
Builds the content of the output output files.
params: STATUS* stat - to access the codeTable and other ststus info
*/
void buildOutputFiles(STATUS* stat){
    FILE* objectFile = NULL;
    FILE* extFile = NULL;
    FILE* entFile = NULL;
    Info flagExt = No;
    Info flagEnt = No;
    Node* cursor;
    CODE_IMG* codeBody =NULL;
    DATA_IMG* dataBody =NULL;
    SYMBOL* symBody =NULL;

    openFiles(&objectFile, &extFile, &entFile, stat);
    addTitleToObjectFile(objectFile, stat);

    /*Iterating over code table and adding words to object file, on the go building .ext file*/
    cursor = stat -> codeTable -> head;
    while (cursor != NULL){
        codeBody = getCodeImageBody(cursor); /*call the bunch of CODE_IMG fields , cursor pints at*/
        addLineToObjectFile(objectFile, cursor -> keyNum ,  codeBody -> code, codeBody-> ARE );

        if (codeBody -> ARE == E){ /*for .ext file*/
            addLineToExtFile(extFile, codeBody -> label , cursor -> keyNum);
            flagExt = Yes;
        }
        cursor = cursor -> next;
    }
    /* add date to object file*/
    cursor = stat -> dataTable -> head;
    while (cursor != NULL){
        dataBody = getDataImageBody(cursor); /*call the bunch of DATA_IMG fields , cursor pints at*/
        addLineToObjectFile(objectFile, (cursor -> keyNum)+ stat-> ICF ,  *dataBody, A );
        cursor = cursor -> next;
    }

    /*Iterating over symbol table to find symbol with Entry attribute and build an .ent file*/
    cursor = stat -> symbolTable -> head;
    while (cursor != NULL){
        symBody = getSymbolBody(cursor); /*call the bunch of SYMBOL fields , cursor pints at*/
        if (symBody -> attr2 == Entry){
            addLineToEntFile(entFile, cursor -> keyStr , symBody -> address);
            flagEnt = Yes;
        }
        cursor = cursor -> next;
    }

    closeFiles(objectFile,  extFile, entFile );
    removeUnnecessaryFiles ( flagExt, flagEnt, stat);
}



/*
Opens all the output files to be filled.
params:
FILE** objectFile - pointer to pointer of output object file
FILE** extFile - pointer to pointer of output extFile file
FILE** entFile - pointer to pointer of output entFile file
STATUS* stat - for fileName
*/
void openFiles(FILE** objectFile, FILE** extFile , FILE** entFile , STATUS* stat){
    char fileName[FILENAME_MAX];

    buildFileName(fileName, ".ob", stat);
    (*objectFile) = fopen(fileName, "w");

    buildFileName(fileName, ".ext", stat);
    (*extFile) = fopen(fileName, "w");

    buildFileName(fileName, ".ent", stat);
    (*entFile) = fopen(fileName, "w");
}

/*
Builds the string of fileName with the matching extention for the reuired output file
char* fileName- to collect the final fileName with the extention
char* extention-  the extention required for the output file
STATUS* stat - for fileName
*/
void buildFileName(char* finalFileName , char* extention, STATUS* stat){
     emptyString(finalFileName);
     sprintf(finalFileName, "%s%s", stat-> fileName, extention);
}


/*
Closed all the open files.
params:
FILE* objectFile- pointer to objectFile
FILE* extFile - pointer to extFile
FILE* entFile - pointer to entFile
*/
void closeFiles(FILE* objectFile, FILE* extFile, FILE* entFile){
    fclose(objectFile);
    fclose(extFile);
    fclose(entFile);
}


/*
Creates the title of the object file and prints it in the file.
params:
FILE* objectFile -pointer to object file, to print in.
STATUS* stat - for ICF, DCF values
*/
void addTitleToObjectFile(FILE* objectFile , STATUS* stat){
    char title[OBJECT_LINE];
    sprintf(title,"\t%d %d\n",stat -> ICF -100, stat -> DCF);
    fputs( title , objectFile);

}


/*
prints line of word into the object file
params:
FILE* objectFile -  pointer to the file to print into.
short address - the first fild of line.
short code - the second field of line.
Info ARE - the third field of line
*/
void addLineToObjectFile(FILE* objectFile, short address , short code, Info ARE){
    char line[OBJECT_LINE];
    sprintf(line,"%04d %03X %c\n",address , shortToTwelveBits(code),  ARE);
    fputs( line , objectFile);
}


/*
Prints line of word into the Externals file
params:
FILE* extFile -  pointer to the file to print into.
short symbol - the first fild of line.
short address - the second field of line.
*/
void addLineToExtFile(FILE* extFile, char* symbol , short address){
    char line[EXT_LINE];
    sprintf(line,"%s %04d\n",symbol ,address);
    fputs( line , extFile);
}


/*
Prints line of word into the Entry file
params:
FILE* entFile -  pointer to the file to print into.
short symbol - the first fild of line.
short address - the second field of line.
*/
void addLineToEntFile(FILE* entFile, char* symbol , short address ){
    char line[EXT_LINE];

    sprintf(line,"%s %04d\n",symbol ,address);
    fputs( line , entFile);

}

/*
Deletes ext/ent files if ther is nothing to write in them.
params:
Info flagExt -  flag if there is any data for ext file
Info flagEnt - flag if there is any data for ent file
STATUS* stat - for filename info
*/
void removeUnnecessaryFiles (Info flagExt, Info flagEnt, STATUS* stat){
    char fileName[FILENAME_MAX];

    sprintf(fileName,"%s%s",stat -> fileName,".ent");
    if (flagEnt == No)
        remove(fileName);

    sprintf(fileName,"%s%s",stat -> fileName,".ext");
    if (flagExt == No)
        remove(fileName);
}
