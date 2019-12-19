#include "fileUtils.h"


mutex mFiles;
condition_variable cv;

FILE* getpFile(MetaData * metaData){
    unique_lock<mutex> lk(mFiles);
    while(metaData->files.size() == 0){
        cv.wait(lk);
    }
    FILE* res = metaData->files.top();
    metaData->files.pop();
    return res;
}

void returnpFile(MetaData * metaData, FILE* p){
    {
        lock_guard<mutex> lk(mFiles);
        metaData->files.push(p);
    }
    cv.notify_one();
}


uint16_t calculateMaxDegree(uint16_t pageSize, uint16_t keySize, uint16_t pChildSize){

    uint16_t maxDeg = 2;
    const uint16_t SEGMENT_SIZE = 8;
    while (true){
        uint16_t tmpPageSize=0;
        uint16_t padding = 0;

        if (pChildSize > SEGMENT_SIZE){
            padding = SEGMENT_SIZE - pChildSize%SEGMENT_SIZE;
            tmpPageSize += (2*maxDeg-1)*(pChildSize + padding)+pChildSize;
        }
        else {
            uint16_t childPerSegment = SEGMENT_SIZE/pChildSize;
            padding = SEGMENT_SIZE%pChildSize;
            uint16_t noOfChild = 2*maxDeg;
            tmpPageSize += 2*maxDeg*pChildSize + padding*(noOfChild/childPerSegment);
        }
        uint16_t noOfKeys = 2*maxDeg - 1;
        tmpPageSize += noOfKeys*keySize + padding;
        padding = SEGMENT_SIZE - (noOfKeys*keySize - padding)%SEGMENT_SIZE;
        //if padding is fit for flag and size
        if (padding >= 3)
            tmpPageSize+=padding;
        else
            tmpPageSize+=padding + SEGMENT_SIZE;
        if (tmpPageSize > pageSize){
            return maxDeg-1;
        }
         if (tmpPageSize == pageSize){
            return maxDeg;
        }
        maxDeg++;
    }
}

uint8_t countBits(uint16_t n)
{
   uint8_t count = 0;
   while (n)
   {
        count++;
        n >>= 1;
    }
    return count;
}


uint8_t calculateBitSizeValueIndex(uint16_t pageSize, uint16_t valueSize){
    int n = pageSize / valueSize - 1;
    if (n<0) return 0;
    return (uint8_t)countBits(n);
}

string getDBFileName(string fileConfigName){
    ifstream myfile;
    string type = "", tmp;
    string value;
    myfile.open(fileConfigName);
    while (myfile >> type >> tmp >> value)
    {
        if (type == "DB_FILE_NAME")
            return value;
    }

    return "";
}

HeaderPage initialFileWithConfig(string fileName, string fileConfigName)
{
    ifstream myfile;
    HeaderPage headerPage;
    string type = "", tmp;
    string value;
    myfile.open(fileConfigName);
    while (myfile >> type >> tmp >> value)
    {
        if (type == "KEY_SIZE")
            headerPage.keySize = stoi(value);
        if (type == "VALUE_SIZE")
            headerPage.valueSize = stoi(value);
        if (type == "PAGE_SIZE")
            headerPage.pageSize = stoi(value);
        if (type == "DEL_NODE_FILE_NAME")
            strcpy(headerPage.deletedNodeFileName, value.c_str());
        if (type == "DEL_VALUE_FILE_NAME")
            strcpy(headerPage.deletedvalueFileName, value.c_str());
        if (type == "DB_FILE_NAME")
            strcpy(headerPage.dbFilename, value.c_str());
    }
    myfile.close();
    headerPage.currentValueNode = 0;
    headerPage.root = 0;
    FILE *file;

    file = fopen(headerPage.deletedNodeFileName, "w+");
    fclose(file);

    file = fopen(headerPage.dbFilename, "w+");
    fseek(file, 0, SEEK_SET);
    fwrite(&headerPage, sizeof(headerPage), 1, file);
    fseek(file, 0, SEEK_END);
    fclose(file);

    return headerPage;
}

