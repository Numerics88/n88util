#include <limits.h>
#include <stdio.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

int main()
{
  int x = __GLIBC__;  // force compiler error if not exist
  printf ( TOSTRING(__GLIBC__) "." TOSTRING(__GLIBC_MINOR__));
  return 0;
}
