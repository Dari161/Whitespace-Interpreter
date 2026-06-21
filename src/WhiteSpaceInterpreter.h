#ifndef WHITESPACEINTERPRETER_H
#define WHITESPACEINTERPRETER_H

#include <iostream>
#include <map>
#include <vector>
#include "Reader.h"
#include "Token.h"
#include "Parser.h"

#ifndef FRIEND_TEST
#define FRIEND_TEST(test_case, test_name)
#endif

class WhiteSpaceInterpreter {
    // Memory alloc errors are handled by the map and vector class themselves

private:

    struct State {
        std::map<int, int> heap;
        std::vector<int> mystack;
        std::vector<size_t> callStack;
        std::string output = "";
        size_t pc = 0; // program counter
        Reader myReader;
    } s;

    int arg = 0;
    bool m_debug = false;

    // helper function that returns and removes the back of mystack
    int pop();

    std::string intToStr(int num);

    // Stack Manipulation
    void stack_push();
    void stack_duplicate();
    void stack_discard();
    void stack_duplicateTop();
    void stack_swapTop();
    void stack_discardTop();
    // Arithmetic
    void arith_add();
    void arith_sub();
    void arith_mult();
    void arith_div();
    void arith_mod();
    // Heap access
    void heap_set();
    void heap_get();
    // Input/Output
    void output_char();
    void output_num();
    void input_char();
    void input_num();
    // Flow Control
    void control_call();
    void control_jump();
    void control_jumpIfZero();
    void control_jumpIfNegative();
    void control_return();
    void control_exit(); // end program
    void unexpected_endOfProgram();

    std::string removeComments(const std::string& code) const;

    std::string execute(const std::vector<Token>& tokens);

public:
    void setDebug(bool enabled) { m_debug = enabled; }

    static std::string getTokenName(const Token token);
    void printTokens(const std::vector<Token>& tokens);
    void printCode(const std::string& code, bool readableFormat = true);

    std::string interpret(const std::string& code, const std::string& inp = "");

    friend class Parser;
};

#endif // WHITESPACEINTERPRETER_H