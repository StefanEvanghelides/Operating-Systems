#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../list/list.h"
#include "parser.h"

/* Checks if the character c is a symbol from the range
   {& , | , < , >}. */
int isShellSymbol(char c) {
  return (c == '&' || c == '|' || c == '<' || c == '>');
}

/* Parses the input string and returs a list of tokens. */
List parseInput(char *input) {
  List tokens = NULL;
  List lastNode = NULL;
  List currentNode = NULL;

  int length = strlen(input);
  int i=0;

  while(i < length) {
    while(i < length && isspace(input[i])) i++; // remove spaces 
    
    if(i == length) break;

    currentNode = newNode(input, &i);

    if (lastNode == NULL) { /* there is no list yet */
      tokens = currentNode;
    } else {  /* there is already a list; add node at the end */
      lastNode->next = currentNode;
    }
    lastNode = currentNode;
  }

  return tokens;
}

int acceptSymbol(List *tl, char *c) {
	if(*tl != NULL && (*tl)->type == Symbol && strcmp((*tl)->data, c) == 0) {
		*tl = (*tl)->next;
		return 1;
	}
	return 0;
}

int acceptString(List *tl) {
	if(*tl != NULL && (*tl)->type == String) {
		*tl = (*tl)->next;
		return 1;
	}
	return 0;
}

int acceptPipe(List *tl) {
	if(!acceptString(tl)) return 0;

	if(acceptSymbol(tl, "<")) {
		if(!acceptString(tl)) return 0;
	}
	
	if(acceptSymbol(tl, ">")) {
		if(!acceptString(tl)) return 0;
	}

	return 1;
}

int acceptBackgroundProcess(List *tl) {
	if(!acceptPipe(tl)) return 0;

	while(acceptSymbol(tl, "|")) {
		if(!acceptPipe(tl)) return 0;
	}

	return 1;
}

int acceptInput(List *tl) {
	if(!acceptBackgroundProcess(tl)) return 0;

	while(acceptSymbol(tl, "&")) {
		if(!acceptBackgroundProcess(tl)) return 0;
	}

	return 1;
}