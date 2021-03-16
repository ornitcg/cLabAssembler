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



/*
Checks if the file name is valid and if it can be opened
params:
char* fileName : the name of the file to be tested if validFile
returns: FILE* - pointer to file if OK and NULL otherwise
*/
FILE* isValidFile(char* fileName, char* extention);


/*
Removes the last charachter in a given string
params:
char* string : a string to remove the last charachter from
*/
void remLastChar(char* string);


/*
Removes leading and trailing whitespaces from a string
params: char* str -  the string to trim
returns: the trimmed string (in different ways of use for different types of strings)
*/
void trimWhiteSpaces(char* string);


/*
Removes the Number of charachters as given in numChars , from beginning of line.
params:
char* line - The line to remove numChars charachters from.
int numChars - the number of charachters to remove from beginning of line
*/
void trimNchars(char* line,int numChars);


/*
Empties the given string.
params: char* string - the string to remove all charachters from
*/
void emptyString(char* string);

/*
Checks if a given string is emptyString
params:
char* string - the string to check if empty.
returns int - the value of macro YES/NO
*/
int isEmptyString(char* string);


/*
Checks if there are commas at the first and last charachter of string lines
assuming there are no heading or tailing whitespaces
params:
char* line - the string to be checkd for heading and tailing comma
returns:
int value for  YES/NO macro values
*/
int externalCommas(char* line);


/*
Checks if a charachter is a space or tab
params: char c- the charachter to check
returns: 1 (YES) if c is a whitespace , and 0 (NO) otherwise
*/
int isWhiteSpace(char c);


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
int firstPosOfChar(char* string, char targetChar);


/*
Turns off the four leftmost bits of the short type
so that only the 12 rightmost bits dictate the code
params: short code - the data of type short, to turn to twelve bits
returns: short type of the same code with 4 leftm oset bits turned to 0
*/
short shortToTwelveBits(short code);





#endif
