#ifndef _UTILS_GEN_
#define _UTILS_GEN_

#define SOMETHING 0;
#define WHITE_SPACE ' ' /*Don't worry I know there are other chars for whitespace , this is just for readability at firstPosOfChar*/
#define YES 1
#define NO 0
#define NOT_FOUND -1
#define COMMA ','
#define EMPTY_STRING ""
#define QUOTE '"'
#define DECIMAL_POINT '.'
#define EXTENTION_IDENTIFIER '.'

#define MAX_STR 81


void removeExtention(char* fileName);
short shortToTwelveBits(short code);

/*
Checks if the file name is valid and if it can be opened
returns pointer to file if OK and NULL otherwise
params:
char* fileName : the name of the file to be tested if validFile
*/
FILE* validFile(char* fileName, char* extention);
/*
Removes the last charachter in a given string
params:
char* string : a string to remove the last charachter from
*/
void remLastChar(char* string);
void emptyString(char* string);

int firstPosOfChar(char* string, char targetChar);
/*removes leading and trailing whitespaces from a string
params: char* str -  the string to trim
returns: the trimmed string
*/
void toBinaryString(short source, char* dest);
void trimWhiteSpaces(char* string);
/*void trimWhiteSpaces(char* string);*/
void trimNchars(char* line,int numChars);
/*
Checks if a charachter is a space or tab
params: char c- the charachter to check
returns: 1 (YES) if c is a whitespace , and 0 (NO) otherwise
*/
int isWhiteSpace(char c);
int isEmptyString(char* string);
int externalCommas(char* line);
int validSymbolChars(char* symbol);
void do_nothing();
#endif
