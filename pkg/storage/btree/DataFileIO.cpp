#include "DataFileIO.h"
#include "BTree.h"
#include "BTreeNode.h"


V readValueInFile(BTree* tree, uint64_t valuePos, FILE* filePtr) {
    string dbFileName = tree->getDBFileName();
    if(filePtr == NULL) {
        filePtr = fopen(dbFileName.c_str(), "rb+");
    }
    uint64_t pagePos = valuePos >> 2;
    int valueSize = tree->getValueSize();
    int pageSize = tree->getPageSize();
    int valueIdx = valuePos & 0x3;
    uint64_t valuePosInFile = pagePos * pageSize + valueIdx * valueSize + tree->getHeaderSize();
    fseek(filePtr, valuePosInFile, SEEK_SET);
    char* valueBuff = new char[valueSize];
    fread(valueBuff, valueSize, 1, filePtr);

    rewind(filePtr);
    return valueBuff;
}

void updateValueAt(BTree* tree, uint64_t valuePos, V newValue, bool isOverWrite, FILE* filePtr) {
    string dbFileName = tree->getDBFileName();
    if(filePtr == NULL) {
        filePtr = fopen(dbFileName.c_str(), "rb+");
    }
    uint64_t pagePos = valuePos >> 2;
    uint16_t valueSize = tree->getValueSize();
    uint16_t pageSize = tree->getPageSize();
    uint8_t valueIdx = valuePos & 0x3;

    if(valueIdx == 0 && !isOverWrite) {
        char* pageLoadedBuff = new char[pageSize];
        fseek(filePtr, 0, SEEK_END);
        memcpy(pageLoadedBuff, newValue, valueSize);
        fwrite(pageLoadedBuff, pageSize, 1, filePtr);
        delete[] pageLoadedBuff;
    }
    else {
        uint64_t valuePosInFile = pagePos * pageSize + tree->getHeaderSize() + valueIdx * valueSize;
        fseek(filePtr, valuePosInFile, SEEK_SET);
        fwrite(newValue, valueSize, 1, filePtr);
    }
    rewind(filePtr);
}

void WRITE_NODE(BTree* b, BTreeNode* node, uint64_t p, FILE* file) {
    string dbFileName = b->getDBFileName();

    uint64_t tmpPos = p;
    uint16_t keySize = b->getBufferSize();
    uint64_t pos = (p * b->getPageSize()) + b->getHeaderSize();
    if(file == NULL) {
        file = fopen(dbFileName.c_str(), "rb+");
    }
    fseek(file, pos, SEEK_SET);

    char buff[4096];
    uint16_t pageSize = b->getPageSize();
    uint32_t T = b->getMaxDegree();

    uint64_t* arrChild = node->getChildPos();
    int byteCopied = 0;
    for(int i = 0; i < 2 * T; i++) {
        memcpy(buff + i * sizeof(uint64_t), &arrChild[i], sizeof(uint64_t));
    }
    byteCopied += (2 * T) * sizeof(uint64_t);

    uint64_t* valuePos = node->getValuePos();
    for(int i = 0; i < 2 * T - 1; i++) {
        memcpy(buff + i * sizeof(uint64_t) + byteCopied, &valuePos[i], sizeof(uint64_t));
    }
    byteCopied += (2 * T - 1) * sizeof(uint64_t);
    int tmp = byteCopied;

    K* arrEntry = node->getEntryKeyList();
    for(int i = 0; i < 2 * T - 1; i++) {
        memcpy(buff + i * keySize + byteCopied, arrEntry[i], keySize);
    }
    byteCopied += (2 * T - 1) * keySize;

    uint8_t flag = node->getFlag();
    memcpy(buff + byteCopied, &flag, sizeof(uint8_t));
    byteCopied += sizeof(uint8_t);
    uint16_t nEntry = node->getNumEntry();
    memcpy(buff + byteCopied, &nEntry, sizeof(uint16_t));
    byteCopied += sizeof(uint16_t);
    memcpy(buff + byteCopied, &tmpPos, sizeof(uint32_t));
    byteCopied += sizeof(uint32_t);

    int chunkSize = 512;
    int chunk = 0;

    while(chunk < byteCopied) {
        fwrite(buff + chunk, chunkSize, 1, file);
        chunk += chunkSize;
    }
    
    rewind(file);
}

