/* POSIX:   rm -f ./circular-buffer;   clear && clang++ -ffast-math                -fno-exceptions -fno-rtti -fomit-frame-pointer -march=native -O3 -pedantic-errors -std=c++98 -Wall -Wextra -Wno-c++11-long-long                      circular-buffer.cpp           -o circular-buffer     && ./circular-buffer;    rm -f ./circular-buffer */
/* Windows: del circular-buffer.exe && cls   && clang++ -ffast-math -ffreestanding -fno-exceptions -fno-rtti -fomit-frame-pointer -march=native -O3 -pedantic-errors -std=c++98 -Wall -Wextra -Wno-c++11-long-long -Wno-unknown-pragmas circular-buffer.cpp -lonecore -o circular-buffer.exe && circular-buffer.exe & del circular-buffer.exe */
#include <ciso646>  // --> and, or
#include <cstdio>   // --> ::std::printf(…)
#include <cstdlib>  // --> EXIT_SUCCESS, NULL; ::std::size_t
#include <stdint.h> // --> SIZE_MAX
#include <new>      // --> ::new
#if defined _WIN32
# include <climits>   // --> CHAR_BIT
# include <windows.h> // --> ::DWORD, ::HANDLE, ::LPCWSTR, ::LPSECURITY_ATTRIBUTES, ::LPSYSTEM_INFO, ::LPVOID, ::MEM_EXTENDED_PARAMETER, ::PVOID, ::SYSTEM_INFO, ::ULONGLONG; FALSE, INVALID_HANDLE_VALUE, MEM_RELEASE, MEM_REPLACE_PLACEHOLDER, MEM_RESERVE, MEM_RESERVE_PLACEHOLDER, PAGE_NOACCESS, PAGE_READWRITE; ::GetSystemInfo(…), ::VirtualAlloc2(…), ::VirtualFree(…)
#
# pragma comment(lib, "onecore.lib")

/* ... */
static void* allocate(::std::size_t const size, ::std::size_t const count, wchar_t const name[] = NULL) /* noexcept */ {
  ::SYSTEM_INFO systemInformation = {};

  // ...
  ::GetSystemInfo(static_cast< ::LPSYSTEM_INFO>(&systemInformation));

  if (count and size and SIZE_MAX / count >= size and 0u == size % systemInformation.dwAllocationGranularity)
  if (unsigned char *const allocation = static_cast<unsigned char*>(::VirtualAlloc2(
    static_cast< ::HANDLE>(NULL),                         // ->> This process
    static_cast< ::PVOID> (NULL),                         // ->> Any base address
    count * size,                                         //
    MEM_RESERVE | MEM_RESERVE_PLACEHOLDER, PAGE_NOACCESS, // ->> Allocation policies and page permission
    static_cast< ::MEM_EXTENDED_PARAMETER*>(NULL), 0u     // ->> No extended parameters
  )))
  for (unsigned char *view = allocation; ; view += size) {
    if (view == &allocation[size * (count - 1u)]) {
      if (::HANDLE const mapping = ::CreateFileMappingW(
        INVALID_HANDLE_VALUE,                                                                     //
        static_cast< ::LPSECURITY_ATTRIBUTES>(NULL),                                              // ->> No child process inheritance with default security descriptor
        PAGE_READWRITE,                                                                           //
        static_cast< ::DWORD>  (static_cast< ::ULONGLONG>(size) >> (CHAR_BIT * sizeof(::DWORD))), // ->> High `size`
        static_cast< ::DWORD>  (static_cast< ::ULONGLONG>(size) >> (CHAR_BIT * 0u)),              // ->> Low  `size`
        static_cast< ::LPCWSTR>(name)                                                             //
      ))
      for (unsigned char *subview = allocation; ; subview += size) {
        if (subview == &allocation[count * size])
        return (void) ::CloseHandle(mapping), allocation;

        if (static_cast< ::PVOID>(NULL) == ::MapViewOfFile3(mapping, static_cast< ::HANDLE>(NULL), static_cast< ::PVOID> (subview), 0uL, size, MEM_REPLACE_PLACEHOLDER, PAGE_READWRITE, static_cast< ::MEM_EXTENDED_PARAMETER*>(NULL), 0uL))
        (void) ::CloseHandle(mapping); // ->> Split into `count` segments
      }
    }

    else if (FALSE != ::VirtualFree(static_cast< ::LPVOID>(view), size, MEM_PRESERVE_PLACEHOLDER | MEM_RELEASE))
    continue; // ->> Ready into `count` segments

    for (; ; view -= size) {
      ::VirtualFree(static_cast< ::LPVOID>(view), 0u, MEM_RELEASE);
      if (allocation == view) return NULL;
    }
  }

  return NULL;
}

