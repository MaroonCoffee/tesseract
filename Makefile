CFLAGS=-fwrapv -Wall -Wextra -Werror -g
CC=gcc -std=c99 -pedantic
CC_FAST:=$(CC) $(CFLAGS)
CC_SAFE:=$(CC) $(CFLAGS) -DDEBUG

LIB=lib/*.c

.PHONY: tesseract-unsafe tesseract tesseract-test clean
default: tesseract-unsafe tesseract tesseract-test

tesseract-unsafe: main.c
	$(CC_FAST) $(LIB) -o tesseract-unsafe main.c

tesseract: main.c
	$(CC_SAFE) $(LIB) -o tesseract main.c

tesseract-test: unit-tests.c
	$(CC_SAFE) $(LIB) -o tesseract-test unit-tests.c

clean:
	rm -Rf tesseract-unsafe tesseract tesseract-test *.dSYM
