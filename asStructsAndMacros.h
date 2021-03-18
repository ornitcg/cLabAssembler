/*
Author: Ornit Cohen Gindi
This file contains assembler structures:
structures used for building the symbol, code and data tables,

status structure that contains the data to be passed in one param to functions.
each structure is followed by its functions prototypes.

enum Info type definition - all in one, for better communication between funcions.

macros to define lists of reserved words and registers.

command table is also defined here as an array of structures using a macro. it contains all info that has to do with each command.
I chose this way because it is a read-only table, and it's easier and less messy to build it  using one line of code in a small function.
*/

#ifndef _STRUCT_AND_MAC_
#define _STRUCT_AND_MAC_

#include "linkedList.h"
#include "utilsGeneral.h"

#define MAX_LINE 82 /*to enable inclusion of \0 and \n*/
#define MAX_LABEL 32
#define MAX_INSTRUCTION 10
#define MAX_CMD_LEN 5
#define REGISTER_LEN 3
#define REGISTERS_NUMBER 8
#define INIT_ADDRESS 100
#define COMMANDS_NUMBER 16
#define LABEL_IDENTIFIER ':'
#define COMMENT_IDENTIFIER_INT ';'
#define COMMENT_IDENTIFIER ';'
#define INSTRUCTION_IDENTIFIER '.'
#define IMMEDIATE_IDENTIFIER '#'
#define RELATIVE_IDENTIFIER '%'

/*Info contains a bunch of word that are helpfull for readability*/
enum Info {A = 'A' , R = 'R' ,E = 'E' , Immediate = '0', Direct ='1' , Relative ='2' , Register ='3', Error = -1 , Warning  = -2 , No =0 , Yes=1 , Ok=2 , Source=3 , Dest=4 , Code =5 , Data=6 , String=7 ,  Entry=8 , Extern=9 , Empty = '_'  , FillLater ='?' , First = 'F', Second = 'S' };
/* The numeric values in this enum are arbitraily chosen*/
typedef enum Info Info;

/*
Contains all the status info to pass from hand to hand in one paramter
for easy communication between functions.
*/
typedef struct STATUS{
    unsigned int IC; /*chose int to enable more than 4095 words*/
    unsigned int DC;
    unsigned int ICF;
    unsigned int DCF;
    int lineNumber;
    Info errorForLine; /*YES/NO*/
    Info errorExists; /*YES/NO*/
    Info symbolFound; /*YES/NO*/
    char fileName[FILENAME_MAX];
    char label[MAX_LABEL];
    int commandNumber;
    Info srcOpAddressType;
    Info destOpAddressType;
    LinkedList* symbolTable;            /*starting a linked list*/
    LinkedList* codeTable;         /*starting a linked list*/
    LinkedList* dataTable;          /*starting a linked list*/
} STATUS;

/*
Initializes the STATUS structure for the current fileName
params:
STATUS* stat - pointer to the status structure to be initiallized
char* fileName - string of the current file name without extention
*/
void initStatus(STATUS* stat, char* fileName);

/* Resets part of the field in status struct, for every line of the input file*/
void resetStatStructForLine(STATUS* stat);

/*
Resets lineNumber to 1
*/
void resetforSecondScan(STATUS* stat);


/*
Turns the errorExists flag in STATUS  to Yes, once an error is Found
params:
STATUS* stat - in which the flag is a field
returns:
Info type Error value
*/
Info activateErrorFlag(STATUS* stat);


/*
Gets the address type of the given type of operand.
params:
Info opType - source (Source) operand or destination (Dest) operand
STATUS* stat - in which the current operands address types are fields
returns:
Info type - address type values of operands: Immediate/Direct/Relative/Register
*/
Info getAddressType(Info opType,STATUS* stat);



/*
Frees all the mallocs of the linked lists of the three tables that stat holds
params:
STATUS* stat - pointer to STATUS that holds the malloced linked lists
*/
void freeMemory(STATUS* stat);

