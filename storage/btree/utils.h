#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <string.h>
#include <algorithm>
#include <map>

using namespace std;

vector<string> extractCommand(string command);
char* convertToken(string token);
char* newKey(uint16_t keySize);