BTreeNode* READ_NODE(BTree* b, BTreeNode* node, uint64_t p, FILE* file) {
    string dbFileName = b->getDBFileName();

    uint64_t pos = (p * b->getPageSize()) + b->getHeaderSize();
    uint32_t T = b->getMaxDegree();
    
    uint16_t keySize = b->getBufferSize();
    uint16_t pageSize = b->getPageSize();
    if(file == 0x0 || file == NULL) {
        file = fopen(dbFileName.c_str(), "rb+");
    }

    fseek(file, pos, SEEK_SET);
    char buff[4096];
    fread(buff, pageSize, 1, file);

    BTreeNode* readNode = new BTreeNode(T, keySize);
    int byteCopied = 0;

    memcpy(readNode->getChildPos(), buff + byteCopied, (2 * T) * sizeof(uint64_t));
    byteCopied += (2 * T) * sizeof(uint64_t);
    memcpy(readNode->getValuePos(), buff + byteCopied, (2 * T - 1) * sizeof(uint64_t));
    byteCopied += (2 * T - 1) * sizeof(uint64_t);

    K* entryKeys = readNode->getEntryKeyList();
    for(int i = 0; i < 2 * T - 1; i++) {
        memcpy(entryKeys[i], buff + byteCopied + i * keySize, keySize);
    }
    byteCopied += (2 * T - 1) * keySize;

    uint8_t tmpFlag;
    uint16_t tmpNumEntry;
    uint32_t tmpPos;
    memcpy(&(tmpFlag), buff + byteCopied, sizeof(uint8_t));
    byteCopied += sizeof(uint8_t);
    memcpy(&(tmpNumEntry), buff + byteCopied, sizeof(uint16_t));
    byteCopied += sizeof(uint16_t);
    memcpy(&(tmpPos), buff + byteCopied, sizeof(uint32_t));
    byteCopied += sizeof(uint32_t);
    readNode->setFlag(tmpFlag);
    readNode->setNumEntry(tmpNumEntry);
    readNode->setNodePos(tmpPos);

    rewind(file);
    return readNode;
}

void getConfigValues(BTreeStore* treeMetaData) {
    ifstream file;
    file.open(configFileName);
    while(!file.eof()) {
        string configName, tmp, val;
        file >> configName >> tmp >> val;
        if(configName == "DELETED_NODES_FILE_NAME") {
            strcpy(treeMetaData->deletedNodesFileName, val.c_str());
        }
        else if(configName == "DELETED_VALUES_FILE_NAME") {
            strcpy(treeMetaData->deletedValuesFileName, val.c_str());
        }
        else if(configName == "BUFFER_SIZE") {
            treeMetaData->bufferSize = stoi(val);
        }
        else if(configName == "PAGE_SIZE") {
            treeMetaData->pageSize = stoi(val);
        }
        else if(configName == "VALUE_SIZE") {
            treeMetaData->valueSize = stoi(val);
        }
        else if(configName == "MAX_DEGREE") {
            treeMetaData->T = stoi(val);
        }
        else if(configName == "DB_FILE_NAME") {
//            cout << val << endl;
            strcpy(treeMetaData->dbFileName, val.c_str());
        }
    }
    file.close();
}

string getDBFileNameInConfig() {
    ifstream file;
    file.open(configFileName);
    while(!file.eof()) {
        string configName, tmp, val;
        file >> configName >> tmp >> val;
        if(configName == "DB_FILE_NAME") {
            file.close();
            return val;
        }
    }
    return NULL;
}

void WRITE_HEADER_INIT() {
    BTreeStore* treeMetaData = new BTreeStore();
    getConfigValues(treeMetaData);
    
    treeMetaData->rootPos = 0;
    treeMetaData->nextPosNode = treeMetaData->rootPos;
    treeMetaData->nextPosNodeValue = (treeMetaData->rootPos + 1) << 2;
    char* dbFileName = new char[32];
    strcpy(dbFileName, treeMetaData->dbFileName);
    FILE* file = fopen(dbFileName, "wb");
    fwrite(treeMetaData, sizeof(struct BTreeStore), 1, file);
    fclose(file);
    delete[] dbFileName;
    delete treeMetaData;
}

void WRITE_HEADER(BTree* tree) {
    string dbFileName = tree->getDBFileName();
    FILE* file =  fopen(dbFileName.c_str(), "rb+");
    fseek(file, 0, SEEK_SET);
    BTreeStore* treeMetaData = new BTreeStore();
    treeMetaData->T = tree->getMaxDegree();
    treeMetaData->bufferSize = tree->getBufferSize();
    treeMetaData->pageSize = tree->getPageSize();
    treeMetaData->valueSize = tree->getValueSize();
    treeMetaData->rootPos = tree->getRootPos();
    treeMetaData->nextPosNode = tree->getNextPosNode();
    treeMetaData->nextPosNodeValue = tree->getNextPosNodeValue();
    char* deletedNodesFileName = tree->getDeletedNodesFileName();
    char* deletedValuesFileName = tree->getDeletedValuesFileName();
    strcpy(treeMetaData->deletedNodesFileName, deletedNodesFileName);
    strcpy(treeMetaData->deletedValuesFileName, deletedValuesFileName);
    strcpy(treeMetaData->dbFileName, dbFileName.c_str());
    fwrite(treeMetaData, sizeof(struct BTreeStore), 1, file);
    
    fclose(file);
    delete[] deletedNodesFileName;
    delete[] deletedValuesFileName;
    delete treeMetaData;
}

