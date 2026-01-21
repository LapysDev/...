#define REFL_EXPR(type)
#define REFL_MAX 10u
#include <ciso646>
#include <cstddef>
#include <cstdio>
#include <typeinfo>

/* ... */
#undef REFL_EXPR
#if __cplusplus >= 201103L or defined _MSC_VER or (!defined __clang__ and (defined __ECC or defined __ICC or defined __ICL or defined __INTEL_COMPILER or defined __INTEL_COMPILER_BUILD_DATE or defined __INTEL_LLVM_COMPILER)) // --> __cpp_initializer_lists {200806L}
# define REFL_EXPR(type) type // ->> Direct list initialization --> __cpp_initializer_lists {200806L}
#elif defined __clang__ or defined __GNUC__
# define REFL_EXPR(type) (type) // ->> Compound literal cast (C99 extension)
# if defined __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wc99-extensions"
#   pragma clang diagnostic ignored "-Wmissing-braces"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
# elif defined __GNUC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wpedantic"
# endif
#endif
  union refl /* final */ {
    private:
      template <std::size_t /* --> sizeof … */> // ->> SFINAE mechanism (`https://en.cppreference.com/w/cpp/language/sfinae.html`)
      union parse {
        typedef unsigned char type;
      };

      template <std::size_t capacity, unsigned char = 0x00u> // ->> Patterned data member value
      struct member /* final */;
        template <unsigned char _>
        struct member<0, _> /* final */ {
          template <typename type>
          /* constexpr */ operator type&() const /* noexcept */;
        };

      struct base_aggregate_initialization {
        template <typename, unsigned char>
        static bool const (&evaluate(...) /* noexcept */)[false + 1u];
      };

      template <typename> // ->> Stall evaluation until template substitution
      struct assert_aggregate_initialization /* final */ : public refl::base_aggregate_initialization {
        using refl::base_aggregate_initialization::evaluate;

        template <typename type, typename refl::parse<sizeof(REFL_EXPR(type) {refl::member<0>(), 0x00u})>::type>
        static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u];

        /* ... */
        enum /* : bool */ { value = sizeof(bool[false + 1u]) == sizeof assert_aggregate_initialization::evaluate<unsigned char[1], static_cast<unsigned char>(0x00u)>("") };
      };

    public:
      template <typename base, typename = void, typename = void>
      class can_aggregate_initialize : private refl::base_aggregate_initialization {
        private:
          friend class refl::can_aggregate_initialize<base, arity>; // ->> ( ͠° ͟ʖ ͡°)
          using refl::base_aggregate_initialization::evaluate;

        public:
          enum { value = assert_aggregate_initialization<void>::value };
      };
        template <typename base>
        class can_aggregate_initialize<base, 1u>::inner /* final */ : private refl::base_aggregate_initialization {
          using refl::base_aggregate_initialization::evaluate;

          template <typename type, typename refl::parse<sizeof(REFL_EXPR(base) {{}, ...})>::type>
          static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u];
        };
  };
#if __cplusplus >= 201103L or defined _MSC_VER or (!defined __clang__ and (defined __ECC or defined __ICC or defined __ICL or defined __INTEL_COMPILER or defined __INTEL_COMPILER_BUILD_DATE or defined __INTEL_LLVM_COMPILER)) // --> __cpp_initializer_lists {200806L}
#elif defined __clang__
#   pragma clang diagnostic pop
#elif defined __GNUC__
#   pragma GCC diagnostic pop
#endif

/* Main */
// struct A { enum {} : 0; };
// struct B { struct {} _; };
// union  C { struct { short _[2]; } a, b; };
// struct D { struct { short _[2]; } a, b; };
// struct E { struct { short _[2]; } a; short : 1; short b; };
// struct F { short a; void *b; struct { char _[3]; } c;    struct {} d; int e; };
// struct G { short a; void *b; char                  c[3]; struct {} d; int e; };
//
// struct F { int a; double b; unsigned char c[3]; struct {} d; };
// static union  U { int x, y; };
// static class  E { public: union { int x1; double x2; }; enum {} : 8; int y; };

