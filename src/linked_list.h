#ifndef LINKEDLIST
#define LINKEDLIST

struct Node {
	struct Node *next;
	void *data;
};

struct Node* genNode();
void freeLL(struct Node *linkedList);

#endif
