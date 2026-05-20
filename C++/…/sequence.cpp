// ...
template <typename base, base...>
struct integer_sequence final {};

// ...
template <std::size_t... indexes>                    struct index_sequence                    final { typedef struct   integer_sequence<std::size_t,     indexes...>            type; };
template <std::size_t... indexes>                    struct index_sequence<0u,    indexes...> final { typedef struct   integer_sequence<std::size_t, 0u, indexes...>            type; };
template <std::size_t index, std::size_t... indexes> struct index_sequence<index, indexes...> final { typedef typename index_sequence<index - 1u, index - 0u, indexes...>::type type; };

// ...
template <std::size_t count> struct make_index_sequence     final { typedef typename index_sequence<count - 1u>::type type; };
template <>                  struct make_index_sequence<0u> final { typedef typename index_sequence<>          ::type type; };
