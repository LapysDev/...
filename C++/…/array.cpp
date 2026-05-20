#include <climits>
#include <cstddef>
#include <cstdio>
#include <iso646.h>
#include <memory>
#include <new>
#include <stdalign.h>
#include <stdint.h>
#if (defined __cplusplus and __cplusplus >= 202002L) or defined _MSVC_LANG or (defined __circle_lang__ or defined __clang__ or defined __CUDACC_VER_BUILD__ or defined __CUDACC_VER_MAJOR__ or defined __CUDACC_VER_MINOR__ or defined __GNUC__ or defined __ICC or defined __INTEL_COMPILER or defined __INTEL_LLVM_COMPILER or defined __NVCC__ or defined __NVCOMPILER)
# include <version>
#endif

#if not defined __has_cpp_attribute
# define __has_cpp_attribute(attribute) false
#endif

/* ... */
// array<char, N> == struct {
//   struct { char[N - (sizeof char* / sizeof char)] stack; };
//   union  { char[0 + (sizeof char* / sizeof char)] stack_leftover; char* heap = malloc(size_t + char[]) + sizeof(size_t); }; // ->> try make `printf(…)` compatible
//   uint   stack_size;
// };

// ... --- TODO (Lapys) -> Consider in-place reallocation (excess reservation then iterative commits as needed)
class array_internals /* final */ {
  template <typename, std::size_t, unsigned char>
  friend struct array;

  /* ... */
  static /* thread_local */ void       *array;
  static /* thread_local */ std::size_t count;
  static /* thread_local */ std::size_t length;
  static /* thread_local */ void       *recent;

  /* ... */
  #if defined __cpp_lib_launder /* --> 201606L */ or (defined _MSC_BUILD or defined _MSC_FULL_VER or defined _MSC_VER and defined _HAS_LAUNDER)
    template <typename type> /* constexpr */ static inline type* launder(type address[]) /* noexcept */ { return std::launder(address); }
  #elif defined __has_builtin and (defined __apple_build_version__ or defined __clang__ or defined __clang_major__ or defined __clang_minor__ or defined __clang_patchlevel__ or defined __clang_version__ or defined __GNUC__ or defined __GNUC_MINOR__ or defined __GNUC_PATCHLEVEL__)
    #if __has_builtin(__builtin_launder)
      template <typename type> /* constexpr */ static inline type* launder(type address[]) /* noexcept */ { return __builtin_launder(address); }
    #else
      template <typename type> /* constexpr */ static inline type* launder(type address[]) /* noexcept */ { return address; } // ->> ¯\ˍ(ツ)ˍ/¯
    #endif
  #elif defined __GNUC__ or defined __GNUC_MINOR__ or defined __GNUC_PATCHLEVEL__
    template <typename type> static inline type* launder(type address[]) /* noexcept */ { __asm__("" : "+r"(address)); return address; } // ->> Assume `address` was modified because it is used as an input & output
  #elif defined _ReadWriteBarrier and ((defined _MSC_BUILD or defined _MSC_FULL_VER or defined _MSC_VER) and (defined __NT__ or defined __TOS_WIN__ or defined __WIN32__ or defined __WINDOWS__ or defined _WIN16 or defined _WIN32 or defined _WIN32_WCE or defined _WIN64 or defined WIN32))
    template <typename type> static inline type* launder(type /* not const */ address[]) /* noexcept */ { _ReadWriteBarrier(); return address; } // ->> Prevent memory access re-ordering
  #else
    template <typename type> /* constexpr */ static inline type* launder(type address[]) /* noexcept */ { return address; } // ->> ¯\ˍ(ツ)ˍ/¯
  #endif

