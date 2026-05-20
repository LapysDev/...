#include <cstddef>
#include <cstdio>
#include <new>
#include <stdint.h>

/* ... ->> Constant-time initialization is the user’s responsibility i.e. `array<…, FIXED_SIZE_ONLY>` */
struct array_internals {
  template <typename, std::size_t>
  friend struct array;

  private:
    static thread_local std::size_t count;
    static thread_local void const *recent;
};
  thread_local std::size_t array_internals::count  = 0u;
  thread_local void const *array_internals::recent = NULL;

template <typename base, std::size_t initial = 0u>
struct array {
  union storage {
    private:
      unsigned char _ : 1;
      base value;

      void initialize(base const *const value = NULL) {
        uintptr_t const address = reinterpret_cast<uintptr_t>(static_cast<void const*>(this));
        uintptr_t const recent  = reinterpret_cast<uintptr_t>(array_internals::recent);

        // ...
        if (array_internals::count >= initial - 0u || sizeof(base) < (address < recent ? recent - address : address - recent))
        array_internals::count = 0u;

        (void) std::printf("[%zu] => ", array_internals::count);
          if (NULL == value) (void) std::printf("%c", array_internals::count == initial - 1u ? '+' : '-');
          else               (void) std::printf("%i", (int) *value);
        (void) std::printf("%1.3s", array_internals::count == initial - 1u ? "\r\n" : ", \t");

        array_internals::count++;
        array_internals::recent = this;
      }

    public:
      storage()                  : _() { this -> initialize(NULL); }
      storage(base const& value) : _() { this -> initialize(reinterpret_cast<base const*>(&reinterpret_cast<unsigned char const&>(value))); }
  } _[initial];
};

template <typename base>
struct array<base, 0u> : array_internals {
  public: union { struct array_internals __; base *_; };
};

/* Main */
int main(void) {
  array<long,          0u> a;
  array<long,          3u> b[] = {{1, 2, 3}, {4, 5, 6}};
  array<unsigned char, 3u> c[] = {{10, 20, 30}, {40, 50, 60}, {70}, {80}};
  array<short,         2u> d[] = {{100, 200}, {300}};
  array<long long,     2u> e[] = {{400}, {500, 600}};
  array<int,           1u> f[] = {{1000}, {2000}, {3000}};

  (void) a;
  (void) b;
  (void) c;
  (void) d;
  (void) e;
  (void) f;
}
