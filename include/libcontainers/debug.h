#ifndef DEBUG_H_
# define DEBUG_H_

# ifdef DEBUG
#  include <stdarg.h>
void print_debug(char const *);
void printf_debug(char const *str, ...);
# else
#  define print_debug(s) ((void)0)
#  define printf_debug(args...) ((void)0) 
# endif

#endif