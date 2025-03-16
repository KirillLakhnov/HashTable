CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -fsanitize=address

SRC = hash_table.c tests.c
OBJ = $(SRC:.c=.o)
EXEC = HashTable

all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)

rebuild: clean all

.PHONY: all clean rebuild
