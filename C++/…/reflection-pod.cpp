#define REFL_MAX 10u
#include <ciso646>
#include <cstddef>
#include <cstdio>
#include <typeinfo>

/* ... */
union refl /* final */ {
  template <typename, std::size_t, bool>            class  can_aggregate_initialize;
  template <typename, bool = false>                 union  count_aggregate_initializers;
  template <typename, std::size_t, typename = void> struct info;

  private:
    enum /* : std::size_t */ { count_maximum = (REFL_MAX) };

    /* ... */
    template <typename> // ->> Stall evaluation until template substitution
    struct assert_aggregate_initialization;

    struct base_aggregate_initialization {
      template <typename, unsigned char>
      static bool const (&evaluate(...) /* noexcept */)[false + 1u];
    };

    template <typename base, typename addresser = void> // ->> Setup for `struct refl::info<…> refl::inspect(…);`
    struct inspect_aggregate_initialized { typedef struct refl::info<base, refl::count_aggregate_initializers<base, true>::value, addresser> type; };
      template <typename base>                     struct inspect_aggregate_initialized<base&>                          /* final */ : public refl::inspect_aggregate_initialized<base>                      {};
      template <typename base, typename addresser> struct inspect_aggregate_initialized<base const,          addresser> /* final */ : public refl::inspect_aggregate_initialized<base, void const>          {};
      template <typename base, typename addresser> struct inspect_aggregate_initialized<base const volatile, addresser> /* final */ : public refl::inspect_aggregate_initialized<base, void const volatile> {};
      template <typename base, typename addresser> struct inspect_aggregate_initialized<base volatile,       addresser> /* final */ : public refl::inspect_aggregate_initialized<base, void volatile>       {};

    template <std::size_t capacity, unsigned char = 0x00u> // ->> Patterned data member value
    struct member /* final */ {
      unsigned char                pattern;
      bool                   const patterned;
      std::size_t           *const size;
      std::type_info const **const type;
      unsigned char                value[capacity];

      /* ... */
      template <typename base, typename addresser, std::size_t arity>
      /* constexpr */ member(struct refl::info<base, arity, addresser>* const information, std::size_t const index, unsigned char const pattern, bool const patterned) /* noexcept */ :
        pattern  (pattern),
        patterned(patterned),
        size     (information -> sizes + index),
        type     (information -> types + index),
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

