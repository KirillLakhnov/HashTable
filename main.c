#include "tests/test_hash_table.h"
#include "tests/test_iterator_hash_table.h"
#include "tests/test_of_speed_for_probing.h"

int main() 
{
    AllTestsHashTable();
    AllTestsIteratorHashTable();

    //TestOfSpeed();

    return 0;
}