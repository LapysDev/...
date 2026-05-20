#include <cstddef>

/* ... */
template <std::size_t...>
union sequence {};
  template <std::size_t size>
  union make_sequence {
    union builtin;
    union divide_and_conquer;
    union linear;
    union modulo;
  };
    // [Built-in]
    #if __cplusplus < 201402L
    # if defined(__clang__) || defined(_MSC_VER)
    #   if false == defined(__clang__) || __has_builtin(__make_integer_seq)
          template <std::size_t size>
          union make_sequence<size>::builtin {
            private:
              template <typename base, base... indexes>
              union value {
                typedef sequence<indexes...> type;
              };

            public:
              typedef typename __make_integer_seq<value, std::size_t, size>::type type;
          };
    #   else
          template <std::size_t size>
          union make_sequence<size>::builtin;
    #   endif
    # elif defined(__GNUC__)
    #   if __has_builtin(__integer_pack)
          template <std::size_t size>
          union make_sequence<size>::builtin {
            typedef sequence<__integer_pack(size)...> type;
          };
    #   else
          template <std::size_t size>
          union make_sequence<size>::builtin;
    #   endif
    # endif
    #else
    # include <utility>

      template <std::size_t size>
      union make_sequence<size>::builtin {
        private:
          template <class>
          union value;

          template <std::size_t... indexes>
          union value<std::index_sequence<indexes...> > {
            typedef sequence<indexes...> type;
          };

        public:
          typedef typename value<typename std::make_index_sequence<size>::type>::type type;
      };
    #endif

    // [Divide & Conquer]
    template <std::size_t size>
    union make_sequence<size>::divide_and_conquer {
      private:
        template <class, class>
        union concatenate;

        template <std::size_t... indexesA, std::size_t... indexesB>
        union concatenate<sequence<indexesA...>, sequence<indexesB...> > {
          typedef sequence<indexesA..., (indexesB + sizeof...(indexesA))...> type;
        };

        // ...
        template <unsigned char control, std::size_t remainder>
        union value {
          typedef typename concatenate<
            typename value<control, remainder / 2u>::type,
            typename value<control, remainder - (remainder / 2u)>::type
          >::type type;
        };

        template <unsigned char control> union value<control, 0u> { typedef sequence<>   type; };
        template <unsigned char control> union value<control, 1u> { typedef sequence<1u> type; };

      public:
        typedef typename value<0u, size>::type type;
    };

    // [Linear]
    template <std::size_t size>
    union make_sequence<size>::linear {
      private:
        template <std::size_t count, std::size_t... indexes>
        union value {
          typedef typename value<count - 1u, count, indexes...>::type type;
        };

        template <std::size_t... indexes>
        union value<0u, indexes...> {
          typedef sequence<indexes...> type;
        };

      public:
        typedef typename value<size>::type type;
    };

    // [Modulo]
    template <std::size_t size>
    union make_sequence<size>::modulo {
      private:
        // ...
        template <std::size_t, std::size_t, class, class>
        union value;

        template <std::size_t maximum, std::size_t remainder, std::size_t... current, std::size_t... next>
        union value<maximum, remainder, sequence<current...>, sequence<next...> > {
          typedef typename value<
            maximum,
            (maximum < remainder ? remainder - maximum : 0u),
            sequence<current..., (next + sizeof...(current))...>,
            typename make_sequence<(maximum < remainder ? maximum : remainder)>::linear::type
          >::type type;
        };

        template <std::size_t maximum, std::size_t... current, std::size_t... last>
        union value<maximum, 0u, sequence<current...>, sequence<last...> > {
          typedef sequence<current..., (last + sizeof...(current))...> type;
        };

      public:
        typedef typename value<500u, size, sequence<>, typename make_sequence<0u>::linear::type>::type type;
    };

/* Main */
int main() {
  // typename make_sequence<+900u>::builtin           ::type{} + 1; // ->> The fastest and magical
  // typename make_sequence<+900u>::divide_and_conquer::type{} + 1;
  // typename make_sequence< 900u>::linear            ::type{} + 1;
  // typename make_sequence<+900u>::modulo            ::type{} + 1;
}
