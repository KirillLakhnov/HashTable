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

typedef struct hashTable 
{
    char** keys;
    int* values;

    size_t size;
    size_t capacity;

    int (*hash_function)(const char*, int);

    enum PROBE probe;
} hashTable;

typedef struct hashTableIterator{
    struct hashTable* hash_table;
    size_t index;
} hashTableIterator;

//============================================================================
int  hashTableCtor (hashTable* hash_table, int (*hash_function)(const char*, int), size_t capacity, enum PROBE probe);
void hashTableDtor (hashTable* hash_table);

int hashTableInsert(hashTable* hash_table, const char* key, int value); 
int hashTableGet   (hashTable* hash_table, const char* key);           
int hashTableRemove(hashTable* hash_table, const char* key);            

int  hashTableIsFull(hashTable* hash_table);                     
int  hashTableResize(hashTable* hash_table, size_t new_capacity); 
void hashTableClear (hashTable* hash_table);                      

void hashTableDump      (hashTable* hash_table);
void hashTableDumpToFile(hashTable* hash_table, const char* filename);
//============================================================================
int MurmurHashFunction(const char* key, int hash_table_capacity);
int WorseHashFunction(const char* key, int hash_table_capacity);
//============================================================================
void hashTableIteratorInit(hashTableIterator* hash_table_iterator, hashTable* hash_table);

const char* hashTableIteratorNext(hashTableIterator* hash_table_iterator, int* value);
const char* hashTableIteratorPrev(hashTableIterator* hash_table_iterator, int* value);

const char* hashTableIteratorGetByIndex(hashTableIterator* hash_table_iterator, size_t target_index, int* value);
//============================================================================

#endif //HASH_TABLE