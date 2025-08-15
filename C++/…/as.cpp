#include <cstdio>
#include <new>
#include <typeinfo>

/* ... */
struct as final {
  template <typename type>
  struct reinterpretation final {
    type &&object;

    template <typename subtype>
    /* [[nodiscard]] */ constexpr subtype operator ->*(subtype* const) const noexcept(noexcept((subtype) static_cast<type&&>(this -> object))) {
      return (subtype) static_cast<type&&>(this -> object);
    }
  };

  constexpr explicit as() /* = default */ {}

  template <typename type>
  constexpr friend as::template reinterpretation<type> operator ->*(type&& object, struct as const) noexcept {
    return as::template reinterpretation<type>{static_cast<type&&>(object)};
  }
};

constexpr void* operator new(std::size_t const, struct as const) noexcept {
  return NULL;
}

#define as ->* ::as() ->* ::new(::as{})

/* Main */
int main(int, char*[]) /* noexcept */ {
  std::printf("%s{%i}" "\r\n", typeid(5.5 as int).name(), 5.5 as int);
}
