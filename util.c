#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chat1002.h"
extern entRespNode*what_header;
extern entRespNode*where_header;
extern entRespNode*who_header;


entRespNode* create_node(const char* entity, const char* response) {
	/*
		This function creates a node for the linked list.

		Arguments:
			entity 	[char *]:	The entity.
			resp 	[char *]:	The response.

		Returns:
			node	[node_t *]:	The newly created node.
			NULL	[NULL]:		Returns NULL if memory allocation fails.
	*/
	entRespNode* node = malloc(sizeof(entRespNode));

	if (node == NULL) {
		printf("!! Memory allocation failure.\n");
		return NULL;
	}

	snprintf(node->entity, MAX_ENTITY, "%s", entity);
	snprintf(node->response, MAX_RESPONSE, "%s", response);
	node->next = NULL;

	return node;
}


void safe_strcat(char *dest, char *src[], size_t src_size, size_t n, int offset) {
	/*
		This function safely concatenates an array of strings to make it does not overrun the given memory space.

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
				strcat(dest," ");
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


void push_linkedList(entRespNode*head, entRespNode*node) {
	/*
		This function adds a new node to a linked list.

		Arguments:
			head 		[entRespNode *]:	The head of the linked list
			node 		[entRespNode *]:	The node to add into the linked list.
			currNode 	[entRespNode *]:	Temp node that points to existing nodes in the linked list.
	*/
	entRespNode*currNode = head;

	do{
		if (compare_token(currNode->entity, node->entity) == 0) {
			// Entity already exist. Node not added to the linked list.
			snprintf(currNode->response, MAX_RESPONSE, "%s", node->response);
			break;
		} else if (currNode->next == NULL) {
			// End of linked list. Add the new node to the linked list.
			currNode->next = node;
			break;
		}
		// Update the temp pointer.
		currNode = currNode->next;
	} while (currNode != NULL);
}


void free_linkedList(entRespNode*node) {
	/*
		This function free up all nodes in a linked list.

		Arguments:
			node 		[node_t *]:			The head node of the linked list.
			currNode 	[entRespNode *]:	Temp node that points to existing nodes in the linked list.
	*/
	entRespNode*current;

	while (node != NULL) {
		current = node;
		node = node->next;
		free(current);
	}
}

int knowledge_update(char* intent, entRespNode* new_node) {
	/*
		This function checks where the statement is 'what', 'where', or 'who', then add the new node to the appropriate linked list.

		Arguments:
			intent 		[char *]:			The intent of the statement.
			new_node 	[entRespNode *]:	The new node to be added to the linked list.
	*/

	if (new_node == NULL) {
		//memory allocation for new node failed
		return KB_NOMEM;
	}
	// If the intent is "What"
	else if (compare_token(intent, "what") == 0) {
		if (what_header == NULL) {
			// set head to new node if head of linkedlist what is empty
			what_header = new_node;
		}
		else {
			// Append new_node to the linked list.
			push_linkedList(what_header, new_node);
		}
		return KB_OK;
	}
	// If the intent is "Where"
	else if (compare_token(intent, "where") == 0) {
		if (where_header == NULL) {
			// set head to new node if head of linkedlist where is empty
			where_header = new_node;
		}
		else {
			// Append new_node to the linked list.
			push_linkedList(where_header, new_node);
		}
		return KB_OK;
	}
	// If the intent is "Who"
	else if (compare_token(intent, "who") == 0) {
		if (who_header == NULL) {
			// set head to new node if head of linkedlist who is empty
			who_header = new_node;
		}
		else {
			// Append new_node to the linked list.
			push_linkedList(who_header, new_node);
		}
		return KB_OK;
	}
}