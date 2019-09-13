CFLAGS=-pedantic -std=gnu99 -Wall -Werror
test: test.o
	gcc -o test $(CFLAGS) test.o

test.o: test.c
	gcc -c $(CFLAGS) test.c
