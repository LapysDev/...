#include <cstddef>
#include <cstdio>
#include <new>
#include <stdint.h>
#include <utility>

/* ... ->> Constant-time initialization is the user’s responsibility i.e. `array<…, FIXED_SIZE_ONLY>` */
// array<char, N> == struct {
//   struct { char[N - (sizeof char* / sizeof char)]; uint; };
//   union  { char[0 + (sizeof char* / sizeof char)]; char* = malloc(size_t + char[]); };
// };

template <typename base, std::size_t initial>
struct array {
  enum /* : unsigned char */ { SOME_INITIAL, NO_INITIAL = 2u };
  enum /* : std::size_t */ { DYNAMIC_SIZE = sizeof(base*) / sizeof(base) };

  /* ... */
  template <unsigned char> union automatic;

  template <>
  union automatic<AUTOMATIC> {
    base A[initial - DYNAMIC_SIZE];
    unsigned B;
  };

  template <>
  union automatic<NO_INITIAL> {
    base A[initial - DYNAMIC_SIZE];
    unsigned B;
  };

  template <>
  union automatic<true> {};

  /* ... */
  union automatic<(DYNAMIC_SIZE >= initial) + (0u == initial)> _;

  // template <bool, typename = void, typename = void> struct conditional;
  // template       <typename baseA, typename baseB>   struct conditional<false, baseA, baseB> { typedef baseB type; };
  // template       <typename base>                    struct conditional<false, base,  void>  {};
  // template       <typename base>                    struct conditional<true,  void,  base>  {};
  // template       <typename baseA, typename baseB>   struct conditional<true,  baseA, baseB> { typedef baseA type; };
};

// template <typename T, std::size_t N>
// struct array {
//   struct elements {
//     union automatic {
//       union element {
//         char keep_uninitialized;
//         T    value;

//         element() {}
//       } nondata[N];
//       T data   [N];

//       automatic() : nondata() {}
//     } automatic;
//     T *dynamic;
//   }           elements;
//   std::size_t length;

//   /* ... */
//   array() noexcept : elements(), length(0u) {}

//   // ...
//   bool add(T&& element) noexcept {
//     if (this -> length < N) {
//       (void) ::new (&this->elements.automatic.nondata[this->length++].value) T(element); // activate union member
//       return true;
//     }

//     return false;
//   }

//   T* data() const {
//     return this->data(std::index_sequence<0u>{}, std::make_index_sequence<N>());
//   }

//   template <std::size_t... Is>
//   T* data(std::index_sequence<Is...>, std::index_sequence<Is...>) const {
//     T elements[sizeof...(Is)] = {this->elements.automatic.nondata[Is].value...};

//     for (union elements::automatic::element const &element : this->elements.automatic.nondata)
//     element.value.~T();

//     return ::new ((void*) &this->elements.automatic.nondata) T[sizeof...(Is)] {elements[Is]...};
//   }

//   template <std::size_t... Is, std::size_t... Js>
//   T* data(std::index_sequence<Is...> sequence, std::index_sequence<Js...> sentinel) const {
//     return this->length == sizeof...(Is) ? this->data(sequence, sequence) : this->data(std::index_sequence<Is..., sizeof...(Is)>{}, sentinel);
//   }
// };

/* Main */
int main(void) /* noexcept */ {
  (void) array<long, 1>();
  (void) std::puts("Hello, World!");
}
