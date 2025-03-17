#include "test_hash_table.h"
#include "test_iterator_hash_table.h"
#include "test_of_speed_for_probing.h"

int main() 
{
    AllTestsHashTable();
    AllTestsIteratorHashTable();

    //TestOfSpeed();

    return 0;
}