#include <limits.h>
#include <stdio.h>

/* … */
struct params {
  size_t max;
  char  *message;
};

bool printp(struct params parameters) {
  return INT_MAX < parameters.max ? EOF != puts(parameters.message) : printf("%1.*s", (int) parameters.max, parameters.message) > 0;
}

#define printp(...) { _Pragma("GCC diagnostic push"); _Pragma("GCC diagnostic ignored \"-Woverride-init\""); printp((struct params) { .max = UINT_MAX, .message = "", __VA_ARGS__ }); _Pragma("GCC diagnostic pop"); }

/* Main */
int main(int count, char* arguments[]) {
  (void) arguments;
  (void) count;

  printp(.message = "Hello, World!");
}
