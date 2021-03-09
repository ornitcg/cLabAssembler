#ifndef _STRUCT_AND_MAC_
#define _STRUCT_AND_MAC_

#include "linkedList.h"
#include "utilsGeneral.h"

#define MAX_LINE 81
#define MAX_LABEL 31
#define MAX_INSTRUCTION 7
#define MAX_CMD_LEN 4
#define REGISTER_LEN 3
#define REGISTERS_NUMBER 8
#define INIT_ADDRESS 100
#define COMMANDS_NUMBER 16
#define LABEL_IDENTIFIER ':'
#define COMMENT_IDENTIFIER_INT ';'
#define COMMENT_IDENTIFIER ";"
#define INSTRUCTION_IDENTIFIER '.'
#define IMMEDIATE_IDENTIFIER "#"
#define RELATIVE_IDENTIFIER "%"

enum info {No, Yes, Ok, Code , Data, String,  Entry, Extern , Empty ,Error = -1};
typedef enum info info;

typedef struct STATUS{ /*to use as PSW*/
    int IC;
    int DC;
    int lineNumber;
    info errorExists; /*YES/NO*/
    info symbolFound; /*YES/NO*/
    char fileName[FILENAME_MAX];
    int commandNumber;
    LinkedList* symbolTable;            /*starting a linked list*/
    LinkedList* codeTable;         /*starting a linked list*/
    LinkedList* dataTable;          /*starting a linked list*/
} STATUS;

void initStatus(STATUS* stat, char* fileName);
/**************************************************SYMBOL TABLE*****************************************/
typedef struct SYMBOL {
    short address;
    enum info attr1;
    enum info attr2; /*sometimes there maybe 2 attributes*/
} SYMBOL;
void addSymbol(LinkedList* symbolTable, short address, char* symbol, info attr1, info attr2);
SYMBOL* lookupSymbol(LinkedList* symbolTable, char* symbol);
/*****************************************CODE (INSTRUCTIONS)TABLE ***************************************/
typedef struct CODE_IMG {
    char inputLine[MAX_LINE];
    short code;
    char ARE;
} CODE_IMG;
void addCode(LinkedList* codeTable, short address, char* inputLine, short code, char ARE);

/*********************************************DATA TABLE*******************************************************/
typedef struct DATA_IMG {
    short data;
    char ARE;
} DATA_IMG;

void addData(LinkedList* dataTable, short address, short code, char ARE);

/*********************************************WORD STRUCTURE********************************************************/
struct FIRST_WORD{
  short dest;
  short src;
  short funct;
  short opcode;
};
typedef struct FIRST_WORD FIRST_WORD;
/*****************************************COMMAND (x) TABLE*********************************************/

typedef char regArr[REGISTERS_NUMBER][REGISTER_LEN] ; /*for array of registers names*/
#define SET_REG_ARR(x)  regArr x = {"r0","r1","r2", "r3","r4","r5","r6","r7"}

typedef char reservedArr[5][MAX_LABEL] ; /*for array of registers names*/
#define SET_RESERVED_ARR(x)  reservedArr x = {"data", "string", "entry","extern", "NULL"};

#define RES_WORDS_LIST {"data", "string", "entry","extern", "NULL"}
/*#define char otherReserved*/
/*****************************************COMMAND (x) TABLE*********************************************/

/*This is the table of 16 commands, to be defined int the first pass function using macro*/
typedef struct {
    short int opcode;
    short int funct;
    short int operators;
    char command[MAX_CMD_LEN];
} CMD;

#define SET_COMMAND_TABLE(x) CMD x[16];\
  x[0].opcode=0; x[0].funct=0; x[0].operators=2; strcpy(x[0].command, "mov");\
  x[1].opcode=1; x[1].funct=0; x[1].operators=2; strcpy(x[1].command, "cmp");\
  x[2].opcode=2; x[2].funct=10; x[2].operators=2; strcpy(x[2].command , "add");\
  x[3].opcode=2; x[3].funct=11; x[3].operators=2; strcpy(x[3].command , "sub");\
  x[4].opcode=4; x[4].funct=0; x[4].operators=2; strcpy(x[4].command , "lea");\
  x[5].opcode=5; x[5].funct=10; x[5].operators=1; strcpy(x[5].command , "clr");\
  x[6].opcode=5; x[6].funct=11; x[6].operators=1; strcpy(x[6].command , "not");\
  x[7].opcode=5; x[7].funct=12; x[7].operators=1; strcpy(x[7].command , "inc");\
  x[8].opcode=5; x[8].funct=13; x[8].operators=1; strcpy(x[8].command , "dec");\
  x[9].opcode=9; x[9].funct=10; x[9].operators=1; strcpy(x[9].command , "jmp");\
  x[10].opcode=9; x[10].funct=11; x[10].operators=1; strcpy(x[10].command , "bne");\
  x[11].opcode=9; x[11].funct=12; x[11].operators=1; strcpy(x[11].command , "jsr");\
  x[12].opcode=12; x[12].funct=0; x[12].operators=1; strcpy(x[12].command , "red");\
  x[13].opcode=13; x[13].funct=0; x[13].operators=1; strcpy(x[13].command , "prn");\
  x[14].opcode=14;x[14].funct=0; x[14].operators=0; strcpy(x[14].command , "rts");\
  x[15].opcode=15; x[15].funct=0; x[15].operators=0; strcpy(x[15].command , "stop");


#endif
