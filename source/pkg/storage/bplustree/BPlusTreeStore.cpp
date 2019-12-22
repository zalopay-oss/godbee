#include "BPlusTreeStore.h"
#include<shared_mutex>
#include <mutex>
#include <thread>
#include <iostream>
#include <string>

using namespace std;

BPlusTreeStore::BPlusTreeStore(int customMaxDeg)
{
    string dbFileName = getDBFileName(CONFIG_FILE);
    if (customMaxDeg==0 && existFile(dbFileName))
    {
        hp = getHeaderPage(dbFileName);
        bplustree = new BPlusTree(hp,dbFileName.c_str(), false);
    }
    else
    {
        hp = initialFileWithConfig(dbFileName, CONFIG_FILE);
        bplustree = new BPlusTree(hp, dbFileName.c_str(),true,customMaxDeg);
    }
}

char *BPlusTreeStore::get(char *key)
{
    shared_lock<std::shared_mutex> lock(mutex_);
    return bplustree->get(key);
}
void BPlusTreeStore::set(char *key, char *value)
{
    std::unique_lock<std::shared_mutex> uniqueLock(mutex_);
    bplustree->set(key,value);
}
bool BPlusTreeStore::exist(char *key)
{
    shared_lock<std::shared_mutex> lock(mutex_);
    return bplustree->exist(key);
}
bool BPlusTreeStore::remove(char *key)
{
    unique_lock<std::shared_mutex> lock(mutex_);
    return bplustree->remove(key);

}

string BPlusTreeStore::traverse()
{
    shared_lock<std::shared_mutex> lock(mutex_);
    return bplustree->traverse();
}
BPlusTreeStore::~BPlusTreeStore()
{
    delete bplustree;
}