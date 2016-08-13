// Copyright (c) Eric Nodwell
// See LICENSE for details.

#include "n88util/text.hpp"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

namespace n88util
{

  //-----------------------------------------------------------------------
  void split_arguments(const std::string& s,
                       std::vector<std::string>& tokens,
                       const char* separators)
    {
    if (separators == NULL)
      {
      separators = " \t,";
      }
    boost::char_separator<char> sep(separators);
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer_t;
    tokens.clear();
    tokenizer_t tok(s, sep);
    for (tokenizer_t::iterator t = tok.begin(); t != tok.end(); ++t)
      {
      tokens.push_back(*t);
      }
    }

  //-----------------------------------------------------------------------
  void split_trim(const std::string& s,
                  std::vector<std::string>& tokens,
                  const char* separators)
    {
    if (separators == NULL)
      {
      separators = ",";
      }
    typedef boost::char_separator<char> sep_t;
    sep_t sep(separators, "", boost::keep_empty_tokens);
    typedef boost::tokenizer<sep_t> tokenizer_t;
    tokens.clear();
    tokenizer_t tok(s, sep);
    for (tokenizer_t::iterator t = tok.begin(); t != tok.end(); ++t)
      {
      tokens.push_back(boost::algorithm::trim_copy(*t));
      }
    }

}   // namespace n88util
