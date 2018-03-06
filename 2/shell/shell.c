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
char **getFlags(List *command) {
	int maxSize = 2;
	char **flags = malloc(maxSize * sizeof(char*));
	assert(flags!= NULL);

	int i=0;
	while((*command) != NULL && !isShellSymbol((*command)->data[0])) {
		if(i == maxSize-1) {
			maxSize *= 2;
			flags = realloc(flags, maxSize * sizeof(char*));
			assert(flags != NULL);
		}
		flags[i] = malloc((strlen((*command)->data) + 1) * sizeof(char));
		memcpy(flags[i], (*command)->data, strlen((*command)->data) + 1);
		*command = (*command)->next;
		i++;
	}
	flags[i] = NULL;

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
		if(i == maxSize - 1) {
			maxSize *= 2;
			input = realloc(input, maxSize * sizeof(char));
			assert(input != NULL);
		}
		input[i] = c;
		i++;
		c = getchar();
	}
	input[i] = '\0';

	return input;
}

/* Checks for the terminating command. */
int quitProgram(char *command) {
	return strcmp(command, "quit") == 0;
}

void execBackgroundProcess(List *tokens) {
	int status;
	char **flags = getFlags(tokens);

	int child = fork(); /* Forks child to execute command. */

	if(child < 0) {
		perror("Forking error. Abord!\n");
		exit(EXIT_FAILURE);
	} else if(child == 0) {	
		execvp (flags[0], flags);			

		// Should never reach here
		perror("execvp error!\n");
		exit(EXIT_FAILURE);
	} else {
		waitpid(child, &status, 0);
	}
}

/* Runs the Shell program. */
void runShell() {
	while(TRUE) {
		printf("$ ");	
		char *input = readInput();

		if(quitProgram(input)) { /* Check for the exit command. */
			free(input);
			break;
		}

		List tokens = parseInput(input);
		printList(tokens);
		free(input);

		List tokensCopy = tokens;

		if(acceptInput(&tokensCopy)) {
			printf("\n -- Input can be executed! --\n\n");
		} else {
			printf("\n ERROR: cannot execute the input!\n\n");
			freeList(tokens);
			continue;
		}

		/* Exec processes. */
		execBackgroundProcess(&tokens);
		while (tokens != NULL){ 
			if(acceptSymbol(&tokens, "&")) {
				execBackgroundProcess(&tokens);
			} else break;
		}

		freeList(tokens);
	}
}

int main(int argc, char** argv) {

	runShell();

	return EXIT_SUCCESS;
}