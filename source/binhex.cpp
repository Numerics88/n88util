// Copyright (c) Eric Nodwell
// See LICENSE for details.

#include "n88util/binhex.hpp"
#include <cstring>
#include <cstdio>
#include <cstdlib>

namespace n88util
{
  
  const char valid_hex_chars[] = "0123456789ABCDEF";
  
  //-----------------------------------------------------------------------
  int bintohex(const unsigned char *buf, long len, char** hex)
  {
    if (len < 0)
    {
      *hex = NULL;
      return 0;
    }
    *hex = (char*)malloc(2*len+1);
    if (!*hex)
    {
      return 0;
    }
    char* h = *hex;
    h[2*len] = '\0';
    char c[3];
    while (len--)
    {
  	  sprintf(c, "%02X", *buf++);
      strncpy(h, c, 2);
      h += 2;
    }
    return 1;
  }
  
  //-----------------------------------------------------------------------
  // This is not particularly efficient, because the output is written
  // to a temporary internal buffer and then copied.
  int bintohex(const unsigned char *buf, long len, std::string& hex)
  {
    hex.clear();
    char* tmp = NULL;
    if (!bintohex(buf,len,&tmp))
    {
      free(tmp);
      return 0;
    }
    hex = tmp;
    free(tmp);
    return 1;
  }

  //-----------------------------------------------------------------------
  int hextobin(const char *hex, unsigned char** buf, long *len)
  {
    *len = strlen(hex)/2;
    *buf = (unsigned char*)malloc(*len);
    if (!*buf)
    {
      *len = 0;
      return 0;
    }
    unsigned char* b = *buf;
    long l = *len;
    char c[3];
    int x;
    while (l--)
    {
      c[2] = c[1] = c[0] = '\0';
      strncpy(c, hex, 2);
      if (!strchr(valid_hex_chars, c[0]) || !strchr(valid_hex_chars, c[1]))
      {
        free(*buf);
        *buf = NULL;
        *len = 0;
        return 0;
      }
      if (sscanf(c, "%X", &x) != 1)
      {
        free(*buf);
        *buf = NULL;
        *len = 0;
        return 0;
      }
      *b++ = x;
      hex += 2;
    }
    return 1;
  }

  //-----------------------------------------------------------------------
  int hextobin(const std::string& hex, unsigned char** buf, long *len)
  {
    return hextobin(hex.c_str(), buf, len);
  }

}   // namespace n88util