static void* allocate(::std::size_t const size, ::std::size_t const count, char const name[] = NULL) /* noexcept */ {
  (void) name; // ->> Widen multibyte
  return ::allocate(size, count, static_cast<wchar_t const*>(NULL));
}
#elif defined __APPLE__ or defined __unix__
# include <fcntl.h>     // --> O_CREAT, O_EXCL, O_RDWR
# include <sys/mman.h>  // --> MAP_ANONYMOUS, MAP_FAILED, MAP_FIXED, MAP_PRIVATE, MAP_SHARED, PROT_NONE, PROT_READ, PROT_WRITE; ::shm_open(…)
# include <sys/stat.h>  // --> S_IRUSR, S_IWUSR
# include <sys/types.h> // --> ::off_t
# include <unistd.h>    // --> _SC_PAGESIZE; ::close(…), ::ftruncate(…), ::sysconf(…)

/* ... */
static void* allocate(::std::size_t const size, ::std::size_t const count, /* [[nonnull]] */ char const name[]) /* noexcept */ {
  long const pageSize = ::sysconf(_SC_PAGESIZE);

  // ...
  if (0u != count and NULL != name and pageSize > 1L and 0u != size and SIZE_MAX / count >= size and 0u == size % pageSize) {
    int const descriptor = ::shm_open(name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (descriptor != -1) {
      (void) ::shm_unlink(name);
      if (::ftruncate(descriptor, static_cast< ::off_t>(size)) != -1) {
        unsigned char *allocation = static_cast<unsigned char*>(::mmap(
          static_cast<void*>(NULL),               // ->> Any base address
          count * size,                           //
          PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, // ->> Page permission and allocation policies
          -1,                                     // ->> No file descriptor, must be `-1` when using `MAP_ANON` or `MAP_ANONYMOUS`
          0                                       // ->> Offset
        ));

        // ...
        if ((allocation = MAP_FAILED == allocation ? NULL : allocation))
        for (unsigned char *view = allocation; view != &static_cast<unsigned char*>(allocation)[count * size]; view += size)
        if (MAP_FAILED == ::mmap(static_cast<void*>(view), size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, descriptor, 0)) {
          (void) ::munmap(allocation, count * size);
          allocation = NULL;

          break;
        }

        (void) ::close(descriptor);
        return allocation;
      }
    }
  }

  return NULL;
}
#endif

/* Main */
int main(int, char*[]) /* noexcept */ {
  ::std::size_t const count    = 4u;
  ::std::size_t const capacity = 65536u;

  // ...
  if (unsigned char *const memory = static_cast<unsigned char*>(::allocate(capacity, count, "ring")))
  for (
    // ->> Write once
    struct T /* final */ {
      char const *id;
      unsigned    value : 12;
    } const _ = {"Lapys", 42u}, *data = ::new (memory) struct T const(_);

    // ->> Read `count` places
    data != static_cast<void*>(&memory[count * capacity]);
    data = reinterpret_cast<struct T const*>(reinterpret_cast<unsigned char const*>(data) + capacity)
  ) (void) ::std::printf("%p {id: \"%1.64s\", value: 0x%03X}" "\r\n", static_cast<void const*>(data), data -> id, data -> value);
}
