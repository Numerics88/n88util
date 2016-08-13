// Copyright (c) Eric Nodwell
// See LICENSE for details.

#ifndef N88UTIL_netcdf_templated_hpp_INCLUDED
#define N88UTIL_netcdf_templated_hpp_INCLUDED

#include <netcdf.h>

namespace n88util
{

  template <typename T> int nc_get_var (int ncid, int varid, T* p);
  template <typename T> int nc_get_var1 (int ncid, int varid, const size_t *indexp, T* p);
  template <typename T> int nc_get_vara (int ncid, int varid, const size_t *startp, const size_t *countp, T* p);
  template <typename T> int nc_put_var (int ncid, int varid, const T* p);

  template <> int nc_get_var<unsigned char> (int ncid, int varid, unsigned char* p)
  { return nc_get_var_uchar (ncid, varid, p); }
  template <> int nc_get_var<int> (int ncid, int varid, int* p)
  { return nc_get_var_int (ncid, varid, p); }
  template <> int nc_get_var<unsigned int> (int ncid, int varid, unsigned int* p)
  { return nc_get_var_uint (ncid, varid, p); }
#ifdef WIN32
  template <> int nc_get_var<unsigned long long> (int ncid, int varid, unsigned long long* p)
  {
    return nc_get_var_ulonglong (ncid, varid, (unsigned long long*)p);
  }
#else
  template <> int nc_get_var<unsigned long> (int ncid, int varid, unsigned long* p)
  {
    BOOST_STATIC_ASSERT(sizeof(unsigned long) == sizeof(unsigned long long));
    return nc_get_var_ulonglong (ncid, varid, (unsigned long long*)p);
  }
#endif
  template <> int nc_get_var<float> (int ncid, int varid, float* p)
  { return nc_get_var_float (ncid, varid, p); }
  template <> int nc_get_var<double> (int ncid, int varid, double* p)
  { return nc_get_var_double (ncid, varid, p); }

  template <> int nc_get_var1<int> (int ncid, int varid, const size_t *indexp, int* p)
  { return nc_get_var1_int (ncid, varid, indexp, p); }
  template <> int nc_get_var1<unsigned int> (int ncid, int varid, const size_t *indexp, unsigned int* p)
  { return nc_get_var1_uint (ncid, varid, indexp, p); }
  template <> int nc_get_var1<float> (int ncid, int varid, const size_t *indexp, float* p)
  { return nc_get_var1_float (ncid, varid, indexp, p); }
  template <> int nc_get_var1<double> (int ncid, int varid, const size_t *indexp, double* p)
  { return nc_get_var1_double (ncid, varid, indexp, p); }

  template <> int nc_get_vara<float> (int ncid, int varid, const size_t *startp, const size_t *countp, float* p)
  { return nc_get_vara_float (ncid, varid, startp, countp, p); }
  template <> int nc_get_vara<double> (int ncid, int varid, const size_t *startp, const size_t *countp, double* p)
  { return nc_get_vara_double (ncid, varid, startp, countp, p); }
  template <> int nc_get_vara<long> (int ncid, int varid, const size_t *startp, const size_t *countp, long* p)
  { return nc_get_vara_long (ncid, varid, startp, countp, p); }
#ifdef WIN32
  template <> int nc_get_vara<unsigned long> (int ncid, int varid, const size_t *startp, const size_t *countp, unsigned long* p)
  {
    BOOST_STATIC_ASSERT(sizeof(unsigned long) == sizeof(unsigned int));
    return nc_get_vara_uint (ncid, varid, startp, countp, (unsigned int*)p);
  }
#else
  template <> int nc_get_vara<unsigned long> (int ncid, int varid, const size_t *startp, const size_t *countp, unsigned long* p)
  {
    BOOST_STATIC_ASSERT(sizeof(unsigned long) == sizeof(unsigned long long));
    return nc_get_vara_ulonglong (ncid, varid, startp, countp, (unsigned long long*)p);
  }
#endif
  template <> int nc_get_vara<long long> (int ncid, int varid, const size_t *startp, const size_t *countp, long long* p)
  { return nc_get_vara_longlong (ncid, varid, startp, countp, p); }
  template <> int nc_get_vara<unsigned long long> (int ncid, int varid, const size_t *startp, const size_t *countp, unsigned long long* p)
  { return nc_get_vara_ulonglong (ncid, varid, startp, countp, p); }

  template <> int nc_put_var<unsigned char> (int ncid, int varid, const unsigned char* p)
  { return nc_put_var_uchar (ncid, varid, p); }
  template <> int nc_put_var<int> (int ncid, int varid, const int* p)
  { return nc_put_var_int (ncid, varid, p); }
  template <> int nc_put_var<unsigned int> (int ncid, int varid, const unsigned int* p)
  { return nc_put_var_uint (ncid, varid, p); }
  template <> int nc_put_var<float> (int ncid, int varid, const float* p)
  { return nc_put_var_float (ncid, varid, p); }
  template <> int nc_put_var<double> (int ncid, int varid, const double* p)
  { return nc_put_var_double (ncid, varid, p); }

}  // namespace n88util

#endif
