#include "BTree.h"
#include "BTreeNode.h"
#include "FileManager.h"

BTree::BTree() {
    dbFileName = getDBFileNameInConfig();
    BTreeStore* treeMetaData = READ_HEADER(this);
    root = NULL;
    T = treeMetaData->T;
    bufferSize = treeMetaData->bufferSize;
    rootPos = treeMetaData->rootPos;
    nextPosNode = treeMetaData->nextPosNode;
    nextPosNodeValue = treeMetaData->nextPosNodeValue;
    pageSize = treeMetaData->pageSize;
    valueSize = treeMetaData->valueSize;
    strcpy(deletedNodesFileName, treeMetaData->deletedNodesFileName);
    strcpy(deletedValuesFileName, treeMetaData->deletedValuesFileName);
    readDeletedNodesPos(this);
    readDeletedValsPos(this);
    fileManager = new FileManager(dbFileName);
    delete treeMetaData;
}

void BTree::insert(KV* entry) {
    FILE* file = fileManager->getFilePtr();
    if(root == NULL) {
        root = new BTreeNode(true, T, this->getBufferSize());
        strcpy(root->arrEntry[0], entry->getKey());
        root->nEntry = 1;
        root->pos = this->nextPosNode;
        root->valuePos[0] = this->nextPosNodeValue;
        WRITE_NODE(this, root, root->pos, file);
        updateValueAt(this, root->valuePos[0], entry->getValue(), false, file);
        return;
    }
    if(root->nEntry == 2 * T - 1) {
        BTreeNode* newNode = new BTreeNode(false, T, this->getBufferSize());
        newNode->arrChild[0] = root->pos;
        if(!deletedNodesPos.empty()) {
            newNode->pos = deletedNodesPos.front();
            deletedNodesPos.pop();
        }
        else {
            updateNextPosNode(file);
            newNode->pos = this->nextPosNode;
        }
        newNode->splitChild(this, 0, root, file);
        int i = 0;
        if(compareKey(newNode->arrEntry[0], entry->getKey()) < 0) {
            i++;
        }
        BTreeNode* childOfNewNode = READ_NODE(this, newNode, newNode->arrChild[i], file);
        childOfNewNode->insertNonFull(this, entry, file);
        delete root;
        root = newNode;
        this->setRootPost(root->pos);
        delete childOfNewNode;
    }
    else {
        root->insertNonFull(this, entry, file);
    }
    fileManager->giveFilePtrBack(file);
}

void BTree::traverse() {
    // if(root != NULL) {
    //     //root->traverse();
    //     // cout << "GET BACK LATER" << endl;
    // }
}

void BTree::traverseByLevel() {
    FILE* file = fileManager->getFilePtr();
    int cnt = 0;
    bool checkedZeroPos = false;
    if(root == NULL) {
        return;
    }
    queue<BTreeNode*> q;
    q.push(root);
    if(root->pos == 0) {
        checkedZeroPos = true;
    }

    while(!q.empty()) {
        int remain = q.size();
        while(remain > 0) {
            BTreeNode* currNode = q.front();
            for(int i = 0; i < currNode->nEntry; i++) {
                printf("%s ", currNode->arrEntry[i]);
                cnt++;
            }
            cout << " | ";
            for(int i = 0; i <= currNode->nEntry; i++) {
                if(currNode->arrChild[i] != BTreeNode::EMPTY_SLOT || (currNode->arrChild[i] == BTreeNode::EMPTY_SLOT && !checkedZeroPos)) {
                    BTreeNode* childNode = READ_NODE(this, currNode, currNode->arrChild[i], file);
                    q.push(childNode);
                    if(currNode->arrChild[i] == BTreeNode::EMPTY_SLOT) {
                        checkedZeroPos = true;
                    }
                }
            }
            if(currNode->pos != root->pos) {
                delete currNode;
            }
            q.pop();
            remain--;
        }
        cout << endl;
    }
    cout << "TOTAL NUM NODES: " << cnt << endl;
    fileManager->giveFilePtrBack(file);
}

V BTree::search(K key) {
    FILE* file = fileManager->getFilePtr();
    V val = NULL;
    if(root != NULL && root->nEntry != 0) {
        val = root->search(this, key, file);
    }
    fileManager->giveFilePtrBack(file);
    return val;
}

bool BTree::remove(K key) {
    FILE* file = fileManager->getFilePtr();
    if (!root) {
        return false;
    }

    bool result = root->remove(this, key, true, file);

    if (root->nEntry == 0)
    {
        BTreeNode *tmp = root;
        if (root->flag == BTreeNode::LEAF) {
            root = NULL;
            clearDeletedNodesFile(this);
            clearDeletedValsFile(this);
        }
        else {
            root = READ_NODE(this, root, root->arrChild[0], file);
            deletedNodesPos.push(tmp->pos);
        }
        tmp->markDeleted();
        WRITE_NODE(this, tmp, tmp->getNodePos(), file);
        delete tmp;
    }
    fileManager->giveFilePtrBack(file);
    return result;
}

FILE* BTree::getFilePtr() {
    FILE* file = this->fileManager->getFilePtr();
    return file;
}

void BTree::updateNextPosNode(FILE* filePtr) {
    fseek(filePtr, 0, SEEK_END);
    uint64_t nextPos = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);
    this->nextPosNode = (nextPos - this->getHeaderSize()) / pageSize;
}

void BTree::updateNextPosNodeValue(FILE* filePtr) {
    uint8_t currValueOffset = this->nextPosNodeValue & 0x3;
    if(currValueOffset == this->pageSize / this->valueSize - 1) {
        fseek(filePtr, 0, SEEK_END);
        uint64_t nextValuePos = ftell(filePtr);
        fseek(filePtr, 0, SEEK_SET);
        this->nextPosNodeValue = (nextValuePos - this->getHeaderSize()) / pageSize;
        this->nextPosNodeValue <<= 2; // reserve 2 bits for value offset in page
    }
    else {
        this->nextPosNodeValue = ((this->nextPosNodeValue >> 2) << 2) | (currValueOffset + 1);
    }
}

BTree::~BTree() {
    WRITE_HEADER(this);

    delete fileManager;

    if(root != NULL) {
        flushDeletedNodesFile(this);
        flushDeletedValsFile(this);
        delete root;
        root = NULL;
    }
}
