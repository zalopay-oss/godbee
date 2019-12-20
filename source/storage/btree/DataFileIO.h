#pragma once
#include "stdio.h"
#include "KVstruct.h"
#include <fstream>

class BTreeNode;

class BTree;

struct BTreeNodeStore;

struct BTreeStore;

using namespace std;

const string configFileName = "./configs/.configB";

KV* getEntry(BTree* tree, int idx);
V readValueInFile(BTree* tree, uint64_t valuePos, FILE* f);
void updateValueAt(BTree* tree, uint64_t valuePos, V newValue, bool isOverWrite, FILE* f);
void WRITE_NODE(BTree* b, BTreeNode* node, uint64_t pos, FILE* f);
BTreeNode* READ_NODE(BTree* b, BTreeNode* node, uint64_t pos, FILE* f);
void WRITE_HEADER_INIT();
bool checkFileDBExist();
void WRITE_HEADER(BTree* tree);
struct BTreeStore* READ_HEADER(BTree* b);
void writeEmptyBytes(BTree* b, uint64_t pos, FILE* f);
void getConfigValues(BTreeStore* treeMetaData);
void flushDeletedNodesFile(BTree* tree);
void flushDeletedValsFile(BTree* tree);
void readDeletedNodesPos(BTree* tree);
void readDeletedValsPos(BTree* tree);
void clearDataDBFile(BTree* tree);
void clearDBFile(BTree* tree);
void clearDeletedNodesFile(BTree* tree);
void clearDeletedValsFile(BTree* tree);
string getDBFileNameInConfig();
