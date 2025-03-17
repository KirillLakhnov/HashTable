#ifndef TEST_OF_SPEED_FOR_PROBING
#define TEST_OF_SPEED_FOR_PROBING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../hash_table/hash_table.h"

void GenerateRandomKey(char *key, size_t length) 
{
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length - 1; i++) 
    {
        key[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    key[length - 1] = '\0';
}

// Замер времени выполнения функции
double MeasureTime(void (*test_hash_function)(struct hashTable*, char**, int), struct hashTable* table, char** keys, int num_keys) 
{
    clock_t start = clock();
    test_hash_function(table, keys, num_keys);
    return (double)(clock() - start)/CLOCKS_PER_SEC;
}

// Функции для тестирования вставки, поиска и удаления
void TestInsert(struct hashTable* table, char** keys, int num_keys) 
{
    for (int i = 0; i < num_keys; i++) 
    {
        hashTableInsert(table, keys[i], i);
    }
}
void TestSearch(struct hashTable* table, char** keys, int num_keys) 
{
    for (int i = 0; i < num_keys; i++) 
    {
        hashTableGet(table, keys[i]);
    }
}
void TestRemove(struct hashTable* table, char** keys, int num_keys) 
{
    for (int i = 0; i < num_keys; i++) 
    {
        hashTableRemove(table, keys[i]);
    }
}

void TestOfSpeed() 
{
    srand(time(NULL));

    int num_keys = 10000000;
    int key_length = 10;

    char* keys[num_keys];

    for (int i = 0; i < num_keys; i++) 
    {
        char key_i[key_length];
        keys[i] = key_i;

        GenerateRandomKey(keys[i], key_length);
    }

    struct hashTable table_linear    = {};
    struct hashTable table_quadratic = {};
    struct hashTable table_double    = {};

    hashTableCtor(&table_linear,    WorseHashFunction, num_keys * 1.2, LINEAR_PROBE);
    hashTableCtor(&table_quadratic, WorseHashFunction, num_keys * 1.2, QUADRATIC_PROBE);
    hashTableCtor(&table_double,    WorseHashFunction, num_keys * 1.2, HASH_PROBE);

    printf("\n🔹 Вставка:\n");
    printf("Линейное пробирование:  %.6f сек\n",    MeasureTime(TestInsert, &table_linear,    keys, num_keys));
    printf("Квадратичное пробирование: %.6f сек\n", MeasureTime(TestInsert, &table_quadratic, keys, num_keys));
    printf("Двойное хеширование:  %.6f сек\n",      MeasureTime(TestInsert, &table_double,    keys, num_keys));

    printf("\n🔍 Поиск:\n");
    printf("Линейное пробирование:  %.6f сек\n",    MeasureTime(TestSearch, &table_linear,    keys, num_keys));
    printf("Квадратичное пробирование: %.6f сек\n", MeasureTime(TestSearch, &table_quadratic, keys, num_keys));
    printf("Двойное хеширование:  %.6f сек\n",      MeasureTime(TestSearch, &table_double,    keys, num_keys));

    printf("\n❌ Удаление:\n");
    printf("Линейное пробирование:  %.6f сек\n",    MeasureTime(TestRemove, &table_linear,    keys, num_keys));
    printf("Квадратичное пробирование: %.6f сек\n", MeasureTime(TestRemove, &table_quadratic, keys, num_keys));
    printf("Двойное хеширование:  %.6f сек\n",      MeasureTime(TestRemove, &table_double,    keys, num_keys));

    hashTableDtor(&table_linear);
    hashTableDtor(&table_quadratic);
    hashTableDtor(&table_double);
}



#endif //TEST_OF_SPEED_FOR_PROBING