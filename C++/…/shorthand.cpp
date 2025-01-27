#include <cstdio>
/* ...
    --- CITE ---
      #Lapys:
        - Circle:                                             https://lapys.godbolt.org/z/qMn1qd43e
        - Clang, GNU, Intel, Microsoft Visual Studio, NVIDIA: https://lapys.godbolt.org/z/r3METo98K
*/
#include <ciso646>    // --> and, or, not
#include <climits>    //
#include <cstdarg>    // --> std::va_list
#include <cstddef>    // --> offsetof, std::byte, std::max_align_t, std::nullptr_t
#include <cstdlib>    // --> std::div_t, std::ldiv_t, std::lldiv_t
#include <exception>  // --> std::bad_exception, std::exception, std::exception_ptr, std::nested_exception, std::terminate_handler, std::unexpected_handler
#include <functional> // --> std::bad_function_call, std::bit_and<void>, std::bit_not<>, std::bit_or<void>, std::bit_xor<void>, std::divides<void>, std::equal_to<void>, std::greater<void>, std::greater_equal<void>, std::identity, std::less<void>, std::less_equal<void>, std::logical_and<void>, std::logical_not<void>, std::logical_or<void>, std::minus<void>, std::modulus<void>, std::multiplies<void>, std::negate<void>, std::not_equal_to<void>, std::plus<void>, decltype(std::placeholders::_1)
#include <iterator>   // --> std::bidirectional_iterator_tag, std::contiguous_iterator_tag, std::default_sentinel_t, std::forward_iterator_tag, std::input_iterator_tag, std::output_iterator_tag, std::random_access_iterator_tag, std::unreachable_sentinel_t
#include <memory>     // --> std::align(...), std::allocator_arg_t, std::bad_weak_ptr, std::owner_less<void>, std::pointer_safety
#include <new>        // --> std::align_val_t, std::bad_alloc, std::bad_array_new_length, std::destroying_delete_t, std::new_handler, std::nothrow_t
#include <stdint.h>   //
#include <typeinfo>   // --> std::bad_cast, std::bad_typeid, std::type_info
#include <utility>    // --> std::in_place_t, std::piecewise_construct_t
#if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) >= 201103L
# include <atomic>      // --> std::atomic_bool, std::atomic_char, std::atomic_char8_t, std::atomic_char16_t, std::atomic_char32_t, std::atomic_flag, std::atomic_int, std::atomic_llong, std::atomic_long, std::atomic_schar, std::atomic_short, std::atomic_uchar, std::atomic_uint, std::atomic_ullong, std::atomic_ulong, std::atomic_ushort, std::atomic_wchar_t, std::memory_order
# include <ratio>       // --> std::atto, std::centi, std::deca, std::deci, std::exa, std::femto, std::giga, std::hecto, std::kilo, std::mega, std::micro, std::milli, std::nano, std::peta, std::pico, std::tera, std::yocto, std::yotta, std::zepto, std::zetta
# include <type_traits> // --> std::false_type, std::is_trivially_destructible, std::true_type
#endif
#if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) >= 202002L or (defined __circle_lang__ or defined __clang__ or defined __CUDACC_VER_BUILD__ or defined __CUDACC_VER_MAJOR__ or defined __CUDACC_VER_MINOR__ or defined __GNUC__ or defined __ICC or defined __INTEL_COMPILER or defined __INTEL_LLVM_COMPILER or defined __NVCC__ or defined __NVCOMPILER)
# include <version>
#endif
#ifdef __cpp_lib_endian // --> 201907L
# include <bit> // --> std::endian
#endif
#ifdef __cpp_lib_three_way_comparison // --> 201907L
# include <compare> // --> std::compare_three_way, std::partial_ordering, std::strong_ordering, std::weak_ordering
# ifdef __cpp_lib_coroutine // --> 201902L
#   include <coroutine> // --> std::coroutine_handle<void>, std::noop_coroutine_handle, std::noop_coroutine_promise, std::suspend_always, std::suspend_never
# endif
#endif
#ifdef __cpp_lib_ranges // --> 201911L
# include <ranges> // --> std::dangling, std::from_range_t, std::subrange_kind
#endif
#ifdef __cpp_lib_source_location // --> 201907L
# include <source_location> // --> std::source_location
#endif
#ifdef __cpp_lib_spanstream // --> 202106L
# include <spanstream> // --> std::ispanstream, std::ospanstream, std::spanbuf, std::spanstream, std::wispanstream, std::wospanstream, std::wspanbuf, std::wspanstream
#endif
#ifdef __cpp_lib_stacktrace // --> 202011L
# include <stacktrace> // --> std::stacktrace, std::stacktrace_entry
#endif
#if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) >= 202002L // --> __cpp_lib_format {201907L}
# include <format> // --> std::format_args, std::wformat_args
#endif
#if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) > 202002L // --> __cpp_lib_expected {202202L}
# include <expected> // --> std::unexpect_t
# include <stdfloat> // --> std::bfloat16_t, std::float128_t, std::float16_t, std::float32_t, std::float64_t
#endif

/* ... */
#define sfinaeptr nullptr

typedef union null_t  &null;
typedef std::nullptr_t sfinaeptr_t;

// ... --> bless(...)
constexpr char*                         bless(char*                         address) noexcept { return address; }
constexpr char          const*          bless(char          const*          address) noexcept { return address; }
constexpr char          const volatile* bless(char          const volatile* address) noexcept { return address; }
constexpr char                volatile* bless(char                volatile* address) noexcept { return address; }
constexpr unsigned char*                bless(unsigned char*                address) noexcept { return address; }
constexpr unsigned char const*          bless(unsigned char const*          address) noexcept { return address; }
constexpr unsigned char const volatile* bless(unsigned char const volatile* address) noexcept { return address; }
constexpr unsigned char       volatile* bless(unsigned char       volatile* address) noexcept { return address; }
#ifdef __cpp_lib_byte // --> 201603L
  constexpr std::byte*                bless(std::byte*                address) noexcept { return address; }
  constexpr std::byte const*          bless(std::byte const*          address) noexcept { return address; }
  constexpr std::byte const volatile* bless(std::byte const volatile* address) noexcept { return address; }
  constexpr std::byte       volatile* bless(std::byte       volatile* address) noexcept { return address; }
#endif

#if defined __cpp_lib_launder /* --> 201606L */ or (defined _MSVC_LANG and defined _HAS_LAUNDER) or (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) >= 201703L
  template <typename type>
  constexpr type* bless(type* const address) noexcept {
    return std::launder(address);
  }
#elif defined __has_builtin and (defined __clang__ or defined __GNUC__)
  #if __has_builtin(__builtin_launder)
    template <typename type>
    constexpr type* bless(type* const address) noexcept {
      return __builtin_launder(address);
    }
  #else
    template <typename type>
    constexpr type* bless(type* const address) noexcept {
      return address; // ->> WARN (Lapys) -> Spoofed
    }
  #endif
#elif defined __GNUC__
  template <typename type>
  inline type* bless(type* address) noexcept {
    __asm__("" : "+r"(address));
    return address;
  }
#elif defined _ReadWriteBarrier and defined _MSC_VER
  template <typename type>
  inline type* bless(type* address) noexcept {
    ::_ReadWriteBarrier();
    return address;
  }
#else
  template <typename type>
  constexpr type* bless(type* const address) noexcept {
    return address; // ->> WARN (Lapys) -> Spoofed
  }
#endif

// ... --> forward_cast<T>(T{}) ->> For perfect-forwarding objects (preserving their value specification)
template <typename type>
constexpr type&& forward_cast(type& object) noexcept {
  return static_cast<type&&>(object);
}

template <typename type>
constexpr type&& forward_cast(type&& object) noexcept {
  return static_cast<type&&>(object);
}

// ... --> instanceof<T>() ->> For testing objects in unevaluated contexts
template <typename type>
constexpr type instanceof() noexcept;

// ... --> bitwise_ceil<N> ->> Rounds up to the next (equal or greater) power of two
template <std::size_t value>
struct bitwise_ceil final {
  private:
    template <std::size_t ceiling, bool = (ceiling < value)>
    struct valueof final {
      static std::size_t const value = ceiling;
    };

    template <std::size_t ceiling>
    struct valueof<ceiling, true> final {
      static std::size_t const value = valueof<(ceiling > SIZE_MAX >> 1u ? SIZE_MAX : 0u != ceiling ? ceiling << 1u : 1u)>::value;
    };

  public:
    static std::size_t const value = valueof<0u, 0u != value>::value;
};

// ... --> boolean_and<bool...> --> std::conjunction
template <bool boolean, bool...>
struct boolean_and final {
  static bool const value = boolean;
};

template <bool booleanA, bool booleanB, bool... booleans>
struct boolean_and<booleanA, booleanB, booleans...> final {
  static bool const value = boolean_and<booleanA and booleanB, booleans...>::value;
};

// ... --> boolean_or<bool...> --> std::disjunction
template <bool boolean, bool...>
struct boolean_or final {
  static bool const value = boolean;
};

template <bool booleanA, bool booleanB, bool... booleans>
struct boolean_or<booleanA, booleanB, booleans...> final {
  static bool const value = boolean_or<booleanA or booleanB, booleans...>::value;
};

// ... --> conditional<bool, T>, conditional<bool, T, T>, std::conditional<bool, T, T>, std::enable_if<bool, T> ->> For SFINAE testing or conditional typing respectively
template <bool, typename = null, typename = null>
struct conditional;

template <>
struct conditional<true> final {};

template <typename base>
struct conditional<false, base> final {};

template <typename baseA, typename baseB>
struct conditional<false, baseA, baseB> final {
  typedef baseB type;
};

template <typename baseA, typename baseB>
struct conditional<true,  baseA, baseB> final {
  typedef baseA type;
};

// ... --> at<N, Ts...> ->> Indexes object from argument list or type from parameter pack
template <std::size_t index, typename...>
struct at final {
  typedef null type;

  // ... ->> at<N>::value(Ts{}...)
  template <typename... types>
  constexpr static typename conditional<(index > sizeof...(types)), sfinaeptr_t>::type value(types&&...) noexcept {
    static_assert(index <= sizeof...(types) + 1u, "Too few arguments passed to function");
    return sfinaeptr;
  }

  template <typename type, typename... types>
  constexpr static typename conditional<
    0u == index and index <= sizeof...(types) + 1u,
    typename at<index, type&&, types&&...>::type
  >::type value(type&& argument, types&&...) noexcept {
    // ->> Indexed argument
    return forward_cast<type>(argument);
  }

  template <typename type, typename... types>
  constexpr static typename conditional<
    0u != index and index <= sizeof...(types) + 1u,
    typename at<index, type&&, types&&...>::type
  >::type value(type&&, types&&... arguments) noexcept {
    // ->> Next argument
    return at<index - 1u>::value(forward_cast<types>(arguments)...);
  }
};

template <typename base, typename... bases>
struct at<0u, base, bases...> final {
  // ->> Indexed argument
  typedef base type;
};

template <std::size_t index, typename base, typename... bases>
struct at<index, base, bases...> final {
  // ->> Next argument
  typedef typename at<index - 1u, bases...>::type type;
};

// ... --> gcd<Ns...> ->> Evaluates the greatest common divisor of `Ns...`
template <std::size_t divisor, std::size_t...>
struct gcd final {
  static std::size_t const value = divisor;
};

template <std::size_t divisor, std::size_t... divisors>
struct gcd<divisor, divisor, divisors...> final {
  static std::size_t const value = gcd<divisor, divisors...>::value;
};

template <std::size_t divisorA, std::size_t divisorB, std::size_t... divisors>
struct gcd<divisorA, divisorB, divisors...> final {
  static std::size_t const value = gcd<
    (divisorA < divisorB ? divisorB % divisorA : divisorB),
    (divisorB < divisorA ? divisorA % divisorB : divisorA),
    divisors...
  >::value;
};

template <std::size_t divisor, std::size_t... divisors>
struct gcd<divisor, 0u, divisors...> final {
  static std::size_t const value = gcd<divisor, divisors...>::value;
};

template <std::size_t divisor, std::size_t... divisors>
struct gcd<0u, divisor, divisors...> final {
  static std::size_t const value = gcd<divisor, divisors...>::value;
};

// ...
template <std::size_t multiple, std::size_t...>
struct lcm final {
  static std::size_t const value = multiple;
};

template <std::size_t multipleA, std::size_t multipleB, std::size_t... multiples>
struct lcm<multipleA, multipleB, multiples...> final {
  static std::size_t const value = lcm<(multipleA / gcd<multipleA, multipleB>::value) * multipleB, multiples...>::value;
};

