#include "BPlusTree.h"

BPlusTree::BPlusTree(HeaderPage& hp,const char *filename,  bool isNewTree, uint16_t customMaxDeg )
:headerPage(hp)
{
    metaData = new MetaData(&hp,isNewTree);
    metaData->maxDegree = customMaxDeg>1?customMaxDeg:metaData->maxDegree;
    if (metaData->maxDegree <=1 || metaData->bSizeOfValueIdx==0){
        metaData->keySize = DEFAULT_KEY_SIZE;
        metaData->valueSize = DEFAULT_VALUE_SIZE;
        metaData->pageSize = DEFAULT_PAGE_SIZE;
        updateHeader(hp,metaData);
        metaData->maxDegree = calculateMaxDegree(metaData->pageSize, metaData->keySize, sizeof(uint64_t));
        metaData->bSizeOfValueIdx = calculateBitSizeValueIndex(metaData->pageSize,metaData->valueSize);
        cout<< "================================================================="<<endl;
        cout << "SOMETHINGS WENT WRONG WITH INITIALIZING B+TREE BY CONFIG FILE.\n" << endl;
        cout<< "USE DEFAULT CONFIG INSTEAD\n";
        cout<< "\tPAGE SIZE: "<< metaData->pageSize<<endl;
        cout<< "\tKEY SIZE: "<< metaData->keySize<<endl;
        cout<< "\nVALUE SIZE: "<< metaData->valueSize<<endl;
        cout<< "================================================================="<<endl;
    }
    root = (!isNewTree)?getBNodeFromFile(metaData,hp.root):new BPlusTreeNode(metaData,BPlusTreeNode::LEAF_NODE);
}

string BPlusTree::traverse()
{
    if (root)
    {
        return root->traverse();
    }
    return "";
}

void BPlusTree::set(char *_key, char *_value)
{
    char * key = new char[metaData->keySize];
    strcpy(key,_key);
    char * value = new char[metaData->valueSize];
    strcpy(value,_value);
    BPlusTreeNode * newRoot = root->insert(key, value);
    if (newRoot != root){
        delete root;
        root =newRoot;
        updateRoot(root->getOffset(),headerPage,metaData);
    }
    delete[] key;
    delete[] value;
}

char *BPlusTree::get(char *key)
{
    return root->get(key);
}

bool BPlusTree::exist(char *key)
{
    return root->exist(key);
}

bool BPlusTree::remove(char *key)
{
    if (!root){
        return false;
    }
    BPlusTreeNode *result = root->remove(key);
    if (result && result != root)
    {
        root->clearChilds();
        delete(root);
        root = result;
        updateRoot(root->getOffset(),headerPage,metaData);

    }
    return (result != NULL);
}

BPlusTree::~BPlusTree()
{
    delete root;
    delete metaData;
}
