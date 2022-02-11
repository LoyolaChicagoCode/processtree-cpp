#include <gtest/gtest.h>

#include "parsing.h"

// Demonstrate some basic assertions.
TEST(ProcessTreeTests, BasicAssertions) {
// Expect two strings not to be equal.
EXPECT_STRNE("hello", "world");
// Expect equality.
EXPECT_EQ(7 * 6, 42);
}

TEST(ProcessTreeTests, Parser1) {
    try {
        process_parser p("a b c");
        FAIL();
    } catch (const int e) {
        SUCCEED();
    }
}
