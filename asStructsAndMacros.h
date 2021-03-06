#ifndef _STRUCT_AND_MAC_
#define _STRUCT_AND_MAC_

#define MAX_LINE 81
#define MAX_LABEL 31
#define MAX_INSTRUCTION 7
#define MAX_CMD 4
#define REGISTER_LEN 2
#define REGISTERS_NUMBER 8
#define INIT_ADDRESS 100
#define COMMANDS_NUMBER 16
#define LABEL_IDENTIFIER ':'
#define COMMA ','

#define COMMENT_IDENTIFIER_INT ';'
#define COMMENT_IDENTIFIER ";"
#define INSTRUCTION_IDENTIFIER '.'
#define IMMEDIATE_IDENTIFIER "#"
#define RELATIVE_IDENTIFIER "%"

enum attribute {Code , Data,  Entry, External};
enum inst {NO, Data, String, Entry, Extern};


typedef char regArr[REGISTERS_NUMBER][REGISTER_LEN] ; /*for array of registers names*/

struct STATUS { /*to use as PSW*/
    int IC;
    int DC;
    int lineNumber;
    enum inst instType;
    int errorExists; /*YES/NO*/
    int symbolFound; /*YES/NO*/
};
typedef struct STATUS STATUS;


/**************************************************SYMBOL TABLE*****************************************/
struct SYMBOL {
    int dcStat;
    char symbol[MAX_LABEL];
    enum attribute attr1;
    enum attribute attr2; /*sometimes there maybe 2 attributes*/
};
typedef struct SYMBOL SYMBOL;

void addSymbol();

/*****************************************CODE (INSTRUCTIONS)TABLE ***************************************/
struct CODE_IMG {
    int icStat;
    char inputLine[MAX_LINE];
    short data;
    short machineCode;
    struct CODE_IMG* prev;
    struct CODE_IMG* next;

};
typedef struct CODE_IMG CODE_IMG;

void addCode(CODE_IMG* codeImg);

/*********************************************DATA TABLE*******************************************************/

struct DATA_IMG{
    int dcStat;
    char inputLine[MAX_LINE];
    short machineCode;
    struct DATA_IMG* prev;
    struct DATA_IMG* next;

};
typedef struct DATA_IMG DATA_IMG;

void addData(DATA_IMG* codeImg);



/*****************************************COMMAND (CMD) TABLE*********************************************/


/*This is the table of 16 commands, to be defined int the first pass function using macro*/
struct COMMAND{
    short int opcode;
    short int funct;
    short int operators;
    char command[MAX_CMD_LEN];
};
typedef struct COMMAND CMD_TABLE;

#define SET_CMD_TABLE(cmd) CMD_TABLE cmd[16];\
  cmd[0].opcode=0; cmd[0].funct=0; cmd[0].operators=2; strcpy(cmd[0].command, "mov");\
  cmd[1].opcode=1; cmd[1].funct=0; cmd[1].operators=2; strcpy(cmd[1].command, "cmp");\
  cmd[2].opcode=2; cmd[2].funct=10; cmd[2].operators=2; strcpy(cmd[2].command , "add");\
  cmd[3].opcode=2; cmd[3].funct=11; cmd[3].operators=2; strcpy(cmd[3].command , "sub");\
  cmd[4].opcode=4; cmd[4].funct=0; cmd[4].operators=2; strcpy(cmd[4].command , "lea");\
  cmd[5].opcode=5; cmd[5].funct=10; cmd[5].operators=1; strcpy(cmd[5].command , "clr");\
  cmd[6].opcode=5; cmd[6].funct=11; cmd[6].operators=1; strcpy(cmd[6].command , "not");\
  cmd[7].opcode=5; cmd[7].funct=12; cmd[7].operators=1; strcpy(cmd[7].command , "inc");\
  cmd[8].opcode=5; cmd[8].funct=13; cmd[8].operators=1; strcpy(cmd[8].command , "dec");\
  cmd[9].opcode=9; cmd[9].funct=10; cmd[9].operators=1; strcpy(cmd[9].command , "jmp");\
  cmd[10].opcode=9; cmd[10].funct=11; cmd[10].operators=1; strcpy(cmd[10].command , "bne");\
  cmd[11].opcode=9; cmd[11].funct=12; cmd[11].operators=1; strcpy(cmd[11].command , "jsr");\
  cmd[12].opcode=12; cmd[12].funct=0; cmd[12].operators=1; strcpy(cmd[12].command , "red");\
  cmd[13].opcode=13; cmd[13].funct=0; cmd[13].operators=1; strcpy(cmd[13].command , "prn");\
  cmd[14].opcode=14;cmd[14].funct=0; cmd[14].operators=0; strcpy(cmd[14].command , "rts");\
  cmd[15].opcode=15; cmd[15].funct=0; cmd[15].operators=0; strcpy(cmd[15].command , "stop");


#endif
