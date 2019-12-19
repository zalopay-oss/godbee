#pragma once
#include "Comparator.h"
#include "DataFileIO.h"
#include "utils.h"
#include <queue>

using namespace std;

class BTree;

struct BTreeNodeStore {
    uint64_t* arrChild;
    uint64_t* valuePos;
    K* arrEntry;
    uint8_t flag; // leaf, deleted, root
    uint16_t nEntry;
    uint32_t pos;

    BTreeNodeStore(uint16_t T, int keySize) {
        arrChild = new uint64_t[2 * T];
        valuePos = new uint64_t[2 * T - 1];
        arrEntry = new K[2 * T - 1];
        for(int i = 0; i < 2 * T - 1; i++) {
            arrEntry[i] = newKey(keySize);
        }
    }

};


struct BTreeNodeStoreTmp {
    uint64_t valuePos[77];
    uint64_t arrChild[78];
    char arrEntry[37][77];
    uint8_t flag; // leaf, deleted, root
    uint16_t nEntry;
    uint32_t pos;
};

class BTreeNode {
private:
    uint16_t nEntry;
    K* arrEntry = NULL;
    uint64_t* valuePos = NULL;
    uint64_t* arrChild = NULL;
    uint16_t bufferKeySize;
    uint8_t flag = 0; // leaf, deleted, root
    uint16_t T;
    uint32_t pos;
public:
    static const uint8_t ROOT = 0x0;
    static const uint8_t LEAF = 0x1;
    static const uint8_t INTERNAL = 0x2;
    static const uint8_t DELETED = 0x3;
    static const int EMPTY_SLOT = 0;
    BTreeNode(){}
    ~BTreeNode();
    BTreeNode(bool isLeaf, int maxDegree, int keySize);
    BTreeNode(int maxDegree, int keySize);
    void insertNonFull(BTree* tree, KV *entry, FILE* f);
    void splitChild(BTree* tree, int idx, BTreeNode* child, FILE* f);
    void borrowFromPrevNode(BTree* tree, int idx, FILE* f);
    void borrowFromNextNode(BTree* tree, int idx, FILE* f);
    void mergeNodes(BTree* tree, int idx, FILE* f);
    void fillNode(BTree* tree, int idx, FILE* f);
    BTreeNode* getPredNode(BTree* tree, int idx, FILE* f);
    BTreeNode* getSuccNode(BTree* tree, int idx, FILE* f);
    bool remove(BTree* tree, K k, bool isDeleteReal, FILE* f);
    bool removeFromLeaf(BTree* tree, int idx, bool isDeleteReal, FILE* f);
    bool removeFromNonLeaf(BTree* tree, int idx, FILE* f);
    void traverse(FILE* f);
    void traverseByLevel();
    void clearNode();
    void clearEntry(KV* entry);
    void markLeaf();
    void markInternal();
    void markDeleted();
    KV* getEntry(BTree* tree, int idx, FILE* file);
    V search(BTree* tree, K k, FILE* file);
    V getValueInNode(BTree* tree, K key, FILE* file);
    K* getEntryList() {
        return this->arrEntry;
    }
    uint16_t getBufferKeySize() {
        return this->bufferKeySize;
    }
    uint16_t getEntryListSize() {
        return this->nEntry;
    }
    void setMaxDegree(uint16_t maxDegree) {
        this->T = maxDegree;
    }
    uint16_t getMaxDegree() {
        return this->T;
    }
    uint8_t getFlag() {
        return this->flag;
    }
    uint32_t getNodePos() {
        return this->pos;
    }
    uint16_t getNumEntry() {
        return this->nEntry;
    }
    uint64_t* getValuePos() {
        return this->valuePos;
    }
    K* getEntryKeyList() {
        return this->arrEntry;
    }
    uint64_t* getChildPos() {
        return this->arrChild;
    }
    void setFlag(uint8_t f) {
        this->flag = f;
    }
    void setNodePos(uint32_t p) {
        this->pos = p;
    }
    void setBufferKeySize(uint16_t keySize) {
        this->bufferKeySize = keySize;
    }
    void setNumEntry(uint16_t num) {
        this->nEntry = num;
    }
    void setValuePos(uint64_t* values) {
        if(this->valuePos == NULL) {
            this->valuePos = new uint64_t[2 * T - 1];
        }
        memcpy(this->valuePos, values, (2 * T - 1) * sizeof(uint64_t));
    }
    void setEntryKeyList(K* entries) {
        if(this->arrEntry == NULL) {
            this->arrEntry = new K[2 * T - 1];
        }
        for(int i = 0; i < 2 * T - 1; i++) {
            this->arrEntry[i] = newKey(this->bufferKeySize);
            strcpy(this->arrEntry[i], entries[i]);
        }
    }
    void setChildPos(uint64_t* childs) {
        if(this->arrChild == NULL) {
            this->arrChild = new uint64_t[2 * T];
        }
        memcpy(this->arrChild, childs, (2 * this->T) * sizeof(uint64_t));
    }

friend class BTree;
};
