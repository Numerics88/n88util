#include <gtest/gtest.h>

#include "n88util/binhex.hpp"


// Create a test fixture class.
class binhexTests : public ::testing::Test
{};

// --------------------------------------------------------------------
// test implementations

// Test that bintohex returns valid hex values.
TEST_F (binhexTests, valid_hex_values)
{
  const unsigned char buffer[] = "This is a longish buffer\nWith two lines even!\n";
  const char valid_hex_chars[] = "0123456789ABCDEF";
  char* hex = NULL;
  int return_val;
  return_val = n88util::bintohex(buffer, strlen((const char*)buffer), &hex);
  ASSERT_EQ(return_val, 1);
  for (long i=0; i<strlen(hex); i++)
  {
    ASSERT_TRUE(strchr(valid_hex_chars,hex[i]));
  }
}

// Test that bintohex followed by hextobin gives us the original result.
TEST_F (binhexTests, round_trip_bin_to_bin)
{
  const unsigned char buffer[] = "This is a longish buffer\nWith two lines even!\n";
  char* hex = NULL;
  int return_val;
  return_val = n88util::bintohex(buffer, strlen((const char*)buffer), &hex);
  ASSERT_EQ(return_val, 1);
  unsigned char* returned_buffer;
  long len;
  return_val = n88util::hextobin(hex, &returned_buffer, &len);
  ASSERT_EQ(return_val, 1);
  ASSERT_EQ(len, strlen((const char*)buffer));
  ASSERT_EQ(strncmp((const char*)buffer, (const char*)returned_buffer, len), 0);
  free(returned_buffer);
  free(hex);
}

// Test that hextobin followed by bintohex gives us the original result.
TEST_F (binhexTests, round_trip_hex_to_hex)
{
  const char hex[] = "3081FA0201000241";
  unsigned char* buffer = NULL;
  int return_val;
  long len;
  return_val = n88util::hextobin(hex, &buffer, &len);
  ASSERT_EQ(return_val, 1);
  ASSERT_EQ(len, strlen(hex)/2);
  char* returned_hex;
  return_val = n88util::bintohex(buffer, len, &returned_hex);
  ASSERT_EQ(return_val, 1);
  ASSERT_EQ(strcmp(returned_hex, hex), 0);
  free(returned_hex);
  free(buffer);
}

// Test that invalid hex value returns 0.
TEST_F (binhexTests, invalid_hex)
{
  const char bad_hex[] = "3081FA0T01000241"; // note: not valid hex
  unsigned char* buffer = NULL;
  int return_val;
  long len;
  return_val = n88util::hextobin(bad_hex, &buffer, &len);
  ASSERT_EQ(return_val, 0);
  ASSERT_EQ(buffer, (unsigned char*)NULL);
  ASSERT_EQ(len, 0);
  const char bad_hex2[] = "3081FA00T1000241"; // note: not valid hex
  return_val = n88util::hextobin(bad_hex2, &buffer, &len);
  ASSERT_EQ(return_val, 0);
  ASSERT_EQ(buffer, (unsigned char*)NULL);
  ASSERT_EQ(len, 0);
}

// Test that empty buffer returns empty hex string.
TEST_F (binhexTests, empty_buffer)
{
  const unsigned char buffer[] = "";
  char* hex = NULL;
  int return_val;
  return_val = n88util::bintohex(buffer, 0, &hex);
  ASSERT_EQ(return_val, 1);
  ASSERT_FALSE(hex == NULL);
  ASSERT_EQ(strlen(hex), 0);
  free(hex);
}

// Test that empty hex string returns empty buffer.
TEST_F (binhexTests, empty_hex)
{
  const char hex[] = "";
  unsigned char* buffer = NULL;
  int return_val;
  long len;
  return_val = n88util::hextobin(hex, &buffer, &len);
  ASSERT_EQ(return_val, 1);
  ASSERT_EQ(len, 0);
  // no test for buffer - don't really care if it's NULL or not.
  free(buffer);
}

// Test that using a C++ string as a parameter gives the same result for bintohex.
TEST_F (binhexTests, test_bintohex_cpp_string)
{
  const unsigned char buffer[] = "This is a longish buffer\nWith two lines even!\n";
  char* hex_char = NULL;
  int return_val;
  return_val = n88util::bintohex(buffer, strlen((const char*)buffer), &hex_char);
  ASSERT_EQ(return_val, 1);
  std::string hex_string;
  return_val = n88util::bintohex(buffer, strlen((const char*)buffer), hex_string);
  ASSERT_EQ(return_val, 1);
  ASSERT_EQ(hex_string.compare(hex_char), 0);
  free(hex_char);
}

// Test that using a C++ string as a parameter gives the same result for hextobin.
TEST_F (binhexTests, test_hextobin_cpp_string)
{
  const char hex_char[] = "";
  unsigned char* buffer_char = NULL;
  long len_char;
  int return_val;
  return_val = n88util::hextobin(hex_char, &buffer_char, &len_char);
  ASSERT_EQ(return_val, 1);
  unsigned char* buffer_string = NULL;
  long len_string;
  const std::string hex_string(hex_char);
  return_val = n88util::hextobin(hex_string, &buffer_string, &len_string);
  ASSERT_EQ(return_val, 1);
  ASSERT_EQ(len_char, len_string);
  ASSERT_EQ(strncmp((char*)buffer_char, (char*)buffer_string, len_char), 0);
  free(buffer_string);
  free(buffer_char);
}
