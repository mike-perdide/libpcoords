CC=gcc
CFLAGS=-g -I../ -I../include -I../parser/
LIBS=-lpcoords -lfl -ly -lpcre

all: tests

tests: test1.c test2.c test3.c test4.c test-colors.c
	$(CC) test1.c -o test1 $(CFLAGS) $(LIBS)
	$(CC) test2.c -o test2 $(CFLAGS) $(LIBS)
	$(CC) test3.c -o test3 $(CFLAGS) $(LIBS)
	$(CC) test4.c -o test4 $(CFLAGS) $(LIBS)
	$(CC) test-colors.c -o test-colors $(CFLAGS) $(LIBS)
	$(CC) open-twotimes.c -o open-twotimes $(CFLAGS) $(LIBS)

clean:
	rm -f test1
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f test-colors
	rm -f open-twotimes
	rm -f *.svg

