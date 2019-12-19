#include "BPlusTreeNode.h"
#include "fileUtils.h"
#include "utils.h"

using namespace std;

BPlusTreeNode::BPlusTreeNode(MetaData *_metaData, uint8_t _flag) : metaData(_metaData), flag(_flag)
{
    childs = new uint64_t[2 * _metaData->maxDegree];
    keys = new char *[2 * _metaData->maxDegree - 1];
    size = 0;

    for (int i = 0; i < 2 * _metaData->maxDegree; i++)
    {
        if (i < 2 * _metaData->maxDegree - 1)
        {
            keys[i] = NULL;
        }
        childs[i] = 0;
    }
    this->appendToFile();
}

BPlusTreeNode::BPlusTreeNode(char *key, BPlusTreeNode *leftChild, BPlusTreeNode *rightChild)
    : BPlusTreeNode(leftChild->metaData, INTERNAL_NODE)
{
    keys[0] = sdup(key, leftChild->metaData->keySize);
    childs[0] = leftChild->offSet;
    childs[1] = rightChild->offSet;
    size = 1;
}

void BPlusTreeNode::leavesTraverse()
{
    if (flag == LEAF_NODE)
    {
        for (int i = 0; i < size; i++)
        {
            cout << keys[i] << ":" << getValue(childs[i]) << "  ";
        }
        cout << " --> ";

        if (childs[size])
        {
            BPlusTreeNode *nextNode = getNode(childs[size]);
            nextNode->leavesTraverse();
            delete nextNode;
        }
    }
    else
    {
        BPlusTreeNode *nextNode = getNode(childs[0]);
        nextNode->leavesTraverse();
        delete nextNode;
    }
}

BPlusTreeNode::BPlusTreeNode(MetaData *_metaData, NodePage *_nodePage, uint64_t _offSet)
{
    childs = new uint64_t[2 * _metaData->maxDegree];
    keys = new char *[2 * _metaData->maxDegree - 1];
    size = _nodePage->size;
    flag = _nodePage->flag;
    for (int i = 0; i < 2 * _metaData->maxDegree; i++)
    {
        if (i < 2 * _metaData->maxDegree - 1)
        {
            keys[i] = NULL;
        }
        childs[i] = 0;
    }
    metaData = _metaData;

    for (int i = 0; i < size; i++)
    {
        keys[i] = sdup(_nodePage->keys[i], metaData->keySize);
        childs[i] = _nodePage->childs[i];
    }
    childs[size] = _nodePage->childs[size];

    this->offSet = _offSet;
}

string BPlusTreeNode::traverse()
{
    string testResult = "";
    if (flag == LEAF_NODE)
    {
        for (int i = 0; i < size; i++)
        {
            cout << keys[i] << "|";
            testResult += keys[i];
            testResult += "|";
        }
        testResult += "\n";
        cout << endl;
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            testResult += keys[i];
            testResult += " ";
            cout << keys[i] << " ";
        }
        testResult += "\n";
        cout << endl;
        for (int i = 0; i <= size; i++)
        {
            BPlusTreeNode *nextNode = getNode(childs[i]);
            testResult += nextNode->traverse();
            delete nextNode;
        }
    }
    return testResult;
}

BPlusTreeNode *BPlusTreeNode::insert(char *key, char *value)
{
    Entry *rightEntry = internalInsert(key, value);

    if (rightEntry) // Need to Split
    {
        BPlusTreeNode *rightChild = getNode(rightEntry->value);
        BPlusTreeNode *newRoot = new BPlusTreeNode(rightEntry->key, this, rightChild);
        newRoot->flushToFile();
        delete[] rightEntry->key;
        delete rightEntry;
        delete rightChild;
        return newRoot;
    }

    return this;
}

// insert and split. if split, return newKey,  newKey->leftValue
Entry *BPlusTreeNode::internalInsert(char *key, char *value)
{
    Entry *returnEntry = NULL;
    if (flag == LEAF_NODE)
    {
        return leafInsert(key, value);
    }
    else
    {
        BPlusTreeNode *childNode = getNode(childs[indexNodeLarger(keys, size, key)]);
        returnEntry = childNode->internalInsert(key, value);
        delete childNode;
    }

    if (returnEntry)
    {
        if (size == 2 * metaData->maxDegree - 1) //split root if it's full
        {
            Entry *newEntry = insertAndSplitInternal(returnEntry->key, returnEntry->value); //insert and get key + right part
            delete[] returnEntry->key;
            delete returnEntry;
            return newEntry;
        }
        else
        {
            insertRight(returnEntry->key, returnEntry->value);
            delete[] returnEntry->key;
        }
        delete returnEntry;
    }
    return NULL;
}

