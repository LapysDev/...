#include <cstddef>

/* ... */
namespace std {
  static struct heap_t  const *const heap  = NULL;
  static struct stack_t const *const stack = NULL;

  inline std::size_t pop() throw();
  inline void push() throw();
}

inline void* operator new  (std::size_t const, std::stack_t const* const&, std::size_t const = 1u) throw();
inline void* operator new[](std::size_t const, std::stack_t const* const&, std::size_t const = 1u) throw();

/* ... */
struct std::stack_t /* final */ {
  friend std::size_t pop() throw();
  friend void push() throw();

  friend void* ::operator new  (std::size_t const, std::stack_t const* const&, std::size_t const) throw();
  friend void* ::operator new[](std::size_t const, std::stack_t const* const&, std::size_t const) throw();

  private:
    enum { capacity = 16 * 1024 };
    static unsigned char memory[std::stack_t::capacity];
    static unsigned char *pointer;
    static std::size_t size;

    // ...
    stack_t() throw();
    stack_t(std::stack_t const&) throw();

    // ...
    void operator =(std::stack_t const&) throw();
};

unsigned char std::stack_t::memory[std::stack_t::capacity];
unsigned char *std::stack_t::pointer = std::stack_t::memory;
std::size_t std::stack_t::size = 0u;

/* ... */
std::size_t std::pop() throw() {
  std::size_t const size = std::stack_t::size;

  std::stack_t::pointer -= size;
  std::stack_t::size = 0u;

  return size;
}

void std::push() throw() {}

// ...
void* operator new(std::size_t const size, std::heap_t const* const&) throw() {
  return ::operator new(size);
}

void* operator new[](std::size_t const size, std::heap_t const* const&) throw() {
  return ::operator new[](size);
}

void* operator new(std::size_t const size, std::stack_t const* const& indicator, std::size_t const count) throw() {
  if (&indicator == &std::stack) {
    void *const allocation = std::stack_t::pointer;

    std::stack_t::pointer += count * size;
    std::stack_t::size += count * size;

    return allocation;
  }

  return NULL;
}

void* operator new[](std::size_t const size, std::stack_t const* const& indicator, std::size_t const count) throw() {
  return ::operator new(size, indicator, count);
}

/* Main */
int main() {
  struct Object {};

  Object *fixed;
  Object *unit;
  Object *variable;

  // allocate the arrays
  std::push();
    fixed    = new (std::stack) Object[10];
    unit     = new (std::stack) Object;

    variable = new (std::stack, 10) Object;
    variable = new (std::stack, 10) Object[2];
  std::pop();

  // ...
  static_cast<void>(fixed);
  static_cast<void>(unit);
  static_cast<void>(variable);

  // struct A { static void* operator new[](std::size_t const, void* const) { return NULL; } };
  // (void) new (NULL) A[2048];
}
