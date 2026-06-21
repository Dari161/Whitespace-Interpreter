#include "gtest/gtest.h"
#include "Reader.h"
#include <stdexcept>

namespace WhitespaceTests {
    TEST(ReaderTest, ReadsCharactersInOrder) {
        Reader reader(" \t\n");

        EXPECT_EQ(reader.readChar(), ' ');
        EXPECT_EQ(reader.readChar(), '\t');
        EXPECT_EQ(reader.readChar(), '\n');
    }

    TEST(ReaderTest, ReadsNumbersCorrectly) {
        Reader readerDec("123\n");
        EXPECT_EQ(readerDec.readNum(), 123);

        Reader readerHex("0x1A\n");
        EXPECT_EQ(readerHex.readNum(), 26);

        Reader readerZero("0\n");
        EXPECT_EQ(readerZero.readNum(), 0);
    }

    TEST(ReaderTest, ThrowsExceptionWhenInputEndsUnexpectedly) {
        // Ensure an exception is thrown when attempting to read past the end of the input
        Reader reader(" \t");

        EXPECT_EQ(reader.readChar(), ' ');
        EXPECT_EQ(reader.readChar(), '\t');

        EXPECT_THROW(reader.readChar(), std::runtime_error);
    }
}
