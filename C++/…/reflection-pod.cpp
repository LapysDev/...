#define REFL_MAX 4u
#include <ciso646>
#include <cstddef>
#include <cstdio>
#include <typeinfo>

/* ... */
union refl /* final */ {
  template <typename, std::size_t, bool>     class  can_aggregate_initialize;
  template <typename>                        union  count_aggregate_initializers;
  template <typename, typename, std::size_t> struct info;

  // private:
    enum /* : std::size_t */ { count_maximum = (REFL_MAX) };

    /* ... */
    template <typename> // ->> Stall evaluation until template substitution
    struct assert_aggregate_initialization;

    struct base_aggregate_initialization {
      template <typename, unsigned char>
      static bool const (&evaluate(...) /* noexcept */)[false + 1u];
    };

    template <typename base, typename addresser = void> // ->> Setup for `struct refl::info<…> refl::inspect(…);`
    struct inspector { typedef struct refl::info<base, addresser, refl::count_aggregate_initializers<base>::value> type; };
      template <typename base>                     struct inspector<base&>                          /* final */ : public refl::inspector<base>                      {};
      template <typename base, typename addresser> struct inspector<base const,          addresser> /* final */ : public refl::inspector<base, void const>          {};
      template <typename base, typename addresser> struct inspector<base const volatile, addresser> /* final */ : public refl::inspector<base, void const volatile> {};
      template <typename base, typename addresser> struct inspector<base volatile,       addresser> /* final */ : public refl::inspector<base, void volatile>       {};

    template <std::size_t capacity, unsigned char = 0x00u> // ->> Patterned data member value
    struct member /* final */ {
      unsigned char                pattern;
      bool                   const patterned;
      std::size_t           *const size;
      std::type_info const **const type;
      unsigned char                value[capacity];

      /* ... */
      template <typename base, typename addresser, std::size_t arity>
      /* constexpr */ member(struct refl::info<base, addresser, arity>* const information, std::size_t const index, unsigned char const pattern, bool const patterned) /* noexcept */ :
        pattern  (pattern),
        patterned(patterned),
        size     (information -> size + index),
        type     (information -> type + index),
        value    ()
      {}

      /* ... */
      inline static void next(unsigned char* const pattern) /* noexcept */ {
        ++*pattern; // ->> Modulo `(1u << CHAR_BIT) - 1u`
      }

      /* ... */
      template <typename type>
      operator type&() /* noexcept */ {
        *(this -> size) =  sizeof(type);
        *(this -> type) = &typeid(type);

        if (not this -> patterned) { for (std::size_t index = sizeof(type); index; ) this -> value[--index] = 0x00u; }
        else for (std::size_t index = 0u; index != sizeof(type); ) { this -> value[index++] = this -> pattern; refl::member<capacity>::next(&this -> pattern); }

        return *reinterpret_cast<type*>(this -> value);
      }
    };
      template <unsigned char _>
      struct member<0, _> /* final */ {
        template <typename type>
        /* constexpr */ operator type&() const /* noexcept */;
      };

    template <std::size_t /* --> sizeof … */> // ->> SFINAE mechanism (`https://en.cppreference.com/w/cpp/language/sfinae.html`)
    union parse { typedef unsigned char type; };