// insert and split. if split, return newKey,  newKey->leftValue
Entry *BPlusTreeNode::leafInsert(char *key, char *value)
{
    int idxEqual = indexNodeEqual(keys, size, key);

    if (idxEqual != -1)
    {
        updateValueDuplicatedKey(idxEqual, value);
        return NULL;
    }
    if (size < 2 * metaData->maxDegree - 1)
        insertLeft(key, value);
    else
    {
        Entry *newEntry = insertAndSplitLeaf(key, value); // new [midKey - value=newRightLeafKey]
        return newEntry;
    }
    return NULL;
}

void BPlusTreeNode::insertLeft(char *key, char *value)
{
    childs[size + 1] = childs[size];

    int i = size - 1;
    if (size == 0 || strcmp(keys[i], key) < 0)
    {
        delete[] keys[size];
        keys[size] = sdup(key, metaData->keySize);
        childs[size] = insertNewValueToFile(value, metaData);
    }
    else
    {
        while (i >= 0 && keys[i] && key && strcmp(keys[i], key) > 0)
        {
            keys[i + 1] = keys[i];
            childs[i + 1] = childs[i];
            i--;
        }
        keys[i + 1] = sdup(key, metaData->keySize);
        childs[i + 1] = insertNewValueToFile(value, metaData);
    }
    size++;

    this->flushToFile();
}

void BPlusTreeNode::insertRight(char *key, uint64_t value)
{
    int i = size - 1;
    if (strcmp(keys[i], key) < 0)
    {
        keys[size] = sdup(key, metaData->keySize);
        childs[size + 1] = value;
    }
    else
    {
        while (i >= 0 && strcmp(keys[i], key) > 0)
        {
            keys[i + 1] = keys[i];
            childs[i + 2] = childs[i + 1];
            i--;
        }

        keys[i + 1] = sdup(key, metaData->keySize);
        childs[i + 2] = value;
    }
    size++;
    this->flushToFile();
}

// split node to right and left node, return new key to append to father node, and right node after split
Entry *BPlusTreeNode::insertAndSplitLeaf(char *key, char *value)
{
    BPlusTreeNode *newRightNode = copyPartOfNode(LEAF_NODE, metaData->maxDegree, size - 1);

    childs[size] = NULL;
    size = metaData->maxDegree;
    if (strcmp(keys[size - 1], key) > 0)
    {
        insertLeft(key, value);
    }
    else
    {
        newRightNode->insertLeft(key, value);
    }

    newRightNode->flushToFile();
    this->flushToFile();
    childs[size] = newRightNode->offSet;
    Entry *res = new Entry(sdup(newRightNode->keys[0], metaData->keySize), newRightNode->offSet);
    delete newRightNode;
    return res;
}

//split node to right and left node, return new key to append to father node, and right node after split
Entry *BPlusTreeNode::insertAndSplitInternal(char *key, uint64_t keyRightChild)
{
    BPlusTreeNode *newRightNode = copyPartOfNode(INTERNAL_NODE, metaData->maxDegree, size - 1);

    size = metaData->maxDegree - 1;
    char *newKey = sdup(keys[metaData->maxDegree - 1], metaData->keySize);
    delete[](keys[metaData->maxDegree - 1]);
    keys[metaData->maxDegree - 1] = NULL;
    if (strcmp(newKey, key) > 0)
    {
        insertRight(key, keyRightChild);
    }
    else
    {
        newRightNode->insertRight(key, keyRightChild);
    }

    newRightNode->flushToFile();
    this->flushToFile();

    Entry *res = new Entry(newKey, newRightNode->offSet);
    delete newRightNode;
    return res;
}

BPlusTreeNode *BPlusTreeNode::copyPartOfNode(int8_t flag, int l, int r)
{
    BPlusTreeNode *newNode = new BPlusTreeNode(metaData, flag);

    int i = 0;
    for (i = l; i <= r; i++)
    {
        newNode->keys[i - l] = sdup(keys[i], metaData->keySize);
        delete[](keys[i]);
        keys[i] = NULL;

        newNode->childs[i - l] = childs[i];
        childs[i] = 0;
        newNode->size++;
    }
    newNode->childs[i - l] = childs[i];
    childs[i] = 0;

    return newNode;
}

