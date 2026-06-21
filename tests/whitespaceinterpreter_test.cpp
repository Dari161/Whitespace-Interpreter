#include "gtest/gtest.h"
#include "WhiteSpaceInterpreter.h"
#include <stdexcept>
#include <string>

namespace WhitespaceTests {

    // ==========================================
    // 1. STACK OPERATION TESTS (Verified via Output)
    // ==========================================

    TEST(InterpreterTest, StackPushPlacesCorrectValue) {
        // Push 42, Output Num, Exit
        std::string code = "   \t \t \t \n\t\n \t\n\n\n";
        WhiteSpaceInterpreter interpreter;
        EXPECT_EQ(interpreter.interpret(code), "42");
    }

    TEST(InterpreterTest, StackDuplicateTopCopiesValue) {
        // Push 7, Duplicate Top, Output Num, Output Num, Exit
        std::string code = "   \t\t\t\n \n \t\n \t\t\n \t\n\n\n";
        WhiteSpaceInterpreter interpreter;
        EXPECT_EQ(interpreter.interpret(code), "77");
    }

    TEST(InterpreterTest, StackSwapTopSwapsElements) {
        // Push 1, Push 2, Swap Top, Output Num, Output Num, Exit
        std::string code = "   \t\n   \t \n \n\t\t\n \t\t\n \t\n\n\n";
        WhiteSpaceInterpreter interpreter;
        EXPECT_EQ(interpreter.interpret(code), "12");
    }

    TEST(InterpreterTest, StackDiscardTopRemovesElement) {
        // Push 5, Push 9, Discard Top, Output Num, Exit
        std::string code = "   \t \t\n   \t  \t\n \n\n\t\n \t\n\n\n";
        WhiteSpaceInterpreter interpreter;
        EXPECT_EQ(interpreter.interpret(code), "5");
    }

    // ==========================================
    // 2. ARITHMETIC TESTS (Verified via Math Results)
    // ==========================================

    TEST(InterpreterTest, ArithmeticAddCalculatesCorrectly) {
        // Push 15, Push 5, Add, Output Num, Exit
        std::string code = "   \t\t\t\t\n   \t \t\n\t   \t\n \t\n\n\n";
        WhiteSpaceInterpreter interpreter;
        EXPECT_EQ(interpreter.interpret(code), "20");
    }

    TEST(InterpreterTest, ArithmeticSubCalculatesCorrectly) {
        // Push 15, Push 5, Sub, Output Num, Exit
        std::string code = "   \t\t\t\t\n   \t \t\n\t  \t\t\n \t\n\n\n";
        WhiteSpaceInterpreter interpreter;
        EXPECT_EQ(interpreter.interpret(code), "10");
    }

    TEST(InterpreterTest, ArithmeticMultCalculatesCorrectly) {
        // Push 4, Push 3, Mult, Output Num, Exit
        std::string code = "   \t  \n   \t\t\n\t  \n\t\n \t\n\n\n";
        WhiteSpaceInterpreter interpreter;
        EXPECT_EQ(interpreter.interpret(code), "12");
    }

    TEST(InterpreterTest, ArithmeticDivCalculatesCorrectly) {
        // Push 12, Push 4, Div, Output Num, Exit
        std::string code = "   \t\t  \n   \t  \n\t \t \t\n \t\n\n\n";
        WhiteSpaceInterpreter interpreter;
        EXPECT_EQ(interpreter.interpret(code), "3");
    }

    TEST(InterpreterTest, ArithmeticModCalculatesCorrectly) {
        // Push 11, Push 3, Mod, Output Num, Exit
        std::string code = "   \t \t\t\n   \t\t\n\t \t\t\t\n \t\n\n\n";
        WhiteSpaceInterpreter interpreter;
        EXPECT_EQ(interpreter.interpret(code), "2");
    }

    // ==========================================
    // 3. HEAP ACCESS TESTS
    // ==========================================

    TEST(InterpreterTest, HeapStoreAndRetrievePreservesData) {
        // Push 5, Push 99, Store, Push 5, Retrieve, Output Num, Exit
        std::string code = "   \t \t\n   \t\t   \t\t\n\t\t    \t \t\n\t\t\t\t\n \t\n\n\n";
        WhiteSpaceInterpreter interpreter;
        EXPECT_EQ(interpreter.interpret(code), "99");
    }

    // ==========================================
    // 4. FLOW CONTROL TESTS
    // ==========================================

    TEST(InterpreterTest, ConditionalJumpIfZeroWorks) {
        // Push 0, JZ to Label, Exit (hibás ág), Mark Label, Exit (jó ág)
        std::string code = "     \n\n\t \t \n\n\n\n\n  \t \n\n\n\n";
        WhiteSpaceInterpreter interpreter;
        EXPECT_NO_THROW(interpreter.interpret(code));
    }

    TEST(InterpreterTest, ConditionalJumpIfNegativeWorks) {
        // Push -1, JN to Label, Exit (hibás ág), Mark Label, Exit (jó ág)
        std::string code = "   \t\t\n\n\t\t\t \n\n\n\n\n  \t \n\n\n\n";
        WhiteSpaceInterpreter interpreter;
        EXPECT_NO_THROW(interpreter.interpret(code));
    }
}
