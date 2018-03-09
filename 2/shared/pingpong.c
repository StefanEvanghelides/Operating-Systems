#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/wait.h>

int pagesize,
	parent,
	child[2],
	parentMsg = 0,
	localCurrent = 0,
	pipefd[2][2];

void handler(int sig, siginfo_t *si, void *unused){
	mprotect(si->si_addr, pagesize, PROT_READ);
	kill(parent, SIGUSR1);
}

void sig1Handler(int sig, siginfo_t *si, void *unused){
	localCurrent = 1 - localCurrent;
	if(write(pipefd[localCurrent][1], &localCurrent, sizeof(int)) == -1){
		printf("WRITE ERROR\n");
	}
	kill(child[localCurrent],SIGUSR2);
	parentMsg++;
}

void sig2Handler(int sig, siginfo_t *si, void *unused){
	if(read(pipefd[1-localCurrent][0], &localCurrent, sizeof(int)) == -1){
		printf("READ ERROR\n");
	}
}

struct sigaction createSigAction(void (*handler)(int, siginfo_t *, void *), int sig){
	struct sigaction sa;
  
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	
	sigaction(sig, &sa, NULL);
	
	return sa;
}

void pingPongProc(int whoami, int *turnVar){
	int msg = 0;
	while(msg < 5){
		while(localCurrent != whoami);
		printf(whoami == 0 ? "Ping!\n" : "...Pong!\n");
		*turnVar = 1 - whoami;
		mprotect(turnVar, pagesize, PROT_NONE);
		localCurrent = *turnVar;
		mprotect(turnVar, pagesize, PROT_WRITE);
		msg++;
	}
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
	int  status = 0, *turnVar = 0;
	void *buffer;
	
	parent = getpid();
	
	struct sigaction sa = createSigAction(handler, SIGSEGV);	
	struct sigaction sa1 = createSigAction(sig1Handler, SIGUSR1);
	struct sigaction sa2 = createSigAction(sig2Handler, SIGUSR2);

	pagesize = sysconf(_SC_PAGE_SIZE);
	
	if (posix_memalign(&buffer, pagesize, pagesize) != 0) {
		fprintf(stderr, "Fatal error: aligned memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	
	turnVar = buffer;
	mprotect(turnVar, pagesize, PROT_WRITE);
	
	for(int i = 0;i<2;i++){
		if (pipe(pipefd[i]) == -1) {
			perror("pipe\n");
			exit(EXIT_FAILURE);
		}
	}
	
	for(int i = 0;i<2;i++){
		child[i] = fork();
		if(child[i] < 0){
			perror("Fork failed: abort\n");
			exit(EXIT_FAILURE);
		} else if(!child[i]){
			pingPongProc(i, turnVar);
		}
	}
	
	while(parentMsg < 10);
	
	waitpid(child[0], &status, 0);
	waitpid(child[1], &status, 0);
	
	free(buffer);
	
	exit(EXIT_SUCCESS);
}
