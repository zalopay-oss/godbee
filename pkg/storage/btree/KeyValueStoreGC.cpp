#include "KeyValueStoreGC.h"
#include "KeyValueStore.h"

KeyValueStoreDiskGC BTreeInit() {
    KeyValueStore* pTree = new KeyValueStoreDisk();
    pTree->INIT_STORE();
    return (void*)pTree;
}

const char* BTreeGet(KeyValueStoreDiskGC btree, K key) {
    KeyValueStoreDisk* pTree = (KeyValueStoreDisk*) btree;
    return pTree->get(key);
}

void BTreeSet(KeyValueStoreDiskGC btree, K key, V value) {
    KeyValueStoreDisk* pTree = (KeyValueStoreDisk*) btree;
    pTree->set(key, value);
}

int BTreeExist(KeyValueStoreDiskGC btree, K key) {
    KeyValueStoreDisk* pTree = (KeyValueStoreDisk*) btree;
    bool res = pTree->exist(key);
    return (res) ? 1 : 0;
}

int BTreeRemove(KeyValueStoreDiskGC btree, K key) {
    KeyValueStoreDisk* pTree = (KeyValueStoreDisk*) btree;
    bool res = pTree->remove(key);
    return (res) ? 1 : 0;
}

void BTreeTraverse(KeyValueStoreDiskGC btree) {
    KeyValueStoreDisk* pTree = (KeyValueStoreDisk*) btree;
    pTree->traverse();
}

void BTreeFree(KeyValueStoreDiskGC btree) {
    KeyValueStoreDisk* pTree = (KeyValueStoreDisk*) btree;
    cout << "DESTRUCTOR CALLED" << endl;
    delete pTree;
}