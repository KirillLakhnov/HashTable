#ifndef HASH_TABLE
#define HASH_TABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DELETED_ELEMENT (char*)-1

struct hashTable 
{
    char** keys;
    int* values;

    size_t size;
    size_t capacity;

    int (*hash_function)(const char*, int);
};
struct hashTableIterator{
    struct hashTable* hash_table;
    size_t index;
};

//============================================================================
int hashTableCtor(struct hashTable* hash_table, int (*hash_function)(const char*, int), size_t capacity);
void hashTableDtor (struct hashTable* hash_table);

int hashTableInsert(struct hashTable* hash_table, const char* key, int value); // Вставляет элемент в таблицу
int hashTableGet   (struct hashTable* hash_table, const char* key);            // Получает значение по ключу
int hashTableRemove(struct hashTable* hash_table, const char* key);            // Удаляет элемент из таблицы

int  hashTableIsFull(struct hashTable* hash_table);                      // Проверяет заполнена ли таблица
int  hashTableResize(struct hashTable* hash_table, size_t new_capacity); // Увеличивает размер таблицы, если та заполнена
void hashTableClear (struct hashTable* hash_table);                      // Очищает таблицу

void hashTableDump (struct hashTable* hash_table);
void hashTableDumpToFile(struct hashTable* hash_table, const char* filename);
//============================================================================
int MurmurHashFunction(const char* key, int hash_table_capacity);
int SDBMHashFunction  (const char* key, int hash_table_capacity);
//============================================================================
void hashTableIteratorInit(struct hashTableIterator* hash_table_iterator, struct hashTable* hash_table);

long long int hashTableIteratorHasNext(struct hashTableIterator* hash_table_iterator);
long long int hashTableIteratorHasPrev(struct hashTableIterator* hash_table_iterator);

const char* hashTableIteratorNext(struct hashTableIterator* hash_table_iterator, int* value);
const char* hashTableIteratorPrev(struct hashTableIterator* hash_table_iterator, int* value);
//============================================================================






#endif //HASH_TABLE