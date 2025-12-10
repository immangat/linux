Quick Start Guide: Understanding Process Management
====================================================

Welcome! This guide will help you quickly understand Linux process management.

Step 1: Build the Examples
---------------------------

Navigate to this directory and build all examples::

    cd samples/process_management
    make

You should see output confirming all examples were built successfully.

Step 2: Start Learning
-----------------------

Run the examples in this order for the best learning experience:

1. **Process Basics** - Start here to understand process creation::

    ./process_basics

   This shows how fork() creates new processes and how parent/child
   processes work together.

2. **Process Information** - Learn what information Linux tracks::

    ./process_info

   This demonstrates how to get process IDs, resource usage, and read
   the /proc filesystem.

3. **Process States** - Understand the process lifecycle::

    ./process_states

   This shows the different states a process can be in: running,
   sleeping, zombie, and stopped.

4. **Process Priority** - Learn about scheduling::

    ./process_priority

   This demonstrates how Linux decides which process runs when, using
   nice values and scheduling policies.

Step 3: Experiment
-------------------

Try modifying the examples:

* Change the nice value in process_priority.c
* Adjust sleep times in process_states.c  
* Add more child processes in process_basics.c
* Explore different /proc files in process_info.c

Step 4: Explore the Kernel
---------------------------

Once you understand the basics, explore the actual kernel code:

* ``kernel/fork.c`` - See how processes are created
* ``kernel/sched/core.c`` - Understand the scheduler
* ``include/linux/sched.h`` - View the task_struct structure
* ``kernel/exit.c`` - See how processes terminate

Key Concepts to Remember
-------------------------

1. **Process Creation**: fork() creates a copy of the calling process
2. **Process States**: Processes transition between running, sleeping, and stopped
3. **Scheduling**: CFS ensures fair CPU time distribution
4. **Process Information**: Available via system calls and /proc filesystem
5. **Parent-Child**: Every process (except init) has a parent

Tools to Use
------------

These command-line tools help you work with processes::

    ps aux          # List all processes
    top             # Monitor processes in real-time
    htop            # Better interactive process viewer
    pstree          # Show process tree
    nice -n 10 cmd  # Run command with lower priority
    renice -n 5 PID # Change priority of running process
    kill -STOP PID  # Stop a process
    kill -CONT PID  # Continue a stopped process

Further Learning
----------------

After mastering these examples, read:

* Documentation/scheduler/sched-design-CFS.rst - CFS scheduler details
* Documentation/admin-guide/kernel-parameters.rst - Kernel tuning
* "Understanding the Linux Kernel" book
* Linux kernel source code

Getting Help
------------

If you need help:

1. Read the README.rst in this directory
2. Check the comments in the example programs
3. Visit https://www.kernel.org/doc/html/latest/
4. Join #kernelnewbies on IRC

Happy Learning!
