// Copyright (c) Eric Nodwell
// See LICENSE for details.

#ifndef N88UTIL_vector_hpp_INCLUDED
#define N88UTIL_vector_hpp_INCLUDED

#include "exception.hpp"
#include <vector>
#include <ostream>
#include <iostream>
#include <cmath>

namespace n88
{

  /** A base class for tuplet.
    *
    * The purpose of this base class is to provide a bunch of common methods
    * that can be inherited in partial specializations of tuplet.  Please
    * refer to tuplet for more details.
    */
  template <int N, typename T>
  class tupletBase
  {

    protected:

      T m_Data[N];

    public:

      typedef T value_type;

      /** Assignment operator : from a tuplet. */
      template <typename T2>
      inline tupletBase operator=(const tupletBase<N,T2>& rhs)
      {
        for (int i=0; i<N; ++i) {this->m_Data[i] = rhs[i];}
        return *this;
      }

      /** Assignment operator : from a pointer. */
      inline tupletBase operator=(const T* const p)
      {
        for (int i=0; i<N; ++i) {this->m_Data[i] = p[i];}
        return *this;
      }

      /** Assignment operator : from a scalar.
        * Note that every element of the tuple will assume the scalar value.
        */
      inline tupletBase operator=(T x)
      {
        for (int i=0; i<N; ++i) {this->m_Data[i] = x;}
        return *this;
      }

      /** Returns element i. */
      inline const T& operator[](size_t i) const
      {
#ifdef RANGE_CHECKING
        if (i >= N) {
          throw_n88_exception("tuplet index out of bounds"); }
#endif
        return this->m_Data[i];
      }
      inline T& operator[](size_t i)
      {
#ifdef RANGE_CHECKING
        if (i >= N) {
          throw_n88_exception("tuplet index out of bounds"); }
#endif
        return this->m_Data[i];
      }

      /** Returns a pointer to the data. */
      inline const T* data() const {
        return this->m_Data; }
      inline T* data() {
        return this->m_Data; }

      /** Pointer to the last element plus one of the data.  This
        * may be used in loops various STL algorithms that require an end
        * value.
        */
      inline const T* end() const {
        return this->m_Data + N;  }

      /** Equality operator : to a tuplet */
      template <typename T2>
      inline bool operator==(const tupletBase<N,T2>& rhs) const
      {
        for (int i=0; i<N; ++i) {
          if (this->m_Data[i] != rhs[i]) {return false;}}
        return true;
      }

      /** Inequality operator : to a tuplet */
      template <typename T2>
      inline bool operator!=(const tupletBase<N,T2>& rhs) const
      {
        for (int i=0; i<N; ++i) {
          if (this->m_Data[i] != rhs[i]) {return true;}}
        return false;
      }

      /** Negation operator */
      inline tupletBase<N,T> operator-() const
      {
        tupletBase<N,T> x;
        for (int i=0; i<N; ++i) {x[i] = -this->m_Data[i];}
        return x;
      }

    protected:

      /* NOTE: Constructors are protected, so that tupletBase cannot be created
       *       directly; they can only be created as the base class of a tuplet.
       */

      /** Constructor.
        * No initialization of the data; assume random memory garbage.
        */
      tupletBase() {}

      /** Constructor that copies data from a pointer. */
      tupletBase(const T* p) {
        for (int i=0; i<N; ++i) {this->m_Data[i] = p[i];}}

      /** Constructor that copies data from a vector. */
      tupletBase(const std::vector<T>& v)
      {
        n88_assert(v.size() == N);
        for (int i=0; i<N; ++i) {this->m_Data[i] = v[i];}
      }

      /** Copy constructor that copies data from another tuplet.
        *
        * Potentially dangerous, as precision can be lost inadvertently.
        */
      template <typename T2>
      tupletBase(const tupletBase<N,T2>& t)
      {
        for (int i=0; i<N; ++i) {this->m_Data[i] = t[i];}
      }

  }; // class tupletBase <int N, typename T>


