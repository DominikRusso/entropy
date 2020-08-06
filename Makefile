CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -O2 -march=native -pipe $(DEFS)
DEFS = -D_DEFAULT_SOURCE
LDFLAGS = -lm

OUT = entropy
OBJ = entropy.o

.PHONY: all
all: $(OUT)

$(OUT): $(OBJ)

.PHONY: clean
clean:
	rm -rf $(OBJ) $(OUT)

