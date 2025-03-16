#include "hash_table.h"

int hashTableCtor(struct hashTable* hash_table, int (*hash_function)(const char*, int), size_t capacity, enum PROBE probe)
{
    assert(hash_table);
    assert(hash_function);

    hash_table->size = 0;
    hash_table->probe = probe;
    if(probe == HASH_PROBE)
    {
        hash_table->capacity = FindNearestPrime(capacity, NEAREST_BIGGER);
    }
    else
    {
        hash_table->capacity = capacity;
    }

    hash_table->keys = (char**)calloc(hash_table->capacity, sizeof(char*));
    if(!hash_table->keys)
    {
        hashTableDtor(hash_table);
        return -1; 
    }

    hash_table->values = (int*)calloc(hash_table->capacity, sizeof(int));
    if(!hash_table->values)
    {
        hashTableDtor(hash_table);
        return -1; 
    }

    hash_table->hash_function = hash_function;

    return 0;
}

void hashTableDtor(struct hashTable* hash_table)
{
    assert(hash_table);

    for(size_t i = 0; i < hash_table->capacity; i++)
    {
        if(hash_table->keys[i] != DELETED_ELEMENT)
        {
            free(hash_table->keys[i]);
        }
        else
        {
            hash_table->keys[i] = NULL;
        }
    }

    free(hash_table->keys);
    free(hash_table->values);
}

//============================================================================
int hashTableInsert(struct hashTable* hash_table, const char* key, int value)
{
    assert(hash_table);
    assert(key);

    if (hashTableIsFull(hash_table)) 
    {
        size_t new_capacity = 0;

        if (hash_table->probe == HASH_PROBE)
        {
            new_capacity = FindNearestPrime(hash_table->capacity * 2, NEAREST_BIGGER);
        }
        else
        {
            new_capacity = hash_table->capacity * 2;
        }

        if (hashTableResize(hash_table, new_capacity) == -1)
        {
            return -1;
        }
    }

    int index = hashTableProbe(hash_table, INSERT_MODE, key, value);

    hash_table->keys[index] = strdup(key);
    if (!hash_table->keys[index])
    {
        return -1;
    }

    hash_table->values[index] = value;
    hash_table->size++;

    return 0;
} 

int hashTableGet(struct hashTable* hash_table, const char* key)
{
    assert(hash_table);
    assert(key);

    return hashTableProbe(hash_table, GET_MODE, key, 0);
}        
  
int hashTableRemove(struct hashTable* hash_table, const char* key)
{
    assert(hash_table);
    assert(key);

    return hashTableProbe(hash_table, REMOVE_MODE, key, 0);
}

//============================================================================
int hashTableProbe(struct hashTable* hash_table, enum PROBE_MODE probe_mode, const char* key, int value)
{
    int hash1 = hash_table->hash_function(key, hash_table->capacity);
    int hash2 = 1 + (hash1 % (hash_table->capacity - 1));

    int index = hash1 % hash_table->capacity;
    int offset = 0;

    while(hash_table->keys[index] != NULL)
    {
        if (probe_mode == INSERT_MODE)
        {
            if(hash_table->keys[index] == DELETED_ELEMENT)
            {
                return index;
            }
        }
        
        if (strcmp(hash_table->keys[index], key) == 0) 
        {
            if (probe_mode == INSERT_MODE)
            {
                hash_table->values[index] = value;
                return 0;
            }
            else
            {
                if (hash_table->keys[index] != DELETED_ELEMENT)
                {
                    if (probe_mode == GET_MODE)
                    {
                        return hash_table->values[index];
                    }
                    else if (probe_mode == REMOVE_MODE)
                    {
                        free(hash_table->keys[index]);
            
                        hash_table->keys[index] = DELETED_ELEMENT;
                        hash_table->size--;

                        if(hash_table->size <= ((hash_table->capacity/2) - 1))
                        {
                            size_t new_capacity = 0;

                            if (hash_table->probe == HASH_PROBE)
                            {
                                new_capacity = FindNearestPrime(hash_table->capacity/2, NEAREST_BIGGER);
                            }
                            else
                            {
                                new_capacity = hash_table->capacity/2;
                            }

                            hashTableResize(hash_table, new_capacity);
                        }

                        return 0;
                    }
                }
            }   
        }

        offset++;
        switch(hash_table->probe)
        {
            case(LINEAR_PROBE):
                index = (index + offset) % hash_table->capacity;
                break;
            case(QUADRATIC_PROBE):
                index = (index + offset*offset) % hash_table->capacity;
                break;
            case(HASH_PROBE):
                index = (hash1 + offset*hash2) % hash_table->capacity;
                break;
        }
    }

    return (probe_mode == INSERT_MODE) ? index : -1;
}
//============================================================================
int hashTableIsFull(struct hashTable* hash_table)
{
    assert(hash_table);

    if(hash_table->size >= hash_table->capacity - 1) 
    {
        /* "-1" используется для того, чтобы не ломать алгоритмы пробирования, 
           которые выполняют циклы до того момента, пока не найдут пустой элемент. 
           То есть в таблице всегда должна быть свободная ячейка. */
        
        return 1;
    }

    return 0;
}

