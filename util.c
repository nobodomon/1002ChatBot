#include "chat1002.h"
extern node_ptr what_header;
extern node_ptr where_header;
extern node_ptr who_header;

/*
*
* Helper function to detect the intent.
*
* Input:
*	intent	-	the question word
*	braces	-	for detecting knowledge file
* 
* Returns:
*	1		-	the intent is what
*	2		-	the intent is where
*	3		-	the intent is who
*	0		-	invalid intent for do_question
*
*/

int detectIntent(const char* intent, int braces) {
	int result = 0;
	if (braces == 0) {
		if (compare_token(intent, "what") == 0) {
			// Intent is what.
			result = 1;
		}
		else if (compare_token(intent, "where") == 0) {
			// Intent is where.
			result = 2;
		}
		else if (compare_token(intent, "who") == 0) {
			// Intent is who.
			result = 3;
		}
		else {
			return result;
		}
		return result;
	}
	else if (braces == 1) {
		if (compare_token(intent, "[what]") == 0) {
			// Intent is what.
			result = 1;
		}
		else if (compare_token(intent, "[where]") == 0) {
			// Intent is where.
			result = 2;
		}
		else if (compare_token(intent, "[who]") == 0) {
			// Intent is who.
			result = 3;
		}
		else {
			return result;
		}
		return result;
	}
}


/*
* Creates a node for the linked list.
* 
* Inputs:
*	entity			-	The question
*	response		-	The response to the question
* 
* Returns:
*	node [node_ptr]	-	The newly created node	
*	NULL [NULL]		-	If memory allocation fails returns NULL
* 
*/

node_ptr create_node(const char* entity, const char* response) {
	node_ptr node = malloc(sizeof(entRespNode));

	if (node == NULL) {
		printf("/!\\ - malloc() failed for entRespNode* node = malloc(sizeof(entRespNode)), node == NULL\n");
		return NULL;
	}

	memccpy(node->entity, entity, strlen(entity), MAX_ENTITY);
	memccpy(node->response, response, strlen(response), MAX_RESPONSE);
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
			/*
			// Not enough space to store the current string.
			remainder = n - strlen(dest);

			if (remainder > 0) {
				strncat(dest, src[i], remainder);
			}
			// No more space for more strings, break.
			*/
			break;
		}
	}

	// Add null terminator to string, for safety.
	dest[strlen(dest)] = '\0';
}

/*
*	Appends a new node to an existing linked list, such as what_header, where_header, who_header
*
*	Input:
*		head - the linked list to be appended into
*		node - the node that is to be appended into the head
* 
*/
void push_linkedList(node_ptr head, node_ptr node) {
	node_ptr currNode = head;

	do {
		// checks if the entities are the same, if so set the response.
		if (compare_token(currNode->entity, node->entity) == 0) {
			// Entity already exist. Node will not added to the linked list.
			// Copies the memory of input node into temp currNode
			memccpy(currNode->response, node->response, strlen(node->response), MAX_RESPONSE);
			break;
		}
		else if (currNode->next == NULL) {
			// Sets the next element of the last element of head to the new node
			currNode->next = node;
			break;
		}
		// Update the temp pointer.
		currNode = currNode->next;
	} while (currNode != NULL);
}

/*
*	Frees memory used by linked list and it's nodes
*
*	Input:
*		node - the linked list to be freed.
* 
*/
void free_linkedList(node_ptr node) {
	node_ptr temp;
	while (node != NULL)
	{
		temp = node;
		node = node-> next;
		free(temp);
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
	else {
		switch (detectIntent(intent, 0)) {
		// If the intent is "What"
		case 1:
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
		
		// If the intent is "where"
		case 2:
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

		// If the intent is "who"
		case 3:
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

		// If the intent is not 'what', 'where', or 'who'
		default:

			return KB_INVALID; 
		}
	}
}
