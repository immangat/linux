/*
 * Process Basics Example
 * ======================
 * 
 * This program demonstrates fundamental process management concepts:
 * - Process creation using fork()
 * - Process IDs (PID)
 * - Parent-child relationship
 * - Return values from fork()
 * 
 * Compile: gcc -o process_basics process_basics.c
 * Run: ./process_basics
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	int status;

	printf("=== Process Basics Example ===\n\n");
	printf("Parent process starting...\n");
	printf("Parent PID: %d\n", getpid());
	printf("Parent's parent PID (PPID): %d\n\n", getppid());

	/*
	 * fork() creates a new process by duplicating the calling process.
	 * The new process is called the child process.
	 * 
	 * Return value:
	 * - 0 is returned to the child process
	 * - Child's PID is returned to the parent
	 * - -1 indicates error
	 */
	printf("Calling fork() to create child process...\n\n");
	pid = fork();

	if (pid < 0) {
		/* Fork failed */
		perror("fork failed");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		/* Child process */
		printf("[CHILD]  I am the child process\n");
		printf("[CHILD]  My PID: %d\n", getpid());
		printf("[CHILD]  My parent's PID: %d\n", getppid());
		printf("[CHILD]  fork() returned: %d\n", pid);
		printf("[CHILD]  Doing some work...\n");
		
		/* Simulate some work */
		sleep(2);
		
		printf("[CHILD]  Exiting with status 42\n");
		exit(42);
	} else {
		/* Parent process */
		printf("[PARENT] I am the parent process\n");
		printf("[PARENT] My PID: %d\n", getpid());
		printf("[PARENT] Child's PID: %d\n", pid);
		printf("[PARENT] fork() returned: %d\n", pid);
		printf("[PARENT] Waiting for child to finish...\n\n");
		
		/*
		 * wait() waits for child to terminate and retrieves
		 * the child's exit status
		 */
		wait(&status);
		
		if (WIFEXITED(status)) {
			printf("\n[PARENT] Child exited normally\n");
			printf("[PARENT] Child exit status: %d\n",
			       WEXITSTATUS(status));
		} else {
			printf("\n[PARENT] Child terminated abnormally\n");
		}
		
		printf("[PARENT] Parent exiting\n");
	}

	printf("\n=== Key Concepts ===\n");
	printf("1. fork() creates a copy of the calling process\n");
	printf("2. After fork(), both processes execute the same code\n");
	printf("3. Return value differs: 0 for child, child PID for parent\n");
	printf("4. Parent should wait() for child to avoid zombie processes\n");

	return 0;
}
