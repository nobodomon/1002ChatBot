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

#include <stdio.h>
#include <string.h>
#include "chat1002.h"

#include <time.h>
#include <stdlib.h>
#include <time.h>

extern node_t *head_what;
extern node_t *head_where;
extern node_t *head_who;
/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {

	return "Chatbot";

}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {

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
int chatbot_main(int inc, char *inv[], char *response, int n) {

	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
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
int chatbot_is_exit(const char *intent) {

	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;

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
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {

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
int chatbot_is_load(const char *intent) {
	return compare_token(intent, "load") == 0;
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
int chatbot_do_load(int inc, char *inv[], char *response, int n) {
	/*
		fp:		The file pointer.
		ctr:	The number of successful results retrieved from the file.
	*/
	FILE *fp;
	int ctr = 0;
	char file_path[MAX_INPUT];

	// Get the file path from the user input.
	if (compare_token(inv[1], "from") == 0) {
		// LOAD[0] from[1] /path/to/file[2]
		strcpy(file_path, inv[2]);
	} else {
		// LOAD[0] /path/to/file[1]
		strcpy(file_path, inv[1]);
	}

	// Open the file in read mode.
	fp = fopen(file_path, "r");

	if (fp != NULL) {
		// File exists.
		ctr = knowledge_read(fp);
		fclose(fp);
		snprintf(response, n, "I have loaded %d results from the knowledge base [%s].", ctr, file_path);
	} else {
		// File does not exist.
		snprintf(response, n, "Sorry, I can't load the knowledge base [%s].", file_path);
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
int chatbot_is_question(const char *intent) {

	return compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0;

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
int chatbot_do_question(int inc, char *inv[], char *response, int n) {

	/*
		unsure:		A string for unsure questions.
		enti:		A string to store the entity.
		ans:		A string to store the answer to the question.
	*/
	char unsure[MAX_RESPONSE] = "I don't know. ";
	char enti[MAX_ENTITY] = "";
	char ans[MAX_RESPONSE] = "";

	size_t offset = 1;

	// Check where does the question start.
	if (
		compare_token(inv[1], "is") == 0 ||
		compare_token(inv[1], "are") == 0
	) {
		offset = 2;
	}

	safe_strcat(enti, inv, inc, (MAX_ENTITY - 1), offset);

	if (knowledge_get(inv[0], enti, response, n) == KB_NOTFOUND) {
		safe_strcat(unsure, inv, inc, (MAX_RESPONSE - 1), 0);
		strcat(unsure, "?");

		prompt_user(ans, MAX_RESPONSE, "%s", unsure);
		knowledge_put(inv[0], enti, ans);
		snprintf(response, n, "Thank you.");
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
int chatbot_is_reset(const char *intent) {

	return compare_token(intent, "reset") == 0;

	return 0;

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
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {
	// Reseed the random number generator.
	srand((unsigned)(time(NULL)));
	// Reset the knowledge base in memory.
	knowledge_reset();
	snprintf(response, n, "I have reset my knowledge for this session.");
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
int chatbot_is_save(const char *intent) {

	return compare_token(intent, "save") == 0;

	return 0;

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
int chatbot_do_save(int inc, char *inv[], char *response, int n) {
	/*
		fp:		The file pointer.
	*/
	FILE *fp;
	char file_path[MAX_INPUT];

	// Get the file path from the user input.
	if (
		compare_token(inv[1], "to") == 0 ||
		compare_token(inv[1], "as") == 0
	) {
		// Save[0] to[1] /path/to/file[2]
		// Save[0] as[1] /path/to/file[2]
		strcpy(file_path, inv[2]);
	} else {
		// save[0] /path/to/file[1]
		strcpy(file_path, inv[1]);
	}

	// Open the file in write mode.
	fp = fopen(file_path, "w");

	if (fp != NULL) {
		// File exists.
		knowledge_write(fp);
		fclose(fp);
		snprintf(response, n, "I have saved the results from the knowledge base to [%s].", file_path);
	} else {
		// File does not exist.
		snprintf(response, n, "Sorry, I can't save the knowledge base to [%s].", file_path);
	}
	return 0;
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
int chatbot_is_smalltalk(const char *intent) {

	const char* smalltalk[] = { "hi", "hello", "hey", "how are you?", "how's everything?", "how's it going?", "are you human?", "are you a robot?", "what is your name?", "who are you?" , "tell me something" , "what day is it today?" , "what is the current time?" , "what is the date today?"};

	for (int i = 0; i < 14; i++) 
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
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n) {
	
	const char *random_hi[] = {"Hi!", "Hello!", "Hello there!", "Hey hey~", "What's Up!!"};
	const char *random_can[] = {"What do you think?", "Maybe I could!", "Well, it's either a yes or a no."};
	const char* greetings[] = {"Hi!", "Hello!", "Hey~", "Hey there!", "Greetings", "Welcome!", "What's up?", "Howdy!", "Hi-ya~"};
	const char* how_response[] = {"I'm good, thanks.", "Great!", "Great great, thanks.", "I'm doing well.", "Fine, thanks."};
	const char* how_question[] = {"And you?", "How are you?", "How's everything?", "How's it going?"};

	const char* joke[] = { "I can’t believe I got fired from the calendar factory. All I did was take a day off!", "Never trust atoms; they make up everything.", "I was wondering why the frisbee kept getting bigger and bigger, but then it hit me.", "I just got kicked out of a secret cooking society. I spilled the beans." };
	const char* fun_fact[] = { "A shrimp's heart is in its head.", "Like fingerprints, everyone's tongue print is different.", "Almonds are a member of the peach family.", "A shark is the only known fish that can blink with both eyes." };
	const char* something_cool[] = { "McDonald’s once made bubblegum-flavored broccoli.", "The first oranges weren’t orange.", "Bananas grow upside-down.", "Movie trailers originally played after the movie" };

	const size_t greetings_count = sizeof(greetings) / sizeof(greetings[0]);
	const size_t how_response_count = sizeof(how_response) / sizeof(how_response[0]);
	const size_t how_question_count = sizeof(how_question) / sizeof(how_question[0]);

	const size_t joke_count = sizeof(joke) / sizeof(joke[0]);
	const size_t fun_fact_count = sizeof(how_response) / sizeof(how_response[0]);
	const size_t something_cool_count = sizeof(how_question) / sizeof(how_question[0]);

	char *random_greetings, *random_how_response, *random_how_question, *random_joke, *random_fun_fact, *random_something_cool;
	srand(time(NULL));

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
	else if (compare_token("how are you?", inv[0]) == 0 || compare_token("how's everything?", inv[0]) == 0 || compare_token("how's it going?", inv[0]) == 0)
	{
		snprintf(response, n, "%s", random_how_response);
		snprintf(response, n, "%s", random_how_question);
	}
	else if (compare_token("are you human?", inv[0]) == 0 || compare_token("are you a robot?", inv[0]) == 0)
	{
		snprintf(response, n, "I am a robot. Are you a robot?");
	}
	else if (compare_token("what is your name?", inv[0]) == 0 || compare_token("who are you?", inv[0]) == 0)
	{
		snprintf(response, n, "I am chatty the chatbot.");
	}else if (compare_token("good", inv[0]) == 0) {
		// SMALLTALK: "Good day" feature.
		if (inc > 1) {
			snprintf(response, n, "Good %s to you too!", inv[1]);
		} else {
			snprintf(response, n, "Good day!");
		}
	} else if (
		compare_token("hello", inv[0]) == 0 ||
		compare_token("hey", inv[0]) == 0 ||
		compare_token("hi", inv[0]) == 0
	) {
		// SMALLTALK: "Hello" feature.
		snprintf(response, n, "%s" , random_hi[rand_int]);
	} else if (compare_token("can", inv[0]) == 0) {
		// SMALLTALK: "Can" feature.
		if (rand_int == 3) {
			if (compare_token(inv[1], "you") == 0) {
				snprintf(response, n, "I don't know, can I?");
			} else if (compare_token(inv[1], "i") == 0) {
				snprintf(response, n, "I don't know, can you?");
			} else {
				snprintf(response, n, "I don't know, can it?");
			}
		} else if (rand_int == 4) {
			char qns[MAX_INPUT] = "";
			safe_strcat(qns, inv, inc, (MAX_INPUT - 1), 0);
			snprintf(response, n, "Think about it, %s?", qns);
		} else {
			snprintf(response, n, "%s", random_can[rand_int]);
		}
	} else if (
		compare_token("it", inv[0]) == 0 ||
		compare_token("its", inv[0]) == 0 ||
		compare_token("it's", inv[0]) == 0
	) {
		// SMALLTALK: "It" feature.
		snprintf(response, n, "Indeed it is.");
	}
	else if (compare_token("tell me something", inv[0]) == 0)
	{

		int choice;

		printf("What do you want to know?\n");
		printf("[1] Tell me a joke\n");
		printf("[2] Tell me a fun fact\n");
		printf("[3] Tell me something cool\n");
		printf("[4] Exit\n");

		scanf_s("%d",&choice);

		while (choice != "1" && choice != "2" && choice != "3" && choice != "4")
		{
			printf("Invalid!\n");

			printf("What do you want to know?\n");
			printf("[1] Tell me a joke\n");
			printf("[2] Tell me a fun fact\n");
			printf("[3] Tell me something cool\n");
			printf("[4] Exit\n");

			scanf_s("%d",& choice);
		}

		if (choice == "1")
		{
			snprintf(response, n, "%s", random_joke);
		}
		else if (choice == "2")
		{
			snprintf(response, n, "%s", random_fun_fact);
		}
		else if (choice == "3")
		{
			snprintf(response, n, "%s", random_something_cool);
		}
		else if (choice == "4")
		{
			return 0;
		}
	}
	else if (compare_token("what day is it today?", inv[0]) == 0 || compare_token("what is the current time?", inv[0]) == 0 || compare_token("what is the date today?", inv[0]) == 0)
	{
		time_t rawtime;
		struct tm* timeinfo;

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		snprintf(response, n, "[%d %d %d %d:%d:%d]", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	}

	return 0;

}