bool BPlusTreeNode::leafRemove(BPlusTreeNode *father, int nodeIndex, char *key)
{
    if (!isExisting(keys, size, key))
    {
        return false;
    }
    if (!father || size > metaData->maxDegree - 1)
    {
        removeLeft(key);
    }
    // right brother
    else
    {
        redistributeFromLeaf(father, nodeIndex);
        removeLeft(key);
    }
    this->flushToFile();
    return true;
}

bool BPlusTreeNode::internalRemove(BPlusTreeNode *father, int nodeIndex, char *key)
{
    if (flag == LEAF_NODE)
    {
        return leafRemove(father, nodeIndex, key);
    }
    else
    {
        int childIndex = indexNodeLarger(keys, size, key);
        BPlusTreeNode *child = getNode(childs[childIndex]);
        bool result = child->internalRemove(this, childIndex, key);

        if (father && size < metaData->maxDegree - 1)
        {
            redistributeFromInternal(father, nodeIndex);
        }
        else if (!father && size == 0)
            childs[0] = child->offSet;
        this->flushToFile();
        delete child;
        return result;
    }
}

BPlusTreeNode *BPlusTreeNode::remove(char *key)
{
    bool result = internalRemove(NULL, -1, key);
    if (!result){
        return NULL;
    }

    if (size == 0)
    {
        if (flag != LEAF_NODE)
        {
            BPlusTreeNode * res = getNode(childs[0]);
            this->markDelete();
            return res;
        }
        else
        {
            this->clearChilds();
            return this;
        }
    }
    return this;
}

void BPlusTreeNode::redistributeFromLeaf(BPlusTreeNode *father, int nodeIndex)
{

    BPlusTreeNode *rightSibling = nodeIndex == father->size ? NULL : getNode(father->childs[nodeIndex + 1]);
    BPlusTreeNode *leftSibling = nodeIndex == 0 ? NULL : getNode(father->childs[nodeIndex - 1]);

    if (rightSibling && rightSibling->size > metaData->maxDegree - 1)
    {
        // cout << "swapCounterClockWiseLeaf" << endl;
        swapCounterClockWiseLeaf(father, nodeIndex, rightSibling);
    }
    else if (leftSibling && leftSibling->size > metaData->maxDegree - 1)
    {
        // cout << "swapClockWiseLeaf" << endl;
        swapClockWiseLeaf(father, nodeIndex, leftSibling);
    }
    else if (leftSibling)
    {
        // cout << "leftMergeLeaf" << endl;
        leftMergeLeaf(father, nodeIndex, leftSibling);
    }
    else
    {
        // cout << "rightMergeLeaf" << endl;
        rightMergeLeaf(father, nodeIndex, rightSibling);
    }
    if (rightSibling)
        delete rightSibling;
    if (leftSibling)
        delete leftSibling;
}

void BPlusTreeNode::redistributeFromInternal(BPlusTreeNode *father, int nodeIndex)
{

    BPlusTreeNode *rightSibling = nodeIndex == father->size ? NULL : getNode(father->childs[nodeIndex + 1]);
    BPlusTreeNode *leftSibling = nodeIndex == 0 ? NULL : getNode(father->childs[nodeIndex - 1]);

    if (rightSibling && rightSibling->size > metaData->maxDegree - 1)
    {
        // cout << "swapCounterClockWiseItn" << endl;
        swapCounterClockWiseItn(father, nodeIndex, rightSibling);
    }
    else if (leftSibling && leftSibling->size > metaData->maxDegree - 1)
    {
        // cout << "swapClockWiseItn" << endl;
        swapClockWiseItn(father, nodeIndex, leftSibling);
    }
    else if (leftSibling)
    {
        // cout << "leftMergeItn" << endl;
        leftMergeItn(father, nodeIndex, leftSibling);
    }
    else
    {
        // cout << "rightMergeItn" << endl;
        rightMergeItn(father, nodeIndex, rightSibling);
    }
    if (rightSibling)
        delete rightSibling;
    if (leftSibling)
        delete leftSibling;
}

void BPlusTreeNode::swapClockWiseItn(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *leftSibling)
{
    childs[size + 1] = childs[size];
    for (int i = size - 1; i >= 0; i--)
    {
        keys[i + 1] = keys[i];
        childs[i + 1] = childs[i];
    }

    keys[0] = father->keys[nodeIndex - 1];
    childs[0] = leftSibling->childs[leftSibling->size];
    father->keys[nodeIndex - 1] = leftSibling->keys[leftSibling->size - 1];
    leftSibling->keys[leftSibling->size - 1] = NULL;
    leftSibling->childs[leftSibling->size] = NULL;
    leftSibling->size--;
    leftSibling->flushToFile();
    size++;
}
void BPlusTreeNode::swapCounterClockWiseItn(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *rightSibling)
{
    keys[size] = father->keys[nodeIndex];
    childs[size + 1] = rightSibling->childs[0];
    father->keys[nodeIndex] = rightSibling->keys[0];

    for (int i = 1; i < rightSibling->size; i++)
    {
        rightSibling->keys[i - 1] = rightSibling->keys[i];
        rightSibling->childs[i - 1] = rightSibling->childs[i];
    }
    rightSibling->childs[rightSibling->size - 1] = rightSibling->childs[rightSibling->size];
    rightSibling->childs[rightSibling->size] = NULL;
    rightSibling->keys[rightSibling->size - 1] = NULL;

    rightSibling->size--;
    rightSibling->flushToFile();
    size++;
}

