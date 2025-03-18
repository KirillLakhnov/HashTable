#ifndef HASH_TABLE
#define HASH_TABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DELETED_ELEMENT (char*)-1

enum PROBE {
    LINEAR_PROBE    = 0,
    QUADRATIC_PROBE = 1,
    HASH_PROBE = 2,
};

typedef struct HashTable 
{
    char** keys;
    int* values;

    size_t size;
    size_t capacity;

    int (*hash_function)(const char*, int);

    enum PROBE probe;
} HashTable;

typedef struct HashTableIterator{
    HashTable* hash_table;
    size_t index;
} HashTableIterator;

//============================================================================
int  hashTableCtor (HashTable* hash_table, int (*hash_function)(const char*, int), size_t capacity, enum PROBE probe);
void hashTableDtor (HashTable* hash_table);

int hashTableInsert(HashTable* hash_table, const char* key, int value); 
int hashTableGet   (HashTable* hash_table, const char* key);           
int hashTableRemove(HashTable* hash_table, const char* key);            

int  hashTableIsFull(HashTable* hash_table);                     
int  hashTableResize(HashTable* hash_table, size_t new_capacity); 
void hashTableClear (HashTable* hash_table);                      

void hashTableDump      (HashTable* hash_table);
void hashTableDumpToFile(HashTable* hash_table, const char* filename);
//============================================================================
int murmurHashFunction(const char* key, int hash_table_capacity);
int  worseHashFunction(const char* key, int hash_table_capacity);
//============================================================================
void hashTableIteratorInit(HashTableIterator* hash_table_iterator, HashTable* hash_table);

const char* hashTableIteratorNext(HashTableIterator* hash_table_iterator, int* value);
const char* hashTableIteratorPrev(HashTableIterator* hash_table_iterator, int* value);

const char* hashTableIteratorGetByIndex(HashTableIterator* hash_table_iterator, size_t target_index, int* value);
//============================================================================

#endif //HASH_TABLE