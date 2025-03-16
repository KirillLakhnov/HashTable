#ifndef HASH_TABLE
#define HASH_TABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DELETED_ELEMENT (char*)-1

enum MODE_PRIME {
    NEAREST_LESSER = 0,
    NEAREST_BIGGER = 1,
};

enum PROBE {
    LINEAR_PROBE    = 0,
    QUADRATIC_PROBE = 1,
    HASH_PROBE = 2,
};

enum PROBE_MODE {
    INSERT_MODE = 0,
    GET_MODE    = 1,
    REMOVE_MODE = 2,
};

struct hashTable 
{
    char** keys;
    int* values;

    size_t size;
    size_t capacity;

    int (*hash_function)(const char*, int);

    enum PROBE probe;
};
struct hashTableIterator{
    struct hashTable* hash_table;
    size_t index;
};

//============================================================================
int hashTableCtor  (struct hashTable* hash_table, int (*hash_function)(const char*, int), size_t capacity, enum PROBE probe);
void hashTableDtor (struct hashTable* hash_table);

int hashTableInsert(struct hashTable* hash_table, const char* key, int value); // Вставляет элемент в таблицу
int hashTableGet   (struct hashTable* hash_table, const char* key);            // Получает значение по ключу
int hashTableRemove(struct hashTable* hash_table, const char* key);            // Удаляет элемент из таблицы

int hashTableProbe (struct hashTable* hash_table, enum PROBE_MODE probe_mode, const char* key, int value);

int  hashTableIsFull(struct hashTable* hash_table);                      // Проверяет заполнена ли таблица
int  hashTableResize(struct hashTable* hash_table, size_t new_capacity); // Увеличивает размер таблицы, если та заполнена
void hashTableClear (struct hashTable* hash_table);                      // Очищает таблицу

void hashTableDump      (struct hashTable* hash_table);
void hashTableDumpToFile(struct hashTable* hash_table, const char* filename);
//============================================================================
int MurmurHashFunction(const char* key, int hash_table_capacity);
//============================================================================
void hashTableIteratorInit(struct hashTableIterator* hash_table_iterator, struct hashTable* hash_table);

long long int hashTableIteratorHasNext(struct hashTableIterator* hash_table_iterator);
long long int hashTableIteratorHasPrev(struct hashTableIterator* hash_table_iterator);

const char* hashTableIteratorNext(struct hashTableIterator* hash_table_iterator, int* value);
const char* hashTableIteratorPrev(struct hashTableIterator* hash_table_iterator, int* value);
//============================================================================
int IsPrime(size_t n);
size_t FindNearestPrime(size_t n, enum MODE_PRIME mode);
//============================================================================


#endif //HASH_TABLE