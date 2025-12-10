/*
 * Process States Example
 * =======================
 * 
 * This program demonstrates different process states in Linux:
 * - Running (R)
 * - Sleeping/Interruptible (S)
 * - Zombie (Z)
 * - Stopped (T)
 * 
 * Compile: gcc -o process_states process_states.c
 * Run: ./process_states
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void print_process_state(pid_t pid, const char *description)
{
	char cmd[256];
	int ret;
	
	printf("\n%s\n", description);
	printf("Check process state with: ps -p %d -o pid,stat,comm\n", pid);
	
	/* Use ps command to show process state */
	/* Note: pid is a kernel-provided value, not user input, so it's safe */
	snprintf(cmd, sizeof(cmd), "ps -p %d -o pid,stat,comm 2>/dev/null || true", pid);
	ret = system(cmd);
	(void)ret;  /* Suppress unused result warning */
}

void demonstrate_running_state(void)
{
	printf("\n=== RUNNING STATE (R) ===\n");
	printf("A process is in running state when:\n");
	printf("- It is currently executing on CPU, OR\n");
	printf("- It is ready to run (in the run queue)\n\n");
	
	printf("Current process PID: %d\n", getpid());
	print_process_state(getpid(), "This process is in RUNNING state:");
	
	printf("\nNote: The STAT column shows process state\n");
	printf("R = Running or runnable (on run queue)\n");
	printf("S = Interruptible sleep (waiting for event)\n");
	printf("D = Uninterruptible sleep (usually I/O)\n");
	printf("T = Stopped (by signal)\n");
	printf("Z = Zombie (terminated but not reaped)\n");
}

void demonstrate_sleeping_state(void)
{
	pid_t pid;

	printf("\n\n=== SLEEPING STATE (S) ===\n");
	printf("A process enters sleeping state when waiting for:\n");
	printf("- I/O operations\n");
	printf("- Signals\n");
	printf("- Timer events\n\n");

	pid = fork();
	
	if (pid == 0) {
		/* Child: sleep to demonstrate sleeping state */
		printf("Child PID: %d will sleep for 30 seconds\n", getpid());
		printf("Quickly check state with: ps -p %d -o pid,stat,comm\n",
		       getpid());
		sleep(30);
		exit(0);
	} else {
		/* Parent */
		sleep(1);  /* Give child time to enter sleep */
		print_process_state(pid, "Child process in SLEEPING state:");
		
		/* Clean up child */
		kill(pid, SIGTERM);
		wait(NULL);
	}
}

void demonstrate_zombie_state(void)
{
	pid_t pid;

	printf("\n\n=== ZOMBIE STATE (Z) ===\n");
	printf("A process becomes a zombie when:\n");
	printf("- It has terminated (exited)\n");
	printf("- But parent hasn't called wait() yet\n");
	printf("- It still has an entry in the process table\n\n");

	pid = fork();
	
	if (pid == 0) {
		/* Child: exit immediately to become zombie */
		printf("Child PID: %d exiting to become zombie\n", getpid());
		exit(0);
	} else {
		/* Parent: don't wait immediately, so child becomes zombie */
		sleep(1);  /* Give child time to exit */
		print_process_state(pid, "Child process in ZOMBIE state:");
		
		printf("\nZombie processes don't consume resources except PID.\n");
		printf("They disappear when parent calls wait().\n");
		
		/* Now reap the zombie */
		wait(NULL);
		printf("\nAfter wait(), zombie is reaped and removed.\n");
	}
}

void demonstrate_stopped_state(void)
{
	pid_t pid;

	printf("\n\n=== STOPPED STATE (T) ===\n");
	printf("A process can be stopped by:\n");
	printf("- SIGSTOP signal\n");
	printf("- SIGTSTP signal (Ctrl+Z in shell)\n");
	printf("- Being traced by debugger (ptrace)\n\n");

	pid = fork();
	
	if (pid == 0) {
		/* Child: loop forever until stopped */
		printf("Child PID: %d running\n", getpid());
		while (1) {
			sleep(1);
		}
	} else {
		/* Parent: send SIGSTOP to child */
		sleep(1);
		printf("Sending SIGSTOP to child PID %d\n", pid);
		kill(pid, SIGSTOP);
		
		sleep(1);
		print_process_state(pid, "Child process in STOPPED state:");
		
		printf("\nTo continue a stopped process, send SIGCONT\n");
		
		/* Clean up */
		kill(pid, SIGKILL);
		wait(NULL);
	}
}

int main(void)
{
	printf("=== Process States Example ===\n");
	printf("\nThis example demonstrates different process states.\n");
	printf("Each state is a fundamental concept in process management.\n");

	demonstrate_running_state();
	demonstrate_sleeping_state();
	demonstrate_zombie_state();
	demonstrate_stopped_state();

	printf("\n\n=== Summary ===\n");
	printf("Process states represent what a process is doing:\n");
	printf("- R: Running or ready to run\n");
	printf("- S: Sleeping (interruptible)\n");
	printf("- D: Sleeping (uninterruptible, usually I/O)\n");
	printf("- T: Stopped by signal\n");
	printf("- Z: Zombie (terminated, awaiting parent)\n");
	printf("\nUse 'ps', 'top', or '/proc/[pid]/status' to check states\n");

	return 0;
}