        for (std::size_t index = 0u; index != sizeof(type); ++index, refl::member<capacity>::next(&this -> pattern))
        this -> value[index] = not this -> patterned ? 0x00u : this -> pattern;

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
    union sentinel /* static_assert(false, …) */ { template <typename base, typename addresser> static base evaluate(struct refl::info<base, arity, addresser>* const, ...) /* noexcept = delete; */; };
      #if defined __clang__
      # pragma clang diagnostic push
      # pragma clang diagnostic ignored "-Wmissing-braces"
      # pragma clang diagnostic ignored "-Wmissing-field-initializers"
      #endif
        template <unsigned char _> union sentinel<0u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, 0u, addresser>* const,             std::size_t const,       unsigned char const)         /* noexcept */ {                                                   base const value = {};                                                                                                                                                                                                                                                                                                                                                                                                                                      return value; } };
        template <unsigned char _> union sentinel<1u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, 1u, addresser>* const information, std::size_t const index, unsigned char const pattern) /* noexcept */ { typedef struct refl::member<sizeof(base)> member; base const value = {member(information, 0u, pattern, index == 0u)};                                                                                                                                                                                                                                                                                                                                                                                         return value; } };
        template <unsigned char _> union sentinel<2u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, 2u, addresser>* const information, std::size_t const index, unsigned char const pattern) /* noexcept */ { typedef struct refl::member<sizeof(base)> member; base const value = {member(information, 0u, pattern, index == 0u), member(information, 1u, pattern, index == 1u)};                                                                                                                                                                                                                                                                                                                                          return value; } };
        template <unsigned char _> union sentinel<3u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, 3u, addresser>* const information, std::size_t const index, unsigned char const pattern) /* noexcept */ { typedef struct refl::member<sizeof(base)> member; base const value = {member(information, 0u, pattern, index == 0u), member(information, 1u, pattern, index == 1u), member(information, 2u, pattern, index == 2u)};                                                                                                                                                                                                                                                                                           return value; } };
        template <unsigned char _> union sentinel<4u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, 4u, addresser>* const information, std::size_t const index, unsigned char const pattern) /* noexcept */ { typedef struct refl::member<sizeof(base)> member; base const value = {member(information, 0u, pattern, index == 0u), member(information, 1u, pattern, index == 1u), member(information, 2u, pattern, index == 2u), member(information, 3u, pattern, index == 3u)};                                                                                                                                                                                                                                            return value; } };
        template <unsigned char _> union sentinel<5u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, 5u, addresser>* const information, std::size_t const index, unsigned char const pattern) /* noexcept */ { typedef struct refl::member<sizeof(base)> member; base const value = {member(information, 0u, pattern, index == 0u), member(information, 1u, pattern, index == 1u), member(information, 2u, pattern, index == 2u), member(information, 3u, pattern, index == 3u), member(information, 4u, pattern, index == 4u)};                                                                                                                                                                                             return value; } };
        template <unsigned char _> union sentinel<6u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, 6u, addresser>* const information, std::size_t const index, unsigned char const pattern) /* noexcept */ { typedef struct refl::member<sizeof(base)> member; base const value = {member(information, 0u, pattern, index == 0u), member(information, 1u, pattern, index == 1u), member(information, 2u, pattern, index == 2u), member(information, 3u, pattern, index == 3u), member(information, 4u, pattern, index == 4u), member(information, 5u, pattern, index == 5u)};                                                                                                                                              return value; } };
        template <unsigned char _> union sentinel<7u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, 7u, addresser>* const information, std::size_t const index, unsigned char const pattern) /* noexcept */ { typedef struct refl::member<sizeof(base)> member; base const value = {member(information, 0u, pattern, index == 0u), member(information, 1u, pattern, index == 1u), member(information, 2u, pattern, index == 2u), member(information, 3u, pattern, index == 3u), member(information, 4u, pattern, index == 4u), member(information, 5u, pattern, index == 5u), member(information, 6u, pattern, index == 6u)};                                                                                               return value; } };
        template <unsigned char _> union sentinel<8u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, 8u, addresser>* const information, std::size_t const index, unsigned char const pattern) /* noexcept */ { typedef struct refl::member<sizeof(base)> member; base const value = {member(information, 0u, pattern, index == 0u), member(information, 1u, pattern, index == 1u), member(information, 2u, pattern, index == 2u), member(information, 3u, pattern, index == 3u), member(information, 4u, pattern, index == 4u), member(information, 5u, pattern, index == 5u), member(information, 6u, pattern, index == 6u), member(information, 7u, pattern, index == 7u)};                                                return value; } };
        template <unsigned char _> union sentinel<9u, _> { template <typename base, typename addresser> static base evaluate(struct refl::info<base, 9u, addresser>* const information, std::size_t const index, unsigned char const pattern) /* noexcept */ { typedef struct refl::member<sizeof(base)> member; base const value = {member(information, 0u, pattern, index == 0u), member(information, 1u, pattern, index == 1u), member(information, 2u, pattern, index == 2u), member(information, 3u, pattern, index == 3u), member(information, 4u, pattern, index == 4u), member(information, 5u, pattern, index == 5u), member(information, 6u, pattern, index == 6u), member(information, 7u, pattern, index == 7u), member(information, 8u, pattern, index == 8u)}; return value; } };
      #if defined __clang__
      # pragma clang diagnostic pop
      #endif

    /* ... */
    template <class info>
    static info inspect(unsigned char inspected[]) {
      return info(inspected);
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
    union count_aggregate_initializers<base, false> {
      enum /* : std::size_t */ { value = 42u };
    };

    template <typename base>
    union count_aggregate_initializers<base, true> {
      private:
        template <std::size_t arity, bool>
        union counter { enum /* : std::size_t */ { value = refl::count_aggregate_initializers<base, true>::counter<arity + 1u, refl::count_maximum != arity + 1u and refl::can_aggregate_initialize<base, arity + 1u>::value>::value + (0u != arity and refl::can_aggregate_initialize<base, arity>::value) }; };
          template <std::size_t arity>
          union counter<arity, false> { enum /* : std::size_t */ { value = 0u }; };

      public:
        enum /* : std::size_t */ { value = refl::count_aggregate_initializers<base, true>::counter<0u, refl::assert_aggregate_initialization<base>::value>::value };
    };

    template <typename base, std::size_t arity, typename addresser>
    struct info /* final */ /* ->> Result of `refl::inspect(…)`; While more types are generated this way, this is more conservative with space requirements and thread-safety */ {
      template <class info>
      friend info refl::inspect(unsigned char[]);

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
          for (std::size_t index = 0u; arity != index; ++index)
          while (true) {
            unsigned char const *address    = NULL;
            std::size_t          count      = 0u;
            bool                 duplicate  = false;
            unsigned char        enumerator = pattern;
            base        const    sentinel   = refl::sentinel<arity>::evaluate(this, index, pattern);
            std::size_t const    size       = this -> sizes[index]; // ->> `sentinel` member byte size

            // ... --- CITE (Lapys) -> https://wandbox.org/permlink/90WlTdW1ntfmVecQ
            for (unsigned char const *iterator = &reinterpret_cast<unsigned char const&>(sentinel) + offset, *const end = iterator + (sizeof(base) - offset); end != iterator; ++iterator) {
              if (enumerator != *iterator) {
                iterator -= enumerator != pattern;
                goto reset;
              }

              count++;
              refl::member<capacity>::next(&enumerator);

              if (count == size) {
                if (NULL != address) {
                  duplicate = true;
                  break;
                }

                address = iterator - (size - 1u);
                goto reset;
              }

              continue;
              reset: {
                count      = 0u;
                enumerator = pattern;
              }
            }

            std::printf("[%u]: {duplicate: %4.5s, @:", (unsigned) index, duplicate ? "true" : "false");
              for (unsigned char const *i = &reinterpret_cast<unsigned char const&>(sentinel), *const n = i + sizeof(base); i != n; ++i)
              std::printf(" %02hX", (unsigned short) *i);
            std::printf(", type: \"%s\" size: %lu, pattern: %02hX..", types[index] -> name(), (unsigned long) size, (unsigned short) pattern);
              for ((count = size), (enumerator = pattern); count--; )
              refl::member<capacity>::next(&enumerator);
            std::printf("%02hX}" "\r\n", (unsigned short) enumerator - 1u);

            // ...
            for (count = size; count--; )
            refl::member<capacity>::next(&pattern); // ->> Reseeding once can be fine

            if (not duplicate) {
              std::size_t const location = NULL != address ? address - &reinterpret_cast<unsigned char const&>(sentinel) : offset;

              // ...
              std::printf("  \toffset @ %li (%s)" "\r\n", (long) location, NULL == address ? "estimated" : "reality");
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
    template <typename base, std::size_t arity, typename addresser>
    static std::size_t format(struct refl::info<base, arity, addresser> const& information, char const string[]) {
      (void) information;
      (void) string;

      return 0u; // or amount to be written
    }

    #if __cplusplus >= 201103L or defined _MSC_VER
      template <typename type>
      static typename refl::inspect_aggregate_initialized<type>::type inspect(type&& object) {
        return refl::inspect<refl::inspect_aggregate_initialized<type>::type>(const_cast<unsigned char*>(&reinterpret_cast<unsigned char const volatile&>(object)));
      }
    #else
      template <typename type> static typename refl::inspect_aggregate_initialized<type>::type inspect(type&                object) { return refl::inspect<typename refl::inspect_aggregate_initialized<type>::type>(const_cast<unsigned char*>(&reinterpret_cast<unsigned char const volatile&>(object))); }
      template <typename type> static typename refl::inspect_aggregate_initialized<type>::type inspect(type const&          object) { return refl::inspect<typename refl::inspect_aggregate_initialized<type>::type>(const_cast<unsigned char*>(&reinterpret_cast<unsigned char const volatile&>(object))); }
      template <typename type> static typename refl::inspect_aggregate_initialized<type>::type inspect(type const volatile& object) { return refl::inspect<typename refl::inspect_aggregate_initialized<type>::type>(const_cast<unsigned char*>(&reinterpret_cast<unsigned char const volatile&>(object))); }
      template <typename type> static typename refl::inspect_aggregate_initialized<type>::type inspect(type       volatile& object) { return refl::inspect<typename refl::inspect_aggregate_initialized<type>::type>(const_cast<unsigned char*>(&reinterpret_cast<unsigned char const volatile&>(object))); }
    #endif
};
  #if __cplusplus >= 201103L or defined _MSC_VER or (!defined __clang__ and (defined __ECC or defined __ICC or defined __ICL or defined __INTEL_COMPILER or defined __INTEL_COMPILER_BUILD_DATE or defined __INTEL_LLVM_COMPILER)) // --> __cpp_initializer_lists
    template <typename base>
    struct refl::assert_aggregate_initialization /* final */ : public refl::base_aggregate_initialization {
      using refl::base_aggregate_initialization::evaluate;
      template <typename type, typename refl::parse<sizeof(type{refl::member<0>(), 0x00u})>::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u];

      enum /* : bool */ { value = sizeof(bool[false + 1u]) == sizeof refl::assert_aggregate_initialization<base>::evaluate<unsigned char[1], static_cast<unsigned char>(0x00u)>("") };
    };

    template <typename base> class refl::can_aggregate_initialize<base, 1u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 1u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof(type {refl::member<0>()})>                                                                                                                                                        ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
    template <typename base> class refl::can_aggregate_initialize<base, 2u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 2u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof(type {refl::member<0>(), refl::member<0>()})>                                                                                                                                     ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
    template <typename base> class refl::can_aggregate_initialize<base, 3u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 3u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof(type {refl::member<0>(), refl::member<0>(), refl::member<0>()})>                                                                                                                  ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
    template <typename base> class refl::can_aggregate_initialize<base, 4u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 4u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof(type {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()})>                                                                                               ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
    template <typename base> class refl::can_aggregate_initialize<base, 5u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 5u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof(type {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()})>                                                                            ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
    template <typename base> class refl::can_aggregate_initialize<base, 6u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 6u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof(type {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()})>                                                         ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
    template <typename base> class refl::can_aggregate_initialize<base, 7u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 7u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof(type {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()})>                                      ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
    template <typename base> class refl::can_aggregate_initialize<base, 8u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 8u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof(type {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()})>                   ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
    template <typename base> class refl::can_aggregate_initialize<base, 9u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 9u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof(type {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()})>::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
  #elif defined __clang__ or defined __GNUC__ // ->> Compound literals (C99 extension)
  # if defined __clang__ // ->> Clang frontend
  #   pragma clang diagnostic push
  #   pragma clang diagnostic ignored "-Wc99-extensions"
  #   pragma clang diagnostic ignored "-Wmissing-braces"
  #   pragma clang diagnostic ignored "-Wmissing-field-initializers"
  # elif defined __GNUC__
  #   pragma GCC diagnostic push
  #   pragma GCC diagnostic ignored "-Wpedantic"
  # endif
    template <typename base>
    struct refl::assert_aggregate_initialization /* final */ : public refl::base_aggregate_initialization {
      using refl::base_aggregate_initialization::evaluate;
      template <typename type, typename refl::parse<sizeof (type) {refl::member<0>(), 0x00u}>::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u];

      enum /* : bool */ { value = sizeof(bool[false + 1u]) == sizeof refl::assert_aggregate_initialization<base>::evaluate<unsigned char[1], static_cast<unsigned char>(0x00u)>("") };
    };

    #if defined __apple_build_version__ or defined __clang_major__ or defined __clang_minor__ or defined __clang_patchlevel__ or defined __clang_version__ // ->> Clang compiler
      template <typename base> class refl::can_aggregate_initialize<base, 1u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 1u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, unsigned char> static bool const (&evaluate(char const[]) /* noexcept */)[sizeof (type) {refl::member<0>()}                                                                                                                                                         + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 2u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 2u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, unsigned char> static bool const (&evaluate(char const[]) /* noexcept */)[sizeof (type) {refl::member<0>(), refl::member<0>()}                                                                                                                                      + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 3u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 3u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, unsigned char> static bool const (&evaluate(char const[]) /* noexcept */)[sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>()}                                                                                                                   + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 4u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 4u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, unsigned char> static bool const (&evaluate(char const[]) /* noexcept */)[sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()}                                                                                                + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 5u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 5u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, unsigned char> static bool const (&evaluate(char const[]) /* noexcept */)[sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()}                                                                             + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 6u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 6u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, unsigned char> static bool const (&evaluate(char const[]) /* noexcept */)[sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()}                                                          + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 7u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 7u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, unsigned char> static bool const (&evaluate(char const[]) /* noexcept */)[sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()}                                       + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 8u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 8u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, unsigned char> static bool const (&evaluate(char const[]) /* noexcept */)[sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()}                    + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 9u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 9u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, unsigned char> static bool const (&evaluate(char const[]) /* noexcept */)[sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()} + 1u]; };
    #else
      template <typename base> class refl::can_aggregate_initialize<base, 1u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 1u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof (type) {refl::member<0>()}>                                                                                                                                                        ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 2u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 2u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof (type) {refl::member<0>(), refl::member<0>()}>                                                                                                                                     ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 3u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 3u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>()}>                                                                                                                  ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 4u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 4u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()}>                                                                                               ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 5u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 5u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()}>                                                                            ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 6u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 6u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()}>                                                         ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 7u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 7u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()}>                                      ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 8u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 8u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()}>                   ::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
      template <typename base> class refl::can_aggregate_initialize<base, 9u, true> /* final */ : private refl::base_aggregate_initialization { friend class refl::can_aggregate_initialize<base, 9u>; friend union refl::count_aggregate_initializers<base, true>; using refl::base_aggregate_initialization::evaluate; template <typename type, typename refl::parse<sizeof (type) {refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>(), refl::member<0>()}>::type> static bool const (&evaluate(char const[]) /* noexcept */)[true + 1u]; };
    #endif
  # if defined __clang__
  #   pragma clang diagnostic pop
  # elif defined __GNUC__
  #   pragma GCC diagnostic pop
  # endif
  #endif

