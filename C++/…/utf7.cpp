#include <ciso646>
#include <climits>
#include <cstdio>
#include <stdint.h>

#if defined __INTEL_COMPILER or defined __INTEL_LLVM_COMPILER
# pragma warning(disable: 1418)
#endif

struct utf7_state /* final */ {
  uint_fast32_t bits;      // --> Base64 bits not yet consumed as UTF-16
  unsigned char bit_count; // --> number of valid low-order bits in `bits`
  bool          shifted;   // --> currently decoding a +... shifted sequence
};

/* ... */
unsigned char get_utf7_codepoint_length(char const units[], struct utf7_state state) {
  char const          *next  = units;
  uint_least32_t const value = get_utf7_codepoint_value(next, state);

  // ...
  if (value == static_cast<uint_least32_t>(-2)) return 0u;
  if (value == static_cast<uint_least32_t>(-1)) return static_cast<unsigned char>(-1);

  return static_cast<unsigned char>(next - units);
}

uint_least32_t get_utf7_codepoint_value(char const*& units, struct utf7_state& state) {
  uint_least32_t value = get_utf7_codeunit_value(units, state);

  // ...
  if (value == static_cast<uint_least32_t>(-1) or value == static_cast<uint_least32_t>(-2))
  return value;

  // ... ->> Low surrogate without a preceding high surrogate
  if (value >= 0xDC00uL and value <= 0xDFFFuL)
  return static_cast<uint_least32_t>(-1);

  // ... ->> Convert a UTF-16 surrogate pair to its Unicode code point
  if (value >= 0xD800uL and value <= 0xDBFFuL) {
    uint_least32_t const low_surrogate = get_utf7_codeunit_value(units, state);

    // ...
    if (low_surrogate < 0xDC00uL or low_surrogate > 0xDFFFuL)
    return static_cast<uint_least32_t>(-1);

    value = 0x10000uL + ((value - 0xD800uL) << 10u) + (low_surrogate - 0xDC00uL);
  }

  // ... ->> If this was the final code point of a shifted sequence, validate its trailing padding bits and consume an explicit `-`
  if (state.shifted and get_utf7_base64_value(*units) == static_cast<unsigned char>(-1)) {
    if (not finish_utf7_shift(units, state))
    return static_cast<uint_least32_t>(-1);
  }

  return value;
}
  bool finish_utf7_shift(char const*& units, struct utf7_state& state) {
    // ->> Encoding whole 16-bit UTF-16 units can leave only 0, 2, or 4 padding bits; RFC 2152 requires those remaining bits to be zero
    if (0uL == state.bits and state.bit_count <= 4u) {
      state.bits      = 0uL;
      state.bit_count = 0u;
      state.shifted   = false;
      units          += *units == '-'; // ->> `-` explicitly terminates shifted mode and is consumed; Any other non-Base64 character implicitly terminates it

      return true;
    }

    return false;
  }

  unsigned char get_utf7_base64_value(char const unit) {
    unsigned char const value = static_cast<unsigned char>(unit - '\0');

    // ->> UTF-7 uses ordinary Base64 without `=` padding
    if (value >= 'A' and value <= 'Z') return static_cast<unsigned char>(value - 'A');
    if (value >= 'a' and value <= 'z') return static_cast<unsigned char>(value - 'a' + 26u);
    if (value >= '0' and value <= '9') return static_cast<unsigned char>(value - '0' + 52u);
    if (value == '+')                  return 62u;
    if (value == '/')                  return 63u;

    return static_cast<unsigned char>(-1);
  }

  uint_least32_t get_utf7_codeunit_value(char const*& units, struct utf7_state& state) {
    while (true) {
      if (not state.shifted) {
        if (*units   == '\0') {          return static_cast<uint_least32_t>(-2); }                                                                             // ->> End of string
        if (*units   != '+')  {          return is_utf7_direct_unit(*units) ? static_cast<unsigned char>(*units++ - '\0') : static_cast<uint_least32_t>(-1); } // ->> Ordinary direct ASCII
        if (*++units == '-')  { ++units; return 0x2BuL; }                                                                                                      // ->> `+-` is the special direct representation of `+`
        if (get_utf7_base64_value(*units) == static_cast<unsigned char>(-1)) return static_cast<uint_least32_t>(-1);                                           // ->> `+` followed by neither Base64 nor `-` is malformed

        state.bits      = 0uL;
        state.bit_count = 0u;
      }

      // ... ->> Collect enough Base64 bits to produce one 16-bit UTF-16 code unit
      for (state.shifted = true; state.bit_count < 16u; ++units) {
        unsigned char const value = get_utf7_base64_value(*units);

        // ...
        if (value == static_cast<unsigned char>(-1)) {
          if (finish_utf7_shift(units, state))
          break;

          return static_cast<uint_least32_t>(-1);
        }

        state.bits      = (state.bits << 6u) | value;
        state.bit_count = static_cast<unsigned char>(state.bit_count + 6u);
      }

      // ... ->> Shifted sequence ended before another UTF-16 code unit, otherwise continue by interpreting its terminating character normally
      if (state.shifted) {
        unsigned char  const shift = static_cast<unsigned char>(state.bit_count - 16u);
        uint_least32_t const value = static_cast<uint_least32_t>((state.bits >> shift) & 0xFFFFuL);

        // ... ->> Retain any Base64 bits belonging to the following UTF-16 code unit
        state.bit_count = shift;
        state.bits      = 0u != shift ? state.bits & ((static_cast<uint_fast32_t>(1uL) << shift) - 1uL) : 0uL;

        return value;
      }
    }
  }

  bool is_utf7_direct_unit(char const unit) {
    unsigned char const value = static_cast<unsigned char>(unit - '\0');
    return value == '\t' or value == '\n' or value == '\r' or (value >= 0x20u and value <= 0x7Du and value != '+' and value != '\\'); // ->> RFC 2152 sets D + O, SPACE, TAB, CR, and LF; '\' and '~' are deliberately excluded from the direct set
  }

/* Main */
int main(int, char*[]) /* noexcept */ {
  // char units[] = "$"; // --> $
  // char units[] = "€"; // --> +AKM-
  // char units[] = "𐐷"; // --> +IKw-
  // char units[] = "𤭢"; // --> +2ADfSA-
  char const        units[] = "💙", *unit = units;
  struct utf7_state state   = {0uL, 0u, false};

  // ...
  for (char *byte = units; byte != units + (sizeof(units) / sizeof(char)); ++byte)
  std::printf("0x%.2hX" " ", static_cast<unsigned char>(*byte - '\0'));

  std::printf("(%hu)" "\r\n", get_utf7_codepoint_length(unit, state));
  std::printf("0x%lX" "\r\n", static_cast<unsigned long>(get_utf7_codepoint_value(unit, state)));
}
