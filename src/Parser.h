#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "WhiteSpaceInterpreter.h"
#include <map>

// Lex and parse in one go
class Parser
{
private:
    std::string code;
    size_t pos;

    int makeNumber();
    int makeLabel();

public:
    Parser(const std::string& code_) : code(code_), pos(0) {}
    const std::vector<Token> parse();
};

#endif // PARSER_H
