#include <ciso646>
#include <cstddef>
#include <cstdio>
#include <cstring>

/* ... */
union refl {
  private:
    template <typename> // ->> Defer evaluation to template substitution
    union assert_aggregate_initialization;

    template <std::size_t> // --> sizeof …
    union parse { typedef unsigned char type; };

  public:
    enum /* : std::size_t */ { count_maximum = 4u };

    /* ... */
    template <typename base, std::size_t arity, bool = false>
    class can_aggregate_initialize {
      template <typename, std::size_t, bool>
      friend class refl::can_aggregate_initialize;

      private: template <typename, unsigned char> static bool const (&evaluate(...) /* noexcept */)[false + 1u];
      public:  enum /* : bool */ { value = refl::template assert_aggregate_initialization<base>::value and sizeof(bool[false + 1u]) != sizeof refl::can_aggregate_initialize<base, arity, true>::template evaluate<base, static_cast<unsigned char>(0x0u)>("") };
    };

    template <typename base>
    union count_aggregate_initializers {
      private:
        template <std::size_t arity, bool>
        union counter {
          enum /* : std::size_t */ { value =
            refl::count_aggregate_initializers<base>::counter<
              arity + 1u,
              refl::count_maximum != arity + 1u and refl::can_aggregate_initialize<base, arity + 1u>::value
            >::value +
            (0u != arity and refl::can_aggregate_initialize<base, arity>::value)
          };
        };
          template <std::size_t arity>
          union counter<arity, false> { enum /* : std::size_t */ { value = 0u }; };

      public:
        enum /* : std::size_t */ { value = refl::count_aggregate_initializers<base>::counter<0u, refl::template assert_aggregate_initialization<base>::value>::value };
    };
};

