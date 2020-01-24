CC := gcc
DEFS := -D_DEFAULT_SOURCE
CFLAGS := -std=c99 -pedantic -Wall -Wextra $(DEFS) -O3

EXEC = entropy
OBJECTS = entropy.o


.PHONY: all
all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(EXEC)