  #if defined __cpp_lib_start_lifetime_as // --> 202207L
    template <typename typeA, typename typeB> /* constexpr */ inline static typeA* reincarnate(typeB                address[], std::size_t const length = 1u) /* noexcept */ { return std::start_lifetime_as_array<typeA>(static_cast<void*>               (address), length); }
    template <typename typeA, typename typeB> /* constexpr */ inline static typeA* reincarnate(typeB const          address[], std::size_t const length = 1u) /* noexcept */ { return std::start_lifetime_as_array<typeA>(static_cast<void const*>         (address), length); }
    template <typename typeA, typename typeB> /* constexpr */ inline static typeA* reincarnate(typeB const volatile address[], std::size_t const length = 1u) /* noexcept */ { return std::start_lifetime_as_array<typeA>(static_cast<void const volatile*>(address), length); }
    template <typename typeA, typename typeB> /* constexpr */ inline static typeA* reincarnate(typeB       volatile address[], std::size_t const length = 1u) /* noexcept */ { return std::start_lifetime_as_array<typeA>(static_cast<void       volatile*>(address), length); }
  #elif defined __has_builtin and (defined _MSC_VER and _MSC_VER >= 1951) and not (defined __apple_build_version__ or defined __clang__ or defined __clang_major__ or defined __clang_minor__ or defined __clang_patchlevel__ or defined __clang_version__)
    #if true // --> __has_builtin(__builtin_start_lifetime_as_array)
      template <typename typeA, typename typeB> constexpr static typeA* reincarnate(typeB                address[], std::size_t const length = 1u) noexcept { return __builtin_start_lifetime_as_array<typeA>(static_cast<void*>               (address), length); }
      template <typename typeA, typename typeB> constexpr static typeA* reincarnate(typeB const          address[], std::size_t const length = 1u) noexcept { return __builtin_start_lifetime_as_array<typeA>(static_cast<void const*>         (address), length); }
      template <typename typeA, typename typeB> constexpr static typeA* reincarnate(typeB const volatile address[], std::size_t const length = 1u) noexcept { return __builtin_start_lifetime_as_array<typeA>(static_cast<void const volatile*>(address), length); }
      template <typename typeA, typename typeB> constexpr static typeA* reincarnate(typeB       volatile address[], std::size_t const length = 1u) noexcept { return __builtin_start_lifetime_as_array<typeA>(static_cast<void       volatile*>(address), length); }
    #endif
  #else
    // ->> Content and subobjects become indeterminate (or non-alive yet non-destructed)
    template <typename typeA, typename typeB> static inline typeA* reincarnate(typeB address[], std::size_t const = 1u) /* noexcept */ { return array_internals::launder(reinterpret_cast<typeB*>(address /* --> ::new ((void*) …) unsigned char[sizeof(typeA) * std::size_t{…}] */)); } // ->> ¯\ˍ(ツ)ˍ/¯
  #endif

  /* ... */
  #if (defined _MSC_BUILD or defined _MSC_FULL_VER or defined _MSC_VER) or (defined __cplusplus and __cplusplus >= 201103L)
    template <typename base>
    union element /* final */ {
      private:
        alignas(base) unsigned char _; // ->> Suppresses `value` destruction/ initialization
        base                        value;

      public:
        constexpr                          element()             noexcept                                             : _()                               {}
        template <typename type> constexpr element(type&& value) noexcept(noexcept(base(static_cast<type&&>(value)))) : value(static_cast<type&&>(value)) {}

        /* ... */
        constexpr operator base&()  noexcept { return const_cast<base&> (this -> value); }
        constexpr operator base&&() noexcept { return const_cast<base&&>(this -> value); }
    };
  #else
    template <typename base>
    struct element /* final */ {
      private:
        #if defined _Alignas
          unsigned char _Alignas(base) value[sizeof(base)];
        #elif defined __GNUC__ or defined __GNUC_MINOR__ or defined __GNUC_PATCHLEVEL__
          unsigned char value[sizeof(base)] __attribute__(aligned(array::alignment));
        #elif defined _MSC_BUILD or defined _MSC_FULL_VER or defined _MSC_VER
          #pragma warning(disable: 4324)
          __declspec(align(array::alignment)) unsigned char value[sizeof(base)];
        #elif defined __alignas_is_defined or (defined __cplusplus and __cplusplus >= 201103L)
          alignas(base) unsigned char value[sizeof(base)];
        #else
          unsigned char value[sizeof(base)]; // ->> Ideally aligned… somehow
        #endif

