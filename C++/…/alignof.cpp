#include <cstddef>
#include <cstdio>

/* ... ->> Measures `T` as sub-object (not object) and returns acceptable (not minimal) alignment. Over-alignment acknowledgement is implementation-specific */
template <typename T>
struct alignment {
  struct sentinel {
    unsigned char a;
    T b;
  };

  enum { value = sizeof(struct alignment<T>::sentinel) - sizeof(T) };
};

/* Main */
int main(int, char*[]) /* noexcept */ {
  (void) std::printf("%i" "\r\n", alignment<bool> ::value);
  (void) std::printf("%i" "\r\n", alignment<char> ::value);
  (void) std::printf("%i" "\r\n", alignment<short>::value);
  (void) std::printf("%i" "\r\n", alignment<int>  ::value);
  (void) std::printf("%i" "\r\n", alignment<long> ::value);
  (void) std::printf("%i" "\r\n", alignment<void*>::value);
}
