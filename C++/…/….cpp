#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <inttypes.h>
#include <iso646.h>
#include <memory>
#include <new>
#include <stdint.h>

/* Main */
int main(int, char*[]) /* noexcept */ {
  std::srand(std::time(NULL));
  std::size_t const TARGET = 30000u;
  std::div_t        rate   = {0, static_cast<int>(std::rand() % TARGET)}; rate.quot = rate.rem + (std::rand() % TARGET);
  // std::div_t  const rate   = {7, 3};
  std::size_t       value  = rate.rem;

  std::printf("%zu -> %zu (%i/%i)" "\r\n", value, TARGET, rate.quot, rate.rem);
  for (std::size_t prevalue = 0u; TARGET > value /*and prevalue != value*/; ) {
    prevalue = value;

    if (false) (void) prevalue;
    else if (value < static_cast<std::size_t>(USHRT_MAX / rate.quot))            { std::printf("[A]: "); value = (value * rate.quot) / rate.rem; }
    else if (value < static_cast<std::size_t>(USHRT_MAX / rate.quot) * rate.rem) { std::printf("[B]: "); value = (value / rate.rem) * rate.quot; }
    else if (value <= static_cast<std::size_t>(USHRT_MAX - rate.quot)) {
      std::printf("[C]: ");
      if (std::size_t const _ = ((TARGET - value) / rate.quot) * rate.quot) value += _;
      else                                                                  value += rate.quot;
    }
    else continue;
    std::printf("%zu -> %zu" "\r\n", value, TARGET);
  }

  if (value > TARGET) {
    std::printf("[…]: %zu" "\r\n", value -= (((value - TARGET) / rate.quot) * rate.quot));
    std::printf("[…]: %zu" "\r\n", value -= (((value - TARGET) / rate.quot) * rate.quot));
  }
}
