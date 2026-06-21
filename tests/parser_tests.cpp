#include "gtest/gtest.h"
#include "Parser.h"
#include "Token.h"
#include "WhiteSpaceInterpreter.h"
#include <vector>
#include <stdexcept>
#include <string>

namespace WhitespaceTests {
    // Helper to check token function names without exposing private pointers
    void AssertTokenName(const Token& token, const std::string& expectedName) {
        // Nem példányosítunk objektumot, hanem statikusan hívjuk a WhiteSpaceInterpreter osztályból
        EXPECT_EQ(WhiteSpaceInterpreter::getTokenName(token), expectedName);
    }

    // ==========================================
    // 1. STACK MANIPULATION TESTS
    // ==========================================

    TEST(ParserTest, ParsesStackPushAndMakesNumbersCorrectly) {
        Parser parser("   \t \t\n");
        auto tokens = parser.parse();

        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "stack_push");
        EXPECT_EQ(tokens[0].value, 5);
    }

    TEST(ParserTest, ParsesStackDuplicateTop) {
        Parser parser(" \n ");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "stack_duplicateTop");
    }

    TEST(ParserTest, ParsesStackSwapTop) {
        Parser parser(" \n\t");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "stack_swapTop");
    }

    TEST(ParserTest, ParsesStackDiscardTop) {
        Parser parser(" \n\n");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "stack_discardTop");
    }

    // ==========================================
    // 2. ARITHMETIC TESTS
    // ==========================================

    TEST(ParserTest, ParsesArithmeticAdd) {
        Parser parser("\t   ");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "arith_add");
    }

    TEST(ParserTest, ParsesArithmeticSub) {
        Parser parser("\t  \t");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "arith_sub");
    }

    TEST(ParserTest, ParsesArithmeticMult) {
        Parser parser("\t  \n");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "arith_mult");
    }

    TEST(ParserTest, ParsesArithmeticDiv) {
        Parser parser("\t \t ");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "arith_div");
    }

    TEST(ParserTest, ParsesArithmeticMod) {
        Parser parser("\t \t\t");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "arith_mod");
    }

    // ==========================================
    // 3. HEAP ACCESS TESTS
    // ==========================================

    TEST(ParserTest, ParsesHeapSet) {
        Parser parser("\t\t ");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "heap_set");
    }

    TEST(ParserTest, ParsesHeapGet) {
        Parser parser("\t\t\t");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "heap_get");
    }

    // ==========================================
    // 4. INPUT / OUTPUT TESTS
    // ==========================================

    TEST(ParserTest, ParsesOutputChar) {
        Parser parser("\t\n  ");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "output_char");
    }

    TEST(ParserTest, ParsesOutputNum) {
        Parser parser("\t\n \t");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "output_num");
    }

    TEST(ParserTest, ParsesInputChar) {
        Parser parser("\t\n\t ");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "input_char");
    }

    TEST(ParserTest, ParsesInputNum) {
        Parser parser("\t\n\t\t");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "input_num");
    }

    // ==========================================
    // 5. FLOW CONTROL & INTEGRATION TESTS
    // ==========================================

    TEST(ParserTest, MakesLabelsWithBase3AndResolvesJump) {
        Parser parser("\n   \t \n\n \n \t \n");
        auto tokens = parser.parse();

        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "control_jump");
        EXPECT_EQ(tokens[0].value, 0);
    }

    TEST(ParserTest, ParsesControlExit) {
        Parser parser("\n\n\n");
        auto tokens = parser.parse();
        ASSERT_GE(tokens.size(), 1);
        AssertTokenName(tokens[0], "control_exit");
    }

    TEST(ParserTest, ParsesMultipleTokensInOrder) {
        Parser parser("\t   \n\n\n");
        auto tokens = parser.parse();

        ASSERT_GE(tokens.size(), 2);
        AssertTokenName(tokens[0], "arith_add");
        AssertTokenName(tokens[1], "control_exit");
    }

    // ==========================================
    // 6. EXCEPTION HANDLING TESTS
    // ==========================================

    TEST(ParserTest, ThrowsExceptionOnInvalidCharacter) {
        Parser parser("   \t\nx\n\n\n");
        EXPECT_THROW(parser.parse(), std::runtime_error);
    }

    TEST(ParserTest, ThrowsExceptionOnUnexpectedEndOfCode) {
        Parser parser("   \t ");
        EXPECT_THROW(parser.parse(), std::runtime_error);
    }

    TEST(ParserTest, ThrowsExceptionOnDuplicateLabel) {
        Parser parser("\n   \t \n\n   \t \n");
        EXPECT_THROW(parser.parse(), std::runtime_error);
    }

    TEST(ParserTest, ThrowsExceptionOnMissingLabelTarget) {
        Parser parser("\n \n \t \n");
        EXPECT_THROW(parser.parse(), std::runtime_error);
    }
}
