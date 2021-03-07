typedef struct LinkedList {
    Node* head;
    Node* tail;
    int len;
    size_t body_size;
}LinkedList ;

typedef struct Node{
    short key;
    void* body;
    struct Node* prev;
    struct Node* next;
} Node;



void appendNode(short key , void* body , LinkedList* ll);
void printList(LinkedList* ll);
void killList(LinkedList* ll);
LinkedList* linkedListInit(size_t body_size);
