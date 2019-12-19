#include "BTreeNode.h"
#include "BTree.h"

using namespace std;

BTreeNode::BTreeNode(bool isLeaf, int maxDegree, int keySize) {
    T = maxDegree;
    bufferKeySize = keySize;
    arrChild = new uint64_t[2*T];
    valuePos = new uint64_t[2*T-1];
    arrEntry = new K[2*T-1];
    int i = 0;

    for(i = 0; i < 2*T-1; i++) {   
        arrEntry[i] = newKey(bufferKeySize);
        strcpy(arrEntry[i], "-1");
        arrChild[i] = 0;
        valuePos[i] = 0;
    }
    arrChild[i] = 0;
    if(isLeaf) {
        markLeaf();
    }
    else {
        markInternal();
    }
    nEntry = 0;
}

BTreeNode::BTreeNode(int maxDegree, int keySize) {
    T = maxDegree;
    bufferKeySize = keySize;
    arrChild = new uint64_t[2*T];
    valuePos = new uint64_t[2*T-1];
    arrEntry = new K[2*T-1];
    int i = 0;
    for(i = 0; i < 2*T-1; i++) {
        arrEntry[i] = newKey(bufferKeySize);
        strcpy(arrEntry[i], "-1");
        arrChild[i] = 0;
        valuePos[i] = 0;
    }
    arrChild[i] = 0;
}

void BTreeNode::markLeaf() {
    // 01
    this->flag = 0x1;
}

void BTreeNode::markInternal() {
    // 10
    this->flag = 0x2;
}

void BTreeNode::markDeleted() {
    // 11
    this->flag = 0x3;
}

void BTreeNode::insertNonFull(BTree* tree, KV* entry, FILE* file) {
    int i = nEntry-1, tmpIdx = i;
    bool checkUpdateNextPos = true;

    if(flag == LEAF) {
        bool checkUpdate = false;
        while(tmpIdx >= 0) {
            if(compareKey(entry->getKey(), arrEntry[tmpIdx]) == 0) {
                checkUpdate = true;
                break;
            }
            tmpIdx--;
        }

        // If insert at existing key -> overwrite its value
        if(checkUpdate) {
            updateValueAt(tree, valuePos[tmpIdx], entry->getValue(), true, file);
            return;
        }

        while(i >= 0 && compareKey(entry->getKey(), arrEntry[i]) < 0) {
            strcpy(arrEntry[i+1], arrEntry[i]);
            valuePos[i+1] = valuePos[i];
            i--;
        }

        if(!tree->deletedValsPos.empty()) {
            valuePos[i+1] = tree->deletedValsPos.front();
            tree->deletedValsPos.pop();
        }
        else {
            tree->updateNextPosNodeValue(file);
            valuePos[i+1] = tree->getNextPosNodeValue();
        }
        strcpy(arrEntry[i+1], entry->getKey());
        nEntry++;

        WRITE_NODE(tree, this, this->pos, file);
        updateValueAt(tree, valuePos[i+1], entry->getValue(), false, file);
        return;
    }

    // Replace this traversal by binary search
    while(i >= 0 && compareKey(entry->getKey(), arrEntry[i]) < 0) {
        i--;
    }

    if(i >= 0 && compareKey(entry->getKey(), arrEntry[i]) == 0) {
        updateValueAt(tree, valuePos[i], entry->getValue(), true, file);
        return;
    }

    BTreeNode* childNode = READ_NODE(tree, this, arrChild[i+1], file);

    if(childNode->nEntry == 2*T-1) {
        splitChild(tree, i+1, childNode, file);
        if(compareKey(arrEntry[i+1], entry->getKey()) < 0) {
            i++;
        }
    }
    delete childNode;

    childNode = READ_NODE(tree, this, arrChild[i+1], file);

    childNode->insertNonFull(tree, entry, file);

    delete childNode;
}