void BPlusTreeNode::swapClockWiseLeaf(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *leftSibling)
{
    childs[size + 1] = childs[size];
    for (int i = size - 1; i >= 0; i--)
    {
        keys[i + 1] = keys[i];
        childs[i + 1] = childs[i];
    }
    keys[0] = leftSibling->keys[leftSibling->size - 1];
    childs[0] = leftSibling->childs[leftSibling->size - 1];
    leftSibling->childs[leftSibling->size - 1] = leftSibling->childs[leftSibling->size]; // pointer to next node
    delete[](father->keys[nodeIndex - 1]);
    father->keys[nodeIndex - 1] = sdup(keys[0], metaData->keySize);

    leftSibling->keys[leftSibling->size - 1] = NULL;
    leftSibling->childs[leftSibling->size] = NULL;
    leftSibling->size--;
    leftSibling->flushToFile();
    size++;
}
void BPlusTreeNode::swapCounterClockWiseLeaf(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *rightSibling)
{
    keys[size] = rightSibling->keys[0];
    childs[size + 1] = rightSibling->offSet;
    childs[size] = rightSibling->childs[0];

    for (int i = 1; i < rightSibling->size; i++)
    {
        rightSibling->keys[i - 1] = rightSibling->keys[i];
        rightSibling->childs[i - 1] = rightSibling->childs[i];
    }
    rightSibling->childs[rightSibling->size - 1] = rightSibling->childs[rightSibling->size];
    delete[](father->keys[nodeIndex]);
    father->keys[nodeIndex] = sdup(rightSibling->keys[0], metaData->keySize);

    rightSibling->childs[rightSibling->size] = NULL;
    rightSibling->keys[rightSibling->size - 1] = NULL;

    rightSibling->size--;
    rightSibling->flushToFile();
    size++;
}

void BPlusTreeNode::leftMergeItn(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *leftSibling)
{
    for (int i = 0; i < size; i++)
    {
        if (keys[leftSibling->size + 1 + i])
            delete[](keys[leftSibling->size + 1 + i]);
        keys[leftSibling->size + 1 + i] = keys[i];
        childs[leftSibling->size + 1 + i] = childs[i];
    }
    childs[leftSibling->size + 1 + size] = childs[size];

    keys[leftSibling->size] = sdup(father->keys[nodeIndex - 1], metaData->keySize);

    size += leftSibling->size + 1;

    for (int i = 0; i < leftSibling->size; i++)
    {
        keys[i] = sdup(leftSibling->keys[i], metaData->keySize);
        childs[i] = leftSibling->childs[i];
    }
    childs[leftSibling->size] = leftSibling->childs[leftSibling->size];
    father->removeLeft(nodeIndex - 1);
    father->childs[nodeIndex - 1] = this->offSet;
    leftSibling->markDelete();
}

void BPlusTreeNode::rightMergeItn(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *rightSibling)
{
    for (int i = 0; i < rightSibling->size; i++)
    {
        if (keys[size + 1 + i])
            delete[](keys[size + 1 + i]);
        keys[size + 1 + i] = sdup(rightSibling->keys[i], metaData->keySize);
        childs[size + 1 + i] = rightSibling->childs[i];
    }
    childs[rightSibling->size + 1 + size] = rightSibling->childs[rightSibling->size];

    keys[size] = sdup(father->keys[nodeIndex], metaData->keySize);

    size += rightSibling->size + 1;

    father->removeLeft(nodeIndex);
    father->childs[nodeIndex] = this->offSet;
    rightSibling->markDelete();
}

