#pragma once
#include "utils.h"
#include "BTree.h"
#include <shared_mutex>
#include <mutex>
#include <thread>

using namespace std;

class KeyValueStore {
public:
    virtual void INIT_STORE() = 0;
    virtual const V get(K key) = 0;
    virtual void set(K key, V value) = 0;
    virtual bool exist(K key) = 0;
    virtual bool remove(K key) = 0;
    virtual ~KeyValueStore() {

    }
};

class KeyValueStoreDisk : public KeyValueStore {
private:
    BTree* database;
    shared_mutex mutex;
public:
    // KeyValueStoreDisk() {
    //     database = new BTree();
    // }
    void INIT_STORE();
    const V get(K key);
    void set(K key, V value);
    bool exist(K key);
    bool remove(K key);
    void traverse();
    BTree* getDatabase(){
        return this->database;
    }
    ~KeyValueStoreDisk() {
        delete database;
    }
};
