#include "n88util/array.hpp"
#include <gtest/gtest.h>

using namespace n88;

// Create a test fixture class.
class arrayTests : public ::testing::Test
{};

// --------------------------------------------------------------------
// test implementations

TEST_F (arrayTests, SimpleAssignment)
{
  array<2,double> A(3,4);
  A(2,3) = 4.0;
  A(1,2) = 3.0;
  ASSERT_EQ(A(2,3), 4.0);
  ASSERT_EQ(A(1,2), 3.0);
}

TEST_F (arrayTests, FlatIndexing)
{
  array<2,double> A(3,4);
  A(2,3) = 4.0;
  A(1,2) = 3.0;
  ASSERT_EQ(A[11], 4.0);
  ASSERT_EQ(A[6], 3.0);
}

TEST_F (arrayTests, PointerConstructor)
{
  double a[] = {1.0,2.0,3.0,4.0,5.0,6.0};
  array<2,double> A(a,2,3);
  ASSERT_EQ(A(0,2), 3.0);
  ASSERT_EQ(A(1,2), 6.0);
}

TEST_F (arrayTests, ReferenceConstructor)
{
  double a[] = {1.0,2.0,3.0,4.0,5.0,6.0};
  array<2,double> A(a,2,3);
  array<2,double> B(A);
  ASSERT_EQ(B.size(), 6);
  ASSERT_EQ(B.dims(), (tuplet<2,size_t>(2,3)));
  ASSERT_EQ(A.data(), B.data());
  ASSERT_EQ(B(0,2), 3.0);
  ASSERT_EQ(B(1,2), 6.0);
}

TEST_F (arrayTests, Pointer)
{
  array<2,double> A(3,4);
  ASSERT_EQ(&(A(1,2)), A.data() + 6);
}

TEST_F (arrayTests, End)
{
  array<2,double> A(3,4);
  ASSERT_EQ(A.data() + 12, A.end());
}

TEST_F (arrayTests, Zero)
{
  array<2,double> A(3,4);
  A(2,3) = 4.0;
  A(1,2) = 3.0;
  A.zero();
  ASSERT_EQ(A(2,3), 0.0);
  ASSERT_EQ(A(1,2), 0.0);
}

TEST_F (arrayTests, Copy)
{
  double a[] = {1.0,2.0,3.0,4.0,5.0,6.0};
  array<2,double> A(a,2,3);
  array<2,double> B(2,3);
  B.copy(A);
  ASSERT_EQ(B.size(), 6);
  ASSERT_NE(A.data(), B.data());
  ASSERT_EQ(B(0,2), A(0,2));
  ASSERT_EQ(B(1,2), A(1,2));
}

TEST_F (arrayTests, CopyPointer)
{
  double a[] = {1.0,2.0,3.0,4.0,5.0,6.0};
  array<2,double> A(2,3);
  A.copy(a);
  ASSERT_NE(A.data(), a);
  ASSERT_EQ(A(0,2),3.0);
  ASSERT_EQ(A(1,2),6.0);
}

TEST_F (arrayTests, SimpleAssignment3)
{
  array<3,double> A(2,3,4);
  A(1,2,3) = 4.0;
  A(0,1,2) = 3.0;
  ASSERT_EQ(A(1,2,3), 4.0);
  ASSERT_EQ(A(0,1,2), 3.0);
}

TEST_F (arrayTests, FlatIndexing3)
{
  array<3,double> A(2,3,4);
  A(1,2,3) = 4.0;
  A(0,1,2) = 3.0;
  ASSERT_EQ(A[23], 4.0);
  ASSERT_EQ(A[6], 3.0);
}

