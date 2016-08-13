#include "n88util/tuplet.hpp"
#include <gtest/gtest.h>
#include <cmath>

using namespace n88;

// Create a test fixture class.
class tupletTests : public ::testing::Test
{};

// --------------------------------------------------------------------
// test implementations

TEST_F (tupletTests, Zeros)
{
  tuplet<3,float> b(tuplet<3,float>::zeros());
  ASSERT_EQ(b[0], 0);
  ASSERT_EQ(b[1], 0);
  ASSERT_EQ(b[2], 0);
}

TEST_F (tupletTests, Ones)
{
  tuplet<3,float> c(tuplet<3,float>(1.0,1.0,1.0));
  ASSERT_EQ(c[0], 1.0);
  ASSERT_EQ(c[1], 1.0);
  ASSERT_EQ(c[2], 1.0);
}

TEST_F (tupletTests, CopyConstruction)
{
  tuplet<3,float> d(2.0, 4.0, 6.0);
  tuplet<3,float> e(d);
  ASSERT_EQ(e[0], 2.0);
  ASSERT_EQ(e[1], 4.0);
  ASSERT_EQ(e[2], 6.0);
}

TEST_F (tupletTests, CopyAndConvertFloatConstruction)
{
  tuplet<3,float> d(2.2, 4.4, 6.6);
  tuplet<3,double> e(d);
  ASSERT_FLOAT_EQ(e[0], 2.2);
  ASSERT_FLOAT_EQ(e[1], 4.4);
  ASSERT_FLOAT_EQ(e[2], 6.6);
}

TEST_F (tupletTests, CopyAndConvertIntConstruction)
{
  tuplet<3,float> d(2.2, 4.4, 6.6);
  tuplet<3,int> e(d);
  ASSERT_EQ(e[0], 2);
  ASSERT_EQ(e[1], 4);
  ASSERT_EQ(e[2], 6);
}

TEST_F (tupletTests, ConstructionFromPointer)
{
  float d[3] = {2.0, 4.0, 6.0};
  tuplet<3,float> e(d);
  ASSERT_EQ(e[0], 2.0);
  ASSERT_EQ(e[1], 4.0);
  ASSERT_EQ(e[2], 6.0);
}

TEST_F (tupletTests, Pointer)
{
  tuplet<3,float> d(2.0, 4.0, 6.0);
  ASSERT_EQ(&(d[2]), d.data()+2);
}

TEST_F (tupletTests, Assignment)
{
  float d[3] = {2.0, 4.0, 6.0};
  tuplet<3,float> f;
  f = d;
  ASSERT_EQ(f[0], 2.0);
  ASSERT_EQ(f[1], 4.0);
  ASSERT_EQ(f[2], 6.0);
}

TEST_F (tupletTests, Equality)
{
  tuplet<3,float> a(1.0,3.0,5.0);
  tuplet<3,float> b(1.0,3.0,5.0);
  ASSERT_EQ(a,a);
  ASSERT_EQ(a,b);
}

TEST_F (tupletTests, Inequality)
{
  tuplet<3,float> a(1.0,3.0,5.0);
  tuplet<3,float> b(1.0,3.0,5.1);
  ASSERT_NE(a,b);
}

TEST_F (tupletTests, Negation)
{
  tuplet<3,float> a(1.0,3.0,5.0);
  tuplet<3,float> b = -a;
  ASSERT_EQ(b[0], -1.0);
  ASSERT_EQ(b[1], -3.0);
  ASSERT_EQ(b[2], -5.0);
}

TEST_F (tupletTests, StreamOperatorsSquareBracket)
{
  tuplet<3,int> a;
  tuplet<3,int> b(1,4,6);
  std::stringstream buffer;
  buffer << "[1,4,6]";
  ASSERT_FALSE(buffer.fail());
  buffer >> a;
  ASSERT_EQ(a, b);
  ASSERT_TRUE(buffer.eof());
  ASSERT_FALSE(buffer.fail());
}

TEST_F (tupletTests, StreamOperatorsRoundBracket)
{
  tuplet<3,int> a;
  tuplet<3,int> b(1,4,6);
  std::stringstream buffer;
  buffer << "(1,4,6)";
  ASSERT_FALSE(buffer.fail());
  buffer >> a;
  ASSERT_EQ(a, b);
  ASSERT_TRUE(buffer.eof());
  ASSERT_FALSE(buffer.fail());
}

TEST_F (tupletTests, StreamOperatorsNoBracket)
{
  tuplet<3,int> a;
  tuplet<3,int> b(1,4,6);
  std::stringstream buffer;
  buffer << "1,4,6";
  ASSERT_FALSE(buffer.fail());
  buffer >> a;
  ASSERT_EQ(a, b);
  ASSERT_TRUE(buffer.eof());
  ASSERT_FALSE(buffer.fail());
}

