#pragma once

#include <string>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "BPlusTreeNode.h"
#include "DelNodeList.h"
#include <unistd.h>
#include <list>
#include <condition_variable>
#include <stack>
#include <mutex>
#include <thread>
#include <map>

using namespace std;

class BPlusTreeNode;

const int IO_BUFFER_SIZE = 4096;
const int NO_PFILES = 8;

struct HeaderPage
{
    uint64_t root;
    uint64_t currentValueNode;
    uint16_t pageSize;
    uint16_t keySize;
    uint16_t valueSize;
    char deletedNodeFileName[200];
    char deletedvalueFileName[200];
    char dbFilename[200];
};

uint16_t calculateMaxDegree(uint16_t pageSize, uint16_t keySize, uint16_t pChildSize);
uint8_t calculateBitSizeValueIndex(uint16_t pageSize, uint16_t valueSize);
struct MetaData
{
    uint16_t maxDegree;
    /*
     * Offset Structure |NODE ID|Index Of value in Node|
     * bSizeOfValueIdx: bit size of 2nd part in Offset Structure
     */
    uint8_t bSizeOfValueIdx;
    uint16_t &valueSize;
    uint16_t &keySize;
    uint16_t &pageSize;
    uint64_t &emptyValueNode;
    // FILE *file;
    // map<thread::id, FILE *> files;
    stack<FILE *> files;
    const char * filename;
    DelNodeList *delNodePages;
    DelNodeList *delValues;
    MetaData(HeaderPage *hp, bool createNewMetaData = false)
    :valueSize(hp->valueSize), filename(hp->dbFilename),
    keySize(hp->keySize),pageSize(hp->pageSize),emptyValueNode(hp->currentValueNode)
    {
        maxDegree = calculateMaxDegree(hp->pageSize,hp->keySize,sizeof(uint64_t));
        bSizeOfValueIdx =  calculateBitSizeValueIndex(hp->pageSize, hp->valueSize);
        delNodePages = new DelNodeList(hp->deletedNodeFileName,hp->pageSize,createNewMetaData);
        delValues = new DelNodeList(hp->deletedvalueFileName,hp->pageSize,createNewMetaData);

        for (int i = 0; i < NO_PFILES;i++){
            files.push(fopen(filename,"rb+"));
        }
    }
    ~MetaData(){
        delete delNodePages;
        delete delValues;
        for (int i = 0; i< files.size();i++){
            FILE* pfile = files.top();
            files.pop();
            fclose(pfile);
        }
    }
};

struct NodePage
{
    uint64_t *childs;
    char **keys;
    uint8_t flag;
    uint16_t size;
    uint16_t maxDegree;
    NodePage(MetaData *metaData, uint8_t _flag = 0) : flag(_flag), size(0)
    {
        maxDegree = metaData->maxDegree;
        keys = new char *[2 * (metaData->maxDegree) - 1];
        childs = new uint64_t[2 * (metaData->maxDegree)];
        for (int i = 0; i < 2 * (metaData->maxDegree) - 1; i++)
        {
            keys[i] = new char[metaData->keySize];
            childs[i] = 0;
        }
        childs[2* metaData->maxDegree-1] = 0;
    };

    ~NodePage(){
        for (int i = 0; i < 2 * (maxDegree) - 1; i++)
        {
            delete[] keys[i];
        }
        delete[]keys;
        delete[]childs;
    }

};


string getDBFileName(string fileConfigName);
HeaderPage initialFileWithConfig(string fileName, string fileConfigName);
HeaderPage getHeaderPage(string fileName);
bool existFile(string fileName);

BPlusTreeNode *getBNodeFromFile(MetaData *metaData, uint64_t offset);
char *getValueFromFile(MetaData *metaData, uint64_t offset);

/*
 * Update Node To File
 */
void flushNodePageToFile(MetaData *metaData, NodePage *npage, uint64_t offset);

/*
 * Append New Node to File.
 * Return the Offset
 */
uint64_t appendNodePageToFile(MetaData *metaData, NodePage *npage);

// void updateRoot(uint64_t newRoot, HeaderPage &currentHeaderPage, FILE *file);
void updateRoot(uint64_t newRoot, HeaderPage &currentHeaderPage, MetaData *metaData);
void updateHeader(HeaderPage &currentHeaderPage, MetaData *metaData);

uint64_t insertNewValueToFile(char *value, MetaData *metaData);
void updateValueToFile(char *value, uint64_t offSet, MetaData *metaData);

// FILE* getpFile(MetaData * metaData);
// void returnpFile(MetaData * metaData, FILE*);

