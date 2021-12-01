/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * knowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include "chat1002.h"

 /*
  * Get the response to a question.
  *
  * Input:
  *   intent   - the question word
  *   entity   - the entity
  *   response - a buffer to receive the response
  *   n        - the maximum number of characters to write to the response buffer
  *
  * Returns:
  *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
  *   KB_NOTFOUND, if no response could be found
  *   KB_INVALID, if 'intent' is not a recognised question word
  */
int knowledge_get(const char* intent, const char* entity, char* response, int n) {

	node_ptr intentHeader;

	switch (detectIntent(intent, 0)) {
	case 1:
		intentHeader = what_header;
		break;
	case 2:
		intentHeader = where_header;
		break;
	case 3:
		intentHeader = who_header;
		break;
	default:
		return KB_INVALID;
	}

	// Search for the correct response for the given entity in the linked list.
	while (intentHeader != NULL) {
		if (compare_token(entity, intentHeader->entity) == 0) {
			memccpy(response, intentHeader->response, 0, MAX_RESPONSE);
			return KB_OK;
		}
		intentHeader = intentHeader->next;
	};

	return KB_NOTFOUND;
}


/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char* intent, const char* entity, const char* response) {
	int result;

	if (compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0) {
		// Intent is what.
		return knowledge_update(intent, create_node(entity, response));
	}
	else {
		// Invalid intent.
		return KB_INVALID;
	}
}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file and sets knowledge headers based on information read from file.
 */
int knowledge_read(FILE* f) {

	//	int linesRead				= a counter for number of lines read
	//	char fileLines[MAX_INPUT]	= the line that will be read from file
	//	char* intent				= which header to be set.
	int linesRead = 0;
	char* intent = NULL;
	char fileLines[MAX_INPUT];

	while (!feof(f)) {
		if (ferror(f)) {
			puts("I/O error when reading");
		}
		else if (feof(f)) {
			break;
		}

		while (fgets(fileLines, MAX_RESPONSE, f)) {
			char* entity, * response, * newLine;

			//Check if the line is empty
			if (strcmp(fileLines, "\n") != 0) {

				//Check if newline character is in line.
				newLine = strchr(fileLines, '\n');

				if (newLine != NULL) {
					// Replace new line character with null terminator.
					*newLine = '\0';
				}

				//If square brackets are found, intent is found.
				if (strcmp(fileLines, "[what]") || strcmp(fileLines, "[where]") || strcmp(fileLines, "[who]")) {
					// Check the intent of the line with braces.
					switch (detectIntent(fileLines, 1)) {
					case 1:
						intent = "what";
						break;
					case 2:
						intent = "where";
						break;
					case 3:
						intent = "who";
						break;
					}

					// Loops until every line under the intent is read
					while (!feof(f)) {
						fgets(fileLines, MAX_RESPONSE, f);

						if (strcmp(fileLines, "\n") == 0 || feof(f)) {
							break;
						}

						newLine = strchr(fileLines, '\n');
						if (newLine != NULL) {
								*newLine = '\0';
						}

						entity = strtok(fileLines, "=");
						response = strtok(NULL, "=");

						//put the knowledge from file into linked list.
						if (knowledge_put(intent, entity, response) == KB_OK) {
							linesRead++;
						}
					}
				}
			}
		}
	}
	return linesRead;
}


/*
 * Reset the knowledge base, removing all known entities from all intents.
 */
void knowledge_reset() {
	//Free each linked list and set the header to NULL.
	free_linkedList(what_header);
	what_header = NULL;

	free_linkedList(where_header);
	where_header = NULL;

	free_linkedList(who_header);
	who_header = NULL;
}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE* f) {
	node_ptr what_ptr = what_header;
	node_ptr where_ptr = where_header;
	node_ptr who_ptr = who_header;
	int i = 0;

	// Save the entity and responses for the three intents.

	fprintf(f, "[what]\n");
	while (i == 0) {
		fprintf(f, "%s=%s\n", what_ptr->entity, what_ptr->response);
		what_ptr = what_ptr->next;

		if (what_ptr == NULL) {
			break;
		}
	}

	fprintf(f, "\n[where]\n");
	while (i == 0) {
		fprintf(f, "%s=%s\n", where_ptr->entity, where_ptr->response);
		where_ptr = where_ptr->next;

		if (where_ptr == NULL) {
			break;
		}
	}

	fprintf(f, "\n[who]\n");
	while (i == 0) {
		fprintf(f, "%s=%s\n", who_ptr->entity, who_ptr->response);
		who_ptr = who_ptr->next;

		if (who_ptr == NULL) {
			break;
		}
	}
}

/*
* Helper function to detect the intent.
*
* intent:	The question word
* braces:	For detecting knowledge file
*
* This function will return:
* 1:	The intent is what
* 2:	The intent is where
* 3:	The intent is who
* 0:	Invalid intent for do_question
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
* This function creates a node for the linked list and returns the newly created node.
*
* entity:	'What', 'Who', or 'Where'
* response:	The response to the question.
*/
node_ptr create_node(const char* entity, const char* response) {
	node_ptr node = malloc(sizeof(entRespNode));

	if (node == NULL) {
		printf("/!\\ - malloc() failed for entRespNode* node = malloc(sizeof(entRespNode)), node == NULL\n");
		return NULL;
	}

	memccpy(node->entity, entity, 0, MAX_ENTITY);
	memccpy(node->response, response, 0, MAX_RESPONSE);
	node->next = NULL;

	return node;
}

/*
* This concatenates an array of strings to dest, given the src array and src size.
*
* destination:			Address of destination.
* source:				Address of source char array.
* source_size:			Size of source.
* destination_size:		Size of destination.
* count:				How far from the source.
*/
void strcat_array_of_strings(char* destination, char* source[], int source_size, int destination_size, int count) {
	int length;

	for (int i = count; i < source_size; i++) {
		// Loop through src[offset+i]
		if (i == (source_size - 1)) {
			length = strlen(destination) + strlen(source[i]);
		}
		else {
			length = strlen(destination) + strlen(source[i]) + 1;
		}

		// Check if destination still has space.
		if (length < destination_size) {
			strncat(destination, source[i], destination_size);

			if (i != (source_size - 1)) {
				strncat(destination, " ", strlen(" "));
			}
		}
		else {
			break;
		}
	}

	// Add null terminator to string, for safety.
	destination[strlen(destination)] = '\0';
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
			memccpy(currNode->response, node->response, 0, MAX_RESPONSE);
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
		node = node->next;
		free(temp);
	}
}