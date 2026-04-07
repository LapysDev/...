#include <cstddef>
#include <cstdio>
#include <new>
#include <stdint.h>

/* ... ->> Constant-time initialization is the user’s responsibility i.e. `array<…, FIXED_SIZE_ONLY>` */
struct array_internals {
  template <typename, std::size_t>
  friend struct array;

  private:
    static std::size_t count;
    static void const *recent;
};
  std::size_t array_internals::count  = 0u;
  void const *array_internals::recent = NULL;

template <typename base, std::size_t initial = 0u>
struct array {
  enum { minimum = sizeof(base) < sizeof(std::size_t) ? (initial - 1u) + (sizeof(std::size_t) / sizeof(base)) : initial }; // --> std::size_t
  private:
    union param { unsigned char unassigned : 1; base _; };
    union storage {
      private:
        unsigned char _ : 1;
        base value;

        void initialize(base const *const value = NULL) {
          uintptr_t const address = reinterpret_cast<uintptr_t>(static_cast<void const*>(this));
          uintptr_t const recent  = reinterpret_cast<uintptr_t>(array_internals::recent);

          // ...
          NULL == value ? std::printf(array_internals::count == minimum - 1u ? "+" : "-") : std::printf("%i", (int) *value);
          if (array_internals::count == minimum - 1u) {
            // PUT INDEX TO HEAP MEMORY HERE @ `storage* this`
            (void) std::printf("{%zu}" "\r\n", array_internals::count);
          } else { std::printf("{%zu}" " ", array_internals::count); }
          std::printf(minimum == 1 ? "\r\n" : "");

          if (array_internals::count >= minimum - 0u || sizeof(base) < (address < recent ? recent - address : address - recent))
          array_internals::count = 0u;

          array_internals::count++;
          array_internals::recent = this;
        }

      public:
        storage()                  : _() { this -> initialize(NULL); }
        storage(base const& value) : _() { this -> initialize(reinterpret_cast<base const*>(&reinterpret_cast<unsigned char const&>(value))); }
    };

    static union storage const *recent;

  public:
    union storage _[minimum];
    bool add(union param const& element1 = param(), union param const& element2 = param(), union param const& element3 = param());
};

template <typename base>
struct array<base, 0u> : array_internals {
  public: union { struct array_internals __; base *_; };
};

/* Main */
int main(void) {
  array<long, 0u> a;
  array<long, 3u> b[] = {{1, 2, 3}, {4, 5, 6}};
  array<unsigned char, 3u> c[] = {{10, 20, 30}, {40, 50, 60}, {70}, {80}};
  array<short, 1u> d[] = {{100}, {200}, {300}};
  array<long long, 1u> e[] = {{400}, {500}, {600}};

  (void) a;
  (void) b;
  (void) c;
  (void) d;
  (void) e;
}
