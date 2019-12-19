#include "KeyValueStore.h"
#include "FileManager.h"

void KeyValueStoreDisk::INIT_STORE() {
    if(!checkFileDBExist()) {
        WRITE_HEADER_INIT();
        database = new BTree();
    }
    else {
        database = new BTree();
        FileManager* fileManager = database->getFileManager();
        FILE* file = fileManager->getFilePtr();
        BTreeNode* root = READ_NODE(database, NULL, database->getRootPos(), file);
        database->setRootNode(root);
        cout << "ROOT POS: " << database->getRootPos() << endl;
        fileManager->giveFilePtrBack(file);
    }
}
const V KeyValueStoreDisk::get(K key) {
    std::shared_lock<std::shared_mutex> lock(mutex);
    // database->createNewFilePtr();
    return database->search(key);
}

void KeyValueStoreDisk::set(K key, V value) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    // database->createNewFilePtr();
    KV* kv = new KV(key, value, database->getBufferSize(), database->getValueSize());
    database->insert(kv);
    delete kv;
}

bool KeyValueStoreDisk::exist(K key) {
    std::shared_lock<std::shared_mutex> lock(mutex);
    // database->createNewFilePtr();
    return (database->search(key) != NULL) ? true : false;
}

bool KeyValueStoreDisk::remove(K key) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    // database->createNewFilePtr();
    return database->remove(key);
}

void KeyValueStoreDisk::traverse() {
    std::shared_lock<std::shared_mutex> lock(mutex);
    // database->createNewFilePtr();
    database->traverseByLevel();
}