int hashTableResize(struct hashTable* hash_table, size_t new_capacity)
{
    assert(hash_table);

    char** new_keys = (char**)calloc(new_capacity, sizeof(char*));
    if(!new_keys)
    {
        return -1;
    }

    int* new_values = (int*)calloc(new_capacity, sizeof(int));
    if(!new_values)
    {
        return -1;
    }

    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        if (hash_table->keys[i] != NULL && hash_table->keys[i] != DELETED_ELEMENT) 
        {
            int new_index = hash_table->hash_function(hash_table->keys[i], new_capacity);
            
            while (new_keys[new_index] != NULL) 
            {
                new_index = (new_index + 1) % new_capacity;
            }

            new_keys[new_index]   = hash_table->keys[i];
            new_values[new_index] = hash_table->values[i];
        }
    }

    free(hash_table->keys);
    free(hash_table->values);

    hash_table->capacity = new_capacity;
    hash_table->keys     = new_keys;
    hash_table->values   = new_values;

    return 0;
}

void hashTableClear(struct hashTable* hash_table)
{
    assert(hash_table);

    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        if (hash_table->keys[i] != NULL && 
            hash_table->keys[i] != DELETED_ELEMENT) 
        {
            free(hash_table->keys[i]);
            hash_table->keys[i] = NULL;
        }
    }

    memset(hash_table->values, 0, hash_table->capacity * sizeof(int));
    hash_table->size = 0;
}

//============================================================================
void hashTableDump(struct hashTable* hash_table)
{
    assert(hash_table);

    double load_factor = (double)(hash_table->size / hash_table->capacity);
    int total_collisions  = 0;
    int total_probes      = 0;
    int inserted_elements = 0;
    int max_probe_length  = 0;

    int collision_counts[hash_table->capacity];
    memset(collision_counts, 0, sizeof(collision_counts));

    printf("\n==================================================================================\n");
    printf("                                Хеш-таблица (размер: %zu/%zu)\n", hash_table->size, hash_table->capacity);
    printf("                                Load Factor: %.2f\n", load_factor);
    printf("==================================================================================\n");
    printf("| %-5s | %-20s | %-10s | %-10s | %-8s |\n", "Index", "Key", "Value", "Hash", "Probes");
    printf("----------------------------------------------------------------------------------\n");

    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        if (hash_table->keys[i] == NULL)
        {
            printf("| %-5zu | %-25s | %-10s | %-10s | %-8s |\n", i, "(пусто)", "-", "-", "-");
        }
        else if (hash_table->keys[i] == DELETED_ELEMENT)
        {
            printf("| %-5zu | %-27s | %-10s | %-10s | %-8s |\n", i, "(удалено)", "-", "-", "-");
        }
        else
        {
            int hash = hash_table->hash_function(hash_table->keys[i], hash_table->capacity);
            int probes = 0;
            size_t index = hash % hash_table->capacity;
            int offset = 0;

            while (index != i)
            {
                offset++;
                switch (hash_table->probe)
                {
                    case LINEAR_PROBE:
                        index = (hash + offset) % hash_table->capacity;
                        break;
                    case QUADRATIC_PROBE:
                        index = (hash + offset * offset) % hash_table->capacity;
                        break;
                    case HASH_PROBE:
                        index = (hash + offset * (1 + (hash % (hash_table->capacity - 1)))) % hash_table->capacity;
                        break;
                }
                probes++;
            }

            if (probes > 0)
            {
                total_collisions++;
                collision_counts[i] = probes;
            }

            total_probes += probes;
            inserted_elements++;

            if (probes > max_probe_length)
            {
                max_probe_length = probes;
            }

            printf("| %-5zu | %-20s | %-10d | %-10d | %-8d |\n", i, hash_table->keys[i], hash_table->values[i], hash, probes);
        }
    }

    printf("----------------------------------------------------------------------------------\n");

    double avg_probing_length = (inserted_elements > 0) ? (double)total_probes / inserted_elements : 0;
    double avg_probes_per_insert = (hash_table->size > 0) ? (double)total_probes / hash_table->size : 0;

    printf("\n📊  Статистика:\n");
    printf("📌 Коллизии: %d\n", total_collisions);
    printf("📌 Средняя длина пробирования: %.2f\n", avg_probing_length);
    printf("📌 Среднее количество пробирований при вставке: %.2f\n", avg_probes_per_insert);
    printf("📌 Максимальная длина пробирования: %d\n", max_probe_length);

    printf("\n🔍 Индексы с наибольшими коллизиями:\n");
    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        if (collision_counts[i] > 0)
        {
            printf("  - [%2zu] Коллизии: %d\n", i, collision_counts[i]);
        }
    }

    printf("==================================================================================\n\n");
}

