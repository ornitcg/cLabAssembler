#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*Initializes the main info about the linked list*/
LinkedList* linkedListInit(size_t bodySize){
    LinkedList* ll = (LinkedList*)malloc(sizeof(LinkedList)); /* ll stands for linked list */
    ll -> head = NULL; /*head is of type Node*/
    ll -> tail = NULL; /*tails is of type Node*/
    ll -> len = 0; /*len is of type int*/
    ll -> bodySize = bodySize;  /*body_size is of type size_t*/
    return ll;
}

void appendNode(short key , void* body , LinkedList* ll){
    Node* newNode = (Node*)malloc(sizeof(Node));
    size_t size = ll -> bodySize;
    newNode->body  = (void*)malloc(size);
    memcpy(newNode->body, body, size);
    newNode -> key = key;
    newNode -> next = NULL;
    newNode -> prev = ll->tail; /*NULL if first*/
    if(ll -> head == NULL) /*if first node*/
        ll -> head = newNode;
    else
        ll -> tail -> next = newNode;
    ll -> tail = newNode;
    (ll -> len) +=1;
}


void printList(LinkedList* ll){
    Node* cursor = ll->head;
    while (cursor-> next != NULL){
        cursor = cursor -> next;
        /*fprintf(stderr, "DEBUG -in printList key is %04d\n",cursor -> key);*/
        return;
    }
}

void killList(LinkedList* ll){
    Node* cursor = ll -> tail;
    while(cursor != (ll -> head)){
        free(cursor -> body);
        /*fprintf(stderr, "DEBUG - %d \n", cursor -> key);*/
        cursor = cursor -> prev;
        free(cursor -> next);
    }
    /*fprintf(stderr, "DEBUG - %d \n", cursor -> key);*/
    if (cursor  == ll -> head)
        free(ll -> head);
    /*fprintf(stderr, "DEBUG - list is killed");*/
}