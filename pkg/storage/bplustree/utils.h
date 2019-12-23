#pragma once
#include <sstream>
#include <stdint.h>
#include <stdio.h>

const int COMMAND_GET = 1;
const int COMMAND_SET = 2;
const int COMMAND_TRAVERSE = 3;
const int COMMAND_ISEXIST = 4;
const int COMMAND_REMOVE = 5;
const int COMMAND_EXIT = 0;

class KeyValueStore;

int indexNodeLargerOrEqual(char **array, int size, char* key);
int indexNodeLarger(char **array, int size, char* key);
int indexNodeEqual(char **array, int size, char* key);
bool isExisting(char **array, int size, char* key);
char* getElm(char **array, int size, char* key);
bool isExitCommand(char * command);
bool excuteComand(KeyValueStore*& store,char * command);

char *sdup(char * s, int bufferSize);
