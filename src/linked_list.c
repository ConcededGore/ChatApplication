#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

struct Node* genNode() {
	struct Node* retval = malloc(sizeof(struct Node));
	retval->next = NULL;
	retval->data = NULL;
	return retval;
}

// NOTE THIS WILL NOT FREE MEMORY CORRECTLY IF DATA IS A MULTIDIMENSIONAL ARRAY (use function pointers to define how to free data)
void freeLL(struct Node *linkedList) {
	if (linkedList == NULL) {
		printf("WARNING: freeLL CALLED ON NULL POINTER");
		return;
	}

	if (linkedList->next != NULL) {
		freeLL(linkedList->next);
	}
	free(linkedList->data);
	free(linkedList);
}
