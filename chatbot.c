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

#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <time.h>

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

	/* to be implemented */

	return 0;

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

	/* to be implemented */

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

	/* to be implemented */

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

	/* to be implemented */

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

	/* to be implemented */

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

	std::vector <string> greetings_vector{ "Hi!", "Hello!", "Hey~", "Hey there!", "Greetings", "Welcome!", "What's up?", "Howdy!", "Hi-ya~"};
	std::vector <string> how_response_vector{ "I'm good, thanks.", "Great!", "Great great, thanks.", "I'm doing well.", "Fine, thanks."};
	std::vector <string> how_question_vector{ "And you?", "How are you?", "How's everything?", "How's it going?"};

	string random_greetings = greetings_vector[rand() % greetings_vector.size()];
	string random_how_response = how_response_vector[rand() % how_response_vector.size()];
	string random_how_question = how_question_vector[rand() % how_question_vector.size()];

	std::vector <string> joke_vector{ "I can’t believe I got fired from the calendar factory. All I did was take a day off!", "Never trust atoms; they make up everything.", "I was wondering why the frisbee kept getting bigger and bigger, but then it hit me.", "I just got kicked out of a secret cooking society. I spilled the beans." };
	std::vector <string> fun_fact_vector{ "A shrimp's heart is in its head.", "Like fingerprints, everyone's tongue print is different.", "Almonds are a member of the peach family.", "A shark is the only known fish that can blink with both eyes." };
	std::vector <string> something_cool_vector{ "McDonald’s once made bubblegum-flavored broccoli.", "The first oranges weren’t orange.", "Bananas grow upside-down.", "Movie trailers originally played after the movie" };
	
	string random_joke = joke_vector[rand() % joke_vector.size()];
	string random_fun_fact = fun_fact_vector[rand() % fun_fact_vector.size()];
	string random_something_cool = something_cool_vector[rand() % something_cool_vector.size()];

	const std::string currentDateTime()
	{
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		strftime(buf, sizeof(buf), "Today's Date:%Y-%m-%d Current Time:%X", &tstruct);

		return buf;
	}

	if (compare_token("hi", inv[0]) == 0 || compare_token("hello", inv[0]) == 0 || compare_token("hey", inv[0]) == 0) 
	{
		std::cout << random_greetings << "\n";
	}
	else if (compare_token("how are you?", inv[0]) == 0 || compare_token("how's everything?", inv[0]) == 0 || compare_token("how's it going?", inv[0]) == 0)
	{
		std::cout << random_how_response << "\n";
		std::cout << random_how_question << "\n";
	}
	else if (compare_token("are you human?", inv[0]) == 0 || compare_token("are you a robot?", inv[0]) == 0)
	{
		snprintf(response, n, "I am a robot. Are you a robot?");
	}
	else if (compare_token("what is your name?", inv[0]) == 0 || compare_token("who are you?", inv[0]) == 0)
	{
		snprintf(response, n, "I am chatty the chatbot.");
	}
	else if (compare_token("tell me something", inv[0]) == 0)
	{
		std::cout << "What do you want to know?\n";
		std::cout << "[1] Tell me a joke\n"
			<< "[2] Tell me a fun fact\n"
			<< "[3] Tell me something cool\n"
			<< "[4] Exit\n";

		std::string input;
		std::getline(std::cin, input);

		while (input != "1" && input != "2" && input != "3" && input != "4") {
			std::cout << "Invalid!\n\n";

			std::cout << "What do you want to know?\n";
			std::cout << "[1] Tell me a joke\n"
				<< "[2] Tell me a fun fact\n"
				<< "[3] Tell me something cool\n"
				<< "[4] Exit\n";
			std::getline(std::cin, input);
		}

		if (input == "1") 
		{
			std::cout << random_joke << "\n";
		}
		else if (input == "2") 
		{
			std::cout << random_fun_fact << "\n";
		}
		else if (input == "3")
		{
			std::cout << random_something_cool << "\n";
		}
		else if (input == "4")
		{
			return 0;
		}
	}
	else if (compare_token("what day is it today?", inv[0]) == 0 || compare_token("what is the current time?", inv[0]) == 0 || compare_token("what is the date today?", inv[0]) == 0)
	{
		std::cout << currentDateTime() << std::endl;
	}

	return 0;

}