// ...
struct commonof final {
  // ... --> commonof::alignment<N, Ns...> ->> Evaluates the best-fit alignment of multiple sizes
  template <typename... bases>
  struct alignment final {
    private:
      template <std::size_t...>
      struct valueof final {
        static std::size_t const value = 0u;
      };

      template <std::size_t alignment>
      struct valueof<alignment> final {
        static std::size_t const value = bitwise_ceil<alignment>::value;
      };

      template <std::size_t alignmentA, std::size_t alignmentB, std::size_t... alignments>
      struct valueof<alignmentA, alignmentB, alignments...> final {
        static std::size_t const value = valueof<(
          0u == alignmentA % alignmentB ? alignmentA :
          0u == alignmentB % alignmentA ? alignmentB :
          lcm<alignmentA, alignmentB>::value
        ), alignments...>::value;
      };

    public:
      static std::size_t /* --> std::align_val_t */ const value = valueof<alignof(bases)...>::value;
  };

  // ... --> commonof::size<N, Ns...> ->> Evaluates the most-fit (maximum) size of multiple types
  template <typename... bases>
  struct size final {
    private:
      template <std::size_t...>
      struct valueof final {
        static std::size_t const value = 0u;
      };

      template <std::size_t size>
      struct valueof<size> final {
        static std::size_t const value = size;
      };

      template <std::size_t sizeA, std::size_t sizeB, std::size_t... sizes>
      struct valueof<sizeA, sizeB, sizes...> final {
        static std::size_t const value = valueof<(
          sizeA > sizeB ? sizeA : sizeB
        ), sizes...>::value;
      };

    public:
      static std::size_t const value = valueof<sizeof(bases)...>::value;
  };
};

// ... --> is_array<T>, std::is_array<T> ->> Determines if `T` is an array type
template <typename>
struct is_array final {
  static bool const value = false;
};

template <typename base>
struct is_array<base[]> final {
  static bool const value = true;
};

template <typename base, std::size_t capacity>
struct is_array<base[capacity]> final {
  static bool const value = true;
};

// ... --> is_boolean<T> ->> Determines if `T` is a boolean type
template <typename>
struct is_boolean final {
  static bool const value = false;
};

template <>
struct is_boolean<bool> final {
  static bool const value = true;
};

// ... --> is_character<T> ->> Determines if `T` is a character type
template <typename>
struct is_character final {
  static bool const value = false;
};

template <>
struct is_character<char> final {
  static bool const value = true;
};

template <>
struct is_character<wchar_t> final {
  static bool const value = true;
};

#ifdef __cpp_char8_t // --> 201811L
  template <>
  struct is_character<char8_t> final {
    static bool const value = true;
  };
#endif

#ifdef __cpp_lib_byte // --> 201603L
  template <>
  struct is_character<std::byte> final {
    static bool const value = true;
  };
#endif

#ifdef __cpp_unicode_characters // --> 200704L
  template <>
  struct is_character<char16_t> final {
    static bool const value = true;
  };

  template <>
  struct is_character<char32_t> final {
    static bool const value = true;
  };
#endif

// ... --> is_union<T>, std::is_union<T> ->> Determines if `T` is a `union` class type
template <class base>
struct is_union final {
  private:
    template <typename type> // ->> Detection through SFINAE member type
    constexpr static bool const (&valueof(sfinaeptr_t const, unsigned char type::* const = NULL) noexcept)[true + 1u];

    template <typename>
    constexpr static bool const (&valueof(...) noexcept)[false + 1u];

  public:
    static bool const value =
      #if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) >= 201103L
        std::is_union<base>::value or
      #elif defined __circle_lang__ or defined __clang__ or defined __CUDACC_VER_BUILD__ or defined __CUDACC_VER_MAJOR__ or defined __CUDACC_VER_MINOR__ or defined __NVCC__ or defined __NVCOMPILER or defined __GNUC__ or defined __ICC or defined __INTEL_COMPILER or defined __INTEL_LLVM_COMPILER or defined __llvm__
      # ifdef __has_builtin
      #   if __has_builtin(__is_union)
            __is_union(base) or
      #   endif
      # endif
      #elif defined _MSC_VER
        __is_union(base) or
      #endif
    sizeof(bool const (&)[true + 1u]) == sizeof valueof<base>(sfinaeptr);
};

// ... --> is_class<T>, std::is_class<T> ->> Determines if `T` is a class type
template <class base>
struct is_class final {
  static bool const value =
    #if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) >= 201103L
      std::is_class<base>::value or
    #elif defined __circle_lang__ or defined __clang__ or defined __CUDACC_VER_BUILD__ or defined __CUDACC_VER_MAJOR__ or defined __CUDACC_VER_MINOR__ or defined __NVCC__ or defined __NVCOMPILER or defined __GNUC__ or defined __ICC or defined __INTEL_COMPILER or defined __INTEL_LLVM_COMPILER or defined __llvm__
    # ifdef __has_builtin
    #   if __has_builtin(__is_class)
          __is_class(base) or
    #   endif
    # endif
    #elif defined _MSC_VER
      __is_class(base) or
    #endif
  is_union<base>::value;
};

// ... --> is_const<T>, std::is_const<T> ->> Determines if `T` is `const`-qualified
template <typename>
struct is_const final {
  static bool const value = false;
};

template <typename base>
struct is_const<base const> final {
  static bool const value = true;
};

// ... --> is_decimal<T>, std::is_floating_point<T> ->> Determines if `T` is a floating-point arithmetic type
template <typename>
struct is_decimal final {
  static bool const value = false;
};

template <>
struct is_decimal<double> final {
  static bool const value = true;
};

template <>
struct is_decimal<float> final {
  static bool const value = true;
};

template <>
struct is_decimal<long double> final {
  static bool const value = true;
};

#if defined __STDCPP_BFLOAT16_T__
  template <>
  struct is_decimal<std::bfloat16_t> final
    static bool const value = true;
  };
#endif

#if defined __STDCPP_FLOAT16_T__
  template <>
  struct is_decimal<std::float16_t> final {
    static bool const value = true;
  };
#endif

#if defined __STDCPP_FLOAT32_T__
  template <>
  struct is_decimal<std::float32_t> final {
    static bool const value = true;
  };
#endif

#if defined __STDCPP_FLOAT64_T__
  template <>
  struct is_decimal<std::float64_t> final {
    static bool const value = true;
  };
#endif

#if defined __STDCPP_FLOAT128_T__
  template <>
  struct is_decimal<std::float128_t> final
   static bool const value = true;
};
#endif

// ... --> is_integer<T>, std::is_integer<T> ->> Determines if `T` is an integer arithmetic type
template <typename>
struct is_integer final {
  static bool const value = false;
};

template <>
struct is_integer<int> final {
  static bool const value = true;
};

template <>
struct is_integer<long> final {
  static bool const value = true;
};

template <>
struct is_integer<long long> final {
  static bool const value = true;
};

template <>
struct is_integer<short> final {
  static bool const value = true;
};

template <>
struct is_integer<signed char> final {
  static bool const value = true;
};

template <>
struct is_integer<unsigned char> final {
  static bool const value = true;
};

template <>
struct is_integer<unsigned int> final {
  static bool const value = true;
};

template <>
struct is_integer<unsigned long> final {
  static bool const value = true;
};

template <>
struct is_integer<unsigned long long> final {
  static bool const value = true;
};

template <>
struct is_integer<unsigned short> final {
  static bool const value = true;
};

#if defined __clang__
  template <>
  struct is_integer<__int128_t> final {
    static bool const value = true;
  };

  template <>
  struct is_integer<__uint128_t> final {
    static bool const value = true;
  };
#elif defined __GNUC__
# ifdef __SIZEOF_INT128__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wpedantic"
  template <>
  struct is_integer<__int128> final {
    static bool const value = true;
  };

  template <>
  struct is_integer<unsigned __int128> final {
    static bool const value = true;
  };
#   pragma GCC diagnostic pop
# endif
#elif defined __cpp_lib_ranges // --> 201911L
  struct is_integer<std::ranges::range_difference_t<std::ranges::iota_view<long long, long long> > > final {
    static bool const value = true;
  };

  template <>
  struct is_integer<std::ranges::range_difference_t<std::ranges::iota_view<unsigned long long, unsigned long long> > > final {
    static bool const value = true;
  };
#endif

// ... --> is_lvalue_reference<T>, std::is_lvalue_reference<T> ->> Determines if `T` is an lvalue-categorized reference type
template <typename>
struct is_lvalue_reference final {
  static bool const value = false;
};

template <typename base>
struct is_lvalue_reference<base&> final {
  static bool const value = true;
};

// ... --> is_member_pointer<T>, std::is_member_pointer<T> ->> Determines if `T` is an data member pointer type
template <typename>
struct is_member_pointer final {
  static bool const value = false;
};

template <typename baseA, class baseB>
struct is_member_pointer<baseA baseB::*> final {
  static bool const value = true;
};

template <typename baseA, class baseB>
struct is_member_pointer<baseA baseB::* const> final {
  static bool const value = true;
};

template <typename baseA, class baseB>
struct is_member_pointer<baseA baseB::* const volatile> final {
  static bool const value = true;
};

template <typename baseA, class baseB>
struct is_member_pointer<baseA baseB::* volatile> final {
  static bool const value = true;
};

// ... --> is_null<T> ->> Determines if `T` is the SFINAE non-type
template <typename>
struct is_null final {
  static bool const value = false;
};

template <>
struct is_null<null> final {
  static bool const value = true;
};

// ... --> is_pointer<T>, std::is_pointer<T> ->> Determines if `T` is an object pointer type
template <typename>
struct is_pointer final {
  static bool const value = false;
};

template <typename base>
struct is_pointer<base*> final {
  static bool const value = true;
};

template <typename base>
struct is_pointer<base* const> final {
  static bool const value = true;
};

template <typename base>
struct is_pointer<base* const volatile> final {
  static bool const value = true;
};

template <typename base>
struct is_pointer<base* volatile> final {
  static bool const value = true;
};

// ... --> is_rvalue_reference<T>, std::is_rvalue_reference<T> ->> Determines if `T` is an rvalue-categorized reference type
template <typename>
struct is_rvalue_reference final {
  static bool const value = false;
};

template <typename base>
struct is_rvalue_reference<base&&> final {
  static bool const value = true;
};

// ... --> is_reference<T>, std::is_reference<T> ->> Determines if `T` is a reference type
template <typename base>
struct is_reference final {
  static bool const value = is_lvalue_reference<base>::value or is_rvalue_reference<base>::value;
};

// ... --> is_same<T, T>, std::is_same<T, T> ->> Determines if `T` and `U` are the same type (considering categorizations, qualifications, and specifications)
template <typename baseA, typename baseB>
struct is_same final {
  static bool const value = false;
};

template <typename base>
struct is_same<base, base> final {
  static bool const value = true;
};

// ... --> is_void<T>, std::is_void<T> ->> Determines if `T` is an incomplete empty set type
template <typename>
struct is_void final {
  static bool const value = false;
};

template <>
struct is_void<void> final {
  static bool const value = true;
};

template <>
struct is_void<void const> final {
  static bool const value = true;
};

template <>
struct is_void<void const volatile> final {
  static bool const value = true;
};

template <>
struct is_void<void volatile> final {
  static bool const value = true;
};

// ... --> is_volatile<T>, std::is_volatile<T> ->> Determines if `T` is `volatile`-qualified
template <typename>
struct is_volatile final {
  static bool const value = false;
};

template <typename base>
struct is_volatile<base volatile> final {
  static bool const value = true;
};

// ... --> is_enum<T>, std::is_enum<T> ->> Determines if `T` is an enumeration type
template <typename base>
struct is_enum final {
  private:
    template <typename type> // ->> Supplemented detection through compile-time implicit shorthand_cast and unary operation
    constexpr static bool (&valueof(sfinaeptr_t const, bool const (*const)[sizeof static_cast<std::size_t>(+instanceof<type>())] = NULL) noexcept)[true + 1u];

    template <typename>
    constexpr static bool (&valueof(...) noexcept)[false + 1u];

  public:
    static bool const value =
      #if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) >= 201103L
        std::is_enum<base>::value or
      #elif defined __circle_lang__ or defined __clang__ or defined __CUDACC_VER_BUILD__ or defined __CUDACC_VER_MAJOR__ or defined __CUDACC_VER_MINOR__ or defined __NVCC__ or defined __NVCOMPILER or defined __GNUC__ or defined __ICC or defined __INTEL_COMPILER or defined __INTEL_LLVM_COMPILER or defined __llvm__
      # ifdef __has_builtin
      #   if __has_builtin(__is_enum)
            __is_enum(base) or
      #   endif
      # endif
      #elif defined _MSC_VER
        __is_enum(base) or
      #endif
    (not is_class<base>::value and not is_character<base>::value and not is_integer<base>::value and not is_pointer<base>::value and not is_void<base>::value and not is_same<base, std::nullptr_t>::value and sizeof(bool const (&)[true + 1u]) == sizeof valueof<base>(sfinaeptr));
};

