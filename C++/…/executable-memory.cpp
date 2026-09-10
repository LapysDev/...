/* POSIX:   rm -f ./executable-memory;   clear && clang++ -ffast-math                -fno-exceptions -fno-rtti -fomit-frame-pointer -march=native -O3 -pedantic-errors -std=c++98 -Wall -Wextra                      executable-memory.cpp           -o executable-memory     && ./executable-memory;    rm -f ./executable-memory */
/* Windows: del executable-memory.exe && cls   && clang++ -ffast-math -ffreestanding -fno-exceptions -fno-rtti -fomit-frame-pointer -march=native -O3 -pedantic-errors -std=c++98 -Wall -Wextra -Wno-unknown-pragmas executable-memory.cpp -lonecore -o executable-memory.exe && executable-memory.exe & del executable-memory.exe */
#include <ciso646> // --> and, or, not
#include <cstdio>  // --> ::std::fprintf(…)
#include <cstdlib> // --> EXIT_FAILURE, EXIT_SUCCESS, NULL; ::std::size_t
#if defined _WIN32
# include <stdint.h>  // --> ::uintptr_t
# include <windows.h> // --> ::HANDLE, ::LPCVOID, ::LPVOID; FALSE, HEAP_CREATE_ENABLE_EXECUTE, HEAP_NO_SERIALIZE, MEM_COMMIT, MEM_RELEASE, MEM_RESERVE, PAGE_EXECUTE_READWRITE; ::FlushInstructionCache(…), ::GetCurrentProcess(), ::HeapAlloc(…), ::HeapCreate(…), ::HeapDestroy(…), ::HeapFree(…), ::VirtualAlloc(…), ::VirtualFree(…), ::VirtualProtect(…)
#
# pragma comment(lib, "onecore.lib")
#elif defined __APPLE__
# include <libkern/OSCacheControl.h> // --> ::sys_icache_invalidate(…)
# include <pthread.h>                // --> PTHREAD_JIT_WRITE_ALLOW_CALLBACKS_NP; ::pthread_jit_write_protect_np(…), ::pthread_jit_write_protect_supported_np()
# include <stdint.h>                 // --> ::uintptr_t
# include <sys/mman.h>               // --> MAP_ANONYMOUS, MAP_FAILED, MAP_PRIVATE, PROT_EXEC, PROT_WRITE; ::mmap(…), ::mprotect(…), ::munmap(…)

struct jit /* final */ {
  char const (&(**const            function)(bool))[14];
  /* unsigned */ char const* const source;
  ::std::size_t              const length;

  static int write(void* const context) {
    for (::std::size_t index = 0u; index != static_cast<struct ::jit const*>(context) -> length; ++index)
    static_cast<unsigned char volatile*>(reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(static_cast<struct ::jit const*>(context) -> function)))[index] = const_cast<char const volatile*>(static_cast<struct ::jit const*>(context) -> source)[index];

    return 0; // ->> Success
  }
};

PTHREAD_JIT_WRITE_ALLOW_CALLBACKS_NP(::jit::write); // ->> Registers at most one callback
#elif defined __unix__
# include <stdint.h>   // --> ::uintptr_t
# include <sys/mman.h> // --> MAP_ANONYMOUS, MAP_FAILED, MAP_PRIVATE, PROT_EXEC, PROT_WRITE; ::mmap(…), ::mprotect(…), ::munmap(…)
#endif

