
#include <iostream>
#include <map>
#include <vector>
#include <cmath> // for floor()

using namespace std;

// Memory alloc errors are handled by the map and vector class themselves

map<int, int> heap;
vector<int> mystack;

vector<size_t> callStack;
map<size_t, size_t> labels;

string output = "";

// helper function to get the value of the last element of mystack and remove it from mystack
int pop() {
    int res = mystack.back();
    mystack.pop_back();
    return res;
}

size_t pc = 0; // program counter

string input = "";

// decimal
string intToStr(int num) {
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

// invalid character may refer to '\0' (end of string)

class Reader {
private:
    size_t pos;

    const int hexCharToInt(const char hexChar) const {
        switch (hexChar) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'A': case 'a': return 10;
        case 'B': case 'b': return 11;
        case 'C': case 'c': return 12;
        case 'D': case 'd': return 13;
        case 'E': case 'e': return 14;
        case 'F': case 'f': return 15;
        default:
            throw runtime_error("Invalid hexadecimal character");
        }
    }
public:
    Reader() : pos(0) {}

    // Note: It does not support minus numbers, nor binary- or octal numeral system

    const int readNum() {
        if (input[pos] == '\0') throw runtime_error("Input was empty when trying to read number");
        if (input[pos] == '0') {
            ++pos;
            if (input[pos] == '\n') {
                ++pos;
                return 0;
            }
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

    const char readChar() {
        if (input[pos] == '\0') throw runtime_error("Input was empty when trying to read a character");
        char ret = input[pos];
        ++pos;
        return ret;
    }

    void reset() {
        pos = 0;
    }
};

Reader myReader;

struct Token {
    // function's type is a pointer to a function, with void return type, and no argument
    void(*function)();
    int value;
};

int arg = 0;

// Stack Manipulation
void stack_push() {
    mystack.push_back(arg);
    ++pc;
}

void stack_duplicate() {
    if (arg >= mystack.size()) throw runtime_error("Range out of bounds");
    mystack.push_back(mystack[mystack.size() - arg - 1]);
    ++pc;
}

void stack_discard() {
    if (arg < 0 || arg >= mystack.size()) { // remove everything but the top value
        if (mystack.size() < 0) throw runtime_error("Stack is empty");
        int top = pop();
        mystack.clear();
        mystack.push_back(top);
        ++pc;
        return;
    }
    int top = pop();
    // Resize the vector to remove the last n elements
    mystack.resize(mystack.size() - arg);
    mystack.push_back(top);
    ++pc;
}

void stack_duplicateTop() {
    if (mystack.size() < 1) throw runtime_error("Can't duplicate top of empty stack");
    mystack.push_back(mystack[mystack.size() - 1]);
    ++pc;
}

void stack_swapTop() {
    if (mystack.size() < 2) throw runtime_error("Must have 2 values on stack to swap them");
    int tmp = mystack[mystack.size() - 1];
    mystack[mystack.size() - 1] = mystack[mystack.size() - 2];
    mystack[mystack.size() - 2] = tmp;
    ++pc;
}

void stack_discardTop() {
    if (mystack.size() < 1) throw runtime_error("Can't discard top of empty stack");

    mystack.pop_back();

    ++pc;
}

// Arithmetic
void arith_add() {
    if (mystack.size() < 2) throw runtime_error("Must have 2 values on stack to add them");
    mystack.push_back(pop() + pop());
    ++pc;
}

void arith_sub() {
    if (mystack.size() < 2) throw runtime_error("Must have 2 values on stack to subtract them");
    mystack.push_back(-pop() + pop());
    ++pc;
}

void arith_mult() {
    if (mystack.size() < 2) throw runtime_error("Must have 2 values on stack to multiply them");
    mystack.push_back(pop() * pop());
    ++pc;
}

void arith_div() {
    if (mystack.size() < 2) throw runtime_error("Must have 2 values on stack to divide them");
    float a = static_cast<float>(pop());
    float b = static_cast<float>(pop());
    if (a == 0) throw runtime_error("Division by 0");
    mystack.push_back(floor(b / a)); // floor is needed so division can floor-division can wrok for negatives too
    ++pc;
}

void arith_mod() {
    if (mystack.size() < 2) throw runtime_error("Must have 2 values on stack to modulo them");
    int a = pop();
    int b = pop();
    if (a == 0) throw runtime_error("Modulo by 0");

    int mod = b % a; // mod always gets the sign of b
    // 5 mod -3 should be -1
    if ((mod > 0 && a < 0) || (mod < 0 && a > 0)) {
        mod += a;
    }

    mystack.push_back(mod);
    ++pc;
}

// Heap access
void heap_set() {
    if (mystack.size() < 2) throw runtime_error("Must have 2 values on stack to pop them");
    int a = pop();
    heap[pop()] = a;
    // in "heap[pop()] = pop()" the order of function evaluation is unspecified, meaning it depends on the complier
    ++pc;
}

void heap_get() {
    if (mystack.size() < 1) throw runtime_error("Must have 2 values on stack to pop them");
    map<int, int>::iterator it = heap.find(pop());
    if (it == heap.end()) throw runtime_error("No such adress in heap exists");
    mystack.push_back(it->second); // it->second is the value
    ++pc;
}

// Input/Output
void output_char() {
    if (mystack.size() < 1) throw runtime_error("Must have a values on stack to output it as a char");
    //output as char
    output += static_cast<char>(pop() % 256);
    ++pc;
}

void output_num() {
    if (mystack.size() < 1) throw runtime_error("Must have a values on stack to output it as a number");
    //output as num
    output += intToStr(pop());
    ++pc;
}

void input_char() {
    if (mystack.size() < 1) throw runtime_error("Must have a values on stack to pop it");
    heap[pop()] = myReader.readChar();
    ++pc;
}

void input_num() {
    if (mystack.size() < 1) throw runtime_error("Must have a values on stack to pop it");
    heap[pop()] = myReader.readNum();
    ++pc;
}

// Flow Control
void control_call() {
    callStack.push_back(pc);
    pc = arg;
}

void control_jump() {
    pc = arg;
}

void control_jumpIfZero() {
    if (mystack.size() < 1) throw runtime_error("Must have a values on stack to test if it is zero");
    if (pop() == 0) {
        pc = arg;
    } else {
        ++pc;
    }
}

void control_jumpIfNegative() {
    if (mystack.size() < 1) throw runtime_error("Must have a values on stack to test if it is negative");
    if (pop() < 0) {
        pc = arg;
    } else {
        ++pc;
    }
}

void control_return() {
    if (callStack.size() < 1) throw runtime_error("Cannot return, because callstack is empty (no place to return to)");
    pc = callStack.back() + 1; // + 1, because it should not call the subroutine again but the instruction after it
    callStack.pop_back();
}

void control_exit() {} // end program

void unexpected_endOfProgram() {
    throw runtime_error("Programcode ended unexpectedly (without control_exit statement)");
}

// Lex and parse in one go
class Parser {
private:
    string code;
    size_t pos;

    const int makeNumber() {
        int sign;
        switch (code[pos]) {
        case '\t': sign = -1; ++pos; break;
        case ' ' : sign =  1; ++pos; break;
        default:
            throw runtime_error("Expected a number. Numbers must start with tab or space");
        }

        int num = 0;
        while (code[pos] != '\n') {
            if (code[pos] == '\0') throw runtime_error("Unexpected end of code");
            num *= 2;
            num += (code[pos] == '\t'); // ch is either tab -> 1, or space -> 0
            ++pos;
        }
        ++pos; // advance past the newLine
        return sign * num;
    }

    const int makeLabel() {
        size_t num = 0;
        while (code[pos] != '\n') {
            if (code[pos] == '\0') throw runtime_error("Unexpected end of code");
            // This cannot be a binary representation of table like in makeNumber, since "" and " " are different labels, but in binary, is space is zero they would be equal
            // so I used a base 3 numeral system, where tab is 2 and space is 1
            num *= 3;
            switch (code[pos]) {
            case '\t': num += 2; break;
            case ' ' : num += 1; break;
            }
            ++pos;
        }
        ++pos; // advance past the newLine

        return num;
    }

public:
    Parser(const string& code_) : code(code_), pos(0) {}

    const vector<Token> parse() {
        vector<Token> res;
        while (code[pos] != '\0') {
            switch (code[pos]) {
            case ' ':
                // Stack manipulation
                ++pos;
                switch (code[pos]) {
                case ' ':
                    ++pos;
                    res.push_back(Token{ stack_push, makeNumber() });
                    break;
                case '\t':
                    ++pos;
                    switch (code[pos]) {
                    case ' ':
                        ++pos;
                        res.push_back(Token{ stack_duplicate, makeNumber() });
                        break;
                    case '\n':
                        ++pos;
                        res.push_back(Token{ stack_discard, makeNumber() });
                        break;
                    default:
                        throw runtime_error("Unexpected character");
                    }
                    break;
                case '\n':
                    ++pos;
                    switch (code[pos]) {
                    case ' ':
                        res.push_back(Token{ stack_duplicateTop, 0 });
                        ++pos; break;
                    case '\t':
                        res.push_back(Token{ stack_swapTop, 0 });
                        ++pos; break;
                    case '\n':
                        res.push_back(Token{ stack_discardTop, 0 });
                        ++pos; break;
                    default:
                        throw runtime_error("Unexpected character");
                    }
                    break;
                default:
                    throw runtime_error("Unexpected character");
                }
                break;
            case '\t':
                ++pos;
                switch (code[pos]) {
                case ' ':
                    // Arithmetic
                    ++pos;
                    switch (code[pos]) {
                    case ' ':
                        ++pos;
                        switch (code[pos]) {
                        case ' ':
                            res.push_back(Token{ arith_add, 0 });
                            ++pos; break;
                        case '\t':
                            res.push_back(Token{ arith_sub, 0 });
                            ++pos; break;
                        case '\n':
                            res.push_back(Token{ arith_mult, 0 });
                            ++pos; break;
                        default:
                            throw runtime_error("Unexpected character");
                        }
                        break;
                    case '\t':
                        ++pos;
                        switch (code[pos]) {
                        case ' ':
                            res.push_back(Token{ arith_div, 0 });
                            ++pos; break;
                        case '\t':
                            res.push_back(Token{ arith_mod, 0 });
                            ++pos; break;
                        default:
                            throw runtime_error("Unexpected character");
                        }
                        break;
                    default:
                        throw runtime_error("Unexpected character");
                    }
                    break;
                case '\t':
                    // Heap Access
                    ++pos;
                    switch (code[pos]) {
                    case ' ':
                        res.push_back(Token{ heap_set, 0 });
                        ++pos; break;
                    case '\t':
                        res.push_back(Token{ heap_get, 0 });
                        ++pos; break;
                    default:
                        throw runtime_error("Unexpected character");
                    }
                    break;
                case '\n':
                    // Input/Output
                    ++pos;
                    switch (code[pos]) {
                    case ' ':
                        ++pos;
                        switch (code[pos]) {
                        case ' ':
                            res.push_back(Token{ output_char, 0 });
                            ++pos; break;
                        case '\t':
                            res.push_back(Token{ output_num, 0 });
                            ++pos; break;
                        default:
                            throw runtime_error("Unexpected character");
                        }
                        break;
                    case '\t':
                        ++pos;
                        switch (code[pos]) {
                        case ' ':
                            res.push_back(Token{ input_char, 0 });
                            ++pos; break;
                        case '\t':
                            res.push_back(Token{ input_num, 0 });
                            ++pos; break;
                        default:
                            throw runtime_error("Unexpected character");
                        }
                        break;
                    default:
                        throw runtime_error("Unexpected character");
                    }
                    break;
                default:
                    throw runtime_error("Unexpected character");
                }
                break;
            case '\n':
                // Flow-Control
                ++pos;
                switch (code[pos]) {
                case ' ':
                    ++pos;
                    switch (code[pos]) {
                    case ' ':
                        ++pos;
                    {
                        size_t label = makeLabel();
                        if (labels.find(label) != labels.end()) throw runtime_error("Label already exists"); // check if already exists
                        labels[label] = res.size();
                    }
                        break;
                    case '\t':
                        ++pos;
                        res.push_back(Token{ control_call, makeLabel() });
                        break;
                    case '\n':
                        ++pos;
                        res.push_back(Token{ control_jump, makeLabel() });
                        break;
                    default:
                        throw runtime_error("Unexpected character");
                    }
                    break;
                case '\t':
                    ++pos;
                    switch (code[pos]) {
                    case ' ':
                        ++pos;
                        res.push_back(Token{ control_jumpIfZero, makeLabel() });
                        break;
                    case '\t':
                        ++pos;
                        res.push_back(Token{ control_jumpIfNegative, makeLabel() });
                        break;
                    case '\n':
                        res.push_back(Token{ control_return, 0 });
                        ++pos; break;
                    default:
                        throw runtime_error("Unexpected character");
                    }
                    break;
                case '\n':
                    ++pos;
                    switch (code[pos]) {
                    case '\n':
                        res.push_back(Token{ control_exit, 0 });
                        ++pos; break;
                    default:
                        throw runtime_error("Unexpected character");
                    }
                    break;
                default:
                    throw runtime_error("Unexpected character");
                }
                break;
            default:
                throw runtime_error("Unexpected character");
            }

            if (pos >= code.size() - 1) break; // IDK if needed

        }

        // Label postprocessing, so they hold the value where they point
        for (Token& tok : res) {
            if (tok.function == control_call ||
                tok.function == control_jump ||
                tok.function == control_jumpIfZero ||
                tok.function == control_jumpIfNegative) {
                map<size_t, size_t>::iterator it = labels.find(tok.value);
                if (it == labels.end()) throw runtime_error("No such label exists");
                tok.value = it->second;
            }
        }

        // should not reach, because programs should end with control_exit
        res.push_back(Token{ unexpected_endOfProgram, 0 });
        return res;
    }
};

void printTokens(const vector<Token>& tokens) {
    cout << "Tokens: <" << endl;
    for (const Token& tok : tokens) {
        if (tok.function == stack_push) {
            cout << "stack_push";
        } else if (tok.function == stack_duplicate) {
            cout << "stack_duplicate";
        } else if (tok.function == stack_discard) {
            cout << "stack_discard";
        } else if (tok.function == stack_duplicateTop) {
            cout << "stack_duplicateTop";
        } else if (tok.function == stack_swapTop) {
            cout << "stack_swapTop";
        } else if (tok.function == stack_discardTop) {
            cout << "stack_discardTop";
        } else if (tok.function == arith_add) {
            cout << "arith_add";
        } else if (tok.function == arith_sub) {
            cout << "arith_sub";
        } else if (tok.function == arith_mult) {
            cout << "arith_mult";
        } else if (tok.function == arith_div) {
            cout << "arith_div";
        } else if (tok.function == arith_mod) {
            cout << "arith_mod";
        } else if (tok.function == heap_set) {
            cout << "heap_set";
        } else if (tok.function == heap_get) {
            cout << "heap_get";
        } else if (tok.function == output_char) {
            cout << "output_char";
        } else if (tok.function == output_num) {
            cout << "output_num";
        } else if (tok.function == input_char) {
            cout << "input_char";
        } else if (tok.function == input_num) {
            cout << "input_num";
        } else if (tok.function == control_call) {
            cout << "control_call";
        } else if (tok.function == control_jump) {
            cout << "control_jump";
        } else if (tok.function == control_jumpIfZero) {
            cout << "control_jumpIfZero";
        } else if (tok.function == control_jumpIfNegative) {
            cout << "control_jumpIfNegative";
        } else if (tok.function == control_return) {
            cout << "control_return";
        } else if (tok.function == control_exit) {
            cout << "control_exit";
        } else if (tok.function == unexpected_endOfProgram) {
            cout << "unexpected_endOfProgram";
        } else {
            throw runtime_error("Unexpected token");
        }
        cout << " " << tok.value << endl;
    }
    cout << ">" << endl;
}

void printCode(const string& code) {
    bool readableFormat = true;

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

// Solution
string whitespace(const string& code, const string& inp = "") {

    // reset global variables
    mystack.clear();
    heap.clear();
    callStack.clear();
    labels.clear();
    output = "";
    pc = 0;
    myReader.reset();

    // get rid of everything but the 3 whitespaces (everything else is considered as comment)
    string codeWithoutComments = "";
    for (const char ch : code) {
        if (ch == ' ' || ch == '\t' || ch == '\n') {
            codeWithoutComments += ch;
        }
    }
    printCode(codeWithoutComments);

    Parser myparser(codeWithoutComments);
    vector<Token> tokens = myparser.parse();
    printTokens(tokens);

    input = inp;

    // Interpreter
    for (;;) { // infinite loop
        Token tok = tokens[pc];
        void(*funcPtr)() = tok.function;
        if (funcPtr == control_exit) { // it needs to be in every correct program, if there is none the loop will break, beacuse the unexpected_endOfProgram function would be called resulting in throwing an error
            return output;
        }
        arg = tok.value;
        funcPtr();
    }

    throw runtime_error("Programcode ended without control_exit");
}

int main() {
    // Tests

    // test Reader

    //tes1
    /*input = "asd";
    cout << myReader.readChar();
    cout << myReader.readChar();
    cout << myReader.readChar();
    cout << myReader.readChar();*/

    //test2
    /*input = "12\nads0xAB1\n32";
    cout << myReader.readNum();
    //cout << myReader.readNum();
    cout << myReader.readChar();
    cout << myReader.readChar();
    cout << myReader.readChar();
    //cout << myReader.readChar();
    cout << myReader.readNum();
    //cout << myReader.readNum();
    cout << myReader.readChar();
    cout << myReader.readChar();*/

    //test3
    /*input = "0x77";
    //cout << myReader.readNum();*/

    // test makeNumber
    //cout << whitespace("push  num \t\t\t \noutputNum\t\n \tend\n\n\n");
    //cout << whitespace("push  num\t\t \t \noutputNum\t\n \tend\n\n\n");

    /*cout << whitespace("   \t\n\t\n \t\n\n\n");
    cout << whitespace("   \t \n\t\n \t\n\n\n");
    cout << whitespace("   \t\t\n\t\n \t\n\n\n");
    cout << whitespace("    \n\t\n \t\n\n\n");*/

    // test makeLabel
    //cout << whitespace("push  num   \npush  num\t\t\t\njump\n \nlabel \t\t\noutputNum\t\n \tMarkWithLabel\n  label \t\t\noutputNum\t\n \tend\n\n\n");

    // push and output test from Codewars
    /*cout << whitespace("   \t\t\n   \t\t\t\n\t\n \t\t\n \t\n\n\n");
    cout << whitespace("   \t\t\n   \t\t\n\t\n \t\t\n \t\n\n\n");
    cout << whitespace("   \t\t\n   \t\t\n\t\n \t\t\n \t\n\n\n");
    cout << whitespace("   \t\t\n \n \t\n \t\t\n \t\n\n\n");

    cout << endl << endl;

    cout << whitespace("   \t\t\n   \t \n \n\t \n\n\t\n \t\n\n\n");

    cout << endl << endl;

    cout << whitespace("   \t\n   \t \n   \t\t\n \t\n\t\t     \n\t\n \t\n\n\n");*/

    // test arithmetic mod

    //cout << whitespace("   \t \t\n  \t\t\t\n\t \t\t\t\n \t\n\n\n");

    // Testing_conditional_and_unconditional_jump_functionality
    //cout << whitespace("   \n   \t\n   \t \n   \t\t\n\n  \n\t\n \t \n \n\t  \n\n \n\n\n   \n\n\n\n");

    //cout << whitespace("   \t      \t \n   \t\t\t \t\t   \t\t  \t \n    \n\t\t    \t  \t   \n\t\n     \t\t  \t \t\n\t\n     \t\t \t\t  \n \n \t\n  \t\n     \t\t \t\t\t\t\n\t\n     \t     \n\t\n     \t \t \t\t\t\n\t\n     \t\t \t\t\t\t\n\t\n     \t\t\t  \t \n\t\n     \t\t \t\t  \n\t\n     \t\t  \t  \n\t\n     \t    \t\n\t\n     \t \t \n\t\n   !", "");

    return 0;
}
