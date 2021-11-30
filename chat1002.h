/*
 * ICT1002 (C Language) Group Project.
 *
 * This file contains the definitions and function prototypes for all of
 * features of the ICT1002 chatbot.
 */

#ifndef _CHAT1002_H
#define _CHAT1002_H
#define _CRT_SECURE_NO_WARNINGS // msvc workaround for calling unsafe functions, without breaking code portability

// simplify includes by having all in 1 header file
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

 /* the maximum number of characters we expect in a line of input (including the terminating null)  */
#define MAX_INPUT    256

/* the maximum number of characters allowed in the name of an intent (including the terminating null)  */
#define MAX_INTENT   32

/* the maximum number of characters allowed in the name of an entity (including the terminating null)  */
#define MAX_ENTITY   64

/* the maximum number of characters allowed in a response (including the terminating null) */
#define MAX_RESPONSE 256

/* return codes for knowledge_get() and knowledge_put() */
#define KB_OK     0
#define KB_NOTFOUND -1
#define KB_INVALID  -2
#define KB_NOMEM    -3

/* Intent-Entity-Response struct. */
typedef struct node {
	char entity[MAX_ENTITY];
	char response[MAX_RESPONSE];
	struct node* next; // Pointer to the next node in the linked list.
} entRespNode;

typedef entRespNode* node_ptr;

/* functions defined in main.c */
int compare_token(const char* token1, const char* token2);
void prompt_user(char* buf, int n, const char* format, ...);

/* functions defined in chatbot.c */
const char* chatbot_botname();
const char* chatbot_username();
int chatbot_main(int inc, char* inv[], char* response, int n);
int chatbot_is_exit(const char* intent);
int chatbot_do_exit(int inc, char* inv[], char* response, int n);
int chatbot_is_load(const char* intent);
int chatbot_do_load(int inc, char* inv[], char* response, int n);
int chatbot_is_question(const char* intent);
int chatbot_do_question(int inc, char* inv[], char* response, int n);
int chatbot_is_reset(const char* intent);
int chatbot_do_reset(int inc, char* inv[], char* response, int n);
int chatbot_is_save(const char* intent);
int chatbot_do_save(int inc, char* inv[], char* response, int n);
int chatbot_is_smalltalk(const char* intent);
int chatbot_do_smalltalk(int inc, char* inv[], char* resonse, int n);

/* functions defined in knowledge.c */
int knowledge_get(const char* intent, const char* entity, char* response, int n);
int knowledge_put(const char* intent, const char* entity, const char* response);
void knowledge_reset();
int knowledge_read(FILE* f);
void knowledge_write(FILE* f);

/* Functions defined in util.c */
int detectIntent(const char* intent, int braces);
void strcat_array_of_strings(char* dest, char* src[], size_t src_size, size_t n, int offset);
node_ptr create_node(const char* entity, const char* response);
void push_linkedList(node_ptr head, node_ptr node);
void free_linkedList(node_ptr node);
int knowledge_update(char* intent, node_ptr new_node);

/*
	Linked lists.
	Use "extern" to mark external variable.
*/
node_ptr what_header;
node_ptr where_header;
node_ptr who_header;

#endif
