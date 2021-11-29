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

extern entRespNode* what_header;
extern entRespNode* where_header;
extern entRespNode* who_header;

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

	entRespNode* search;
	int result = KB_NOTFOUND;

	if (compare_token(intent, "what") == 0) {
		// Intent is what.
		search = what_header;
	}
	else if (compare_token(intent, "where") == 0) {
		// Intent is where.
		search = where_header;
	}
	else if (compare_token(intent, "who") == 0) {
		// Intent is who.
		search = who_header;
	}
	else {
		// Invalid intent.
		return KB_INVALID;
	}

	while (search != NULL) {
		if (compare_token(entity, search->entity) == 0) {
			// Found response in knowledge base.
			snprintf(response, n, "%s", search->response);
			result = KB_OK;
			break;
		}
		search = search->next;
	}

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
		result = knowledge_update(intent, create_node(entity, response));
	}
	else {
		// Invalid intent.
		result = KB_INVALID;
	}

	return result;
}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE* f) {

	/*
		ctr:	The number of successful results retrieved from the file.
		result:	The result of inserting the entity-response into the knowledge base.
		line:	The buffer to store content of the file.
	*/
	int ctr = 0, result;
	char line[MAX_INPUT];
	char* f_intent = NULL;

	//const size_t MAX_READLEN = (size_t)(MAX_ENTITY + MAX_RESPONSE);

	while (fgets(line, MAX_RESPONSE, (FILE*)f)) {
		char* f_entity, * f_response;
		char* cr, * nl;

		// Read line by line.
		if (strcmp(line, "\n") == 0 || strcmp(line, "\r\n") == 0) {
			// Empty line.
			continue;
		}
		else {
			cr = strchr(line, '\r');
			nl = strchr(line, '\n');

			if (cr != NULL) {
				// Carriage return found, replace it with null terminator.
				*cr = '\0';
			}
			else if (nl != NULL) {
				// newline found, replace it with null terminator.
				*nl = '\0';
			}
			else {
				// Clear any remaining input to prevent overflow.
				int c;
				while ((c = getchar()) != '\n' && c != EOF) {
					continue;
				}
			}

			if (strchr(line, '[') != NULL && strchr(line, ']') != NULL) {
				// Square brackets found. Check intent.
				if (compare_token(line, "[what]") == 0) {
					// Intent: what.
					f_intent = "what";
				}
				else if (compare_token(line, "[where]") == 0) {
					// Intent: where.
					f_intent = "where";
				}
				else if (compare_token(line, "[who]") == 0) {
					// Intent: who.
					f_intent = "who";
				}
				else {
					// Invalid intent.
					f_intent = NULL;
				}
			}
			else if (f_intent != NULL && strchr(line, '=') != NULL) {
				// Entity-Response pair line.
				f_entity = strtok(line, "=");
				f_response = strtok(NULL, "=");
				result = knowledge_put(f_intent, f_entity, f_response);

				if (result == KB_OK) {
					// Increment the successful counter.
					ctr++;
				}
			}
		}
	}

	return ctr;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {
	free_linkedList(what_header);
	free_linkedList(where_header);
	free_linkedList(who_header);

	what_header = NULL;
	where_header = NULL;
	who_header = NULL;
}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE* f) {
	entRespNode* what = what_header;
	entRespNode* where = where_header;
	entRespNode* who = who_header;

	// Save "what" intent's entitiy-responses.
	fprintf(f, "[what]\n");
	while (what != NULL) {
		fprintf(f, "%s=%s\n", what->entity, what->response);
		what = what->next;
	}

	// Save "where" intent's entitiy-responses.
	fprintf(f, "\n[where]\n");
	while (where != NULL) {
		fprintf(f, "%s=%s\n", where->entity, where->response);
		where = where->next;
	}

	// Save "what" intent's entitiy-responses.
	fprintf(f, "\n[who]\n");
	while (who != NULL) {
		fprintf(f, "%s=%s\n", who->entity, who->response);
		who = who->next;
	}
}
