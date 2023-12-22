CFLAGS=-fwrapv -Wall -Wextra -Werror -g
CC=gcc -std=c99 -pedantic
CC_FAST:=$(CC) $(CFLAGS)
CC_SAFE:=$(CC) $(CFLAGS) -fsanitize=undefined -DDEBUG

LIB=lib/*.c

.PHONY: tesseract tesseract-debug tesseract-test clean
default: tesseract tesseract-debug tesseract-test

tesseract: main.c
	$(CC_FAST) $(LIB) -o tesseract main.c

tesseract-debug: main.c
	$(CC_SAFE) $(LIB) -o tesseract-debug main.c

tesseract-test: unit-tests.c
	$(CC_SAFE) $(LIB) -o tesseract-test unit-tests.c

clean:
	rm -Rf tesseract tesseract-debug tesseract-test *.dSYM