void BTreeNode::splitChild(BTree* tree, int idx, BTreeNode* child, FILE* file) {
    bool isLeaf = (child->flag == LEAF) ? true : false;
    BTreeNode* newNode = new BTreeNode(isLeaf, T, tree->getBufferSize());
    newNode->nEntry = T - 1;

    for(int j = 0; j < T - 1; j++) {
        strcpy(newNode->arrEntry[j], child->arrEntry[j+T]);
        newNode->valuePos[j] = child->valuePos[j+T];
    }

    if(child->flag != LEAF) {
        for(int j = 0; j < T; j++) {
            newNode->arrChild[j] = child->arrChild[j+T];
        }
    }

    child->nEntry = T - 1;

    for(int j = nEntry; j >= idx+1; j--) {
        arrChild[j+1] = arrChild[j];
    }

    for(int j = nEntry-1; j >= idx; j--) {
        strcpy(arrEntry[j+1], arrEntry[j]);
        valuePos[j+1] = valuePos[j];
        valuePos[j] = 0;
    }

    strcpy(arrEntry[idx], child->arrEntry[T-1]);
    valuePos[idx] = child->valuePos[T-1];

    // Update second half of child to init values
    for(int i = T - 1; i < 2 * T - 1; i++) {
        delete[] child->arrEntry[i];
        child->arrEntry[i] = newKey(bufferKeySize);
        strcpy(child->arrEntry[i], "-1");
        child->valuePos[i] = 0;
    }
    if(child->flag != LEAF) {
        for(int i = T; i < 2 * T; i++) {
            child->arrChild[i] = 0;
        }
    }

    nEntry++;

    if(!tree->deletedNodesPos.empty()) {
        newNode->pos = tree->deletedNodesPos.front();
        tree->deletedNodesPos.pop();
    }
    else {
        writeEmptyBytes(tree, pos, file);
        tree->updateNextPosNode(file);
        newNode->pos = tree->getNextPosNode(); // Write new node after this node immediately
    }
    arrChild[idx+1] = newNode->pos;

    WRITE_NODE(tree, child, child->pos, file);
    WRITE_NODE(tree, this, pos, file);
    WRITE_NODE(tree, newNode, newNode->pos, file);

    delete newNode;
}

bool BTreeNode::remove(BTree* tree, K key, bool isDeleteReal, FILE* file) {
    int idx = 0;
    bool result;

    while(idx < nEntry && compareKey(arrEntry[idx], key) < 0) {
        idx++;
    }

    if(idx < nEntry && compareKey(arrEntry[idx], key) == 0) {
        if(this->flag == LEAF) {
            result = removeFromLeaf(tree, idx, isDeleteReal, file);
        }
        else {
            result = removeFromNonLeaf(tree, idx, file);
        }
        return result;
    }
    else if(this->flag == LEAF) {
        return false;
    }

    bool check = (idx == nEntry) ? true : false;

    BTreeNode* childNode = READ_NODE(tree, this, arrChild[idx], file);
    if(childNode->nEntry < T) {
        fillNode(tree, idx, file);
    }
    delete childNode;

    if(check && idx > nEntry) {
        childNode = READ_NODE(tree, this, arrChild[idx-1], file);
        result = childNode->remove(tree, key, isDeleteReal, file);
        delete childNode;
    }
    else {
        childNode = READ_NODE(tree, this, arrChild[idx], file);
        result = childNode->remove(tree, key, isDeleteReal, file);
        delete childNode;
    }

    return result;
}

bool BTreeNode::removeFromLeaf(BTree* tree, int idx, bool isDeleteReal, FILE* file) {
    tree->deletedValsPos.push(valuePos[idx]);
    for(int i = idx + 1; i < nEntry; i++) {
        strcpy(arrEntry[i-1], arrEntry[i]);
        valuePos[i-1] = valuePos[i];
    }
    nEntry--;
    WRITE_NODE(tree, this, this->pos, file);
    return true;
}

bool BTreeNode::removeFromNonLeaf(BTree* tree, int idx, FILE* file) {
    bool result = false;
    int checkWriteNode = 0;
    BTreeNode* childNodePred = READ_NODE(tree, this, arrChild[idx], file);
    BTreeNode* childNodeSucc = READ_NODE(tree, this, arrChild[idx+1], file);

    if(childNodePred->nEntry >= T) {
        BTreeNode* predNode = getPredNode(tree, idx, file);
        KV* entry = predNode->getEntry(tree, predNode->nEntry-1, file);
        strcpy(arrEntry[idx], entry->getKey());
        tree->deletedValsPos.push(valuePos[idx]);
        valuePos[idx] = predNode->valuePos[predNode->nEntry-1];
        result = childNodePred->remove(tree, entry->getKey(), false, file);
        checkWriteNode = 1;
        delete entry;
        delete predNode;
    }
    else if(childNodeSucc->nEntry >= T) {
        BTreeNode* succNode = getSuccNode(tree, idx, file);
        KV* entry = succNode->getEntry(tree, 0, file);
        strcpy(arrEntry[idx], entry->getKey());
        tree->deletedValsPos.push(valuePos[idx]);
        valuePos[idx] = succNode->valuePos[0];
        result = childNodeSucc->remove(tree, entry->getKey(), false, file);
        checkWriteNode = 2;
        delete entry;
        delete succNode;
    }
    else {
        mergeNodes(tree, idx, file);
        delete childNodePred;
        childNodePred = READ_NODE(tree, this, arrChild[idx], file);
        result = childNodePred->remove(tree, childNodePred->arrEntry[T-1], true, file);
        checkWriteNode = 1;
    }

    if(result) {
        if(checkWriteNode == 1) {
            WRITE_NODE(tree, childNodePred, childNodePred->pos, file);
        }
        else {
            WRITE_NODE(tree, childNodeSucc, childNodeSucc->pos, file);
        }
        WRITE_NODE(tree, this, this->pos, file);
    }

    delete childNodePred;
    delete childNodeSucc;
    return result;
}

