/*=========================================================================

                                binhex

  Encoding and decoding of binary data to hexadecimal.

  Copyright (c) Eric Nodwell
  See LICENSE for details.

=========================================================================*/


#ifndef __n88util_binhex_h
#define __n88util_binhex_h

#include <string>
#include "n88util_export.h"

namespace n88util
{
  /**
  \brief Encodes a binary buffer to a hexadecimal string.
  \param buf The input binary buffer.
  \param len The length of buf.
  \param hex The returned hexadecimal encoding (a null-terminated string).
  \return 1 on success; 0 otherwise.
  The user is responsible for freeing the memory pointed to by hex.
  If hex points to existing data before this call, that is a memory leak.
  */
  N88UTIL_EXPORT int bintohex(const unsigned char *buf, long len, char** hex);

  /**
  \brief Version that returns C++ string.
  No memory management is required.
  */
  N88UTIL_EXPORT int bintohex(const unsigned char *buf, long len, std::string& hex);

  /**
  \brief Decodes a hexadecimal string to a binary buffer.
  \param hex The input hexadecimal encoding (a null-terminated string).
  \param buf The returned binary buffer.
  \param len The length of buf.
  \return 1 on success; 0 otherwise.

  The user is responsible for freeing the memory pointed to by buf.
  If buf points to existing data before this call, that is a memory leak.
  */
  N88UTIL_EXPORT int hextobin(const char *hex, unsigned char** buf, long *len);

  /**
  \brief Version accepts C++ string as input.
  */
  N88UTIL_EXPORT int hextobin(const std::string& hex, unsigned char** buf, long *len);

}   // namespace n88util

#endif  // #ifndef __n88util_binhex_h
