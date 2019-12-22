#include "utils.h"
#include <iostream>
#include <fstream>
#include "BPlusTreeNode.h"
#include "KeyValueStore.h"

using namespace std;

int indexNodeLargerOrEqual(char **array, int size, char *key)
{
    if (size == 0)
        return -1;

    int mid, left = 0, right = size - 1;

    while (left <= right)
    {
        mid = (left + right) / 2;
        int cmp = strcmp(array[mid], key);
        if (cmp == 0)
            return mid;
        if (cmp < 0)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    if (strcmp(array[mid], key) < 0)
        return mid + 1;
    else
        return mid;
}
int indexNodeLarger(char **array, int size, char *key)
{
    if (size == 0)
        return -1;
    int mid, left = 0, right = size - 1;

    while (left <= right)
    {
        mid = (left + right) / 2;
        int cmp = strcmp(array[mid], key);
        if (cmp == 0)
            return mid + 1;
        if (cmp < 0)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    if (strcmp(array[mid], key) < 0)
        return mid + 1;
    else
        return mid;
}

int indexNodeEqual(char **array, int size, char *key)
{
    int res = indexNodeLargerOrEqual(array, size, key);
    return (res == -1 || res >= size || strcmp(array[res], key) != 0) ? -1 : res;
}
bool isExisting(char **array, int size, char *key)
{
 
    return indexNodeEqual(array, size, key) != -1;
}
char *getElm(char **array, int size, char *key)
{
    int index = indexNodeEqual(array, size, key);
    return (index == -1) ? NULL : array[index];
}

char *sdup(char *s, int bufferSize)
{
    char *res = new char[bufferSize];
    strcpy(res, s);
    res[strlen(s)] = '\0';
    return res;
}


bool isExitCommand(char *command)
{
    return strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0 || strcmp(command, "q") == 0;
}

void lowerCase(string &s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] <= 'Z' && s[i] >= 'A')
            s[i] = s[i] - ('Z' - 'z');
    }
}

bool readArg(stringstream & sstream, string& arg1){
    string tmp="";
    arg1="";
    if (sstream >> arg1)
    {
        char startArg = arg1[0];
        if (startArg == '\'' || startArg == '\"')
        {
            arg1.erase(0, 1);
            if (arg1[arg1.size()-1]!=startArg){
                 while (sstream >> tmp && tmp[tmp.size() - 1] != startArg)
                {
                    if (tmp.size() != 1 && tmp.find(startArg) != string::npos)
                        return false;
                    arg1 += " "+tmp;
                    tmp="";
                }
                if (tmp[tmp.size() - 1] != startArg)
                {
                    return false;
                }
                else{
                    if (tmp.size()-1 != tmp.find(startArg) && tmp.find(startArg) != string::npos)
                        return false;
                    arg1 += " "+tmp;
                }
            }
            arg1.erase(arg1.size() - 1, 1);
        }
    }
    return true;
}

bool excuteComand(KeyValueStore *&store, char *command)
{
    if (isExitCommand(command))
        return false;

    string s = command;
    stringstream sstream;
    sstream << s;
    string type, arg1 = "", arg2 = "", tmp = "";
    sstream >> type;

    if (!readArg(sstream,arg1)){
        cout << "Invalid argument(s)" << endl;
        return true;
    }

    if (!readArg(sstream,arg2)){
        cout << "Invalid argument(s)" << endl;
        return true;
    }

    readArg(sstream,tmp);

    if(tmp!=""){
        cout<<"(error) ERR syntax error"<<endl;
        return true;
    }

    char *_arg1 = arg1.size() == 0 ? NULL : strdup(arg1.c_str());
    char *_arg2 = arg2.size() == 0 ? NULL : strdup(arg2.c_str());
    lowerCase(type);


    if (type == "set")
    {
        if (arg1.size() == 0 || arg2.size() == 0)
            cout << "(error) ERR wrong number of arguments for 'set' command" << endl;
        else
        {
            store->set(_arg1, _arg2);
            cout << "OK" << endl;
        }
    }
    else if (type == "get")
    {
        if (arg1.size() == 0)
            cout << "(error) ERR wrong number of arguments for 'get' command" << endl;
        else
        {
            char *res = store->get(_arg1);
            if (!res)
                cout << "(nil)" << endl;
            else
                cout << "\"" << res << "\"" << endl;
                delete[] res;
        }
    }
    else if (type == "exist")
    {
        if (arg1.size() == 0)
            cout << "(error) ERR wrong number of arguments for 'get' command" << endl;
        else
        {
            if (store->exist(_arg1))
                cout << "TRUE" << endl;
            else
                cout << "FALSE" << endl;
        }
    }
    else if (type == "delete"|| type == "remove" || type=="del")
    {
        if (arg1.size() == 0)
            cout << "(error) ERR wrong number of arguments for 'get' command" << endl;
        else
        {
            if (store->remove(_arg1))
                cout << "OK"<<endl;
            else
                cout << "Can not delete." << endl;
        }
    }
    else if (type == "traverse" || type == "t")
    {
        store->traverse();
    }
    else
    {
        cout << "(error) ERR unknown command." << endl;
    }

    if (_arg1)
        free(_arg1);
    if (_arg2)
        free(_arg2);
    return true;
}