  /** A class for fixed-length tuples (vectors) of simple types.
    *
    * tuplets are intended to be a replacement for C static arrays, e.g.
    *    float x[3];
    *
    * tuplets have no additional storage requirements compared with static arrays
    * and are equally fast to access.  Unlike C static arrays, the length
    * can be checked at compile time in places it is used.
    *
    * Additionally, for debugging purposes, range checking is performed if
    * RANGE_CHECKING is defined at compile time.
    *
    * It can be faster to pass small (e.g. size 2, 3 or 4) tuplets on the stack
    * by value than it is to pass them by reference.
    */
  template <int N, typename T>
  class tuplet : public tupletBase<N,T>
  {
    public:

      /** Constructor.
        * No initialization of the data; assume random memory garbage.
        */
      tuplet () {}

      /** Constructor that copies data from a pointer. */
      tuplet(const T* p) : tupletBase<N,T>(p) {}

      /** Constructor that copies data from a vector. */
      tuplet(const std::vector<T>& v) : tupletBase<N,T>(v) {}

      /** Copy constructor that copies data from another tuplet.
        *
        * Potentially dangerous, as precision can be lost inadvertently.
        */
      template <typename T2>
      tuplet(const tupletBase<N,T2>& t) : tupletBase<N,T>(t) {}

      /** Returns a tuplet filled with zeros. */
      inline static tuplet<N,T> zeros()
      {
        tuplet<N,T> t;
        for (int i=0; i<N; ++i) {t[i] = 0;}
        return t;
      }

      /** Returns a tuplet filled with ones. */
      inline static tuplet<N,T> ones()
      {
        tuplet<N,T> t;
        for (int i=0; i<N; ++i) {t[i] = 1;}
        return t;
      }

  }; // class tuplet <int N, typename T>

  template <typename T>
  class tuplet<1,T> : public tupletBase<1,T>
  {
    public:
  
      tuplet() {}
  
      tuplet(const T* p) : tupletBase<1,T>(p) {}
  
      tuplet(const std::vector<T>& v) : tupletBase<1,T>(v) {}
  
      template <typename T2>
      tuplet(const tupletBase<1,T2>& t) : tupletBase<1,T>(t) {}
  
      tuplet(T x0)
      {
        this->m_Data[0] = x0;
      }
  
      inline static tuplet<1,T> zeros()
      {
        tuplet<1,T> t;
        t[0] = 0;
        return t;
      }
  
      inline static tuplet<1,T> ones()
      {
        tuplet<1,T> t;
        t[0] = 1;
        return t;
      }
  
  }; // class tuplet <1, typename T>
  
  template <typename T>
  class tuplet<2,T> : public tupletBase<2,T>
  {
    public:
  
      tuplet() {}
  
      tuplet(const T* p) : tupletBase<2,T>(p) {}
  
      tuplet(const std::vector<T>& v) : tupletBase<2,T>(v) {}
  
      template <typename T2>
      tuplet(const tupletBase<2,T2>& t) : tupletBase<2,T>(t) {}
  
      tuplet(T x0, T x1)
      {
        this->m_Data[0] = x0;
        this->m_Data[1] = x1;
      }
  
      inline static tuplet<2,T> zeros()
      {
        tuplet<2,T> t;
        t[0] = 0;
        t[1] = 0;
        return t;
      }
  
      inline static tuplet<2,T> ones()
      {
        tuplet<2,T> t;
        t[0] = 1;
        t[1] = 1;
        return t;
      }
  
  }; // class tuplet <2, typename T>
  
  template <typename T>
  class tuplet<3,T> : public tupletBase<3,T>
  {
    public:
  
      tuplet() {}
  
      tuplet(const T* p) : tupletBase<3,T>(p) {}
  
      template <typename T2>
      tuplet(const tupletBase<3,T2>& t) : tupletBase<3,T>(t) {}
  
      tuplet(T x0, T x1, T x2)
      {
        this->m_Data[0] = x0;
        this->m_Data[1] = x1;
        this->m_Data[2] = x2;
      }
  
      tuplet(const std::vector<T>& v) : tupletBase<3,T>(v) {}
  
      inline static tuplet<3,T> zeros()
      {
        tuplet<3,T> t;
        t[0] = 0;
        t[1] = 0;
        t[2] = 0;
        return t;
      }
  
