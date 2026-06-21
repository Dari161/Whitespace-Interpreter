
#include <iostream>
#include <map>
#include <vector>
#include <cmath> // for floor()
#include "Reader.h"
#include "WhiteSpaceInterpreter.h"
#include "Parser.h"
#include "Token.h"

using namespace std;

int WhiteSpaceInterpreter::pop() {
    int res = s.mystack.back();
    s.mystack.pop_back();
    return res;
}

// decimal
string WhiteSpaceInterpreter::intToStr(int num) {
    if (num == 0) return "0";
    
    bool isNegative = num < 0;
    if (isNegative) {
        num = -num;
    }

    string res = "";
    while (num > 0) {
        res += char((num % 10) + '0');
        num /= 10;
    }
    
    if (isNegative) {
        res += '-';
    }

    reverse(res.begin(), res.end());
    return res;
}

// Stack Manipulation
void WhiteSpaceInterpreter::stack_push() {
    s.mystack.push_back(arg);
    ++s.pc;
}

void WhiteSpaceInterpreter::stack_duplicate() {
    if (arg >= s.mystack.size()) throw runtime_error("Range out of bounds");
    s.mystack.push_back(s.mystack[s.mystack.size() - arg - 1]);
    ++s.pc;
}

void WhiteSpaceInterpreter::stack_discard() {
    if (arg < 0 || arg >= s.mystack.size()) { // remove everything but the top value
        if (s.mystack.size() < 0) throw runtime_error("Stack is empty");
        int top = pop();
        s.mystack.clear();
        s.mystack.push_back(top);
        ++s.pc;
        return;
    }
    int top = pop();
    // Resize the vector to remove the last n elements
    s.mystack.resize(s.mystack.size() - arg);
    s.mystack.push_back(top);
    ++s.pc;
}

void WhiteSpaceInterpreter::stack_duplicateTop() {
    if (s.mystack.size() < 1) throw runtime_error("Can't duplicate top of empty stack");
    s.mystack.push_back(s.mystack[s.mystack.size() - 1]);
    ++s.pc;
}

void WhiteSpaceInterpreter::stack_swapTop() {
    if (s.mystack.size() < 2) throw runtime_error("Must have 2 values on stack to swap them");
    int tmp = s.mystack[s.mystack.size() - 1];
    s.mystack[s.mystack.size() - 1] = s.mystack[s.mystack.size() - 2];
    s.mystack[s.mystack.size() - 2] = tmp;
    ++s.pc;
}

void WhiteSpaceInterpreter::stack_discardTop() {
    if (s.mystack.size() < 1) throw runtime_error("Can't discard top of empty stack");

    s.mystack.pop_back();

    ++s.pc;
}

// Arithmetic
void WhiteSpaceInterpreter::arith_add() {
    if (s.mystack.size() < 2) throw runtime_error("Must have 2 values on stack to add them");
    s.mystack.push_back(pop() + pop());
    ++s.pc;
}

void WhiteSpaceInterpreter::arith_sub() {
    if (s.mystack.size() < 2) throw runtime_error("Must have 2 values on stack to subtract them");
    s.mystack.push_back(-pop() + pop());
    ++s.pc;
}

void WhiteSpaceInterpreter::arith_mult() {
    if (s.mystack.size() < 2) throw runtime_error("Must have 2 values on stack to multiply them");
    s.mystack.push_back(pop() * pop());
    ++s.pc;
}

void WhiteSpaceInterpreter::arith_div() {
    if (s.mystack.size() < 2) throw runtime_error("Must have 2 values on stack to divide them");
    float a = static_cast<float>(pop());
    float b = static_cast<float>(pop());
    if (a == 0) throw runtime_error("Division by 0");
    s.mystack.push_back(static_cast<int>(floor(b / a))); // floor is needed so division can floor-division can wrok for negatives too
    ++s.pc;
}

void WhiteSpaceInterpreter::arith_mod() {
    if (s.mystack.size() < 2) throw runtime_error("Must have 2 values on stack to modulo them");
    int a = pop();
    int b = pop();
    if (a == 0) throw runtime_error("Modulo by 0");

    int mod = b % a; // mod always gets the sign of b
    // 5 mod -3 should be -1
    if ((mod > 0 && a < 0) || (mod < 0 && a > 0)) {
        mod += a;
    }

    s.mystack.push_back(mod);
    ++s.pc;
}

