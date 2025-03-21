#ifndef TEST_ITERATOR_HASH_TABLE
#define TEST_ITERATOR_HASH_TABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../hash_table/hash_table.h"

void testIteratorNext() 
{
    HashTable table = {};
    hashTableCtor(&table, murmurHashFunction, 10, LINEAR_PROBE);

    hashTableInsert(&table, "apple", 100);
    hashTableInsert(&table, "banana", 200);
    hashTableInsert(&table, "cherry", 300);

    hashTableDumpToFile(&table, "log_file.log");

    HashTableIterator it = {};
    hashTableIteratorInit(&it, &table);

    int value = 0;
    const char* key = hashTableIteratorNext(&it, &value);
    printf("\t%s - %d\n", key, value);

    key = hashTableIteratorNext(&it, &value);
    printf("\t%s - %d\n", key, value);

    key = hashTableIteratorNext(&it, &value);
    printf("\t%s - %d\n", key, value);

    key = hashTableIteratorNext(&it, &value);
    assert(key == NULL); // Должно вернуть NULL в конце

    printf("✅ TestIteratorNext PASSED\n");

    hashTableDtor(&table);
}

void testIteratorPrev() 
{
    HashTable table = {};
    hashTableCtor(&table, murmurHashFunction, 10, HASH_PROBE);

    hashTableInsert(&table, "apple", 100);
    hashTableInsert(&table, "banana", 200);
    hashTableInsert(&table, "cherry", 300);

    hashTableDumpToFile(&table, "log_file.log");

    HashTableIterator it = {};
    hashTableIteratorInit(&it, &table);

    int value = 0;
    hashTableIteratorNext(&it, &value);
    hashTableIteratorNext(&it, &value);
    hashTableIteratorNext(&it, &value);

    const char* key = hashTableIteratorPrev(&it, &value);
    printf("\t%s - %d\n", key, value);

    key = hashTableIteratorPrev(&it, &value);
    printf("\t%s - %d\n", key, value);

    key = hashTableIteratorPrev(&it, &value);
    assert(key == NULL); // Должно вернуть NULL в начале

    printf("✅ TestIteratorPrev PASSED\n");

    hashTableDtor(&table);
}

void testIteratorGetByIndex() 
{
    HashTable table = {};
    hashTableCtor(&table, murmurHashFunction, 10, QUADRATIC_PROBE);

    hashTableInsert(&table, "apple", 100);
    hashTableInsert(&table, "banana", 200);
    hashTableInsert(&table, "cherry", 300);

    hashTableDumpToFile(&table, "log_file.log");

    HashTableIterator it = {};
    hashTableIteratorInit(&it, &table);

    int value = 0;

    const char* key = hashTableIteratorGetByIndex(&it, 0, &value);
    printf("\t%s - %d\n", key, value);

    key = hashTableIteratorGetByIndex(&it, 1, &value);
    printf("\t%s - %d\n", key, value);

    key = hashTableIteratorGetByIndex(&it, 2, &value);
    printf("\t%s - %d\n", key, value);

    key = hashTableIteratorGetByIndex(&it, 5, &value);
    assert(key == NULL); // Должно вернуть NULL, если индекс выходит за границы

    printf("✅ TestIteratorGetByIndex PASSED\n");

    hashTableDtor(&table);
}

void allTestsIteratorHashTable() 
{
    testIteratorNext();
    testIteratorPrev();
    testIteratorGetByIndex();

    printf("\n🎉 Все тесты для итератора успешно пройдены!\n");
}

#endif //TEST_ITERATOR_HASH_TABLE