#ifndef TEST_HASH_TABLE
#define TEST_HASH_TABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../hash_table/hash_table.h"

void TestHashTableInsertAndGet() 
{
    struct hashTable table = {};
    hashTableCtor(&table, MurmurHashFunction, 10, LINEAR_PROBE);

    hashTableInsert(&table, "apple", 50);
    hashTableInsert(&table, "banana", 200);
    hashTableInsert(&table, "cherry", 300);

    hashTableDumpToFile(&table, "log_file.log");

    assert(hashTableGet(&table, "apple") == 50);
    assert(hashTableGet(&table, "banana") == 200);
    assert(hashTableGet(&table, "cherry") == 300);
    assert(hashTableGet(&table, "grape") == -1);

    printf("✅ TestInsertAndGet PASSED\n");

    hashTableDtor(&table);
}
void TestHashTableRemove() 
{
    struct hashTable table = {};
    hashTableCtor(&table, MurmurHashFunction, 10, QUADRATIC_PROBE);

    hashTableInsert(&table, "apple", 100);
    hashTableDumpToFile(&table, "log_file.log");

    hashTableRemove(&table, "apple");
    hashTableDumpToFile(&table, "log_file.log");

    assert(hashTableGet(&table, "apple") == -1);

    printf("✅ TestRemove PASSED\n");

    hashTableDtor(&table);
}
void TestHashTableCollisions() 
{
    struct hashTable table = {};
    hashTableCtor(&table, MurmurHashFunction, 10, HASH_PROBE); // Маленький размер для создания коллизий

    hashTableInsert(&table, "one", 1);
    hashTableInsert(&table, "two", 2);
    hashTableInsert(&table, "three", 3);
    hashTableInsert(&table, "four", 4);
    hashTableInsert(&table, "five", 5);
    hashTableDumpToFile(&table, "log_file.log");

    assert(hashTableGet(&table, "one") == 1);
    assert(hashTableGet(&table, "two") == 2);
    assert(hashTableGet(&table, "three") == 3);
    assert(hashTableGet(&table, "four") == 4);
    assert(hashTableGet(&table, "five") == 5);
    assert(hashTableGet(&table, "six") == -1);

    printf("✅ TestCollisions PASSED\n");

    hashTableDtor(&table);
}
void TestHashTableResize() 
{
    struct hashTable table = {};
    hashTableCtor(&table, MurmurHashFunction, 2, HASH_PROBE); 

    hashTableInsert(&table, "a", 1); // Должно вызвать увеличение таблицы
    hashTableDumpToFile(&table, "log_file.log");

    hashTableInsert(&table, "b", 2);
    hashTableInsert(&table, "c", 3); 
    hashTableDumpToFile(&table, "log_file.log");

    hashTableInsert(&table, "d", 4); // Должно вызвать увеличение таблицы
    hashTableInsert(&table, "e", 5);
    hashTableDumpToFile(&table, "log_file.log");

    hashTableRemove(&table, "a");
    hashTableDumpToFile(&table, "log_file.log");

    hashTableRemove(&table, "b"); // Должно вызвать уменьшение таблицы
    hashTableDumpToFile(&table, "log_file.log");

    hashTableRemove(&table, "c"); 
    hashTableDumpToFile(&table, "log_file.log");

    hashTableRemove(&table, "d"); // Должно вызвать уменьшение таблицы
    hashTableDumpToFile(&table, "log_file.log");
    
    assert(hashTableGet(&table, "e") == 5);

    printf("✅ TestResize PASSED\n");

    hashTableDtor(&table);
}

void AllTestsHashTable()
{
    TestHashTableInsertAndGet();
    TestHashTableRemove();
    TestHashTableCollisions();
    TestHashTableResize();

    printf("\n🎉 Все тесты успешно пройдены!\n");
}

#endif //TEST_HASH_TABLE