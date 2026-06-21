#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "WhiteSpaceInterpreter.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename.ws> [optional_input]" << endl;
        return 1;
    }

    string filename = argv[1];
    string inputData = "";

    if (argc >= 3) {
        inputData = argv[2];
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file: " << filename << endl;
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string code = buffer.str();
    file.close();

    WhiteSpaceInterpreter interpreter;

    try {
        string output = interpreter.interpret(code, inputData);
        cout << output;
    }
    catch (const exception& e) {
        cerr << "\n[Runtime error]: " << e.what() << endl;
        return 1;
    }

    return 0;
}
