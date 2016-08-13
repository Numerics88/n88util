// Equality comparision of floating point values.
//
// Copyright (c) Eric Nodwell
// See LICENSE for details.
//
// Since floating point values are approximate, equality comparison should be
// done to within some limited precision.  In some cases, the best measure of
// precision is a certain number of "ulps" ("Units in Last Place"), where ulps
// essentially is a count of how many representable floating point numbers
// separate two given values.  This is what these functions do.
//
// It is worth emphasizing that a maximum difference of ulps is often not
// the best equality comparison.  However you typically require specific
// domain knowledge of expected precision or error in order to do better.  In
// cases where you don't know or don't care particularly about the expected
// precision, ULPs is typically the best default fall-back.
//
// Basic usage:
//
//   Step 1. (Optional but highly recommended.)
//   Call bonelabMisc::SanityCheck() somewhere in your code.
//
//   Step 2.
//   Compare floating point values (floats and/or doubles), like this:
//
//     if (bonelabMisc::EssentiallyEqual(x,y))
//
//   or
//
//     if (bonelabMisc::ApproximatelyEqual(x,y))
//
//  The former checks that two floating point numbers are within 1 ULP
//  of each other, while the latter has a looser tolerance equal to
//  APPROXIMATELY_EQUAL_ULPS .  I have set APPROXIMATELY_EQUAL_ULPS to 16,
//  but you may tweak it if you want something else.
//
//  As an additional note, one should never use the == operator to compare
//  a float with a double.  The comparsion mostly returns false because
//  following the standard rules of C++, the float is promoted to a double
//  before == is applied.  Thus, you are effectively trying to compare more
//  digits than the number actually has.  The templated comparison functions
//  here by contrast, will demote the double to float if passed both a float
//  and a double.  This is what you want: a meaningful comparison can only be
//  made with the precision of the least precise input. 
//
//  This algorithm is a modified verion of the one from:
//    http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm 
//
//  Boost also has a floating-point comparison module.  It is more appropriate
//  in cases where one can make an estimate for the uncertainty epsilon:
//    http://www.boost.org/doc/libs/1_34_0/libs/test/doc/components/test_tools/floating_point_comparison.html
//
//  For even more information on floating point numbers, refer to
//    http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.22.6768

#include <boost/static_assert.hpp>
#include <boost/math/special_functions/fpclassify.hpp>  // for isnan
#include <boost/cstdint.hpp>
#include <limits>
#include <cstdlib>

// Uncomment this line to get very verbose reporting of comparison.
//#define TRACE_ALMOSTEQUAL2SCOMPLEMENT

