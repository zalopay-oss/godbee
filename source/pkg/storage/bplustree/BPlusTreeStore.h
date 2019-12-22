#pragma once

#include "BPlusTree.h"
#include "KeyValueStore.h"
#include<shared_mutex>
#include <mutex>

using namespace std;
const string CONFIG_FILE = "./configs/.configBPlus";
const int NO_WORKERS = 8;
class BPlusTreeStore: public KeyValueStore {
private:
    BPlusTree *bplustree;
    HeaderPage hp;
    shared_mutex mutex_;
public:
    BPlusTreeStore(int maxDegree=0);

    char* get(char* key);
    void set(char* key, char *value);
    bool exist(char* key);
    bool remove(char* key);

    string traverse();
    ~BPlusTreeStore();
};