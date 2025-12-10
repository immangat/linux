# Makefile for Process Management Examples
#
# This Makefile builds educational examples demonstrating process management
# concepts in Linux.

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -O2
LDFLAGS :=

# Targets
TARGETS := process_basics process_states process_priority process_info

.PHONY: all clean help

all: $(TARGETS)
	@echo ""
	@echo "All examples built successfully!"
	@echo ""
	@echo "Run examples with:"
	@echo "  ./process_basics     - Learn about fork() and process creation"
	@echo "  ./process_states     - See different process states"
	@echo "  ./process_priority   - Understand scheduling and priorities"
	@echo "  ./process_info       - Get process information"
	@echo ""

process_basics: process_basics.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

process_states: process_states.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

process_priority: process_priority.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

process_info: process_info.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o

help:
	@echo "Process Management Examples - Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all              - Build all examples (default)"
	@echo "  process_basics   - Build process_basics example"
	@echo "  process_states   - Build process_states example"
	@echo "  process_priority - Build process_priority example"
	@echo "  process_info     - Build process_info example"
	@echo "  clean            - Remove built binaries"
	@echo "  help             - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make                    # Build all examples"
	@echo "  make process_basics     # Build specific example"
	@echo "  make clean              # Clean build artifacts"