      public:
        inline                          element()                  throw() : value() { array_internals::initialize(this, NULL,                                                  sizeof(base), initial); }
        template <typename type> inline element(type const& value) throw() : value() { array_internals::initialize(this, ::new (static_cast<void*>(this -> value)) base(value), sizeof(base), initial); }

        /* ... */
        inline operator base&()             throw() { return *array_internals::launder(reinterpret_cast<base*>      (this -> value)); }
        inline operator base const&() const throw() { return *array_internals::launder(reinterpret_cast<base const*>(this -> value)); }
    };
  #endif

  template <typename base, std::size_t capacity = 1u> // ->> For `.dynamic`
  struct elements /* final */ {
    std::size_t length;
    base        value[capacity];
  };

  union traits;

  template <uintmax_t maximum> // --> uintmin_t<255u> == uint8_t and uintmin_t<256u> == uint16_t and …
  union uintmin /* final */ {
    private:
      template <typename base, bool = maximum <= (((static_cast<uintmax_t>(1u) << ((CHAR_BIT * sizeof(base)) - 1u)) - 1u) << 1u) + 1u, unsigned char = 0x00u>
      union valueof /* final */ { typedef base type; };
        template <typename base, unsigned char _> union valueof<base,           false, _> /* final */ { typedef uintmax_t type; };
        template                <unsigned char _> union valueof<unsigned char,  false, _> /* final */ { typedef typename array_internals::uintmin::template valueof<unsigned short>    ::type type; };
        template                <unsigned char _> union valueof<unsigned short, false, _> /* final */ { typedef typename array_internals::uintmin::template valueof<unsigned int>      ::type type; };
        template                <unsigned char _> union valueof<unsigned int,   false, _> /* final */ { typedef typename array_internals::uintmin::template valueof<unsigned long>     ::type type; };
        #if (defined _MSC_BUILD or defined _MSC_FULL_VER or defined _MSC_VER) or (defined __cplusplus and __cplusplus >= 201103L)
          template <unsigned char _> union valueof<unsigned long, false, _> /* final */ { typedef typename array_internals::uintmin::template valueof<unsigned long long>::type type; };
          #if defined __apple_build_version__ or defined __clang__ or defined __clang_major__ or defined __clang_minor__ or defined __clang_patchlevel__ or defined __clang_version__
            template <unsigned char _> union valueof<unsigned long long, false, _> /* final */ { typedef typename array_internals::uintmin::template valueof<__uint128_t>::type type; };
          #elif defined __SIZEOF_INT128__ and (defined __GNUC__ or defined __GNUC_MINOR__ or defined __GNUC_PATCHLEVEL__)
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wpedantic"
            template <unsigned char _> union valueof<unsigned long long, false, _> /* final */ { typedef typename array_internals::uintmin::template valueof<unsigned __int128>::type type; };
            #pragma GCC diagnostic pop
          #elif defined __cpp_lib_ranges // --> 201911L
            template <unsigned char _> union valueof<unsigned long long, false, _> /* final */ { typedef typename array_internals::uintmin::template valueof<std::ranges::range_difference_t<std::ranges::iota_view<unsigned long long, unsigned long long> > >::type type; };
          #endif
        #endif

    public:
      typedef typename valueof<unsigned char>::type type;
  };

  /* ... */
  template <typename type>
  /* constexpr */ static type* compact(typename array_internals::template element<type> elements[], std::size_t const length) /* noexcept */ {
    // ... ->> Convert the `elements` array to its conventional form
    return array_internals::reincarnate<type>(elements, length);
  }

