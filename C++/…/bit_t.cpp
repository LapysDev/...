#include <stdint.h>

#include <climits>
#include <cstdio>

/* ... */
namespace {
  template <bool, typename, typename> union uint_conditional_t;
  template <typename true_t, typename false_t> union uint_conditional_t<true , true_t, false_t> { typedef true_t  type; };
  template <typename true_t, typename false_t> union uint_conditional_t<false, true_t, false_t> { typedef false_t type; };

  // ...
  template <std::size_t count>
  union bit_t {
    typedef
      typename uint_conditional_t<
        count <= (CHAR_BIT * sizeof(uint8_t )), uint8_t ,
      typename uint_conditional_t<
        count <= (CHAR_BIT * sizeof(uint16_t)), uint16_t,
      typename uint_conditional_t<
        count <= (CHAR_BIT * sizeof(uint32_t)), uint32_t,
      typename uint_conditional_t<
        count <= (CHAR_BIT * sizeof(uint64_t)), uint64_t,
        unsigned char [(count / CHAR_BIT) + (count % CHAR_BIT)]
      >::type >::type >::type >::type
    type;

    type value : count;
    bit_t(type const value) : value(value) {}
  };

  template <> union bit_t<8u>  { typedef uint8_t  type; uint8_t  value; bit_t(uint8_t  const value) : value(value) {} };
  template <> union bit_t<16u> { typedef uint16_t type; uint16_t value; bit_t(uint16_t const value) : value(value) {} };
  template <> union bit_t<32u> { typedef uint32_t type; uint32_t value; bit_t(uint32_t const value) : value(value) {} };
  template <> union bit_t<64u> { typedef uint64_t type; uint64_t value; bit_t(uint64_t const value) : value(value) {} };
}

template <std::size_t> union bit;
typedef bit<8u> byte; // octet

template <std::size_t count>
union bit {
  private:
    bit_t<count> value;

  public:
    bit() : value() {}
    bit(typename bit_t<count>::type const value) : value(value) {}

    // ...
    // friend bit<count>& operator += (bit<count>& a, bit<count> const b);
    // friend bit<count>& operator -= (bit<count>& a, bit<count> const b);
    // friend bit<count>& operator *= (bit<count>& a, bit<count> const b);
    // friend bit<count>& operator /= (bit<count>& a, bit<count> const b);
    // friend bit<count>& operator %= (bit<count>& a, bit<count> const b);
    friend bit<count>& operator &= (bit<count>& a, bit<count> const b) { a.value.value &=  b.value.value; return a; }
    friend bit<count>& operator |= (bit<count>& a, bit<count> const b) { a.value.value |=  b.value.value; return a; }
    friend bit<count>& operator ^= (bit<count>& a, bit<count> const b) { a.value.value ^=  b.value.value; return a; }
    friend bit<count>& operator <<=(bit<count>& a, bit<count> const b) { a.value.value <<= b.value.value; return a; }
    friend bit<count>& operator >>=(bit<count>& a, bit<count> const b) { a.value.value >>= b.value.value; return a; }
    // friend bit<count>& operator ++ (bit<count>&)           ;
    // friend bit<count>  operator ++ (bit<count>&, int const);
    // friend bit<count>& operator -- (bit<count>&)           ;
    // friend bit<count>  operator -- (bit<count>&, int const);

    /* explicit */ operator typename bit_t<count>::type()       { return this -> value.value; }
    /* explicit */ operator typename bit_t<count>::type() const { return this -> value.value; }
};

/* Main */
int main() {
  std::printf("[1]: %hu" "\r\n", static_cast<unsigned char>(bit<1u>(-1)));
  std::printf("[2]: %hu" "\r\n", static_cast<unsigned char>(bit<2u>(-1)));
  std::printf("[3]: %hu" "\r\n", static_cast<unsigned char>(bit<3u>(-1)));
  std::printf("[4]: %hu" "\r\n", static_cast<unsigned char>(bit<4u>(-1)));
  std::printf("[5]: %hu" "\r\n", static_cast<unsigned char>(bit<5u>(-1)));
  std::printf("[6]: %hu" "\r\n", static_cast<unsigned char>(bit<6u>(-1)));
  std::printf("[7]: %hu" "\r\n", static_cast<unsigned char>(bit<7u>(-1)));
  std::printf("[8]: %hu" "\r\n", static_cast<unsigned char>(bit<8u>(-1)));

  std::puts("");
  std::printf("[16]: %hu" "\r\n", static_cast<uint16_t>(bit<16u>(-1)));
  std::printf("[32]: %u " "\r\n", static_cast<uint32_t>(bit<32u>(-1)));
  std::printf("[64]: %lu" "\r\n", static_cast<uint64_t>(bit<64u>(-1)));
}