TEST_F (tupletTests, StreamOperators)
{
  tuplet<3,int> a;
  tuplet<3,int> b(1,4,6);
  std::stringstream buffer;
  buffer << "[1,4,6]";
  ASSERT_FALSE(buffer.fail());
  buffer >> a;
  ASSERT_EQ(a, b);
  ASSERT_TRUE(buffer.eof());
  ASSERT_FALSE(buffer.fail());
}

TEST_F (tupletTests, StreamOperators2)
{
  tuplet<3,int> a;
  tuplet<3,int> b(1,4,6);
  std::stringstream buffer;
  buffer.clear();
  buffer << b;
  buffer >> a;
  ASSERT_EQ(a, b);
  ASSERT_TRUE(buffer.eof());
  ASSERT_FALSE(buffer.fail());
}

TEST_F (tupletTests, StreamOperatorstuplettuplet)
{
  std::stringstream buffer;
  buffer << "[[2,4],[3,5]]";
  tuplet<2,tuplet<2,int> > a;
  buffer >> a;
  ASSERT_EQ(a[0][0], 2);
  ASSERT_EQ(a[0][1], 4);
  ASSERT_EQ(a[1][0], 3);
  ASSERT_EQ(a[1][1], 5);
  ASSERT_TRUE(buffer.eof());
  ASSERT_FALSE(buffer.fail());
}

TEST_F (tupletTests, AddAScalar)
{
  tuplet<1,int> a(2);
  tuplet<2,int> b(2,3);
  tuplet<3,int> c(2,3,4);
  tuplet<4,int> d(2,3,4,5);
  int s = 3;
  ASSERT_EQ(a+s, (tuplet<1,int>(5)));
  ASSERT_EQ(b+s, (tuplet<2,int>(5,6)));
  ASSERT_EQ(c+s, (tuplet<3,int>(5,6,7)));
  ASSERT_EQ(d+s, (tuplet<4,int>(5,6,7,8)));
}

TEST_F (tupletTests, Addition)
{
  tuplet<1,int> a(2);
  tuplet<1,int> a2(10);
  tuplet<2,int> b(2,3);
  tuplet<2,int> b2(20,10);
  tuplet<3,int> c(2,3,4);
  tuplet<3,int> c2(30,20,10);
  tuplet<4,int> d(2,3,4,5);
  tuplet<4,int> d2(40,30,20,10);
  ASSERT_EQ(a+a2, (tuplet<1,int>(12)));
  ASSERT_EQ(b+b2, (tuplet<2,int>(22,13)));
  ASSERT_EQ(c+c2, (tuplet<3,int>(32,23,14)));
  ASSERT_EQ(d+d2, (tuplet<4,int>(42,33,24,15)));
}

TEST_F (tupletTests, SubtractAScalar)
{
  tuplet<1,int> a(2);
  tuplet<2,int> b(2,3);
  tuplet<3,int> c(2,3,4);
  tuplet<4,int> d(2,3,4,5);
  int s = 3;
  ASSERT_EQ(a-s, (tuplet<1,int>(-1)));
  ASSERT_EQ(b-s, (tuplet<2,int>(-1,0)));
  ASSERT_EQ(c-s, (tuplet<3,int>(-1,0,1)));
  ASSERT_EQ(d-s, (tuplet<4,int>(-1,0,1,2)));
}

TEST_F (tupletTests, Subtraction)
{
  tuplet<1,int> a(2);
  tuplet<1,int> a2(10);
  tuplet<2,int> b(2,3);
  tuplet<2,int> b2(20,10);
  tuplet<3,int> c(2,3,4);
  tuplet<3,int> c2(30,20,10);
  tuplet<4,int> d(2,3,4,5);
  tuplet<4,int> d2(40,30,20,10);
  ASSERT_EQ(a-a2, (tuplet<1,int>(-8)));
  ASSERT_EQ(b-b2, (tuplet<2,int>(-18,-7)));
  ASSERT_EQ(c-c2, (tuplet<3,int>(-28,-17,-6)));
  ASSERT_EQ(d-d2, (tuplet<4,int>(-38,-27,-16,-5)));
}

TEST_F (tupletTests, MultiplicationByScalar)
{
  tuplet<1,int> a(2);
  tuplet<2,int> b(2,3);
  tuplet<3,int> c(2,3,4);
  tuplet<4,int> d(2,3,4,5);
  int s = 3;
  ASSERT_EQ(s*a, (tuplet<1,int>(6)));
  ASSERT_EQ(s*b, (tuplet<2,int>(6,9)));
  ASSERT_EQ(s*c, (tuplet<3,int>(6,9,12)));
  ASSERT_EQ(s*d, (tuplet<4,int>(6,9,12,15)));
}

