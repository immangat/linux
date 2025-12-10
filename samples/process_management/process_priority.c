/*
 * Process Priority Example
 * =========================
 * 
 * This program demonstrates process scheduling and priorities:
 * - Nice values (-20 to 19)
 * - Scheduling policies
 * - CPU affinity
 * 
 * Compile: gcc -o process_priority process_priority.c
 * Run: ./process_priority
 * Note: Some operations require root privileges
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sched.h>
#include <errno.h>
#include <string.h>

/* CPU-intensive work loop iteration count */
#define WORK_ITERATIONS 100000000

void demonstrate_nice_values(void)
{
	int nice_val;
	pid_t pid;

	printf("=== Nice Values and Priority ===\n\n");
	printf("Nice values range from -20 (highest priority) to 19 (lowest)\n");
	printf("Default nice value is 0\n");
	printf("Only root can set negative nice values\n\n");

	/* Get current nice value */
	errno = 0;
	nice_val = nice(0);
	if (nice_val == -1 && errno != 0) {
		perror("nice");
	} else {
		printf("Current nice value: %d\n", nice_val);
	}

	/* Create child with different nice value */
	pid = fork();
	
	if (pid == 0) {
		/* Child process - try to be nicer (lower priority) */
		printf("\nChild process trying to increase nice value by 5\n");
		
		nice_val = nice(5);
		if (nice_val == -1 && errno != 0) {
			perror("Child: nice");
		} else {
			printf("Child: New nice value: %d\n", nice_val);
		}
		
		/* Do some work */
		printf("Child: Doing CPU-intensive work with lower priority\n");
		volatile long sum = 0;
		for (long i = 0; i < WORK_ITERATIONS; i++) {
			sum += i;
		}
		
		exit(0);
	} else {
		/* Parent process */
		printf("\nParent process keeping default nice value\n");
		printf("Parent: Nice value: %d\n", nice(0));
		
		/* Do some work */
		printf("Parent: Doing CPU-intensive work with normal priority\n");
		volatile long sum = 0;
		for (long i = 0; i < WORK_ITERATIONS; i++) {
			sum += i;
		}
		
		wait(NULL);
		printf("\nNice values affect how much CPU time a process gets\n");
		printf("Lower nice value = higher priority = more CPU time\n");
	}
}

void demonstrate_scheduling_policies(void)
{
	struct sched_param param;
	int policy;
	const char *policy_name;

	printf("\n\n=== Scheduling Policies ===\n\n");
	
	/* Get current scheduling policy */
	policy = sched_getscheduler(0);
	
	switch (policy) {
	case SCHED_OTHER:
		policy_name = "SCHED_OTHER (SCHED_NORMAL)";
		break;
	case SCHED_FIFO:
		policy_name = "SCHED_FIFO";
		break;
	case SCHED_RR:
		policy_name = "SCHED_RR";
		break;
#ifdef SCHED_BATCH
	case SCHED_BATCH:
		policy_name = "SCHED_BATCH";
		break;
#endif
#ifdef SCHED_IDLE
	case SCHED_IDLE:
		policy_name = "SCHED_IDLE";
		break;
#endif
	default:
		policy_name = "UNKNOWN";
	}
	
	printf("Current scheduling policy: %s\n", policy_name);
	
	/* Get priority limits for different policies */
	printf("\nScheduling Policy Priority Ranges:\n");
	printf("SCHED_FIFO: %d - %d\n",
	       sched_get_priority_min(SCHED_FIFO),
	       sched_get_priority_max(SCHED_FIFO));
	printf("SCHED_RR: %d - %d\n",
	       sched_get_priority_min(SCHED_RR),
	       sched_get_priority_max(SCHED_RR));
	printf("SCHED_OTHER: %d - %d (uses nice values instead)\n",
	       sched_get_priority_min(SCHED_OTHER),
	       sched_get_priority_max(SCHED_OTHER));

	/* Try to set real-time priority (requires root) */
	printf("\nTrying to set real-time scheduling (SCHED_FIFO)...\n");
	param.sched_priority = 10;
	
	if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
		printf("Failed: %s\n", strerror(errno));
		printf("(Real-time scheduling requires root privileges)\n");
	} else {
		printf("Success! Process now using SCHED_FIFO with priority 10\n");
		/* Reset to normal */
		param.sched_priority = 0;
		sched_setscheduler(0, SCHED_OTHER, &param);
	}

	printf("\nScheduling Policies Explained:\n");
	printf("- SCHED_OTHER: Default time-sharing (CFS scheduler)\n");
	printf("- SCHED_FIFO: Real-time first-in-first-out\n");
	printf("- SCHED_RR: Real-time round-robin\n");
#ifdef SCHED_BATCH
	printf("- SCHED_BATCH: For batch processing jobs\n");
#endif
#ifdef SCHED_IDLE
	printf("- SCHED_IDLE: For very low priority tasks\n");
#endif
}

void demonstrate_cpu_affinity(void)
{
	cpu_set_t cpuset;
	int cpu_count;

	printf("\n\n=== CPU Affinity ===\n\n");
	printf("CPU affinity determines which CPU cores a process can run on\n\n");

	/* Get number of CPUs */
	cpu_count = sysconf(_SC_NPROCESSORS_ONLN);
	printf("Number of online CPUs: %d\n", cpu_count);

	/* Get current CPU affinity */
	CPU_ZERO(&cpuset);
	if (sched_getaffinity(0, sizeof(cpuset), &cpuset) == 0) {
		printf("Process can run on CPUs: ");
		for (int i = 0; i < cpu_count; i++) {
			if (CPU_ISSET(i, &cpuset)) {
				printf("%d ", i);
			}
		}
		printf("\n");
	}

	/* Try to set affinity to CPU 0 only */
	if (cpu_count > 1) {
		printf("\nTrying to restrict process to CPU 0...\n");
		CPU_ZERO(&cpuset);
		CPU_SET(0, &cpuset);
		
		if (sched_setaffinity(0, sizeof(cpuset), &cpuset) == 0) {
			printf("Success! Process now restricted to CPU 0\n");
			
			/* Reset to all CPUs */
			CPU_ZERO(&cpuset);
			for (int i = 0; i < cpu_count; i++) {
				CPU_SET(i, &cpuset);
			}
			sched_setaffinity(0, sizeof(cpuset), &cpuset);
		} else {
			perror("sched_setaffinity");
		}
	}

	printf("\nCPU affinity is useful for:\n");
	printf("- Performance optimization (cache locality)\n");
	printf("- Real-time applications\n");
	printf("- NUMA systems\n");
}

int main(void)
{
	printf("=== Process Priority and Scheduling Example ===\n\n");
	printf("This example demonstrates how Linux manages process priorities\n");
	printf("and scheduling policies.\n\n");

	demonstrate_nice_values();
	demonstrate_scheduling_policies();
	demonstrate_cpu_affinity();

	printf("\n\n=== Summary ===\n");
	printf("Process scheduling determines which process runs and when:\n");
	printf("1. Nice values: -20 (high priority) to 19 (low priority)\n");
	printf("2. Scheduling policies: OTHER, FIFO, RR");
#ifdef SCHED_BATCH
	printf(", BATCH");
#endif
#ifdef SCHED_IDLE
	printf(", IDLE");
#endif
	printf("\n");
	printf("3. CPU affinity: Control which CPUs a process can use\n");
	printf("4. CFS (Completely Fair Scheduler) manages normal processes\n");
	printf("\nUse 'nice', 'renice', 'chrt', 'taskset' commands for control\n");

	return 0;
}
