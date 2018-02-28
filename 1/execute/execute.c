/* execute.c
 * 
 * Programs takes a command as argument, forks a child process and
 * executes the command using execvp() system call.
 * 
 * NOTE: The command must be passed as a single argument:
 *   ex: ./execute "Command with flag1 flag2 flag3..."
 *
 * This command is then split into multiple parts and stored in an array,
 * which will be passed in to execvp() as parameters.
 * (see "array" package)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "array/array.h"


/* Spits a strings into its components and stores them into an Array. */
Array getArrayOfFlags(char** argv) {
	Array array;
	char *found;

	initArray(&array,1);

	do {
		found = strsep(&argv[1], " ");
		addElement(&array, found);
    } while (found != NULL);

    return array;
}

int main (int argc, char *argv[]) {
  int child, status;

  // FORKING
  child = fork();
  if (child < 0) {
    fprintf (stderr, "Fork failed: abort\n");
    return EXIT_FAILURE;
  }

  // EXECUTING
  if(child == 0) {
  	Array flags = getArrayOfFlags(argv);
	execvp (flags.data[0], flags.data);

	// ERROR
	printf ("execute: command %s not found\n", flags.data[0]);
	exit(99);
  }

  waitpid(child, &status, 0);

  return EXIT_SUCCESS;
}
