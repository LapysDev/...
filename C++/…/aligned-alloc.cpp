#include <version>

#if defined __APPLE__ || defined __bsdi__ || defined __CYGWIN__ || defined __DragonFly__ || defined __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ || defined __FreeBSD__ || defined __FreeBSD_version || defined __gnu_linux__ || defined __linux || defined __linux__ || defined __MACH__ || defined __NETBSD__ || defined __NETBSD_version || defined __OpenBSD__ || defined __OS400__ || defined __QNX__ || defined __QNXNTO__ || defined __sun || defined __SVR4 || defined __svr4__ || defined __sysv__ || defined __unix || defined __unix__ || defined __VMS || defined __VMS_VER || defined _NTO_VERSION || defined _POSIX_SOURCE || defined _SYSTYPE_SVR4 || defined _XOPEN_SOURCE || defined linux || defined NetBSD0_8 || defined NetBSD0_9 || defined NetBSD1_0 || defined OpenBSD2_0 || defined OpenBSD2_1 || defined OpenBSD2_2 || defined OpenBSD2_3 || defined OpenBSD2_4 || defined OpenBSD2_5 || defined OpenBSD2_6 || defined OpenBSD2_7 || defined OpenBSD2_8 || defined OpenBSD2_9 || defined OpenBSD3_0 || defined OpenBSD3_1 || defined OpenBSD3_2 || defined OpenBSD3_3 || defined OpenBSD3_4 || defined OpenBSD3_5 || defined OpenBSD3_6 || defined OpenBSD3_7 || defined OpenBSD3_8 || defined OpenBSD3_9 || defined OpenBSD4_0 || defined OpenBSD4_1 || defined OpenBSD4_2 || defined OpenBSD4_3 || defined OpenBSD4_4 || defined OpenBSD4_5 || defined OpenBSD4_6 || defined OpenBSD4_7 || defined OpenBSD4_8 || defined OpenBSD4_9 || defined sun || defined unix || defined VMS
# include <stdlib.h>
# if _ISOC11_SOURCE
    void* ::aligned_alloc(std::size_t alignment, std::size_t size); //
    void* ::realloc      (void*       address,   std::size_t size); // ->> Overrides `::aligned_alloc(...)`
    void  ::free         (void*       address);                     //
# elif _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
    void* ::posix_memalign(void** pointer, std::size_t alignment, std::size_t size); //
    void* ::realloc       (void*  address,                        std::size_t size); // ->> Overrides `::posix_memalign(...)`; Unspecified compatibility with `::posix_memalign(...)`
    void  ::free          (void*  address);                                          //
# else
#   include <malloc.h>
    void* ::memalign(std::size_t alignment, std::size_t size); // ->> `alignment` is a power of two
    void* ::realloc (void*       address,   std::size_t size); // ->> Overrides `::memalign(...)`; Unspecified compatibility with `::memalign(...)`
    void  ::free    (void*       address);                     //
# endif
#elif defined __INTEL_COMPILER // ->> Intel C++ Compiler Classic for C++ (ICC), not Intel oneAPI DPC++/ C++ Compiler (ICX)
  _mm_malloc(std::size_t size, std::size_t alignment);
  _mm_free  (void* address);
#elif defined _MSC_VER // ->> `_MSVC_LANG` unnecessary afterward
  ::_aligned_malloc (               std::size_t size, std::size_t alignment); //
  ::_aligned_realloc(void* address, std::size_t size, std::size_t alignment); // ->> Overrides `::_aligned_malloc(...)`
  ::_aligned_free   (void* address);                                          //
#elif __cplusplus >= 201703L
# include <cstdlib>
  std::aligned_alloc(std::size_t alignment, std::size_t size); //
  std::realloc      (void* address,         std::size_t size); // ->> Overrides `std::aligned_alloc(...)`
  std::free         (void* address);                           //
#elif defined __cpp_aligned_new // --> 201606L
# include <new>
  ::new   (std::size_t size, std::align_val_t alignment, std::nothrow_t const&);
  ::delete(void* address,    std::align_val_t alignment);
