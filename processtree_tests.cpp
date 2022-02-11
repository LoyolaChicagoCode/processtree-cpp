#include <gtest/gtest.h>

#include "parsing.h"

// Demonstrate some basic assertions.
TEST(ProcessTreeTests, BasicAssertions) {
// Expect two strings not to be equal.
EXPECT_STRNE("hello", "world");
// Expect equality.
EXPECT_EQ(7 * 6, 42);
}

TEST(ProcessTreeTests, ParserHeaderBad1) {
    try {
        process_parser p("a b c");
        FAIL();
    } catch (const int e) {
        SUCCEED();
    }
}

TEST(ProcessTreeTests, ParserHeaderBad2) {
    try {
        process_parser p("a CMD b PID c PPID d");
        FAIL();
    } catch (const int e) {
        SUCCEED();
    }
}

TEST(ProcessTreeTests, ParserHeaderBad3) {
    try {
        process_parser p("a COMMAND b PID c PPID d");
        FAIL();
    } catch (const int e) {
        SUCCEED();
    }
}

TEST(ProcessTreeTests, ParserHeaderBad4) {
    try {
        process_parser p("a PID c PPID d CMD e");
        FAIL();
    } catch (const int e) {
        SUCCEED();
    }
}

TEST(ProcessTreeTests, ParserHeaderGood1) {
    //                     01234567890123456789012345678901234
    process_parser p("a bb PID cccc PPID dddd eee COMMAND");
}

TEST(ProcessTreeTests, ParserHeaderGood2) {
    //                     01234567890123456789012345678901234
    process_parser p("a bb PID cccc PPID dddd eee CMD");
}
