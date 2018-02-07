# Target library
lib := libuthread.a
ofiles := queue.o

CC := gcc
ARC := ar rcs
CFLAGS := -Wall -Werror -g

all: $(lib)
#Make a .o for queue.c
queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c -o $@ $<
#Make a .o for preempt.c
#Make a .o for context.c

#Make a .o for uthread.c

#Make a static library including the made .o files
$(lib): queue.o
	$(ARC) $(lib) $(ofiles)

clean:
	rm -f $(lib) $(ofiles)
## TODO: Phase 1.1
