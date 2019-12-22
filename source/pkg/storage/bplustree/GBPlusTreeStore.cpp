#include "BPlusTreeStore.h"
#include "GBPlusTreeStore.h"
#include <iostream>

using namespace std;
GBPlusTreeStore GBPlusInit()
{
    BPlusTreeStore * btree= new BPlusTreeStore();
    return (void*)btree;
}
void GBPlusFree(GBPlusTreeStore btree)
{
    BPlusTreeStore *pTree = (BPlusTreeStore *)btree;
    delete pTree;
}
void GBPlusSet(GBPlusTreeStore btree, char *key, char *value)
{
    BPlusTreeStore *pTree = (BPlusTreeStore *)btree;
    pTree->set(key, value);
}
char *GBPlusGet(GBPlusTreeStore btree, char *key)
{
    BPlusTreeStore *pTree = (BPlusTreeStore *)btree;
    return pTree->get(key);
}
int GBPlusRemove(GBPlusTreeStore btree, char *key)
{
    BPlusTreeStore *pTree = (BPlusTreeStore *)btree;
    return pTree->remove(key);
}
int GBPlusExist(GBPlusTreeStore btree, char *key)
{
    BPlusTreeStore *pTree = (BPlusTreeStore *)btree;
    return pTree->exist(key);
}
void GBPlusTraverse(GBPlusTreeStore btree)
{
    BPlusTreeStore *pTree = (BPlusTreeStore *)btree;
    pTree->traverse();
}