  static void initialize(void* const element, void* const value, std::size_t const size, std::size_t const space) /* throw() */ {
    uintptr_t const address = reinterpret_cast<uintptr_t>(static_cast<void*>(element));
    uintptr_t const recent  = reinterpret_cast<uintptr_t>(array_internals::recent);

    // ...
    if (array_internals::count >= space || size < (address < recent ? recent - address : address - recent))
    array_internals::count = 0u;

    if (0u == array_internals::count) {
      array_internals::array  = element;
      array_internals::length = 0u;
    }

    array_internals::count++;
    array_internals::length += NULL != value;
    array_internals::recent  = element;
  }
};
  /* thread_local */ void       *array_internals::array  = NULL;
  /* thread_local */ std::size_t array_internals::count  = 0u;
  /* thread_local */ std::size_t array_internals::length = 0u;
  /* thread_local */ void       *array_internals::recent = NULL;

template <typename base, std::size_t initial = (sizeof(void*) /* --> sizeof(base*) */ / sizeof(base)) + 1u, unsigned char = initial ? (initial > /* ->> Some arbitrary threshold */ sizeof(void*) / sizeof(base)) ? 0xFFu : 0x01u : 0x00u>
struct array {
  /* TODO (Lapys) -> Does not move `.automatic` to `.dynamic`, it keeps both and pretends they’re contiguous until `.data()` (or operator overload) flattens them */
};

template <typename base, std::size_t initial>
struct array<base, initial, false> /* --> [[assume(initial == 0zu)]] */ {
  public:
    // TODO (Lapys) -> 3 (arbitrary maximum) initializers to catch elements with a sentinel to stop extra — check if `[[no_unique_address]]` can apply to the first member
    base *dynamic;
};

#if (defined _MSC_BUILD or defined _MSC_FULL_VER or defined _MSC_VER) or (defined __cplusplus and __cplusplus >= 201103L)
  union array_internals::traits /* final */ {
    template <bool... conditions>
    struct conjunction final {
      private:
        template <unsigned char, bool...>
        struct valueof;
          template <unsigned char _>                                           struct valueof<_>                                 final { static bool const value = true; };
          template <unsigned char _, bool subcondition, bool... subconditions> struct valueof<_, subcondition, subconditions...> final { static bool const value = subcondition && array_internals::traits::template conjunction<conditions...>::template valueof<_, subconditions...>::value; };

      public:
        static bool const value = array_internals::traits::template conjunction<conditions...>::template valueof<0x00u, conditions...>::value;
    };
  };

  template <typename base, std::size_t initial>
  struct array<base, initial, true> /* --> [[assume(sizeof ::automatic <= sizeof ::dynamic)]] */ {
    private:
      typedef struct array_internals::template elements<base> elements;

      union { typename array_internals::template element<base>  automatic[initial]; base *dynamic; };
      static std::size_t const                                  alignment = alignof(typename array::elements); // ->> For `.dynamic` only
      typename array_internals::template uintmin<initial>::type automaticLength;
      static std::size_t const                                  offset = offsetof(typename array::elements, value); // ->> For `.dynamic` only

      /* ... */
      template <typename... types>
      constexpr array(typename array_internals::template element<base> (*const)[initial >= sizeof...(types)], types&&... values) noexcept(array_internals::traits::template conjunction<noexcept(base(static_cast<types&&>(values)))...>::value) :
        automatic      {static_cast<base>(static_cast<types&&>(values))...},
        automaticLength(sizeof...(types))
      { /* TODO (Lapys) -> `array_internals::compact(this -> automatic, sizeof...(types))` turns off compile-time */ }

      template <typename... types>
      inline array(typename array_internals::template element<base> (*const)[initial < sizeof...(types)], types&&... values) noexcept(false) :
        #if defined __cpp_aligned_new // ->> Assume `alignof(…)` is provided since C++11 --> 201606L
          dynamic(static_cast<base*>(::operator new(sizeof(struct array_internals::template elements<base, sizeof...(types)>), static_cast<std::align_val_t>(array::alignment), std::nothrow))),
        #else
          dynamic(static_cast<base*>(::operator new(sizeof(struct array_internals::template elements<base, sizeof...(types)>), std::nothrow))),
        #endif
        automaticLength(NULL != this -> dynamic ? initial + 1u : 0u)
      {
        if (NULL == this -> dynamic)
        return;

        (void)            ::new (static_cast<void*>(this -> dynamic))                                                   std::size_t(sizeof...(types));
        this -> dynamic = ::new (static_cast<void*>(reinterpret_cast<unsigned char*>(this -> dynamic) + array::offset)) base[]     {static_cast<base>(static_cast<types&&>(values))...};
      }

      /* ... */
      // void* next() {}

    public:
      template <typename... types>
      constexpr array(types&&... values) noexcept(noexcept(array(static_cast<union array_internals::template element<base> (*)[true]>(NULL), static_cast<types&&>(values)...))) :
        array::array(static_cast<typename array_internals::template element<base> (*)[true]>(NULL), static_cast<types&&>(values)...)
      {}

      /* constexpr */ ~array() noexcept(noexcept(this -> dynamic -> ~base())) {
        if (initial >= this -> automaticLength)
        return; // TODO (Lapys) -> Destruct `.automatic` elements

        for (std::size_t index = *reinterpret_cast<std::size_t*>(reinterpret_cast<unsigned char*>(this -> dynamic) - array::offset); index--; )
        this -> dynamic[index].~base();

        #if defined __cpp_aligned_new // ->> Assume `alignof(…)` is provided since C++11 --> 201606L
          ::operator delete(reinterpret_cast<unsigned char*>(this -> dynamic) - array::offset, static_cast<std::align_val_t>(array::alignment), std::nothrow);
        #else
          ::operator delete(reinterpret_cast<unsigned char*>(this -> dynamic) - array::offset, std::nothrow);
        #endif
      }
  };
