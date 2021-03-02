

/*
  Performs the first pass over the assembly file
  params: FILE* inputFile - pointer to the input assembly file
  returns: 1 if errors occured and 0 otherwise
*/
int firstPass(FILE* inputFile);
void secondPass(FILE* inputFile,char* fileName);
void runAssembler(FILE* inputFile, char* fileName);
/*updates the data table so that every
data symbol is given a shift at the size of ICF
params: int ICF -  the size to add to data symbols
        data table - to be updated with ICF
 */
void updateDataTable(int ICF);
