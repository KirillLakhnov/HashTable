CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -fsanitize=address
LDFLAGS = -Wl,-stack_size,0x20000000

SRC = hash_table.c main.c
OBJ = $(SRC:.c=.o)
EXEC = HashTable

TEST_HEADERS = test_hash_table.h test_iterator_hash_table.h test_of_speed_for_probing.h

all: $(EXEC)

$(EXEC): $(OBJ) hash_table.h $(TEST_HEADERS)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

clean_log:
	rm log_file.log

rebuild: clean all

.PHONY: all clean rebuild
