#ifndef LINKEDLIST
#define LINKEDLIST

typedef struct Node {
	struct Node *next;
	void *data;
} Node;

typedef struct LinkedList {
	Node *head;
	Node *tail;
	int size;
} LinkedList;

Node* genNode();

LinkedList* genLL();
LinkedList* addNode(LinkedList *ll, Node *n);
LinkedList* addNodeFront(LinkedList *ll, Node *n);

void freeNode(Node *n);
void freeLL(LinkedList *linkedList);

#endif