#else // ->> `struct array` can’t construct more than `initial` elements on initialization
  template <typename base, std::size_t initial>
  struct array<base, initial, true> /* --> [[assume(sizeof ::automatic <= sizeof ::dynamic)]] */ {
    private:
      struct alignment /* final */ {
        unsigned char const padding;
        base          const object;
      };

      struct constructor /* final */ {
        inline constructor() throw() {
          static_cast<struct array*>(array_internals::array) -> automaticLength = array_internals::count ? array_internals::length : 0u;
          // TODO (Lapys) -> `array_internals::compact(… -> automatic, … -> automaticLength)`
        }
      };

      union initializer_guard /* final */ {
        inline initializer_guard() throw() {}

        template <typename type>
        inline initializer_guard(type const&) {
          for (array_internals::count = 0u; array_internals::length; )
          reinterpret_cast<base*>(static_cast<struct array*>(array_internals::array) -> automatic[--array_internals::length].value) -> ~base();
        }
      };

      /* ... */
      enum /* : std::size_t */ { alignment = sizeof(struct array::alignment) - sizeof(base) };

    public: // --- TODO (Lapys) -> Obfuscate public non-static data members’ names
      typename array_internals::template element<base> automatic[initial]; // ->> Consumes pre-C++11 initializer lists
      #if (not defined __has_cpp_attribute and not (defined __cplusplus and __cplusplus >= 202002L)) and not (defined __apple_build_version__ or defined __clang__ or defined __clang_major__ or defined __clang_minor__ or defined __clang_patchlevel__ or defined __clang_version__ or defined __ECC or defined __GNUC__ or defined __GNUC_MINOR__ or defined __GNUC_PATCHLEVEL__ or defined __ICC or defined __ICL or defined __INTEL_COMPILER or defined __INTEL_COMPILER_BUILD_DATE or defined __INTEL_LLVM_COMPILER)
      # error ONE
        // struct /* final */ : public array::constructor {
        //   private: typename array_internals::template uintmin<initial>::type value; // --> reinterpret_cast<uintmin_t<…>&>(.automaticLength)
        //   public:                                         // --> static_cast     <uintmin_t<…>&>(.automaticLength)
        //     inline operator typename array_internals::template uintmin<initial>::type&()             throw() { return this -> value; }
        //     inline operator typename array_internals::template uintmin<initial>::type const&() const throw() { return this -> value; }
        // } automaticLength;
      #elif __has_cpp_attribute(no_unique_address)
      # error TWO
        [[no_unique_address]] union array::initializer_guard      guard;           // ->> Prevents excess initialization of other non-`base` members
        typename array_internals::template uintmin<initial>::type automaticLength; //
        [[no_unique_address]] struct array::constructor           constructor;     // ->> Re-initializes `.automaticLength` to the number of explicitly instantiated `.elements`
        // TODO (Lapys) -> 3 (arbitrary maximum) initializers to catch elements with a sentinel to stop extra
      #endif

      /* ... */
      ~array() {
        if (initial >= this -> automaticLength) {
          while (this -> automaticLength)
          reinterpret_cast<base*>(this -> automatic[--this -> automaticLength].value) -> ~base();
        }

        else {}
      }
      // TODO (Lapys) -> `array::_` is `union` with dynamic `base*` elements (all automatic gets shifted), with `.length >= initial` denoting this
  };
