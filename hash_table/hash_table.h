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
int  hashTableCtor (struct hashTable* hash_table, int (*hash_function)(const char*, int), size_t capacity, enum PROBE probe);
void hashTableDtor (struct hashTable* hash_table);

int hashTableInsert(struct hashTable* hash_table, const char* key, int value); 
int hashTableGet   (struct hashTable* hash_table, const char* key);           
int hashTableRemove(struct hashTable* hash_table, const char* key);            

int  hashTableIsFull(struct hashTable* hash_table);                     
int  hashTableResize(struct hashTable* hash_table, size_t new_capacity); 
void hashTableClear (struct hashTable* hash_table);                      

void hashTableDump      (struct hashTable* hash_table);
void hashTableDumpToFile(struct hashTable* hash_table, const char* filename);
//============================================================================
int MurmurHashFunction(const char* key, int hash_table_capacity);
int WorseHashFunction(const char* key, int hash_table_capacity);
//============================================================================
void hashTableIteratorInit(struct hashTableIterator* hash_table_iterator, struct hashTable* hash_table);

const char* hashTableIteratorNext(struct hashTableIterator* hash_table_iterator, int* value);
const char* hashTableIteratorPrev(struct hashTableIterator* hash_table_iterator, int* value);

const char* hashTableIteratorGetByIndex(struct hashTableIterator* hash_table_iterator, size_t target_index, int* value);
//============================================================================

#endif //HASH_TABLE