      inline static tuplet<3,T> ones()
      {
        tuplet<3,T> t;
        t[0] = 1;
        t[1] = 1;
        t[2] = 1;
        return t;
      }
  
  }; // class tuplet <3, typename T>
  
  template <typename T>
  class tuplet<4,T> : public tupletBase<4,T>
  {
    public:
  
      tuplet() {}
  
      tuplet(const T* p) : tupletBase<4,T>(p) {}
  
      tuplet(const std::vector<T>& v) : tupletBase<4,T>(v) {}
  
      template <typename T2>
      tuplet(const tupletBase<4,T2>& t) : tupletBase<4,T>(t) {}
  
      tuplet(T x0, T x1, T x2, T x3)
      {
        this->m_Data[0] = x0;
        this->m_Data[1] = x1;
        this->m_Data[2] = x2;
        this->m_Data[3] = x3;
      }
  
      inline static tuplet<4,T> zeros()
      {
        tuplet<4,T> t;
        t[0] = 0;
        t[1] = 0;
        t[2] = 0;
        t[3] = 0;
        return t;
      }
  
      inline static tuplet<4,T> ones()
      {
        tuplet<4,T> t;
        t[0] = 1;
        t[1] = 1;
        t[2] = 1;
        t[3] = 1;
        return t;
      }
  
  }; // class tuplet <4, typename T>

  /** Returns the product of all elements of tuplet. */
  template <int N, typename T> inline T product(const tuplet<N,T>& x)
  {
    T p = x[0];
    for (int i=1; i<N; ++i) {p *= x[i];}
    return p;
  }

  /** Returns the product of all elements of tuplet as size_t.
    * The return value is size_t in order to avoid overflow
    * (e.g. as could happen if tuplet elements were of type int).
    */
  template <int N, typename T> inline size_t long_product(const tuplet<N,T>& x)
  {
    size_t p = x[0];
    for (size_t i=1; i<N; ++i) {p *= x[i];}
    return p;
  }

  /** Returns the sum of all elements of tuplet. */
  template <int N, typename T> inline T sum(const tuplet<N,T>& x)
  {
    T s = x[0];
    for (int i=1; i<N; ++i) {s += x[i];}
    return s;
  }

  /** Stream output operator.
    * tuplets are output in brackets, with individual elements separated
    * by commas.  For example, tuplet<3,int> might be output like this:
    * [4,24,1] .
    */
  template <int N, typename T>
  std::ostream& operator<<(std::ostream& s, const tuplet<N,T>& t)
  {
    s << "[" << t[0];
    for (int i=1; i<N; ++i) {s << "," << t[i];}
    s << "]";
    return s;
  }

