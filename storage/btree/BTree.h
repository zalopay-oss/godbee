#pragma once
#include "DataFileIO.h"
#include <queue>
#include <map>
#include <shared_mutex>
#include <mutex>
#include <thread>

using namespace std;

// const string dbFileName = "dataTest/bTreeDatabase";

class BTreeNode;
class FileManager;

struct BTreeStore {
    uint64_t rootPos;
    uint64_t nextPosNode;
    uint64_t nextPosNodeValue;
    char deletedNodesFileName[32];
    char deletedValuesFileName[32];
    char dbFileName[32];
    uint16_t bufferSize;
    uint16_t pageSize;
    uint16_t valueSize;
    uint32_t T;
};

class BTree {
private:
    BTreeNode* root;
    uint16_t T;
    uint16_t bufferSize;
    uint16_t pageSize;
    uint16_t valueSize;
    uint64_t rootPos;
    uint64_t nextPosNode;
    uint64_t nextPosNodeValue; // 2 bit cuoi se la max offset hien tai cua value trong page
    char deletedNodesFileName[32];
    char deletedValuesFileName[32];
    string dbFileName;
    FileManager* fileManager;
protected:
    queue<uint64_t> deletedNodesPos;
    queue<uint64_t> deletedValsPos;
public:
    BTree();
    void insert(KV* entry);
    bool remove(K k);
    void traverse();
    void traverseByLevel();
    V search(K k);
    void clear();
    void updateNextPosNode(FILE* file);
    void updateNextPosNodeValue(FILE* file);
    int getHeaderSize() {
        return sizeof(BTreeStore);
    }
    void createNewFilePtr();
    FileManager* getFileManager() {
        return this->fileManager;
    }
    void setRootPost(uint64_t pos) {
        this->rootPos = pos;
    }
    void setRootNode(BTreeNode* r) {
        this->root = r;
    }
    uint16_t getBufferSize() {
        return this->bufferSize;
    }
    uint32_t getMaxDegree() {
        return this->T;
    }
    uint64_t getNextPosNode() {
        return this->nextPosNode;
    }
    uint64_t getNextPosNodeValue() {
        return this->nextPosNodeValue;
    }
    uint64_t getRootPos() {
        return this->rootPos;
    }
    FILE* getFilePtr();
    uint16_t getPageSize() {
        return this->pageSize;
    }
    uint16_t getValueSize() {
        return this->valueSize;
    }
    char* getDeletedNodesFileName() {
        char* fileName = new char[32];
        strcpy(fileName, this->deletedNodesFileName);
        return fileName;
    }
    char* getDeletedValuesFileName() {
        char* fileName = new char[32];
        strcpy(fileName, this->deletedValuesFileName);
        return fileName;
    }
    string getDBFileName() {
        return this->dbFileName;
    }
    void setNextPosNodeValue(uint64_t pos) {
        this->nextPosNodeValue = pos;
    }
    queue<uint64_t> getDeletedNodesPos() {
        return this->deletedNodesPos;
    }
    queue<uint64_t> getDeletedValsPos() {
        return this->deletedValsPos;
    }
    void setDeletedNodesPos(queue<uint64_t> q) {
        this->deletedNodesPos = q;
    }
    void setDeletedValsPos(queue<uint64_t> q) {
        this->deletedValsPos = q;
    }
    ~BTree();
friend class BTreeNode;
};
