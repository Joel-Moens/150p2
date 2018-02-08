# Target library
lib := libuthread.a
ofiles := queue.o preempt.o context.o uthread.o 

CC := gcc
ARC := ar rcs
CFLAGS := -Wall -Werror -g

all: $(lib)
#Make a .o for queue.c
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<
#Make a .o for preempt.c
#Make a .o for context.c

#Make a .o for uthread.c

#Make a static library including the made .o files
$(lib): $(ofiles)
	$(ARC) $(lib) $(ofiles)

clean:
	rm -f $(lib) $(ofiles)
## TODO: Phase 1.1