#if __cplusplus >= 201103L or defined _MSC_VER or (!defined __clang__ and (defined __ECC or defined __ICC or defined __ICL or defined __INTEL_COMPILER or defined __INTEL_COMPILER_BUILD_DATE or defined __INTEL_LLVM_COMPILER)) // --> __cpp_initializer_lists
  template <typename base>
  union refl::assert_aggregate_initialization {
    private:
      template <typename, unsigned char> static bool const (&evaluate(...) /* noexcept */)[true + 1u];
      template <typename type, typename refl::parse<sizeof(type{0x00u, 0x00u})>::type> static bool const (&evaluate(char const[]) /* noexcept */)[false + 1u];

    public:
      enum /* : bool */ { value = sizeof(bool[false + 1u]) != sizeof refl::assert_aggregate_initialization<base>::evaluate<unsigned char[1], static_cast<unsigned char>(0x00u)>("") };
  };

  template <typename base> class refl::can_aggregate_initialize<base, 1u, true> /* final */ : private refl::can_aggregate_initialize<base, 0u, false> { friend class refl::can_aggregate_initialize<base, 1u>; friend union refl::count_aggregate_initializers<base>; using refl::can_aggregate_initialize<base, 0u>::evaluate; template <typename type, typename refl::parse<sizeof(type {0})>      ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
  template <typename base> class refl::can_aggregate_initialize<base, 2u, true> /* final */ : private refl::can_aggregate_initialize<base, 0u, false> { friend class refl::can_aggregate_initialize<base, 2u>; friend union refl::count_aggregate_initializers<base>; using refl::can_aggregate_initialize<base, 0u>::evaluate; template <typename type, typename refl::parse<sizeof(type {0, 0})>   ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
  template <typename base> class refl::can_aggregate_initialize<base, 3u, true> /* final */ : private refl::can_aggregate_initialize<base, 0u, false> { friend class refl::can_aggregate_initialize<base, 3u>; friend union refl::count_aggregate_initializers<base>; using refl::can_aggregate_initialize<base, 0u>::evaluate; template <typename type, typename refl::parse<sizeof(type {0, 0, 0})>::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
#elif defined __clang__ or defined __GNUC__ // ->> Compound literals (C99 extension)
# if defined __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wc99-extensions"
# elif defined __GNUC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wpedantic"
# endif
# if defined __CUDACC_VER_BUILD__ || defined __CUDACC_VER_MAJOR__ || defined __CUDACC_VER_MINOR__ || defined __NVCC__ || defined __NVCOMPILER
#   pragma diag_suppress extended_friends_is_cpp11
# endif
  template <typename base>
  union refl::assert_aggregate_initialization {
    private:
      template <typename, unsigned char> static bool const (&evaluate(...) /* noexcept */)[true + 1u];
      template <typename type, typename refl::parse<sizeof (type) {0x00u, 0x00u}>::type> static bool const (&evaluate(char const[]) /* noexcept */)[false + 1u];

    public:
      enum /* : bool */ { value = sizeof(bool[false + 1u]) != sizeof refl::assert_aggregate_initialization<base>::evaluate<unsigned char[1], static_cast<unsigned char>(0x00u)>("") };
  };

  template <typename base> class refl::can_aggregate_initialize<base, 1u, true> /* final */ : private refl::can_aggregate_initialize<base, 0u, false> { friend class refl::can_aggregate_initialize<base, 1u>; friend union refl::count_aggregate_initializers<base>; using refl::can_aggregate_initialize<base, 0u>::evaluate; template <typename type, typename refl::parse<sizeof (type) {0}>      ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
  template <typename base> class refl::can_aggregate_initialize<base, 2u, true> /* final */ : private refl::can_aggregate_initialize<base, 0u, false> { friend class refl::can_aggregate_initialize<base, 2u>; friend union refl::count_aggregate_initializers<base>; using refl::can_aggregate_initialize<base, 0u>::evaluate; template <typename type, typename refl::parse<sizeof (type) {0, 0}>   ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
  template <typename base> class refl::can_aggregate_initialize<base, 3u, true> /* final */ : private refl::can_aggregate_initialize<base, 0u, false> { friend class refl::can_aggregate_initialize<base, 3u>; friend union refl::count_aggregate_initializers<base>; using refl::can_aggregate_initialize<base, 0u>::evaluate; template <typename type, typename refl::parse<sizeof (type) {0, 0, 0}>::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
# if defined __clang__
#   pragma clang diagnostic pop
# elif defined __GNUC__
#   pragma GCC diagnostic pop
# endif
#endif

/* Main */
// static int                    array[2]  = {1, 2};
// static struct S { int x, y; } aggregate = {3, 4};
// static union  U { int x, y; } variant   = {5};
// static class  C { public: union { int x1; double x2; }; enum {} : 8; int y; } whacky = {{6}, 7};

// #if __cplusplus < 201103L and not defined _MSC_VER
// # include <cassert>
// # include <cstdio>
// # include <cstdlib>
// # undef static_assert
//   static void (static_assert)(bool condition, char const message[]) {
//     if (condition)
//     return;

//     (void) std::fputs(message, stderr);
//     (void) std::fputs("\r\n", stderr);

//     std::abort();
//   }
// #endif

int main(int, char*[]) /* noexcept */ {
  // (void) array;
  // (void) aggregate;
  // (void) variant;
  // (void) whacky;

  (void) std::printf("%u" "\r\n", (unsigned) refl::can_aggregate_initialize<int[1], 2u>::value ? 1u : 0u);
  (void) std::printf("%u" "\r\n", (unsigned) refl::can_aggregate_initialize<int[2], 2u>::value ? 1u : 0u);
  (void) std::printf("%u" "\r\n", (unsigned) refl::count_aggregate_initializers<int[1]>::value);
  (void) std::printf("%u" "\r\n", (unsigned) refl::count_aggregate_initializers<int[2]>::value);
  // static_assert((refl::can_aggregate_initialize<int[1], 2u>::value == false), "CHALLENGE TWO");
  // static_assert((refl::can_aggregate_initialize<int[2], 2u>::value == true), "CHALLENGE THREE");
  // static_assert((refl::count_aggregate_initializers<int[1]>::value == 1u), "CHALLENGE FOUR");
  // static_assert((refl::count_aggregate_initializers<int[2]>::value == 2u), "CHALLENGE FIVE");

  // std::printf("\n" "[#]: %p %p" "\r\n", (void*) &array[0],     (void*) &array[1]);    for (void *const *member = refl::inspect(array);     NULL != *member; ++member) std::printf("%p" "\r\n", *member);
  // std::printf("\n" "[#]: %p %p" "\r\n", (void*) &aggregate.x,  (void*) &aggregate.y); for (void *const *member = refl::inspect(aggregate); NULL != *member; ++member) std::printf("%p" "\r\n", *member);
  // std::printf("\n" "[#]: %p %p" "\r\n", (void*) &variant  .x,  (void*) &variant  .y); for (void *const *member = refl::inspect(variant);   NULL != *member; ++member) std::printf("%p" "\r\n", *member);
  // std::printf("\n" "[#]: %p %p" "\r\n", (void*) &whacky   .x1, (void*) &whacky   .y); for (void *const *member = refl::inspect(whacky);    NULL != *member; ++member) std::printf("%p" "\r\n", *member);
}
