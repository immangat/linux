=========================
Process Management Guide
=========================

Overview
========

This directory contains educational examples and documentation to help you
understand process management in the Linux kernel. Process management is one
of the core responsibilities of the kernel, involving creating, scheduling,
and terminating processes.

What is a Process?
==================

A process is a running instance of a program. Each process has:

* **Process ID (PID)**: A unique identifier
* **Process State**: Running, sleeping, stopped, or zombie
* **Memory Space**: Code, data, stack, and heap
* **File Descriptors**: Open files and resources
* **Scheduling Information**: Priority and CPU time
* **Parent-Child Relationship**: Every process has a parent (except init)

Process States
==============

Linux processes can be in one of several states:

1. **TASK_RUNNING (R)**: Process is running or ready to run
2. **TASK_INTERRUPTIBLE (S)**: Sleeping, waiting for an event
3. **TASK_UNINTERRUPTIBLE (D)**: Sleeping, cannot be interrupted
4. **TASK_STOPPED (T)**: Stopped by a signal
5. **TASK_ZOMBIE (Z)**: Terminated but parent hasn't read exit status

Process Creation
=================

In Linux, new processes are created using the ``fork()`` system call:

1. Parent process calls ``fork()``
2. Kernel creates a copy of the parent process
3. Child process gets a new PID
4. Both parent and child continue execution after fork
5. ``fork()`` returns 0 to child, child's PID to parent

The ``exec()`` family of system calls replaces a process's memory with a
new program, which is how shells run commands (fork + exec).

Process Scheduling
==================

The Linux kernel uses the Completely Fair Scheduler (CFS) for normal processes:

* **Time Slices**: CPU time is divided among processes
* **Priority**: Higher priority processes get more CPU time
* **Fair Scheduling**: Tries to give each process equal CPU time
* **Real-Time Scheduling**: Special policies for time-critical tasks

Key scheduling policies:

* **SCHED_NORMAL (SCHED_OTHER)**: Default time-sharing policy (CFS)
* **SCHED_FIFO**: Real-time first-in-first-out
* **SCHED_RR**: Real-time round-robin
* **SCHED_BATCH**: For batch processing (lower priority)
* **SCHED_IDLE**: For very low priority tasks

Process Context
===============

When a process runs, the kernel maintains:

* **task_struct**: Main process descriptor (defined in include/linux/sched.h)
* **Stack**: Kernel stack for the process
* **Registers**: CPU register state
* **Page Tables**: Memory mapping information

Understanding task_struct
==========================

The ``task_struct`` structure contains all information about a process:

* Process identification (PID, TGID)
* Process state
* Scheduling information
* Memory management info
* File system information
* Signal handling
* Parent/child relationships

Examples in This Directory
===========================

1. **process_basics.c**: Demonstrates process creation with fork()
2. **process_states.c**: Shows different process states
3. **process_priority.c**: Demonstrates process priorities
4. **process_info.c**: Shows how to get process information

Building the Examples
======================

To build all examples::

    make

To build a specific example::

    make process_basics

To clean build artifacts::

    make clean

Running the Examples
====================

After building, run examples as::

    ./process_basics
    ./process_states
    ./process_priority
    ./process_info

Note: Some examples may require root privileges to change scheduling policies.

Key Kernel Files
================

To learn more, examine these kernel source files:

* **kernel/sched/core.c**: Core scheduler code
* **kernel/fork.c**: Process creation
* **include/linux/sched.h**: Process descriptor definitions
* **kernel/exit.c**: Process termination

Documentation
=============

For more detailed information, see:

* Documentation/scheduler/sched-design-CFS.rst
* Documentation/scheduler/sched-deadline.rst
* Documentation/scheduler/sched-rt-group.rst

Further Reading
===============

* "Understanding the Linux Kernel" by Bovet & Cesati
* "Linux Kernel Development" by Robert Love
* Kernel source code: kernel/sched/ directory
* https://www.kernel.org/doc/html/latest/