/**************************************************SYMBOL TABLE*****************************************/
/*
code image structure  with all the relevant fields for one symbol
to be linked as the symbol table.
*/
typedef struct SYMBOL {
    short address;
    enum Info attr1;
    enum Info attr2; /*sometimes there maybe 2 attributes*/
} SYMBOL;


/*
Adds a symbol to symbolTable
params: symbolTable - LinkedList pointer
short address - the address of the symbol
char* symbol - the label itself
Info attr1 - attributes Data/Code
Info attr2 - attributes Extern/Entry
STATUS* stat - pointer to statust structure, for easy access to current line number in input
*/
void addSymbol(LinkedList* symbolTable, short address, char* symbol, Info attr1, Info attr2, STATUS* stat);



/*
Searches through the symbol table for a given label (symbol)
params:
LinkedList* symbolTable - the able to search within.
char* symbol - the sembol to lookup.
returns: SYMBOL* - pointer to SYMBOL structure that has all the relevant dat fields of the  required symbol.
if none found , returns NULL
*/
SYMBOL* lookupSymbol(LinkedList* symbolTable, char* symbol);

/*
Updates The symbol table at the end of the first scan
wherever there is a symbol that is related to dataTable ,its
 address should be shifted with the size of ICF
  */
void updateSymbolTable(STATUS* stat);

/*
Gets the bunch of fields of a SYMBOL structure
params:
Node* cursor -  pointer to head of symbolTable linkedList
returns:
SYMBOL* - pointer to the SYMBOL structure which is a field in Node
*/
SYMBOL* getSymbolBody (Node* cursor);


/*updates the data table so that every
data symbol is given a shift at the size of ICF
params: STATUS* stat - for easy access to status info
 */
void updateDataTable(STATUS* stat);


/*****************************************CODE (INSTRUCTIONS)TABLE ***************************************/

/*
code image structure  with all the relevant fields for one word
to be linked as the code table.
*/
typedef struct CODE_IMG {
    Info comment;
    int lineNumber;
    char label[MAX_LABEL];
    short code;
    Info ARE;
} CODE_IMG;


/*
    Adds the code into the matching structure and linkedList.
    params:
    LinkedList* codeTable
    short address - the address of the current word, to be printed into the object file
    int lineNumber - the number of relevent line in the input file
    Info comment - other info that helps filling ithis table in the second scan
    char* label - if the current word is for a label, then this parameter contains the label string and stays empty otherwise
    short code - the word required in the object file
    Info ARE - the charachter 'A','E','E'  as required in the output object file , or FillLater if the character is not known yet -
 */
void addCode(LinkedList* codeTable, short address, int lineNumber, Info comment, char* label, short code, Info ARE);


/*
Gets the bunch of fields of a CODE_IMG structure
params:
Node* cursor -  pointer to head node of codeTable linkedList.
returns:
CODE_IMG* - pointer to the CODE_IMG structure which is a field in Node
*/
CODE_IMG* getCodeImageBody (Node* cursor);
/*********************************************DATA TABLE*******************************************************/
/*
data image siple since there is only one field of value (the key is on the data structure)
to be linked as the data table.
*/
typedef short  DATA_IMG;


/*
    Adds the data into the matching structure and linkedList.
    params:
    LinkedList* dataTable
    short address - the address of the current word, to be printed into the object file
    short data - the word required in the object file
 */
void addData(LinkedList* dataTable, short address, short data);


/*
Gets the data value in the data table
params:
Node* cursor -  pointer to head of codeTable linkedList
returns:
CODE_IMG* - pointer to the body - data value is a field in Node
*/
DATA_IMG* getDataImageBody (Node* cursor);


/***************************************** Macro lists for reserved words and registers*********************************************/

typedef char regArr[REGISTERS_NUMBER][REGISTER_LEN] ; /*for array of registers names*/
#define SET_REG_ARR(x)  regArr x = {"r0","r1","r2", "r3","r4","r5","r6","r7"}

typedef char reservedArr[5][MAX_LABEL] ; /*for array of registers names*/
#define SET_RESERVED_ARR(x)  reservedArr x = {"data", "string", "entry","extern", "NULL"};

