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
    string v;
    HashTableEntry(int k, string v){
        this->k = k;
        this->v = v;
    }
};

class HashTable {
private:
    HashTableEntry **t;
    int size;
    int c1;
    int c2;
public:
    HashTable(int size, int c1, int c2){
        this->size = size;
        this->c1 = c1;
        this->c2 = c2;
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
    int encode(string id){
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
        return a;
    }
    int HashFunc(int k){
        return k % size;    
    }
    void Insert(int k, string v) {
        int h = HashFunc(k);
        while (t[h] != NULL && t[h]->k != k) {
            h = HashFunc(h + 1);
        }
        if (t[h] != NULL) delete t[h];
        t[h] = new HashTableEntry(k, v);
    }
    void print(){
        for (int i = 0; i<size; i++){
            if (t[i]) cout<<i<<" "<<t[i]->v<<endl;
        }
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