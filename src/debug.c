#ifdef DEBUG

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "libcontainers/debug.h"

void
print_debug(char const *s)
{
  fprintf(stderr, "libcontainers: %s\n", s);
}

void
printf_debug(char const *str, ...)
{
  va_list ap;
  char *s;

  if ((s = malloc(sizeof(*s) + strlen(str) + 2)) == NULL)
    {
      return;
    }
  strcpy(s, str);
  s[strlen(str)] = '\n';
  s[strlen(str) + 1] = 0;
  va_start(ap, str);
  vfprintf(stderr, s, ap);
  va_end(ap);
  free(s);
}

#else

typedef int make_gcc_happy;

#endif