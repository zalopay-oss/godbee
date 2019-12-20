#pragma once

#include <unistd.h>
#include <list>
#include <stdint.h>
#include <iostream>
#include <string>
// #include <fstream>

using namespace std;

class DelNodeList
{
private:
    FILE * file = NULL;
    list<uint64_t> dList;
    uint16_t bufferSize;

    uint64_t getSize();
    void setSize(uint64_t newSize);
    uint64_t getValue(uint64_t offset);
    void setValue(uint64_t value, uint64_t offset);

public:
    DelNodeList(string fileName, uint16_t bufferSize, bool createNewFile = false);

    void push(uint64_t value);
    bool pop(uint64_t& result);
    ~DelNodeList();
};