  /** Stream input operator.
    * Converts a text representation of a tuplet to a tuplet.
    * The text representation may or may not include enclosing brackets,
    * which may be either round or square, but must match.
    * White space is ignored.
    * Elements must be separated by commas.
    * The input stream operator of the element type is used to parse the
    * elements.
    */
  template <int N, typename T>
  std::istream& operator>>(std::istream& s, tuplet<N,T>& t)
  {
    char c = s.get();
    if (s.fail() || s.bad() || s.eof())
    {
      s.setstate(std::ios::failbit);
      return s;
    }
    // Skip white space
    while (c == ' ')
    {
      c = s.get();
      if (s.fail() || s.bad() || s.eof())
      {
        s.setstate(std::ios::failbit);
        return s;
      }
    }
    // optional bracket
    char match_bracket = 0;
    if (c == '(') { match_bracket = ')'; }
    if (c == '[') { match_bracket = ']'; }
    if (match_bracket)
    {
      // Skip white space after bracket
      c = s.get();
      if (s.fail() || s.bad() || s.eof())
      {
        s.setstate(std::ios::failbit);
        return s;
      }
      while (c == ' ')
      {
        c = s.get();
        if (s.fail() || s.bad() || s.eof())
        {
          s.setstate(std::ios::failbit);
          return s;
        }
      }
    }

    // Loop over dimensions
    for (int i=0; i<N; ++i)
    {
      // We have picked off one too many characters, so put it back
      s.unget();
      s >> t[i];
      if (s.fail() || s.bad())
        {return s;}
      if (s.eof())
      {
        if (match_bracket || i != N-1)
          { s.setstate(std::ios::failbit); }
        return s;
      }
      // Skip white space
      c = s.get();
      if (s.fail() || s.bad())
        {return s;}
      if (s.eof())
      {
        if (match_bracket || i != N-1)
          { s.setstate(std::ios::failbit); }
        return s;
      }
      while (c == ' ')
      {
        c = s.get();
        if (s.fail() || s.bad())
          {return s;}
        if (s.eof())
        {
          if (match_bracket || i != N-1)
            { s.setstate(std::ios::failbit); }
          return s;
        }
      }
      // Optional comma
      if (c == ',' && i < N-1)
      {
        // Move past the comma
        c = s.get();
        if (s.fail() || s.bad())
          {return s;}
        if (s.eof())
        {
          if (match_bracket || i != N-1)
            { s.setstate(std::ios::failbit); }
          return s;
        }
      }
      // Skip white space again
      while (c == ' ')
      {
        c = s.get();
        if (s.fail() || s.bad())
          {return s;}
        if (s.eof())
        {
          if (match_bracket || i != N-1)
            { s.setstate(std::ios::failbit); }
          return s;
        }
      }
      // Note: at end of for loop have read next character already.
    }

    // If we don't need to match the bracket, will have already returned.
    n88_assert(match_bracket);

    // Skip white space
    while (c == ' ')
    {
      c = s.get();
      if (s.fail() || s.bad() || s.eof())
      {
        // No matching bracket found.
        s.setstate(std::ios::failbit);
        return s;
      }
    }
    if (c != match_bracket)
    {
      s.setstate(std::ios::failbit);
    }

    // Note: Correct behaviour seems to be to return with eof set if
    //       we used all the input stream, so read one more char to
    //       ensure this.
    s.peek();
    return s;
  }

  /** Returns the norm of a tuplet.
    * The norm is sqrt(sum_i(x_i^2)) .
    */
  template <int N, typename T> inline T norm(const tuplet<N,T>& a)
  {
    T x = a[0]*a[0];
    for (int i=1; i<N; ++i) {x += a[i]*a[i];}
    return sqrt(x);
  }
  template <typename T> inline T norm(const tuplet<1,T>& a)
  {
    return sqrt(a[0]*a[0]);
  }
  template <typename T> inline T norm(const tuplet<2,T>& a)
  {
    return sqrt(a[0]*a[0] + a[1]*a[1]);
  }
  template <typename T> inline T norm(const tuplet<3,T>& a)
  {
    return sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
  }

  /** Adds a scalar and a tuplet. */
  template <int N, typename T> inline tuplet<N,T> operator+
  (const tuplet<N,T>& a, T s)
  {
    tuplet<N,T> x;
    for (int i=0; i<N; ++i) {x[i] = a[i] + s;}
    return x;
  }
  template <typename T> inline tuplet<1,T> operator+
  (const tuplet<1,T>& a, T s)
  {
    return tuplet<1,T>(a[0]+s);
  }
  template <typename T> inline tuplet<2,T> operator+
  (const tuplet<2,T>& a, T s)
  {
    return tuplet<2,T>(a[0]+s,a[1]+s);
  }
  template <typename T> inline tuplet<3,T> operator+
  (const tuplet<3,T>& a, T s)
  {
    return tuplet<3,T>(a[0]+s,a[1]+s,a[2]+s);
  }

  /** Adds two tuplets. */
  template <int N, typename T> inline tuplet<N,T> operator+
  (const tuplet<N,T>& a, const tuplet<N,T>& b)
  {
    tuplet<N,T> x;
    for (int i=0; i<N; ++i) {x[i] = a[i] + b[i];}
    return x;
  }
  template <typename T> inline tuplet<1,T> operator+
  (const tuplet<1,T>& a, const tuplet<1,T>& b)
  {
    return tuplet<1,T>(a[0]+b[0]);
  }
  template <typename T> inline tuplet<2,T> operator+
  (const tuplet<2,T>& a, const tuplet<2,T>& b)
  {
    return tuplet<2,T>(a[0]+b[0],a[1]+b[1]);
  }
  template <typename T> inline tuplet<3,T> operator+
  (const tuplet<3,T>& a, const tuplet<3,T>& b)
  {
    return tuplet<3,T>(a[0]+b[0],a[1]+b[1],a[2]+b[2]);
  }

