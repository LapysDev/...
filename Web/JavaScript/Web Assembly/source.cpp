/* CITE (Lapys) -> https://mbebenita.github.io/WasmExplorer/ */
/* CODE (Lapys) -> em++ source.cpp -s WASM=1 -s SIDE_MODULE=1 -o source.wasm */
#include <cstddef>
#include <ctime>
#include <stdint.h>

/* ... */
extern "C" {
  void* allocate(std::size_t const); // --> `std::launder(...)`ed?

  uintmax_t const* sort(uint32_t array[], std::size_t const length) {
    std::time_t const recentTimestamp = std::time(static_cast<std::time_t*>(NULL));
    uintmax_t       (&statistics)[2]  = *static_cast<uintmax_t (*)[2]>(allocate(sizeof(uintmax_t[2])));

    // ...
    for (uint32_t *iterator = array + length; array != iterator; )
    for (uint32_t *subiterator = iterator--; array != subiterator; ) {
      if (*iterator < *--subiterator) {
        *iterator    ^= *subiterator;
        *subiterator ^= *iterator;
        *iterator    ^= *subiterator;

        iterator    = array + length;
        subiterator = iterator--;
        ++*statistics;
      }
    }

    if (recentTimestamp != static_cast<std::time_t>(-1)) {
      std::time_t const currentTimestamp = std::time(static_cast<std::time_t*>(NULL));
      if (currentTimestamp != static_cast<std::time_t>(-1)) { *(statistics + 1) = static_cast<uintmax_t>(currentTimestamp - recentTimestamp); }
    }

    // ...
    return statistics;
  }
}
