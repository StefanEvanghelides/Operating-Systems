#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int 	parent = getpid(), 
			status = 0, 
			num_children, 
			next_process_id = 0,
			next_process_index = 0,
			current_process_index = 0,
			value = 0;
	
	/* Ensure the number of processes is within the required bounds */
	
	if(argc != 2 || atoi(argv[1]) < 2 || atoi(argv[1]) > 16){
		fprintf(stderr, "ERROR: Improper usage\n\nUSAGE: ./ring <number> where number is between 2 and 16.\n");
		exit(EXIT_FAILURE);
	}
	num_children = atoi(argv[1]);
	
	/* Initialise pipes and number of children. */
	int child[num_children];
	int pipefd[num_children][2];
	
	for(int i = 0;i < num_children;i++){
		if (pipe(pipefd[i]) == -1) {
			perror("pipe\n");
			exit(EXIT_FAILURE);
		}
	}
	
	/* Create processes, and start calculating and passing data */
	
	for(int i = 0;i<num_children;i++){
		child[i] = fork();
		
		if(child[i] < 0){
			perror("Fork failed: abort\n");
			exit(EXIT_FAILURE);
		}
		
		if(child[i] == 0){
			/* Here, we calculate the current process' index, and the index and id of the process it will pass to.
			 * We need the indexes so we know which pipes to read from and write to. */
			
			current_process_index 	= (getpid() % parent) - 1;
			next_process_id 		=  getpid() == (parent + num_children) ? parent + 1 : getpid() + 1; //If this is the last process, the next_process_id is set to the first process.
			next_process_index 		= (next_process_id % parent) - 1;
			
			/* Initialise data to first child process */
			
			if(getpid() == parent + 1){
				write(pipefd[current_process_index][1], &value, sizeof(int));
			}
			
			while(1){
				read(pipefd[current_process_index][0], &value, sizeof(int));
					
				// When the value is greater than 50, it will be passed to the other processes
				// which will trigger them to stop executing and exit successfully.
				if (value > 50){
					write(pipefd[next_process_index][1], &value, sizeof(int));
					exit(EXIT_SUCCESS);
				}
				
				printf("pid=%d: %d\n", getpid(), value);
				value += 1;
				
				write(pipefd[next_process_index][1], &value, sizeof(int));
			}
		}
	}
	
	for(int i=0; i<num_children; i++) {
		waitpid(child[i], &status, 0);
	}

	return EXIT_SUCCESS;
}
