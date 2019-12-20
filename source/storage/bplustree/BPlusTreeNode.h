#pragma once
#include <string>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include "utils.h"
#include "fileUtils.h"
using namespace std;

struct Entry
{
    char *key = NULL;
    uint64_t value;
    Entry(char *_key, uint64_t _value) : key(_key), value(_value)
    {
    }
};

struct MetaData;
struct NodePage;

class BPlusTreeNode
{
private:
    uint64_t offSet;
    uint64_t *childs;
    char **keys;
    uint8_t flag;
    uint16_t size;

    MetaData *metaData;

    // Return null if not need to split, else return key and child to append to fathernode
    Entry *internalInsert(char *key, char *value);
    // Return null if not need to split, else return key and child to append to fathernode
    Entry *leafInsert(char *key, char *value);

    bool leafRemove(BPlusTreeNode *father, int nodeIndex, char *key);
    bool internalRemove(BPlusTreeNode *father, int nodeIndex, char *key);

    // Insert value to the left of the INTERNAL Key
    void insertLeft(char *key, char *lefValue);
    // Insert value to the right of the LEAF Key
    void insertRight(char *key, uint64_t rightValue);

    void removeLeft(char *key);
    void removeLeft(int index);

    Entry *insertAndSplitLeaf(char *newKey, char *newValue);
    Entry *insertAndSplitInternal(char *newKey, uint64_t rightChildValue);

    void redistributeFromLeaf(BPlusTreeNode *father, int nodeIndex);
    void redistributeFromInternal(BPlusTreeNode *father, int nodeIndex);
    BPlusTreeNode *copyPartOfNode(int8_t flag, int left, int right);

    void swapClockWiseItn(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *rightSibling);
    void swapCounterClockWiseItn(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *rightSibling);
    void swapClockWiseLeaf(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *rightSibling);
    void swapCounterClockWiseLeaf(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *rightSibling);

    void leftMergeItn(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *leftSibling);
    void rightMergeItn(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *leftSibling);
    void leftMergeLeaf(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *leftSibling);
    void rightMergeLeaf(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *leftSibling);

    BPlusTreeNode *getNode(uint64_t index);
    char *getValue(uint64_t offSet);

    void flushToFile();
    void appendToFile();
    void updateValueDuplicatedKey(int index, char *value);
    void markDelete();
    NodePage *toNodePage();

public:
    static const uint8_t LEAF_NODE = 0x00;
    static const uint8_t INTERNAL_NODE = 0x01;
    static const uint8_t REMOVED_NODE = 0x03;

    BPlusTreeNode(MetaData *_metaData, uint8_t flag);
    BPlusTreeNode(char *key, BPlusTreeNode *leftChild, BPlusTreeNode *rightChild);
    BPlusTreeNode(MetaData *_metaData, NodePage *_nodePage, uint64_t _offSet);

    uint64_t getOffset();
    uint8_t getFlag();

    BPlusTreeNode *insert(char *key, char *value);
    BPlusTreeNode *remove(char *key);
    char *get(char *key);
    bool exist(char *key);

    void leavesTraverse();
    string traverse();
    void clearChilds();

    ~BPlusTreeNode();
};
