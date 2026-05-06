#include <cassert>

template <bool, typename = void, typename = void> struct conditional;
template       <typename baseA, typename baseB>   struct conditional<false, baseA, baseB> { typedef baseB type; };
template       <typename base>                    struct conditional<false, base,  void>  {};
template       <typename base>                    struct conditional<true,  void,  base>  {};
template       <typename baseA, typename baseB>   struct conditional<true,  baseA, baseB> { typedef baseA type; };

/* Main */
template <unsigned count, typename = typename conditional<
  (count > 5u),
  int[1], // ->> Keep this for explicit specialization
  int[2]  // ->> Remove this to prevent this instantiation
>::type> struct greater_than_5 { static bool value[false + 1]; };

template <unsigned count>
struct greater_than_5<count, int[1]> { static bool value[true + 1]; };

int main(int, char*[]) /* noexcept */ {
  assert(sizeof greater_than_5<1>::value == sizeof(bool[false + 1])); // ->> expected less than/equals 5
  assert(sizeof greater_than_5<9>::value == sizeof(bool[true  + 1])); // ->> expected greater than 5
}