HeaderPage getHeaderPage(string fileName)
{
    FILE *file = fopen(fileName.c_str(), "rb+");
    HeaderPage res;
    fseek(file, 0, SEEK_SET);
    fread(&res, sizeof(HeaderPage), 1, file);
    fclose(file);
    return res;
}

bool existFile(string fileName)
{
    return access(fileName.c_str(), F_OK) != -1;
}

// void updateRoot(uint64_t newRoot, HeaderPage &currentHeaderPage, FILE *file);
void updateRoot(uint64_t newRoot, HeaderPage &currentHeaderPage, MetaData *metaData){
    currentHeaderPage.root = newRoot;
    FILE * file = getpFile(metaData);
    fseek(file, 0, SEEK_SET);
    fwrite(&currentHeaderPage, sizeof(HeaderPage), 1, file);
    returnpFile(metaData, file);
}
void updateHeader(HeaderPage &currentHeaderPage, MetaData *metaData)
{
    FILE * file = getpFile(metaData);
    fseek(file, 0, SEEK_SET);
    fwrite(&currentHeaderPage, sizeof(HeaderPage), 1, file);
    returnpFile(metaData, file);
}

// uint64_t calculateRealOffset(uint64_t offSet)
// {
//     return ((offSet >> 2) << 12) | ((offSet & 3) << 10) | sizeof(HeaderPage);
// }

uint64_t calculateRealOffset(uint64_t offSet, MetaData * metaData)
{
    uint64_t valueIdx = offSet & ((1<<metaData->bSizeOfValueIdx)-1);
    uint64_t nodeID = offSet >> metaData->bSizeOfValueIdx;
    return nodeID*metaData->pageSize + valueIdx*metaData->valueSize + sizeof(HeaderPage);
    // return ((offSet >> bitSizeValueIdx) << 12) | ((offSet & tmp) << 10) | sizeof(HeaderPage);
}

uint64_t calculatePageNodeID(uint64_t realOffSet, MetaData * metaData)
{
    realOffSet -= sizeof(HeaderPage);
    return (realOffSet / metaData->pageSize)<<metaData->bSizeOfValueIdx;
}

// uint64_t calculatePageNodeID(uint64_t realOffSet)
// {
//     return (realOffSet ^ sizeof(HeaderPage)) >> 12 << 2;
// }

uint64_t calculateValueNodeID(uint64_t realOffSet, MetaData * metaData)
{
    realOffSet -= sizeof(HeaderPage);
    int valueIndex = realOffSet & ((1<<metaData->bSizeOfValueIdx)-1);
    int nodeIdx = (realOffSet >> metaData->bSizeOfValueIdx) / metaData->pageSize;
    return (nodeIdx << metaData->bSizeOfValueIdx) ^ valueIndex;
}

// uint64_t calculateValueNodeID(uint64_t offSet)
// {
//     return ((offSet ^ sizeof(HeaderPage)) >> 12 << 2) | ((offSet ^ sizeof(HeaderPage)) >> 10 & 3);
// }

uint64_t getNextFreeOffset(MetaData *metaData, FILE *file)
{
    fseek(file, 0, SEEK_END);
    uint64_t eof = ftell(file);
    char k = '0';
    fwrite(&k, sizeof(char), metaData->pageSize, file);
    return ((eof - sizeof(HeaderPage)) / metaData->pageSize) << metaData->bSizeOfValueIdx;
}


uint64_t calculateNextEmtyValueNode(MetaData *metaData, FILE * file)
{
    uint64_t valueId = metaData->emptyValueNode;
    uint64_t realOffset = calculateRealOffset(valueId,metaData);
    uint64_t nextPos = ((realOffset - sizeof(HeaderPage)) % metaData->pageSize) / metaData->valueSize+1;
    if (valueId == 0 || nextPos == ceil(1.0*metaData->pageSize / metaData->valueSize ))//no empty slot
    {
        metaData->emptyValueNode = getNextFreeOffset(metaData,file);
    }
    else
    {
        metaData->emptyValueNode = (calculatePageNodeID(realOffset,metaData)) | (nextPos);
    }
    return metaData->emptyValueNode;
}


