/*
  Group of helper functions that are general and not related specifically to the assembler
*/

#include <stdio.h>
#include <string.h>
#include "utilsGeneral.h"


/*
Checks if the file name is valid and if it can be opened
returns pointer to file if OK and NULL otherwise
params:
char* fileName : the name of the file to be tested if validFile
*/
FILE* validFile(char* fileName, char* extention){
    FILE* file = NULL;
    char* findExtention  = strchr(fileName,'.');
    char tmpName[FILENAME_MAX];

    if (findExtention != NULL ){
      fprintf(stderr,"[Error] - in file name %s\n", fileName);
      return NULL;
    }
    sprintf(tmpName,"%s%s",fileName,extention);
    file = fopen(tmpName,"r");
    if (file == NULL)
      fprintf(stderr,"[Error] - cannot open the file %s\n", tmpName);
    return file;
}

/*
Removes the last charachter in a given string
params:
char* string : a string to remove the last charachter from
*/
void remLastChar(char* string){
  int len = strlen(string);
  if (len > 0)
    string[len-1] = '\0';
}
/*removes leading and trailing whitespaces from a string
params: char* str -  the string to trim
returns: the trimmed string (in different ways of use for different types of strings)
*/
void trimWhiteSpaces(char* string){
    int last;
    char strP[MAX_STR] = EMPTY_STRING;
    char* tmpStr = strP;
    strcpy(strP,string);

    if (strlen(tmpStr) == 0 )
      return;

    while (isWhiteSpace(tmpStr[0]) == YES){
      tmpStr += 1;
    }
    last = strlen(tmpStr)-1;

    while ((last >= 0) && isWhiteSpace(tmpStr[last])){
      tmpStr[last] = '\0';
      last--;
    }
    strcpy(string, EMPTY_STRING);
    strcpy(string, tmpStr);
}


void trimNchars(char* line,int numChars){
    char tmpStr[MAX_STR] = EMPTY_STRING;
    char* strP = tmpStr;
    strcpy(strP,line);

    if (strlen(strP) == 0)
        return;

    strP += numChars;
    strcpy(line, EMPTY_STRING);
    strcpy(line, strP);
}

int isEmptyString(char* string){
    if (strlen(string)==0)
        return YES;
    return NO;
}

int externalCommas(char* line){
    int len = strlen(line);
    if (len > 0)
        if (line[0] == COMMA || line[len-1] == COMMA )
            return YES;
    return NO;
}

/*
Checks if a charachter is a space or tab
params: char c- the charachter to check
returns: 1 (YES) if c is a whitespace , and 0 (NO) otherwise
*/
int isWhiteSpace(char c){
  if ((c == ' ') || (c =='\t') || (c =='\n'))
    return YES;
  return NO;
}

int firstPosOfChar(char* string, char targetChar){
    int pos = 0;
    while (string[pos] != '\0'){
        if (targetChar == WHITE_SPACE){
        /* WHITESPACE is used here for readability, the function isWhiteSpace does the full check of whitespaces*/
            if (isWhiteSpace(string[pos]))
                return pos;
        }
        else if (string[pos] == targetChar) /*for other chaachters search*/
            return pos;
        pos++;
    }
    return NOT_FOUND;
}

void emptyString(char* string){
    string[0] = '\0';
}

short shortToTwelveBits(short code){
    short mask = ((unsigned short)(-1)) >> 4;
    return (code & mask);
}

/* Removes the extention from a fileName, if there is an extention*/
void removeExtention(char* fileName){
    fileName[firstPosOfChar(fileName,EXTENTION_IDENTIFIER)]='\0';
}

/**/
void toBinaryString(short source, char* final){
    char dest[12] = "000000000000";
    int len =strlen(dest);
    int ind=len-1;

    while ((source/2) >0 ){
        if ((source%2) ==0)
            dest[ind] = '0' ;
        else dest[ind] = '1' ;
        source=(source/2);
        ind--;
    }
    if ((source%2) ==0)
        dest[ind] = '0' ;
    else dest[ind] = '1' ;
    strcpy(final,dest);


}

void do_nothing(){
}