BTreeNode* BTreeNode::getPredNode(BTree* tree, int idx, FILE* file) {
    BTreeNode* currentChild = READ_NODE(tree, this, arrChild[idx], file);
    while(currentChild->flag != LEAF) {
        uint64_t nextNodePos = currentChild->arrChild[currentChild->nEntry];
        delete currentChild;
        currentChild = READ_NODE(tree, currentChild, nextNodePos, file);
    }
    return currentChild;
}

BTreeNode* BTreeNode::getSuccNode(BTree* tree, int idx, FILE* file) {
    BTreeNode* currentChild = READ_NODE(tree, this, arrChild[idx+1], file);
    while(currentChild->flag != LEAF) {
        uint64_t nextNodePos = currentChild->arrChild[0];        
        delete currentChild;
        currentChild = READ_NODE(tree, currentChild, nextNodePos, file);
    }
    return currentChild;
}

void BTreeNode::borrowFromNextNode(BTree* tree, int idx, FILE* file) {
    BTreeNode* child = READ_NODE(tree, this, arrChild[idx], file);
    BTreeNode* nextChild = READ_NODE(tree, this, arrChild[idx+1], file);
    strcpy(child->arrEntry[child->nEntry], arrEntry[idx]);
    child->valuePos[child->nEntry] = valuePos[idx];

    if(child->flag != LEAF) {
        child->arrChild[child->nEntry+1] = nextChild->arrChild[0];
    }

    strcpy(arrEntry[idx], nextChild->arrEntry[0]);
    valuePos[idx] = nextChild->valuePos[0];

    int nextChildEntrySize = nextChild->nEntry;
    for(int i = 1; i < nextChildEntrySize; i++) {
        strcpy(nextChild->arrEntry[i-1], nextChild->arrEntry[i]);
        nextChild->valuePos[i-1] = nextChild->valuePos[i];
    }

    if(nextChild->flag != LEAF) {
        for(int i = 1; i <= nextChildEntrySize; i++) {
            nextChild->arrChild[i-1] = nextChild->arrChild[i];
        }
    }

    child->nEntry++;
    nextChild->nEntry--;

    WRITE_NODE(tree, this, this->pos, file);
    WRITE_NODE(tree, child, child->pos, file);
    WRITE_NODE(tree, nextChild, nextChild->pos, file);

    delete child;
    delete nextChild;
    return;
}

void BTreeNode::borrowFromPrevNode(BTree* tree, int idx, FILE* file) {
    BTreeNode* child = READ_NODE(tree, this, arrChild[idx], file);
    BTreeNode* prevChild = READ_NODE(tree, this, arrChild[idx-1], file);

    int childEntrySize = child->nEntry;
    for(int i = childEntrySize - 1; i >= 0; i--) {
        strcpy(child->arrEntry[i+1], child->arrEntry[i]);
        child->valuePos[i+1] = child->valuePos[i];
    }

    if(child->flag != LEAF) {
        for(int i = childEntrySize; i >= 0; i--) {
            child->arrChild[i+1] = child->arrChild[i];
        }
    }

    strcpy(child->arrEntry[0], arrEntry[idx-1]);
    child->valuePos[0] = valuePos[idx-1];

    if(child->flag != LEAF) {
        child->arrChild[0] = prevChild->arrChild[prevChild->nEntry];
    }

    strcpy(arrEntry[idx-1], prevChild->arrEntry[prevChild->nEntry-1]);
    valuePos[idx-1] = prevChild->valuePos[prevChild->nEntry-1];

    child->nEntry++;
    prevChild->nEntry--;

    WRITE_NODE(tree, this, this->pos, file);
    WRITE_NODE(tree, child, child->pos, file);
    WRITE_NODE(tree, prevChild, prevChild->pos, file);

    delete child;
    delete prevChild;
    return;
}

