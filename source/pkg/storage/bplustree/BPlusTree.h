#pragma once
#include <string>
#include <iostream>
#include "BPlusTreeNode.h"
#include "fileUtils.h"
#include <thread>

using namespace std;
class BPlusTree {
private:
    int maxDegree;
    HeaderPage& headerPage;
    MetaData *metaData;
    BPlusTreeNode* root;
    const uint16_t DEFAULT_MAX_DEG = 48;
    const uint16_t DEFAULT_PAGE_SIZE = 4096;
    const uint16_t DEFAULT_KEY_SIZE = 35;
    const uint16_t DEFAULT_VALUE_SIZE = 1024;
public:
    BPlusTree(HeaderPage& headerPage,const char *filename, bool isNewTree=true, uint16_t customMaxDeg = 0);
    char* get( char* key);
    void set(char* key, char *value);
    bool exist(char* key);
    bool remove(char* key);
    string traverse();
    ~BPlusTree();
};