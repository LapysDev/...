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
  std::size_t const extent  = 69u;
  std::size_t const maximum = SIZE_MAX;
  std::div_t  const rate    = {3, 2};

  /* ... */
  for (std::size_t amount = rate.quot <= rate.rem ? extent : rate.rem, preamount = 0u; amount != preamount; ) {
    (void) std::printf("%zu, pre: %zu" "\r\n", amount, preamount);

    preamount = amount;      // ->> Avoid non-growing `rate`s
    amount   += amount % 2u; // ->> Ensure `amount` (’s parity) is arbitrarily even

    if (amount < extent) /* ... --> amount ·= rate */ {
      (void) std::printf("[+]" "\r\n");

      if      (amount < (maximum / rate.quot))            amount = (amount * rate.quot) / rate.rem;
      else if (amount < (maximum / rate.quot) * rate.rem) amount = (amount / rate.rem) * rate.quot;
      else if (amount <= maximum - rate.quot) {
        if (std::size_t const _ = (extent - amount) / rate.quot) amount += rate.quot * _;
        else                                                     amount += rate.quot;
      }
    } else { (void) std::printf("[-]" "\r\n"); amount -= ((amount - extent) / rate.quot) * rate.quot; } // ->> Constrain `amount >= extent`
  }
}
