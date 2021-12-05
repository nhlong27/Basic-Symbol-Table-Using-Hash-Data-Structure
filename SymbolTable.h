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
    int rebound = 1;
    int scope;
    string type = "none";
    int para = 0;
    HashTableEntry(int k, string v){
        this->k = k;
        this->v = v;
    }
};

class HashTable {
public:
    HashTableEntry **t;
    int size;
    int c1;
    int c2;
    int scope;
    string probeType;
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
        int times2 = 1;
        while (times2 <= a){
            times2 *= 10;
        }
        a = scope*times2 + a;
        return a;
    }
    int HashFunc(int k){
        return k % size;    
    }
    void Insert(HashTableEntry *temp) {
        int h = HashFunc(temp->k);
        while (t[h] != nullptr && t[h]->k != temp->k) {
            if (probeType == "LINEAR"){
                h = HashFunc(h + temp->rebound*c1);
            }
            else if (probeType == "QUADRATIC"){
                h = HashFunc(h + temp->rebound*c1 + temp->rebound*temp->rebound*c2);
            }
            else if (probeType == "DOUBLE"){
                int h2 = 1 + (temp->k % (size - 2));
                h = HashFunc(h + temp->rebound*c1*h2);
            }
            ++(temp->rebound);
        }
        if (t[h] != nullptr) delete t[h];
        t[h] = temp;
        temp->scope = scope;
        // cout<<D;
    }
    int SearchKey(HashTableEntry *temp) {
        int h = HashFunc(temp->k);
        while (t[h] != nullptr && t[h]->k != temp->k) {
            if (probeType == "LINEAR"){
                h = HashFunc(h + temp->rebound*c1);
            }
            else if (probeType == "QUADRATIC"){
                h = HashFunc(h + temp->rebound*c1 + temp->rebound*temp->rebound*c2);
            }
            else if (probeType == "DOUBLE"){
                int h2 = 1 + (temp->k % (size - 2));
                h = HashFunc(h + temp->rebound*c1*h2);
            }
            ++(temp->rebound);
        }
        if (t[h] == nullptr)
            return -1;
        else
            return h;
    }
    void print(){
        for (int i = 0; i<size; i++){
            if (t[i]) cout<<i<<" "<<t[i]->v<<"//"<<t[i]->scope<<";";
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