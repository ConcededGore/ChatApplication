#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

Node* genNode() {
	Node* retval = malloc(sizeof(Node));
	retval->next = NULL;
	retval->data = NULL;
	return retval;
}

// Adds to the tail of the list by default
LinkedList* addNode(LinkedList *ll, Node *n) {
	if (ll == NULL) {
		ll = genLL();
	}

	if (ll->tail == NULL) {
		ll->tail = n;
		ll->head = n;
		ll->size = 1;
		return ll;
	}

	ll->tail->next = n;
	ll->tail = n;
	ll->size++;
	return ll;
}

LinkedList* addNodeFront(LinkedList *ll, Node *n) {
	if (ll == NULL) {
		ll = genLL();
	}

	if (ll->head == NULL) {
		ll->head = n;
		ll->tail = n;
		ll->size = 1;
		return ll;
	}

	n->next = ll->head;
	ll->head = n;
	ll->size++;
	return ll;
}

LinkedList* genLL() {
	LinkedList *retval = malloc(sizeof(LinkedList));

	retval->head = NULL;
	retval->tail = NULL;
	retval ->size = 0;

	return retval;
}

// NOTE THIS WILL NOT FREE MEMORY CORRECTLY IF DATA IS A MULTIDIMENSIONAL ARRAY (use function pointers to define how to free data)
void freeNode(Node *n) {
	if (n->next != NULL) {
		freeNode(n->next);
		n->next = NULL;
	}
	free(n->data);
	free(n);
}

void freeLL(LinkedList *ll) {
	freeNode(ll->head);
	free(ll);
}
