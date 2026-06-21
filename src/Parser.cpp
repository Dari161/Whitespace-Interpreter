#include "Parser.h"
#include <iostream>
#include <vector>
#include <map>
#include "WhiteSpaceInterpreter.h"
#include "Token.h"

int Parser::makeNumber() {
    int sign;
    switch (code[pos]) {
    case '\t': sign = -1; ++pos; break;
    case ' ': sign = 1; ++pos; break;
    default:
        throw std::runtime_error("Expected a number. Numbers must start with tab or space");
    }

    int num = 0;
    while (code[pos] != '\n') {
        if (code[pos] == '\0') throw std::runtime_error("Unexpected end of code");
        num *= 2;
        num += (code[pos] == '\t'); // ch is either tab -> 1, or space -> 0
        ++pos;
    }
    ++pos; // advance past the newLine
    return sign * num;
}

int Parser::makeLabel() {
    int num = 0;
    while (code[pos] != '\n') {
        if (code[pos] == '\0') throw std::runtime_error("Unexpected end of code");
        // This cannot be a binary representation of table like in makeNumber, since "" and " " are different labels, but in binary, is space is zero they would be equal
        // so I used a base 3 numeral system, where tab is 2 and space is 1
        num *= 3;
        switch (code[pos]) {
        case '\t': num += 2; break;
        case ' ': num += 1; break;
        }
        ++pos;
    }
    ++pos; // advance past the newLine

    return num;
}

const std::vector<Token> Parser::parse() {
    std::map<size_t, size_t> labels;
    std::vector<Token> res;
    while (code[pos] != '\0') {
        switch (code[pos]) {
        case ' ':
            // Stack manipulation
            ++pos;
            switch (code[pos]) {
            case ' ':
                ++pos;
                res.push_back(Token{ &WhiteSpaceInterpreter::stack_push, makeNumber() });
                break;
            case '\t':
                ++pos;
                switch (code[pos]) {
                case ' ':
                    ++pos;
                    res.push_back(Token{ &WhiteSpaceInterpreter::stack_duplicate, makeNumber() });
                    break;
                case '\n':
                    ++pos;
                    res.push_back(Token{ &WhiteSpaceInterpreter::stack_discard, makeNumber() });
                    break;
                default:
                    throw std::runtime_error("Unexpected character");
                }
                break;
            case '\n':
                ++pos;
                switch (code[pos]) {
                case ' ':
                    res.push_back(Token{ &WhiteSpaceInterpreter::stack_duplicateTop, 0 });
                    ++pos; break;
                case '\t':
                    res.push_back(Token{ &WhiteSpaceInterpreter::stack_swapTop, 0 });
                    ++pos; break;
                case '\n':
                    res.push_back(Token{ &WhiteSpaceInterpreter::stack_discardTop, 0 });
                    ++pos; break;
                default:
                    throw std::runtime_error("Unexpected character");
                }
                break;
            default:
                throw std::runtime_error("Unexpected character");
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
                        res.push_back(Token{ &WhiteSpaceInterpreter::arith_add, 0 });
                        ++pos; break;
                    case '\t':
                        res.push_back(Token{ &WhiteSpaceInterpreter::arith_sub, 0 });
                        ++pos; break;
                    case '\n':
                        res.push_back(Token{ &WhiteSpaceInterpreter::arith_mult, 0 });
                        ++pos; break;
                    default:
                        throw std::runtime_error("Unexpected character");
                    }
                    break;
                case '\t':
                    ++pos;
                    switch (code[pos]) {
                    case ' ':
                        res.push_back(Token{ &WhiteSpaceInterpreter::arith_div, 0 });
                        ++pos; break;
                    case '\t':
                        res.push_back(Token{ &WhiteSpaceInterpreter::arith_mod, 0 });
                        ++pos; break;
                    default:
                        throw std::runtime_error("Unexpected character");
                    }
                    break;
                default:
                    throw std::runtime_error("Unexpected character");
                }
                break;
            case '\t':
                // Heap Access
                ++pos;
                switch (code[pos]) {
                case ' ':
                    res.push_back(Token{ &WhiteSpaceInterpreter::heap_set, 0 });
                    ++pos; break;
                case '\t':
                    res.push_back(Token{ &WhiteSpaceInterpreter::heap_get, 0 });
                    ++pos; break;
                default:
                    throw std::runtime_error("Unexpected character");
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
                        res.push_back(Token{ &WhiteSpaceInterpreter::output_char, 0 });
                        ++pos; break;
                    case '\t':
                        res.push_back(Token{ &WhiteSpaceInterpreter::output_num, 0 });
                        ++pos; break;
                    default:
                        throw std::runtime_error("Unexpected character");
                    }
                    break;
                case '\t':
                    ++pos;
                    switch (code[pos]) {
                    case ' ':
                        res.push_back(Token{ &WhiteSpaceInterpreter::input_char, 0 });
                        ++pos; break;
                    case '\t':
                        res.push_back(Token{ &WhiteSpaceInterpreter::input_num, 0 });
                        ++pos; break;
                    default:
                        throw std::runtime_error("Unexpected character");
                    }
                    break;
                default:
                    throw std::runtime_error("Unexpected character");
                }
                break;
            default:
                throw std::runtime_error("Unexpected character");
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
                        if (labels.find(label) != labels.end()) throw std::runtime_error("Label already exists"); // check if already exists
                        labels[label] = res.size();
                    }
                    break;
                case '\t':
                    ++pos;
                    res.push_back(Token{ &WhiteSpaceInterpreter::control_call, makeLabel() });
                    break;
                case '\n':
                    ++pos;
                    res.push_back(Token{ &WhiteSpaceInterpreter::control_jump, makeLabel() });
                    break;
                default:
                    throw std::runtime_error("Unexpected character");
                }
                break;
            case '\t':
                ++pos;
                switch (code[pos]) {
                case ' ':
                    ++pos;
                    res.push_back(Token{ &WhiteSpaceInterpreter::control_jumpIfZero, makeLabel() });
                    break;
                case '\t':
                    ++pos;
                    res.push_back(Token{ &WhiteSpaceInterpreter::control_jumpIfNegative, makeLabel() });
                    break;
                case '\n':
                    res.push_back(Token{ &WhiteSpaceInterpreter::control_return, 0 });
                    ++pos; break;
                default:
                    throw std::runtime_error("Unexpected character");
                }
                break;
            case '\n':
                ++pos;
                switch (code[pos]) {
                case '\n':
                    res.push_back(Token{ &WhiteSpaceInterpreter::control_exit, 0 });
                    ++pos; break;
                default:
                    throw std::runtime_error("Unexpected character");
                }
                break;
            default:
                throw std::runtime_error("Unexpected character");
            }
            break;
        default:
            throw std::runtime_error("Unexpected character");
        }

    }

    // Label postprocessing, so they hold the value where they point
    for (Token& tok : res) {
        if (tok.function == &WhiteSpaceInterpreter::control_call ||
            tok.function == &WhiteSpaceInterpreter::control_jump ||
            tok.function == &WhiteSpaceInterpreter::control_jumpIfZero ||
            tok.function == &WhiteSpaceInterpreter::control_jumpIfNegative) {
            std::map<size_t, size_t>::iterator it = labels.find(tok.value);
            if (it == labels.end()) throw std::runtime_error("No such label exists");
            tok.value = static_cast<int>(it->second);
        }
    }

    // should not reach, because programs should end with control_exit
    res.push_back(Token{ &WhiteSpaceInterpreter::unexpected_endOfProgram, 0 });
    return res;
}
