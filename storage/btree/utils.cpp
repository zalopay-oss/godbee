#include "utils.h"

vector<string> extractCommand(string command) {
    istringstream ss(command);
    string token;
    vector<string> tokenList;
    while(ss >> quoted(token)) {
        tokenList.push_back(token);
    }
    return tokenList;
}

char* convertToken(string token) {
    char* convertedToken = new char[1024];
    strcpy(convertedToken, token.c_str());
    return convertedToken;
}

char* newKey(uint16_t keySize) {
    return new char[keySize];
}