#include <cstddef>

/* ... */
union as {
  template <typename T>
  struct reinterpretation final {
    T &&object;
  };

  /* ... */
  template <typename T, typename U> // ->> The second `->*` derefences the result to a non-allocating placement `new`
  [[nodiscard]] constexpr friend U operator ->*(struct as::template reinterpretation<T>&& reinterpretation, U*) noexcept(noexcept((U) static_cast<T&&>(reinterpretation.object))) {
    return (U) static_cast<T&&>(reinterpretation.object);
  }

  template <typename T, typename U> // ->> The second `->*` derefences the result to a user-specified template `as::operator T() noexcept` member function
  [[nodiscard]] constexpr friend U operator ->*(struct as::template reinterpretation<T>&& reinterpretation, U (as::*)() /* noexcept */) noexcept(noexcept((U) static_cast<T&&>(reinterpretation.object))) {
    return (U) static_cast<T&&>(reinterpretation.object);
  }

  template <typename T> // ->> The first `->*` produces a `struct as::reinterpretation<T>` result
  constexpr friend struct as::template reinterpretation<T> operator ->*(T&& object, union as) noexcept {
    return {static_cast<T&&>(object)};
  }

  template <typename T>
  operator T() noexcept {
    return reinterpret_cast<T&&>(reinterpret_cast<unsigned char&&>(*this));
  }
};

constexpr void* operator new  (std::size_t, union as*) noexcept { return NULL; }
constexpr void* operator new[](std::size_t, union as*) noexcept;

#define as ->* ::as() ->* &::as::operator                      // ->> Supported from C++98; Compile-time since C++11
// #define as ->* ::as() ->* ::new (static_cast<union as*>(NULL)) // ->> Supported from C++98; Compile-time since C++26; Accepts `T[]` with second-class support i.e. `new T[]` decays to `T*` instead of `T (*)[]`
// #define as ->* ::as() ->* ^^                                   // ->> Supported from C++26; Dereference the `std::meta::info` and splice it (i.e. `[:…:]`) back to a usable `T`

/* Main */
int main(int, char*[]) /* noexcept */ {
  static_assert(-42.0f as int == -42, "bruh");
}