// ... --> is_function<T>, std::is_function<T> ->> Determines if `T` is a function type
template <typename base>
struct is_function final {
  static bool const value = not is_const<base const>::value and not is_reference<base>::value and not is_volatile<base volatile>::value;
};

// ... --> novoid{}, ..., novoid{} ->> Re-evaluates possibly `void` expressions as `struct novoid` expressions
struct novoid final {
  #ifdef __cpp_rvalue_references // --> 200610L
    template <typename type>
    constexpr sfinaeptr_t operator ,(type&&) const noexcept;
  #else
    template <typename type> constexpr sfinaeptr_t operator ,(type&)                const noexcept;
    template <typename type> constexpr sfinaeptr_t operator ,(type const&)          const noexcept;
    template <typename type> constexpr sfinaeptr_t operator ,(type const volatile&) const noexcept;
    template <typename type> constexpr sfinaeptr_t operator ,(type       volatile&) const noexcept;
  #endif

  constexpr friend sfinaeptr_t operator ,(sfinaeptr_t const, novoid const) noexcept;
};

// ...
struct opinfo final {
  enum op : unsigned char {
    access,                      // -->                  x   . y
    access_pointer,              // -->                  x  .* y
    add,                         // -->                  x   + y
    address,                     // -->                 &x
    alignof_object,              // --> alignof         (x)
    assign,                      // -->                  x   = y
    assign_add,                  // -->                  x  += y
    assign_bitwise_and,          // -->                  x  &= y
    assign_bitwise_or,           // -->                  x  |= y
    assign_bitwise_shift_left,   // -->                  x <<= y
    assign_bitwise_shift_right,  // -->                  x >>= y
    assign_bitwise_xor,          // -->                  x  ^= y
    assign_copy,                 // -->                  x   = static_cast<x const&>(...)
    assign_divide,               // -->                  x  /= y
    assign_modulo,               // -->                  x  %= y
    assign_move,                 // -->                  x   = static_cast<x&&>(...)
    assign_multiply,             // -->                  x  *= y
    assign_subtract,             // -->                  x  -= y
    bitwise_and,                 // -->                  x   & y
    bitwise_or,                  // -->                  x   | y
    bitwise_shift_left,          // -->                  x  << y
    bitwise_shift_right,         // -->                  x  >> y
    bitwise_xor,                 // -->                  x   ^ y
    boolean_and,                 // -->                  x  && y
    boolean_or,                  // -->                  x  || y
    call,                        // -->                  x             (...)
    call_static,                 // -->                  x::operator ()(...)
    cast,                        // -->                 (x)    y
    cast_const,                  // --> const_cast      <x>   (y)
    cast_dynamic,                // --> dynamic_cast    <x>   (y)
    cast_reinterpret,            // --> reinterpret_cast<x>   (y)
    cast_static,                 // --> static_cast     <x>   (y)
    comma,                       // -->                  x   , y
    compare,                     // -->                  x <=> y
    complement,                  // -->                 ~x
    construct,                   // -->                  x::x(...)
    copy,                        // -->                  x::x (static_cast<x const&>(...))
    delete_array,                // --> delete[]         x
    delete_object,               // --> delete           x
    dereference,                 // -->                 *x
    dereferenced_access,         // -->                  x  -> y
    dereferenced_access_pointer, // -->                  x ->* y
    destruct,                    // -->                  x  .~ x()
    divide,                      // -->                  x   / y
    equals,                      // -->                  x  == y
    greater,                     // -->                  x   > y
    greater_equals,              // -->                  x  >= y
    initialize,                  // -->                  x {...}
    lesser,                      // -->                  x   < y
    lesser_equals,               // -->                  x  <= y
    minus,                       // -->                  x   - y
    modulo,                      // -->                  x   % y
    move,                        // -->                  x::x (static_cast<x&&>(...))
    multiply,                    // -->                  x   * y
    negate,                      // -->                 !x
    new_array,                   // --> new              x    [y]   {...}
    new_array_placement,         // --> new             (x)    y[z] {...}
    new_constructed,             // --> new              x          (...)
    new_constructed_placement,   // --> new             (x)    y    (...)
    new_initialized,             // --> new              x          {...}
    new_initialized_placement,   // --> new             (x)    y    {...}
    noexcept_object,             // --> noexcept        (x)
    plus,                        // -->                 +x
    post_decrement,              // -->                  x--
    post_increment,              // -->                  x++
    pre_decrement,               // -->                --x
    pre_increment,               // -->                ++x
    scope,                       // -->                  x  :: y
    sizeof_object,               // --> sizeof           x
    subscript,                   // -->                  x             [...]
    subscript_static,            // -->                  x::operator [](...)
    subtract,                    // -->                  x   - y
    trilean_conditional,         // -->                  x   ? y : z
    typeid_object,               // --> typeid          (x)
    unequals,                    // -->                  x  != y

    constant = static_cast<unsigned char>(1u) << ((CHAR_BIT * sizeof(unsigned char)) - 1u),
      constant_access, constant_access_pointer, constant_add, constant_address, constant_alignof_object, constant_assign, constant_assign_add, constant_assign_bitwise_and, constant_assign_bitwise_or, constant_assign_bitwise_shift_left, constant_assign_bitwise_shift_right, constant_assign_bitwise_xor, constant_assign_copy, constant_assign_divide, constant_assign_modulo, constant_assign_move, constant_assign_multiply, constant_assign_subtract,
      constant_bitwise_and, constant_bitwise_or, constant_bitwise_shift_left, constant_bitwise_shift_right, constant_bitwise_xor, constant_boolean_and, constant_boolean_or,
      constant_call, constant_call_static, constant_cast, constant_cast_const, constant_cast_dynamic, constant_cast_reinterpret, constant_cast_static, constant_comma, constant_compare, constant_complement, constant_construct, constant_copy,
      constant_delete_array, constant_delete_object, constant_dereference, constant_dereferenced_access, constant_dereferenced_access_pointer, constant_destruct, constant_divide,
      constant_equals,
      constant_greater, constant_greater_equals,
      constant_initialize,
      constant_lesser, constant_lesser_equals,
      constant_minus, constant_move, constant_modulo, constant_multiply,
      constant_negate, constant_new_array, constant_new_array_placement, constant_new_constructed, constant_new_constructed_placement, constant_new_initialized, constant_new_initialized_placement, constant_noexcept_object,
      constant_plus, constant_post_decrement, constant_post_increment, constant_pre_decrement, constant_pre_increment,
      constant_scope, constant_sizeof_object, constant_subscript, constant_subscript_static, constant_subtract,
      constant_trilean_conditional, constant_typeid_object,
      constant_unequals
  };

  /* ... */
  #ifdef _MSC_VER
  # pragma warning(push)
  # pragma warning(disable: 4913)
  #endif
    template <op, typename, typename...>
    struct can;

    template <typename base>
    struct can<opinfo::address, base> final {
      private:
        template <typename type>
        static bool const (&valueof(sfinaeptr_t const, bool const (*const)[sizeof(novoid{}, &instanceof<type>(), novoid{})] = NULL) noexcept)[true + 1u];

        template <typename>
        static bool const (&valueof(...) noexcept)[false + 1u];

      public:
        static bool const value = sizeof(bool const (&)[true + 1u]) == sizeof valueof<base>(sfinaeptr);
    };

    template <typename base>
    struct can<opinfo::alignof_object, base> final {
      private:
        template <typename type>
        static bool const (&valueof(sfinaeptr_t const, bool const (*const)[sizeof(novoid{}, alignof(type), novoid{})] = NULL) noexcept)[true + 1u];

        template <typename>
        static bool const (&valueof(...) noexcept)[false + 1u];

      public:
        static bool const value = sizeof(bool const (&)[true + 1u]) == sizeof valueof<base>(sfinaeptr);
    };

    template <typename base, typename... bases>
    struct can<opinfo::constant_construct, base, bases...> final {
      private:
        template <typename type, typename... types>
        static bool const (&valueof(sfinaeptr_t const, bool const (*const)[(static_cast<void>(type(instanceof<types>()...)), 1u)] = NULL) noexcept)[true + 1u];

        template <typename, typename...>
        static bool const (&valueof(...) noexcept)[false + 1u];

      public:
        static bool const value = sizeof(bool const (&)[true + 1u]) == sizeof valueof<base, bases...>(sfinaeptr);
    };

    template <typename base>
    struct can<opinfo::constant_destruct, base> final {
      private:
        template <typename type>
        static bool const (&valueof(sfinaeptr_t const, bool const (*const)[(instanceof<type>().~type(), 1u)] = NULL) noexcept)[true + 1u];

        template <typename, typename...>
        static bool const (&valueof(...) noexcept)[false + 1u];

      public:
        static bool const value = sizeof(bool const (&)[true + 1u]) == sizeof valueof<base>(sfinaeptr);
    };

    template <typename base>
    struct can<opinfo::copy, base> final {
      static bool const value = (
        opinfo::can<opinfo::construct, base, base>                ::value or
        opinfo::can<opinfo::construct, base, base&>               ::value or
        opinfo::can<opinfo::construct, base, base const&>         ::value or
        opinfo::can<opinfo::construct, base, base const volatile&>::value or
        opinfo::can<opinfo::construct, base, base       volatile&>::value
      );
    };

    template <typename base, typename... bases>
    struct can<opinfo::construct, base, bases...> final {
      private:
        template <typename type, typename... types>
        static bool const (&valueof(sfinaeptr_t const, bool const (*const)[sizeof(novoid{}, type(instanceof<types>()...), novoid{})] = NULL) noexcept)[true + 1u];

        template <typename, typename...>
        static bool const (&valueof(...) noexcept)[false + 1u];

      public:
        static bool const value = sizeof(bool const (&)[true + 1u]) == sizeof valueof<base, bases...>(sfinaeptr);
    };

    template <typename base>
    struct can<opinfo::sizeof_object, base> final {
      private:
        template <typename type>
        static bool const (&valueof(sfinaeptr_t const, bool const (*const)[sizeof(novoid{}, sizeof(type), novoid{})] = NULL) noexcept)[true + 1u];

        template <typename>
        static bool const (&valueof(...) noexcept)[false + 1u];

      public:
        static bool const value = sizeof(bool const (&)[true + 1u]) == sizeof valueof<base>(sfinaeptr);
    };

    // ...
    template <op, typename, typename...>
    struct has_overloaded_function;

    template <typename base>
    struct has_overloaded_function<opinfo::address, base> final {
      private:
        template <typename type>
        static bool const (&valueof(sfinaeptr_t const, bool const (*const)[sizeof(novoid{}, operator &(instanceof<type>()), novoid{})] = NULL) noexcept)[true + 1u];

        template <typename>
        static bool const (&valueof(...) noexcept)[false + 1u];

      public:
        static bool const value = sizeof(bool const (&)[true + 1u]) == sizeof valueof<base>(sfinaeptr);
    };

    // ...
    template <op, typename, typename...>
    struct has_overloaded_member;

    template <typename base>
    struct has_overloaded_member<opinfo::address, base> final {
      private:
        template <typename type>
        static bool const (&valueof(sfinaeptr_t const, bool const (*const)[sizeof(novoid{}, instanceof<type>().operator &(), novoid{})] = NULL) noexcept)[true + 1u];

        template <typename>
        static bool const (&valueof(...) noexcept)[false + 1u];

      public:
        static bool const value = sizeof(bool const (&)[true + 1u]) == sizeof valueof<base>(sfinaeptr);
    };
  #ifdef _MSC_VER
  # pragma warning(pop)
  #endif
};

// ... --> remove_const<T>, std::remove_const<T> ->> Aliases `T` without `const`-qualification
template <typename base>
struct remove_const final {
  typedef base type;
};

template <typename base>
struct remove_const<base const> final {
  typedef base type;
};

// ... --> remove_lvalue_reference<T>, std::remove_lvalue_reference<T> ->> Aliases `T` without lvalue-categorized reference-qualification
template <typename base>
struct remove_lvalue_reference final {
  typedef base type;
};

template <typename base>
struct remove_lvalue_reference<base&> final {
  typedef base type;
};

// ... --> remove_pointer<T>, std::remove_pointer<T> ->> Aliases `T` without pointer-qualification
template <typename base>
struct remove_pointer final {
  typedef base type;
};

