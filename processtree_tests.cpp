#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

#include "processtree.h"

class ParserTest : public testing::Test {
protected:
    static void SetUpTestSuite() {
        spdlog::set_level(spdlog::level::off);
        spdlog::info("spdlog level set to {}", spdlog::get_level()); // should be invisible!
    }

    static void TearDownTestSuite() {
    }
};

TEST_F(ParserTest, ParserHeaderBad1) {
    EXPECT_ANY_THROW(
        auto p = parse_header("a b c");
    );
}

TEST_F(ParserTest, ParserHeaderBad2) {
    EXPECT_ANY_THROW(
        auto p = parse_header("a CMD b PID c PPID d");
    );
}

TEST_F(ParserTest, ParserHeaderBad3) {
    EXPECT_ANY_THROW(
        auto p = parse_header("a COMMAND b PID c PPID d");
    );
}

TEST_F(ParserTest, ParserHeaderBad4) {
    EXPECT_ANY_THROW(
        auto p = parse_header("a PID c PPID d CMD e");
    );
}

TEST_F(ParserTest, ParserHeaderGood1) {
    //                     01234567890123456789012345678901234
    auto p = parse_header("a bb PID cccc PPID dddd eee COMMAND");
    EXPECT_EQ(p.pid, 2);
    EXPECT_EQ(p.ppid, 4);
    EXPECT_EQ(p.cmd, 28);
}

TEST_F(ParserTest, ParserHeaderGood2) {
    //                     01234567890123456789012345678901234
    auto p = parse_header("a bb PID cccc PPID dddd eee CMD");
    EXPECT_EQ(p.pid, 2);
    EXPECT_EQ(p.ppid, 4);
    EXPECT_EQ(p.cmd, 28);
}

TEST_F(ParserTest, ParserHeaderGood3) {
    //                     01234567890123456789012345678901234
    auto p = parse_header("a bb PPID cccc PID dddd eee COMMAND");
    EXPECT_EQ(p.pid, 4);
    EXPECT_EQ(p.ppid, 2);
    EXPECT_EQ(p.cmd, 28);
}

TEST_F(ParserTest, ParserHeaderGood4) {
    //                     01234567890123456789012345678901234
    auto p = parse_header("a bb PPID cccc PID dddd eee CMD");
    EXPECT_EQ(p.pid, 4);
    EXPECT_EQ(p.ppid, 2);
    EXPECT_EQ(p.cmd, 28);
}

TEST_F(ParserTest, ParserHeaderGood5) {
    //                     01234567890123456789012345678901234567890123456789
    auto p = parse_header("  UID   PID  PPID   C STIME   TTY           TIME CMD");
    EXPECT_EQ(p.pid, 1);
    EXPECT_EQ(p.ppid, 2);
    EXPECT_EQ(p.cmd, 49);
}
