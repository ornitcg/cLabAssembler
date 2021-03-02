#define WHITE_SPACE " \t\n"
/*
Checks if the file name is valid and if it can be opened
returns pointer to file if OK and NULL otherwise
params:
char* fileName : the name of the file to be tested if validFile
*/
FILE* validFile(char* fileName);
/*
Removes the last charachter in a given string
params:
char* string : a string to remove the last charachter from
*/
void remLastChar(char* string);


/*removes leading and trailing whitespaces from a string
params: char* str -  the string to trim
returns: the trimmed string
*/
char* trimWhiteSpaces(char* str);

/*
Checks if a charachter is a space or tab
params: char c- the charachter to check
returns: 1 (YES) if c is a whitespace , and 0 (NO) otherwise
*/
int isWhiteSpace(char c);
