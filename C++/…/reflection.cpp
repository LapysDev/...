#include <cstdio>
#include <meta>

/* ... */
template <std::meta::info info>
consteval auto foo() {
  return typename [:info:](42);
}

/* Main */
int main(int, char*[]) /* noexcept */ {
  (void) std::printf("%i", foo<^^int>());
}