namespace bonelabMisc
{

using boost::int64_t;

// Change this value to
const int APPROXIMATELY_EQUAL_ULPS = 16;

// Checks we can do at compile time.
// Please ensure that you also call SanityCheck() in your code.
//
// Note: Hypothetically, one could write the functions below as macros
//       which could then be called as static asserts, thus permitting
//       the whole of SanityCheck to be implemented as static asserts.
//       Then, since inline functions are much preferable to macros
//       for the user, the functions would all wrap the corresponding macro.
//       For now, this is too much work.
BOOST_STATIC_ASSERT(sizeof(int) == sizeof(float));
BOOST_STATIC_ASSERT(sizeof(int64_t) == sizeof(double));


// Description:
// Explicit reinterpretation of float as a 32 bit integer.
// Key point: The value of the int is meaningless, but order is preserved.
inline int FloatTwosComplementUlps (float x)
{
  int xInt = *(int*)&x;
  // Make aInt lexicographically ordered as a twos-complement int
  if (xInt < 0)
    xInt = 0x80000000 - xInt;
  return xInt;
}

// Description:
// Explicit reinterpretation of double as a 64 bit integer
// Key point: The value of the int is meaningless, but order is preserved.
inline int64_t DoubleTwosComplementUlps (double x)
{
  int64_t xInt = *(int64_t*)&x;
  // Make aInt lexicographically ordered as a twos-complement int
  if (xInt < 0)
    xInt = 0x8000000000000000 - xInt;
  return xInt;
}

// Description:
// Given a certain specified precision, return the corresponding number of ulps.
// Note that the correspondence only holds approximately, and is wildly
// inaccurate for denormalized numbers.
inline int FloatUlpsFromPrecision (float p)
{
  float x = 1.0f;
  int ulps = FloatTwosComplementUlps(x+p) - FloatTwosComplementUlps(x);
  return ulps;
}

// Description:
// Given a certain specified precision, return the corresponding number of ulps.
// Note that the correspondence only holds approximately, and is wildly
// inaccurate for denormalized numbers.
inline int64_t DoubleUlpsFromPrecision (double p)
{
  double x = 1.0;
  int64_t ulps = DoubleTwosComplementUlps(x+p) - DoubleTwosComplementUlps(x);
  return ulps;
}

// Description:
// Given the specified number of ulps, return the approximate corresponding
// relative precision.
// Note: Not valid for denormalized numbers.
inline float PrecisionOfFloatUlps (int ulps=1)
{
  float x = 1.0f;
  int xInt = FloatTwosComplementUlps(x);
  int yInt = xInt + ulps;
  float y = *(float*)&yInt;
  return y-x;
}

// Description:
// Given the specified number of ulps, return the approximate corresponding
// relative precision.
// Note: Not valid for denormalized numbers.
inline double PrecisionOfDoubleUlps (int ulps=1)
{
  double x = 1.0f;
  int64_t xInt = DoubleTwosComplementUlps(x);
  int64_t yInt = xInt + ulps;
  double y = *(double*)&yInt;
  return y-x;
}

// Description:
// Compare two floats and return true if they are within a certain number of
// ULPS of each other.
inline bool FloatAlmostEqual2sComplement (float a, float b, int maxUlps=1)
{
    int aInt = FloatTwosComplementUlps(a);
    int bInt = FloatTwosComplementUlps(b);
    int intDiff = std::abs(aInt - bInt);
#ifdef TRACE_ALMOSTEQUAL2SCOMPLEMENT
    cout << "\n";
    cout << std::dec << "a = " << a << "\n";
    cout << std::dec << "b = " << b << "\n";
    cout << std::dec << "*(int*)&a = " << *(int*)&a << " " << std::hex << *(int*)&a << "\n";
    cout << std::dec << "*(int*)&b = " << *(int*)&b << " " << std::hex << *(int*)&b << "\n";
    cout << std::dec << "aInt = " << aInt << " " << std::hex << aInt << "\n";
    cout << std::dec << "bInt = " << bInt << " " << std::hex << bInt << "\n";
    cout << std::dec << "abs(aInt - BInt) = " << intDiff << " " << std::hex << intDiff << "\n";
    cout << std::dec << "maxUlps = " << maxUlps << "\n";
#endif
    // Note that the correct answer to NaN == NaN is false; hence the special case.
    if (intDiff <= maxUlps && !(boost::math::isnan)(a))
        return true;
    return false;
}

// Description:
// Compare two doubles and return true if they are within a certain number of
// ULPS of each other.
inline bool DoubleAlmostEqual2sComplement (double a, double b, int64_t maxUlps=1)
{
    int64_t aInt = DoubleTwosComplementUlps(a);
    int64_t bInt = DoubleTwosComplementUlps(b);
#ifdef WIN32
    // 64 bit Windows has no abs function for 64 bit integers.
    int64_t signedIntDiff = aInt - bInt;
    int64_t intDiff = (signedIntDiff >= 0) ? signedIntDiff : -signedIntDiff;
#else
    int64_t intDiff = std::abs(aInt - bInt);
#endif
#ifdef TRACE_ALMOSTEQUAL2SCOMPLEMENT
    cout << "\n";
    cout << std::dec << "a = " << a << "\n";
    cout << std::dec << "b = " << b << "\n";
    cout << std::dec << "*(int64_t*)&a = " << *(int64_t*)&a << " " << std::hex << *(int64_t*)&a << "\n";
    cout << std::dec << "*(int64_t*)&b = " << *(int64_t*)&b << " " << std::hex << *(int64_t*)&b << "\n";
    cout << std::dec << "aInt = " << aInt << " " << std::hex << aInt << "\n";
    cout << std::dec << "bInt = " << bInt << " " << std::hex << bInt << "\n";
    cout << std::dec << "abs(aInt - BInt) = " << intDiff << " " << std::hex << intDiff << "\n";
    cout << std::dec << "maxUlps = " << maxUlps << "\n";
#endif
    // Note that the correct answer to NaN == NaN is false; hence the special case.
    if (intDiff <= maxUlps && !(boost::math::isnan)(a))
        return true;
    return false;
}

// --------------------------------------------------------------------------
// Templated versions of comparison functions
//
// Note: Comparison of mixed float and double will default to the float
//       versions of the comparison functions.  This is generally sensible.

template <typename T1, typename T2> inline bool AlmostEqual2sComplement (T1 a, T2 b, int maxUlps=1);

template <> inline bool AlmostEqual2sComplement<float, float> (float a, float b, int maxUlps)
{return FloatAlmostEqual2sComplement(a,b,maxUlps);}

template <> inline bool AlmostEqual2sComplement<double, double> (double a, double b, int maxUlps)
{return DoubleAlmostEqual2sComplement(a,b,maxUlps);}

template <> inline bool AlmostEqual2sComplement<float, double> (float a, double b, int maxUlps)
{return FloatAlmostEqual2sComplement(a,(float)b,maxUlps);}

template <> inline bool AlmostEqual2sComplement<double, float> (double a, float b, int maxUlps)
{return FloatAlmostEqual2sComplement((float)a,b,maxUlps);}

// Description:
// Returns true if the arguments are within 1 ULP of each other.
template <typename T1, typename T2> inline bool EssentiallyEqual (T1 a, T2 b)
{return AlmostEqual2sComplement<T1,T2>(a,b,1);}

// Description:
// Returns true if the arguments are within APPROXIMATELY_EQUAL_ULPS of each other.
template <typename T1, typename T2> inline bool ApproximatelyEqual (T1 a, T2 b)
{return AlmostEqual2sComplement<T1,T2>(a,b,APPROXIMATELY_EQUAL_ULPS);}

// Description:
// Checks that the comparison functions work as expected.
// I STRONGLY recommend that you call this somewhere in your code before using
// and of the comparison functions.
void SanityCheck ()
{
  // --------------------------------------------------------------------------
  // Floats
  
  // Trival check
  assert(!FloatAlmostEqual2sComplement(1.0f,-1.0f,256));
  assert(!FloatAlmostEqual2sComplement(-1.0f,1.0f,256));

  // Positive and Negative zero have different representations, but should
  // compare as equal.
  assert(FloatAlmostEqual2sComplement(0.0f,-0.0f));
  assert(FloatAlmostEqual2sComplement(-0.0f,0.0f));

  // Denormalized minimum value of float should equal 1 when reinterpreted
  // as an int.
  assert(FloatAlmostEqual2sComplement(0.0f,std::numeric_limits<float>::denorm_min()));
  assert(FloatAlmostEqual2sComplement(std::numeric_limits<float>::denorm_min(),0.0f));
  assert(FloatAlmostEqual2sComplement(-0.0f,std::numeric_limits<float>::denorm_min()));
  assert(FloatAlmostEqual2sComplement(std::numeric_limits<float>::denorm_min(),-0.0f));
  assert(FloatAlmostEqual2sComplement(0.0f,-std::numeric_limits<float>::denorm_min()));
  assert(FloatAlmostEqual2sComplement(-std::numeric_limits<float>::denorm_min(),0.0f));
  assert(FloatAlmostEqual2sComplement(-0.0f,-std::numeric_limits<float>::denorm_min()));
  assert(FloatAlmostEqual2sComplement(-std::numeric_limits<float>::denorm_min(),-0.0f));

  // Normalized minimum value of float should be a rather large number when
  // reinterpreted as an int.
  assert(!FloatAlmostEqual2sComplement(0.0f,std::numeric_limits<float>::min()));
  assert(!FloatAlmostEqual2sComplement(std::numeric_limits<float>::min(),0.0f));
  assert(!FloatAlmostEqual2sComplement(-0.0f,std::numeric_limits<float>::min()));
  assert(!FloatAlmostEqual2sComplement(std::numeric_limits<float>::min(),-0.0f));
  assert(!FloatAlmostEqual2sComplement(0.0f,-std::numeric_limits<float>::min()));
  assert(!FloatAlmostEqual2sComplement(-std::numeric_limits<float>::min(),0.0f));
  assert(!FloatAlmostEqual2sComplement(-0.0f,-std::numeric_limits<float>::min()));
  assert(!FloatAlmostEqual2sComplement(-std::numeric_limits<float>::min(),-0.0f));

  // Expected accuracies.
  float lessThanFloatPrecision = PrecisionOfFloatUlps() / 3;
  float moreThanFloatPrecision = PrecisionOfFloatUlps() * 2;
  assert(FloatAlmostEqual2sComplement(1.0f,1.0f+lessThanFloatPrecision));
  assert(FloatAlmostEqual2sComplement(1.0f+lessThanFloatPrecision,1.0f));
  assert(!FloatAlmostEqual2sComplement(1.0f,1.0f+moreThanFloatPrecision));
  assert(!FloatAlmostEqual2sComplement(1.0f+moreThanFloatPrecision,1.0f));
  assert(FloatAlmostEqual2sComplement(-1.0f,-1.0f+lessThanFloatPrecision));
  assert(FloatAlmostEqual2sComplement(-1.0f+lessThanFloatPrecision,-1.0f));
  assert(!FloatAlmostEqual2sComplement(-1.0f,-1.0f+moreThanFloatPrecision));
  assert(!FloatAlmostEqual2sComplement(-1.0f+moreThanFloatPrecision,-1.0f));

  // --------------------------------------------------------------------------
  // Doubles
  
  // Trival check
  assert(!DoubleAlmostEqual2sComplement(1.0,-1.0,256));
  assert(!DoubleAlmostEqual2sComplement(-1.0,1.0,256));

  // Positive and Negative zero have different representations, but should
  // compare as equal.
  assert(DoubleAlmostEqual2sComplement(0.0,-0.0));
  assert(DoubleAlmostEqual2sComplement(-0.0,0.0));

  // Denormalized minimum value of double should equal 1 when reinterpreted
  // as an int.
  assert(DoubleAlmostEqual2sComplement(0.0,std::numeric_limits<double>::denorm_min()));
  assert(DoubleAlmostEqual2sComplement(std::numeric_limits<double>::denorm_min(),0.0));
  assert(DoubleAlmostEqual2sComplement(-0.0,std::numeric_limits<double>::denorm_min()));
  assert(DoubleAlmostEqual2sComplement(std::numeric_limits<double>::denorm_min(),-0.0));
  assert(DoubleAlmostEqual2sComplement(0.0,-std::numeric_limits<double>::denorm_min()));
  assert(DoubleAlmostEqual2sComplement(-std::numeric_limits<double>::denorm_min(),0.0));
  assert(DoubleAlmostEqual2sComplement(-0.0,-std::numeric_limits<double>::denorm_min()));
  assert(DoubleAlmostEqual2sComplement(-std::numeric_limits<double>::denorm_min(),-0.0));

  // Normalized minimum value of double should be a rather large number when
  // reinterpreted as an int.
  assert(!DoubleAlmostEqual2sComplement(0.0,std::numeric_limits<double>::min()));
  assert(!DoubleAlmostEqual2sComplement(std::numeric_limits<double>::min(),0.0));
  assert(!DoubleAlmostEqual2sComplement(-0.0,std::numeric_limits<double>::min()));
  assert(!DoubleAlmostEqual2sComplement(std::numeric_limits<double>::min(),-0.0));
  assert(!DoubleAlmostEqual2sComplement(0.0,-std::numeric_limits<double>::min()));
  assert(!DoubleAlmostEqual2sComplement(-std::numeric_limits<double>::min(),0.0));
  assert(!DoubleAlmostEqual2sComplement(-0.0,-std::numeric_limits<double>::min()));
  assert(!DoubleAlmostEqual2sComplement(-std::numeric_limits<double>::min(),-0.0));

  // Expected accuracies.
  double lessThanDoublePrecision = PrecisionOfDoubleUlps() / 3;
  double moreThanDoublePrecision = PrecisionOfDoubleUlps() * 2;
  assert(DoubleAlmostEqual2sComplement(1.0,1.0+lessThanDoublePrecision));
  assert(DoubleAlmostEqual2sComplement(1.0+lessThanDoublePrecision,1.0));
  assert(!DoubleAlmostEqual2sComplement(1.0,1.0+moreThanDoublePrecision));
  assert(!DoubleAlmostEqual2sComplement(1.0+moreThanDoublePrecision,1.0));
  assert(DoubleAlmostEqual2sComplement(-1.0,-1.0+lessThanDoublePrecision));
  assert(DoubleAlmostEqual2sComplement(-1.0+lessThanDoublePrecision,-1.0));
  assert(!DoubleAlmostEqual2sComplement(-1.0,-1.0+moreThanDoublePrecision));
  assert(!DoubleAlmostEqual2sComplement(-1.0+moreThanDoublePrecision,-1.0));

  // -------------------------------------------------------------------------
  // EssentiallyEqual and Mixed Floats and Doubles

  // Mixes Zeros (positive and negative)
  assert(EssentiallyEqual(0.0f,0.0));
  assert(EssentiallyEqual(0.0,0.0f));
  assert(EssentiallyEqual(-0.0f,0.0));
  assert(EssentiallyEqual(0.0,-0.0f));
  assert(EssentiallyEqual(0.0f,-0.0));
  assert(EssentiallyEqual(-0.0,0.0f));
  assert(EssentiallyEqual(-0.0f,-0.0));
  assert(EssentiallyEqual(-0.0,-0.0f));

  // Mixed Ones
  assert(EssentiallyEqual(1.0f,1.0));
  assert(EssentiallyEqual(1.0,1.0f));
  assert(EssentiallyEqual(-1.0f,-1.0));
  assert(EssentiallyEqual(-1.0,-1.0f));

  // Expected accuracies should be the same as float, not double
  assert(EssentiallyEqual(1.0f,1.0+lessThanFloatPrecision));
  assert(EssentiallyEqual(1.0+lessThanFloatPrecision,1.0f));
  assert(!EssentiallyEqual(1.0f,1.0+moreThanFloatPrecision));
  assert(!EssentiallyEqual(1.0+moreThanFloatPrecision,1.0f));

  // Finally verify that EssentiallyEqual does give us double accuracies
  // for pure double inputs.
  assert(!EssentiallyEqual(1.0,1.0+moreThanDoublePrecision));
  assert(!EssentiallyEqual(1.0+moreThanDoublePrecision,1.0));

  // -------------------------------------------------------------------------
  // ApproximatelyEqual

  // Float - More tolerant than EssentiallyEqual
  assert(ApproximatelyEqual(1.0f,1.0f+moreThanFloatPrecision));
  assert(ApproximatelyEqual(1.0f+moreThanFloatPrecision,1.0f));

  // Float - Outside specified ULP range for ApproximatelyEqual
  assert(!ApproximatelyEqual(1.0f,1.0f+APPROXIMATELY_EQUAL_ULPS*moreThanFloatPrecision));
  assert(!ApproximatelyEqual(1.0f+APPROXIMATELY_EQUAL_ULPS*moreThanFloatPrecision,1.0f));

  // Double - More tolerant than EssentiallyEqual
  assert(ApproximatelyEqual(1.0,1.0+moreThanDoublePrecision));
  assert(ApproximatelyEqual(1.0+moreThanDoublePrecision,1.0));

  // Double - Outside specified ULP range for ApproximatelyEqual
  assert(!ApproximatelyEqual(1.0,1.0+APPROXIMATELY_EQUAL_ULPS*moreThanDoublePrecision));
  assert(!ApproximatelyEqual(1.0+APPROXIMATELY_EQUAL_ULPS*moreThanDoublePrecision,1.0));

  // -------------------------------------------------------------------------
  // Infinities and NaNs

  // Float infinities
  if (std::numeric_limits<float>::has_infinity)
    {
    assert(!FloatAlmostEqual2sComplement(0.0f,std::numeric_limits<float>::infinity()));
    assert(!FloatAlmostEqual2sComplement(std::numeric_limits<float>::infinity(), 0.0f));
    assert(!FloatAlmostEqual2sComplement(0.0f,-std::numeric_limits<float>::infinity()));
    assert(!FloatAlmostEqual2sComplement(-std::numeric_limits<float>::infinity(), 0.0f));
    assert(!FloatAlmostEqual2sComplement(std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity()));
    assert(!FloatAlmostEqual2sComplement(-std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()));
    assert(FloatAlmostEqual2sComplement(std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity()));
    assert(FloatAlmostEqual2sComplement(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()));
    }

  // Double infinities
  if (std::numeric_limits<double>::has_infinity)
    {
    assert(!DoubleAlmostEqual2sComplement(0.0,std::numeric_limits<double>::infinity()));
    assert(!DoubleAlmostEqual2sComplement(std::numeric_limits<double>::infinity(), 0.0));
    assert(!DoubleAlmostEqual2sComplement(0.0,-std::numeric_limits<double>::infinity()));
    assert(!DoubleAlmostEqual2sComplement(-std::numeric_limits<double>::infinity(), 0.0));
    assert(!DoubleAlmostEqual2sComplement(std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity()));
    assert(!DoubleAlmostEqual2sComplement(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()));
    assert(DoubleAlmostEqual2sComplement(std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity()));
    assert(DoubleAlmostEqual2sComplement(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()));
    }

  // float NaNs
  if (std::numeric_limits<float>::has_quiet_NaN)
    {
    assert(!FloatAlmostEqual2sComplement(0.0f,std::numeric_limits<float>::quiet_NaN()));
    assert(!FloatAlmostEqual2sComplement(std::numeric_limits<float>::quiet_NaN(),0.0f));
    assert(!FloatAlmostEqual2sComplement(1.0f,std::numeric_limits<float>::quiet_NaN()));
    assert(!FloatAlmostEqual2sComplement(std::numeric_limits<float>::quiet_NaN(),1.0f));
    assert(!FloatAlmostEqual2sComplement(std::numeric_limits<float>::infinity(),std::numeric_limits<float>::quiet_NaN()));
    assert(!FloatAlmostEqual2sComplement(std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::infinity()));
    assert(!FloatAlmostEqual2sComplement(-std::numeric_limits<float>::infinity(),std::numeric_limits<float>::quiet_NaN()));
    assert(!FloatAlmostEqual2sComplement(std::numeric_limits<float>::quiet_NaN(),-std::numeric_limits<float>::infinity()));
    // Note that according to the standard the correct answer to NaN == NaN is false
    assert(!FloatAlmostEqual2sComplement(std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::quiet_NaN()));
    }

  // Double NaNs
  if (std::numeric_limits<double>::has_quiet_NaN)
    {
    assert(!DoubleAlmostEqual2sComplement(0.0,std::numeric_limits<double>::quiet_NaN()));
    assert(!DoubleAlmostEqual2sComplement(std::numeric_limits<double>::quiet_NaN(),0.0));
    assert(!DoubleAlmostEqual2sComplement(1.0,std::numeric_limits<double>::quiet_NaN()));
    assert(!DoubleAlmostEqual2sComplement(std::numeric_limits<double>::quiet_NaN(),1.0));
    assert(!DoubleAlmostEqual2sComplement(std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()));
    assert(!DoubleAlmostEqual2sComplement(std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::infinity()));
    assert(!DoubleAlmostEqual2sComplement(-std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()));
    assert(!DoubleAlmostEqual2sComplement(std::numeric_limits<double>::quiet_NaN(),-std::numeric_limits<double>::infinity()));
    // Note that according to the standard the correct answer to NaN == NaN is false
    assert(!DoubleAlmostEqual2sComplement(std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::quiet_NaN()));
    }

}

}  // namespace
