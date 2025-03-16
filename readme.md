# О проекте

В этом проекте реализуем **хеш-таблицу с открытой адресацией**.

## Немного теории

**Хеш-таблица** — это структура данных, которая используется для хранения и быстрого поиска пар «ключ-значение». Она основана на хеш-функции, которая преобразует ключ в индекс массива, где будет храниться соответствующее значение.

### Основные принципы работы

1. Хеш-функция берет ключ и вычисляет по нему хеш-значение (обычно это число).
2. Хеш-значение определяет индекс массива, где будет храниться значение.
3. Коллизии (когда два разных ключа дают одинаковый хеш) разрешаются с помощью:
	* Метода цепочек (связывание значений в списки).
	* Открытой адресации (поиск другого свободного места в массиве по определённому алгоритму пробирования).

Существует три основных метода пробирования:
* Линейное пробирование
    * Каждым шаг увеличивает индекс массива на единицу (+1, +2, +3…).
    * Плюсы: Простая реализация, быстрая на практике.
    * Минусы: Образуется кластеризация (группы занятых ячеек → замедляет поиск).
```c
    index = (index + offset) % hash_table->capacity;
```
* Квадратичное пробирование
    * Каждым шаг увеличивает индекс массива по квадратичной формуле (+1², +2², +3²…).
    * Плюсы: Уменьшает кластеризацию.
    * Минусы: Если размер таблицы не простое число, может не найти свободную ячейку.
```c
    index = (index + offset*offset) % hash_table->capacity;
```
* Двойное хеширование
    * Используем вторую хеш-функцию для расчета шага.
    * Плюсы: Нет кластеризации, хороший равномерный поиск.
    * Минусы: Медленнее, чем линейное пробирование.
```c
    int hash1 = hash_table->hash_function(key, hash_table->capacity);
    int hash2 = 1 + (hash1 % (hash_table->capacity - 1));

    index = (hash1 + offset*hash2) % hash_table->capacity;
```
### Хеш-функция Murmur

Как работает алгоритм хеширование Murmur:
* Использует фиксированный seed (начальное значение), чтобы хеширование было детерминированным.
* Проходится по всем символам строки, применяя перемножение и сдвиги, чтобы обеспечить хорошее распределение.
* Применяет финальное перемешивание, уменьшая влияние схожих ключей.
* Ограничивает результат полученного хеша, чтобы индекс не выходил за границы таблицы.

#TODO

* ДОПИСАТЬ РЕДМИ И СРАВНИТЬ АЛГОРИТМЫ ПРОБИРОВАНИЯ