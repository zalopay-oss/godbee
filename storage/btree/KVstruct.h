#pragma once
#include <string.h>
#include <iostream>

using namespace std;

typedef char* K;
typedef char* V;

class KV {
private:
    K key = NULL;
    V value = NULL;
public:
    KV(){
        key = NULL;
        value = NULL;
    }
    KV(K &k, V &v, int keySize, int valueSize) {
        this->key = new char[keySize];
        this->value = new char[valueSize];
        strcpy(this->key, k);
        strcpy(this->value, v);
    }
    void setKey(K &k, int keySize) {
        this->key = new char[keySize];
        strcpy(this->key, k);
        delete[] k;
    }
    void setValue(V &v, int valueSize) {
        this->value = new char[valueSize];
        strcpy(this->value, v);
        delete[] v;
    }
    char* getKey() {
        return this->key;
    }
    char* getValue() {
        return this->value;
    }
    ~KV() {
        if(this->key != NULL)
            delete[] this->key;
        if(this->value != NULL)
            delete[] this->value;
        this->key = NULL;
        this->value = NULL;
    }
};
