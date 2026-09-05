// /* 🐢 Common but slowest speeds */
// #include <climits>
// #include <cstdio>

// int main(void) {
//   for (unsigned short iterator = USHRT_MAX; iterator--; )
//   std::fprintf(stdout, "[]: %u" "\r\n", USHRT_MAX - iterator);
// }

/* 🐇 Faster but memory-intensive */
#include <climits>
#include <cstdio>
#include <cstdlib>

int main(void) {
  char *const buffer = static_cast<char*>(std::malloc(((USHRT_MAX * 11u) + 1u) * sizeof(char)));

  std::setbuf(stdout, buffer); // ->> Before other I/O
    for (unsigned short iterator = USHRT_MAX; iterator--; )
    std::fprintf(stdout, "[]: %u" "\r\n", USHRT_MAX - iterator);
  std::fflush(stdout);
  std::free(buffer);
}