struct BTreeStore* READ_HEADER(BTree* b) {
    string dbFileName = b->getDBFileName();
    FILE* file = fopen(dbFileName.c_str(), "rb");
    BTreeStore* treeMedaData = new BTreeStore();
    fseek(file, 0, SEEK_SET);
    fread(treeMedaData, sizeof(struct BTreeStore), 1, file);
    fclose(file);
    return treeMedaData;
}

void writeEmptyBytes(BTree* b, uint64_t pos, FILE* file) {
    string dbFileName = b->getDBFileName();
    pos = (pos * b->getPageSize()) + b->getHeaderSize();
    if(file == NULL) {
        file = fopen(dbFileName.c_str(), "rb+");
    }
    fseek(file, pos, SEEK_SET);
    char* buff = new char[b->getPageSize()];
    fwrite(buff, b->getPageSize(), 1, file);
    rewind(file);
    delete[] buff;
}

void clearDeletedNodesFile(BTree* tree) {
    ofstream out;
    char* deletedNodesFileNameTmp = tree->getDeletedNodesFileName();
    out.open(deletedNodesFileNameTmp);
    out << 0 << endl;
    delete[] deletedNodesFileNameTmp;
}

void clearDeletedValsFile(BTree* tree) {
    ofstream out;
    char* deletedValuesFileNameTmp = tree->getDeletedValuesFileName();
    out.open(deletedValuesFileNameTmp);
    out << 0 << endl;
    delete[] deletedValuesFileNameTmp;
}

void clearDBFile(BTree* tree) {
    string dbFileName = tree->getDBFileName();
    remove(dbFileName.c_str());
}

void clearDataDBFile(BTree* tree) {
    ofstream out;
    string dbFileName = tree->getDBFileName();
    out.open(dbFileName, std::ofstream::out|std::ofstream::trunc);
    out.close();
}

void flushDeletedNodesFile(BTree* tree) {
    ofstream out;
    char* deletedNodesFileNameTmp = tree->getDeletedNodesFileName();
    queue<uint64_t> deletedNodesPos = tree->getDeletedNodesPos();
    out.open(deletedNodesFileNameTmp);
    out << deletedNodesPos.size() << endl;
    while(!deletedNodesPos.empty()){
        out << deletedNodesPos.front() << endl;
        deletedNodesPos.pop();
    }
    out.close();
    delete[] deletedNodesFileNameTmp;
}

void flushDeletedValsFile(BTree* tree) {
    ofstream out;
    char* deletedValsFileNameTmp = tree->getDeletedValuesFileName();
    queue<uint64_t> deletedValsPos = tree->getDeletedValsPos();
    out.open(deletedValsFileNameTmp);
    out << deletedValsPos.size() << endl;
    while(!deletedValsPos.empty()){
        out << deletedValsPos.front() << endl;
        deletedValsPos.pop();
    }
    out.close();
    delete[] deletedValsFileNameTmp;
}

void readDeletedNodesPos(BTree* tree) {
    char* deletedNodesFileName = tree->getDeletedNodesFileName();
    ifstream checkFile(deletedNodesFileName);
    if(!checkFile) {
        clearDeletedNodesFile(tree);
    }
    else {
        ifstream in;
        queue<uint64_t> q;
        uint64_t tmp, num;
        in.open(deletedNodesFileName);
        in >> num;
        for(int i = 0; i < num; i++) {
            in >> tmp;
            q.push(tmp);
        }
        tree->setDeletedNodesPos(q);
        in.close();
    }
}

void readDeletedValsPos(BTree* tree) {
    char* deletedValsFileName = tree->getDeletedValuesFileName();
    ifstream checkFile(deletedValsFileName);
    if(!checkFile) {
        clearDeletedValsFile(tree);
    }
    else {
        queue<uint64_t> q;
        uint64_t tmp, num;
        ifstream in;
        in.open(deletedValsFileName);
        in >> num;
        for(int i = 0; i < num; i++) {
            in >> tmp;
            q.push(tmp);
        }
        tree->setDeletedValsPos(q);
        in.close();
    }
}

bool checkFileDBExist() {
    string dbFileName = getDBFileNameInConfig();
//    cout << "DB FILE NAME: " << dbFileName << endl;
    ifstream ifile(dbFileName);
    return ifile.good();
}
