/*
  Group of helper functions that are related to assembler
*/
#define MAX_LINE 80
#define MAX_LABEL 31
#define MAX_CMD_LEN 4
#define INIT_ADDRESS 100
#define YES 1
#define NO 0
#define LABEL_IDENTIFIER ':'
#define COMMENT_IDENTIFIER ';'
#define CMD_IDENTIFIER '.'
#define IMMEDIATE_IDENTIFIER '#'
#define RELATIVE_IDENTIFIER '%'


struct PROGRAM_STATUS {
  int IC;
  int DC;
  int errorExists;
  int symbolFound;
};
typedef struct PROGRAM_STATUS STATUS;

struct CMD{
  int opcode;
  int funct;
  char command[MAX_CMD_LEN];
};
typedef struct CMD CMD;

#define SET_CMD_TABLE   CMD cmd[16]; \
  cmd[0].opcode=0; cmd[0].funct=0; cmd[0].command = "mov";\
  cmd[1].opcode=1; cmd[1].funct=0; cmd[1].command = "cmp";\
  cmd[2].opcode=2; cmd[2].funct=10; cmd[2].command = "add";\
  cmd[3].opcode=2; cmd[3].funct=11; cmd[3].command = "sub";\
  cmd[4].opcode=4; cmd[4].funct=0; cmd[4].command = "lea";\
  cmd[5].opcode=5; cmd[5].funct=10; cmd[5].command = "clr";\
  cmd[6].opcode=5; cmd[6].funct=11; cmd[6].command = "not";\
  cmd[7].opcode=5; cmd[7].funct=12; cmd[7].command = "inc";\
  cmd[8].opcode=5; cmd[8].funct=13; cmd[8].command = "dec";\
  cmd[9].opcode=9; cmd[9].funct=10; cmd[9].command = "jmp";\
  cmd[10].opcode=9; cmd[10].funct=11; cmd[10]command = "bne";\
  cmd[11].opcode=9; cmd[11].funct=12; cmd[11].command = "jsr";\
  cmd[12].opcode=12; cmd[12].funct=0; cmd[12].command = "red";\
  cmd[13].opcode=13; cmd[13].funct=0; cmd[13].command = "prn";\
  cmd[14].opcode=14;cmd[14].funct=0; cmd[14].command = "rts";\
  cmd[15].opcode=15; cmd[15].funct=0; cmd[15].command = "stop";\

struct FIRST_WORD{
  short dest;
  short src;
  short funct;
  short opcode;
};
typedef struct FIRST_WORD FIRST_WORD;

struct OUTPUT_TABLE {
  int adress;
  WORD machineCode;
  char ARE;
};

typedef char regArr[8][3] ;
/*
  Checks the possibility of the line to contain a label by searching for a ':'
  params:   char* line - the string to search for a label in
  returns:   1 if found, and 0 otherwise
*/
int containsLabelDef(char* line);

/*
  Gets the label from line
  params:   char* line - the string to get the label from
  returns:   string of the label if label is valid, and NULL otherwise
*/
char* getLabel(line);

char* parseNext(char* line, char* delimiter)
int toIgnore(char* line);
