#include "n88util/const_array.hpp"
#include "n88util/array.hpp"
#include <gtest/gtest.h>

using namespace n88;

// Create a test fixture class.
class const_array_Tests : public ::testing::Test
{};

// --------------------------------------------------------------------
// test implementations

TEST_F (const_array_Tests, FlatIndexing)
{
  array<2,double> A(3,4);
  A(2,3) = 4.0;
  A(1,2) = 3.0;
  const_array<2,double> C(A);
  ASSERT_EQ(C[11], 4.0);
  ASSERT_EQ(C[6], 3.0);
}

TEST_F (const_array_Tests, PointerConstructor)
{
  double a[] = {1.0,2.0,3.0,4.0,5.0,6.0};
  const_array<2,double> C(a,2,3);
  ASSERT_EQ(C(0,2), 3.0);
  ASSERT_EQ(C(1,2), 6.0);
}

TEST_F (const_array_Tests, ReferenceConstructor)
{
  double a[] = {1.0,2.0,3.0,4.0,5.0,6.0};
  const_array<2,double> A(a,2,3);
  const_array<2,double> B(A);
  ASSERT_EQ(B.size(), 6);
  ASSERT_EQ(B.dims(), (tuplet<2,size_t>(2,3)));
  ASSERT_EQ(A.data(), B.data());
  ASSERT_EQ(B(0,2), 3.0);
  ASSERT_EQ(B(1,2), 6.0);
}

TEST_F (const_array_Tests, Pointer)
{
  array<2,double> A(3,4);
  const_array<2,double> C(A);
  ASSERT_EQ(&(C(1,2)), C.data() + 6);
}

TEST_F (const_array_Tests, End)
{
  array<2,double> A(3,4);
  const_array<2,double> C(A);
  ASSERT_EQ(C.data() + 12, C.end());
}

TEST_F (const_array_Tests, FlatIndexing3)
{
  array<3,double> A(2,3,4);
  A(1,2,3) = 4.0;
  A(0,1,2) = 3.0;
  const_array<3,double> C(A);
  ASSERT_EQ(C[23], 4.0);
  ASSERT_EQ(C[6], 3.0);
}

