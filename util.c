#include "chat1002.h"
extern node_ptr what_header;
extern node_ptr where_header;
extern node_ptr who_header;


node_ptr create_node(const char* entity, const char* response) {
	/*
		This function creates a node for the linked list.

		Arguments:
			entity 	[char *]:	The entity.
			resp 	[char *]:	The response.

		Returns:
			node	[node_ptr]:	The newly created node.
			NULL	[NULL]:		Returns NULL if memory allocation fails.
	*/
	node_ptr node = malloc(sizeof(entRespNode));

	if (node == NULL) {
		printf("/!\\ - malloc() failed for entRespNode* node = malloc(sizeof(entRespNode)), node == NULL\n");
		return NULL;
	}

	snprintf(node->entity, MAX_ENTITY, "%s", entity);
	snprintf(node->response, MAX_RESPONSE, "%s", response);
	node->next = NULL;

	return node;
}


void strcat_array_of_strings(char* dest, char* src[], size_t src_size, size_t n, int offset) {
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
		// Loop through src[offset+i]
		if (i == (src_size - 1)) {
			len_check = strlen(dest) + strlen(src[i]);
		}
		else {
			// Account for additional space.
			len_check = strlen(dest) + strlen(src[i]) + 1;
		}

		if (len_check < n) {
			// "dest" still has space for the next string.
			strncat(dest, src[i], n);

			if (i != (src_size - 1)) {
				// Add a space between words.
				strncat(dest, " ", strlen(" "));
			}
		}
		else {
			// Not enough space to store the current string.
			remainder = n - strlen(dest);

			if (remainder > 0) {
				strncat(dest, src[i], remainder);
			}
			// No more space for more strings, break.
			break;
		}
	}

	// Add null terminator to string, for safety.
	dest[strlen(dest)] = 0;
}


void push_linkedList(node_ptr head, node_ptr node) {
	/*
		This function adds a new node to a linked list.

		Arguments:
			head 		[node_ptr]:		The head of the linked list
			node 		[node_ptr]:		The node to add into the linked list.

		Variables:
			currNode 	[node_ptr]:		Temp node that points to existing nodes in the linked list.
	*/
	node_ptr currNode = head;

	do {
		if (compare_token(currNode->entity, node->entity) == 0) {
			// Entity already exist. Node will not added to the linked list.
			snprintf(currNode->response, MAX_RESPONSE, "%s", node->response);
			break;
		}
		else if (currNode->next == NULL) {
			// Reached end of linked list. Add the new node to the end of linked list.
			currNode->next = node;
			break;
		}
		// Update the temp pointer.
		currNode = currNode->next;
	} while (currNode != NULL);
}


void free_linkedList(node_ptr node) {
	/*
		This function free up all nodes in a linked list.

		Arguments:
			node 		[node_ptr]:			The head node of the linked list.
	*/

	node_ptr temp = node;
	node_ptr next = NULL;
	while (temp != NULL)
	{
		next = temp->next;
		free(temp);
		temp = next;
	}
}

int knowledge_update(char* intent, node_ptr new_node) {
	/*
		This function checks where the statement is 'what', 'where', or 'who', then add the new node to the appropriate linked list.

		Arguments:
			intent 		[char *]:			The intent of the statement.
			new_node 	[node_ptr]:			The new node to be added to the linked list.
	*/

	if (new_node == NULL) {
		// memory allocation for new node failed
		printf("/!\\ - knowledge_update(char* intent, node_ptr new_node) new_node == NULL\n");
		printf("did malloc() fail?\n");
		return KB_NOMEM;
	}
	// If the intent is "What"
	else if (compare_token(intent, "what") == 0) {
		if (what_header != NULL) {
			// linkedlist not empty, append new_node to the "what" linked list.
			push_linkedList(what_header, new_node);
		}
		else {
			// linkedlist is empty
			// set head to new node if head of linkedlist "what" is empty
			what_header = new_node;
		}
		return KB_OK;
	}
	// If the intent is "Where"
	else if (compare_token(intent, "where") == 0) {
		if (where_header != NULL) {
			// linkedlist not empty, append new_node to the "where" linked list.
			push_linkedList(where_header, new_node);
		}
		else {
			// linkedlist is empty
			// set head to new node if head of linkedlist "where" is empty
			where_header = new_node;
		}
		return KB_OK;
	}
	// If the intent is "Who"
	else if (compare_token(intent, "who") == 0) {
		if (who_header != NULL) {
			// linkedlist not empty, append new_node to the "who" linked list.
			push_linkedList(who_header, new_node);
		}
		else {
			// linkedlist is empty
			// set head to new node if head of linkedlist "who" is empty
			who_header = new_node;
		}
		return KB_OK;
	}
	else
	{
		return KB_INVALID; // not 'what', 'where', or 'who'
	}
}
