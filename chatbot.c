/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include "chat1002.h"

 // Header for 'what','where' and 'who' entity linked list
extern entRespNode* what_header;
extern entRespNode* where_header;
extern entRespNode* who_header;


/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char* chatbot_botname() {

	return "Chatbot";

}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char* chatbot_username() {

	return "User";

}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char* inv[], char* response, int n) {

	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	char intent[MAX_INPUT] = { 0 };
	strcat_array_of_strings(intent, inv, inc, MAX_INPUT - 1, 0);

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(intent))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char* intent) {

	const char* exitConditions[] = { "exit", "quit", "bye", "goodbye" };
	int size = sizeof(exitConditions) / sizeof(exitConditions[0]);

	for (int i = 0; i < size; i++)
	{
		if (compare_token(intent, exitConditions[i]) == 0)
		{
			return 1;
		}
	}

	return 0;
}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char* inv[], char* response, int n) {

	// Free the allocated memory.
	knowledge_reset();
	snprintf(response, n, "Goodbye!");

	return 1;

}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char* intent) {
	if (compare_token(intent, "load") == 0) {
		return 1;
	}
	else {
		return 0;
	}
}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char* inv[], char* response, int n) {
	/*
		file_ptr:		The file pointer.
	*/
	FILE* file_ptr;
	char file_path[MAX_INPUT];

	char* point;
	char format[5] = ".ini";
	format[strlen(format)] = '\0';

	// Get the file path from the user input.
	if (inv[1] == NULL) {
		// If the load statement has 1 word. If file not stated, prompt user for filename.

		// LOAD[0]
		char tempFilePath[MAX_INPUT] = "";
		printf("%s: What file would you like to load?\n", chatbot_botname());
		printf("%s:", chatbot_username());
		fgets(tempFilePath, MAX_INPUT, stdin);
		tempFilePath[strlen(tempFilePath) - 1] = '\0';

		// If user did not indicate file type, default it as a '.ini'.
		if ((point = strrchr(tempFilePath, '.')) == NULL) {
			strcat(tempFilePath, format);
		}

		strncpy(file_path, tempFilePath, strlen(tempFilePath));
		file_path[strlen(tempFilePath)] = 0;
	}
	else if (inv[2] == NULL) {
		// If the load statement has 2 words. If file not stated, prompt user for filename.

		if (compare_token(inv[1], "from") == 0) {
			//LOAD[0] from[1]

			char tempFilePath[MAX_INPUT] = "";
			printf("%s: What file would you like to load?\n", chatbot_botname());
			printf("%s:", chatbot_username());
			fgets(tempFilePath, MAX_INPUT, stdin);
			tempFilePath[strlen(tempFilePath) - 1] = '\0';

			// If user did not indicate file type, default it as a '.ini'.
			if ((point = strrchr(tempFilePath, '.')) == NULL) {
				strcat(tempFilePath, format);
			}

			strncpy(file_path, tempFilePath, strlen(tempFilePath));
			file_path[strlen(tempFilePath)] = 0;
		}
		else if (compare_token(inv[0], "load") == 0 && inv[1] != NULL) {
			// LOAD[0] /file.txt[1]

			// If user did not indicate file type, default it as a '.ini'.
			if ((point = strrchr(inv[1], '.')) == NULL) {
				strcat(inv[1], format);
			}

			strncpy(file_path, inv[1], strlen(inv[1]));
			file_path[strlen(inv[1])] = 0;
		}
	}
	else if (inv[3] == NULL) {
		// If the load statement has 3 words. If file not stated, prompt user for filename.

		if (compare_token(inv[1], "the") == 0 && compare_token(inv[2], "file") == 0) {
			// LOAD[0] the[1] file[2]

			char tempFilePath[MAX_INPUT] = "";
			printf("%s: What file would you like to load?\n", chatbot_botname());
			printf("%s:", chatbot_username());
			fgets(tempFilePath, MAX_INPUT, stdin);
			tempFilePath[strlen(tempFilePath) - 1] = '\0';

			// If user did not indicate file type, default it as a '.ini'.
			if ((point = strrchr(tempFilePath, '.')) == NULL) {
				strcat(tempFilePath, format);
			}

			strncpy(file_path, tempFilePath, strlen(tempFilePath));
			file_path[strlen(tempFilePath)] = 0;
		}
		else if (compare_token(inv[0], "load") == 0 && compare_token(inv[1], "from") == 0) {
			// LOAD[0] from[1] /file.txt[2]

			// If user did not indicate file type, default it as a '.ini'.
			if ((point = strrchr(inv[2], '.')) == NULL) {
				strcat(inv[2], format);
			}

			strncpy(file_path, inv[2], strlen(inv[2]));
			file_path[strlen(inv[2])] = 0;
		}
	}
	else if (inv[4] == NULL) {
		// If the load statement has 4 words.

		if (compare_token(inv[1], "the") == 0 && compare_token(inv[2], "file") == 0) {
			// LOAD[0] the[1] file[2] /file.txt[3]

			// If user did not indicate file type, default it as a '.ini'.
			if ((point = strrchr(inv[3], '.')) == NULL) {
				strcat(inv[3], format);
			}

			strncpy(file_path, inv[3], strlen(inv[3]));
			file_path[strlen(inv[3])] = 0;
		}
	}

	// Open the file indicated by user in read mode.
	file_ptr = fopen(file_path, "r");

	if (file_ptr != NULL) {
		knowledge_read(file_ptr);
		// Print if file exists.
		fclose(file_ptr);
		snprintf(response, n, "I have regained my memories by reading the file '[%s]', much thanks!", file_path);
	}
	else {
		// Print if file does not exist or cannot be found.
		snprintf(response, n, "Unfortunately, I cannot read the file.");
	}

	return 0;
}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char* intent) {

	const char* questions[] = { "what", "what", "who" };
	int size = sizeof(questions) / sizeof(questions[0]);

	for (int i = 0; i < size; i++)
	{
		if (compare_token(intent, questions[i]) == 0)
		{
			return 1;
		}
	}

	return 0;
}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char* inv[], char* response, int n) {

	/*
		unknown:	Store text for unknown questions.
		ent:		Store text for the entity.
		reply:		Store text for the reply to the question.
	*/
	char unknown[MAX_RESPONSE] = "I don't know the answer to that question. ";
	char ent[MAX_ENTITY] = "";
	char reply[MAX_RESPONSE] = "";

	size_t offset = 1;

	// Check where does the question start.
	if (compare_token(inv[1], "is") == 0 || compare_token(inv[1], "are") == 0) {
		offset = 2;
	}

	strcat_array_of_strings(ent, inv, inc, (MAX_ENTITY - 1), offset);

	if (knowledge_get(inv[0], ent, response, n) == KB_NOTFOUND) {
		inv[0][0] = toupper(inv[0][0]);
		strcat_array_of_strings(unknown, inv, inc, (MAX_RESPONSE - 1), 0);
		strncat(unknown, "?", strlen("?") + 1);

		prompt_user(reply, MAX_RESPONSE, "%s", unknown);

		knowledge_put(inv[0], ent, reply);
		snprintf(response, n, "Thank you for teaching me!");
	}

	return 0;
}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char* intent) {
	if (compare_token(intent, "reset") == 0) {
		return 1;
	}
	else {
		return 0;
	}
}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char* inv[], char* response, int n) {
	// Reseed the random number generator.
	srand((unsigned)(time(NULL)));

	// Reset the chatbot's knowledge.
	knowledge_reset();
	snprintf(response, n, "Beep Boop. I have been resetted.");
	return 0;
}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char* intent) {

	if (compare_token(intent, "save") == 0) {
		return 1;
	}
	else {
		return 0;
	}
}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char* inv[], char* response, int n) {
	/*
		file_ptr:		A pointer to the file.
	*/
	FILE* file_ptr;
	char file_path[MAX_INPUT];

	char* point;
	char format[5] = ".ini";
	format[strlen(format)] = '\0';

	// Get the file path from the user input.
	if (compare_token(inv[1], "to") == 0 || compare_token(inv[1], "as") == 0) {
		// Save[0] to[1] /file.txt[2] or Save[0] as[1] /file.txt[2]

		// If user did not indicate file type, set it as a '.ini'.
		if ((point = strrchr(inv[2], '.')) == NULL) {
			strcat(inv[2], format);
		}

		strncpy(file_path, inv[2], strlen(inv[2]));
		file_path[strlen(inv[2])] = 0;
	}
	else {
		// Save[0] /file.txt[1]

		// If user did not indicate file type, set it as a '.ini'.
		if ((point = strrchr(inv[1], '.')) == NULL) {
			strcat(inv[1], format);
		}

		strncpy(file_path, inv[1], strlen(inv[1]));
		file_path[strlen(inv[1])] = 0;
	}

	//Open file in read mode to check if it exists.
	FILE* temp;
	temp = fopen(file_path, "r");

	char check[MAX_INPUT] = "";

	// Check if file already exists. If it does, check if user wants to overwrite.
	if (temp) {
		fclose(temp);

		printf("%s: The file '[%s]' already exists. Do you want to overwrite the existing file?\n", chatbot_botname(), file_path);
		printf("%s:", chatbot_username());
		fgets(check, MAX_INPUT, stdin);
		check[strlen(check) - 1] = '\0';

		if (compare_token(check, "overwrite") == 0 || compare_token(check, "yes") == 0) {
			// Open the file in write mode and saves file.
			file_ptr = fopen(file_path, "w");
			knowledge_write(file_ptr);

			snprintf(response, n, "I have written down my memories to '[%s]'.", file_path);

			fclose(file_ptr);

			return 0;
		}
		else {
			// If user says no to overwrite, don't save.
			snprintf(response, n, "My knowledge has not been saved.");

			return 0;
		}
	}
	else {
		// File does not already exist. Writes and saves file.
		file_ptr = fopen(file_path, "w");
		knowledge_write(file_ptr);
		snprintf(response, n, "I have written down my memories to '[%s]'.", file_path);
		fclose(file_ptr);

		return 0;
	}
}


