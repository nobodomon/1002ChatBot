#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chat1002.h"
extern entRespNode*head_what;
extern entRespNode*head_where;
extern entRespNode*head_who;


void safe_strcat(char *dest, char *src[], size_t src_size, size_t n, int offset) {
	/*
		This function safely concatenates an array of strings.

		Arguments:
			dest 		[char *]:	The pointer to the destination.
			src 		[char *[]]:	The source array of strings.
			src_size	[size_t]:	The number of elements in src.
			n			[size_t]:	The maximum size of the destination.
			offset		[int]:		The position of src to start from.
	*/
	size_t len_check, remainder, last;

	for (int i = offset; i < src_size; i++) {
		if (i != (src_size - 1)) {
			// Account for additional space.
			len_check = strlen(dest) + strlen(src[i]) + 1;
		} else {
			len_check = strlen(dest) + strlen(src[i]);
		}

		if (len_check < n) {
			// "dest" still has space for the next string.
			strncat(dest, src[i], n);

			if (i != (src_size - 1)) {
				// Add a space between words.
				strncat(dest, ' ',1);
			}
		} else {
			// Not enough space to store the current string.
			remainder = n - strlen(dest);

			if (remainder > 0) {
				strncat(dest, src[i], remainder);
			}
			// No more space for more strings, break.
			break;
		}
	}

	// In case string isn't null terminated.
	last = strlen(dest);
	dest[last] = '\0';
}


entRespNode* create_node(const char *entity, const char *response) {
	/*
		This function creates a node.

		Arguments:
			entity 	[char *]:	The entity.
			resp 	[char *]:	The response.

		Returns:
			node	[node_t *]:	The newly created node.
			NULL	[NULL]:		Returns NULL if memory allocation fails.
	*/
	entRespNode*node = malloc(sizeof(entRespNode));

	if (node == NULL) {
		printf("[!] Memory allocation failure.\n");
		return NULL;
	}

	snprintf(node->entity, MAX_ENTITY, "%s", entity);
	snprintf(node->response, MAX_RESPONSE, "%s", response);
	node->next = NULL;

	return node;
}


void push_linkedList(entRespNode*head, entRespNode*node) {
	/*
		This function adds a new node to a linked list.

		Arguments:
			head 	[entRespNode *]:	The head of the linked list
			node 	[entRespNode *]:	The node to add into the linked list.
	*/
	entRespNode*currNode = head;

	do{
		if (compare_token(currNode->entity, node->entity) == 0) {
			// Entity already exist. Overwrite the response.
			snprintf(currNode->response, MAX_RESPONSE, "%s", node->response);
			break;
		} else if (currNode->next == NULL) {
			// End of linked list.
			currNode->next = node;
			break;
		}
		// Update the pointer.
		currNode = currNode->next;
	} while (currNode != NULL);
}


void free_linkedList(entRespNode*node) {
	/*
		This function attempts to free up all nodes in a linked list.

		Arguments:
			node 	[node_t *]:	The head node of the linked list.
	*/
	entRespNode*current;

	while (node != NULL) {
		current = node;
		node = node->next;
		free(current);
	}
}

int kb_update(char* intent, entRespNode* new_node) {
	if (new_node == NULL) {
		//memory allocation for new node failed
		return KB_NOMEM;
	}
	// if the intent is "What"
	else if (compare_token(intent, "what") == 0) {
		if (head_what == NULL) {
			// set head to new node if head of linkedlist what is empty
			head_what = new_node;
		}
		else {
			// Append new_node to the linked list.
			push_linkedList(head_what, new_node);
		}
		return KB_OK;
	}
	// if the intent is "Where"
	else if (compare_token(intent, "where") == 0) {
		if (head_where == NULL) {
			// set head to new node if head of linkedlist where is empty
			head_where = new_node;
		}
		else {
			// Append new_node to the linked list.
			push_linkedList(head_where, new_node);
		}
		return KB_OK;
	}
	// if the intent is "Who"
	else if (compare_token(intent, "who") == 0) {
		if (head_who == NULL) {
			// set head to new node if head of linkedlist who is empty
			head_who = new_node;
		}
		else {
			// Append new_node to the linked list.
			push_linkedList(head_who, new_node);
		}
		return KB_OK;
	}
	else {
	}
}