void BTreeNode::fillNode(BTree* tree, int idx, FILE* file) {
    BTreeNode* prevChild = READ_NODE(tree, this, arrChild[idx-1], file);
    BTreeNode* nextChild = READ_NODE(tree, this, arrChild[idx+1], file);

    if(idx != 0 && prevChild->nEntry >= T) {
        borrowFromPrevNode(tree, idx, file);
    }
    else if(idx != nEntry && nextChild->nEntry >= T) {
        borrowFromNextNode(tree, idx, file);
    }
    else {
        if(idx != nEntry) {
            mergeNodes(tree, idx, file);
        }
        else {
            mergeNodes(tree, idx-1, file);
        }
    }
    
    delete prevChild;
    delete nextChild;
    return;
}

void BTreeNode::mergeNodes(BTree* tree, int idx, FILE* file) {
    BTreeNode* child = READ_NODE(tree, this, arrChild[idx], file);
    BTreeNode* nextChild = READ_NODE(tree, this, arrChild[idx+1], file);

    strcpy(child->arrEntry[T-1], arrEntry[idx]);
    child->valuePos[T-1] = valuePos[idx];

    int nextChildEntrySize = nextChild->nEntry;
    for(int i = 0; i < nextChildEntrySize; i++) {
        strcpy(child->arrEntry[i+T], nextChild->arrEntry[i]);
        child->valuePos[i+T] = nextChild->valuePos[i];
    }

    if(child->flag != LEAF) {
        for(int i = 0; i <= nextChildEntrySize; i++) {
            child->arrChild[i+T] = nextChild->arrChild[i];
        }
    }

    for(int i = idx + 1; i < nEntry; i++) {
        strcpy(arrEntry[i-1], arrEntry[i]);
        valuePos[i-1] = valuePos[i];
    }

    for(int i = idx + 2; i <= nEntry; i++) {
        arrChild[i-1] = arrChild[i];
    }

    child->nEntry += nextChildEntrySize + 1;

    tree->deletedNodesPos.push(nextChild->pos);
    nextChild->markDeleted();
    nEntry--;

    WRITE_NODE(tree, child, child->pos, file);
    WRITE_NODE(tree, nextChild, nextChild->pos, file);
    WRITE_NODE(tree, this, this->pos, file);

    delete child;
    delete nextChild;

    return;
}

V BTreeNode::search(BTree* tree, K k, FILE* file) {
    int i = 0;
    while(i < nEntry && compareKey(k, arrEntry[i]) > 0) {
        i++;
    }
    if(i < nEntry && compareKey(arrEntry[i], k) == 0) {
        return this->getValueInNode(tree, k, file);
    }
    if(this->flag == LEAF) {
        return NULL;
    }
    BTreeNode* childNode = READ_NODE(tree, this, arrChild[i], file);

    V val = childNode->search(tree, k, file);

    delete childNode;
    return val;
}

void BTreeNode::clearEntry(KV* entry) {
    if(entry->getKey() != NULL)
        delete[] entry->getKey();
    if(entry->getValue() != NULL)
        delete[] entry->getValue();
    delete entry;
    entry = NULL;
}

KV* BTreeNode::getEntry(BTree* tree, int idx, FILE* file) {
    K key = this->arrEntry[idx];
    V value = readValueInFile(tree, valuePos[idx], file);
    KV* entry = new KV(key, value, tree->getBufferSize(), tree->getValueSize());
    delete value;
    return entry;
}

V BTreeNode::getValueInNode(BTree* tree, K key, FILE* file) {
    // Remember to change this to binary search
    for(int i = 0; i < nEntry; i++) {
        if(compareKey(arrEntry[i], key) == 0) {
            V value = readValueInFile(tree, valuePos[i], file);
            return value;
        }
    }
    return NULL;
}

BTreeNode::~BTreeNode() {
    if(this->arrChild != NULL) {
        delete[] this->arrChild;
    }
    if(this->valuePos != NULL) {
        delete[] this->valuePos;
    }
    if(this->arrEntry != NULL) {
        for(int i = 0; i < 2 * T - 1; i++) {
            delete[] this->arrEntry[i];
        }
        delete[] this->arrEntry;
    }
}