    template <std::size_t arity, unsigned char = 0x00u> // ->> Aggregate-initialized object with patterned non-static data members
    union sentinel /* static_assert(false, …) */ { template <typename base, typename addresser> static base evaluate(struct refl::info<base, addresser, arity>* const, ...) /* noexcept = delete; */; };
      template <unsigned char _> union sentinel<0u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, addresser, 0u>* const,             std::size_t const,       unsigned char const)         /* noexcept */ {                                                   base const value = {};                                                                                                                                            return value; } };
      template <unsigned char _> union sentinel<1u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, addresser, 1u>* const information, std::size_t const index, unsigned char const pattern) /* noexcept */ { typedef struct refl::member<sizeof(base)> member; base const value = {member(information, 0u, pattern, index == 0u)};                                                                                               return value; } };
      template <unsigned char _> union sentinel<2u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, addresser, 2u>* const information, std::size_t const index, unsigned char const pattern) /* noexcept */ { typedef struct refl::member<sizeof(base)> member; base const value = {member(information, 0u, pattern, index == 0u), member(information, 1u, pattern, index == 1u)};                                                return value; } };
      template <unsigned char _> union sentinel<3u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, addresser, 3u>* const information, std::size_t const index, unsigned char const pattern) /* noexcept */ { typedef struct refl::member<sizeof(base)> member; base const value = {member(information, 0u, pattern, index == 0u), member(information, 1u, pattern, index == 1u), member(information, 2u, pattern, index == 2u)}; return value; } };

    /* ... */
    template <class type /* --> struct refl::info<…> */>
    static type inspect(unsigned char inspected[]) {
      return type(inspected);
    }

  public:
    template <typename base, std::size_t arity, bool = false>
    class can_aggregate_initialize : private refl::base_aggregate_initialization {
      private:
        friend class refl::can_aggregate_initialize<base, arity>; // ->> ( ͠° ͟ʖ ͡°)
        using refl::base_aggregate_initialization::evaluate;

      public:
        enum /* : bool */ { value = refl::assert_aggregate_initialization<base>::value and sizeof(bool[false + 1u]) != sizeof refl::can_aggregate_initialize<base, arity, true>::template evaluate<base, static_cast<unsigned char>(0x0u)>("") };
    };

    template <typename base>
    union count_aggregate_initializers {
      private:
        template <std::size_t arity, bool>
        union counter { enum /* : std::size_t */ { value = refl::count_aggregate_initializers<base>::counter<arity + 1u, refl::count_maximum != arity + 1u and refl::can_aggregate_initialize<base, arity + 1u>::value>::value + (0u != arity and refl::can_aggregate_initialize<base, arity>::value) }; };
          template <std::size_t arity>
          union counter<arity, false> { enum /* : std::size_t */ { value = 0u }; };

      public:
        enum /* : std::size_t */ { value = refl::count_aggregate_initializers<base>::counter<0u, refl::assert_aggregate_initialization<base>::value>::value };
    };

    template <typename base, typename addresser, std::size_t arity>
    struct info /* final */ /* ->> Result of `refl::inspect(…)`; While more types are generated this way, this is more conservative with space requirements and thread-safety */ {
      friend struct info<base, addresser, arity> refl::inspect(unsigned char[]);

      public:
        enum /* : std::size_t */ { count = arity };

        addresser            *addresses[arity + 1u]; // ->> Null-terminated
        std::type_info const *types    [arity + 1u];
        std::size_t           sizes    [arity + 1u];

      /* ... */
      private:
        info(unsigned char inspected[]) /* noexcept */ :
          addresses(),
          types    (),
          sizes    ()
        {
          std::size_t const capacity = sizeof(base);
          std::size_t       offset   = 0u;
          unsigned char     pattern  = 0u; // ->> Pattern seed for sentinel members i.e. `0u == {0x00u, 0x01u, …}`

          // ...
          // [123][123][123][123]END
          // [000][456][000][456]END
          // [000][789][000][000]END
          // NEXT MEMBER
          for (std::size_t index = 0u; arity != index; ++index)
          for (std::size_t const size = this -> sizes[index] /* ->> Sentinel member byte size */; ; ) {
            unsigned char const *address    = NULL;
            std::size_t          count      = 0u;
            bool                 duplicate  = false;
            unsigned char        enumerator = pattern;
            base const           sentinel   = refl::sentinel<arity>::evaluate(this, index, pattern);

            // ...
            for (unsigned char const *iterator = &reinterpret_cast<unsigned char const&>(sentinel) + offset, *const end = iterator + (sizeof(base) - offset); end != iterator; ++iterator) {
              if (count == size) {
                if (NULL != address) {
                  duplicate = true;
                  break;
                }

                address = iterator - size;
                goto reset;
              }

              if (enumerator != *iterator)
                goto reset;

              // ...
              count++;
              refl::member<capacity>::next(&enumerator);

              continue;
              reset: {
                count      = 0u;
                enumerator = pattern;
              }
            }

            // ...
            for (std::size_t count = size; count--; )
            refl::member<capacity>::next(&pattern); // ->> Reseeding once can be fine

            if (not duplicate) {
              std::size_t const location = NULL != address ? address - &reinterpret_cast<unsigned char const&>(sentinel) : offset;

              // ...
              this -> addresses[index] = inspected + location; // ->> Alignment of `inspected` and its member remains unknown
              offset                   = size      + location;

              break;
            }
          }

          this -> sizes    [arity] = 0u;
          this -> addresses[arity] = NULL;
          this -> types    [arity] = NULL;
        }
    };

    /* ... */
    #if __cplusplus >= 201103L or defined _MSC_VER
      template <typename type>
      static typename refl::inspector<type>::type inspect(type&& object) {
        return refl::inspect<refl::inspector<type>::type>(const_cast<unsigned char*>(&reinterpret_cast<unsigned char const volatile&>(object)));
      }
    #else
      template <typename type> static struct refl::inspector<type>::type inspect(type&                object) { return refl::inspect<refl::inspector<type>::type>(const_cast<unsigned char*>(&reinterpret_cast<unsigned char const volatile&>(object))); }
      template <typename type> static struct refl::inspector<type>::type inspect(type const&          object) { return refl::inspect<refl::inspector<type>::type>(const_cast<unsigned char*>(&reinterpret_cast<unsigned char const volatile&>(object))); }
      template <typename type> static struct refl::inspector<type>::type inspect(type const volatile& object) { return refl::inspect<refl::inspector<type>::type>(const_cast<unsigned char*>(&reinterpret_cast<unsigned char const volatile&>(object))); }
      template <typename type> static struct refl::inspector<type>::type inspect(type       volatile& object) { return refl::inspect<refl::inspector<type>::type>(const_cast<unsigned char*>(&reinterpret_cast<unsigned char const volatile&>(object))); }
    #endif
};

