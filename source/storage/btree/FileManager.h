#pragma once
#include <map>
#include <shared_mutex>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>

using namespace std;

const uint8_t filePtrPoolSize = 8;

class BTree;

class FileManager {
private:
    map<std::thread::id, FILE*> filePtrs;
    queue<FILE*> filePtrPool;
    mutex filePtrMutex;
    condition_variable cv;
public:
    FileManager(string dbFileName);
    void createNewFilePtr();
    void acquireFilePtr();
    void giveFilePtrBack(FILE* file);
    FILE* getFilePtr();
    ~FileManager();
};