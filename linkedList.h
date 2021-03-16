#ifndef _LINKE_LIST_
#define _LINKE_LIST_
#define MAX_STRING 100 /*some arbitrary general length*/


/*Structures of the general node and linked list*/
typedef struct Node{
    short keyNum;
    char keyStr[MAX_STRING]; /*to enable Label sort of key*/
    void* body;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct LinkedList {
    Node* head;
    Node* tail;
    int len;
    size_t bodySize;
}LinkedList ;


/*
Initializes the main info about the linked list
params:
size_t bodySize - the size of the bunch of fileds to
be linked at the body field of the linked list nodes
returns: LinkedList - pointer to the new linked list, initiated here
*/
LinkedList* linkedListInit(size_t body_size);


/*
Adds a new node to the end of the given linked list
only one type of key is relevant to each linkelist.
for the irrelevant thpe of key: the numeric type gets 0
ans the string type get EMPTY_STRING
params:
short keyNum- the numeric key given to the new node
char* keyStr- the string key given to the new node
void* body- set of fields that are relevant to the linked list
LinkedList ll- the relevant linked list
*/
void appendNode(short keyNum, char* keyStr , void* body , LinkedList* ll);



/*
Iterates over a given linked list and Frees all
the allocated memory.
params: LinkedList ll - the linked list to free
*/
void killList(LinkedList* ll);






#endif
