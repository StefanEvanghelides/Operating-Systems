/* duplicates.c
 * 
 * This program will print on the standard output all the pairs of
 * duplicate files. It makes use of Unix commands find() and diff().
 * The algorithm works in the following way:
 *
 *   - First, all the files from the current directory and subdirectory are
 *     generated using find() command and stored in a separate file ("list.out").
 *     For this, a child process was create which executed this command using execvp().
 *
 *   - Then, the contents of the file are saved in a char matrix, based on which
 *     the pairs will be processed.
 *
 *   - The diff() command will be used to compare the files. If the files are not different 
 *     (i.e. they are identical), the pair will be displayed. Each comparison is done
 *     using popen() command which forks a child, opens a pipe and sends the feedback
 *     (duplicate or different) to the parent.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


/* Removes the file from the computer. */
void removeFileIfExists(char *filename) {
	if(access(filename, F_OK)==0) {
		if(remove(filename) != 0) {
			perror("Could not remove existing file!");
			exit(EXIT_FAILURE);
		}
	}
}

/* Executes the UNIX command find() to show a list of all
 * files starting from the current directory, including
 * subdirectories. This list is then saved in specified filename. */
void generateList(char *filename) {
	char **flags = malloc(4 * sizeof(char*));
	flags[0] = "find";
	flags[1] = "-type";
	flags[2] = "f";
	flags[3] = NULL;

	/* If the file exists, remove it to create a new one
	 * with the specified permissions. */
	removeFileIfExists(filename);

	/* Create a new file to store the results. */
	int new_stdout = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0777);
	close(1);
	dup2(new_stdout, 1);

	/* Execute the find command and store the output in the file. */
	execvp(flags[0], flags);

	// Note, it should never reach this part
	perror("execvp failed in find(). Abort!\n");
	exit(EXIT_FAILURE);
}

/* Check if 2 files are duplicates. */
int isDuplicate(char *file1, char *file2) {
	char isDifferent='\0', command[1000];
	
	sprintf(command, "diff %s %s", file1, file2);

	FILE *fp = popen(command, "r");

	if (fp != NULL ) {
		// if isDifferent will be \0 after this call, then diff command
		// had no output, which means that the files are duplicates
        fread(&isDifferent,1,1,fp);

        pclose(fp);
    } else {
    	perror("Eror in popen(). Abort!\n");
    	exit(EXIT_FAILURE);
    }

    return !isDifferent;
}

/* Opens file filename. */
FILE *openFile(char *filename) {
	FILE *fp = fopen(filename, "r");
	if(fp == NULL) {
		perror("Unable to open file using fopen()!\n");
		exit(EXIT_FAILURE);
	}
	return fp;
}

/* reads a file and stores its content in a char matrix. */
char **readFile(FILE *fp, int *rows) {
	char c, **file;
	int i;

	// preallocate memory 1000x1000
	file = malloc(1000*sizeof(char*));
	for(i=0; i<1000; i++) {
		file[i] = malloc(1000*sizeof(char));
	}

	*rows = 0;
	while(1) {
		i=0;
		getc(fp); getc(fp); // remove ./ chars from the file
		while((c = getc(fp)) != '\n' && c != EOF) {
			file[*rows][i++] = c;
		}
		
		if(c != EOF) (*rows)++;
		else break;
	}

	return file;
}

/* Compare pairs of files and show duplicates.*/
void showDuplicates(char *file[], int rows) {
	for(int i=0; i<rows-1; i++) {
		for(int j=i+1; j<rows; j++) {
			if(isDuplicate(file[i], file[j])) {
				printf("%s and %s are the same file\n", file[i], file[j]);
			}
		}
	}
}

int main(int argc, char **argv){
	int status, rows;;
	char *listFile = "list.out";
	FILE *fp;
	char **fileContent;

	/* Forks child and generates the list of files. */
	int child = fork();
	if (child == 0) {
	 	generateList(listFile);
	 	exit(EXIT_SUCCESS);
	} else if(child < 0) {
		perror("Forking error!\n");
		exit(EXIT_FAILURE);
	}

	/* Wait for the list to be generated. */
	waitpid(child, &status, 0);

	/* Open the file created by the child processor. */
	fp = openFile(listFile);

	/* Reading file and storing it in matrix. */
	fileContent = readFile(fp, &rows);
	
	/* Compare files and show duplicates.*/
	showDuplicates(fileContent, rows);


	return 0;
}