// Heap access
void WhiteSpaceInterpreter::heap_set() {
    if (s.mystack.size() < 2) throw runtime_error("Must have 2 values on stack to pop them");
    int a = pop();
    s.heap[pop()] = a;
    // in "heap[pop()] = pop()" the order of function evaluation is unspecified, meaning it depends on the complier
    ++s.pc;
}

void WhiteSpaceInterpreter::heap_get() {
    if (s.mystack.size() < 1) throw runtime_error("Must have 2 values on stack to pop them");
    map<int, int>::iterator it = s.heap.find(pop());
    if (it == s.heap.end()) throw runtime_error("No such adress in heap exists");
    s.mystack.push_back(it->second); // it->second is the value
    ++s.pc;
}

// Input/Output
void WhiteSpaceInterpreter::output_char() {
    if (s.mystack.size() < 1) throw runtime_error("Must have a values on stack to output it as a char");
    //output as char
    s.output += static_cast<char>(pop() % 256);
    ++s.pc;
}

void WhiteSpaceInterpreter::output_num() {
    if (s.mystack.size() < 1) throw runtime_error("Must have a values on stack to output it as a number");
    //output as num
    s.output += intToStr(pop());
    ++s.pc;
}

void WhiteSpaceInterpreter::input_char() {
    if (s.mystack.size() < 1) throw runtime_error("Must have a values on stack to pop it");
    s.heap[pop()] = s.myReader.readChar();
    ++s.pc;
}

void WhiteSpaceInterpreter::input_num() {
    if (s.mystack.size() < 1) throw runtime_error("Must have a values on stack to pop it");
    s.heap[pop()] = s.myReader.readNum();
    ++s.pc;
}

// Flow Control
void WhiteSpaceInterpreter::control_call() {
    s.callStack.push_back(s.pc);
    s.pc = arg;
}

void WhiteSpaceInterpreter::control_jump() {
    s.pc = arg;
}

void WhiteSpaceInterpreter::control_jumpIfZero() {
    if (s.mystack.size() < 1) throw runtime_error("Must have a values on stack to test if it is zero");
    if (pop() == 0) {
        s.pc = arg;
    } else {
        ++s.pc;
    }
}

void WhiteSpaceInterpreter::control_jumpIfNegative() {
    if (s.mystack.size() < 1) throw runtime_error("Must have a values on stack to test if it is negative");
    if (pop() < 0) {
        s.pc = arg;
    } else {
        ++s.pc;
    }
}

void WhiteSpaceInterpreter::control_return() {
    if (s.callStack.size() < 1) throw runtime_error("Cannot return, because callstack is empty (no place to return to)");
    s.pc = s.callStack.back() + 1; // + 1, because it should not call the subroutine again but the instruction after it
    s.callStack.pop_back();
}

void WhiteSpaceInterpreter::control_exit() {} // end program

void WhiteSpaceInterpreter::unexpected_endOfProgram() {
    throw runtime_error("Programcode ended unexpectedly (without control_exit statement)");
}