  /** Subtracts a scalar from a tuplet. */
  template <int N, typename T> inline tuplet<N,T> operator-
  (const tuplet<N,T>& a, T s)
  {
    tuplet<N,T> x;
    for (int i=0; i<N; ++i) {x[i] = a[i] - s;}
    return x;
  }
  template <typename T> inline tuplet<1,T> operator-
  (const tuplet<1,T>& a, T s)
  {
    return tuplet<1,T>(a[0]-s);
  }
  template <typename T> inline tuplet<2,T> operator-
  (const tuplet<2,T>& a, T s)
  {
    return tuplet<2,T>(a[0]-s,a[1]-s);
  }
  template <typename T> inline tuplet<3,T> operator-
  (const tuplet<3,T>& a, T s)
  {
    return tuplet<3,T>(a[0]-s,a[1]-s,a[2]-s);
  }

  /** Subtracts two tuplets. */
  template <int N, typename T> inline tuplet<N,T> operator-
  (const tuplet<N,T>& a, const tuplet<N,T>& b)
  {
    tuplet<N,T> x;
    for (int i=0; i<N; ++i) {x[i] = a[i] - b[i];}
    return x;
  }
  template <typename T> inline tuplet<1,T> operator-
  (const tuplet<1,T>& a, const tuplet<1,T>& b)
  {
    return tuplet<1,T>(a[0]-b[0]);
  }
  template <typename T> inline tuplet<2,T> operator-
  (const tuplet<2,T>& a, const tuplet<2,T>& b)
  {
    return tuplet<2,T>(a[0]-b[0],a[1]-b[1]);
  }
  template <typename T> inline tuplet<3,T> operator-
  (const tuplet<3,T>& a, const tuplet<3,T>& b)
  {
    return tuplet<3,T>(a[0]-b[0],a[1]-b[1],a[2]-b[2]);
  }

  /** Multiplies a scalar by a tuplet. */
  template <int N, typename T> inline tuplet<N,T> operator*
  (T x, const tuplet<N,T>& a)
  {
    tuplet<N,T> b;
    for (int i=0; i<N; ++i) {b[i] = x*a[i];}
    return b;
  }
  template <typename T> inline tuplet<1,T> operator*
  (T x, const tuplet<1,T>& a)
  {
    return tuplet<1,T>(x*a[0]);
  }
  template <typename T> inline tuplet<2,T> operator*
  (T x, const tuplet<2,T>& a)
  {
    return tuplet<2,T>(x*a[0],x*a[1]);
  }
  template <typename T> inline tuplet<3,T> operator*
  (T x, const tuplet<3,T>& a)
  {
    return tuplet<3,T>(x*a[0],x*a[1],x*a[2]);
  }

  /** Multiplies a tuplet by a scalar. */
  template <int N, typename T> inline tuplet<N,T> operator*
  (const tuplet<N,T>& a, T x)
  {
    tuplet<N,T> b;
    for (int i=0; i<N; ++i) {b[i] = x*a[i];}
    return b;
  }
  template <typename T> inline tuplet<1,T> operator*
  (const tuplet<1,T>& a, T x)
  {
    return tuplet<1,T>(x*a[0]);
  }
  template <typename T> inline tuplet<2,T> operator*
  (const tuplet<2,T>& a, T x)
  {
    return tuplet<2,T>(x*a[0],x*a[1]);
  }
  template <typename T> inline tuplet<3,T> operator*
  (const tuplet<3,T>& a, T x)
  {
    return tuplet<3,T>(x*a[0],x*a[1],x*a[2]);
  }

