CC := gcc
DEFS := -D_DEFAULT_SOURCE
FLAGS := -std=c99 -pedantic -Wall -Wextra $(DEFS) -O3

EXEC = entropy
OBJECTS = entropy.o


.PHONY: all
all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ -lm

%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(EXEC)