/* Main */
int main(int, char*[]) /* noexcept */ {
  /* unsigned */ char const source[] = {
    #if defined __aarch64__ or defined _M_ARM64        // ->> AArch64 / ARM64 (little-endian)
      0x60u, 0x00u, 0x00u, 0x35u,                      // -->   cbnz w0, lorem
                                                       // -->   hello:
      0x80u, 0x00u, 0x00u, 0x10u,                      // -->     adr x0, hello_ref
      0xC0u, 0x03u, 0x5Fu, 0xD6u,                      // -->     ret
                                                       // -->   lorem:
      0xA0u, 0x00u, 0x00u, 0x50u,                      // -->     adr x0, lorem_ref
      0xC0u, 0x03u, 0x5Fu, 0xD6u,                      // -->     ret
    #elif defined __arm__                              // ->> ARM32 / A32 (little-endian)
      0x00u, 0x00u, 0x50u, 0xE3u,                      // -->   cmp   r0, #0                ; `bool` argument as the R0 (Register 0) register
      0x04u, 0x00u, 0x8Fu, 0x02u,                      // -->   adreq r0, hello_ref         ;
      0x0Eu, 0x00u, 0x8Fu, 0x12u,                      // -->   adrne r0, lorem_ref         ;
      0x1Eu, 0xFFu, 0x2Fu, 0xE1u,                      // -->   bx    lr                    ; Return pointer as the R0 (Register 0) register
    #elif defined __i386__ or defined _M_IX86          // ->> IA-32 / x86
      0xE8u, 0x00u, 0x00u, 0x00u, 0x00u,               // -->     call base                 ;
                                                       // -->   base:                       ;
      0x58u,                                           // -->     pop eax                   ;
      0x80u, 0x7Cu, 0x24u, 4u, 0x00u,                  // -->     cmp byte ptr [esp + 4], 0 ;
      0x75u, 0x04u,                                    // -->     jne lorem                 ;
                                                       // -->   hello:                      ;
      0x83u, 0xC0u, 16u,                               // -->     add eax, 16               ; hello_ref - base
      0xC3u,                                           // -->     ret                       ;
                                                       // -->   lorem:                      ;
      0x83u, 0xC0u, 30u,                               // -->     add eax, 30               ; lorem_ref - base
      0xC3u,                                           // -->     ret                       ;
    #elif defined __riscv                              // ->> RISC-V RV32/RV64 (little-endian)
      0x63u, 0x08u, 0x05u, 0x00u,                      // -->     beqz a0, hello            ; `bool` argument as the A0 (Argument 0) register
                                                       // -->   lorem:                      ;
      0x17u, 0x05u, 0x00u, 0x00u,                      // -->     auipc a0, 0               ;
      0x13u, 0x05u, 0x65u, 0x02u,                      // -->     addi  a0, a0, 38          ; lorem_ref - lorem
      0x67u, 0x80u, 0x00u, 0x00u,                      // -->     ret                       ;
                                                       // -->   hello:                      ;
      0x17u, 0x05u, 0x00u, 0x00u,                      // -->     auipc a0, 0               ;
      0x13u, 0x05u, 0xC5u, 0x00u,                      // -->     addi  a0, a0, 12          ; hello_ref - hello
      0x67u, 0x80u, 0x00u, 0x00u,                      // -->     ret                       ;
    #elif defined __x86_64__ or defined _M_X64         // ->> x86-64
      #if defined _WIN32                               // ->>   Microsoft
        0x84u, 0xC9u,                                  // -->     test cl, cl               ; `bool` argument as the CL (Counter-Low) register
      #else                                            // ->>   SysV AMD
        0x40u, 0x84u, 0xFFu,                           // -->     test dil, dil             ; `bool` argument as the DIL (Destination Index-Low) register
      #endif                                           // ->> x86-64
      0x75u, 0x08u,                                    // -->     jne lorem                 ;
                                                       // -->   hello:                      ;
      0x48u, 0x8Du, 0x05u, 9u, 0x00u, 0x00u, 0x00u,    // -->     lea rax, [rip + 9]        ; hello_ref
      0xC3u,                                           // -->     ret                       ;
                                                       // -->   lorem:                      ;
      0x48u, 0x8Du, 0x05u, 15u, 0x00u, 0x00u, 0x00u,   // -->     lea rax, [rip + 15]       ; lorem_ref
      0xC3u,                                           // -->     ret                       ;
    #else
    # error [executable-memory]: no code for environment architecture…
    #endif
    'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd',  '!',  '\0', // --> char const hello_ref[14] = {"Hello, World!"}
    'L', 'o', 'r', 'e', 'm', ' ', 'i', 'p', 's', 'u', 'm', '\0', '\0', '\0'  // --> char const lorem_ref[14] = {"Lorem ipsum"}
  },              (&(*function)(bool))[14] = NULL;
  ::std::size_t const length               = sizeof source / sizeof(unsigned char);
  int                 exit                 = EXIT_SUCCESS;

  // ... ->> Attempt executable allocation first, then regular allocation modified into executable storage
  #if defined _WIN32
    enum /* : unsigned char */ {
      NON_EXECUTABLE,       //
      EXECUTABLE_FROM_PAGE, // --> ::VirtualProtect(…, PAGE_EXECUTE_READ, …)
      EXECUTABLE_FROM_HEAP  // --> ::HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, …)
    } const  statuses[] = {EXECUTABLE_FROM_PAGE, EXECUTABLE_FROM_HEAP, NON_EXECUTABLE}, *status = &statuses[0];
    ::HANDLE heap       = NULL;

    // ...
    for (; NULL == function and status != &statuses[sizeof statuses / sizeof *status]; ++status)
    switch (*status) {
      default:;

      case EXECUTABLE_FROM_HEAP: {
        heap = ::HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_NO_SERIALIZE, 0u, 0u);
        if (NULL == heap) continue;

        function = reinterpret_cast<char const (&(*)(bool))[14]>(reinterpret_cast< ::uintptr_t>(::HeapAlloc(heap, 0x00u, length)));
        if (NULL == function) continue;

        for (::std::size_t index = 0u; index != length; ++index)
        static_cast<unsigned char volatile*>(reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function)))[index] = const_cast<char const volatile*>(source)[index];
      } break;

      case EXECUTABLE_FROM_PAGE: ::DWORD protection = 0x00u; {
        function = reinterpret_cast<char const (&(*)(bool))[14]>(reinterpret_cast< ::uintptr_t>(::VirtualAlloc(static_cast< ::LPVOID>(NULL), length, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE /* | PAGE_READWRITE */)));
        if (NULL == function) continue;

        for (::std::size_t index = 0u; index != length; ++index)
        static_cast<unsigned char volatile*>(reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function)))[index] = const_cast<char const volatile*>(source)[index];

        (void) (FALSE != ::VirtualProtect(reinterpret_cast< ::LPVOID>(reinterpret_cast< ::uintptr_t>(function)), length, PAGE_EXECUTE_READ, &protection)); // ->> Unneeded when executable already; Does not work for existing non-executable heaps
      } break;
    }

    if (NULL == function or FALSE == ::FlushInstructionCache(::GetCurrentProcess(), reinterpret_cast< ::LPCVOID>(reinterpret_cast< ::uintptr_t>(function)), length)) {
      exit = EXIT_FAILURE;
      goto error;
    }
  #elif defined __APPLE__
    struct ::jit const context = {&function, source, length};

    // ...
    function = reinterpret_cast<char const (&(*)(bool))[14]>(reinterpret_cast< ::uintptr_t>(::mmap(static_cast<void*>(NULL), length, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_JIT | MAP_PRIVATE , -1, 0L)));

    if (MAP_FAILED == reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function))) {
      exit = EXIT_FAILURE;
      goto error;
    }

    if (0 != ::pthread_jit_write_with_callback_np(&::jit::write, const_cast<struct ::jit*>(&context))) {
      // ->> `com.apple.security.cs.allow-jit`           entitlement enabled
      // ->> `com.apple.security.cs.jit-write-allowlist` entitlement disabled
      (void) (0 != ::pthread_jit_write_protect_supported_np());
      ::pthread_jit_write_protect_np(0); // ->> Make `MAP_JIT` memory writable for this thread
        for (::std::size_t index = 0u; index != length; ++index)
        static_cast<unsigned char volatile*>(reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function)))[index] = const_cast<char const volatile*>(source)[index];
      ::pthread_jit_write_protect_np(+1); // ->> Make `MAP_JIT` memory executable again for this thread
    }

    ::sys_icache_invalidate(reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function)), length);
  #elif defined __unix__
    function = reinterpret_cast<char const (&(*)(bool))[14]>(reinterpret_cast< ::uintptr_t>(::mmap(static_cast<void*>(NULL), length, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0L)));

    if (MAP_FAILED == reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function))) {
      exit = EXIT_FAILURE;
      goto error;
    }

    for (::std::size_t index = 0u; index != length; ++index)
    static_cast<unsigned char volatile*>(reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function)))[index] = const_cast<char const volatile*>(source)[index];

    (void) (0 == ::mprotect(reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function)), length, PROT_EXEC | PROT_READ)); // ->> Unneeded when executable already
    #if defined __clang__ or defined __GNUC__
      #if defined __has_builtin
        #if __has_builtin(__builtin___clear_cache)
        __builtin___clear_cache(static_cast<char*>(reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function))), reinterpret_cast<char*>(&static_cast<unsigned char*>(reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function)))[length]));
        #endif
      #elif defined __GNUC__ and not defined __clang__
        __builtin___clear_cache(reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function)), static_cast<void*>(&static_cast<unsigned char*>(reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function)))[length]));
      #endif
    #endif
  #endif

  // ...
  if (NULL != function) {
    (void) ::std::fprintf(stdout, "%1.13s", "function(F): "), ::std::fflush(stdout), ::std::fprintf(stdout, "\"%.*s\"" "\r\n", 13u, function(false));
    (void) ::std::fprintf(stdout, "%1.13s", "function(T): "), ::std::fflush(stdout), ::std::fprintf(stdout, "\"%.*s\"" "\r\n", 13u, function(true));
  } else { error: exit = ::std::fprintf(stdout, "%1.21s", "function(...): NULL" "\r\n") < 0 ? EXIT_FAILURE : exit; }

  // ...
  #if defined _WIN32
    switch (*status) {
      case EXECUTABLE_FROM_HEAP: exit = FALSE == ::HeapFree(heap, HEAP_NO_SERIALIZE, reinterpret_cast< ::LPVOID>(reinterpret_cast< ::uintptr_t>(function))) ? EXIT_FAILURE : exit; break;
      case EXECUTABLE_FROM_PAGE: exit = FALSE == ::VirtualFree(reinterpret_cast< ::LPVOID>(reinterpret_cast< ::uintptr_t>(function)), 0u, MEM_RELEASE)      ? EXIT_FAILURE : exit; break;
      default:;
    }

    if (NULL != heap) // --> `::HeapFree(…)` redundant when `::HeapDestroy(…)` is applied
    exit = FALSE == ::HeapDestroy(heap) ? EXIT_FAILURE : exit;
  #elif defined __APPLE__ or defined __unix__
    if (MAP_FAILED != reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function)))
    exit = 0 != ::munmap(reinterpret_cast<void*>(reinterpret_cast< ::uintptr_t>(function)), length) ? EXIT_FAILURE : exit;
  #endif

  return exit;
}