/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char* intent) {

	const char* smalltalk[] = { "hi", "hello", "hey", "how are you", "how's everything", "how's it going", "are you human", "are you a robot", "what is your name", "who are you" , "tell me something" , "1","2","3", "time","what is the current time" , "what is the date today", };

	int size = sizeof(smalltalk) / sizeof(smalltalk[0]);

	for (int i = 0; i < size; i++)
	{
		if (compare_token(intent, smalltalk[i]) == 0)
		{
			return 1;
		}
	}

	return 0;

}


/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char* inv[], char* response, int n) {

	char intent[MAX_INPUT] = { 0 };
	strcat_array_of_strings(intent, inv, inc, MAX_INPUT - 1, 0);

	const char* random_hi[] = { "Hi!", "Hello!", "Hello there!", "Hey hey~", "What's Up!!" };
	const char* random_can[] = { "What do you think?", "Maybe I could!", "Well, it's either a yes or a no." };
	const char* greetings[] = { "Hi!", "Hello!", "Hey~", "Hey there!", "Greetings", "Welcome!", "What\'s up?", "Howdy!", "Hi-ya~" };
	const char* how_response[] = { "I'm good, thanks.", "Great!", "Great great, thanks.", "I'm doing well.", "Fine, thanks." };
	const char* how_question[] = { "And you?", "How are you?", "How's everything?", "How's it going?" };

	const char* joke[] = { "I can\'t believe I got fired from the calendar factory. All I did was take a day off!", "Never trust atoms, they make up everything.", "I was wondering why the frisbee kept getting bigger and bigger, but then it hit me.", "I just got kicked out of a secret cooking society. I spilled the beans." };
	const char* fun_fact[] = { "A shrimp\'s heart is in its head.", "Like fingerprints, everyone\'s tongue print is different.", "Almonds are a member of the peach family.", "A shark is the only known fish that can blink with both eyes." };
	const char* something_cool[] = { "McDonald\'s once made bubblegum-flavored broccoli.", "The first oranges weren\'t orange.", "Bananas grow upside-down.", "Movie trailers originally played after the movie" };

	const size_t greetings_count = sizeof(greetings) / sizeof(greetings[0]);
	const size_t how_response_count = sizeof(how_response) / sizeof(how_response[0]);
	const size_t how_question_count = sizeof(how_question) / sizeof(how_question[0]);

	const size_t joke_count = sizeof(joke) / sizeof(joke[0]);
	const size_t fun_fact_count = sizeof(fun_fact) / sizeof(fun_fact[0]);
	const size_t something_cool_count = sizeof(something_cool) / sizeof(something_cool[0]);

	char* random_greetings, * random_how_response, * random_how_question, * random_joke, * random_fun_fact, * random_something_cool;
	srand((unsigned int)time(NULL));

	size_t rand_int = (size_t)(rand() % 5);
	random_greetings = greetings[rand() % greetings_count];
	random_how_response = how_response[rand() % how_response_count];
	random_how_question = how_question[rand() % how_question_count];

	random_joke = joke[rand() % joke_count];
	random_fun_fact = fun_fact[rand() % fun_fact_count];
	random_something_cool = something_cool[rand() % something_cool_count];


	if (compare_token("hi", inv[0]) == 0 || compare_token("hello", inv[0]) == 0 || compare_token("hey", inv[0]) == 0)
	{
		snprintf(response, n, "%s", random_greetings);
	}
	else if (compare_token("how are you", intent) == 0 || compare_token("how\'s everything?", intent) == 0 || compare_token("how\'s it going?", intent) == 0)
	{
		snprintf(response, n, "%s\n%s: %s", random_how_response, chatbot_botname(), random_how_question);
	}
	else if (compare_token("are you human", intent) == 0 || compare_token("are you a robot", intent) == 0)
	{
		snprintf(response, n, "I am a robot. Are you a robot?");
	}
	else if (compare_token("what is your name", intent) == 0 || compare_token("who are you?", intent) == 0)
	{
		snprintf(response, n, "I am chatty the chatbot.");
	}
	else if (compare_token("tell me something", intent) == 0)
	{
		snprintf(response, n, "What do you want to know?\n[1] Tell me a joke\n[2] Tell me a fun fact\n[3] Tell me something cool");
	}
	else if (compare_token("1", intent) == 0) {

		snprintf(response, n, "%s", random_joke);
	}
	else if (compare_token("2", intent) == 0) {

		snprintf(response, n, "%s", random_fun_fact);
	}
	else if (compare_token("3", intent) == 0) {

		snprintf(response, n, "%s", random_something_cool);
	}
	else if (compare_token("time", intent) == 0)
	{
		time_t rawtime;
		struct tm* timeinfo;

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		char* months[] = { "January","February","March","April","May","June","July","August","September","November","December" };
		char ampm[3];
		int twelvehrformat = 0;
		if (timeinfo->tm_hour >= 12) {
			if (timeinfo->tm_hour == 12) {
				twelvehrformat = timeinfo->tm_hour;
			}
			else {
				twelvehrformat = timeinfo->tm_hour - 12;
			}
			strcpy(ampm, "PM");
		}
		else {
			strcpy(ampm, "AM");
		}
		snprintf(response, n, "It is %d %s %d, %02d:%02d:%02d %s", timeinfo->tm_mday, months[timeinfo->tm_mon - 1], timeinfo->tm_year + 1900, twelvehrformat, timeinfo->tm_min, timeinfo->tm_sec, ampm);
	}
	else {
		return 0;
	}

	return 0;

}