int main(int, char*[]) /* noexcept */ {
  std::puts(refl::can_aggregate_initialize<void>::value ? "true" : "false");
  // (void) std::printf("[int[1]]: %4.5s" "\r\n", refl::can_aggregate_initialize<int[1], 2u>::value ? "false" : "true");
  // (void) std::printf("[int[2]]: %4.5s" "\r\n", refl::can_aggregate_initialize<int[2], 2u>::value ? "true"  : "false");
  // (void) std::printf("[A]:      %4.5s" "\r\n", refl::can_aggregate_initialize<A,      0u>::value ? "true"  : "false");
  // (void) std::printf("[B]:      %4.5s" "\r\n", refl::can_aggregate_initialize<B,      1u>::value ? "true"  : "false");
  // (void) std::printf("[C]:      %4.5s" "\r\n", refl::can_aggregate_initialize<C,      1u>::value ? "true"  : "false");
  // (void) std::printf("[D]:      %4.5s" "\r\n", refl::can_aggregate_initialize<D,      2u>::value ? "true"  : "false");
  // (void) std::printf("[E]:      %4.5s" "\r\n", refl::can_aggregate_initialize<E,      2u>::value ? "true"  : "false");
  // (void) std::printf("[F]:      %4.5s" "\r\n", refl::can_aggregate_initialize<F,      5u>::value ? "true"  : "false");
  // (void) std::printf("[G]:      %4.5s" "\r\n", refl::can_aggregate_initialize<G,      5u>::value ? "true"  : "false");
  // (void) std::puts("");
  // (void) std::printf("[int[1]]: %u" "\r\n", (unsigned) refl::count_aggregate_initializers<int[1]>::value);
  // (void) std::printf("[int[2]]: %u" "\r\n", (unsigned) refl::count_aggregate_initializers<int[2]>::value);
  // (void) std::printf("[A]:      %u" "\r\n", (unsigned) refl::count_aggregate_initializers<A>     ::value);
  // (void) std::printf("[B]:      %u" "\r\n", (unsigned) refl::count_aggregate_initializers<B>     ::value);
  // (void) std::printf("[C]:      %u" "\r\n", (unsigned) refl::count_aggregate_initializers<C>     ::value);
  // (void) std::printf("[D]:      %u" "\r\n", (unsigned) refl::count_aggregate_initializers<D>     ::value);
  // (void) std::printf("[E]:      %u" "\r\n", (unsigned) refl::count_aggregate_initializers<E>     ::value);
  // (void) std::printf("[F]:      %u" "\r\n", (unsigned) refl::count_aggregate_initializers<F>     ::value);
  // (void) std::printf("[G]:      %u" "\r\n", (unsigned) refl::count_aggregate_initializers<G>     ::value);
  // (void) std::puts("");

  // ...
  // (void) std::printf("[A]" "\r\n");
  // refl::inspect(A());
  // (void) std::puts("");

  // (void) std::printf("[B]: {%u}" "\r\n", (unsigned) offsetof(B, _));
  // refl::inspect(B());
  // (void) std::puts("");

  // (void) std::printf("[C]: {%u, %u}" "\r\n", (unsigned) offsetof(C, a), (unsigned) offsetof(C, b));
  // refl::inspect(C());
  // (void) std::puts("");

  // (void) std::printf("[D]: {%u, %u}" "\r\n", (unsigned) offsetof(D, a), (unsigned) offsetof(D, b));
  // refl::inspect(D());
  // (void) std::puts("");

  // (void) std::printf("[E]: {%u, %u}" "\r\n", (unsigned) offsetof(E, a), (unsigned) offsetof(E, b));
  // refl::inspect(E());
  // (void) std::puts("");

  // (void) std::printf("[F]: {%u, %u, %u, %u, %u}" "\r\n", (unsigned) offsetof(F, a), (unsigned) offsetof(F, b), (unsigned) offsetof(F, c), (unsigned) offsetof(F, d), (unsigned) offsetof(F, e));
  // refl::inspect(F());
  // (void) std::puts("");

  // (void) std::printf("[G]: {%u, %u, %u, %u, %u}" "\r\n", (unsigned) offsetof(G, a), (unsigned) offsetof(G, b), (unsigned) offsetof(G, c), (unsigned) offsetof(G, d), (unsigned) offsetof(G, e));
  // refl::inspect(G());
  // (void) std::puts("");
}