BPlusTreeNode *getBNodeFromFile(MetaData *metaData, uint64_t offset)
{
    FILE * file = getpFile(metaData);
    fseek(file, calculateRealOffset(offset,metaData), SEEK_SET);

    NodePage *nodePage = new NodePage(metaData);

    int readTimes = ceil(1.0 * metaData->pageSize / IO_BUFFER_SIZE);
    char buffer[IO_BUFFER_SIZE];
    char tmp[IO_BUFFER_SIZE] = "";

    uint16_t mD = metaData->maxDegree;
    enum Stage
    {
        CHILDS,
        KEYS,
        FLAG,
        SIZE,
        NONE
    };

    uint64_t stageReadSize = 0;
    uint64_t idx = 0;
    Stage stage = Stage::CHILDS;

    uint64_t keysSize = (2 * mD - 1) * metaData->keySize;
    uint32_t valueSize = 2 * mD * sizeof(uint64_t);
    uint8_t pageSize = sizeof(uint8_t);
    uint8_t sizeSz = sizeof(uint16_t);
    uint16_t remainingSize = 0;

    for (int i = 0; i < readTimes; i++)
    {
        fread(&buffer, IO_BUFFER_SIZE, 1, file);
        stageReadSize += IO_BUFFER_SIZE;
        int pos = 0;
        switch (stage)
        {
        case CHILDS:
            if (remainingSize > 0)
            {
                int rPos = 0;
                while (remainingSize - rPos > sizeof(uint64_t))
                {
                    memcpy(&nodePage->childs[idx], tmp + rPos, sizeof(uint64_t));
                    idx++;
                    rPos += sizeof(uint64_t);
                }
                memcpy(tmp + remainingSize, buffer, sizeof(uint64_t) - (remainingSize - rPos));
                memcpy(&nodePage->childs[idx], tmp + rPos, sizeof(uint64_t));
                idx++;
                remainingSize = 0;
                pos += sizeof(uint64_t) - remainingSize;
            }
            if (stageReadSize < valueSize)
            {
                while (idx * sizeof(uint64_t) < IO_BUFFER_SIZE)
                {
                    memcpy(&nodePage->childs[idx], buffer + pos, sizeof(uint64_t));
                    idx++;
                    pos += sizeof(uint64_t);
                }
                break;
            }
            else
            {
                while (idx < (2 * mD))
                {
                    memcpy(&nodePage->childs[idx], buffer + pos, sizeof(uint64_t));
                    idx++;
                    pos += sizeof(uint64_t);
                }
                stage = KEYS;
                idx = 0;
                stageReadSize -= valueSize;
            }
        case KEYS:
            if (remainingSize > 0)
            {
                int rPos = 0;
                while (remainingSize - rPos > metaData->keySize)
                {
                    memcpy(nodePage->keys[idx], tmp + rPos, metaData->keySize);
                    idx++;
                    rPos += metaData->keySize;
                }
                memcpy(tmp + remainingSize, buffer, metaData->keySize - (remainingSize - rPos));
                memcpy(nodePage->keys[idx], tmp + rPos, metaData->keySize);
                idx++;
                pos += (metaData->keySize - remainingSize);
                // stageReadSize += (metaData->keySize - remainingSize);
                remainingSize = 0;
            }

            if (stageReadSize < keysSize)
            {
                while ((idx + 1) * metaData->keySize < stageReadSize)
                {
                    memcpy(nodePage->keys[idx], buffer + pos, metaData->keySize);
                    idx++;
                    pos += metaData->keySize;
                }
                // pos-=metaData->keySize;
                uint16_t restSize = IO_BUFFER_SIZE - pos;
                if (restSize > 0)
                {
                    memcpy(tmp, buffer + pos, restSize);
                    remainingSize = restSize;
                }
                break;
            }
            else
            {
                while (idx < (2 * mD - 1))
                {
                    memcpy(nodePage->keys[idx], buffer + pos, metaData->keySize);
                    idx++;
                    pos += metaData->keySize;
                }
                stage = FLAG;
                stageReadSize -= keysSize;
            }
        case FLAG:
            if (stageReadSize < pageSize)
                break;
            else
            {
                memcpy(&nodePage->flag, buffer + pos, sizeof(uint8_t));
                stageReadSize -= pageSize;
                pos += sizeof(uint8_t);
                stage = SIZE;
            }
        case SIZE:
            if (stageReadSize < sizeSz)
                break;
            else
            {
                memcpy(&nodePage->size, buffer + pos, sizeof(uint16_t));
                stageReadSize -= sizeSz;
                pos += sizeof(uint16_t);
                stage = NONE;
                break;
            }
        default:
            break;
        }
    }

    BPlusTreeNode *bnode = NULL;
    bnode = new BPlusTreeNode(metaData, nodePage, offset);
    delete nodePage;
    returnpFile(metaData, file);

    return bnode;
}

