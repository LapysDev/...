#include <cstdio>
#include <new>
#include <typeinfo>

/* ... */
// struct as{
//   template<class T>
//   struct p {
//     T&&x;
//     template<class U>
//     [[nodiscard]]constexpr auto operator->*(U*)noexcept(noexcept(U((T&&)x))){
//       return U((T&&)x);
//     }
//   };

//   explicit as()=default;
//   friend constexpr auto operator->*(auto&&x,as)noexcept{
//     return p((decltype(x)&&)x);
//   }
// };
// constexpr void*operator new(decltype(0uz),as)noexcept{return 0;}
// #define as ->*::as()->*::new(::as())
/* ================================================================================================================ */
struct as final {
  template <typename type>
  struct reinterpretation final {
    type &&object;

    template <typename subtype>
    [[nodiscard]] constexpr auto operator ->*(subtype* const) noexcept(noexcept((subtype) static_cast<type&&>(this -> object))) {
      return (subtype) static_cast<type&&>(this -> object);
    }
  };

  explicit as() = default;

  template <typename type>
  constexpr friend as::template reinterpretation<type> operator ->*(type&& object, struct as const) noexcept {
    return as::template reinterpretation<type>{static_cast<type&&>(object)};
  }
}

constexpr void* operator new(std::size_t const, struct as const) noexcept {
  return NULL;
}

#define as ->*::as()->*::new(::as{})
/* ================================================================================================================ */
// struct as final {
//   template <typename type>
//   struct reinterpretation final {
//     type &&object;

//     template <typename subtype>
//     constexpr auto operator %(subtype* const placeholder) const noexcept {
//       return (subtype) (::delete placeholder, static_cast<type&&>(this -> object));
//     }
//   };

//   template <typename type>
//   constexpr friend as::template reinterpretation<type> operator %(type&& object, struct as const) noexcept {
//     return as::template reinterpretation<type>{static_cast<type&&>(object)};
//   }
// };
// #define as /* … */ % ::as{} % ::new (::std::nothrow) /* T */ // ->> `%` because high precedence and overload-able

/* Main */
int main(int, char*[]) /* noexcept */ {
  std::printf("%s{%i}" "\r\n", typeid(5.5 as int).name(), 5.5 as int);
}
