// Copyright (c) Eric Nodwell
// See LICENSE for details.

#ifndef N88UTIL_printarray_hpp_INCLUDED
#define N88UTIL_printarray_hpp_INCLUDED


#include "const_array.hpp"
#include <iostream>


namespace n88
{

  template <typename TValue, typename TIndex>
  void printarray (std::ostream& s, const_array<1,TValue,TIndex> A)
  {
    for (size_t i = 0; i < A.size(); ++i)
    {
      s << i << "\t" << A[i] << "\n";
    }
  }

  template <typename TValue, typename TIndex>
  void printarray (std::ostream& s, array<1,TValue,TIndex> A)
  {
    for (size_t i = 0; i < A.size(); ++i)
    {
      s << i << "\t" << A[i] << "\n";
    }
  }
  
  template <typename TValue, typename TIndex>
  void printarray (std::ostream& s, const_array<2,TValue,TIndex> A)
  {
    for (size_t i = 0; i < A.dims()[0]; ++i)
      for (size_t j = 0; j < A.dims()[1]; ++j)
      {
        s << i << "," << j << "\t" << A(i,j) << "\n";
      }
  }

  template <typename TValue, typename TIndex>
  void printarray (std::ostream& s, array<2,TValue,TIndex> A)
  {
    for (size_t i = 0; i < A.dims()[0]; ++i)
      for (size_t j = 0; j < A.dims()[1]; ++j)
      {
        s << i << "," << j << "\t" << A(i,j) << "\n";
      }
  }
  
  template <typename TValue, typename TIndex>
  void printarray (std::ostream& s, const_array<3,TValue,TIndex> A)
  {
    for (size_t i = 0; i < A.dims()[0]; ++i)
      for (size_t j = 0; j < A.dims()[1]; ++j)
        for (size_t k = 0; k < A.dims()[2]; ++k)
        {
          s << i << "," << j << "," << k << "\t" << A(i,j,k) << "\n";
        }
  }

  template <typename TValue, typename TIndex>
  void printarray (std::ostream& s, array<3,TValue,TIndex> A)
  {
    for (size_t i = 0; i < A.dims()[0]; ++i)
      for (size_t j = 0; j < A.dims()[1]; ++j)
        for (size_t k = 0; k < A.dims()[2]; ++k)
        {
          s << i << "," << j << "," << k << "\t" << A(i,j,k) << "\n";
        }
  }

}

#endif
