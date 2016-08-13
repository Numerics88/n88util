/*=========================================================================

                                text

  Assorted text utilities.

  Copyright (c) Eric Nodwell
  See LICENSE for details.

=========================================================================*/


#ifndef __n88util_text_h
#define __n88util_text_h

#include <vector>
#include <string>
#include "n88util_export.h"

namespace n88util
{

  /** \brief Splits a string into tokens.
   *
   * Repeated tokens are treated as one.
   */
  N88UTIL_EXPORT void split_arguments(const std::string& s,
                       std::vector<std::string>& tokens,
                       const char* separators = NULL);

  /** \brief Splits a string into tokens.
   *
   * Repeated tokens are NOT treated as one.  The resulting tokens are
   * trimmed of whitespace of both sides.
   */
  N88UTIL_EXPORT void split_trim(const std::string& s,
                  std::vector<std::string>& tokens,
                  const char* separators = NULL);

}   // namespace n88util

#endif  // #ifndef __n88util_text_h
