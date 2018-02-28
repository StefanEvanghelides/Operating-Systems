#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "array/array.h"

#define TRUE 1

/* Spits a strings into its components and stores them into an Array. */
ArrayList getArrayOfFlags(Array command) {
	ArrayList flags;
	char *found;

	initArrayList(&flags);
	do {
		found = strsep(&(command.data), " ");
		Array foundArray;
		initArray(&foundArray);

		if(found != NULL) {
			for(int i=0; i<strlen(found); i++) {
				addElement(&foundArray, found[i]);
			}
		} else {
			foundArray.length++;
			foundArray.data = NULL;
		}

		if(foundArray.length > 0) addElementList(&flags, foundArray);
    } while (found != NULL);

    return flags;
}

/* Stores the input in an Array file. */
Array typePrompt() {
	Array command;

	initArray(&command);

	printf("$ "); // The funny shell char

	char c = getchar();
	while(c != '\n') {
		addElement(&command, c);
		c = getchar();
	}

	return command;
}

/* Checks for the terminating command. */
int quitProgram(Array command) {
	return strcmp(command.data, "quit") == 0;
}

/* Parses the input. */
ArrayList parseInput(Array input) {
	ArrayList tokens;
	initArrayList(&tokens);

	int i=0;
	while(i < input.length) {
		Array currentToken;
		initArray(&currentToken);

		while(i < input.length && input.data[i] == ' ') i++; //removes spaces
		
		while(i < input.length && input.data[i] != '&') {
			addElement(&currentToken, input.data[i]);
			i++;
		}
		addElementList(&tokens, currentToken);
		i++;
	}

	return tokens;
}

/* Runs the Shell program. */
void runShell() {
	int status, child;

	while(TRUE) {
		Array input = typePrompt();
		
		if(quitProgram(input)) { /* Check for the exit command. */
			break;
		}

		ArrayList tokens = parseInput(input);
		//printArrayList(tokens);

		for(int i=0; i<tokens.length; i++) {
			child = fork(); /* Forks child to execute command. */
			if(child < 0) {
				perror("Forking error. Abord!\n");
				exit(EXIT_FAILURE);
			} else if(child == 0) {
	  			ArrayList flags = getArrayOfFlags(tokens.data[i]);
				char **flagsData = getArrayListData(flags);
				execvp (flagsData[0], flagsData);
				
				// Should never reach here
				perror("execvp error!\n");
				exit(EXIT_FAILURE);
			} else {
				waitpid(child, &status, 0);
			}
		}
	}
}

int main(int argc, char** argv) {

	runShell();

	return EXIT_SUCCESS;
}