template <typename base>
struct remove_pointer<base*> final {
  typedef base type;
};

template <typename base>
struct remove_pointer<base* const> final {
  typedef base type;
};

template <typename base>
struct remove_pointer<base* const volatile> final {
  typedef base type;
};

template <typename base>
struct remove_pointer<base* volatile> final {
  typedef base type;
};

// ... --> remove_rvalue_reference<T>, std::remove_rvalue_reference<T> ->> Aliases `T` without rvalue-categorized reference-qualification
template <typename base>
struct remove_rvalue_reference final {
  typedef base type;
};

template <typename base>
struct remove_rvalue_reference<base&&> final {
  typedef base type;
};

// ... --> remove_volatile<T>, std::remove_volatile<T> ->> Aliases `T` without `volatile`-qualification
template <typename base>
struct remove_volatile final {
  typedef base type;
};

template <typename base>
struct remove_volatile<base volatile> final {
  typedef base type;
};


// ... --> remove_cv<T>, remove_qualifiers<T>, remove_reference<T>, std::remove_cv<T>, std::remove_reference<T> ->> Aliases `T` without specific qualifications
template <typename base>
struct remove_cv final {
  typedef typename remove_const<typename remove_volatile<base>::type>::type type;
};

template <typename base>
struct remove_reference final {
  typedef typename remove_lvalue_reference<typename remove_rvalue_reference<base>::type>::type type;
};

template <typename base>
struct remove_qualifiers final {
  typedef typename remove_cv<typename remove_reference<base>::type>::type type;
};

// ... --> addressof(...) ->> Evaluates the address of a given object
template <typename type>
inline typename conditional<(
  opinfo::has_overloaded_function<opinfo::address, type&&>::value or
  opinfo::has_overloaded_member  <opinfo::address, type&&>::value
), typename remove_reference<type>::type*>::type addressof(type&& object) noexcept {
  return reinterpret_cast<typename remove_reference<type>::type*>(const_cast<unsigned char*>(&reinterpret_cast<unsigned char const volatile&>(object)));
}

template <typename type>
constexpr typename conditional<opinfo::can<opinfo::address, type&&>::value and not (
  opinfo::has_overloaded_function<opinfo::address, type&&>::value or
  opinfo::has_overloaded_member  <opinfo::address, type&&>::value
), typename remove_reference<type>::type*>::type addressof(type&& object) noexcept {
  return &forward_cast<type>(object);
}

template <typename type>
constexpr typename conditional<not opinfo::can<opinfo::address, type&&>::value and not (
  opinfo::has_overloaded_function<opinfo::address, type&&>::value or
  opinfo::has_overloaded_member  <opinfo::address, type&&>::value
), typename remove_reference<type>::type*>::type addressof(type&& object) noexcept {
  return &object;
}

/* Shorthand */
#if defined __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
# pragma clang diagnostic ignored "-Wunused-const-variable"
#elif defined __GNUC__
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wignored-attributes"
# pragma GCC diagnostic ignored "-Wunused-const-variable"
#elif defined _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4131)
# pragma warning(disable: 4584)
#endif

enum $$ : unsigned char {
  $c = 0x01u, // ->> Constant  - Allows compile-time evaluation of constructed `$n` shorthands
  $o = 0x02u, // ->> Object    - Allows constructor elision for objects e.g.: `$1(...) == $n<1u>(...)`
  $r = 0x04u  // ->> Reference - Allows constructor elision for lvalue references
};

// ... ->> Trait for shorthand formats e.g. `$c`, `$o`, and `$r`
template <$$ = $c, $$...>
struct $f;
  // ->> Shortened tokens for convenience
  typedef $f<$c, $o>     $fco;
  typedef $f<$c, $r>     $fcr;
  typedef $f<$o, $c>     $foc;
  typedef $f<$o, $r>     $for;
  typedef $f<$r, $c>     $frc;
  typedef $f<$r, $o>     $fro;
  typedef $f<$c, $o, $r> $fcor;
  typedef $f<$c, $r, $o> $fcro;
  typedef $f<$o, $c, $r> $focr;
  typedef $f<$o, $r, $c> $forc;
  typedef $f<$r, $c, $o> $frco;
  typedef $f<$r, $o, $c> $froc;

// ... ->> Lambda expression shorthand: Composited type for operations on other shorthands
template <opinfo::op, class...>
struct $lambda;

// ... ->> Index shorthand: Unit type for shorthands
template <
  std::size_t,           //
  class          = $for, // ->> Allow for object elision and deference to lvalue references when eliding by default
  typename... /* = $s */ // ->> List of types to implicitly handle within the shorthand
> struct $n;

// ... ->> Object shorthand: Unit type for non-shorthands operating with index shorthands
template <typename>
struct $object;

// ... ->> Default types also implicitly handled by shorthands, in this case: freestanding-implementation standard library types
#ifdef $s
# error Unexpected macro definition: `$s`
#else
# define $s typename $shorthand::types::align_val_t, typename $shorthand::types::allocator_arg_t, typename $shorthand::types::atomic_bool, typename $shorthand::types::atomic_char8_t, typename $shorthand::types::atomic_char16_t, typename $shorthand::types::atomic_char32_t, typename $shorthand::types::atomic_char, typename $shorthand::types::atomic_flag, typename $shorthand::types::atomic_int, typename $shorthand::types::atomic_llong, typename $shorthand::types::atomic_long, typename $shorthand::types::atomic_schar, typename $shorthand::types::atomic_short, typename $shorthand::types::atomic_uchar, typename $shorthand::types::atomic_uint, typename $shorthand::types::atomic_ullong, typename $shorthand::types::atomic_ulong, typename $shorthand::types::atomic_ushort, typename $shorthand::types::atomic_wchar_t, typename $shorthand::types::atto, typename $shorthand::types::bad_alloc, typename $shorthand::types::bad_array_new_length, typename $shorthand::types::bad_cast, typename $shorthand::types::bad_exception, typename $shorthand::types::bad_function_call, typename $shorthand::types::bad_typeid, typename $shorthand::types::bad_weak_ptr, typename $shorthand::types::bfloat16_t, typename $shorthand::types::bidirectional_iterator_tag, typename $shorthand::types::bit_and, typename $shorthand::types::bit_not, typename $shorthand::types::bit_or, typename $shorthand::types::bit_xor, typename $shorthand::types::byte, typename $shorthand::types::centi, typename $shorthand::types::compare_three_way, typename $shorthand::types::contiguous_iterator_tag, typename $shorthand::types::coroutine_handle, typename $shorthand::types::dangling, typename $shorthand::types::deca, typename $shorthand::types::deci, typename $shorthand::types::default_sentinel_t, typename $shorthand::types::destroying_delete_t, typename $shorthand::types::div_t, typename $shorthand::types::divides, typename $shorthand::types::endian, typename $shorthand::types::equal_to, typename $shorthand::types::exa, typename $shorthand::types::exception, typename $shorthand::types::exception_ptr, typename $shorthand::types::false_type, typename $shorthand::types::femto, typename $shorthand::types::float16_t, typename $shorthand::types::float32_t, typename $shorthand::types::float64_t, typename $shorthand::types::float128_t, typename $shorthand::types::format_args, typename $shorthand::types::forward_iterator_tag, typename $shorthand::types::giga, typename $shorthand::types::greater, typename $shorthand::types::greater_equal, typename $shorthand::types::hecto, typename $shorthand::types::identity, typename $shorthand::types::in_place_t, typename $shorthand::types::input_iterator_tag, typename $shorthand::types::ispanstream, typename $shorthand::types::kilo, typename $shorthand::types::ldiv_t, typename $shorthand::types::less, typename $shorthand::types::less_equal, typename $shorthand::types::lldiv_t, typename $shorthand::types::logical_and, typename $shorthand::types::logical_not, typename $shorthand::types::logical_or, typename $shorthand::types::max_align_t, typename $shorthand::types::mega, typename $shorthand::types::memory_order, typename $shorthand::types::micro, typename $shorthand::types::milli, typename $shorthand::types::minus, typename $shorthand::types::modulus, typename $shorthand::types::multiplies, typename $shorthand::types::nano, typename $shorthand::types::negate, typename $shorthand::types::nested_exception, typename $shorthand::types::new_terminate_unexpected_handler, typename $shorthand::types::noop_coroutine_handle, typename $shorthand::types::noop_coroutine_promise, typename $shorthand::types::not_equal_to, typename $shorthand::types::nothrow_t, typename $shorthand::types::nullptr_t, typename $shorthand::types::ospanstream, typename $shorthand::types::output_iterator_tag, typename $shorthand::types::owner_less, typename $shorthand::types::partial_ordering, typename $shorthand::types::peta, typename $shorthand::types::pico, typename $shorthand::types::piecewise_construct_t, typename $shorthand::types::placeholders_1, typename $shorthand::types::plus, typename $shorthand::types::pointer_safety, typename $shorthand::types::random_access_iterator_tag, typename $shorthand::types::source_location, typename $shorthand::types::spanbuf, typename $shorthand::types::spanstream, typename $shorthand::types::strong_ordering, typename $shorthand::types::subrange_kind, typename $shorthand::types::suspend_always, typename $shorthand::types::suspend_never, typename $shorthand::types::tera, typename $shorthand::types::true_type, typename $shorthand::types::type_info, typename $shorthand::types::unexpect_t, typename $shorthand::types::unreachable_sentinel_t, typename $shorthand::types::va_listA, typename $shorthand::types::va_listB, typename $shorthand::types::va_listC, typename $shorthand::types::va_listD, typename $shorthand::types::weak_ordering, typename $shorthand::types::wformat_args, typename $shorthand::types::wispanstream, typename $shorthand::types::wospanstream, typename $shorthand::types::wspanbuf, typename $shorthand::types::wspanstream
#endif

// ...
struct $shorthand final {
  template <opinfo::op, class...>            friend struct $lambda;
  template <std::size_t, class, typename...> friend struct $n;

  public:
    struct fallback final {
      struct types { typedef null align_val_t, allocator_arg_t, atomic_bool, atomic_char8_t, atomic_char16_t, atomic_char32_t, atomic_char, atomic_flag, atomic_int, atomic_llong, atomic_long, atomic_schar, atomic_short, atomic_uchar, atomic_uint, atomic_ullong, atomic_ulong, atomic_ushort, atomic_wchar_t, atto, bad_alloc, bad_array_new_length, bad_cast, bad_exception, bad_function_call, bad_typeid, bad_weak_ptr, bfloat16_t, bidirectional_iterator_tag, bit_and, bit_not, bit_or, bit_xor, byte, centi, compare_three_way, contiguous_iterator_tag, coroutine_handle, dangling, deca, deci, default_sentinel_t, destroying_delete_t, div_t, divides, endian, equal_to, exa, exception, exception_ptr, false_type, femto, float16_t, float32_t, float64_t, float128_t, format_args, forward_iterator_tag, giga, greater, greater_equal, hecto, identity, in_place_t, input_iterator_tag, ispanstream, kilo, ldiv_t, less, less_equal, lldiv_t, logical_and, logical_not, logical_or, max_align_t, mega, memory_order, micro, milli, minus, modulus, multiplies, nano, negate, nested_exception, new_terminate_unexpected_handler, noop_coroutine_handle, noop_coroutine_promise, not_equal_to, nothrow_t, nullptr_t, ospanstream, output_iterator_tag, owner_less, partial_ordering, peta, pico, piecewise_construct_t, placeholders_1, plus, pointer_safety, random_access_iterator_tag, source_location, spanbuf, spanstream, strong_ordering, subrange_kind, suspend_always, suspend_never, tera, true_type, type_info, unexpect_t, unreachable_sentinel_t, va_listA, va_listB, va_listC, va_listD, weak_ordering, wformat_args, wispanstream, wospanstream, wspanbuf, wspanstream; };
    };

