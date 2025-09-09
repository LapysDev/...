#include <cstddef>
#include <stdint.h>

/* ... */
extern "C" {
  // ... ->> Current system time in milliseconds
  std::size_t timestamp();

  // ... ->> Number of unit operations can be run in an elapsed time
  std::size_t benchmark(std::size_t const milliseconds) {
    std::size_t       count     = 0u;
    std::size_t const timestamp = ::timestamp();

    // ...
    while (milliseconds > ::timestamp() - timestamp)
    ++count;

    return count;
  }

  // ... ->> Compress redundant color information ie: group equally valued pixels and ignore fully transparent colors (`size` determines the bit width of `unsigned`) --> NULL == index || rate > (NULL == subindex ? 1u : 2u)
  std::size_t extrapolate(uint32_t values[], unsigned counts[], unsigned* const count, uint8_t const data[], std::size_t length, unsigned* index, unsigned* subindex, std::size_t rate, std::size_t const size) {
    if (0u == rate) {
      index    = NULL;
      subindex = NULL;
    }

    for (data += (length -= NULL != index ? 4u * (size == 32u ? *reinterpret_cast<uint32_t*>(index) : size == 64u ? *reinterpret_cast<uint64_t*>(index) : *index) : 0u); ; length -= 4u) {
      if (0u == length)
      return size == 32u ? *reinterpret_cast<uint32_t*>(count) : size == 64u ? *reinterpret_cast<uint64_t*>(count) : *count;

      // ... ->> Halt for next asynchronous call
      if (NULL != index && 0u == rate--) {
        std::size_t subcount = size == 32u ? *reinterpret_cast<uint32_t*>(count) : size == 64u ? *reinterpret_cast<uint64_t*>(count) : *count;

        // ...
        if (NULL != subindex) size == 32u ? *reinterpret_cast<uint32_t*>(subindex) = 0u : size == 64u ? *reinterpret_cast<uint64_t*>(subindex) = 0u : (*subindex = 0u);
        for (size == 32u ? *reinterpret_cast<uint32_t*>(index) = 0u : size == 64u ? *reinterpret_cast<uint64_t*>(index) = 0u : (*index = 0u); subcount--; counts = size == 32u ? reinterpret_cast<unsigned*>(reinterpret_cast<uint32_t*>(counts) + 1) : size == 64u ? reinterpret_cast<unsigned*>(reinterpret_cast<uint64_t*>(counts) + 1) : (counts + 1)) size == 32u ? *reinterpret_cast<uint32_t*>(index) += *reinterpret_cast<uint32_t*>(counts) : size == 64u ? *reinterpret_cast<uint64_t*>(index) += *reinterpret_cast<uint64_t*>(counts) : (*index += *counts);

        return false;
      }

      if (0u != *--data) {
        uint32_t *value = values + ((size == 32u ? *reinterpret_cast<uint32_t*>(count) : size == 64u ? *reinterpret_cast<uint64_t*>(count) : *count) - (NULL != subindex ? *subindex : 0u));

        while (true) {
          // ... ->> Halt for next asynchronous call
          if (NULL != subindex && 0u == rate--) {
            std::size_t subcount = size == 32u ? *reinterpret_cast<uint32_t*>(count) : size == 64u ? *reinterpret_cast<uint64_t*>(count) : *count;

            // ...
            size == 32u ? *reinterpret_cast<uint32_t*>(subindex) = (*reinterpret_cast<uint32_t*>(count)) - (value - values) : size == 64u ? *reinterpret_cast<uint64_t*>(subindex) = (*reinterpret_cast<uint64_t*>(count)) - (value - values) : (*subindex = (*count) - (value - values));
            for (size == 32u ? *reinterpret_cast<uint32_t*>(index) = 0u : size == 64u ? *reinterpret_cast<uint64_t*>(index) = 0u : (*index = 0u); subcount--; counts = size == 32u ? reinterpret_cast<unsigned*>(reinterpret_cast<uint32_t*>(counts) + 1) : size == 64u ? reinterpret_cast<unsigned*>(reinterpret_cast<uint64_t*>(counts) + 1) : (counts + 1)) size == 32u ? *reinterpret_cast<uint32_t*>(index) += *reinterpret_cast<uint32_t*>(counts) : size == 64u ? *reinterpret_cast<uint64_t*>(index) += *reinterpret_cast<uint64_t*>(counts) : (*index += *counts);

            return false;
          }

          // ... ->> Add color (iteration ended)
          if (value == values) {
            size == 32u ? reinterpret_cast<uint32_t*>(counts)[*reinterpret_cast<uint32_t*>(count)] = 1u : size == 64u ? reinterpret_cast<uint64_t*>(counts)[*reinterpret_cast<uint64_t*>(count)] = 1u : (counts[*count] = 1u);
            *(value = values + (size == 32u ? (*reinterpret_cast<uint32_t*>(count))++ : size == 64u ? (*reinterpret_cast<uint64_t*>(count))++ : (*count)++)) = *--data << 0x00u;
            *value |= *--data << 0x08u;
            *value |= *--data << 0x10u;

            break;
          }

          // ... ->> Increment count of repeated color
          if (*--value == static_cast<uint32_t>((data[-3] << 0x10u) | (data[-2] << 0x08u) | (data[-1] << 0x00u))) {
            size == 32u ? ++(reinterpret_cast<uint32_t*>(counts)[value - values]) : size == 64u ? ++(reinterpret_cast<uint64_t*>(counts)[value - values]) : ++(counts[value - values]);
            data -= 3;

            break;
          }
        }
      }
    }
  }
}
