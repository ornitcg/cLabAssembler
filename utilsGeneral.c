/*
Author: Ornit Cohen gindi

Here is a group of helper functions that are general and not related specifically to the assembler
and not dependent on assembler related files.

*/

#include <stdio.h>
#include <string.h>
#include "utilsGeneral.h"


/*
Checks if the file name is valid and if it can be opened
params:
char* fileName : the name of the file to be tested if validFile
returns: FILE* - pointer to file if OK and NULL otherwise
*/
FILE* isValidFile(char* fileName, char* extention){
    FILE* file = NULL;
    char* findExtention  = strchr(fileName,'.');
    char tmpName[FILENAME_MAX];

    if (findExtention != NULL ){
      fprintf(stderr,"[Error] - in file name %s\n", fileName); /*general error for including extention or just a '.' charchter*/
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



/*
Removes leading and trailing whitespaces from a string
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



/*
Removes the Number of charachters as given in numChars , from beginning of line.
params:
char* line - The line to remove numChars charachters from.
int numChars - the number of charachters to remove from beginning of line
*/
void trimNchars(char* line,int numChars){
    char tmpStr[MAX_STR] = EMPTY_STRING; /*for holding the original string*/
    char* strP = tmpStr;                 /*for running over the string*/
    int len = strlen(line);
    strcpy(tmpStr,line);

    if (len == 0 || len < numChars)
        return;

    strP += numChars;
    /*strcpy(line, EMPTY_STRING);*/ /*restarting line string*/ /*DEBUG*/
    strcpy(line, strP); /*copy strP into line*/

}


/*
Empties the given string.
params: char* string - the string to remove all charachters from
*/
void emptyString(char* string){
    string[0] = '\0';
}



/*
Checks if a given string is emptyString
params:
char* string - the string to check if empty.
returns int - the value of macro YES/NO
*/
int isEmptyString(char* string){
    if (strlen(string)==0)
        return YES;
    return NO;
}


/*
Checks if there are commas at the first and last charachter of string lines
assuming there are no heading or tailing whitespaces
params:
char* line - the string to be checkd for heading and tailing comma
returns:
int value for  YES/NO macro values
*/
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


/*
Searches for a charachter targetChar in strings
and finds the first position of it
params:
char* string - the string to search within
char targetChar - the charachter to lookup in string
Returns
int - the position value of the charachter in
string and NOT_FOUND macro  (integer) otherwise
*/
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



/*
Turns off the four leftmost bits of the short type
so that only the 12 rightmost bits dictate the code
params: short code - the data of type short, to turn to twelve bits
returns: short type of the same code with 4 leftm oset bits turned to 0
*/
short shortToTwelveBits(short code){
    short mask = ((unsigned short)(-1)) >> 4;
    return (code & mask);
}