  /** Multiplies two tuplets. */
  template <int N, typename T> inline tuplet<N,T> operator*
  (const tuplet<N,T>& a, const tuplet<N,T>& b)
  {
    tuplet<N,T> c;
    for (int i=0; i<N; ++i) {c[i] = a[i] * b[i];}
    return c;
  }
  template <typename T> inline tuplet<1,T> operator*
  (const tuplet<1,T>& a, const tuplet<1,T>& b)
  {
    return tuplet<1,T>(a[0]*b[0]);
  }
  template <typename T> inline tuplet<2,T> operator*
  (const tuplet<2,T>& a, const tuplet<2,T>& b)
  {
    return tuplet<2,T>(a[0]*b[0],a[1]*b[1]);
  }
  template <typename T> inline tuplet<3,T> operator*
  (const tuplet<3,T>& a, const tuplet<3,T>& b)
  {
    return tuplet<3,T>(a[0]*b[0],a[1]*b[1],a[2]*b[2]);
  }

  /** Divides one tuplet by another. */
  template <int N, typename T> inline tuplet<N,T> operator/
  (const tuplet<N,T>& a, const tuplet<N,T>& b)
  {
    tuplet<N,T> c;
    for (int i=0; i<N; ++i) {c[i] = a[i] / b[i];}
    return c;
  }
  template <typename T> inline tuplet<1,T> operator/
  (const tuplet<1,T>& a, const tuplet<1,T>& b)
  {
    return tuplet<1,T>(a[0]/b[0]);
  }
  template <typename T> inline tuplet<2,T> operator/
  (const tuplet<2,T>& a, const tuplet<2,T>& b)
  {
    return tuplet<2,T>(a[0]/b[0],a[1]/b[1]);
  }
  template <typename T> inline tuplet<3,T> operator/
  (const tuplet<3,T>& a, const tuplet<3,T>& b)
  {
    return tuplet<3,T>(a[0]/b[0],a[1]/b[1],a[2]/b[2]);
  }

  /** Divides a tuplet by a scalar. */
  template <int N, typename T> inline tuplet<N,T> operator/
  (const tuplet<N,T>& a, T b)
  {
    tuplet<N,T> c;
    for (int i=0; i<N; ++i) {c[i] = a[i] / b;}
    return c;
  }
  template <typename T> inline tuplet<1,T> operator/
  (const tuplet<1,T>& a, T b)
  {
    return tuplet<1,T>(a[0]/b);
  }
  template <typename T> inline tuplet<2,T> operator/
  (const tuplet<2,T>& a, T b)
  {
    return tuplet<2,T>(a[0]/b,a[1]/b);
  }
  template <typename T> inline tuplet<3,T> operator/
  (const tuplet<3,T>& a, T b)
  {
    return tuplet<3,T>(a[0]/b,a[1]/b,a[2]/b);
  }

  /** Returns the dot product of a tuplet. */
  template <int N, typename T> inline T dot(const tuplet<N,T>& a, const tuplet<N,T>& b)
  {
    T x = a[0]*b[0];
    for (int i=1; i<N; ++i) {x += a[i]*b[i];}
    return x;
  }
  template <typename T> inline T dot(const tuplet<1,T>& a, const tuplet<1,T>& b)
  {
    return a[0]*b[0];
  }
  template <typename T> inline T dot(const tuplet<2,T>& a, const tuplet<2,T>& b)
  {
    return a[0]*b[0] + a[1]*b[1];
  }
  template <typename T> inline T dot(const tuplet<3,T>& a, const tuplet<3,T>& b)
  {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
  }

  /** Returns a tuplet which has the elements in the reverse order. */
  template <int N, typename T> inline tuplet<N,T> reverse(const tuplet<N,T>& a)
  {
    tuplet<N,T> b;
    for (int i=0; i<N; ++i) {b[N-1-i] = a[i];}
    return b;
  }
  template <typename T> inline tuplet<1,T> reverse(const tuplet<1,T>& a)
  {
    return a;
  }
  template <typename T> inline tuplet<2,T> reverse(const tuplet<2,T>& a)
  {
    return tuplet<2,T>(a[1],a[0]);
  }
  template <typename T> inline tuplet<3,T> reverse(const tuplet<3,T>& a)
  {
    return tuplet<3,T>(a[2],a[1],a[0]);
  }

} // namespace n88

#endif
