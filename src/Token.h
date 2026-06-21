#ifndef TOKEN_H
#define TOKEN_H

class WhiteSpaceInterpreter;

struct Token {
    void(WhiteSpaceInterpreter::*function)();
    int value;
};

#endif // TOKEN_H
