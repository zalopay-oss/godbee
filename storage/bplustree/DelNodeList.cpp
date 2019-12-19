#include "DelNodeList.h"

using namespace std;

DelNodeList::DelNodeList(string fileName, uint16_t bufferSize, bool createNewFile)
{
    file=NULL;
    if (createNewFile)
    {
        file = fopen(fileName.c_str(), "w+");
        fseek(file, 0, SEEK_SET);
        uint64_t tmpSize = 0;
        fwrite(&tmpSize, sizeof(uint64_t), 1, file);
        fclose(file);
        file = fopen(fileName.c_str(), "rb+");
    }
    else
    {
        file = fopen(fileName.c_str(), "rb+");
        uint64_t size = getSize();
        dList.clear();
        for (int i = 0; i < size; i++)
            dList.push_back(getValue(i));
    }
}

void DelNodeList::push(uint64_t value)
{
    setSize(dList.size() + 1);
    setValue(value, dList.size());
    dList.push_back(value);
}
bool DelNodeList::pop(uint64_t &result)
{
    if (dList.size() == 0)
        return false;
    result = dList.back();
    setSize(dList.size() - 1);
    dList.pop_back();
    return true;
}
DelNodeList::~DelNodeList()
{
    if (file)
    {
        fclose(file);
        file=NULL;
    }
}

uint64_t DelNodeList::getSize()
{
    uint64_t res;
    fseek(file, 0, SEEK_SET);
    fread(&res, sizeof(uint64_t), 1, file);
    return res;
}
void DelNodeList::setSize(uint64_t newSize)
{
    fseek(file, 0, SEEK_SET);
    fwrite(&newSize, sizeof(uint64_t), 1, file);
}
uint64_t DelNodeList::getValue(uint64_t offset)
{
    uint64_t res;
    fseek(file, (offset + 1) * sizeof(uint64_t), SEEK_SET);
    fread(&res, sizeof(uint64_t), 1, file);
    return res;
}
void DelNodeList::setValue(uint64_t value, uint64_t offset)
{
    fseek(file, (offset + 1) * sizeof(uint64_t), SEEK_SET);
    fwrite(&value, sizeof(uint64_t), 1, file);
}