CC := gcc
CFLAGS := -Wall -Wextra -Werror -pedantic -std=c11 -D_XOPEN_SOURCE=700

.PHONY: all
all: tqueue tqueue.o

.PHONY: clean
clean:
	-rm -f tqueue tqueue.o test.o

tqueue: tqueue.o test.o
	$(CC) $(CFLAGS) $^ -o $@

test.o: test.c tqueue.h makefile
	$(CC) $(CFLAGS) $< -c -o $@

tqueue.o: tqueue.c tqueue.h makefile
	$(CC) $(CFLAGS) $< -c -o $@
