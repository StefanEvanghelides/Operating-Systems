#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "array/array.h"

#define TRUE 1

/* Spits a strings into its components and stores them into an Array. */
Array2D getArrayOfFlags(Array command) {
	Array2D array2d;
	char *found;

	initArray2D(&array2d,1,1);
	do {
		found = strsep(&(command.data), " ");
		if(found == NULL || strcmp(found, "")) addElement2D(&array2d, found);
    } while (found != NULL);

    return array2d;
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
		printArrayList(&tokens);

		for(int i=0; i<tokens.length; i++) {
			child = fork(); /* Forks child to execute command. */
			if(child < 0) {
				perror("Forking error. Abord!\n");
				exit(EXIT_FAILURE);
			} else if(child == 0) {
	  			Array2D flags = getArrayOfFlags(tokens.data[i]);
				execvp (flags.data[0], flags.data);

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