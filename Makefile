CC := gcc
DEFS := -D_DEFAULT_SOURCE
CFLAGS := -std=c99 -pedantic -Wall $(DEFS) -O3

EXEC = entropy-calc
OBJECTS = entropy-calc.o


.PHONY: all
all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(EXEC)
