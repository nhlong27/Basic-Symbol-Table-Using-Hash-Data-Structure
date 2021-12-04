#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
};


class HashTableEntry{
public:
    int k;
    int v;
    HashTableEntry(int k, int v){
        this->k = k;
        this->v = v;
    }
};

class HashTable {
private:
    HashTableEntry **t;
    int size;
public:
    HashTable(int size){
        this->size = size;
        t = new HashTableEntry*[size];
        for (int i = 0; i<size; i++){
            t[i] = nullptr;
        }
    }
    // int encode(string id){
    //     unsigned long a;
    //     unsigned long b = 0;
    //     for (int i = 0; id[i]; i++){
    //         a = b;
    //         b = id[i] - 48;
    //         int times = 1;
    //         while (times <= b){
    //             times *= 10;
    //         }
    //         a = a*times + b;
    //     }
    //     return a;
    // }
    int HashFunc(string id){
        unsigned long a;
        unsigned long b = 0;
        for (int i = 0; id[i]; i++){
            a = b;
            b = id[i] - 48;
            int times = 1;
            while (times <= b){
                times *= 10;
            }
            a = a*times + b;
        }
        return a % size;    
    }
    ~HashTable() {
        for (int i = 0; i < size; i++) {
        if (t[i] != nullptr)
            delete t[i];
            delete[] t;
        }
      }
};













#endif