CC=gcc
DEBUG=-g
CFLAGS=-Wall -pedantic $(DEBUG)
LFLAGS=

define cc-command
$(CC) $(CFLAGS) -c -o $@ $< 
endef

all-programs=example tester
all: $(all-programs)

# .c files
example: example.o stopwatch.o
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ -lrt

example.o: example.c stopwatch.h ymca_timing.h
	$(cc-command)

tester: tester.o stopwatch.o
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ -lrt

tester.o: tester.c stopwatch.h ymca_timing.h
	$(cc-command)

stopwatch.o: stopwatch.c stopwatch.h
	$(cc-command)

# .h files
stopwatch.h:
ymca_timing.h:

.PHONY: all clean test

test: example tester
	./tester ./example

clean:
	rm -f *.o $(all-programs)