TEST_F (tupletTests, Multiplication)
{
  tuplet<1,int> a(2);
  tuplet<1,int> a2(10);
  tuplet<2,int> b(2,3);
  tuplet<2,int> b2(20,10);
  tuplet<3,int> c(2,3,4);
  tuplet<3,int> c2(30,20,10);
  tuplet<4,int> d(2,3,4,5);
  tuplet<4,int> d2(40,30,20,10);
  ASSERT_EQ(a*a2, (tuplet<1,int>(20)));
  ASSERT_EQ(b*b2, (tuplet<2,int>(40,30)));
  ASSERT_EQ(c*c2, (tuplet<3,int>(60,60,40)));
  ASSERT_EQ(d*d2, (tuplet<4,int>(80,90,80,50)));
}

TEST_F (tupletTests, DivideByScalar)
{
  tuplet<1,float> a(2);
  tuplet<2,float> b(2,3);
  tuplet<3,float> c(2,3,4);
  float s = 3;
  ASSERT_EQ(a/s, (tuplet<1,float>(float(2)/float(3))));
  ASSERT_EQ(b/s, (tuplet<2,float>(float(2)/float(3),float(3)/float(3))));
  ASSERT_EQ(c/s, (tuplet<3,float>(float(2)/float(3),float(3)/float(3),float(4)/float(3))));
}

TEST_F (tupletTests, Division)
{
  tuplet<1,float> a(2);
  tuplet<1,float> a2(10);
  tuplet<2,float> b(2,3);
  tuplet<2,float> b2(20,10);
  ASSERT_EQ(a/a2, (tuplet<1,float>(float(2)/float(10))));
  ASSERT_EQ(b/b2, (tuplet<2,float>(float(2)/float(20),float(3)/float(10))));
}

TEST_F (tupletTests, Product)
{
  tuplet<1,int> a(2);
  tuplet<2,int> b(2,3);
  tuplet<3,int> c(2,3,4);
  tuplet<4,int> d(2,3,4,5);
  ASSERT_EQ(product(a), 2);
  ASSERT_EQ(product(b), 6);
  ASSERT_EQ(product(c), 24);
  ASSERT_EQ(product(d), 120);
}

TEST_F (tupletTests, LongProduct)
{
  tuplet<2,int> b(1073741824,1073741824);
  ASSERT_EQ(long_product(b), 1152921504606846976);
}

TEST_F (tupletTests, Sum)
{
  tuplet<1,int> a(2);
  tuplet<2,int> b(2,3);
  tuplet<3,int> c(2,3,4);
  tuplet<4,int> d(2,3,4,5);
  ASSERT_EQ(sum(a), 2);
  ASSERT_EQ(sum(b), 5);
  ASSERT_EQ(sum(c), 9);
  ASSERT_EQ(sum(d), 14);
}

TEST_F (tupletTests, Norm)
{
  tuplet<1,float> a(2);
  tuplet<2,float> b(3,4);
  tuplet<3,float> c(2,3,4);
  tuplet<4,float> d(2,3,4,5);
  ASSERT_FLOAT_EQ(norm(a), 2);
  ASSERT_FLOAT_EQ(norm(b), 5);
  ASSERT_FLOAT_EQ(norm(c), sqrt(sum(c*c)));
  ASSERT_FLOAT_EQ(norm(d), sqrt(sum(d*d)));
}

TEST_F (tupletTests, Dot)
{
  tuplet<1,int> a(2);
  tuplet<1,int> a2(10);
  tuplet<2,int> b(2,3);
  tuplet<2,int> b2(20,10);
  tuplet<3,int> c(2,3,4);
  tuplet<3,int> c2(30,20,10);
  tuplet<4,int> d(2,3,4,5);
  tuplet<4,int> d2(40,30,20,10);
  ASSERT_EQ(dot(a,a2), 20);
  ASSERT_EQ(dot(a,a), 4);
  ASSERT_EQ(dot(b,b2), 70);
  ASSERT_EQ(dot(b,b), 13);
  ASSERT_EQ(dot(c,c2), 160);
  ASSERT_EQ(dot(c,c), 29);
  ASSERT_EQ(dot(d,d2), 300);
  ASSERT_EQ(dot(d,d), 54);
}

TEST_F (tupletTests, Reverse)
{
  tuplet<1,int> a(2);
  tuplet<2,int> b(2,3);
  tuplet<3,int> c(2,3,4);
  tuplet<4,int> d(2,3,4,5);
  ASSERT_EQ(reverse(a), (tuplet<1,int>(2)));
  ASSERT_EQ(reverse(b), (tuplet<2,int>(3,2)));
  ASSERT_EQ(reverse(c), (tuplet<3,int>(4,3,2)));
  ASSERT_EQ(reverse(d), (tuplet<4,int>(5,4,3,2)));
}
