/*
Author: Ornit Cohen Gindi
Group of functions that relate to the general datastructure of a linked list.
contains just the relevant ones, used in this project
The general structures of a linked list are defined in the linkedList.h file
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linkedList.h"

/*
Initializes the main info about the linked list
params:
size_t bodySize - the size of the bunch of fileds to
be linked at the body field of the linked list nodes
returns: LinkedList - pointer to the new linked list, initiated here
*/
LinkedList* linkedListInit(size_t bodySize){
    LinkedList* ll = (LinkedList*)malloc(sizeof(LinkedList)); /* ll stands for linked list */
    ll -> head = NULL; /*head is of type Node*/
    ll -> tail = NULL; /*tails is of type Node*/
    ll -> len = 0; /*len is of type int*/
    ll -> bodySize = bodySize;  /*body_size is of type size_t*/
    return ll;
}


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
void appendNode(short keyNum , char* keyStr , void* body , LinkedList* ll){
    Node* newNode = (Node*)malloc(sizeof(Node));
    size_t size = ll -> bodySize;
    newNode -> body  = (void*)malloc(size); /*allocate the right size for the body field*/
    memcpy(newNode -> body, body, size); /*copy the body into the allocated memory in node*/
    if (strlen(keyStr)!=0)  /*check if keyStr is relevant*/
        strcpy(newNode -> keyStr, keyStr);

    else                    /*if keyStr is not relevant here then the keyNumber is*/
        newNode -> keyNum = keyNum;
    newNode -> next = NULL;     /*all the connections for the new node*/
    newNode -> prev = ll->tail; /*NULL if first*/
    if(ll -> head == NULL) /*if first node*/
        ll -> head = newNode;
    else
        ll -> tail -> next = newNode;
    ll -> tail = newNode;
    (ll -> len) +=1;
}



/*
Iterates over a given linked list and Frees all
the allocated memory.
params: LinkedList ll - the linked list to free
*/
void killList(LinkedList* ll){
    Node* cursor = ll -> tail;
    while(cursor != (ll -> head)){
        free(cursor -> body);
        cursor = cursor -> prev;
        free(cursor -> next);
    }
    if (cursor  == ll -> head)
        free(ll -> head);
}
