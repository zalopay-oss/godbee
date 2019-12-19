#pragma once
#include <string>

using namespace std;

class KeyValueStore
{
public:
    virtual char* get(char* key) = 0;
    virtual void set(char *key, char *value) = 0;
    virtual bool exist(char* key) = 0;
    virtual bool remove(char* key) = 0;
    virtual string traverse() = 0;

    virtual ~KeyValueStore()
    {
    }
};
