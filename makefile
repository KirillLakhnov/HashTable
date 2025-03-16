CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -fsanitize=address

SRC = hash_table.c main.c
OBJ = $(SRC:.c=.o)
EXEC = HashTable

TEST_HEADERS = test_hash_table.h test_iterator_hash_table.h

all: $(EXEC)

$(EXEC): $(OBJ) hash_table.h $(TEST_HEADERS)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)

clean_log:
	rm log_file.log

rebuild: clean all

.PHONY: all clean rebuild