void BPlusTreeNode::leftMergeLeaf(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *leftSibling)
{
    childs[size + leftSibling->size] = childs[size];
    if (nodeIndex - 2 >= 0)
    {
        BPlusTreeNode *sibling = getNode(father->childs[nodeIndex - 2]);
        sibling->childs[sibling->size] = this->offSet;
        delete sibling;
    }
    for (int i = 0; i < size; i++)
    {
        keys[leftSibling->size + i] = keys[i];
        childs[leftSibling->size + i] = childs[i];
    }

    size += leftSibling->size;

    for (int i = 0; i < leftSibling->size; i++)
    {
        keys[i] = sdup(leftSibling->keys[i], metaData->keySize);
        childs[i] = leftSibling->childs[i];
    }

    father->removeLeft(nodeIndex - 1);

    father->childs[nodeIndex - 1] = this->offSet;

    leftSibling->markDelete();
}

void BPlusTreeNode::rightMergeLeaf(BPlusTreeNode *father, int nodeIndex, BPlusTreeNode *rightSibling)
{
    for (int i = 0; i < rightSibling->size; i++)
    {
        keys[size + i] = sdup(rightSibling->keys[i], metaData->keySize);
        childs[size + i] = rightSibling->childs[i];
    }
    childs[rightSibling->size + size] = rightSibling->childs[rightSibling->size];

    size += rightSibling->size;
    father->removeLeft(nodeIndex);

    father->childs[nodeIndex] = this->offSet;
    rightSibling->markDelete();
}
void BPlusTreeNode::removeLeft(char *key)
{
    removeLeft(indexNodeEqual(keys, size, key));
}

void BPlusTreeNode::removeLeft(int index)
{
    delete[](keys[index]);
    
    if (flag==LEAF_NODE)
    {
        metaData->delValues->push(childs[index]);
    }

    for (int i = index; i < size - 1; i++)
    {
        keys[i] = keys[i + 1];
        childs[i] = childs[i + 1];
    }
    childs[size - 1] = childs[size];
    childs[size] = NULL;
    keys[size - 1] = NULL;
    size--;
}

BPlusTreeNode::~BPlusTreeNode()
{
    for (int i = 0; i < 2 * metaData->maxDegree - 1; i++)
    {
        if (keys[i] != NULL)
        {
            delete[](keys[i]);
            keys[i] = NULL;
        }
    }
    delete[] childs;
    delete[] keys;
}

void BPlusTreeNode::markDelete()
{
    this->clearChilds();
    flag = REMOVED_NODE;
    this->flushToFile();
    metaData->delNodePages->push(offSet);
}

void BPlusTreeNode::clearChilds()
{
    size = 0;
    for (int i = 0; i < 2 * metaData->maxDegree; i++)
    {
        childs[i] = 0;
    }
}

char *BPlusTreeNode::get(char *key)
{
    if (flag == LEAF_NODE)
    {
        int index = indexNodeEqual(keys, size, key);
        if (index == -1)
            return NULL;
        return getValue(childs[index]);
    }
    BPlusTreeNode *bnode = getNode(childs[indexNodeLarger(keys, size, key)]);
    char *res = bnode->get(key);
    delete bnode;
    return res;
}

bool BPlusTreeNode::exist(char *key)
{
    if (flag == LEAF_NODE)
        return isExisting(keys, size, key);
    BPlusTreeNode *bnode = getNode(childs[indexNodeLarger(keys, size, key)]);
    bool res = bnode->exist(key);
    delete bnode;
    return res;
}

BPlusTreeNode *BPlusTreeNode::getNode(uint64_t offSet)
{
    BPlusTreeNode *res = getBNodeFromFile(metaData, offSet);
    return res;
}

char *BPlusTreeNode::getValue(uint64_t offSet)
{
    return getValueFromFile(metaData, offSet);
}

NodePage *BPlusTreeNode::toNodePage()
{
    NodePage *npage = new NodePage(metaData);
    npage->size = size;
    npage->flag = flag;
    for (int i = 0; i < size; i++)
    {
        strcpy(npage->keys[i], keys[i]);
        npage->childs[i] = childs[i];
    }
    npage->childs[size] = childs[size];
    return npage;
}

void BPlusTreeNode::flushToFile()
{
    NodePage *npage = toNodePage();
    flushNodePageToFile(metaData, npage, offSet);
    delete npage;
}

void BPlusTreeNode::appendToFile()
{
    NodePage *npage = toNodePage();
    offSet = appendNodePageToFile(metaData, npage);
    delete npage;
}

void BPlusTreeNode::updateValueDuplicatedKey(int index, char *value)
{
    uint64_t valueOffSet = childs[index];
    updateValueToFile(value, valueOffSet, metaData);
}

uint64_t BPlusTreeNode::getOffset()
{
    return offSet;
}

uint8_t BPlusTreeNode::getFlag()
{
    return flag;
}