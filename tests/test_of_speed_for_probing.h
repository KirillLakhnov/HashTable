#ifndef TEST_OF_SPEED_FOR_PROBING
#define TEST_OF_SPEED_FOR_PROBING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../hash_table/hash_table.h"

void generateRandomKey(char *key, size_t length) 
{
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length - 1; i++) 
    {
        key[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    key[length - 1] = '\0';
}

double measureTime(void (*test_hash_function)(HashTable*, char**, int), HashTable* table, char** keys, int num_keys) 
{
    clock_t start = clock();
    test_hash_function(table, keys, num_keys);
    return (double)(clock() - start)/CLOCKS_PER_SEC;
}

void testInsert(HashTable* table, char** keys, int num_keys) 
{
    for (int i = 0; i < num_keys; i++) 
    {
        hashTableInsert(table, keys[i], i);
    }
}
void testSearch(HashTable* table, char** keys, int num_keys) 
{
    for (int i = 0; i < num_keys; i++) 
    {
        hashTableGet(table, keys[i]);
    }
}
void testRemove(HashTable* table, char** keys, int num_keys) 
{
    for (int i = 0; i < num_keys; i++) 
    {
        hashTableRemove(table, keys[i]);
    }
}

void testOfSpeed() 
{
    srand(time(NULL));

    int num_keys = 10000000;
    int key_length = 10;

    char* keys[num_keys];

    for (int i = 0; i < num_keys; i++) 
    {
        char key_i[key_length];
        keys[i] = key_i;

        generateRandomKey(keys[i], key_length);
    }

    HashTable table_linear    = {};
    HashTable table_quadratic = {};
    HashTable table_double    = {};

    hashTableCtor(&table_linear,    worseHashFunction, num_keys * 1.2, LINEAR_PROBE);
    hashTableCtor(&table_quadratic, worseHashFunction, num_keys * 1.2, QUADRATIC_PROBE);
    hashTableCtor(&table_double,    worseHashFunction, num_keys * 1.2, HASH_PROBE);

    printf("\n🔹 Вставка:\n");
    printf("Линейное пробирование:  %.6f сек\n",    measureTime(testInsert, &table_linear,    keys, num_keys));
    printf("Квадратичное пробирование: %.6f сек\n", measureTime(testInsert, &table_quadratic, keys, num_keys));
    printf("Двойное хеширование:  %.6f сек\n",      measureTime(testInsert, &table_double,    keys, num_keys));

    printf("\n🔍 Поиск:\n");
    printf("Линейное пробирование:  %.6f сек\n",    measureTime(testSearch, &table_linear,    keys, num_keys));
    printf("Квадратичное пробирование: %.6f сек\n", measureTime(testSearch, &table_quadratic, keys, num_keys));
    printf("Двойное хеширование:  %.6f сек\n",      measureTime(testSearch, &table_double,    keys, num_keys));

    printf("\n❌ Удаление:\n");
    printf("Линейное пробирование:  %.6f сек\n",    measureTime(testRemove, &table_linear,    keys, num_keys));
    printf("Квадратичное пробирование: %.6f сек\n", measureTime(testRemove, &table_quadratic, keys, num_keys));
    printf("Двойное хеширование:  %.6f сек\n",      measureTime(testRemove, &table_double,    keys, num_keys));

    hashTableDtor(&table_linear);
    hashTableDtor(&table_quadratic);
    hashTableDtor(&table_double);
}



#endif //TEST_OF_SPEED_FOR_PROBING