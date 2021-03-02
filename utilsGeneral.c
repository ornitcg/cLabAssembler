/*
  Group of helper functions that are general and not related specifically to the assembler
*/

#include <stdio.h>
#include <string.h>
#include "utilsAssembler.h"
#include "utilsGeneral.h"

#define EXT_AS ".as"

/*
Checks if the file name is valid and if it can be opened
returns pointer to file if OK and NULL otherwise
params:
char* fileName : the name of the file to be tested if validFile
*/
FILE* validFile(char* fileName){
    FILE* file = NULL;
    char* findExtention  = strchr(fileName,'.');

    if (findExtention != NULL ){
      fprintf(stderr,"Error - in file name %s\n", fileName);
      return NULL;
    }
    fileName = strcat(fileName,EXT_AS);
    file = fopen(fileName,"r");
    if (file == NULL)
      fprintf(stderr,"Error - cannot open the file %s\n", fileName);
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
returns: the trimmed string
*/
char* trimWhiteSpaces(char* str){
    int last;
    if (strlen(str) == 0 )
      return str;
    while (isWhiteSpace(str[0])){
      str++;
    }
    last = strlen(str)-1;
    while ((last+1) && isWhiteSpace(str[last])){
      str[last] = '\0';
      last--;
    }
    return str;
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