/* Main */
struct A { enum {} : 0; };
struct B { struct {} _; };
union  C { struct { short _[2]; } a, b; };
struct D { struct { short _[2]; } a, b; };
struct E { struct { short _[2]; } a; short : 1; short b; };
struct F { short a; void *b; struct { char _[3]; } c;    struct {} d; int e; };
struct G { short a; void *b; char                  c[3]; struct {} d; int e; };
// struct F { int a; double b; unsigned char c[3]; struct {} d; };
// static union  U { int x, y; };
// static class  E { public: union { int x1; double x2; }; enum {} : 8; int y; };

int main(int, char*[]) /* noexcept */ {
  (void) std::printf("[int[1]]: %4.5s" "\r\n", refl::can_aggregate_initialize<int[1], 2u>::value ? "false" : "true");
  (void) std::printf("[int[2]]: %4.5s" "\r\n", refl::can_aggregate_initialize<int[2], 2u>::value ? "true"  : "false");
  (void) std::printf("[A]:      %4.5s" "\r\n", refl::can_aggregate_initialize<A,      0u>::value ? "true"  : "false");
  (void) std::printf("[B]:      %4.5s" "\r\n", refl::can_aggregate_initialize<B,      1u>::value ? "true"  : "false");
  (void) std::printf("[C]:      %4.5s" "\r\n", refl::can_aggregate_initialize<C,      1u>::value ? "true"  : "false");
  (void) std::printf("[D]:      %4.5s" "\r\n", refl::can_aggregate_initialize<D,      2u>::value ? "true"  : "false");
  (void) std::printf("[E]:      %4.5s" "\r\n", refl::can_aggregate_initialize<E,      2u>::value ? "true"  : "false");
  (void) std::printf("[F]:      %4.5s" "\r\n", refl::can_aggregate_initialize<F,      5u>::value ? "true"  : "false");
  (void) std::printf("[G]:      %4.5s" "\r\n", refl::can_aggregate_initialize<G,      5u>::value ? "true"  : "false");
  (void) std::puts("");
  (void) std::printf("[int[1]]: %u (%u)" "\r\n", (unsigned) refl::count_aggregate_initializers<int[1], false>::value, (unsigned) refl::count_aggregate_initializers<int[1], true>::value);
  (void) std::printf("[int[2]]: %u (%u)" "\r\n", (unsigned) refl::count_aggregate_initializers<int[2], false>::value, (unsigned) refl::count_aggregate_initializers<int[2], true>::value);
  (void) std::printf("[A]:      %u (%u)" "\r\n", (unsigned) refl::count_aggregate_initializers<A,      false>::value, (unsigned) refl::count_aggregate_initializers<A,      true>::value);
  (void) std::printf("[B]:      %u (%u)" "\r\n", (unsigned) refl::count_aggregate_initializers<B,      false>::value, (unsigned) refl::count_aggregate_initializers<B,      true>::value);
  (void) std::printf("[C]:      %u (%u)" "\r\n", (unsigned) refl::count_aggregate_initializers<C,      false>::value, (unsigned) refl::count_aggregate_initializers<C,      true>::value);
  (void) std::printf("[D]:      %u (%u)" "\r\n", (unsigned) refl::count_aggregate_initializers<D,      false>::value, (unsigned) refl::count_aggregate_initializers<D,      true>::value);
  (void) std::printf("[E]:      %u (%u)" "\r\n", (unsigned) refl::count_aggregate_initializers<E,      false>::value, (unsigned) refl::count_aggregate_initializers<E,      true>::value);
  (void) std::printf("[F]:      %u (%u)" "\r\n", (unsigned) refl::count_aggregate_initializers<F,      false>::value, (unsigned) refl::count_aggregate_initializers<F,      true>::value);
  (void) std::printf("[G]:      %u (%u)" "\r\n", (unsigned) refl::count_aggregate_initializers<G,      false>::value, (unsigned) refl::count_aggregate_initializers<G,      true>::value);
  (void) std::puts("");

  // ...
  (void) std::printf("[A]" "\r\n");
  refl::inspect(A());
  (void) std::puts("");

  (void) std::printf("[B]: {%u}" "\r\n", (unsigned) offsetof(B, _));
  refl::inspect(B());
  (void) std::puts("");

  (void) std::printf("[C]: {%u, %u}" "\r\n", (unsigned) offsetof(C, a), (unsigned) offsetof(C, b));
  refl::inspect(C());
  (void) std::puts("");

  (void) std::printf("[D]: {%u, %u}" "\r\n", (unsigned) offsetof(D, a), (unsigned) offsetof(D, b));
  refl::inspect(D());
  (void) std::puts("");

  (void) std::printf("[E]: {%u, %u}" "\r\n", (unsigned) offsetof(E, a), (unsigned) offsetof(E, b));
  refl::inspect(E());
  (void) std::puts("");

  (void) std::printf("[F]: {%u, %u, %u, %u, %u}" "\r\n", (unsigned) offsetof(F, a), (unsigned) offsetof(F, b), (unsigned) offsetof(F, c), (unsigned) offsetof(F, d), (unsigned) offsetof(F, e));
  refl::inspect(F());
  (void) std::puts("");

  (void) std::printf("[G]: {%u, %u, %u, %u, %u}" "\r\n", (unsigned) offsetof(G, a), (unsigned) offsetof(G, b), (unsigned) offsetof(G, c), (unsigned) offsetof(G, d), (unsigned) offsetof(G, e));
  refl::inspect(G());
  (void) std::puts("");
  // G g = {0, NULL, 0, 0, 0};
  // (void) g;
}
