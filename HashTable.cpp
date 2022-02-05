#include "HashTable.h"

#include <cassert>
#include <iostream>
#include <iomanip>

const int KeyedHashTable::PRIME_LIST[PRIME_TABLE_COUNT] =
    {
        2, 3, 5, 7, 11, 13, 17, 19,
        23, 29, 31, 37, 41, 43, 47, 53,
        59, 61, 67, 71, 73, 79, 83, 89,
        97, 101, 103, 107, 109, 113, 127, 131,
        137, 139, 149, 151, 157, 163, 167, 173,
        179, 181, 191, 193, 197, 199, 211, 223,
        227, 229, 233, 239, 241, 251, 257, 263,
        269, 271, 277, 281, 283, 293, 307, 311,
        313, 317, 331, 337, 347, 349, 353, 359,
        367, 373, 379, 383, 389, 397, 401, 409,
        419, 421, 431, 433, 439, 443, 449, 457,
        461, 463, 467, 479, 487, 491, 499, 503,
        509, 521, 523, 541};

int KeyedHashTable::Hash(const std::string &key) const
{
    // TODO
    int ln = key.length();
    int code = 0;
    for (int i = 0; i < ln; i++)
    {
        code = (code + key[i] * PRIME_LIST[i]) % tableSize;
    }
    return code;
}

void KeyedHashTable::ReHash()
{
    // TODO
    int oldSize = tableSize;
    while (occupiedElementCount * EXPAND_THRESHOLD >= tableSize)
    {
        tableSize = FindNearestLargerPrime(2 * tableSize);
    }
    HashData *oldTable = table;
    table = new HashData[tableSize];
    occupiedElementCount = 0;
    for (int i = 0; i < oldSize; i++)
    {
        if (oldTable[i].key != "" && oldTable[i].key != "__DELETED__")
        {
            Insert(oldTable[i].key, oldTable[i].intArray);
        }
    }
    delete[] oldTable;
}

int KeyedHashTable::FindNearestLargerPrime(int requestedCapacity)
{
    // TODO
    for (int i = 0; i < PRIME_TABLE_COUNT; i++)
    {
        if (PRIME_LIST[i] > requestedCapacity)
        {
            return PRIME_LIST[i];
        }
    }
}

KeyedHashTable::KeyedHashTable()
{
    // TODO
    table = new HashData[2];
    tableSize = 2;
    occupiedElementCount = 0;
}

KeyedHashTable::KeyedHashTable(int requestedCapacity)
{
    // TODO
    tableSize = FindNearestLargerPrime(requestedCapacity);
    table = new HashData[tableSize];
    occupiedElementCount = 0;
}

KeyedHashTable::KeyedHashTable(const KeyedHashTable &other)
{
    // TODO
    tableSize = other.tableSize;

    table = new HashData[tableSize];

    occupiedElementCount = 0;

    for (int i = 0; i < tableSize; i++)
    {

        if (other.table[i].key != "" && other.table[i].key != "__DELETED__")
        {
            Insert(other.table[i].key, other.table[i].intArray);
        }
    }
}

KeyedHashTable &KeyedHashTable::operator=(const KeyedHashTable &other)
{
    // TODO
    if (this == &other)
        return *this;
    tableSize = other.tableSize;
    delete[] table;
    table = new HashData[tableSize];
    occupiedElementCount = 0;
    for (int i = 0; i < tableSize; i++)
    {
        if (other.table[i].key != "" && other.table[i].key != "__DELETED__")
        {
            Insert(other.table[i].key, other.table[i].intArray);
        }
    }
    return *this;
}

KeyedHashTable::~KeyedHashTable()
{
    // TODO
    delete[] table;
}

bool KeyedHashTable::Insert(const std::string &key,
                            const std::vector<int> &intArray)
{
    // TODO
    std::vector<int> tmp;
    if (Find(tmp, key))
        return false;
    int code = Hash(key);
    int cnt = 0;
    while (table[(code + (cnt * cnt)) % tableSize].key != "" && table[(code + (cnt * cnt)) % tableSize].key != "__DELETED__")
    {
        cnt = (cnt + 1);
    }
    table[(code + (cnt * cnt)) % tableSize].key = key;
    table[(code + (cnt * cnt)) % tableSize].intArray = intArray;
    occupiedElementCount++;
    if ((occupiedElementCount * EXPAND_THRESHOLD >= tableSize))
    {
        ReHash();
    }
    return true;
}

bool KeyedHashTable::Remove(const std::string &key)
{
    // TODO
    int code = Hash(key);
    int cnt = 0;
    while (table[(code + (cnt * cnt)) % tableSize].key != key)
    {
        cnt = (cnt + 1);
        if (table[(code + (cnt * cnt)) % tableSize].key == "")
            return false;
    }
    table[(code + (cnt * cnt)) % tableSize].key = "__DELETED__";
    table[(code + (cnt * cnt)) % tableSize].intArray.clear();
    occupiedElementCount--;
    return true;
}

void KeyedHashTable::ClearTable()
{
    // TODO
    for (int i = 0; i < tableSize; i++)
    {
        table[i].key = "";
        table[i].intArray.clear();
    }

    occupiedElementCount = 0;
}

bool KeyedHashTable::Find(std::vector<int> &valueOut,
                          const std::string &key) const
{
    // TODO
    int code = Hash(key);
    int cnt = 0;
    while (table[(code + (cnt * cnt)) % tableSize].key != key)
    {
        cnt = (cnt + 1);
        if (table[(code + (cnt * cnt)) % tableSize].key == "")
            return false;
    }
    valueOut = table[(code + (cnt * cnt)) % tableSize].intArray;
    return true;
}

void KeyedHashTable::Print() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    std::cout << "HT:";
    if (occupiedElementCount == 0)
    {
        std::cout << " Empty";
    }
    std::cout << "\n";
    for (int i = 0; i < tableSize; i++)
    {
        if (table[i].key == "")
            continue;

        std::cout << "[" << std::setw(3) << i << "] ";
        std::cout << "[" << table[i].key << "] ";
        std::cout << "[";
        for (size_t j = 0; j < table[i].intArray.size(); j++)
        {
            std::cout << table[i].intArray[j];
            if ((j + 1) != table[i].intArray.size())
                std::cout << ", ";
        }
        std::cout << "]\n";
    }
    std::cout.flush();
}