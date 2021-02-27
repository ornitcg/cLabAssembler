#include <stdio.h>
#include <string.h>

#define EXT_AS ".as"

/*
Checks if the file name is valid and if it can be opened
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



int getNumber() {
    return 5;
}
