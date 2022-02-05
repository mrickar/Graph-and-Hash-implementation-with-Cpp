#include "HashTable.h"
using namespace std;
int main()
{
    KeyedHashTable ht;
    ht.Insert("A", std::vector<int>());
    ht.Insert("X", std::vector<int>());
    ht.Insert("U", std::vector<int>());
    ht.Insert("T", std::vector<int>());
    ht.Insert("I", std::vector<int>());
    ht.Insert("A", std::vector<int>());


    KeyedHashTable ht2;
    ht2=ht;

    
    ht2.Print();

}