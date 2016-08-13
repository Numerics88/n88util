#include <gtest/gtest.h>

#include "n88util/text.hpp"


// Create a test fixture class.
class textTests : public ::testing::Test
{};

// --------------------------------------------------------------------
// test implementations

// Basic test of split_arguments
TEST_F (textTests, split_arguments)
{
  std::string input = "one two\t3 four,five";
  std::vector<std::string> tokens;
  n88util::split_arguments(input, tokens);
  ASSERT_EQ(tokens.size(), 5);
  ASSERT_EQ(tokens[0], "one");
  ASSERT_EQ(tokens[1], "two");
  ASSERT_EQ(tokens[2], "3");
  ASSERT_EQ(tokens[3], "four");
  ASSERT_EQ(tokens[4], "five");
}

// test split_arguments with consecutive separators
TEST_F (textTests, split_arguments_consecutive_separators)
{
  std::string input = "one  two\t,3";
  std::vector<std::string> tokens;
  n88util::split_arguments(input, tokens);
  ASSERT_EQ(tokens.size(), 3);
  ASSERT_EQ(tokens[0], "one");
  ASSERT_EQ(tokens[1], "two");
  ASSERT_EQ(tokens[2], "3");
}

// test specifying separators
TEST_F (textTests, split_arguments_specify_separators)
{
  std::string input = "one two\t3 four,five";
  std::vector<std::string> tokens;
  n88util::split_arguments(input, tokens, "f3");
  ASSERT_EQ(tokens.size(), 4);
  ASSERT_EQ(tokens[0], "one two\t");
  ASSERT_EQ(tokens[1], " ");
  ASSERT_EQ(tokens[2], "our,");
  ASSERT_EQ(tokens[3], "ive");
}

// Basic test of split_trim
TEST_F (textTests, split_trim)
{
  std::string input = "one, two\t3 four,five";
  std::vector<std::string> tokens;
  n88util::split_trim(input, tokens);
  ASSERT_EQ(tokens.size(), 3);
  ASSERT_EQ(tokens[0], "one");
  ASSERT_EQ(tokens[1], "two\t3 four");
  ASSERT_EQ(tokens[2], "five");
}

// test split_trim with consecutive separators
TEST_F (textTests, split_trim_consecutive_separators)
{
  std::string input = "one, two\t3 four,,five";
  std::vector<std::string> tokens;
  n88util::split_trim(input, tokens);
  ASSERT_EQ(tokens.size(), 4);
  ASSERT_EQ(tokens[0], "one");
  ASSERT_EQ(tokens[1], "two\t3 four");
  ASSERT_EQ(tokens[2], "");
  ASSERT_EQ(tokens[3], "five");
}