#if __cplusplus >= 201103L or defined _MSC_VER or (!defined __clang__ and (defined __ECC or defined __ICC or defined __ICL or defined __INTEL_COMPILER or defined __INTEL_COMPILER_BUILD_DATE or defined __INTEL_LLVM_COMPILER)) // --> __cpp_initializer_lists
  template <typename base>
  struct refl::assert_aggregate_initialization /* final */ : public refl::base_aggregate_initialization {
    using refl::base_aggregate_initialization::evaluate;

    template <typename type, typename refl::parse<sizeof(type{0x00u, 0x00u})>::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u];
    enum /* : bool */ { value = sizeof(bool[false + 1u]) == sizeof refl::assert_aggregate_initialization<base>::evaluate<unsigned char[1], static_cast<unsigned char>(0x00u)>("") };
  };

  template <typename base> class refl::can_aggregate_initialize<base, 1u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 1u>; friend union refl::count_aggregate_initializers<base>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof(type {refl::member<0>()})>                                      ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
  template <typename base> class refl::can_aggregate_initialize<base, 2u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 2u>; friend union refl::count_aggregate_initializers<base>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof(type {refl::member<0>(), refl::member<0>()})>                   ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
  template <typename base> class refl::can_aggregate_initialize<base, 3u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 3u>; friend union refl::count_aggregate_initializers<base>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof(type {refl::member<0>(), refl::member<0>(), refl::member<0>()})>::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
#elif defined __clang__ or defined __GNUC__ // ->> Compound literals (C99 extension)
# if defined __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wc99-extensions"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
# elif defined __GNUC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wpedantic"
# endif
  template <typename base>
  struct refl::assert_aggregate_initialization /* final */ : public refl::base_aggregate_initialization {
    using refl::base_aggregate_initialization::evaluate;

    template <typename type, typename refl::parse<sizeof (type) {0x00u, 0x00u}>::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u];
    enum /* : bool */ { value = sizeof(bool[false + 1u]) == sizeof refl::assert_aggregate_initialization<base>::evaluate<unsigned char[1], static_cast<unsigned char>(0x00u)>("") };
  };

  template <typename base> class refl::can_aggregate_initialize<base, 1u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 1u>; friend union refl::count_aggregate_initializers<base>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof (type) {refl::member<0>()}>                                      ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
  template <typename base> class refl::can_aggregate_initialize<base, 2u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 2u>; friend union refl::count_aggregate_initializers<base>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof (type) {refl::member<0>(), refl::member<0>()}>                   ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
  template <typename base> class refl::can_aggregate_initialize<base, 3u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 3u>; friend union refl::count_aggregate_initializers<base>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>()}>::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
# if defined __clang__
#   pragma clang diagnostic pop
# elif defined __GNUC__
#   pragma GCC diagnostic pop
# endif
#endif

/* Main */
// static int                    array[2]  = {1, 2};
struct aggregate { int x, y; struct {} z; };
// static union  U { int x, y; } variant   = {5};
// static class  C { public: union { int x1; double x2; }; enum {} : 8; int y; } whacky = {{6}, 7};

int main(int, char*[]) /* noexcept */ {
  // (void) array;
  // (void) aggregate;
  // (void) variant;
  // (void) whacky;

  // std::printf("%u" "\r\n\n", (unsigned) sizeof((int[2]) {refl::member<0>(), refl::member<0>()}));
  (void) std::printf("%4.5s" "\r\n", refl::assert_aggregate_initialization<void>::value ? "true" : "false");
  (void) std::puts("");
  (void) std::printf("%4.5s" "\r\n", refl::can_aggregate_initialize<int[1],           2u>::value ? "false" : "true");
  (void) std::printf("%4.5s" "\r\n", refl::can_aggregate_initialize<int[2],           2u>::value ? "true"  : "false");
  (void) std::printf("%4.5s" "\r\n", refl::can_aggregate_initialize<struct aggregate, 3u>::value ? "true"  : "false");
  (void) std::puts("");
  (void) std::printf("%u"    "\r\n", (unsigned) refl::count_aggregate_initializers<int[1]>          ::value);
  (void) std::printf("%u"    "\r\n", (unsigned) refl::count_aggregate_initializers<int[2]>          ::value);
  (void) std::printf("%u"    "\r\n", (unsigned) refl::count_aggregate_initializers<struct aggregate>::value);

  // std::printf("\n" "[#]: %p %p" "\r\n", (void*) &array[0],     (void*) &array[1]);    for (void *const *member = refl::inspect(array);     NULL != *member; ++member) std::printf("%p" "\r\n", *member);
  // std::printf("\n" "[#]: %p %p" "\r\n", (void*) &aggregate.x,  (void*) &aggregate.y); for (void *const *member = refl::inspect(aggregate); NULL != *member; ++member) std::printf("%p" "\r\n", *member);
  // std::printf("\n" "[#]: %p %p" "\r\n", (void*) &variant  .x,  (void*) &variant  .y); for (void *const *member = refl::inspect(variant);   NULL != *member; ++member) std::printf("%p" "\r\n", *member);
  // std::printf("\n" "[#]: %p %p" "\r\n", (void*) &whacky   .x1, (void*) &whacky   .y); for (void *const *member = refl::inspect(whacky);    NULL != *member; ++member) std::printf("%p" "\r\n", *member);
}
