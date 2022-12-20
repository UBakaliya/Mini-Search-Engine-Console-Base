#include "headerFiles/search.h"
#include "gtest/gtest.h"

// Test the cleanToken() function
TEST(SearchTest, TestCleanToken)
{
    EXPECT_EQ("hello, world", cleanToken("Hello, World!"));
    EXPECT_EQ("", cleanToken("!!!"));
    EXPECT_EQ("hello, world! 123", cleanToken("Hello, World! 123"));
}

// Test the gatherTokens() function
TEST(SearchTest, TestGatherTokens)
{
    set<string> expected1 = {"hello", "world"};
    set<string> expected2 = {"hello", "world", "123"};
    set<string> expected3 = {"hello", "world", "123", "456", "789"};
    EXPECT_EQ(expected1, gatherTokens("Hello, World!"));
    EXPECT_EQ(expected2, gatherTokens("Hello, World! 123"));
    EXPECT_EQ(expected3, gatherTokens("Hello, World! 123 456 789"));
}