#endif

// struct {
//   0u           == initial ==> 2 // no stack; just heap
//   DYNAMIC_SIZE >= initial ==> 1 // stack bool for heap
//     //         array<char, 3>  vs array<char, sizeof(void*) + 1 == 9>
//     // add:    stack           vs stack
//     // add:    stack           vs stack
//     // add(n): heap (move all) vs heap (move leftover)
//   ...                     ==> 0 // stack added to heap
// };

/* Main */
// --> del array.exe & cls && clang++ -pedantic-errors -std=c++98 -Wall -Werror -Wextra -Wno-c++11-extensions -Wno-missing-braces -Wno-missing-field-initializers array.cpp -o array.exe && array.exe & del array.exe
// --> del array.exe & cls && clang++ -pedantic-errors -std=c++11 -Wall -Werror -Wextra -Wno-c++11-extensions -Wno-missing-braces -Wno-missing-field-initializers array.cpp -o array.exe && array.exe & del array.exe
// --> del array.exe & cls && g++ -pedantic-errors -std=c++98 -Wall -Werror -Wextra -Wno-c++11-extensions -Wno-missing-field-initializers -Wno-shift-count-overflow array.cpp -o array.exe && array.exe & del array.exe
// --> del array.exe & cls && g++ -pedantic-errors -std=c++11 -Wall -Werror -Wextra -Wno-c++11-extensions -Wno-missing-field-initializers -Wno-shift-count-overflow array.cpp -o array.exe && array.exe & del array.exe
// --> del array.exe array.obj & cls && cl /std:c++14 /W4 array.cpp && array.exe & del array.exe array.obj
struct T { static unsigned char total; char n; T() : n('\0') { (void) std::printf("[#%hu]" "\r\n", ++T::total); } T(char c) : n(c) { (void) std::printf("[#%hu]  => %c" "\r\n", ++T::total, n); } T(struct T const& t) : n(t.n) { (void) std::printf("[#%hu]  -> %c" "\r\n", ++T::total, n); } ~T() { (void) std::printf(n ? "[rip] => %c" "\r\n" : "[ded]" "\r\n", n); } };
  unsigned char T::total = 0u;

int main(int, char*[]) /* noexcept */ {
  array<T, 3> const A = {};                   (void) A; (void) std::puts("·");
  array<T, 3> const B = {'a'};                (void) B; (void) std::puts("·");
  array<T, 3> const C = {'b', 'c'};           (void) C; (void) std::puts("·");
  array<T, 3> const D = {'d', 'e', 'f'};      (void) D; (void) std::puts("·");
  array<T, 3> const E = {'g', 'h', 'i', 'j'}; (void) E; (void) std::puts("·");

  (void) std::printf("[A]: %lu" "\r\n", static_cast<unsigned long>(A.automaticLength));
  (void) std::printf("[B]: %lu" "\r\n", static_cast<unsigned long>(B.automaticLength));
  (void) std::printf("[C]: %lu" "\r\n", static_cast<unsigned long>(C.automaticLength));
  (void) std::printf("[D]: %lu" "\r\n", static_cast<unsigned long>(D.automaticLength));
  (void) std::printf("[E]: %lu" "\r\n", static_cast<unsigned long>(E.automaticLength));

  (void) std::puts("×");
}
