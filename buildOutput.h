#include "asStructsAndMacros.h"



/*
Builds the content of the output output files.
params: STATUS* stat - to access the codeTable and other ststus info
*/
void buildOutputFiles(STATUS* stat);



/*
Creates the title of the object file and prints it in the file.
params:
FILE* objectFile -pointer to object file, to print in.
STATUS* stat - for ICF, DCF values
*/
void addTitleToObjectFile(FILE* fileName , STATUS* stat);



/*
prints line of word into the object files
params:
FILE* objectFile -  pointer to the file to print into.
short address - the first fild of line.
short code - the second field of line.
Info ARE - the third field of line
*/
void addLineToObjectFile(FILE* objectFile, short address , short code, Info ARE);





/*
Prints line of word into the Externals files
params:
FILE* fileName -  pointer to the file to print into.
short symbol - the first fild of line.
short address - the second field of line.
*/
void addLineToExtFile(FILE* extFile, char* label , short address);


/*
Prints line of word into the Entry file
params:
FILE* entFile -  pointer to the file to print into.
short symbol - the first fild of line.
short address - the second field of line.
*/
void addLineToEntFile(FILE* entFile, char* symbol , short address );



/*
Opens all the output files to be filled.
params:
FILE** objectFile - pointer to pointer of output object file
FILE** extFile - pointer to pointer of output extFile file
FILE** entFile - pointer to pointer of output entFile file
STATUS* stat - for fileName
*/
void openFiles(FILE** objectFile, FILE** extFile , FILE** entFile , STATUS* stat);


/*
Closed all the open files.
params:
FILE* objectFile- pointer to objectFile
FILE* extFile - pointer to extFile
FILE* entFile - pointer to entFile
*/
void closeFiles(FILE* objectFile, FILE* extFile, FILE* entFile);



/*
Deletes ext/ent files if ther is nothing to write in them.
params:
Info flagExt -  flag if there is any data for ext file
Info flagEnt - flag if there is any data for ent file
STATUS* stat - for filename info
*/
void removeUnnecessaryFiles (Info flagExt, Info flagEnt, STATUS* stat);


/*
Builds the string of fileName with the matching extention for the reuired output file
char* fileName- to collect the final fileName with the extention
char* extention-  the extention required for the output file
STATUS* stat - for fileName
*/
void buildFileName(char* fileName , char* extention, STATUS* stat);
