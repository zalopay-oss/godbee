#include "FileManager.h"
#include "BTree.h"

FileManager::FileManager(string dbFileName) {
    for(int i = 0; i < filePtrPoolSize; i++) {
        FILE* file = fopen(dbFileName.c_str(), "rb+");
        filePtrPool.push(file);
    }
}

FILE* FileManager::getFilePtr() {
    std::unique_lock<std::mutex> lk(filePtrMutex);
    while(filePtrPool.empty()) {
        cv.wait(lk);
    }
    FILE* file = filePtrPool.front();
    filePtrPool.pop();
    return file;
}

void FileManager::acquireFilePtr() {
    std::unique_lock<std::mutex> lk(filePtrMutex);
    while(filePtrPool.empty()) {
        cv.wait(lk);
    }
    FILE* file = filePtrPool.front();
    std::thread::id tid = this_thread::get_id();
    filePtrs.insert(pair<std::thread::id, FILE*>(tid, file));
    filePtrPool.pop();
}

void FileManager::giveFilePtrBack(FILE* file) {
    {
        std::lock_guard<std::mutex> lk(filePtrMutex);
        filePtrPool.push(file);
    }
    cv.notify_one();
}

FileManager::~FileManager() {
    while(!filePtrPool.empty()) {
        FILE* file = filePtrPool.front();
        fclose(file);
        filePtrPool.pop();
    }
}