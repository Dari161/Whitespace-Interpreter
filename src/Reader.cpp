#include <iostream>
#include "Reader.h"

using namespace std;

int Reader::hexCharToInt(const char hexChar) const {
    if (hexChar >= '0' && hexChar <= '9') return hexChar - '0';
    if (hexChar >= 'A' && hexChar <= 'F') return hexChar - 'A' + 10;
    if (hexChar >= 'a' && hexChar <= 'f') return hexChar - 'a' + 10;

    throw runtime_error("Invalid hexadecimal character");
}

char Reader::readChar() {
    if (input[pos] == '\0') throw runtime_error("Input was empty when trying to read a character");
    char ret = input[pos];
    ++pos;
    return ret;
}

int Reader::readNum() {
    if (input[pos] == '\0') throw runtime_error("Input was empty when trying to read number");
    if (input[pos] == '0') {
        ++pos;
        if (input[pos] == '\n') {
            ++pos;
            return 0;
        }
        // hex number
        if (input[pos] == 'x') {
            ++pos; // eat 'x'
            int num = 0;
            while (input[pos] != '\n') {
                num *= 16;
                num += hexCharToInt(input[pos]);
                ++pos;
            }
            ++pos; // eat '\n'
            return num;
        }
        throw runtime_error("Input contains unexpected character when trying to read number");
    }
    int num = 0;
    while (input[pos] != '\n') {
        num *= 10;
        if (input[pos] < '0' || input[pos] > '9') throw runtime_error("Invalid decimal character");
        num += input[pos] - '0';
        ++pos;
    }
    ++pos; // eat '\n'
    return num;
}
