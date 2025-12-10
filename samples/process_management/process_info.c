/*
 * Process Information Example
 * ============================
 * 
 * This program demonstrates how to get information about processes:
 * - Reading /proc filesystem
 * - Process IDs and relationships
 * - Memory usage
 * - CPU time
 * 
 * Compile: gcc -o process_info process_info.c
 * Run: ./process_info
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <string.h>

void print_process_ids(void)
{
	printf("=== Process Identification ===\n\n");
	
	printf("PID (Process ID): %d\n", getpid());
	printf("PPID (Parent Process ID): %d\n", getppid());
	printf("UID (User ID): %d\n", getuid());
	printf("EUID (Effective User ID): %d\n", geteuid());
	printf("GID (Group ID): %d\n", getgid());
	printf("EGID (Effective Group ID): %d\n", getegid());
	printf("PGID (Process Group ID): %d\n", getpgrp());
	printf("SID (Session ID): %d\n", getsid(0));
	
	printf("\nExplanation:\n");
	printf("- PID: Unique identifier for this process\n");
	printf("- PPID: PID of the parent process that created this one\n");
	printf("- UID/GID: User and group ownership\n");
	printf("- PGID: Process group (for job control)\n");
	printf("- SID: Session (group of process groups)\n");
}

void print_resource_usage(void)
{
	struct rusage usage;

	printf("\n\n=== Resource Usage ===\n\n");
	
	if (getrusage(RUSAGE_SELF, &usage) == 0) {
		printf("User CPU time: %ld.%06ld seconds\n",
		       usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
		printf("System CPU time: %ld.%06ld seconds\n",
		       usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
		printf("Maximum resident set size: %ld KB\n", usage.ru_maxrss);
		printf("Page faults (no I/O): %ld\n", usage.ru_minflt);
		printf("Page faults (I/O required): %ld\n", usage.ru_majflt);
		printf("Voluntary context switches: %ld\n", usage.ru_nvcsw);
		printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);
	}
	
	printf("\nExplanation:\n");
	printf("- User time: Time spent executing user code\n");
	printf("- System time: Time spent in kernel on behalf of process\n");
	printf("- RSS: Physical memory currently used\n");
	printf("- Page faults: Memory accesses requiring page loads\n");
	printf("- Context switches: Times process gave up CPU\n");
}

void print_proc_info(pid_t pid)
{
	char path[256];
	char line[256];
	FILE *file;

	printf("\n\n=== Information from /proc ===\n\n");
	printf("The /proc filesystem provides detailed process information\n\n");

	/* Read process status */
	snprintf(path, sizeof(path), "/proc/%d/status", pid);
	file = fopen(path, "r");
	if (file) {
		printf("Key information from /proc/%d/status:\n", pid);
		while (fgets(line, sizeof(line), file)) {
			if (strncmp(line, "Name:", 5) == 0 ||
			    strncmp(line, "State:", 6) == 0 ||
			    strncmp(line, "Pid:", 4) == 0 ||
			    strncmp(line, "PPid:", 5) == 0 ||
			    strncmp(line, "Threads:", 8) == 0 ||
			    strncmp(line, "VmSize:", 7) == 0 ||
			    strncmp(line, "VmRSS:", 6) == 0) {
				printf("  %s", line);
			}
		}
		fclose(file);
	}

	/* Read command line */
	snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
	file = fopen(path, "r");
	if (file) {
		printf("\nCommand line: ");
		int c;
		while ((c = fgetc(file)) != EOF) {
			if (c == '\0') {
				putchar(' ');
			} else {
				putchar(c);
			}
		}
		printf("\n");
		fclose(file);
	}

	/* Show some /proc files */
	printf("\nUseful /proc files for process %d:\n", pid);
	printf("  /proc/%d/status    - Process status and memory info\n", pid);
	printf("  /proc/%d/stat      - Process statistics\n", pid);
	printf("  /proc/%d/cmdline   - Command line arguments\n", pid);
	printf("  /proc/%d/environ   - Environment variables\n", pid);
	printf("  /proc/%d/maps      - Memory mappings\n", pid);
	printf("  /proc/%d/fd/       - Open file descriptors\n", pid);
	printf("  /proc/%d/task/     - Threads in this process\n", pid);
}

void demonstrate_limits(void)
{
	struct rlimit limit;

	printf("\n\n=== Resource Limits ===\n\n");
	
	/* Check various resource limits */
	if (getrlimit(RLIMIT_CPU, &limit) == 0) {
		printf("CPU time limit:\n");
		/* RLIM_INFINITY means no limit */
		printf("  Soft: %ld seconds\n", limit.rlim_cur == RLIM_INFINITY ? -1 : (long)limit.rlim_cur);
		printf("  Hard: %ld seconds\n", limit.rlim_max == RLIM_INFINITY ? -1 : (long)limit.rlim_max);
	}

	if (getrlimit(RLIMIT_AS, &limit) == 0) {
		printf("\nAddress space (virtual memory) limit:\n");
		printf("  Soft: %ld bytes\n", limit.rlim_cur == RLIM_INFINITY ? -1 : (long)limit.rlim_cur);
		printf("  Hard: %ld bytes\n", limit.rlim_max == RLIM_INFINITY ? -1 : (long)limit.rlim_max);
	}

	if (getrlimit(RLIMIT_NOFILE, &limit) == 0) {
		printf("\nOpen files limit:\n");
		printf("  Soft: %ld\n", (long)limit.rlim_cur);
		printf("  Hard: %ld\n", (long)limit.rlim_max);
	}

	if (getrlimit(RLIMIT_NPROC, &limit) == 0) {
		printf("\nNumber of processes limit:\n");
		printf("  Soft: %ld\n", (long)limit.rlim_cur);
		printf("  Hard: %ld\n", (long)limit.rlim_max);
	}

	printf("\nExplanation:\n");
	printf("- Soft limit: Can be changed by process up to hard limit\n");
	printf("- Hard limit: Maximum value (only root can increase)\n");
	printf("- -1 or RLIM_INFINITY: No limit\n");
	printf("\nUse 'ulimit' command to view/set limits\n");
}

int main(void)
{
	printf("=== Process Information Example ===\n\n");
	printf("This example shows how to retrieve information about processes\n\n");

	print_process_ids();
	print_resource_usage();
	print_proc_info(getpid());
	demonstrate_limits();

	printf("\n\n=== Summary ===\n");
	printf("Process information can be obtained from:\n");
	printf("1. System calls: getpid(), getrusage(), getrlimit(), etc.\n");
	printf("2. /proc filesystem: /proc/[pid]/* files\n");
	printf("3. Tools: ps, top, htop, /proc/[pid]/status\n");
	printf("\nThe kernel maintains extensive information about each process\n");
	printf("in the task_struct structure (see include/linux/sched.h)\n");

	return 0;
}
