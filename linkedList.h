#ifndef _LINKE_LIST_
#define _LINKE_LIST_
#define MAX_STRING 100 /*some arbitrary general length*/

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




void appendNode(short keyNum, char* keyStr , void* body , LinkedList* ll);
void killList(LinkedList* ll);
LinkedList* linkedListInit(size_t body_size);



#endif