std::string WhiteSpaceInterpreter::getTokenName(const Token token) {
    if (token.function == &WhiteSpaceInterpreter::stack_push) {
        return "stack_push";
    }
    if (token.function == &WhiteSpaceInterpreter::stack_duplicate) {
        return "stack_duplicate";
    }
    if (token.function == &WhiteSpaceInterpreter::stack_discard) {
        return "stack_discard";
    }
    if (token.function == &WhiteSpaceInterpreter::stack_duplicateTop) {
        return "stack_duplicateTop";
    }
    if (token.function == &WhiteSpaceInterpreter::stack_swapTop) {
        return "stack_swapTop";
    }
    if (token.function == &WhiteSpaceInterpreter::stack_discardTop) {
        return "stack_discardTop";
    }
    if (token.function == &WhiteSpaceInterpreter::arith_add) {
        return "arith_add";
    }
    if (token.function == &WhiteSpaceInterpreter::arith_sub) {
        return "arith_sub";
    }
    if (token.function == &WhiteSpaceInterpreter::arith_mult) {
        return "arith_mult";
    }
    if (token.function == &WhiteSpaceInterpreter::arith_div) {
        return "arith_div";
    }
    if (token.function == &WhiteSpaceInterpreter::arith_mod) {
        return "arith_mod";
    }
    if (token.function == &WhiteSpaceInterpreter::heap_set) {
        return "heap_set";
    }
    if (token.function == &WhiteSpaceInterpreter::heap_get) {
        return "heap_get";
    }
    if (token.function == &WhiteSpaceInterpreter::output_char) {
        return "output_char";
    }
    if (token.function == &WhiteSpaceInterpreter::output_num) {
        return "output_num";
    }
    if (token.function == &WhiteSpaceInterpreter::input_char) {
        return "input_char";
    }
    if (token.function == &WhiteSpaceInterpreter::input_num) {
        return "input_num";
    }
    if (token.function == &WhiteSpaceInterpreter::control_call) {
        return "control_call";
    }
    if (token.function == &WhiteSpaceInterpreter::control_jump) {
        return "control_jump";
    }
    if (token.function == &WhiteSpaceInterpreter::control_jumpIfZero) {
        return "control_jumpIfZero";
    }
    if (token.function == &WhiteSpaceInterpreter::control_jumpIfNegative) {
        return "control_jumpIfNegative";
    }
    if (token.function == &WhiteSpaceInterpreter::control_return) {
        return "control_return";
    }
    if (token.function == &WhiteSpaceInterpreter::control_exit) {
        return "control_exit";
    }
    if (token.function == &WhiteSpaceInterpreter::unexpected_endOfProgram) {
        return "unexpected_endOfProgram";
    }
    return "unknown_token";
}


void WhiteSpaceInterpreter::printTokens(const vector<Token>& tokens) {
    cout << "Tokens: <" << endl;
    for (const Token& tok : tokens) {
        cout << getTokenName(tok);
        cout << " " << tok.value << "\n";
    }
    cout << ">" << endl;
}

void WhiteSpaceInterpreter::printCode(const string& code, bool readableFormat) {

    cout << "Code: <";
    if (readableFormat) {
        for (const char ch : code) {
            switch (ch) {
            case '\t': cout << "t"; break;
            case '\n': cout << "n"; break;
            case ' ' : cout << "s"; break;
            }
        }
    } else {
        for (const char ch : code) {
            switch (ch) {
            case '\t': cout << "\\t"; break;
            case '\n': cout << "\\n"; break;
            case ' ' : cout << " "  ; break;
            }
        }
    }
    cout << ">" << endl;
}

string WhiteSpaceInterpreter::removeComments(const string& code) const {
    // everything is a comment that is not ' ', '\t' or '\n'
    string codeWithoutComments = "";
    for (const char ch : code) {
        if (ch == ' ' || ch == '\t' || ch == '\n') {
            codeWithoutComments += ch;
        }
    }
    return codeWithoutComments;
}

string WhiteSpaceInterpreter::execute(const vector<Token>& tokens) {
    while (s.pc < tokens.size()) {
        Token tok = tokens[s.pc];
        void(WhiteSpaceInterpreter ::*funcPtr)() = tok.function;
        if (funcPtr == &WhiteSpaceInterpreter::control_exit) { // it needs to be in every correct program, if there is none the loop will break, beacuse the unexpected_endOfProgram function would be called resulting in throwing an error
            return s.output;
        }
        arg = tok.value;
        (this->*funcPtr)();
    }

    throw runtime_error("Programcode ended without control_exit");
}

string WhiteSpaceInterpreter::interpret(const string& code, const string& input) {
    
    // Reset state for consecutive runs
    s.heap.clear();
    s.mystack.clear();
    s.callStack.clear();
    s.output = "";
    s.pc = 0;

    s.myReader.setInput(input);

    string codeWithoutComments = removeComments(code);

    if (m_debug) {
        printCode(codeWithoutComments);
    }

    Parser myparser(codeWithoutComments);
    vector<Token> tokens = myparser.parse();

    if (m_debug) {
        printTokens(tokens);
    }

    return execute(tokens);
}
