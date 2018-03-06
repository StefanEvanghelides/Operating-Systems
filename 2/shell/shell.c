#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "list/list.h"
#include "parser/parser.h"

#define TRUE 1

/* Returns the list of flags for the command
 * to be executed. */
char **getFlags(List command) {
	int maxSize = 2;
	char **flags = malloc(maxSize * sizeof(char*));
	assert(flags!= NULL);

	int i=0;
	while(command != NULL) {
		if(i == maxSize-1) {
			maxSize *= 2;
			flags = realloc(flags, maxSize * sizeof(char*));
			assert(flags != NULL);
		}
		flags[i] = command->data;
		command = command->next;
		i++;
	}
	flags[i] = NULL;

	for(int k=0; k<=i; k++) {
		printf("%s (%d) \n", flags[k], flags[k][0]);
	}

	return flags;
}

/* Reads the input and saves it into a string. */
char *readInput() {
	int maxSize = 2;
	char *input = malloc(maxSize * sizeof(char));
	assert(input != NULL);

	char c = getchar();
	int i=0;
	while(c != '\n') {
		if(i == maxSize) {
			maxSize *= 2;
			input = realloc(input, maxSize * sizeof(char));
			assert(input != NULL);
		}
		input[i] = c;
		i++;
		c = getchar();
	}

	return input;
}

/* Checks for the terminating command. */
int quitProgram(char *command) {
	return strcmp(command, "quit") == 0;
}

/* Runs the Shell program. */
void runShell() {
	int status, child;
	char *input;
	List tokens;

	while(TRUE) {
		printf("$ ");	
		input = readInput();

		if(quitProgram(input)) { /* Check for the exit command. */
			free(input);
			break;
		}

		tokens = parseInput(input);
		printList(tokens);

		List tokensCopy = tokens;

		if(acceptInput(&tokensCopy) && tokensCopy != NULL) {
			printf("\n -- Input can be executed! --\n\n");
		} else {
			perror("\n ERROR: cannot execute the input!\n\n");
			continue;
		}

		while(tokens != NULL) {
			child = fork(); /* Forks child to execute command. */
			
			if(child < 0) {
				perror("Forking error. Abord!\n");
				exit(EXIT_FAILURE);
			} else if(child == 0) {	
			  	char **flags = getFlags(tokens);
				execvp (flags[0], flags);			

				// Should never reach here
				perror("execvp error!\n");
				exit(EXIT_FAILURE);
			} else {
				waitpid(child, &status, 0);
			}

			tokens = tokens->next;
		}


		freeList(tokens);
		free(input);
	}
}

int main(int argc, char** argv) {

	runShell();

	return EXIT_SUCCESS;
}