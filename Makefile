CC = gcc
DEFS = -D_DEFAULT_SOURCE
CFLAGS = -std=c99 -pedantic -Wall $(DEFS) -O3

OBJECTS = entropy.o


.PHONY: all clean

all: entropy

entropy: $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.dSYM entropy
