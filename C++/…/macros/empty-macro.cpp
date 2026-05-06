#include <cstdio>

/* ... */
#undef check
#undef is_empty_
#undef fail
#undef pass

#define concatenate(argument1, argument2) argument1 ## argument2
#define defer(function, ...) defer_parse(function, (__VA_ARGS__))
# define defer_parse(function, call) function call
#define second(argument1, argument2, ...) argument2

#define is_empty(macro)  defer(concatenate, is_empty_, defer(second, defer(concatenate, is_empty_, defer(concatenate, check, macro)), fail, ~))()
# define is_empty_check ~, pass
# define is_empty_fail() false
# define is_empty_pass() true

/* Main */
#define empty
#define nonempty A

int main() {
  std::puts(is_empty(empty)    ? "true" : "false");
  std::puts(is_empty(nonempty) ? "true" : "false");
}