/*#define RES_WORDS_LIST {"data", "string", "entry","extern", "NULL"}*/
/*#define char otherReserved*/

/*****************************************COMMAND (x) TABLE*********************************************/

/*This is the table of 16 commands, to be defined int the first pass function using macro*/
typedef struct CMD{
    short int opcode;
    short int funct;
    short int operands;
    char opSrc[4];
    char opDest[4];
    char command[MAX_CMD_LEN];
} CMD;
/*
opSrc and opDest define the allowed types of addressing types for each type of operand, for each command
empty string means noaddressing type(or no operand) allowed as source or destination
The rason for this macro is explained at the top of this file.
*/
#define SET_COMMAND_TABLE(x) CMD x[16];\
  x[0].opcode=0; x[0].funct=0; x[0].operands=2; strcpy(x[0].opSrc , "013") ; strcpy(x[0].opDest,"13"); strcpy(x[0].command, "mov");\
  x[1].opcode=1; x[1].funct=0; x[1].operands=2; strcpy(x[1].opSrc ,"013"); strcpy(x[1].opDest ,"013");strcpy(x[1].command, "cmp");\
  x[2].opcode=2; x[2].funct=10; x[2].operands=2;strcpy(x[2].opSrc ,"013"); strcpy(x[2].opDest ,"13"); strcpy(x[2].command , "add");\
  x[3].opcode=2; x[3].funct=11; x[3].operands=2; strcpy(x[3].opSrc , "013"); strcpy(x[3].opDest ,"13");strcpy(x[3].command , "sub");\
  x[4].opcode=4; x[4].funct=0; x[4].operands=2; strcpy(x[4].opSrc ,"1"); strcpy(x[4].opDest ,"13");strcpy(x[4].command , "lea");\
  x[6].opcode=5; x[6].funct=11; x[6].operands=1; strcpy(x[5].opSrc , ""); strcpy(x[5].opDest ,"13");strcpy(x[6].command , "not");\
  x[5].opcode=5; x[5].funct=10; x[5].operands=1; strcpy(x[6].opSrc ,""); strcpy(x[6].opDest ,"13");strcpy(x[5].command , "clr");\
  x[7].opcode=5; x[7].funct=12; x[7].operands=1; strcpy(x[7].opSrc, ""); strcpy(x[7].opDest ,"13"); strcpy(x[7].command , "inc");\
  x[8].opcode=5; x[8].funct=13; x[8].operands=1; strcpy(x[8].opSrc ,""); strcpy(x[8].opDest ,"13");strcpy(x[8].command , "dec");\
  x[9].opcode=9; x[9].funct=10; x[9].operands=1; strcpy(x[9].opSrc ,""); strcpy(x[9].opDest ,"12");strcpy(x[9].command , "jmp");\
  x[10].opcode=9; x[10].funct=11; x[10].operands=1;strcpy(x[10].opSrc ,""); strcpy(x[10].opDest ,"12"); strcpy(x[10].command , "bne");\
  x[11].opcode=9; x[11].funct=12; x[11].operands=1; strcpy(x[11].opSrc ,""); strcpy(x[11].opDest ,"12");strcpy(x[11].command , "jsr");\
  x[12].opcode=12; x[12].funct=0; x[12].operands=1; strcpy(x[12].opSrc, ""); strcpy(x[12].opDest ,"13");strcpy(x[12].command , "red");\
  x[13].opcode=13; x[13].funct=0; x[13].operands=1; strcpy(x[13].opSrc, ""); strcpy(x[13].opDest ,"013");strcpy(x[13].command , "prn");\
  x[14].opcode=14;x[14].funct=0; x[14].operands=0; strcpy(x[14].opSrc ,""); strcpy(x[14].opDest,"");strcpy(x[14].command , "rts");\
  x[15].opcode=15; x[15].funct=0; x[15].operands=0; strcpy(x[15].opSrc,""); strcpy(x[15].opDest,"");strcpy(x[15].command , "stop");


#endif
