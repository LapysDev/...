#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iso646.h>
#include <memory>
#include <stdint.h>

/* ... */
static struct allocator {
  enum : std::size_t { maximum = 131072u };
  std::max_align_t _;
  mutable unsigned char buffer[maximum];
  void* align(void* address, std::size_t alignment, std::size_t const size) const /* noexcept */ throw() /* --> [[assume(address >= ::buffer and address <= &::buffer[maximum])]] */ {
    #if false and defined PRIuPTR and defined UINTPTR_MAX // ->> Pointer arithmetic
      uintptr_t const offset = reinterpret_cast<uintptr_t>(address);

      if (UINTPTR_MAX - offset >= alignment - 1u and maximum >= size) /* --> [[assume(alignment and not (alignment & (alignment - 1u)))]] */ {
        uintptr_t const aligned = (offset + (alignment - 1u)) & ~static_cast<uintptr_t>(alignment - 1u);
        return aligned <= reinterpret_cast<uintptr_t>(&this -> buffer[maximum]) - size ? &static_cast<unsigned char*>(address)[aligned - offset] : NULL;
      }
    #else
      // ->> Manual arithmetic (in base-`UCHAR_MAX + 1` instead of base-2, base-16, …)
      // ->> Assumes little-endian representation and full integer-to-pointer mapping
      std::size_t const width = (sizeof address > sizeof alignment ? sizeof address : sizeof alignment) + 1u;
      unsigned char     aligned[width];
      union { bool borrow, zero; unsigned char carry; };

      // ... --> aligned = (uintptr_t) address
      alignment--;

      for (std::size_t index = width;          index-- != sizeof address; ) aligned[index] = 0x00u;
      for (std::size_t index = sizeof address; index-- != 0u; )             aligned[index] = reinterpret_cast<unsigned char const*>(&address)[index];

      // ... --> aligned += alignment - 1u
      for (std::size_t index = carry = 0x0u; index != width; ++index) {
        unsigned char      &destination = aligned[index];
        unsigned char const source      = index < sizeof alignment ? reinterpret_cast<unsigned char const*>(&alignment)[index] : 0x00u;
        unsigned char const sum         = carry + destination + source;

        // ...
        carry       = sum < (carry | destination | source);
        destination = sum;
      }

      // ... --> aligned &= ~(void*) (alignment - 1u)
      alignment = ~alignment;

      for (std::size_t index = sizeof alignment; index--; )
      aligned[index] &= reinterpret_cast<unsigned char const*>(&alignment)[index];

      // ... --> aligned -= offset
      for (std::size_t index = borrow = false; index != width; ++index) /* --> [[assume(aligned > offset)]] */ {
        bool          const borrowed = borrow;
        unsigned char const offset   = index < sizeof address ? reinterpret_cast<unsigned char const*>(&address)[index] : 0x00u;

        // ...
        borrow         = aligned[index] < offset; // --> if (borrow) aligned[…] = (UCHAR_MAX + aligned[…]) - offset[…] + 1u;
        aligned[index] = aligned[index] - offset - borrowed;
      }

      // ... ->> Does endianness matter from this point on to determine how much to increment `address`? --> address += aligned - address
      do {
        for (std::size_t index = 0u; index != sizeof(std::size_t); ++index) {
          std::size_t offset = aligned[index]; // --> aligned[…] << (CHAR_BIT * index)

          // ...
          for (std::size_t subindex = index; subindex--; ) {
            for (unsigned char representable = 0x1u; representable; representable <<= 1u)
            offset <<= 1u;
          }

          if (offset > static_cast<std::size_t>(&this -> buffer[maximum] - static_cast<unsigned char*>(address)))
          return NULL;

          address        = static_cast<unsigned char*>(address) + offset;
          aligned[index] = 0x00u;
        }

        zero = true;

        for (std::size_t index = sizeof(std::size_t); index < width; ++index)
        if (0x00u != aligned[index]--) {
          while (index--)
          --aligned[index]; // --> aligned[…] = UCHAR_MAX;

          zero = false;
          break;
        }
      } while (not zero);

      return size > static_cast<std::size_t>(&this -> buffer[maximum] - static_cast<unsigned char*>(address)) ? NULL : address;
    #endif

    return NULL;
  }
} arena = {};

/* Main */
int main(int, char*[]) /* noexcept */ throw() {
  std::size_t const alignments[] = {1u, 2u, 4u, 8u, 16u, 32u, 64u, 128u, 256u, 512u, 1024u};
  for (std::size_t const alignment : alignments) {
    void       *address  = arena.buffer + 69;
    std::size_t capacity = arena.maximum;
    void *const aligned  = arena.align(address, alignment, 1u);

    (void) capacity;
    (void) std::printf("(%zu) %td [", alignment, static_cast<unsigned char*>(aligned) - arena.buffer);
    do {
      address = std::align(alignment, 1u, address, capacity);
      (void) std::printf("%td" " ", static_cast<unsigned char*>(address) - arena.buffer);
      address = static_cast<unsigned char*>(address) + 1;
    } while (NULL != address and address < aligned and address < arena.buffer + arena.maximum);
    (void) std::printf("\b" "]" "\r\n");
  }
}