    // ... ->> See `$s` macro
    struct types final : public $shorthand::fallback::types {
      typedef std::allocator_arg_t            allocator_arg_t;
      typedef std::bad_alloc                  bad_alloc;
      typedef std::bad_array_new_length       bad_array_new_length;
      typedef std::bad_cast                   bad_cast;
      typedef std::bad_exception              bad_exception;
      typedef std::bad_function_call          bad_function_call;
      typedef std::bad_typeid                 bad_typeid;
      typedef std::bad_weak_ptr               bad_weak_ptr;
      typedef std::bidirectional_iterator_tag bidirectional_iterator_tag;
      typedef std::div_t                      div_t;
      typedef std::exception                  exception;
      typedef std::exception_ptr              exception_ptr;
      typedef std::forward_iterator_tag       forward_iterator_tag;
      typedef std::input_iterator_tag         input_iterator_tag;
      typedef std::ldiv_t                     ldiv_t;
      typedef std::lldiv_t                    lldiv_t;
      typedef std::max_align_t                max_align_t;
      typedef std::memory_order               memory_order;
      typedef std::nested_exception           nested_exception;
      typedef std::new_handler                new_terminate_unexpected_handler;
      typedef std::nothrow_t                  nothrow_t;
      typedef std::nullptr_t                  nullptr_t;
      typedef std::output_iterator_tag        output_iterator_tag;
      typedef std::owner_less<void>           owner_less;
      typedef std::piecewise_construct_t      piecewise_construct_t;
      typedef decltype(std::placeholders::_1) placeholders_1;
      typedef std::random_access_iterator_tag random_access_iterator_tag;
      typedef std::type_info const           &type_info;
      typedef std::va_list                   *va_listA;
      typedef std::va_list const             *va_listB;
      typedef std::va_list const volatile    *va_listC;
      typedef std::va_list const volatile    *va_listD;
      #ifdef __cpp_aligned_new // --> 201606L
        typedef std::align_val_t align_val_t;
      #endif
      #ifdef __cpp_char8_t // --> 201811L
        typedef std::atomic_char8_t atomic_char8_t;
      #endif
      #ifdef __cpp_lib_byte // --> 201603L
        typedef std::byte byte;
      #endif
      #ifdef __cpp_lib_endian // --> 201907L
        typedef std::endian endian;
      #endif
      #ifdef __cpp_lib_ranges // --> 201911L
        typedef std::ranges::dangling      dangling;
        typedef std::ranges::subrange_kind subrange_kind;
      #endif
      #ifdef __cpp_lib_source_location // --> 201907L
        typedef std::source_location source_location;
      #endif
      #ifdef __cpp_lib_spanstream // --> 202106L
        typedef std::ispanstream  ispanstream;
        typedef std::ospanstream  ospanstream;
        typedef std::spanbuf      spanbuf;
        typedef std::spanstream   spanstream;
        typedef std::wispanstream wispanstream;
        typedef std::wospanstream wospanstream;
        typedef std::wspanbuf     wspanbuf;
        typedef std::wspanstream  wspanstream;
      #endif
      #ifdef __cpp_lib_three_way_comparison // --> 201907L
        typedef std::compare_three_way compare_three_way;
        typedef std::partial_ordering  partial_ordering;
        typedef std::strong_ordering   strong_ordering;
        typedef std::weak_ordering     weak_ordering;
        #ifdef __cpp_lib_coroutine // --> 201902L
          typedef std::coroutine_handle<void> coroutine_handle;
          typedef std::noop_coroutine_handle  noop_coroutine_handle;
          typedef std::noop_coroutine_promise noop_coroutine_promise;
          typedef std::suspend_always         suspend_always;
          typedef std::suspend_never          suspend_never;
        #endif
      #endif
      #ifdef __cpp_unicode_characters // --> 200704L
        typedef std::atomic_char16_t atomic_char16_t;
        typedef std::atomic_char32_t atomic_char32_t;
      #endif
      #if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) >= 201103L
        typedef std::atomic_bool                 atomic_bool;
        typedef std::atomic_char                 atomic_char;
        typedef std::atomic_flag                *atomic_flagA;
        typedef std::atomic_flag const          *atomic_flagB;
        typedef std::atomic_flag const volatile *atomic_flagC;
        typedef std::atomic_flag       volatile *atomic_flagD;
        typedef std::atomic_int                  atomic_int;
        typedef std::atomic_llong                atomic_llong;
        typedef std::atomic_long                 atomic_long;
        typedef std::atomic_schar                atomic_schar;
        typedef std::atomic_short                atomic_short;
        typedef std::atomic_uchar                atomic_uchar;
        typedef std::atomic_uint                 atomic_uint;
        typedef std::atomic_ullong               atomic_ullong;
        typedef std::atomic_ulong                atomic_ulong;
        typedef std::atomic_ushort               atomic_ushort;
        typedef std::atomic_wchar_t              atomic_wchar_t;
        typedef std::atto                        atto;
        typedef std::centi                       centi;
        typedef std::deca                        deca;
        typedef std::deci                        deci;
        typedef std::exa                         exa;
        typedef std::false_type                  false_type;
        typedef std::femto                       femto;
        typedef std::giga                        giga;
        typedef std::hecto                       hecto;
        typedef std::kilo                        kilo;
        typedef std::mega                        mega;
        typedef std::micro                       micro;
        typedef std::milli                       milli;
        typedef std::nano                        nano;
        typedef std::peta                        peta;
        typedef std::pico                        pico;
        typedef std::tera                        tera;
        typedef std::true_type                   true_type;
      #endif
      #if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) >= 201402L
        typedef std::bit_and      <void> bit_and;
        typedef std::bit_not      <>     bit_not;
        typedef std::bit_or       <void> bit_or;
        typedef std::bit_xor      <void> bit_xor;
        typedef std::divides      <void> divides;
        typedef std::equal_to     <void> equal_to;
        typedef std::greater      <void> greater;
        typedef std::greater_equal<void> greater_equal;
        typedef std::less         <void> less;
        typedef std::less_equal   <void> less_equal;
        typedef std::logical_and  <void> logical_and;
        typedef std::logical_not  <void> logical_not;
        typedef std::logical_or   <void> logical_or;
        typedef std::minus        <void> minus;
        typedef std::modulus      <void> modulus;
        typedef std::multiplies   <void> multiplies;
        typedef std::negate       <void> negate;
        typedef std::not_equal_to <void> not_equal_to;
        typedef std::plus         <void> plus;
      #endif
      #if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) >= 201703L
        typedef std::in_place_t in_place_t;
      #endif
      #if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) >= 202002L
        typedef std::contiguous_iterator_tag contiguous_iterator_tag;
        typedef std::default_sentinel_t      default_sentinel_t;
        typedef std::destroying_delete_t     destroying_delete_t;
        typedef std::format_args             format_args;
        typedef std::identity                identity;
        typedef std::unreachable_sentinel_t  unreachable_sentinel_t;
        typedef std::wformat_args            wformat_args;
      #endif
      #if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) > 202002L
        typedef std::unexpect_t unexpect_t;
      #endif
      #if (defined _MSVC_LANG ? _MSVC_LANG : __cplusplus) <= 202002L // --> < 2023..L
        typedef std::pointer_safety pointer_safety;
      #endif
      #if defined __STDCPP_BFLOAT16_T__
        typedef std::bfloat16_t bfloat16_t;
      #endif
      #if defined __STDCPP_FLOAT16_T__
        typedef std::float16_t float16_t;
      #endif
      #if defined __STDCPP_FLOAT32_T__
        typedef std::float32_t float32_t;
      #endif
      #if defined __STDCPP_FLOAT64_T__
        typedef std::float64_t float64_t;
      #endif
      #if defined __STDCPP_FLOAT128_T__
        typedef std::float128_t float128_t;
      #endif
    };

  private:
    static std::size_t const alignment = commonof::alignment<bool, long double, sfinaeptr_t $shorthand::*, unsigned long long, void*, void (*)(...)>::value;
    static std::size_t const size      = commonof::size     <bool, long double, sfinaeptr_t $shorthand::*, unsigned long long, void*, void (*)(...)>::value;

    enum : bool {
      destruct   = false,
      nodestruct = true
    };

    // ... ->> Context information for shorthand formats
    template <bool, $$...>
    struct format;

    // ... --> is_shorthand<T> ->> Determines if `T` is a `$shorthand` type
    template <class>
    struct is_shorthand final {
      static bool const value = false;
    };

    template <typename base>
    struct is_shorthand<$object<base> > final {
      static bool const value = true;
    };

    template <opinfo::op operation, class... shorthands>
    struct is_shorthand<$lambda<operation, shorthands...> > final {
      static bool const value = true;
    };

    template <std::size_t arity, $$... states, typename... extensions>
    struct is_shorthand<$n<arity, $f<states...>, extensions...> > final {
      static bool const value = true;
    };

    // ... ->> Container for object pointer or referable `$shorthand::value` object
    struct referrable final {
      union {
        void *const undefined;

        bool               *const value_bool;
        char               *const value_char;
        wchar_t            *const value_wchar_t;
        double             *const value_double;
        float              *const value_float;
        long double        *const value_long_double;
        int                *const value_int;
        long               *const value_long;
        long long          *const value_long_long;
        short              *const value_short;
        signed char        *const value_signed_char;
        unsigned char      *const value_unsigned_char;
        unsigned int       *const value_unsigned_int;
        unsigned long      *const value_unsigned_long;
        unsigned long long *const value_unsigned_long_long;
        unsigned short     *const value_unsigned_short;
        #ifdef __cpp_unicode_characters // --> 200704L
          char16_t *const value_char16_t;
          char32_t *const value_char32_t;
        #else
          decltype(&instanceof<null>()) (*const value_char16_t)[1];
          decltype(&instanceof<null>()) (*const value_char32_t)[2];
        #endif
        #ifdef __cpp_char8_t // --> 201811L
          char8_t *const value_char8_t;
        #else
          decltype(&instanceof<null>()) (*const value_char8_t)[3];
        #endif
        #if defined __clang__
          __int128_t  *const value_int128_t;
          __uint128_t *const value_uint128_t;
        #elif defined __GNUC__
        # ifdef __SIZEOF_INT128__
        #   pragma GCC diagnostic push
        #   pragma GCC diagnostic ignored "-Wpedantic"
          __int128          *const value_int128_t;
          unsigned __int128 *const value_uint128_t;
        #   pragma GCC diagnostic pop
        # else
          decltype(&instanceof<null>()) (*const value_int128_t) [4];
          decltype(&instanceof<null>()) (*const value_uint128_t)[5];
        # endif
        #elif defined __cpp_lib_ranges // --> 201911L
          std::ranges::range_difference_t<std::ranges::iota_view<long long,          long long> >          *const value_int128_t;
          std::ranges::range_difference_t<std::ranges::iota_view<unsigned long long, unsigned long long> > *const value_uint128_t;
        #else
          decltype(&instanceof<null>()) (*const value_int128_t) [4];
          decltype(&instanceof<null>()) (*const value_uint128_t)[5];
        #endif
      };

