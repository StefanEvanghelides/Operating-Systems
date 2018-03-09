#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "list/list.h"
#include "parser/parser.h"

#define TRUE 1

void free2DArray(char **array, int row) {
	if(array != NULL) {
		for(int i=0; i<row; i++) {
		free(array[i]);
		}
		free(array);
	}
}

/* Returns the list of flags for the command
 * to be executed. */
char **getStrings(List *command, int *maxSize) {
	*maxSize = 2;
	char **flags = malloc((*maxSize) * sizeof(char*));
	assert(flags!= NULL);

	int i=0;
	while((*command) != NULL && !isShellSymbol((*command)->data[0])) {
		if(i == (*maxSize)-1) {
			(*maxSize) *= 2;
			flags = realloc(flags, (*maxSize) * sizeof(char*));
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

/* Close pipes. */
void closePipes(int prevFD[2], int nextFD[2]) {
	close(prevFD[0]);
	close(prevFD[1]);
	close(nextFD[0]);
	close(nextFD[1]);
}

/* Executes the command. */
void execCommand(List *tokens, int background) {
	int status;
	int new_stdin, new_stdout;
	int rowsFlags=0, rowsInFile=0, rowsOutFile=0;
	char **flags=NULL, **inFile=NULL, **outFile=NULL;

 	// Get the command and the flags
	flags = getStrings(tokens, &rowsFlags);

	// Get the files to redirect input/output
	if(acceptSymbol(tokens, "<")) {
		inFile = getStrings(tokens, &rowsInFile);
	}
	if(acceptSymbol(tokens, ">")) {
		outFile = getStrings(tokens, &rowsOutFile);
	}

	int child = fork();

	if(child < 0) {
		free2DArray(flags, rowsFlags);
		free2DArray(inFile, rowsInFile);
		free2DArray(outFile, rowsOutFile);
		perror("Forking error. Abord!\n");
		exit(EXIT_FAILURE);

	} else if(child == 0) {
		// check if the process should run in background
		if(background == 1) {
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
			int x = open("/dev/null", O_RDWR);
			dup(x);
			dup(x);
		}

		// checks "<" redirect
		if(inFile != NULL && inFile[0] != NULL) {
			close(0);
			printf("inFile[0] = %s\n", inFile[0]);
			new_stdin = open(inFile[0], O_RDONLY);

			if(new_stdin < 0) { //error opening the file
				dup2(new_stdin, 0);
				fprintf(stdout, "Error: could not open file <%s>\n", inFile[0]);
			}

		}

		// checks ">" redirect
		if(outFile != NULL && outFile[0] != NULL) {
			close(1);
			printf("outFile[0] = %s\n", outFile[0]);
			new_stdout = open(outFile[0], O_CREAT|O_WRONLY, 0777);
			

			if(new_stdout < 0) { //error opening the file
				dup2(new_stdout, 1);
				fprintf(stdout, "Error: could not create or write file <%s>\n", outFile[0]);
			}
		}

		// Executes the command
		execvp (flags[0], flags);			

		// Should never reach here. Free memory and show error
		free2DArray(flags, rowsFlags);
		free2DArray(inFile, rowsInFile);
		free2DArray(outFile, rowsOutFile);
		perror("execvp error!\n");
		exit(EXIT_FAILURE);
	} else { // waits only for processes in foreground
		if(!background) waitpid(child, &status, 0);
	}
	free2DArray(flags, rowsFlags);
	free2DArray(inFile, rowsInFile);
	free2DArray(outFile, rowsOutFile);
}

/* Executes the next command from the sequence. */
void execNextCommand(List *tokens) {
	execCommand(tokens, 0);
	while(tokens != NULL) {
		if(acceptSymbol(tokens, "|")) {
			// NOTE: this is not implemented properly
	 		execCommand(tokens, 0);
	 	} else if(acceptSymbol(tokens, "&")) {
	 		execCommand(tokens, 1);
	 	} else break;
	}
}

/* Run the commands sequentially. */
void runCommands(List *tokens) {
	execNextCommand(tokens);
	while (tokens != NULL){ 
		if(acceptSymbol(tokens, ";")) {
			execNextCommand(tokens);
		} else break;
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

		/* Verify the validity of the input. */
		List tokensCopy = tokens; // Copy of the pointer
		if(acceptInput(&tokensCopy)) {
			printf("\n -- Input can be executed! --\n\n");
		} else {
			printf("\n ERROR: cannot execute the input!\n\n");
			freeList(tokens);
			continue;
		}

		/* Exec commands. */
		tokensCopy = tokens; // Use the copy again
		runCommands(&tokensCopy);

		/* Free memory. */
		free(input);
		freeList(tokens);
	}
}

int main(int argc, char** argv) {

	runShell();

	return EXIT_SUCCESS;
}