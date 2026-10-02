#include <ciso646>
#include <cstddef>
#include <cstdint>

static bool bounded(void* address, int array[], ::std::size_t length) /* noexcept */ {
  if (NULL != address and NULL != array) {
    // `std::less<void*>` supplies a strict total ordering for arbitrary pointers, but that ordering is not required to represent containment within array’s storage
    // an unrelated pointer may simply sort between the two boundaries — not ::std::less<void*>(address, begin) and ::std::less<void*>(address, end)
    #if defined UINTPTR_MAX and (                                       \
      defined __ANDROID__    or /* Android / Bionic */                  \
      defined __APPLE__      or /* macOS, iOS, etc. */                  \
      defined __CYGWIN__     or /* Cygwin */                            \
      defined __DragonFly__  or /* DragonFly BSD */                     \
      defined __EMSCRIPTEN__ or /* Emscripten / WebAssembly */          \
      defined __FreeBSD__    or /* FreeBSD */                           \
      defined __Fuchsia__    or /* Fuchsia */                           \
      defined __GNU__        or /* GNU/Hurd */                          \
      defined __HAIKU__      or /* Haiku */                             \
      defined __linux__      or /* GNU/Linux and other Linux systems */ \
      defined __NetBSD__     or /* NetBSD */                            \
      defined __OpenBSD__    or /* OpenBSD */                           \
      defined __QNXNTO__     or /* QNX Neutrino */                      \
      defined __sun          or /* Solaris / illumos */                 \
      defined __wasi__       or /* WASI */                              \
      defined __wasm__       or /* Generic WebAssembly */               \
      defined _AIX           or /* IBM AIX */                           \
      defined _WIN32            /* Windows: MSVC, MinGW, Clang, etc. */ \
    )
      // Assumption for these supported ABIs: uintptr_t represents a flat, byte-addressed address space whose integer ordering matches addresses.
      // Also consider CHERI (Capability Hardware Enhanced RISC Instructions)
      return
        (reinterpret_cast< ::std::uintptr_t>(address) >= reinterpret_cast< ::std::uintptr_t>(array)) and
        (reinterpret_cast< ::std::uintptr_t>(address)  - reinterpret_cast< ::std::uintptr_t>(array)) / sizeof(int) < length;
    #else
      // Strictly portable: test every possible byte address by equality.
      for (::std::size_t index = 0u; index != length * sizeof(int); ++index) {
        if (address == &reinterpret_cast<unsigned char*>(array)[index]) // --> &::std::as_bytes(::std::span<int>(array, length))[index]
        return true;
      }
    #endif
  }

  return false;
}