      // ...
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, bool>                                                               ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_bool              (const_cast<bool*>                               (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, char>                                                               ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_char              (const_cast<char*>                               (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, double>                                                             ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_double            (const_cast<double*>                             (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, float>                                                              ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_float             (const_cast<float*>                              (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, int>                                                                ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_int               (const_cast<int*>                                (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, long>                                                               ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_long              (const_cast<long*>                               (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, long double>                                                        ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_long_double       (const_cast<long double*>                        (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, long long>                                                          ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_long_long         (const_cast<long long*>                          (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, short>                                                              ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_short             (const_cast<short*>                              (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, signed char>                                                        ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_signed_char       (const_cast<signed char*>                        (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, unsigned char>                                                      ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_unsigned_char     (const_cast<unsigned char*>                      (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, unsigned int>                                                       ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_unsigned_int      (const_cast<unsigned int*>                       (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, unsigned long>                                                      ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_unsigned_long     (const_cast<unsigned long*>                      (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, unsigned long long>                                                 ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_unsigned_long_long(const_cast<unsigned long long*>                 (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, unsigned short>                                                     ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_unsigned_short    (const_cast<unsigned short*>                     (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, wchar_t>                                                            ::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_wchar_t           (const_cast<wchar_t*>                            (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, typename remove_pointer<decltype(reference::value_char8_t)>  ::type>::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_char8_t           (const_cast<decltype(reference::value_char8_t)>  (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, typename remove_pointer<decltype(reference::value_char16_t)> ::type>::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_char16_t          (const_cast<decltype(reference::value_char16_t)> (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, typename remove_pointer<decltype(reference::value_char32_t)> ::type>::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_char32_t          (const_cast<decltype(reference::value_char32_t)> (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, typename remove_pointer<decltype(reference::value_int128_t)> ::type>::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_int128_t          (const_cast<decltype(reference::value_int128_t)> (reference)) {}
      template <typename type> constexpr referrable(type* const reference, typename conditional<is_same<typename remove_cv<type>::type, typename remove_pointer<decltype(reference::value_uint128_t)>::type>::value, sfinaeptr_t>::type = sfinaeptr) noexcept : value_uint128_t         (const_cast<decltype(reference::value_uint128_t)>(reference)) {}

      template <typename type>
      constexpr referrable(type* const reference, typename conditional<not (
        is_same<typename remove_cv<type>::type, bool>                                                               ::value or
        is_same<typename remove_cv<type>::type, char>                                                               ::value or
        is_same<typename remove_cv<type>::type, double>                                                             ::value or
        is_same<typename remove_cv<type>::type, float>                                                              ::value or
        is_same<typename remove_cv<type>::type, int>                                                                ::value or
        is_same<typename remove_cv<type>::type, long>                                                               ::value or
        is_same<typename remove_cv<type>::type, long double>                                                        ::value or
        is_same<typename remove_cv<type>::type, long long>                                                          ::value or
        is_same<typename remove_cv<type>::type, short>                                                              ::value or
        is_same<typename remove_cv<type>::type, signed char>                                                        ::value or
        is_same<typename remove_cv<type>::type, unsigned char>                                                      ::value or
        is_same<typename remove_cv<type>::type, unsigned int>                                                       ::value or
        is_same<typename remove_cv<type>::type, unsigned long>                                                      ::value or
        is_same<typename remove_cv<type>::type, unsigned long long>                                                 ::value or
        is_same<typename remove_cv<type>::type, unsigned short>                                                     ::value or
        is_same<typename remove_cv<type>::type, wchar_t>                                                            ::value or
        is_same<typename remove_cv<type>::type, typename remove_pointer<decltype(reference::value_char8_t)>  ::type>::value or
        is_same<typename remove_cv<type>::type, typename remove_pointer<decltype(reference::value_char16_t)> ::type>::value or
        is_same<typename remove_cv<type>::type, typename remove_pointer<decltype(reference::value_char32_t)> ::type>::value or
        is_same<typename remove_cv<type>::type, typename remove_pointer<decltype(reference::value_int128_t)> ::type>::value or
        is_same<typename remove_cv<type>::type, typename remove_pointer<decltype(reference::value_uint128_t)>::type>::value
      ), sfinaeptr_t>::type = sfinaeptr) noexcept :
        undefined(const_cast<void*>(static_cast<void const volatile*>(reference)))
      {}

      /* ... */
      // ... ->> Enumeration for tagging trivial non-class type references
      enum type : unsigned char {
        array = 0x01u,
        boolean,
        character_char,
        character_char8_t,
        character_char16_t,
        character_char32_t,
        character_wchar_t,
        decimal_double,
        decimal_float,
        decimal_long_double,
        integer_int,
        integer_int128_t,
        integer_long,
        integer_long_long,
        integer_short,
        integer_signed_char,
        integer_uint128_t,
        integer_unsigned_char,
        integer_unsigned_int,
        integer_unsigned_long,
        integer_unsigned_long_long,
        integer_unsigned_short
      };

      // ... --> kindof<T> ->> Evaluates the shorthand enumerator tag for elidable types
      template <typename base, std::size_t = alignment, std::size_t = size>
      struct kindof final {
        static type const value = (
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, bool>                                  ::value ? $shorthand::reference::boolean                    :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, char>                                  ::value ? $shorthand::reference::character_char             :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, double>                                ::value ? $shorthand::reference::decimal_double             :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, float>                                 ::value ? $shorthand::reference::decimal_float              :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, int>                                   ::value ? $shorthand::reference::integer_int                :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, long>                                  ::value ? $shorthand::reference::integer_long               :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, long double>                           ::value ? $shorthand::reference::decimal_long_double        :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, long long>                             ::value ? $shorthand::reference::integer_long_long          :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, short>                                 ::value ? $shorthand::reference::integer_short              :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, signed char>                           ::value ? $shorthand::reference::integer_signed_char        :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, unsigned char>                         ::value ? $shorthand::reference::integer_unsigned_char      :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, unsigned int>                          ::value ? $shorthand::reference::integer_unsigned_int       :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, unsigned long>                         ::value ? $shorthand::reference::integer_unsigned_long      :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, unsigned long long>                    ::value ? $shorthand::reference::integer_unsigned_long_long :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, unsigned short>                        ::value ? $shorthand::reference::integer_unsigned_short     :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, wchar_t>                               ::value ? $shorthand::reference::character_wchar_t          :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, decltype(reference::integer_int128_t)> ::value ? $shorthand::reference::integer_int128_t           :
          is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, decltype(reference::integer_uint128_t)>::value ? $shorthand::reference::integer_uint128_t          :
          #ifdef __cpp_unicode_characters // --> 200704L
            is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, char16_t>::value ? $shorthand::reference::character_char16_t :
            is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, char32_t>::value ? $shorthand::reference::character_char32_t :
          #endif
          #ifdef __cpp_char8_t // --> 201811L
            is_same<typename remove_pointer<typename remove_qualifiers<base>::type>::type, char8_t>::value ? $shorthand::reference::character_char8_t :
          #endif
          static_cast<type>(0x00u) // --> $shorthand::null
        );
      };
    };

    // ... ->> Enumeration for tagging trivial non-class type elidable objects
    enum type : std::size_t {
      null = 0x00u,                                                                              // ->> Flag for default `$shorthand::value` or un-elidable object
      decimal,                                                                                   // ->> Flag for elidable floating-point numbers
      function,                                                                                  // ->> Flag for elidable function pointers
      integer,                                                                                   // ->> Flag for elidable booleans, characters, and integer numbers
      member,                                                                                    // ->> Flag for elidable data member pointers
      shorthand,                                                                                 // ->> Flag for shorthands such as `$lambda`, `$n`, or `$object`
      undefined,                                                                                 // ->> Flag for elidable type-ambiguous objects that align and can be stored within `$n::value`
                                                                                                 //
      extended        = static_cast<std::size_t>(1u) << ((CHAR_BIT * sizeof(std::size_t)) - 1u), // ->> Extensible flag for other possible enumerator tags
      pointer         = static_cast<std::size_t>(1u) << ((CHAR_BIT * sizeof(std::size_t)) - 2u), // ->> Flag for elidable object pointers
      reference       = static_cast<std::size_t>(1u) << ((CHAR_BIT * sizeof(std::size_t)) - 3u), // ->> Flag for elidable references

      reference_array    = reference | $shorthand::reference::array,
      reference_extended = reference | extended
    };

    // ... ->> Container for non-reconstructible `$shorthand::value` object
    template <std::size_t alignment = $shorthand::alignment, std::size_t capacity /* --> size */ = $shorthand::size>
    struct unknown final {
      template <typename base>
      struct handlerof final {
        static void* value(opinfo::op const operation, void* const destination, void* const source) {
          switch (operation) {
            case opinfo::construct: return (void*) ::new (destination) base(*static_cast<base*>(source));
            case opinfo::destruct : static_cast<base*>(destination) -> ~base(); return NULL;
            default: return NULL;
          }
        }
      };

      /* ... */
      alignas(alignment) unsigned char value[capacity];
      void*                    (*const handler)(opinfo::op, void*, void*);
      void                      *const address;
      std::size_t                const size;
      std::size_t                const alignment;
    };

    // ... --> kindof<T> ->> Evaluates the shorthand enumerator tag for elidable types
    template <typename base, std::size_t alignment = $shorthand::alignment, std::size_t size = $shorthand::size>
    struct kindof final {
      typedef typename conditional<opinfo::can<opinfo::alignof_object, base>::value or opinfo::can<opinfo::sizeof_object, base>::value, base, sfinaeptr_t>::type subbase;
      static type const value = (
        is_shorthand       <typename remove_qualifiers<base>::type>                               ::value ? $shorthand::shorthand :
        is_function        <typename remove_pointer<typename remove_reference<base>::type>::type> ::value ? $shorthand::function  :
        is_lvalue_reference<base>::value or is_array<typename remove_rvalue_reference<base>::type>::value ? $shorthand::reference :
        is_boolean         <typename remove_qualifiers<base>::type>                               ::value ? $shorthand::integer   :
        is_character       <typename remove_qualifiers<base>::type>                               ::value ? $shorthand::integer   :
        is_decimal         <typename remove_qualifiers<base>::type>                               ::value ? $shorthand::decimal   :
        is_enum            <typename remove_qualifiers<base>::type>                               ::value ? $shorthand::integer   :
        is_integer         <typename remove_qualifiers<base>::type>                               ::value ? $shorthand::integer   :
        is_member_pointer  <typename remove_qualifiers<base>::type>                               ::value ? $shorthand::member    :
        is_pointer         <typename remove_qualifiers<base>::type>                               ::value ? $shorthand::pointer   :

        opinfo::can     <opinfo::copy, typename remove_qualifiers<base>::type>::value and                                                               // ->> Elide copy-constructible objects
        std::is_trivially_destructible<typename remove_qualifiers<base>::type>::value and                                                               // ->> Elide trivially-destructible objects
        (is_rvalue_reference<base>::value or not is_reference<base>::value)           and                                                               // ->> Elide temporary objects
        (alignof(subbase) <= alignof(decltype(unknown<alignment, size>::value))                                                                         // ->> `base` can be properly aligned within `unknown::value`
          ? sizeof(subbase) <= sizeof unknown<alignment, size>::value and 0u == (alignof(decltype(unknown<alignment, size>::value)) % alignof(subbase)) // ->> `unknown::value` can store and is aligned with `base`
          : sizeof(subbase) <= sizeof unknown<alignment, size>::value -         (alignof(subbase) - alignof(decltype(unknown<alignment, size>::value))) // ->> `unknown::value` can store `base` along with its alignment i.e.: `alignof(base)`
        ) ? $shorthand::undefined :

        $shorthand::null
      );
    };

    // ... --> valueinfo<arity, type>
    template <std::size_t alignment = $shorthand::alignment, std::size_t size = $shorthand::size, std::size_t...>
    struct valueinfo final {
      union {                                                                      //
        std::size_t           capacity;                                            // ->> Track bounds for type-unambiguous reference-to-arrays
        std::type_info const *metadata;                                            // ->> Track type information for type-ambiguous objects and reference-to-arrays
      };                                                                           //
                                                                                   //
      typename conditional<size <= sizeof(unsigned char),      unsigned char,      // ->> Track type information for
      typename conditional<size <= sizeof(unsigned short),     unsigned short,     //       data member,
      typename conditional<size <= sizeof(unsigned int),       unsigned int,       //       floating-point, function,
      typename conditional<size <= sizeof(unsigned long),      unsigned long,      //       integer,
      typename conditional<size <= sizeof(unsigned long long), unsigned long long, //       object pointer,
        std::size_t                                                                //       reference, and
      >::type>::type>::type>::type>::type type;                                    //       type-ambiguous types

      /* ... */
      constexpr valueinfo() noexcept :
        valueinfo::valueinfo(NULL, $shorthand::null)
      {}

      constexpr valueinfo(std::size_t const capacity, type const type) noexcept :
        capacity(capacity),
        type    (type)
      {}

      constexpr valueinfo(std::type_info const* const metadata, type const type) noexcept :
        metadata(metadata),
        type    (type)
      {}
    };

    // ... ->> Container for elidable objects
    template <std::size_t alignment = $shorthand::alignment, std::size_t size = $shorthand::size>
    struct value final {
      typedef valueinfo<alignment, size> memberinfo;

      /* ... */
      constexpr static struct /* final */ {
        mutable memberinfo value;
        constexpr operator memberinfo&() const noexcept {
          return this -> value;
        }
      } dummy = {};

      union {
        #if defined __clang__                                     // ->> Elide boolean, character, enumeration, and integer objects
          __uint128_t integer;                                    //
        #elif defined __GNUC__                                    //
        # ifdef __SIZEOF_INT128__                                 //
        #   pragma GCC diagnostic push                            //
        #   pragma GCC diagnostic ignored "-Wpedantic"            //
            unsigned __int128 integer;                            //
        #   pragma GCC diagnostic pop                             //
        # else                                                    //
          unsigned long long integer;                             //
        # endif                                                   //
        #elif defined __cpp_lib_ranges // --> 201911L             //
          std::ranges::range_difference_t<std::ranges::iota_view< //
            unsigned long long,                                   //
            unsigned long long                                    //
          > > integer;                                            //
        #else                                                     //
          unsigned long long integer;                             //
        #endif                                                    //
        long double                               decimal;        // ->> Elide floating-point      objects
        void                                    (*function)(...); // ->> Elide function    pointer objects
        sfinaeptr_t $shorthand::                 *member;         // ->> Elide data member pointer objects
        sfinaeptr_t                               null;           // ->> Default-/ nil-constructed `$shorthand::value` or un-elidable object
        struct referrable                         pointer;        // ->> Elide object pointer objects
        struct referrable                         reference;      // ->> Elide references
        struct unknown<alignment, size>           undefined;      // ->> Elide type-ambiguous objects
      };                                                          //

      /* ... */
      #ifdef __GNUC__
      # pragma GCC diagnostic push
      # pragma GCC diagnostic ignored "-Wpedantic"
      #endif
        template <std::size_t arity, typename type, typename conditional<(
          is_same<typename remove_qualifiers<type>::type, sfinaeptr_t>                             ::value or
          is_same<typename remove_qualifiers<type>::type, typename remove_qualifiers<::null>::type>::value
        ), sfinaeptr_t>::type = sfinaeptr> // ERROR (Lapys) -> Elide `null` or `sfinaeptr_t` objects
        constexpr value(memberinfo&, valueinfo<arity> const, type&&) noexcept :
          value::value(dummy, valueinfo<arity, $shorthand::null>(), sfinaeptr)
        { static_assert(arity != arity, "Unable to construct shorthand with argument(s)"); }

        template <std::size_t arity, typename type, typename conditional<not (
          is_same<typename remove_qualifiers<type>::type, sfinaeptr_t>                             ::value or
          is_same<typename remove_qualifiers<type>::type, typename remove_qualifiers<::null>::type>::value
        ), sfinaeptr_t>::type = sfinaeptr> // NOTE (Lapys) -> Defer elision of object
        constexpr value(memberinfo& information, valueinfo<arity> const, type&& value) noexcept(noexcept($shorthand::value<alignment, size>(
          instanceof<memberinfo&>                                              (),
          instanceof<valueinfo<arity, kindof<type&&, alignment, size>::value> >(),
          instanceof<type&&>                                                   ()
        ))) :
          value::value(
            information,
            valueinfo<arity, kindof<type&&, alignment, size>::value>(),
            forward_cast<type>(value)
          )
        { static_assert(kindof<type&&, alignment, size>::value != $shorthand::null, "Unable to construct shorthand with argument(s)"); }

        template <std::size_t arity> // ->> Elide floating-point objects
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::decimal> const, long double const value) noexcept :
          decimal((static_cast<void>(information = memberinfo(
            static_cast<std::type_info const*>(NULL),
            $shorthand::decimal
          )), value))
        {}

        template <std::size_t arity> // ->> Elide function pointer objects
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::function> const, decltype(value::function) const value) noexcept :
          function((static_cast<void>(information = memberinfo(
            static_cast<std::type_info const*>(NULL),
            $shorthand::function
          )), value))
        {}

        template <std::size_t arity, typename type> // ->> Elide function pointer objects
        inline value(memberinfo& information, valueinfo<arity, $shorthand::function> const, type const value) noexcept :
          function((static_cast<void>(information = memberinfo(
            static_cast<std::type_info const*>(NULL),
            $shorthand::function
          )), reinterpret_cast<decltype(value::function)>(value)))
        {}

        template <std::size_t arity> // ->> Elide integer objects
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::integer> const, decltype(value::integer) const value) noexcept :
          integer((static_cast<void>(information = memberinfo(
            static_cast<std::type_info const*>(NULL),
            $shorthand::integer
          )), value))
        {}

        template <std::size_t arity, typename typeA, class typeB> // ->> Elide data member pointer objects
        inline value(memberinfo& information, valueinfo<arity, $shorthand::member> const, typeA typeB::* const value) noexcept :
          member((static_cast<void>(information = memberinfo(
            static_cast<std::type_info const*>(NULL),
            $shorthand::member
          )), reinterpret_cast<sfinaeptr_t $shorthand::*>(value)))
        {}

        template <std::size_t arity, typename type> // ->> Elide nothing
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::null> const, type&&) noexcept :
          null((static_cast<void>(information = memberinfo(
            static_cast<std::type_info const*>(NULL),
            $shorthand::null
          )), sfinaeptr()))
        {}

        template <std::size_t arity, typename type> // ->> Elide object pointer objects
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::pointer> const, type* const value) noexcept :
          pointer((static_cast<void>(information = memberinfo(
            static_cast<std::type_info const*>(NULL),
            $shorthand::pointer | referrable::kindof<type>::value
          )), value))
        {}

        // template <std::size_t arity, std::size_t subalignment, std::size_t subsize> // TODO (Lapys) -> Elide references
        // constexpr value(memberinfo& information, valueinfo<arity, $shorthand::reference> const, value<subalignment, subsize> const& value, decltype(value::capacity) value::* const) noexcept :
        //   reference((static_cast<void>(information = memberinfo(
        //     value.capacity,
        //     value.type
        //   )), value.reference))
        // {}

        // template <std::size_t arity, std::size_t subalignment, std::size_t subsize> // TODO (Lapys) -> Elide references
        // constexpr value(memberinfo& information, valueinfo<arity, $shorthand::reference> const, value<subalignment, subsize> const& value, decltype(value::metadata) value::* const) noexcept :
        //   reference((static_cast<void>(information = memberinfo(
        //     value.metadata,
        //     value.type
        //   )), value.reference))
        // {}

        template <std::size_t arity, typename type> // ->> Elide references
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::reference> const, type&& value) noexcept :
          reference((static_cast<void>(information = memberinfo(
            referrable::kindof<type>::value == static_cast<reference::type>($shorthand::null) ? &typeid(type) : static_cast<std::type_info const*>(NULL),
            $shorthand::reference | referrable::kindof<type>::value
          )), addressof(forward_cast<type>(value))))
        {}

        template <std::size_t arity, typename type> // ->> Elide reference-to-arrays
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::reference> const, type (&value)[], typename conditional<$shorthand::null == referrable::kindof<type>::value, sfinaeptr_t>::type = sfinaeptr) noexcept :
          reference((static_cast<void>(information = memberinfo(
            &typeid(type),
            $shorthand::reference_array
          )), &value))
        {}

        template <std::size_t arity, typename type, std::size_t subcapacity> // ->> Elide reference-to-arrays
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::reference> const, type (&value)[subcapacity], typename conditional<$shorthand::null == referrable::kindof<type>::value, sfinaeptr_t>::type = sfinaeptr) noexcept :
          reference((static_cast<void>(information = memberinfo(
            &typeid(type),
            $shorthand::reference_array
          )), &value))
        {}

        template <std::size_t arity, typename type> // ->> Elide reference-to-arrays
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::reference> const, type (&value)[], typename conditional<$shorthand::null != referrable::kindof<type>::value, sfinaeptr_t>::type = sfinaeptr) noexcept :
          reference((static_cast<void>(information = memberinfo(
            static_cast<std::size_t>(0u),
            $shorthand::reference_array | (referrable::kindof<type>::value << 1u /* --> $shorthand::reference::array */)
          )), static_cast<type*>(value)))
        {}

        template <std::size_t arity, typename type, std::size_t capacity> // ->> Elide reference-to-arrays
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::reference> const, type (&value)[capacity], typename conditional<$shorthand::null != referrable::kindof<type>::value, sfinaeptr_t>::type = sfinaeptr) noexcept :
          reference((static_cast<void>(information = memberinfo(
            capacity,
            $shorthand::reference_array | (referrable::kindof<type>::value << 1u /* --> $shorthand::reference::array */)
          )), static_cast<type*>(value)))
        {}

        template <std::size_t arity, typename type> // ->> Elide reference-to-arrays
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::reference> const, type (&&value)[]) noexcept :
          value::value(
            information,
            valueinfo<arity, $shorthand::reference>(),
            static_cast<type (&)[]>(value)
          )
        {}

        template <std::size_t arity, typename type, std::size_t capacity> // ->> Elide reference-to-arrays
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::reference> const, type (&&value)[capacity]) noexcept :
          value::value(
            information,
            valueinfo<arity, $shorthand::reference>(),
            static_cast<type (&)[capacity]>(value)
          )
        {}

        template <std::size_t arity, typename type> // ->> Elide `$object` shorthand
        constexpr value(memberinfo& information, valueinfo<arity, $shorthand::shorthand> const, $object<type> const& value) noexcept(noexcept($shorthand::value<alignment, size>(
          instanceof<memberinfo&>      ()
          instanceof<valueinfo<arity> >(),
          instanceof<type>             ()
        ))) :
          value::value(
            information,
            valueinfo<arity>(),
            static_cast<type>(value.operator type())
          )
        {}

        template <std::size_t arity, opinfo::op operation, class... operands> // ->> Elide `$lambda` shorthand
        constexpr value(memberinfo&, valueinfo<arity, $shorthand::shorthand> const, $lambda<operation, operands...> const&) noexcept :
          value::value(dummy, valueinfo<arity, $shorthand::null>(), sfinaeptr)
        { static_assert(arity != arity, "Unable to construct index shorthand with lambda expression shorthand"); }

        template <std::size_t arity, std::size_t subarity, $$... states, typename... extensions> // ->> Elide `$n` shorthand
        constexpr value(memberinfo&, valueinfo<arity, $shorthand::shorthand> const, $n<subarity, $f<states...>, extensions...> const&, typename conditional<not boolean_or<(states == $o)...>::value, sfinaeptr_t>::type = sfinaeptr) noexcept :
          value::value(dummy, valueinfo<arity, $shorthand::null>(), sfinaeptr)
        { static_assert(arity != arity, "Unable to copy-construct shorthand without the `$o` format"); }

        // template <std::size_t arity, std::size_t subarity, $$... states, typename... extensions> // TODO (Lapys) -> Elide `$n` shorthand
        // constexpr value(memberinfo& information, valueinfo<arity, $shorthand::shorthand> const, $n<subarity, $f<states...>, extensions...> const& value, typename conditional<boolean_or<(states == $o)...>::value, sfinaeptr_t>::type = sfinaeptr) noexcept(noexcept($shorthand::value<alignment, size>(
        //   valueinfo<arity, $shorthand::undefined>(),
        //   instanceof<$n<subarity, $f<states...>, extensions...> const&>().value,
        //   instanceof<unsigned char*>()
        // ))) :
        //   value::value(
        //     value.type == $shorthand::decimal  ? $shorthand::value<alignment, size>(valueinfo<arity, $shorthand::decimal> (), value.value.decimal)  :
        //     value.type == $shorthand::function ? $shorthand::value<alignment, size>(valueinfo<arity, $shorthand::function>(), value.value.function) :
        //     value.type == $shorthand::integer  ? $shorthand::value<alignment, size>(valueinfo<arity, $shorthand::integer> (), value.value.integer)  :
        //     value.type == $shorthand::member   ? $shorthand::value<alignment, size>(valueinfo<arity, $shorthand::member>  (), value.value.member)   :
        //     value.type == $shorthand::pointer  ? $shorthand::value<alignment, size>(valueinfo<arity, $shorthand::pointer> (), value.value.pointer)  :
        //     value.type == $shorthand::null     ? $shorthand::value<alignment, size>(valueinfo<arity, $shorthand::null>    (), sfinaeptr)            :

        //     value.type & $shorthand::reference ? (
        //       (value.type & $shorthand::reference_array) and value.type != $shorthand::reference_array
        //       ? $shorthand::value<alignment, size>(valueinfo<arity, $shorthand::reference>(), value.value, &$shorthand::value<alignment, size>::capacity)
        //       : $shorthand::value<alignment, size>(valueinfo<arity, $shorthand::reference>(), value.value, &$shorthand::value<alignment, size>::metadata)
        //     ) :

        //     value.type == $shorthand::undefined and (value.undefined.alignment <= alignof(decltype(unknown<size>::value))                                   // ->> See `kindof<...>::value == $shorthand::undefined`
        //       ? value.undefined.size <= sizeof unknown<size>::value and 0u == (alignof(decltype(unknown<size>::value)) % value.undefined.alignment)         //
        //       : value.undefined.size <= sizeof unknown<size>::value -         (value.undefined.alignment - alignof(decltype(unknown<size>::value)))         //
        //     ) ? $shorthand::value<alignment, size>(valueinfo<arity, $shorthand::undefined>(), value.value, ::new (this -> undefined.value) unsigned char[size]) : // WARN (Lapys) -> Unsure if undefined behavior

        //     instanceof<$shorthand::value<alignment, size> >()
        //   )
        // { static_assert(arity == subarity, "Unable to copy-construct shorthand with another of different index"); }

        // template <std::size_t arity, std::size_t subalignment, std::size_t subsize> // TODO (Lapys) -> Elide type-ambiguous objects
        // inline value(memberinfo& information, valueinfo<arity, $shorthand::undefined> const, value<subalignment, subsize> const& value, void* buffer) :
        //   undefined()
        // {
        //   std::size_t    capacity  = sizeof unknown<size>::value;
        //   unsigned char *prebuffer = static_cast<unsigned char*>(buffer);

        //   // ...
        //   buffer      = std::align(value.undefined.alignment, value.undefined.size, buffer, capacity);
        //   information = memberinfo(&typeid(type), $shorthand::undefined);

        //   this -> undefined.size      = value.undefined.size;
        //   this -> undefined.handler   = value.undefined.handler;
        //   this -> undefined.alignment = value.undefined.alignment;
        //   this -> undefined.address   = value.undefined.handler(opinfo::construct, (
        //     NULL == buffer
        //     ? prebuffer + (value.undefined.alignment > alignof(decltype(unknown<size>::value)) ? value.undefined.alignment - alignof(decltype(unknown<size>::value)) : 0u)
        //     : buffer
        //   ), value.undefined.address);

        //   if (prebuffer != this -> undefined.value) {
        //     for (unsigned char *const end = prebuffer + size, *iterator = this -> undefined.value; end != prebuffer; ++iterator, ++prebuffer)
        //     *iterator = *prebuffer;
        //   }
        // }

        template <std::size_t arity, typename type> // ->> Elide type-ambiguous objects
        inline value(memberinfo& information, valueinfo<arity, $shorthand::undefined> const, type&& value) noexcept(noexcept(typename remove_qualifiers<type>::type(instanceof<type&>()))) :
          undefined()
        {
          void       *buffer   = this -> undefined.value;
          std::size_t capacity = sizeof unknown<size>::value;

          // ...
          buffer      = std::align(alignof(type), sizeof(type), buffer, capacity);
          information = memberinfo(&typeid(type), $shorthand::undefined);

          this -> undefined.size      = sizeof(type);
          this -> undefined.handler   = &unknown<size>::template handlerof<typename remove_reference<type>::type>::value;
          this -> undefined.alignment = alignof(type);
          this -> undefined.address   = const_cast<void*>(static_cast<void const volatile*>(::new (
            NULL == buffer
            ? this -> undefined.value + (alignof(type) > alignof(decltype(unknown<size>::value)) ? alignof(type) - alignof(decltype(unknown<size>::value)) : 0u)
            : buffer
          ) typename remove_qualifiers<type>::type(value)));
        }
      #ifdef __GNUC__
      # pragma GCC diagnostic pop
      #endif
    };
};

// TODO (Lapys) -> Make code readable, support array conversions, lambda-to-function conversion
template <std::size_t arity, $$... states, typename... extensions>
struct $n<arity, $f<states...>, extensions...> final {
  template <class>
  friend struct $shorthand::value;

  static_assert((
    sizeof...(extensions) < static_cast<std::size_t>($shorthand::extended) and
    sizeof...(extensions) < static_cast<std::size_t>($shorthand::pointer)  and
    sizeof...(extensions) < static_cast<std::size_t>($shorthand::reference)
  ), "Too many types passed to shorthand");

  private:
    $n<
      arity,
      $shorthand::format<boolean_or<(
        #ifdef __cpp_constexpr // --> 200704L
        # if __cpp_constexpr < 201907L
            states != $c and
        # endif
        #endif
        states == $o
      )...>::value ? $shorthand::destruct : $shorthand::nodestruct, states...>,
      std::nullptr_t, std::type_info const&, extensions...
    > value;

  public:
    template <typename... types>
    constexpr $n(types&&... arguments) noexcept(noexcept(decltype($n::value)(instanceof<types&&>()...))) :
      value(forward_cast<types>(arguments)...)
    {}

    /* TODO (Lapys) -> Operator composition-inheritance */
};

template <std::size_t arity, $$... states, typename... extensions>
struct $n<arity, $shorthand::format<$shorthand::destruct, states...>, extensions...> /* final */ : public $n<arity, $shorthand::format<$shorthand::nodestruct, states...>, extensions...> {
  #ifdef __cpp_constexpr // --> 200704L
  # if __cpp_constexpr >= 201907L
      constexpr
  # endif
  #endif
  ~$n() noexcept(noexcept(1)) {}
};

template <std::size_t arity, $$... states, typename... extensions>
struct $n<arity, $shorthand::format<$shorthand::nodestruct, states...>, extensions...> /* final */ {
  template <class>
  friend struct $shorthand::value;

  private:
    static std::size_t const alignment = commonof::alignment<typename conditional<opinfo::can<opinfo::alignof_object, extensions>::value, extensions, sfinaeptr_t>::type...>::value;
    static std::size_t const size      = commonof::size     <typename conditional<opinfo::can<opinfo::sizeof_object,  extensions>::value, extensions, sfinaeptr_t>::type...>::value;

    /* ... */
    typedef $shorthand::valueinfo<alignment, size> memberinfo;

    // ...
    struct novalue final {
      template <typename... types>
      constexpr novalue(types&&...) noexcept {}
    };

    // ...
    template <typename base, std::size_t alignment, std::size_t size>
    struct kindof final {
      private:
        template <typename...>
        struct subkindof {
          static type const value = $shorthand::kindof<base, alignment, size>::value;
        };

        template <typename subbase, typename... subbases>
        struct subkindof<subbase, subbases...> : public subkindof<subbases...> {
          static type const value = (
            opinfo::can<opinfo::construct, subbase, base>::value
            ? $shorthand::extended | (sizeof...(extensions) - sizeof...(subbases) - 1u)
            : subkindof<subbases...>::value
          );
        };

      public:
        static type const value = (
          is_shorthand       <typename remove_qualifiers<base>::type>                               ::value ? $shorthand::shorthand :
          is_lvalue_reference<base>::value or is_array<typename remove_rvalue_reference<base>::type>::value ? $shorthand::reference :
          subkindof          <base, extensions...>                                                  ::value
        );
    };

    // ...
    template <typename...>
    struct extvalue final {
      $shorthand::value<alignment, size> subvalue;

      /* ... */
      template <typename... types>
      constexpr extvalue(types&&... values) noexcept(noexcept($shorthand::value<alignment, size>(instanceof<types&&>()...))) :
        subvalue(forward_cast<types>(values)...)
      {}
    };

    // TODO (Lapys)
    //   IF REFERENCE: CHECK $r, OTHERWISE SKIP TO NEXT-FITTING OBJECT OR POSSIBLY UNKNOWN
    //   IF CONSTANT: SKIP NON-BYTE OBJECTS
    template <typename subextension, typename... subextensions>
    struct extvalue<subextension, subextensions...> final {
      typedef typename conditional<opinfo::can<opinfo::alignof_object, subextension>::value and not is_reference<subextension>::value, subextension, sfinaeptr_t>::type type;

      /* ... */
      static std::size_t const index               = sizeof...(extensions) - sizeof...(subextensions) - 1u;
      static bool        const is_trivial_lifetime = (opinfo::can<opinfo::constant_destruct, type>::value) and (opinfo::can<opinfo::copy, type>::value or opinfo::can<opinfo::move, type>::value);

      union {
        alignas(type) typename conditional<is_trivial_lifetime, type, sfinaeptr_t>::type extended;   // ->> Elide extended-type that are constant-destructible objects and copy-/ move-constructible objects
        typename remove_reference<subextension>::type                                   *referrable; // ->> Elide pointer-/ reference-to-extended-type objects
        extvalue<subextensions...>                                                       subvalue;   // ->> Elide ...
      };

      /* ... */
      constexpr extvalue(memberinfo& information, valueinfo<arity, $shorthand::pointer> const, typename remove_reference<subextension>::type* const value) noexcept : // ->> Elide object pointer objects
        referrable((static_cast<void>(information = memberinfo(
          NULL,
          $shorthand::pointer | index
        )), value))
      {}

      constexpr extvalue(memberinfo& information, valueinfo<arity, $shorthand::reference> const, typename remove_reference<subextension>::type& value) noexcept : // ->> Elide references
        referrable((static_cast<void>(information = memberinfo(
          NULL,
          $shorthand::reference_extended | index
        )), addressof(value)))
      {}

      constexpr extvalue(memberinfo& information, valueinfo<arity, $shorthand::reference> const, typename remove_reference<subextension>::type&& value) noexcept : // ->> Elide references
        referrable((static_cast<void>(information = memberinfo(
          NULL,
          $shorthand::reference_extended | index
        )), addressof(static_cast<typename remove_reference<subextension>::type&&>(value))))
      {}

      template <type subtype, typename type> // ->> Defer elision of object
      constexpr extvalue(memberinfo& information, valueinfo<arity, subtype> const, type&& value, typename conditional<not is_same<typename remove_qualifiers<type>::type, typename remove_qualifiers<subextension>::type>::value, sfinaeptr_t>::type = sfinaeptr) noexcept(noexcept(extvalue<subextensions...>(
        instanceof<memberinfo&>                           (),
        instanceof<$shorthand::valueinfo<arity, subtype> >(),
        instanceof<type&&>                                ()
      ))) :
        subvalue(
          information,
          $shorthand::valueinfo<arity, subtype>(),
          forward_cast<type>(value)
        )
      {}

      template <type subtype, typename type> // ->> Elide extended-type objects
      constexpr extvalue(memberinfo& information, valueinfo<arity, subtype> const, type&& value, typename conditional<is_trivial_lifetime and is_same<typename remove_qualifiers<type>::type, typename remove_qualifiers<subextension>::type>::value, sfinaeptr_t>::type = sfinaeptr) noexcept(noexcept(
        ...
      )) :
        extended()
      {}
    };

    /* ... */
    memberinfo                                                                                                   information;
    typename conditional<arity and boolean_or<(states == $o)...>::value, extvalue<extensions...>, novalue>::type value;

  public:
    constexpr $n() noexcept :
      information(),
      value      (
        this -> information,
        valueinfo<arity, $shorthand::null>(),
        sfinaeptr
      )
    {}

    template <typename type>
    constexpr $n(type&& value) noexcept(noexcept(decltype($n::value)(
      instanceof<$shorthand::valueinfo<alignment, size>&>                              (),
      instanceof<$shorthand::valueinfo<arity, kindof<type&&, alignment, size>::value> >(),
      instanceof<type&&>                                                               ()
    ))) :
      information(),
      value      (
        this -> information,
        $shorthand::valueinfo<arity, kindof<type&&, alignment, size>::value>(),
        forward_cast<type>(value)
      )
    {}

    /* TODO (Lapys) -> Operator overloads */
};

/* Constant */
constexpr static $n<0u, $f<$c>, $s> $0 = {};
// constexpr static $n<1u, $f<$c>, $s> $1 = {};
// constexpr static $n<2u, $f<$c>, $s> $2 = {};
// constexpr static $n<3u, $f<$c>, $s> $3 = {};
// constexpr static $n<4u, $f<$c>, $s> $4 = {};
// constexpr static $n<5u, $f<$c>, $s> $5 = {};
// constexpr static $n<6u, $f<$c>, $s> $6 = {};
// constexpr static $n<7u, $f<$c>, $s> $7 = {};
// constexpr static $n<8u, $f<$c>, $s> $8 = {};
// constexpr static $n<9u, $f<$c>, $s> $9 = {};

// constexpr static $lambda<$op::add, $n<1u, $f<$c>, $s>, $n<2u, $f<$c>, $s> > $add  = $1 + $2;
// constexpr static $n<1u, $c, $s>                                             $self = $1;
// --> $bitwise_and
// --> $bitwise_or
// --> $bitwise_xor
// --> $boolean_and
// --> $boolean_or
// --> $complement
// --> $divide
// --> $equals
// --> $equals_greater
// --> $equals_lesser
// --> $greater
// --> $lesser
// --> $minus
// --> $modulo
// --> $multiply
// --> $negate
// --> $subtract
// --> $unequals
//
// --> $if  ???
// --> $for ???

// #if defined __clang__
// # pragma clang diagnostic pop
// #elif defined __GNUC__
// # pragma GCC diagnostic pop
// #elif defined _MSC_VER
// # pragma warning(pop)
// #endif

/* Main */
int main(int, char*[]) /* noexcept */ {
  // del shorthand.exe & cls && clang++ -pedantic-errors -std=c++11 -Wall -Werror -Wextra shorthand.cpp -o shorthand.exe && shorthand.exe & del shorthand.exe
  // del shorthand.exe & cls && g++ -pedantic-errors -std=c++11 -Wall -Werror -Wextra -Wno-pragmas shorthand.cpp -o shorthand.exe && shorthand.exe & del shorthand.exe
  // del shorthand.exe shorthand.obj & cls && cl /std:c++14 /W4 /wd4584 shorthand.cpp && shorthand.exe & del shorthand.exe shorthand.obj
  // del shorthand.exe shorthand.obj & cls && icl -std=c++11 -Wall -Werror -w3 -wd2415,3280,4131 shorthand.cpp -o shorthand.exe && shorthand.exe & del shorthand.exe shorthand.obj
  // rm ./shorthand & clear && circle shorthand.cpp --pedantic -std=c++11 -Wall -Werror -Wextra && ./shorthand || rm ./shorthand
}