char *getValueFromFile(MetaData *metaData, uint64_t offset)
{
    FILE * file = getpFile(metaData);
    fseek(file, calculateRealOffset(offset,metaData), SEEK_SET);
    char *value = new char[metaData->valueSize];
    uint64_t readSize = 0;
    while (readSize < metaData->valueSize)
    {
        char buffer[IO_BUFFER_SIZE];
        uint64_t size = IO_BUFFER_SIZE < (metaData->valueSize - readSize) ? IO_BUFFER_SIZE : (metaData->valueSize - readSize);
        fread(buffer, size, 1, file);
        memcpy(value + readSize, buffer, size);
        readSize += size;
    }
    returnpFile(metaData, file);
    return value;
}

void writeNodePageToFile(FILE * file, MetaData *metaData, NodePage *npage)
{
    char buffer[IO_BUFFER_SIZE + 1];
    uint64_t copyedSize = 0;
    int idx = 0;
    int tmp = 0;
    while (idx < 2 * metaData->maxDegree)
    {
        if (copyedSize + sizeof(uint64_t) > IO_BUFFER_SIZE)
        {
            memcpy(buffer + copyedSize, &npage->childs[idx], sizeof(uint64_t) - (copyedSize + sizeof(uint64_t) - IO_BUFFER_SIZE));
            fwrite(buffer, IO_BUFFER_SIZE, 1, file);
            tmp += IO_BUFFER_SIZE;
            copyedSize = sizeof(uint64_t) - (copyedSize + sizeof(uint64_t) - IO_BUFFER_SIZE);
            memcpy(buffer + 0, &npage->childs[idx] + copyedSize, sizeof(uint64_t) - copyedSize);
            copyedSize = sizeof(uint64_t) - copyedSize;
        }
        else
        {
            memcpy(buffer + copyedSize, &npage->childs[idx], sizeof(uint64_t));
            copyedSize += sizeof(uint64_t);
            if (copyedSize == IO_BUFFER_SIZE)
            {
                fwrite(buffer, IO_BUFFER_SIZE, 1, file);
                tmp += IO_BUFFER_SIZE;
                copyedSize = 0;
            }
        }
        idx++;
    }
    idx = 0;
    uint16_t keySize = metaData->keySize;
    while (idx < 2 * metaData->maxDegree - 1)
    {
        if (copyedSize + keySize > IO_BUFFER_SIZE)
        {
            memcpy(buffer + copyedSize, npage->keys[idx], keySize - (copyedSize + keySize - IO_BUFFER_SIZE));
            fwrite(buffer, IO_BUFFER_SIZE, 1, file);
            tmp += IO_BUFFER_SIZE;
            copyedSize = keySize - (copyedSize + keySize - IO_BUFFER_SIZE);
            memcpy(buffer + 0, npage->keys[idx] + copyedSize, keySize - copyedSize);
            copyedSize = keySize - copyedSize;
        }
        else
        {
            memcpy(buffer + copyedSize, npage->keys[idx], keySize);
            copyedSize += keySize;
            if (copyedSize == IO_BUFFER_SIZE)
            {
                fwrite(buffer, IO_BUFFER_SIZE, 1, file);
                tmp += IO_BUFFER_SIZE;
                copyedSize = 0;
            }
        }
        idx++;
    }

    if (copyedSize + sizeof(npage->flag) > IO_BUFFER_SIZE)
    {
        memcpy(buffer + copyedSize, &npage->flag, sizeof(npage->flag) - (copyedSize + sizeof(npage->flag) - IO_BUFFER_SIZE));
        fwrite(buffer, IO_BUFFER_SIZE, 1, file);
        tmp += IO_BUFFER_SIZE;
        copyedSize = sizeof(npage->flag) - (copyedSize + sizeof(npage->flag) - IO_BUFFER_SIZE);
        memcpy(buffer + 0, &npage->flag + copyedSize, sizeof(npage->flag) - copyedSize);
        copyedSize = sizeof(npage->flag) - copyedSize;
    }
    else
    {
        memcpy(buffer + copyedSize, &npage->flag, sizeof(npage->flag));
        copyedSize += sizeof(npage->flag);
        if (copyedSize == IO_BUFFER_SIZE)
        {
            fwrite(buffer, IO_BUFFER_SIZE, 1, file);
            tmp += IO_BUFFER_SIZE;
            copyedSize = 0;
        }
    }
    if (copyedSize + sizeof(npage->size) > IO_BUFFER_SIZE)
    {
        memcpy(buffer + copyedSize, &npage->size, sizeof(npage->size) - (copyedSize + sizeof(npage->size) - IO_BUFFER_SIZE));
        fwrite(buffer, IO_BUFFER_SIZE, 1, file);
        tmp += IO_BUFFER_SIZE;
        copyedSize = sizeof(npage->size) - (copyedSize + sizeof(npage->size) - IO_BUFFER_SIZE);
        memcpy(buffer + 0, &npage->size + copyedSize, sizeof(npage->size) - copyedSize);
        copyedSize = sizeof(npage->size) - copyedSize;
    }
    else
    {
        memcpy(buffer + copyedSize, &npage->size, sizeof(npage->size));
        copyedSize += sizeof(npage->size);
        if (copyedSize == IO_BUFFER_SIZE)
        {
            fwrite(buffer, IO_BUFFER_SIZE, 1, file);
            tmp += IO_BUFFER_SIZE;
            copyedSize = 0;
        }
    }
    if (copyedSize != 0)
    {
        fwrite(buffer, copyedSize, 1, file);
        tmp += copyedSize;
    }
}

