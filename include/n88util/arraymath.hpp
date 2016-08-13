// Copyright (c) Eric Nodwell
// See LICENSE for details.

#ifndef N88UTIL_arraymath_hpp_INCLUDED
#define N88UTIL_arraymath_hpp_INCLUDED

#include "array.hpp"
#include <cmath>

// Note: This loop is vectorized by gcc 4.3.2 .
template <typename TValue, typename TIndex>
TValue sum (const n88::array<1,TValue,TIndex>& A)
{
  TValue s = 0;
  const TValue* a = A.data();
  const TValue* const end = A.end();
  while (a != end)
  {
    s += *a;
    ++a;
  }
  return s;
}


// Sometimes template resolution fails, then can use this name.
template <typename TValue, typename TIndex>
TValue array_sum (const n88::array<1,TValue,TIndex>& A)
{
  return sum (A);
}


// Note: This loop is probably fundamentally unvectorizable.
template <typename TValue, typename TIndex>
TValue max (const n88::array<1,TValue,TIndex>& A)
{
  const TValue* a = A.data();
  const TValue* const end = A.end();
  TValue m = *a;
  ++a;
  while (a != end)
  {
    if (m < *a) m = *a;
    ++a;
  }
  return m;
}


// Note: This loop is probably fundamentally unvectorizable.
template <typename TValue, typename TIndex>
TValue min (const n88::array<1,TValue,TIndex>& A)
{
  const TValue* a = A.data();
  const TValue* const end = A.end();
  TValue m = *a;
  ++a;
  while (a != end)
  {
    if (m > *a) m = *a;
    ++a;
  }
  return m;
}

// Note: This loop is probably fundamentally unvectorizable.
template <typename TValue, typename TIndex>
TValue maxabs (const n88::array<1,TValue,TIndex>& A)
{
  const TValue* a = A.data();
  const TValue* const end = A.end();
  TValue m = fabs(*a);
  ++a;
  while (a != end)
  {
    TValue t = fabs(*a);
    if (m < t) m = t;
    ++a;
  }
  return m;
}

#endif