void hashTableDumpToFile(struct hashTable* hash_table, const char* filename)
{
    assert(hash_table);
    assert(filename);

    FILE* file = fopen(filename, "a");
    if (!file)
    {
        perror("Ошибка открытия файла");
        return;
    }

    double load_factor = (double)(hash_table->size / hash_table->capacity);
    int total_collisions  = 0;
    int total_probes      = 0;
    int inserted_elements = 0;
    int max_probe_length  = 0;

    int collision_counts[hash_table->capacity];
    memset(collision_counts, 0, sizeof(collision_counts));

    fprintf(file, "\n==================================================================================\n");
    fprintf(file, "                                Хеш-таблица (размер: %zu/%zu)\n", hash_table->size, hash_table->capacity);
    fprintf(file, "                                Load Factor: %.2f\n", load_factor);
    fprintf(file, "==================================================================================\n");
    fprintf(file, "| %-5s | %-20s | %-10s | %-10s | %-8s |\n", "Index", "Key", "Value", "Hash", "Probes");
    fprintf(file, "----------------------------------------------------------------------------------\n");

    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        if (hash_table->keys[i] == NULL)
        {
            fprintf(file, "| %-5zu | %-25s | %-10s | %-10s | %-8s |\n", i, "(пусто)", "-", "-", "-");
        }
        else if (hash_table->keys[i] == DELETED_ELEMENT)
        {
            fprintf(file, "| %-5zu | %-27s | %-10s | %-10s | %-8s |\n", i, "(удалено)", "-", "-", "-");
        }
        else
        {
            int hash = hash_table->hash_function(hash_table->keys[i], hash_table->capacity);
            int probes = 0;
            size_t index = hash % hash_table->capacity;
            int offset = 0;

            while (index != i)
            {
                offset++;
                switch (hash_table->probe)
                {
                    case LINEAR_PROBE:
                        index = (hash + offset) % hash_table->capacity;
                        break;
                    case QUADRATIC_PROBE:
                        index = (hash + offset * offset) % hash_table->capacity;
                        break;
                    case HASH_PROBE:
                        index = (hash + offset * (1 + (hash % (hash_table->capacity - 1)))) % hash_table->capacity;
                        break;
                }
                probes++;
            }

            if (probes > 0)
            {
                total_collisions++;
                collision_counts[i] = probes;
            }

            total_probes += probes;
            inserted_elements++;

            if (probes > max_probe_length)
            {
                max_probe_length = probes;
            }

            fprintf(file, "| %-5zu | %-20s | %-10d | %-10d | %-8d |\n", i, hash_table->keys[i], hash_table->values[i], hash, probes);
        }
    }

    fprintf(file, "----------------------------------------------------------------------------------\n");

    double avg_probing_length = (inserted_elements > 0) ? (double)total_probes / inserted_elements : 0;
    double avg_probes_per_insert = (hash_table->size > 0) ? (double)total_probes / hash_table->size : 0;

    fprintf(file, "\n📊  Статистика:\n");
    fprintf(file, "📌 Коллизии: %d\n", total_collisions);
    fprintf(file, "📌 Средняя длина пробирования: %.2f\n", avg_probing_length);
    fprintf(file, "📌 Среднее количество пробирований при вставке: %.2f\n", avg_probes_per_insert);
    fprintf(file, "📌 Максимальная длина пробирования: %d\n", max_probe_length);

    fprintf(file, "\n🔍 Индексы с наибольшими коллизиями:\n");
    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        if (collision_counts[i] > 0)
        {
            fprintf(file, "  - [%2zu] Коллизии: %d\n", i, collision_counts[i]);
        }
    }

    fprintf(file, "==================================================================================\n\n");
    if (fclose(file)) 
    {
        perror("Ошибка при закрытии файла");
    }
}