#elif defined __NT__ || defined __TOS_WIN__ || defined __WIN32__ || defined __WINDOWS__ || defined _WIN16 || defined _WIN32 || defined _WIN32_WCE || defined _WIN64
# include <windows.h>
# if __cplusplus >= 201703L
#   include <memory>
# endif
  void         *allocation           = NULL;
  HANDLE  const currentProcess       = ::GetCurrentProcess();
  HMODULE const kernelbase           = ::LoadLibraryExW(L"kernelbase.dll", static_cast<HANDLE>(NULL), 0x00u); // --> mincore.lib
  SIZE_T  const minimumLargePageSize = ::GetLargePageMinimum();
    PVOID (*const VirtualAlloc2)(HANDLE, PVOID, SIZE_T, ULONG, ULONG, MEM_EXTENDED_PARAMETER, ULONG) = reinterpret_cast<PVOID (*)(HANDLE, PVOID, SIZE_T, ULONG, ULONG, MEM_EXTENDED_PARAMETER, ULONG)>(reinterpret_cast<void*>(NULL != kernelbase ? ::GetProcAddress(kernelbase, "VirtualAlloc2") : NULL));

  // ...
  (void) ::SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_SYSTEM32);

  if (minimumLargePageSize % alignment && minimumLargePageSize % size)
  allocation = ::VirtualAllocEx(currentProcess, static_cast<LPVOID>(NULL), std::size_t size, MEM_COMMIT | MEM_LARGE_PAGES | MEM_RESERVE, PAGE_READWRITE);

  if (NULL == allocation && NULL != VirtualAlloc2) {
    MEM_ADDRESS_REQUIREMENTS allocationExtendedParameterRequirements;
    MEM_EXTENDED_PARAMETER   allocationExtendedParameters[1];

    // ...
    allocationExtendedParameterRequirements.Alignment             = std::size_t alignment;                     // ->> `alignment >= 65536zu` and `alignment` is a power of two
    allocationExtendedParameterRequirements.HighestEndingAddress  = static_cast<PVOID>(NULL);                  //
    allocationExtendedParameterRequirements.LowestStartingAddress = static_cast<PVOID>(NULL);                  //
    allocationExtendedParameters -> Pointer                       = &allocationExtendedParameterRequirements;  //
    allocationExtendedParameters -> Type                          = ::MemExtendedParameterAddressRequirements; //

    allocation = VirtualAlloc2(currentProcess, static_cast<PVOID>(NULL), std::size_t size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, allocationExtendedParameters, 1u);
  }

  if (NULL == allocation) {
    #if __cplusplus >= 201703L
      allocation = ::VirtualAllocEx(currentProcess, static_cast<LPVOID>(NULL), std::size_t size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
      std::align(std::size_t alignment, sizeof(T), allocation, std::size_t size);
    #else
      allocation = ::VirtualAllocEx(currentProcess, static_cast<LPVOID>(NULL), std::size_t size + (std::size_t alignment - 1u), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
      // ... ->> Manually align `::VirtualAllocEx(...)`
    #endif
  }

  ::VirtualFreeEx(currentProcess, LPVOID address = static_cast<LPVOID>(allocation), std::size_t size + ..., MEM_RELEASE); // ->> `::VirtualAlloc(...)` can be resized
#elif __cplusplus >= 201703L
# include <cstdlib>
# include <memory>
  std::align  (std::size_t alignment, sizeof(T), std::malloc(std::size_t size), std::size_t size); //
  std::realloc(void* address, std::size_t size);                                                   // ->> Overrides `std::align(..., std::malloc(...), ...)`
  std::free   (void* address);                                                                     //
#else
# include <cstdlib>
  std::malloc (               std::size_t size + (std::size_t alignment - 1u)); // ->> Manually align `std::malloc(...)`
  std::realloc(void* address, std::size_t size);                                // ->> Overrides      `std::malloc(...)`
  std::free   (void* address);                                                  //
#endif

// void* next(std::size_t const alignment = sizeof(allocation_t)) const /* noexcept */ throw() {
//   #if defined UINTPTR_MAX
//     uintptr_t const base = reinterpret_cast<uintptr_t>(this -> buffer);
//     return &this -> buffer[((base + (alignment - 1u)) & ~static_cast<uintptr_t>(alignment - 1u)) - base];
//   #else
//     static std::size_t const base = offsetof(allocator, allocator::buffer);
//     return &this -> buffer[((base + (alignment - 1u)) & ~static_cast<std::size_t>(alignment - 1u)) - base];
//   #endif

//   if (0u == base % alignment) return buffer;
//   if (alignment > base)       return &buffer[alignment - (base * (alignment / base))];
//   if (alignment < base)       return &buffer[(alignment * ((base / alignment) + 1u)) - base];

//   return NULL;
// }
