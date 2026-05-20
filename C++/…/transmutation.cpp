#include <cstddef>
#include <cstdio>
#include <inttypes.h>
#include <new>

/* Main */
static unsigned n = 0u;
typedef struct text_ptr {
  char const *v;
  unsigned i;

  text_ptr (char const v[] = "...") : v(v),   i(++n) { (void) std::printf("+T:%X"    "\t" "\"%s\"" "\r\n", i,      v); }
  text_ptr (text_ptr const& t)      : v(t.v), i(++n) { (void) std::printf("+T:%X=%X" "\t" "\"%s\"" "\r\n", i, t.i, v); }
  ~text_ptr()                                        { (void) std::printf("-T:%X"    "\t" "\"%s\"" "\r\n", i,      v); }
} text_ptr;

struct T {
  union {
    alignas(text_ptr) unsigned char _;
    text_ptr                        t;
  };
  bool w;

  T()                                       : _(),  w(false) {}
  T(T const& t)                             : _(),  w(t.w)   { if (w) (void) ::new (static_cast<void*>(&this->t)) text_ptr(t.t); }
  template <typename type> T(type const& t) : t(t), w(true)  {}
  ~T()                                                       { if (w) t.~text_ptr(); }
}                 old_texts[] = {"one", "two", "three"}; // ->> Three times more (copy) constructions, three times more destruction-s
text_ptr         *new_texts   = NULL;
std::size_t const length      = 3u;

int main(int, char*[]) /* noexcept */ {
  (void) std::puts("=== === ===");

  #if true
    alignas(struct T) unsigned char copy[sizeof old_texts];

    for (std::size_t index = 0u; index != length; ++index)
    (void) ::new (static_cast<void*>(reinterpret_cast<struct T*>(copy + (sizeof(struct T) * index)))) struct T(old_texts[index]);

    for (std::size_t index = 0u; index != length; ++index)
    old_texts[index].~T();
  #else
    for (std::size_t index = 0u; index != length; ++index) /* → transform from `struct T` (with active union member `t`) to `text_ptr`? */ {
      unsigned char *const address = reinterpret_cast<unsigned char*>(old_texts) + (index * sizeof(text_ptr));
      struct T            &text    = *reinterpret_cast<struct T*>(reinterpret_cast<unsigned char*>(old_texts) + (index * sizeof(struct T)));

      if (address + sizeof(text_ptr) >= reinterpret_cast<unsigned char*>(old_texts) + (index * sizeof(struct T))) {
        struct T const copy = text;

        text.~T();
        (void) ::new (static_cast<void*>(address)) text_ptr(copy.t); // → placement pointer can’t be retrieved
      }

      else {
        (void) ::new (static_cast<void*>(address)) text_ptr(text.t); // → placement pointer can’t be retrieved
        text.~T();
      }
    }
  #endif

  (void) std::puts("=== === ===");

  #if true
    new_texts = ::new (static_cast<void*>(old_texts)) text_ptr[length] {
      reinterpret_cast<struct T*>(copy + (sizeof(struct T) * 0u)) -> t,
      reinterpret_cast<struct T*>(copy + (sizeof(struct T) * 1u)) -> t,
      reinterpret_cast<struct T*>(copy + (sizeof(struct T) * 2u)) -> t
    };

    for (std::size_t index = 0u; index != length; ++index)
    reinterpret_cast<struct T*>(copy + (sizeof(struct T) * index)) -> ~T();
  #else
    new_texts = std::launder(reinterpret_cast<text_ptr*>(::new (static_cast<void*>(old_texts)) unsigned char[sizeof(text_ptr) * length])); // --> std::start_lifetime_as_array<text_ptr>((void*) old_texts, length)
  #endif

  for (std::size_t index = 0u; index != length; ++index)
    (void) std::puts(new_texts[index].v); // → do stuff with `new_texts` as one would a live `text_ptr[]` object

  (void) std::puts("=== === ===");

  for (std::size_t index = length; index--; )
    new_texts[index].~text_ptr(); // → explicitly destruct `new_texts` (not the array, just the elements)

  (void) ::new (static_cast<void*>(old_texts)) struct T[length]; // → re-establish `old_texts` for automatic destruction
  (void) std::puts("=== === ===");
}