//============================================================================
int MurmurHashFunction(const char* key, int hash_table_capacity)
{
    assert(key);
    
    const uint32_t seed = 0x9747b28c;  // Фиксированное начальное значение
    const uint32_t m = 0x5bd1e995;      // Константа алгоритма MurmurHash
    const int r = 24;

    uint32_t hash = seed ^ strlen(key);
    const unsigned char* data = (const unsigned char*)key;

    while (*data) {
        uint32_t k = *data++;
        k *= m;
        k ^= k >> r;
        k *= m;

        hash *= m;
        hash ^= k;
    }

    hash ^= hash >> 13;
    hash *= m;
    hash ^= hash >> 15;

    return (int)(hash % hash_table_capacity);  // Приводим хеш к диапазону
}
//============================================================================
void hashTableIteratorInit(struct hashTableIterator* hash_table_iterator, struct hashTable* hash_table)
{
    assert(hash_table_iterator);
    assert(hash_table);

    hash_table_iterator->hash_table = hash_table;
    hash_table_iterator->index = 0;
}

long long int hashTableIteratorHasNext(struct hashTableIterator* hash_table_iterator)
{
    assert(hash_table_iterator);
    
    size_t index = hash_table_iterator->index + 1;
    while (index < hash_table_iterator->hash_table->capacity) 
    {
        if (hash_table_iterator->hash_table->keys[index] != NULL &&
            hash_table_iterator->hash_table->keys[index] != DELETED_ELEMENT) 
        {
            return index;
        }
        index++;
    }
    
    return -1; 
}

long long int hashTableIteratorHasPrev(struct hashTableIterator* hash_table_iterator)
{
    assert(hash_table_iterator);

    if (hash_table_iterator->index == 0) 
    {
        return -1;
    }

    size_t index = hash_table_iterator->index - 1;
    while (index > 0) 
    {
        if (hash_table_iterator->hash_table->keys[index] != NULL &&
            hash_table_iterator->hash_table->keys[index] != DELETED_ELEMENT) 
        {
            return index; 
        }
        index--;
    }
    
    if(index == 0)
    {
        if (hash_table_iterator->hash_table->keys[index] != NULL &&
            hash_table_iterator->hash_table->keys[index] != DELETED_ELEMENT) 
        {
            return index; 
        }
    }

    return -1;
}

const char* hashTableIteratorNext(struct hashTableIterator* hash_table_iterator, int* value)
{
    assert(hash_table_iterator);
    assert(value);
    
    long long int next_index = hashTableIteratorHasNext(hash_table_iterator);

    if(next_index != -1)
    {
        hash_table_iterator->index = next_index;
        *value = hash_table_iterator->hash_table->values[next_index];
        return hash_table_iterator->hash_table->keys[next_index];
    }

    return NULL;
}

const char* hashTableIteratorPrev(struct hashTableIterator* hash_table_iterator, int* value)
{
    assert(hash_table_iterator);
    assert(value);
    
    long long int prev_index = hashTableIteratorHasPrev(hash_table_iterator);

    if(prev_index != -1)
    {
        hash_table_iterator->index = prev_index;
        *value = hash_table_iterator->hash_table->values[prev_index];
        return hash_table_iterator->hash_table->keys[prev_index];
    }
    
    return NULL;
}

const char* hashTableIteratorGetByIndex(struct hashTableIterator* hash_table_iterator, size_t target_index, int* value)
{
    assert(hash_table_iterator);
    assert(value);

    if (target_index >= hash_table_iterator->hash_table->size) 
    {
        return NULL;
    }

    hash_table_iterator->index = 0;

    size_t count = 0;
    long long int index = hashTableIteratorHasNext(hash_table_iterator);

    while(index != -1)
    {
        if(count == target_index)
        {
            hash_table_iterator->index = index;
            *value = hash_table_iterator->hash_table->values[index];
            return hash_table_iterator->hash_table->keys[index];
        }

        hash_table_iterator->index = index;
        index = hashTableIteratorHasNext(hash_table_iterator);
        count++;
    }
    return NULL;
}
//============================================================================
int IsPrime(size_t n) 
{
    if (n < 2) 
    {
        return 0;
    }
    if (n == 2 || n == 3) 
    {
        return 1;
    }
    if (n % 2 == 0 || n % 3 == 0) 
    {
        return 0;
    }
    for (size_t i = 5; i * i <= n; i += 6) 
    {
        if (n % i == 0 || n % (i + 2) == 0) 
        {
            return 0;
        }
    }
    
    return 1;
}

size_t FindNearestPrime(size_t n, enum MODE_PRIME mode) 
{
    if (n < 2) return 2; 

    size_t lower = n, upper = n;

    while (1) 
    {
        if (mode == NEAREST_BIGGER)
        {
            if (IsPrime(upper)) 
            {
                return upper; 
            }
            upper++;
        }
        if (mode == NEAREST_LESSER)
        {
            if (IsPrime(lower)) 
            {
                return lower; 
            }
            lower = (lower > 2) ? lower - 1 : 2;
        }
    }
}
//============================================================================