void flushNodePageToFile(MetaData *metaData, NodePage *npage, uint64_t offset)
{
    FILE *file = getpFile(metaData);
    fseek(file, calculateRealOffset(offset,metaData), SEEK_SET);
    // fwrite(npage, metaData->pageSize, 1, metaData->file);
    writeNodePageToFile(file, metaData, npage);
    returnpFile(metaData, file);
}

uint64_t appendNodePageToFile(MetaData *metaData, NodePage *npage)
{
    uint64_t nodeID = 0;
    FILE * file = getpFile(metaData);
    if (!metaData->delNodePages->pop(nodeID))
    {
        fseek(file, 0, SEEK_END);
        nodeID = getNextFreeOffset(metaData,file);
    }
    fseek(file, calculateRealOffset(nodeID,metaData), SEEK_SET);
    writeNodePageToFile(file, metaData, npage);
    returnpFile(metaData,file);
    return nodeID;
}

uint64_t insertNewValueToFile(char *value, MetaData *metaData)
{
    FILE * file = getpFile(metaData);
    uint64_t valueID;
    if (!metaData->delValues->pop(valueID))
    {
        valueID = calculateNextEmtyValueNode(metaData,file);
        metaData->emptyValueNode = valueID;
    }
    fseek(file, calculateRealOffset(valueID,metaData), SEEK_SET);
    fwrite(value, metaData->valueSize, 1, file);
    returnpFile(metaData,file);
    return valueID;
}

void updateValueToFile(char *value, uint64_t offSet, MetaData *metaData)
{
    FILE * file = getpFile(metaData);
    fseek(file, calculateRealOffset(offSet,metaData), SEEK_SET);
    fwrite(value, metaData->valueSize, 1, file);
    returnpFile(metaData, file);
}
