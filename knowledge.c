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

node_ptr what_header;
node_ptr where_header;
node_ptr who_header;

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
	int result = KB_NOTFOUND;

	switch (detectIntent(intent,0)) {
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

	while (intentHeader != NULL) {
		if (compare_token(entity, intentHeader->entity) == 0) {
			// Found response in knowledge base.
			memccpy(response, intentHeader->response,0, MAX_RESPONSE);
			result = KB_OK;
			break;
		}
		intentHeader = intentHeader->next;
	};

	return result;

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

	//	int linesRead			= a counter for number of lines read
	//	char line[MAX_INPUT]	= the line that will be read from file
	//	char* f_intent			= which header to be set.
	//	int result				= the results from knowledge_update()
	int linesRead = 0, result;
	char line[MAX_INPUT];
	char* f_intent = NULL;

	while (!feof(f)) {
		if (ferror(f)) {
			puts("I/O error when reading");
		}
		else if (feof(f)) {
			break;
		}
		while (fgets(line, MAX_RESPONSE, (FILE*)f)) {
			char* f_entity, * f_response;
			char* carriageReturn, * newLine;

			//Check if the line is empty
			if (strcmp(line, "\r\n") == 0 || strcmp(line, "\n") == 0) {
				//skip to next line
				continue;
			}
			else {
				//Check if carriage return is in line.
				carriageReturn = strchr(line, '\r');
				//Check if newline character is in line.
				newLine = strchr(line, '\n');

				if (carriageReturn != NULL) {
					// Replace carriage return character with null terminator.
					*carriageReturn = '\0';
				}
				else if (newLine != NULL) {
					// Replace new line character with null terminator.
					*newLine = '\0';
				}
				else {
					//getchar to clear stdin if line exceeds MAX_RESPONSE
					int c;
					do {
						c = getchar();
						continue;
					} while (c != '\n' && c != EOF);
				}
				//If square brackets are found, intent is found.
				if (strchr(line, '[') != NULL && strchr(line, ']') != NULL) {
					// Check the intent of the line with braces.
					switch (detectIntent(line, 1)) {
					case 1:
						f_intent = "what";
						break;
					case 2:
						f_intent = "where";
						break;
					case 3:
						f_intent = "who";
						break;
					default:
						f_intent = NULL;
						break;
					}
				}
				//after square brackets line, knowledge will be in next line.
				else if ((f_intent != NULL) && (strchr(line, '=') != NULL)) {
					// Entity-Response pair line.
					f_entity = strtok(line, "=");
					f_response = strtok(NULL, "=");
					//put the knowledge from file into lined list.
					result = knowledge_put(f_intent, f_entity, f_response);

					if (result == KB_OK) {
						// Increment the successful counter.
						linesRead++;
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

	// Save the entity and responses for 'what' intent.
	fprintf(f, "[what]\n");
	while (what_ptr != NULL) {
		fprintf(f, "%s=%s\n", what_ptr->entity, what_ptr->response);
		what_ptr = what_ptr->next;
	}

	// Save the entity and responses for 'where' intent.
	fprintf(f, "\n[where]\n");
	while (where_ptr != NULL) {
		fprintf(f, "%s=%s\n", where_ptr->entity, where_ptr->response);
		where_ptr = where_ptr->next;
	}

	// Save the entity and responses for 'who' intent.
	fprintf(f, "\n[who]\n");
	while (who_ptr != NULL) {
		fprintf(f, "%s=%s\n", who_ptr->entity, who_ptr->response);
		who_ptr = who_ptr->next;
	}
}
