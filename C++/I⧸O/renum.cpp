/* POSIX:   del renum.exe && cls && clang++ -ffast-math -ffreestanding -fno-exceptions -fno-rtti -fomit-frame-pointer -march=native -O3 -pedantic -std=c++98 -Wall -Wextra -Wno-cast-function-type                      renum.cpp -lc -ldl   -o renum.exe && renum.exe "  A " "👋" C & del renum.exe */
/* Windows: del renum.exe && cls && clang++ -ffast-math -ffreestanding -fno-exceptions -fno-rtti -fomit-frame-pointer -march=native -O3 -pedantic -std=c++98 -Wall -Wextra -Wno-cast-function-type -Wno-unknown-pragmas renum.cpp -lkernel32 -o renum.exe && renum.exe "  A " "👋" C & del renum.exe */
#include <ciso646> // --> and, or, not
#include <climits> // --> MB_LEN_MAX
#include <clocale> // --> LC_ALL; ::std::setlocale(…)
#include <cstdarg> // --> va_arg(…), va_end(…), va_start(…); ::std::va_list
#include <cstddef> // --> ::std::max_align_t, ::std::size_t
#include <cstdio>  // --> ::std::FILE; _IOFBF, _IOLBF, stdout; ::std::fflush(…), ::std::setbuf(…), ::std::setvbuf(…)
#include <cstdlib> // --> MB_LEN_MAX, NULL; ::std::qsort(…), ::std::rand(…), ::std::srand(…)
#include <ctime>   // --> ::std::time_t; ::std::time(…)
#include <cwchar>  // --> WEOF; ::std::mbstate_t; ::std::fputwc(…), ::std::fwide(…), ::std::fwprintf(…), ::std::mbrtowc(…), ::std::mbsrtowcs(…)
#include <new>     // --> ::delete[], ::new, ::std::nothrow

#if defined _WIN32
# define _CRT_SECURE_NO_WARNINGS
# define  UNICODE
# define _UNICODE
# undef  _MBCS
#
# include <fcntl.h>    // --> _O_BINARY, _O_U8TEXT, _O_U16TEXT, _O_WTEXT
# include <io.h>       // --> ::_setmode(…)
# include <ntstatus.h> // --> ::NTSTATUS; STATUS_SUCCESS
# include <sal.h>      // --> _Printf_format_string_
# include <stdio.h>    // --> ::_fileno(…)
# include <stdlib.h>   // --> ::_get_pgmptr(…)
# include <windows.h>  // --> ::BOOL, ::DWORD, ::FARPROC, ::HMODULE, ::LPCCH, ::LPCSTR, ::LPCWSTR, ::LPWSTR, ::PUCHAR, ::PVOID, ::SIZE_T, ::TCHAR, ::UINT, ::ULONG, ::WCHAR; CP_UTF8, FALSE, GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, LOAD_LIBRARY_SEARCH_SYSTEM32, LOAD_WITH_ALTERED_SEARCH_PATH, MAX_PATH, WINAPI, SecureZeroMemory(…), ZeroMemory(…); ::GetModuleHandleExW(…), ::GetProcAddress(…), ::GetSystemDirectoryW(…)
#   include <bcrypt.h> // --> ::BCRYPT_ALG_HANDLE
#
# pragma comment(lib, "kernel32.lib")

extern "C" __declspec(dllimport) ::BOOL                GetModuleHandleExW (::DWORD, ::LPCWSTR, ::HMODULE*);
extern "C" __declspec(dllimport) ::FARPROC             GetProcAddress     (::HMODULE, ::LPCSTR);
extern "C" __declspec(dllimport) ::UINT                GetSystemDirectoryW(::LPWSTR, ::UINT);
extern "C"                       volatile void* WINAPI SecureZeroMemory2  (volatile void*, ::SIZE_T);
#elif defined __APPLE__ or defined __unix__
# include <dirent.h>    // --> ::DIR; dirent; ::closedir(…), ::opendir(…), ::readdir(…)
# include <dlfcn.h>     // --> RTLD_LOCAL; ::dlerror(…), ::dlopen(…), ::dlsym(…)
# include <errno.h>     // --> EINTR; errno
# include <iconv.h>     // --> ::iconv_t
# include <stdint.h>    // --> uint32_t ->> will be defined
# include <stdlib.h>    // --> ::realpath(…)
# include <string.h>    // --> ::strlen(…)
# include <sys/stat.h>  // --> S_ISLNK(…), S_ISREG(…); stat; ::lstat(…)
# include <sys/types.h> // --> ::ssize_t
# include <unistd.h>    // --> ::readlink(…)

extern "C" char* dlerror(void);
extern "C" void* dlopen (char const[], int);
extern "C" void* dlsym  (void*, char const[]);
# if (defined __APPLE__ and defined __MACH__) or defined __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__
#   include <mach-o/dyld.h> // --> ::_NSGetExecutablePath(…)
# elif defined __FreeBSD__ or defined __FreeBSD_version
#   include <sys/sysctl.h> // --> ::sysctl(…)
#   include <sys/types.h>  // --> u_int
# elif defined __SUNPRO_C or defined __SUNPRO_CC
#   include <stdlib.h>    // --> ::getexecname()
#   include <sys/types.h> // --> pid_t
#   include <unistd.h>    // --> ::getcwd(…), ::getpid()
# endif
#endif

/* Renumerator */
struct renum /* final */ {
  wchar_t const *const name; // --> wchar_t const[]
  wchar_t const       *directory;
} renum = {L"renum", NULL};

/* Main */
int main(int count, char* arguments[] /* , char* environment[] */) {
  struct option /* final */ {
    enum flag /* : unsigned char */ {
      DIGITS,       // ->> Indexing characters e.g. `10.png`, `11.jpg`, and so on from arabic numerals --> "0123456789"
      HELP,         // ->> Script information                                                          --> true
      HIDDEN_FILES, // ->> Renumerate hidden files                                                     --> false
      INDEX,        // ->> Index start                                                                 --> "1"
      INPUT,        // ->> `printf(…)`-formatted filter for indexed files                              --> "%s"
      OUTPUT,       // ->> `printf(…)`-formatted filter for renamed files                              --> "%u"
      PADDING,      // ->> Padding character to format indexes to the same length (NUL does not pad)   --> '0'
      REVERSE,      // ->> Reversed indexing                                                           --> false
      SYSTEM_FILES  // ->> Renumerate system files                                                     --> false
    };

    struct namelist /* final */ { char const *pre, *values; };
    struct nameset  /* final */ {
      union { struct namelist full,  longname,  verbose; };     // --> "--"
      union { struct namelist alias, shortname, abbreviated; }; // --> "-"
      struct namelist extended;                                 // --> "/", …
    };

    enum option::flag const flag;                           // ->> There can be multiple switches per flag rather than overriding prior switches
    union { struct nameset name; struct namelist *names; }; // ->> Exclusively initialize either pre-/ user-allocated names (explicit tagging required)
    union /* --> alignas(std::max_align_t) */ {
      unsigned char address[/* --> max(…) */ sizeof(long double) | sizeof(std::size_t) | sizeof(void*) | sizeof(void (*)(...)) | sizeof(void (option::*)(...))];
      #if defined _MSVC_LANG /* --> 201402L+ */ or __cplusplus >= 201103L
        ::std::max_align_t const _0;
      #endif
      long double   const _1;
      ::std::size_t const _2;
      void         *const _3, (*const _4)(...), (option:: *const _5)(...);
    }     memory;
    void *value; // ->> Blessed versatile representation of `memory`
  } options[] = {
    {option::DIGITS,       {{{{"/" "\0" "--" "\0", "digits"       "\0"}},               {{"/" "\0" "-" "\0", "d"  "\0"}},          {}}}, {{}}, ::new (options[0].memory.address) char const*("0123456789")},
    {option::HELP,         {{{{"/" "\0" "--" "\0", "help"         "\0"}},               {{"/" "\0",          "?"  "\0"}},          {}}}, {{}}, ::new (options[1].memory.address) bool       (true)},
    {option::HIDDEN_FILES, {{{{"/" "\0" "--" "\0", "hidden-files" "\0"}},               {{"/" "\0" "-" "\0", "hf" "\0"}},          {}}}, {{}}, ::new (options[2].memory.address) bool       (false)},
    {option::INDEX,        {{{{"/" "\0" "--" "\0", "index"        "\0"}},               {{"/" "\0" "-" "\0", "s"  "\0"}},          {}}}, {{}}, ::new (options[3].memory.address) char const*("1")},
    {option::INPUT,        {{{{"/" "\0" "--" "\0", "input"        "\0" "filter" "\0"}}, {{"/" "\0" "-" "\0", "i"  "\0" "f" "\0"}}, {}}}, {{}}, ::new (options[4].memory.address) char const*("%s")},
    {option::OUTPUT,       {{{{"/" "\0" "--" "\0", "output"       "\0" "rename" "\0"}}, {{"/" "\0" "-" "\0", "o"  "\0"}},          {}}}, {{}}, ::new (options[5].memory.address) char const*("%u")},
    {option::PADDING,      {{{{"/" "\0" "--" "\0", "padding"      "\0"}},               {{"/" "\0" "-" "\0", "p"  "\0"}},          {}}}, {{}}, ::new (options[6].memory.address) char       ('0')},
    {option::REVERSE,      {{{{"/" "\0" "--" "\0", "reverse"      "\0"}},               {{"/" "\0" "-" "\0", "r"  "\0"}},          {}}}, {{}}, ::new (options[7].memory.address) bool       (false)},
    {option::SYSTEM_FILES, {{{{"/" "\0" "--" "\0", "system-files" "\0"}},               {{"/" "\0" "-" "\0", "sf" "\0"}},          {}}}, {{}}, ::new (options[8].memory.address) bool       (false)}
  };

  static struct library *libraries;
  struct library /* final */ {
    #if defined _WIN32
      enum    /* : unsigned char */ { COUNT = 4u };
      enum id /* : unsigned char */ { advapi32, bcrypt, credui, kernel32, ole32, oleaut32, shell32, taskschd /* , ... */ };

      enum library::id                                                  const id;
      ::LPCWSTR                                                         const name; // ->> Case-insensitive
      struct /* final */ { ::HMODULE handle; ::BOOL (*unloader)(::HMODULE); } module;
    #else
      enum    /* : unsigned char */ { COUNT = 1u };
      enum id /* : unsigned char */ {};

      enum library::id                                       const id;
      wchar_t const                                         *const name;
      struct /* final */ { void *handle; int (*unloader)(void*); } module;
    #endif

    /* ... */
    inline static struct library* get(enum library::id const id) /* noexcept */ {
      for (struct library const *library = libraries; library != &libraries[library::COUNT]; ++library) {
        if (id == library -> id)
        return const_cast<struct library*>(library);
      }

      return NULL;
    }

    inline static void load() /* noexcept */ {
      #if defined _WIN32
        ::WCHAR path[MAX_PATH + 13]; // --> ::GetSystemDirectoryW(…) + '\\' + libraries[kernel32].name

        if (::UINT const pathLength = (::GetSystemDirectoryW(path, sizeof path / sizeof(::TCHAR)) * sizeof(::TCHAR)) / sizeof(::WCHAR)) {
          // ... ->> More robust than destructor function
          (void) ::std::atexit(&libraries -> unload);
          #if __cplusplus >= 201103L
            (void) ::std::at_quick_exit(&libraries -> unload);
          #endif

          // ... ->> Load the userland `kernel32` library first
          path[pathLength] = L'\\';

          if (struct library *const kernel32Library = libraries -> get(library::kernel32)) {
            for (std::size_t index = 0u; ; ++index) {
              path[index + pathLength + 1u] = kernel32Library -> name[index];
              if (L'\0' == kernel32Library -> name[index]) break;
            }

            if (FALSE != ::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, path, &kernel32Library -> module.handle))
            if (NULL != kernel32Library -> module.handle) {
              ::BOOL    (*const FreeLibrary)   (::HMODULE)                    = reinterpret_cast< ::BOOL    (*)(::HMODULE)>                   (::GetProcAddress(kernel32Library -> module.handle, "FreeLibrary"));    // --> <windows.h>
              ::HMODULE (*const LoadLibraryExW)(::LPCWSTR, ::HANDLE, ::DWORD) = reinterpret_cast< ::HMODULE (*)(::LPCWSTR, ::HANDLE, ::DWORD)>(::GetProcAddress(kernel32Library -> module.handle, "LoadLibraryExW")); // --> <windows.h>

              // ... ->> Load remaining Windows API libraries —
              if (::BOOL (*const SetDefaultDllDirectories)(::DWORD) = reinterpret_cast< ::BOOL (*)(::DWORD)>(::GetProcAddress(kernel32Library -> module.handle, "SetDefaultDllDirectories"))) // --> <windows.h> not NULL
              (void) SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_SYSTEM32); // --> %SystemRoot%\System32

              for (struct library *library = libraries; library != &libraries[library::COUNT]; ++library) {
                if (kernel32Library == library)
                continue;

                // ... ->> — except the already loaded userland `kernel32` library
                for (std::size_t index = 0u; ; ++index) {
                  path[index + pathLength + 1u] = library -> name[index];
                  if (L'\0' == library -> name[index]) break;
                }

                if (FALSE != ::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, path, &library -> module.handle)) {
                  if (NULL != library -> module.handle)
                  continue; // --> library -> module.unloader = NULL;
                }

                library -> module.handle   = LoadLibraryExW(path, static_cast< ::HANDLE>(NULL), LOAD_WITH_ALTERED_SEARCH_PATH);
                library -> module.unloader = NULL == library -> module.handle ? NULL : FreeLibrary;
              }
            }

            return;
          }
        }
      #elif defined __APPLE__ or defined __unix__
        libraries -> module.handle = ::dlopen(static_cast<char const*>(NULL), RTLD_LOCAL /* | RTLD_LAZY */);
        if (NULL != libraries -> module.handle) {
          (void) ::dlerror(); // ->> Clear prior linker error diagnostics

          libraries -> module.unloader = reinterpret_cast<int (*)(void*)>(::dlsym(libraries -> module.handle, "dlclose"));
          libraries -> module.unloader = static_cast<char*>(NULL) != ::dlerror() ? NULL : libraries -> module.unloader;
        }
      #endif
    }

    static void unload() /* extern "C" */ {
      if (NULL == libraries)
      return;

      for (struct library *library = libraries; library != &libraries[library::COUNT]; ++library) {
        if (NULL != library -> module.handle and NULL != library -> module.unloader)
        library -> module.unloader(library -> module.handle); // --> FreeLibrary(…)
      }

      libraries = NULL;
    }
  } l[library::COUNT] = {
    #if defined _WIN32
      {library::advapi32, L"advapi32" ".dll", {NULL, NULL}},
      {library::bcrypt,   L"bcrypt"   ".dll", {NULL, NULL}},
      {library::kernel32, L"kernel32" ".dll", {NULL, NULL}},
      {library::shell32,  L"shell32"  ".dll", {NULL, NULL}}
    #elif defined __APPLE__ or defined __unix__
      {static_cast<enum library::id>(0x00u), L"libdl.so", {NULL, NULL}} // ->> Recently `libc.so` suffices
    #else
      {static_cast<enum library::id>(0x00u), NULL, {NULL, NULL}}
    #endif
  };

  struct memory /* final */ {
    typedef struct /* final */ {
      enum flag /* : unsigned char */ {
        PRESERVE_MEMORY = 0x1u, // ->> Destroys prior objects contained in `.value`
        RAW_MEMORY      = 0x2u, //
        SCRAMBLE_MEMORY = 0x4u, // ->> Needn’t be cryptographically secure
        ZERO_MEMORY     = 0x8u  //
      };
    } policy;

    /* ... */
    unsigned char *value; // ->> Not copy-safe
    ::std::size_t  capacity;

    /* ... */
    ~memory() {
      ::delete[] this -> value;
    }

    /* ... */
    inline static unsigned char* scramble(unsigned char bytes[], ::std::size_t const size) /* noexcept */ {
      #if defined _WIN32
        if (struct library *const library = libraries -> get(library::bcrypt))
        if (NULL != library -> module.handle) {
          if (::NTSTATUS (*const BCryptGenRandom)(::BCRYPT_ALG_HANDLE, ::PUCHAR, ::ULONG, ::ULONG) = reinterpret_cast< ::NTSTATUS (*)(::BCRYPT_ALG_HANDLE, ::PUCHAR, ::ULONG, ::ULONG)>(::GetProcAddress(library -> module.handle, "BCryptGenRandom"))) // --> <bcrypt.h> not NULL
          if (STATUS_SUCCESS == BCryptGenRandom(static_cast< ::BCRYPT_ALG_HANDLE>(NULL), static_cast< ::PUCHAR>(bytes), static_cast< ::ULONG>(size), 0x00000000uL))
          return bytes;
        }

        if (struct library *const library = libraries -> get(library::advapi32))
        if (NULL != library -> module.handle) {
          if (::BOOLEAN (*const RtlGenRandom)(::PVOID, ::ULONG) = reinterpret_cast< ::BOOLEAN (*)(::PVOID, ::ULONG)>(::GetProcAddress(library -> module.handle, "SystemFunction036"))) // --> <ntsecapi.h> not NULL
          if (FALSE != RtlGenRandom(static_cast< ::PVOID>(buffer), static_cast< ::ULONG>(size)))
          return bytes;
        }
      #elif defined __APPLE__ or defined __unix__
        if (int (*const getentropy)(void*, ::std::size_t) = reinterpret_cast<int (*)(void*, ::std::size_t)>(::dlerror(), ::dlsym(libraries -> module.handle, "getentropy"))) // --> <sys/random.h> not NULL
        if (static_cast<char*>(NULL) == ::dlerror()) {
          ::std::size_t index = 0u;

          // ...
          for (::std::size_t count; index != size; index += count) {
            count = size - index < 256u ? size - index : 256u;
            if (getentropy(&bytes[index], count) == -1) break;
          }

          if (index == size)
          return bytes;
        }

        if (::ssize_t (*const getrandom)(void*, ::std::size_t, unsigned) = reinterpret_cast< ::ssize_t (*)(void*, ::std::size_t, unsigned)>(::dlerror(), ::dlsym(libraries -> module.handle, "getrandom"))) // --> <sys/random.h> not NULL
        if (static_cast<char*>(NULL) == ::dlerror()) {
          ::std::size_t index = 0u;

          // ...
          for (::ssize_t count; index != size; index += static_cast< ::std::size_t>(count)) {
            count = getrandom(&bytes[index], size - index, 0x00u);

            if      (count == -1 and EINTR == errno) count = 0;
            else if (count <= 0)                     break;
          }

          if (index == size)
          return bytes;
        }
      #endif
      unsigned seed = 0u;

      // ...
      for (unsigned char const volatile *byte = &reinterpret_cast<unsigned char const volatile*>(&bytes)[sizeof(unsigned char*)]; byte != reinterpret_cast<unsigned char*>(&bytes); )
      seed = *--byte | (seed << 8u); // ->> `CHAR_BIT` minimum

      #if __STDC_HOSTED__
        ::std::srand(seed); // ->> Seed with `bytes`

        for (unsigned char volatile *byte = &bytes[size]; byte != bytes; )
        *--byte = ::std::rand();
      #else
        for (unsigned char volatile *byte = &bytes[size]; byte != bytes; ) {
          seed = (seed * 25173u) + 13849u;                                                                // ->> Linear Congruential Generator
          *--byte = static_cast<unsigned char>(seed /* >> ((CHAR_BIT * sizeof(unsigned)) - CHAR_BIT) */); // ->> The high-end is more cryptographically sound
        }
      #endif

      return bytes;
    }

    unsigned char* reserve(::std::size_t const capacity, enum memory::policy::flag const policy = static_cast<enum memory::policy::flag>(memory::policy::PRESERVE_MEMORY | memory::policy::ZERO_MEMORY)) /* ->> Aligned to `__STDCPP_DEFAULT_NEW_ALIGNMENT__` (since C++17) or `alignof(::std::max_align_t)` (since C++11) */ {
      if (capacity > this -> capacity) {
        if (unsigned char *const allocation = ::new (::std::nothrow) unsigned char[capacity]) /* --> not NULL */ {
          if (not (static_cast<unsigned char>(policy) & static_cast<unsigned char>(memory::policy::RAW_MEMORY))) {
            if      (static_cast<unsigned char>(policy) & static_cast<unsigned char>(memory::policy::ZERO_MEMORY))     (void) memory::zero    (allocation, capacity);
            else if (static_cast<unsigned char>(policy) & static_cast<unsigned char>(memory::policy::SCRAMBLE_MEMORY)) (void) memory::scramble(allocation, capacity);
          }

          if (NULL != this -> value) {
            // ... --> ::std::memcpy(allocation, this -> value, this -> capacity)
            if (static_cast<unsigned char>(policy) & static_cast<unsigned char>(memory::policy::PRESERVE_MEMORY)) {
              for (unsigned char *destination = &allocation[this -> capacity], *source = &this -> value[this -> capacity]; source != this -> value; )
              *--destination = *--source;
            }

            // ... ->> In case `.value` can be inspected after deletion/ release
            if (not (static_cast<unsigned char>(policy) & static_cast<unsigned char>(memory::policy::RAW_MEMORY))) {
              if      (static_cast<unsigned char>(policy) & static_cast<unsigned char>(memory::policy::ZERO_MEMORY))     (void) memory::zero    (this -> value, this -> capacity);
              else if (static_cast<unsigned char>(policy) & static_cast<unsigned char>(memory::policy::SCRAMBLE_MEMORY)) (void) memory::scramble(this -> value, this -> capacity);
            }
          }

          ::delete[] this -> value;

          this -> capacity = capacity;
          this -> value    = allocation;

          return this -> value;
        }

        return NULL;
      }

      return this -> value;
    }

    inline static unsigned char* zero(unsigned char bytes[], ::std::size_t const size) /* noexcept */ {
      #if defined _WIN32
        #if defined _MSC_VER and defined NTDDI_VERSION ? NTDDI_VERSION >= 0x0A000010 : false              //
          #pragma comment(lib, "volatileaccessu.lib")                                                     //
          return const_cast<unsigned char*>(SecureZeroMemory2(static_cast<void volatile*>(bytes), size)); // --> RtlSecureZeroMemory2(…)
        #elif defined SecureZeroMemory                                       //
          return static_cast<unsigned char*>(SecureZeroMemory(bytes, size)); // --> RtlSecureZeroMemory(…)
        #elif defined ZeroMemory                       //
          ZeroMemory(static_cast<void*>(bytes), size); // --> RtlZeroMemory(…)
        #endif
      #elif defined __APPLE__ or defined __unix__
        if (void (*const explicit_bzero)(void*, ::std::size_t) = reinterpret_cast<void (*)(void*, ::std::size_t)>(::dlerror(), ::dlsym(libraries -> module.handle, "explicit_bzero"))) // --> <string.h> not NULL
        if (static_cast<char*>(NULL) == ::dlerror()) {
          explicit_bzero(bytes, size);
          return bytes;
        }

        if (void (*const bzero)(void*, ::std::size_t) = reinterpret_cast<void (*)(void*, ::std::size_t)>(::dlerror(), ::dlsym(libraries -> module.handle, "bzero"))) // --> <strings.h> not NULL
        if (static_cast<char*>(NULL) == ::dlerror()) {
          bzero(bytes, size);
          return bytes;
        }
      #endif

      for (unsigned char volatile *byte = &bytes[size]; byte != bytes; )
      *--byte = 0x00u; // --> ::std::memset_explicit(allocation, 0x00, capacity)

      return bytes;
    }
  } memory = {NULL, 0u};

  struct console /* final */ {
    struct policy /* final */ {
      enum flag /* : unsigned char */ { FLUSH_ON_NEWLINE, FLUSH_ON_OVERFLOW };
    };

    /* ... */
    inline static bool buffer(::std::FILE* const stream, char buffer[], ::std::size_t const size, enum console::policy::flag const policy = console::policy::FLUSH_ON_OVERFLOW) /* noexcept */ {
      if (NULL == buffer) {
        ::std::setbuf(stream, static_cast<char*>(NULL)); // --> ::std::setvbuf(stream, static_cast<char*>(NULL), _IONBF, 0u)
        return true;
      }

      switch (policy) {
        case console::policy::FLUSH_ON_NEWLINE:  return 0 == ::std::setvbuf(stream, buffer, _IOLBF, size);
        case console::policy::FLUSH_ON_OVERFLOW: return 0 == ::std::setvbuf(stream, buffer, _IOFBF, size);
      }

      return 0 == ::std::setvbuf(stream, buffer, _IONBF, size);
    }

    inline static bool flush(::std::FILE* const stream) /* noexcept */ {
      return 0 == ::std::fflush(stream);
    }

    #if defined _WIN32
      static int (format)(::std::FILE* const stream, _Printf_format_string_ wchar_t const* const format, ...) /* noexcept */
    #else
      /* [[gnu::format(wprintf, 2, 3)]] */
      static int (format)(::std::FILE* const stream, wchar_t const format[], ...) /* noexcept */
    #endif
    {
      ::std::va_list arguments;                                      va_start(arguments, format);
      int const count = ::std::vfwprintf(stream, format, arguments); va_end  (arguments);

      return count;
    }

    inline static bool narrow(::std::FILE* const stream) /* noexcept */ {
      return ::std::fwide(stream, -1) < 0;
    }

    static unsigned char text(::std::FILE* const stream, char const character, wchar_t const escape[] = L"") /* noexcept */ {
      ::std::mbstate_t state = ::std::mbstate_t();
      wchar_t          subcharacter;

      // ...
      switch (::std::mbrtowc(&subcharacter, &character, 1u, &state)) {
        case static_cast< ::std::size_t>(-2):            // ->> Incomplete
        case static_cast< ::std::size_t>(-1): return 0u; // ->> Invalid
      }

      return console::text(stream, subcharacter, escape);
    }

    static ::std::size_t text(::std::FILE* const stream, char const* text, wchar_t const escape[] = L"") /* noexcept */ {
      ::std::size_t    length = 0u;
      ::std::mbstate_t state  = ::std::mbstate_t();

      // ... ->> Handles indeterminately-terminated text
      for (; '\0' != *text; ++length) {
        wchar_t       character;
        ::std::size_t sublength = 0u;

        // ...
        while (static_cast< ::std::size_t>(MB_LEN_MAX) != ++sublength) {
          if ('\0' == text[sublength])
          break;
        }

        switch (sublength = ::std::mbrtowc(&character, text, sublength, &state)) {
          case static_cast< ::std::size_t>(-2):
          case static_cast< ::std::size_t>(-1): return 0u;
          case 0x00u:                           return length; // ->> NUL-terminated
        }

        if (not console::text(stream, character, escape))
        return 0u;

        text += sublength;
      }

      return length;
    }

    inline static unsigned char text(::std::FILE* const stream, wchar_t const character, wchar_t const* escape = L"") /* noexcept */ {
      for (; L'\0' != *escape; ++escape)
      if (character == *escape or character == L'\\') {
        (void) ::std::fputwc(L'\\', stream);
        break;
      }

      return WEOF != ::std::fputwc(character, stream) ? 1u : 0u;
    }

    static ::std::size_t text(::std::FILE* const stream, wchar_t const* text, wchar_t const escape[] = L"") /* noexcept */ {
      ::std::size_t length = 0u;

      // ...
      for (; L'\0' != *text; ++length) {
        if (not console::text(stream, *text, escape))
        return 0u;

        ++text;
      }

      return length;
    }

    inline static bool widen(::std::FILE* const stream) /* noexcept */ {
      return ::std::fwide(stream, +1) > 0;
    }
  } const console = {};

  // ... ->> Assume standard file streams as UTF-8 (via functions dependent on `::std::setlocale(…)`)
  (void) console.buffer(stderr, static_cast<char*>(NULL), 0u);
  (libraries = l) -> load();

  for (char const *const locales[] = {".UTF-8", ".UTF8", "C.UTF-8", "C.utf8", "en_US.UTF-8", /* ->> User-preferred */ ""}, *const *locale = locales; locale != &locales[sizeof locales / sizeof(char const*)]; ++locale)
  if (static_cast<char*>(NULL) != ::std::setlocale(LC_ALL, *locale)) /* --> LC_ALL == LC_COLLATE | LC_CTYPE | LC_MONETARY | LC_NUMERIC | LC_TIME | … | LC_MESSAGES | … | LC_ADDRESS | LC_IDENTIFICATION | LC_KEYBOARD | LC_MEASUREMENT | LC_NAME | LC_PAPER | LC_TELEPHONE | LC_XLITERATE */ {
    (void) ::std::fwide(stderr, +1);
    (void) ::std::fwide(stdout, +1);

    break;
  }

  #if defined _WIN32
    if (struct library *const library = libraries -> get(library::kernel32))
    if (NULL != library -> module.handle) {
      if (::BOOL WINAPI (*const SetConsoleOutputCP)(::UINT) = reinterpret_cast< ::BOOL WINAPI (*)(::UINT)>(::GetProcAddress(library -> module.handle, "SetConsoleOutputCP"))) // --> <windows.h> not NULL
      (void) SetConsoleOutputCP(CP_UTF8); // --> 65001 "utf-8"
    }

    for (::std::FILE *const streams[] = {stdout, stderr}, *const *stream = streams; stream != &streams[sizeof streams / sizeof(::std::FILE*)]; ++stream) {
      int const descriptor = (::_fileno)(*stream);

      if (descriptor != -1 and 0 == ::std::fflush(*stream))
      for (int const translations[] = {_O_U8TEXT, _O_WTEXT, _O_U16TEXT, _O_BINARY}, *translation = translations; translation != &translations[sizeof translations / sizeof(int)]; ++translation) {
        if ((::_setmode)(descriptor, *translation) != -1)
        break;
      }
    }
  #endif

  // ... ->> Parse arguments
  for (unsigned index = 1u; index < static_cast<unsigned>(count > 0 ? count : 0); ++index) {
    struct /* final */ { char const *multibyte; wchar_t *wide; } argument = {arguments[index], NULL};

    // ...
    #if defined _WIN32
      if (struct library *const library = libraries -> get(library::kernel32))
      if (NULL != library -> module.handle) {
        if (int (*const MultiByteToWideChar)(::UINT, ::DWORD, ::LPCCH, int, ::LPWSTR, int) = reinterpret_cast<int (*)(::UINT, ::DWORD, ::LPCCH, int, ::LPWSTR, int)>(::GetProcAddress(library -> module.handle, "MultiByteToWideChar"))) /* --> <windows.h> not NULL */ {
          int const length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, argument.multibyte, -1, static_cast< ::LPWSTR>(NULL), 0);

          argument.wide = length > 0 ? reinterpret_cast<wchar_t*>(memory.reserve(length + sizeof(wchar_t), memory::policy::RAW_MEMORY)) : NULL;
          argument.wide = NULL != argument.wide and MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, argument.multibyte, -1, static_cast< ::LPWSTR>(argument.wide), length) > 0 ? argument.wide : NULL;
        }
      }
    #elif defined __APPLE__ or defined __unix__
      //   ::iconv_t const converter = ::iconv_open("WCHAR_T", "UTF-8");
      //
      //   if (static_cast< ::iconv_t>(-1) == converter)
      //     return NULL;
      //
      //   ::std::size_t const inputLength = ::std::strlen(source);
      //   ::std::size_t const capacity    = (inputLength + 1u) * sizeof(wchar_t);
      //
      //   wchar_t *wide = reinterpret_cast<wchar_t*>(
      //     memory.reserve(capacity, memory::policy::RAW_MEMORY)
      //   );
      //
      //   if (NULL == wide) {
      //     (void) ::iconv_close(converter);
      //     return NULL;
      //   }
      //
      //   wide = ::new (wide) wchar_t[inputLength + 1u];
      //
      //   char       *input     = const_cast<char*>(source);
      //   char       *output    = reinterpret_cast<char*>(wide);
      //   ::std::size_t inputLeft  = inputLength;
      //   ::std::size_t outputLeft = capacity - sizeof(wchar_t);
      //
      //   if (
      //     static_cast< ::std::size_t>(-1) ==
      //     ::iconv(
      //       converter,
      //       &input,
      //       &inputLeft,
      //       &output,
      //       &outputLeft
      //     )
      //   ) {
      //     (void) ::iconv_close(converter);
      //     return NULL;
      //   }
      //
      //   wchar_t *terminator = reinterpret_cast<wchar_t*>(output);
      //   *terminator = L'\0';
      //
      //   (void) ::iconv_close(converter);
      //   return wide;
    #endif

    if (NULL == argument.wide) {
      ::std::mbstate_t    state  = ::std::mbstate_t();
      ::std::size_t const length = ::std::mbsrtowcs(static_cast<wchar_t*>(NULL), &argument.multibyte, 0u, &state);

      // ...
      argument.multibyte = arguments[index];
      state              = ::std::mbstate_t();

      argument.wide = length != static_cast< ::std::size_t>(-1) ? reinterpret_cast<wchar_t*>(memory.reserve((length + 1u) * sizeof(wchar_t), memory::policy::RAW_MEMORY)) : NULL; // ->> Placement-new unneeded for implicit lifetime `wchar_t`
      argument.wide = NULL != argument.wide and ::std::mbsrtowcs(argument.wide, &argument.multibyte, length + 1u, &state) != static_cast< ::std::size_t>(-1) ? argument.wide : NULL;
    }

    if (NULL == argument.wide) {
      (void) console.format(stderr, L"%lc%ls%lc%1.2ls%1.50ls", L'[', renum.name, L']', L": ", L"Aborting; Unable to parse command-line option \u2014 " "\r\n");
      (void) console.text  (stderr, argument.multibyte);

      return EXIT_FAILURE;
    }

    // ...
    (void) ::std::fwprintf(stdout, L"[%u] \"%hs\" \"%ls\"" "\r\n", index, arguments[index], argument.wide);
  }

  // while (true)
  // switch (*argument.wide) /* --> ::std::isspace(…) */ {
  //   case L' ': case L'\f': case L'\n': case L'\r': case L'\t': case L'\v': ++argument.wide; break;
  //   default:                                                               goto trimmed;
  // } trimmed:;
  (void) arguments;
  (void) count;
  (void) options;
  (void) renum;
}

#if false
  int main(int count, char* arguments[]) /* noexcept */ {
    for (unsigned index = 0u; NULL != *arguments and index != static_cast<unsigned>(count > 0 ? count : 0); ++arguments) /* ->> US-ASCII encoding */ {
      char const *commandLineError = NULL;

      // ... ->> Parse command-line options
      if (not commandLineError and **arguments == '-') {
        enum /* : bool */ { LONG_OPTION, SHORT_OPTION } const commandLineOptionType  = (*arguments)[1] == '-'               ? LONG_OPTION                              : SHORT_OPTION;
        char const* renum::switchoption_t::     *const commandLineOptionNames = LONG_OPTION == commandLineOptionType ? &renum::switchoption_t::longNames : &renum::switchoption_t::shortNames;

        // ...
        commandLineError = "Unrecognized command-line option";

        for (struct renum::switches *iterator = commandLineSwitches; iterator != commandLineSwitches + (sizeof commandLineSwitches / sizeof(struct renum::switches)); ++iterator)
        for (char const *argument = (*arguments) + (LONG_OPTION == commandLineOptionType) + 1, *names = NULL != iterator -> option.*commandLineOptionNames ? iterator -> option.*commandLineOptionNames : "\0"; ; ++argument, ++names) {
          if ('\0' == *names) { // ->> Delimited `name` prefixes `argument`
            commandLineError   = NULL;
            iterator -> active = true;

            // ... ->> Parse command-line option value
            for (bool trim = true; trim; ++argument)
            switch (*argument) { case ' ': case '\f': case '\n': case '\r': case '\t': case '\v': break; default: trim = false; }

            if (*argument == (NULL != iterator -> value ? '=' : '\0')) {
              if (NULL != iterator -> value) {
                ++argument;

                for (bool trim = true; trim; ++argument) // Do not repeat yourself…
                switch (*argument) { case ' ': case '\f': case '\n': case '\r': case '\t': case '\v': break; default: trim = false; }

                // ...
                if (iterator == commandLineSwitches + renum::MEMORY) {
                  switch (*argument) // [0-9]+
                  commandLineError = "Invalid command-line option value";
                }

                else { // ["'].*["']
                  commandLineError = "Invalid command-line option value";
                }
              }

              break;
            }
          }

          if (*argument != *names) { // ->> Iterate the next delimited `name`
            while ('\0' != names[0]) ++names;
            if    ('\0' == names[1]) break;

            argument = *arguments + (LONG_OPTION == commandLineOptionType);
          }
        }

        if (not commandLineError)
        continue;
      }

      // ... ->> Parse other command-line arguments e.g.: renumeration directory
      if (not commandLineError and index == 1u) {
        continue;
      }

      // ... ->> Invalid command-line argument
      commandLineError = NULL == commandLineError ? "Invalid command-line argument" : commandLineError;

      for (char *argument = *arguments; '\0' != *argument; ++argument)
      switch (*argument) { case '\f': case '\n': case '\r': case '\t': case '\v': *argument = ' '; }

      (void) std::fflush (stderr);
      (void) std::fprintf(stderr, "%s%1.3s%s%1.3s", commandLineError, ": `", *arguments, "`" "\r\n");
      (void) std::fflush (stderr);

      return EXIT_FAILURE;
    }

    (void) programName;
    (void) renumerationDirectory;
  }
#endif

#if false
  int main(int count, char* arguments[]) /* noexcept */ {
    static struct renumerator_arena /* final */ {
      struct /* final */ {
        unsigned char      chunk[128u * 1024u]; // ->> 128 kilobytes
        renumerator_arena *chunks;              // ->> Collection of linked fixed-size storage buffers
        std::size_t        chunksCapacity;      //
        std::size_t        chunksLength;        //
      } storage;

      unsigned char *buffer;         // ->> Defaults to fixed-size `renumerator_arena::...::chunk`
      std::size_t    bufferCapacity; // ->> Defaults to     `sizeof(renumerator_arena::...::chunk)`
      bool           invalidated;    // ->> Storage allocated prior from `renumerator_arena::...::chunks` is invalid
      unsigned char *pointer;        // ->> Next un-allocated region within `renumerator_arena::buffer`

      /* ... ->> Not intended to be re-initialized after destruction */
      inline ~renumerator_arena() /* noexcept */ {
        if (NULL != this -> storage.chunks) {
          this -> storage.chunks += this -> storage.chunksCapacity;

          while (this -> storage.chunksCapacity--)
          (--(this -> storage.chunks)) -> ~renumerator_arena();

          std::free(this -> storage.chunks);
        }

        if (this -> buffer != this -> storage.chunk)
        ::delete[] this -> buffer;
      }

      /* ... */
      /* [[nodiscard]] */ inline unsigned char* allocate(std::size_t const size, std::size_t const alignment, bool const growable = true) /* noexcept */ {
        unsigned char *allocated = this -> pointer;

        // ... ->> Cannot `renumerator_arena::allocate(…)` more than one `renumerator_arena::...::chunk` (or `renumerator_arena::bufferCapacity`) at a time
        if (size > this -> bufferCapacity)
        return NULL;

        // ...
        if (0u == this -> storage.chunksLength) {
          // ... ->> Align `allocated` region
          for (; 0u != (allocated - this -> buffer) % alignment; ++allocated)
          if (this -> bufferCapacity == static_cast<std::size_t>(allocated - this -> buffer)) break;

          // ... ->> Out of memory
          if (size > static_cast<std::size_t>((this -> buffer + this -> bufferCapacity) - allocated)) {
            if (false == growable)
            return NULL;

            // ... ->> Allocate `size` on `alignment` via dynamically managed `renumerator_arena::...::chunks`
            if (0u == this -> storage.chunksCapacity) {
              void *const allocation =
                #if __cplusplus < 201703L
                  std::malloc(sizeof(renumerator_arena) * 2u) // ->> Ideally `alignof renumerator_arena` (could be aligned by offset-ing)
                #else
                  std::aligned_alloc(sizeof(renumerator_arena), sizeof(renumerator_arena) * 2u)
                #endif
              ;

              // ...
              if (NULL == allocation)
              return NULL;

              this -> storage.chunksCapacity = 2u;
              this -> storage.chunks         = ::new (allocation) renumerator_arena[this -> storage.chunksCapacity];

              for (renumerator_arena *allocator = this -> storage.chunks; this -> storage.chunksCapacity != static_cast<std::size_t>(allocator - this -> storage.chunks); ++allocator) {
                allocator -> buffer         = allocator -> storage.chunk;
                allocator -> bufferCapacity = sizeof(allocator -> storage.chunk);
                allocator -> pointer        = allocator -> buffer;
              }
            }

            this -> storage.chunksLength = 1u;

            // ...
            return this -> allocate(size, alignment, true);
          }

          // ... ->> Prepare for next `renumerator_arena::allocate(…)`
          this -> pointer = allocated + size;
        }

        else {
          renumerator_arena *chunks    = this -> storage.chunks;
          renumerator_arena *allocator = chunks + (this -> storage.chunksLength - 1u);

          // ... ->> Attempt allocating from a `renumerator_arena::...::chunk` within `this -> storage.chunks`
          allocated = allocator -> allocate(size, alignment, false);

          if (NULL == allocated and allocator -> pointer != allocator -> storage.chunk)
          if (growable) {
            uintptr_t const value = reinterpret_cast<uintptr_t>(chunks);

            // ... ->> Unable to track additional `renumerator_arena::...::chunk`s
            if (SIZE_MAX == this -> storage.chunksLength)
            return NULL;

            if (this -> storage.chunksCapacity == this -> storage.chunksLength) {
              std::size_t capacity = this -> storage.chunksCapacity;

              // ... ->> Unable to allocate additional `renumerator_arena::...::chunk`s
              for (; this -> storage.chunksCapacity > SIZE_MAX - capacity; capacity /= 2u)
              if (capacity == 1u) return NULL;

              chunks = static_cast<renumerator_arena*>(std::realloc(chunks, (this -> storage.chunksCapacity + capacity) * sizeof(renumerator_arena))); // --> std::launder(static_cast<renumerator_arena*>(std::realloc(…)) + ...);

              if (NULL == chunks)
              return NULL;

              this -> invalidated = (
                value  != reinterpret_cast<uintptr_t>(chunks) or // ->> `std::realloc(…)` probably moved storage, can't guarantee using pointer inequality due to `chunks` possibly being invalidated
                chunks != this -> storage.chunks                 // ->> Then risk undefined behavior from accessing the possibly invalid `this -> storage.chunks` pointer to guarantee `std::realloc(…)` didn't move storage
              );

              // ... ->> Construct newly allocated `renumerator_arena`s
              this -> storage.chunksCapacity += capacity;
              this -> storage.chunks          = chunks;
              chunks                         += this -> storage.chunksCapacity;

              while (true) {
                allocator                   = ::new (--chunks) renumerator_arena();
                allocator -> buffer         = allocator -> storage.chunk;
                allocator -> bufferCapacity = sizeof(allocator -> storage.chunk);
                allocator -> pointer        = allocator -> buffer;

                if (this -> storage.chunksLength == static_cast<std::size_t>(chunks - this -> storage.chunks))
                break;
              }
            }

            // ... ->> Attempt allocating from a newly reserved `renumerator_arena::...::chunk`
            allocator                     = this -> storage.chunksLength++ + this -> storage.chunks;
            allocated                     = allocator -> allocate(size, alignment, false);
            this -> storage.chunksLength -= NULL == allocated;

            if (this -> invalidated)
            return NULL;
          }
        }

        // ...
        return allocated;
      }

      /* [[nodiscard]] */ inline unsigned char* reallocate(std::size_t const releaseSize, std::size_t const releaseAlignment, std::size_t const allocationSize, std::size_t const allocationAlignment, bool const growable = true) /* noexcept(noexcept(this -> release(releaseSize, releaseAlignment)) and noexcept(this -> allocate(allocationSize, allocationAlignment, growable))) */ {
        unsigned char       *reallocated  = NULL;
        std::size_t    const chunksLength = this -> storage.chunksLength;
        renumerator_arena   &allocator    = 0u != chunksLength ? this -> storage.chunks[chunksLength - 1u] : *this;
        unsigned char *const released     = allocator.pointer;

        // ... ->> Attempt a straight-forward `release(…)`-then-`allocate(…)` routine
        if (this -> release(releaseSize, releaseAlignment))
          reallocated = this -> allocate(allocationSize, allocationAlignment, growable);

        // ... ->> Otherwise reset the `renumerator_arena` state
        if (NULL == reallocated) {
          allocator.pointer            = released;
          this -> storage.chunksLength = chunksLength;
        }

        // ...
        return reallocated;
      }

      /* [[nodiscard]] */ inline static char const (&size(long double amount) /* noexcept */)[30] {
        static char                                   measurement[30] = {'\0'}; // --> char[(log10(SIZE_MAX) + strlen(".00")) + strlen(" ") + max(...(B, ..., YB)) + 1u]
        enum unit_t { B, KB, MB, TB, PB, EB, ZB, YB } unit            = B;
        char const                                   *unitString      = "bytes";

        // ... ->> Derive the correct unit
        for (; amount >= 1024.0L; amount /= 1024.0L) {
          unit = static_cast<unit_t>(static_cast<int>(unit) + 1);
          if (YB == unit) break;
        }

        // ... ->> Shift by `2` ranks --> std::sprintf(measurement, "%0.2Lf%c%2.5s", amount, ' ', unitString) >= 0
        amount *= 100.0L; {
          unsigned char place  = 6u;
          long double   rank   = 100000.0L;
          char         *string = measurement;

          // ...
          for (; 0u != place-- and LDBL_EPSILON < amount; rank /= 10.0L) {
            if (false) {}
            else if (amount >= rank * 9.0L) { amount -= rank * 9.0L; *(string++) = '9'; }
            else if (amount >= rank * 8.0L) { amount -= rank * 8.0L; *(string++) = '8'; }
            else if (amount >= rank * 7.0L) { amount -= rank * 7.0L; *(string++) = '7'; }
            else if (amount >= rank * 6.0L) { amount -= rank * 6.0L; *(string++) = '6'; }
            else if (amount >= rank * 5.0L) { amount -= rank * 5.0L; *(string++) = '5'; }
            else if (amount >= rank * 4.0L) { amount -= rank * 4.0L; *(string++) = '4'; }
            else if (amount >= rank * 3.0L) { amount -= rank * 3.0L; *(string++) = '3'; }
            else if (amount >= rank * 2.0L) { amount -= rank * 2.0L; *(string++) = '2'; }
            else if (amount >= rank * 1.0L) { amount -= rank * 1.0L; *(string++) = '1'; }
            else if (measurement != string) *(string++) = '0';

            if (place == 2u and LDBL_EPSILON < amount) {
              if (measurement == string)
                *(string++) = '0';

              *(string++) = '.';
            }
          }

          if (measurement == string)
            *(string++) = '0';

          *(string++) = ' ';

          switch (unit) {
            case EB: unitString = "EB"; break;
            case KB: unitString = "KB"; break;
            case MB: unitString = "MB"; break;
            case PB: unitString = "PB"; break;
            case TB: unitString = "TB"; break;
            case YB: unitString = "YB"; break;
            case ZB: unitString = "ZB"; break;
            default: {}
          }

          while ('\0' != *unitString)
            *(string++) = *(unitString++);

          *string = '\0';
        }

        return measurement;
      }

      /* [[nodiscard]] */ inline char const (&size() const /* noexcept */)[30] {
        std::size_t amount = this -> pointer - this -> buffer;

        // ...
        if (0u != this -> storage.chunksLength) {
          amount  = this -> bufferCapacity + ((this -> storage.chunksLength - 1u) * sizeof(this -> storage.chunk));
          amount += this -> storage.chunks[this -> storage.chunksLength - 1u].pointer - this -> storage.chunks[this -> storage.chunksLength - 1u].buffer;
        }

        return renumerator_arena::size(static_cast<long double>(amount));
      }

      inline bool release(std::size_t const size, std::size_t const alignment) /* noexcept */ {
        renumerator_arena &allocator = 0u != this -> storage.chunksLength ? this -> storage.chunks[this -> storage.chunksLength - 1u] : *this;
        unsigned char     *released  = allocator.pointer;

        // ... ->> Prevent `renumerator_arena::release(…)`s larger than prior `renumerator_arena::allocate(…)`s
        if (size > static_cast<std::size_t>(released - allocator.buffer))
        return false;

        // ...
        for (released -= size; ; --released) {
          if (0u == (released - allocator.buffer) % alignment) break;
          if (released == allocator.buffer) return false;
        }

        allocator.pointer             = released;
        this -> storage.chunksLength -= released == allocator.buffer and this != &allocator;

        // ...
        return true;
      }
    } allocator = {
      // ->> Linear allocator which releases/ reserves contiguous storage in order only
      {{}, NULL, 0u, 0u}, allocator.storage.chunk, sizeof(allocator.storage.chunk), false, allocator.storage.chunk
    };

    struct renum; // ->> `renum::sort(…)` function for `filePaths`
    struct renumerator_convert /* final */ {
      // ... ->> Convert command-line option to `renameFormat`
      inline static wchar_t* formatof(char const commandLineOption[], std::size_t* const size) /* noexcept */ {
        std::mbstate_t     state       = std::mbstate_t();
        char const        *option      = commandLineOption;
        std::mbstate_t     lengthState = state;
        std::size_t const  length      = std::mbsrtowcs(static_cast<wchar_t*>(NULL), &option, 0u, &lengthState);
        wchar_t           *format;

        // ...
        *size = 0u;

        if (length == static_cast<std::size_t>(-1)) return NULL;
        format = reinterpret_cast<wchar_t*>(allocator.allocate((length + 1u) * sizeof(wchar_t), sizeof(char), false));

        if (NULL == format) return NULL;
        format = ::new (format) wchar_t[length + 1u];
        option = commandLineOption;
        *size  = length;

        // ...
        return length == std::mbsrtowcs(format, &option, length + 1u, &state) ? format : NULL;
      }

      inline static wchar_t* formatof(wchar_t const commandLineOption[], std::size_t* const size) /* noexcept */ {
        *size = std::wcslen(commandLineOption);
        return const_cast<wchar_t*>(commandLineOption);
      }

      // ... ->> Convert NUL-terminated paths in `filePaths` to usable values for some file renaming function
      #if defined _WIN32
        inline static wchar_t const* renamableof(wchar_t const filePath[], std::size_t* const size) /* noexcept */ {
          *size = std::wcslen(filePath);
          return filePath;
        }
      #else
        inline static char const* renamableof(wchar_t const filePath[], std::size_t* const size) /* noexcept */ {
          std::mbstate_t     state             = {};
          char              *renamableFilePath = NULL;
          wchar_t     const *path              = filePath;
          std::mbstate_t     lengthState       = state;
          std::size_t const  length            = std::wcsrtombs(static_cast<char*>(NULL), &path, 0u, &lengthState);

          // ...
          *size = 0u;

          if (length == static_cast<std::size_t>(-1)) return NULL;
          renamableFilePath = reinterpret_cast<char*>(allocator.allocate(length + sizeof(char), sizeof(char), false));

          if (NULL == renamableFilePath) return NULL;
          path  = filePath;
          *size = length / sizeof(char);

          // ...
          return length == std::wcsrtombs(renamableFilePath, &path, length + sizeof(char), &state) ? renamableFilePath : NULL;
        }
      #endif
    };

    /* ... */
    bool             allocatorExhausted                = false;                        // ->> Indicator out-of-memory state for `allocator`
    bool             enumerateHiddenFiles              = false;                        // ->> Switch denoting `--hidden` command-line option
    bool             enumerateSystemFiles              = false;                        // ->> Switch denoting `--system` command-line option
    int              executableExitCode                = EXIT_SUCCESS;                 //
    bool             executableFilePathAllocated       = false;                        //
    wchar_t        **filePaths                         = NULL;                         // ->> Array                of NUL-terminated file paths
    wchar_t         *filePathsBuffer                   = NULL;                         // ->> NUL-delimited string of NUL-terminated file paths
    std::size_t      filePathsBufferSize               = 0u;                           // ->> With NUL terminator; Measured in `sizeof wchar_t`
    std::size_t      filePathsLength                   = 0u;                           // ->> Number of `filePaths`
    bool             memoryRequested                   = false;                        // ->> Switch denoting `--memory` (or `-m`) command-line option
    std::size_t      memorySize                        = 0u;                           // ->> Number of bytes allowed to be pre-allocated by the user
    bool /* const */ promptBuffersNewline              = true;                         // ->> Denotes if `stdin` reads cause the stream to additionally buffer with a `\n` character
    int              promptToken                       = EOF;                          // ->> User input
    static bool      quietRequested                    = false;                        // ->> Switch denoting `--quiet` (or `-q`) command-line option; Toggles most diagnostic logs
    wchar_t const   *renameFormat                      = L"??";                        // ->> See `--help` `message` for more information
    bool             renameFormatAllocated             = false;                        //
    bool             renameFormatIncludesRenameIndex   = false;                        // ->> `renameFormat` includes `?` or `??` character sequences
    std::size_t      renameFormatIndex                 = static_cast<std::size_t>(-1); // ->> Position within `arguments` array; Used for Windows-specific Unicode (`wchar_t`) re-interpretation of `arguments`
    std::size_t      renameFormatLength                = 0u;                           //
    std::size_t      renameIndex                       = 1u;                           //
    bool             renameIndexRequested              = false;                        // ->> Switch denoting `--index` (or `-i`) or `--zero` (or `-z`) command-line options
    bool             renumerationAbsolutePathAllocated = false;                        //
    bool             renumerationDirectoryAllocated    = false;                        //
    bool             renumerationDirectoryRecognized   = true;                         // ->> User specified empty directory or there was not enough memory to store it

    // ... ->> Parse command-line arguments for possible command-line options
    if (count > 2) {
      struct renumerator_options /* final */ {
        // ... ->> Trim (leading whitespace) from `commandLineArgument`
        inline static char* trim(char* commandLineArgument) /* noexcept */ {
          for (bool trimmed = false; false == trimmed; ) {
            char const character = *commandLineArgument;

            // ... --> std::isspace(character);
            switch (character) {
              case ' ': case '\f': case '\n': case '\r': case '\t': case '\v': ++commandLineArgument; break;
              default: trimmed = true; break;
            }
          }

          return commandLineArgument;
        }
      };

      /* ... */
      enum /* : int */ { FORMAT, HELP, HIDDEN, INDEX, MEMORY, QUIET, SYSTEM, ZERO };
      bool commandLineOptionDuplicated = false;
      bool commandLineOptionRecognized = false;
      bool helpRequested               = false;

      // ... ->> Iterate over `arguments`, sub-iterate over `commandLineOptions`
      for (char **commandLineArguments = arguments + 2; NULL != *commandLineArguments; ++commandLineArguments)
      for (struct { signed type; char value[9]; } commandLineOptions[] = {
        {FORMAT, "--format"}, {FORMAT, "-f"},
        {HELP,   "--help"  }, {HELP,   "-h"},
        {HIDDEN, "--hidden"},
        {INDEX,  "--index" }, {INDEX,  "-i"},
        {MEMORY, "--memory"}, {MEMORY, "-m"},
        {QUIET,  "--quiet" }, {QUIET,  "-q"},
        {SYSTEM, "--system"},
        {ZERO,   "--zero"  }, {ZERO,   "-z"}
      }, *commandLineOption = commandLineOptions; NULL != commandLineOption and commandLineOption != commandLineOptions + (sizeof(commandLineOptions) / sizeof(*commandLineOption)); commandLineOption += NULL != commandLineOption) {
        // ... ->> String-match iterate over `commandLineArgument` and `option`
        for (char *commandLineArgument = renumerator_options::trim(*commandLineArguments), *option = commandLineOption -> value; ; ++commandLineArgument, ++option) {
          commandLineOptionRecognized = false;

          // ... ->> `commandLineArgument` begins with `option`
          if ('\0' == *option) {
            char *const trimmedCommandLineArgument = renumerator_options::trim(commandLineArgument);

            // ... ->> Recognized command-line option
            if (
              FORMAT == commandLineOption -> type ||
              INDEX  == commandLineOption -> type ||
              MEMORY == commandLineOption -> type
            ) {
              // ... ->> No value found for command-line option
              if ('\0' == *trimmedCommandLineArgument) {
                if (false == quietRequested)
                (void) std::fprintf(stderr, "%1.71s%s%c", "renum: Expected corresponding command-line option value using `=` for `", commandLineOption -> value, '`');

                return EXIT_FAILURE;
              }

              // ... ->> No value found for command-line option
              if (*trimmedCommandLineArgument != '=') {
                if (commandLineArgument != trimmedCommandLineArgument) {
                  if (false == quietRequested)
                  (void) std::fprintf(stderr, "%1.61s%s%1.43s", "renum: Expected corresponding command-line option value for `", commandLineOption -> value, "`; consider using `=` instead of whitespace");

                  return EXIT_FAILURE;
                }

                commandLineOptionRecognized = false;
              }

              // ... ->> Value found for command-line option
              else {
                commandLineOptionRecognized = true;

                // ...
                if (FORMAT == commandLineOption -> type) {
                  commandLineArgument   = trimmedCommandLineArgument + 1;
                  renameFormat          = renumerator_convert::formatof(commandLineArgument, &renameFormatLength);
                  renameFormatAllocated = NULL != renameFormat;
                  renameFormatIndex     = commandLineArguments - arguments;

                  for (; '\0' != *commandLineArgument; ++commandLineArgument)
                    renameFormatIncludesRenameIndex = renameFormatIncludesRenameIndex or *commandLineArgument == '?';

                  // ...
                  if (false == renameFormatIncludesRenameIndex) {
                    if (false == quietRequested)
                    (void) std::fprintf(stderr, "%1.75s%s%c", "renum: Expected `?`, `??`, or `???` character sequences in rename format: \"", trimmedCommandLineArgument + 1, '"');

                    return EXIT_FAILURE;
                  }
                }

                else /* if (commandLineOption -> type & (INDEX | MEMORY)) */ {
                  bool         calculable = true; // ->> Command-line option is representable using `value` or not `NaT` (Not-a-Thing)
                  char const  *names   [(INDEX > MEMORY ? INDEX : MEMORY) + 1u];
                  bool        *switches[(INDEX > MEMORY ? INDEX : MEMORY) + 1u];
                  std::size_t &value =
                    INDEX  == commandLineOption -> type ? renameIndex :
                    MEMORY == commandLineOption -> type ? memorySize  :
                  /* --> std::unreachable() */ const_cast<std::size_t&>(static_cast<std::size_t const&>(0u));

                  // ...
                  names   [INDEX]  = "`--index` (`-i`)"  " " "option";
                  names   [MEMORY] = "`--memory` (`-m`)" " " "option";
                  switches[INDEX]  = &renameIndexRequested;
                  switches[MEMORY] = &memoryRequested;

                  if (*switches[commandLineOption -> type]) {
                    commandLineOptionDuplicated = true;

                    (void) std::fprintf(stdout, "%1.7s%23.24s%1.27s", "renum: ", names[commandLineOption -> type], " specified more than once" "\r\n");
                    (void) std::fflush(stdout);
                  }

                  commandLineArgument                  = trimmedCommandLineArgument + 1;
                  *switches[commandLineOption -> type] = true;
                  value                                = 0u;

                  // ... ->> Account for leading zeroes
                  if (*commandLineArgument == '0')
                  switch (*renumerator_options::trim(commandLineArgument + 1)) {
                    // ... ->> Single digit zero
                    case '\0': break;

                    // ... ->> Redundant leading zero(es)
                    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': /* --> 0 != std::isdigit(static_cast<unsigned char>(…)) */ {
                      commandLineArgument = NULL;

                      if (false == quietRequested)
                      (void) std::fputs("renum: Expected integer value (with no leading zeroes) for `", stderr);
                    } break;

                    // ... ->> Invalid character
                    default: {
                      commandLineArgument = NULL;

                      if (false == quietRequested)
                      (void) std::fputs("renum: Expected integer value for `", stderr);
                    } break;
                  }

                  // ... ->> Parse command-line option as non-signed integer less than `static_cast<unsigned long>(SIZE_MAX)`
                  if (NULL != commandLineArgument) {
                    for (std::size_t rank = 1u; ; ++commandLineArgument, rank *= 10u) {
                      char const          character = *commandLineArgument;
                      unsigned char const digit     = static_cast<unsigned char>(character - '0');
                      char const   *const token     = renumerator_options::trim(commandLineArgument);

                      // ... ->> End of significant command-line option value
                      if ('\0' == *token)
                      break;

                      // ... ->> Invalid "digit" encountered or command-line option broken up with whitespace
                      if (commandLineArgument != token or (/* --> 0 == std::isdigit(static_cast<unsigned char>(character)) */ character != '0' and character != '1' and character != '2' and character != '3' and character != '4' and character != '5' and character != '6' and character != '7' and character != '8' and character != '9')) {
                        commandLineArgument = NULL;

                        if (false == quietRequested)
                        (void) std::fputs("renum: Expected integer value for `", stderr);

                        break;
                      }

                      // ...
                      calculable = calculable and value <= static_cast<unsigned long>(SIZE_MAX / rank) - digit;
                      if (calculable) value = (value * 10u) + digit;
                    }

                    // ...
                    if (false == calculable) {
                      if (false == quietRequested)
                      (void) std::fprintf(stderr, "%1.22s%s%1.17s%2.8s%1.33s%lu%1.55s", "renum: Integer value `", trimmedCommandLineArgument + 1, "` specified for `", commandLineOption -> value, "` too large; maximum allowed is `", static_cast<unsigned long>(SIZE_MAX), "` minus the number of files in the enumerated directory");

                      return EXIT_FAILURE;
                    }
                  }

                  // ... ->> Error parsing command-line option value as non-signed integer
                  if (NULL == commandLineArgument) {
                    if (false == quietRequested)
                    (void) std::fprintf(stderr, "%2.8s%1.28s%s%1.9s", commandLineOption -> value, "` command-line option; got `", renumerator_options::trim(trimmedCommandLineArgument + 1), "` instead");

                    return EXIT_FAILURE;
                  }
                }
              }
            }

            else if ('\0' == *trimmedCommandLineArgument) {
              char const *names   [(/* --> max(…) */ HELP | HIDDEN | QUIET | SYSTEM | ZERO) + 1u];
              bool       *switches[(/* --> max(…) */ HELP | HIDDEN | QUIET | SYSTEM | ZERO) + 1u];

              // ...
              names   [HELP]   = "`--help` (`-h`)"                     " " "option";
              names   [HIDDEN] = "`--hidden`"                          " " "option";
              names   [QUIET]  = "`--quiet` (`-q`)"                    " " "option";
              names   [SYSTEM] = "`--system`"                          " " "option";
              names   [ZERO]   = "`--index` (`-i`) or `--zero` (`-z`)" " " "options";
              switches[HELP]   = &helpRequested;
              switches[HIDDEN] = &enumerateHiddenFiles;
              switches[QUIET]  = &quietRequested;
              switches[SYSTEM] = &enumerateSystemFiles;
              switches[ZERO]   = &renameIndexRequested;

              // ...
              if (*switches[commandLineOption -> type]) {
                (void) std::fprintf(stdout, "%1.7s%17.43s%1.27s", "renum: ", names[commandLineOption -> type], " specified more than once" "\r\n");
                (void) std::fflush(stdout);

                commandLineOptionDuplicated = true;
              }

              commandLineOptionRecognized          = true;
              *switches[commandLineOption -> type] = true;

              if (ZERO == commandLineOption -> type)
              renameIndex = 0u;
            }

            // ... ->> Unrecognized command-line option
            if (false == commandLineOptionRecognized) {
              if (false == quietRequested) {
                (void) std::fputs("renum: `", stderr);

                for (commandLineArgument = renumerator_options::trim(*commandLineArguments); ; ++commandLineArgument) {
                  char const token = *renumerator_options::trim(commandLineArgument);

                  // ...
                  if ('\0' == token or token == '=') break;
                  (void) std::fputc(*commandLineArgument, stderr);
                }

                (void) std::fprintf(stderr, "%1.57s%2.8s%1.11s", "` is not a recognized command-line option; did you mean `", commandLineOption -> value, "`, instead?");
              }

              return EXIT_FAILURE;
            }

            // ...
            commandLineOption = NULL; // ->> Next `commandLineArguments`
            break;                    // ->> Next `commandLineOptions`
          }

          // ... ->> `commandLineArgument` doesn't match with `option`
          if (*commandLineArgument != *option) {
            if (commandLineOption != commandLineOptions + ((sizeof(commandLineOptions) / sizeof(*commandLineOption)) - 1u))
            break; // ->> Next `commandLineOptions`

            // ... ->> Unrecognized command-line option, instead
            if ('-' == *renumerator_options::trim(*commandLineArguments)) {
              if (false == quietRequested) {
                (void) std::fputc('`', stderr);

                for (commandLineArgument = renumerator_options::trim(*commandLineArguments); ; ++commandLineArgument) {
                  char const token = *renumerator_options::trim(commandLineArgument);

                  // ...
                  if ('\0' == token or token == '=') break;
                  (void) std::fputc(*commandLineArgument, stderr);
                }

                (void) std::fputs("` is not a recognized command-line option", stderr);
              }

              // ...
              return EXIT_FAILURE;
            }

            // ... ->> Assume `commandLineArgument` is the `--format` command-line option
            if (commandLineArguments - arguments < 2) {
              std::size_t       length  = 0u;
              std::size_t const maximum = 18u; // ->> Maximum number of "characters" (less than `INT_MAX`) in `commandLineArgument` to log

              // ...
              for (commandLineArgument = *commandLineArguments; '\0' != *commandLineArgument; ++commandLineArgument)
              if (++length == maximum + 1u) break;

              if (false == quietRequested)
              (void) std::fprintf(stderr, "%1.41s%1.*s%1.3s%1.13s", "renum: Unrecognized command-line option `", static_cast<int>(maximum), '\0' == **commandLineArguments ? "\0" : *commandLineArguments, length == maximum + 1u ? "..." : "\0", "` encountered");

              // ...
              return EXIT_FAILURE;
            }

            commandLineArgument   = *commandLineArguments;
            renameFormat          = renumerator_convert::formatof(commandLineArgument, &renameFormatLength);
            renameFormatAllocated = NULL != renameFormat;
            renameFormatIndex     = commandLineArguments - arguments;

            for (; '\0' != *commandLineArgument; ++commandLineArgument)
              renameFormatIncludesRenameIndex = renameFormatIncludesRenameIndex or *commandLineArgument == '?';

            if (false == renameFormatIncludesRenameIndex) {
              if (false == quietRequested)
              (void) std::fprintf(stderr, "%1.75s%s%c", "renum: Expected `?`, `??`, or `???` character sequences in rename format: \"", *commandLineArguments, '"');

              return EXIT_FAILURE;
            }

            break; // ->> Next `commandLineOptions`
          }
        }
      }

      // ...
      if (helpRequested) {
        char const message[] =
          "renum [current_working_directory] [options] [rename_format]"                                                                                                                             "\r\n"
          "  Renames files by their indexes"                                                                                                                                                        "\r\n"
          ""                                                                                                                                                                                        "\r\n"
          "  current_working_directory = Location containing files to be renamed"                                                                                                                   "\r\n"
          "  rename_format             = New (multi-byte) name to rename files as (should be ideally valid for its filesystem);"                                                                    "\r\n"
          "                                must include either `?` or `??` character sequences (at least once) to proceed renaming files"                                                           "\r\n"
          "                                ?   = Index of a file"                                                                                                                                   "\r\n"
          "                                ??  = Evenly padded index of a file with leading zeroes"                                                                                                 "\r\n"
          "                                ??? = Name of a file"                                                                                                                                    "\r\n"
          ""                                                                                                                                                                                        "\r\n"
          "                                example(s): "                                                                                                                                            "\r\n"
          "                                - \"? - file\"  denotes \"69 - file.bmp\"       for the 69th file (with extension \"bmp\") to be renamed"                                                "\r\n"
          "                                - \"?? - file\" denotes \"069 - file.jpeg\"     for the 69th file (with extension \"jpeg\") to be renamed in a directory of a hundred (100-999) files"   "\r\n"
          "                                - \"??? file\"  denotes \"sixty-nine file.png\" for the file named \"sixty-nine\" (with extension \"png\") to be renamed; must be used with `?` or `??`" "\r\n"
          "  options                   = Command-line options to configure how files are renamed"                                                                                                   "\r\n"
          "                                --format | -f = Specifies the `rename_format`"                                                                                                           "\r\n"
          "                                --help   | -h = Logs this help message"                                                                                                                  "\r\n"
          "                                --hidden      = Renames hidden files, as well (ignored by default)"                                                                                      "\r\n"
          "                                --index  | -i = Specifies the starting offset for the file index;"                                                                                       "\r\n"
          "                                  example: \"file-?\" would denote \"file-420\" instead of \"file-69\" with an offset of 351 (420 minus 69)"                                             "\r\n"
          "                                --memory | -m = Specifies the number of bytes allowed to pre-allocate for ideally more available memory"                                                 "\r\n"
          "                                --quiet  | -q = Silences (most) output diagnostic to the standard error or output devices"                                                               "\r\n"
          "                                --system      = Renames system files, as well (ignored by default)"                                                                                      "\r\n"
          "                                --zero   | -z = Sets the starting offset for the file index to 0"                                                                                        "\0"
        ;

        return std::fprintf(stdout, "%s%1.*s", commandLineOptionDuplicated ? "\n" : "", static_cast<int>((sizeof(message) / sizeof(char)) - 2u), message) < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
      }
    }

    // ... ->> Allocate user-requested memory size
    if (memorySize > sizeof(allocator.storage.chunk)) {
      allocator.buffer = ::new (std::nothrow) unsigned char[memorySize];

      // ...
      if (NULL == allocator.buffer) {
        allocator.buffer = allocator.storage.chunk;
        (void) std::fprintf(stdout, "%1.30s%s%1.15s", "renum: Failed to pre-allocate ", renumerator_arena::size(static_cast<long double>(memorySize)), " as requested" "\r\n");
      }

      else {
        allocator.bufferCapacity = memorySize;
        allocator.pointer        = allocator.buffer;
      }
    }

    // ... ->> Must declare `executableFilePath`, `executableFilePathLength`, `renumerationAbsolutePath`, `renumerationAbsolutePathLength`, `renumerationDirectory`, and `renumerationDirectoryLength` variables; See `else` case
    #if defined _WIN32
      struct renum /* final */ {
        inline static int sort(void const* const filePathA, void const* const filePathB) /* noexcept */ {
          switch (::CompareStringEx(LOCALE_NAME_USER_DEFAULT /* --> L"en-US" */, SORT_DIGITSASNUMBERS | SORT_STRINGSORT, *static_cast<wchar_t* const*>(filePathA), -1, *static_cast<wchar_t* const*>(filePathB), -1, static_cast<LPNLSVERSIONINFO>(NULL), static_cast<LPVOID>(NULL), static_cast<LPARAM>(0x00))) {
            case 0: // ->> Silently fails `filePathA <=> filePathB`
            case CSTR_EQUAL:        return 0;
            case CSTR_GREATER_THAN: return +1;
            case CSTR_LESS_THAN:    return -1;
          }

          return 0;
        }
      };

      /* ... */
      (void) ::SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_SYSTEM32);

      // ...
      HMODULE const normaliz = ::LoadLibraryExW(L"normaliz" ".dll", static_cast<HANDLE>(NULL), 0x00u);
      HMODULE const shell32  = ::LoadLibraryExW(L"shell32"  ".dll", static_cast<HANDLE>(NULL), 0x00u);
        LPWSTR* (*const CommandLineToArgvW)(LPCWSTR, int[])                       = reinterpret_cast<LPWSTR* (*)(LPCWSTR, int[])>                      (reinterpret_cast<void*>(NULL != shell32  ? ::GetProcAddress(shell32,  "CommandLineToArgvW") : NULL));
        int     (*const NormalizeString)   (NORM_FORM, LPCWSTR, int, LPWSTR, int) = reinterpret_cast<int     (*)(NORM_FORM, LPCWSTR, int, LPWSTR, int)>(reinterpret_cast<void*>(NULL != normaliz ? ::GetProcAddress(normaliz, "NormalizeString")    : NULL));

      std::size_t       renumerationDirectoryLength     = 0u;                                                                                                                          //
      HANDLE            renumerationDirectoryFindHandle = INVALID_HANDLE_VALUE;                                                                                                        //
      WIN32_FIND_DATAW  renumerationDirectoryFindData   = {};                                                                                                                          //
      LPTSTR            renumerationDirectory           = NULL;                                                                                                                        // ->> Directory to enumerate files to be renamed
      std::size_t       renumerationAbsolutePathLength  = 0u;                                                                                                                          //
      LPWSTR            renumerationAbsolutePath        = NULL;                                                                                                                        //
      std::size_t const subdirectorySuffixLength        = 2u;                                                                                                                          //
      LPCTSTR     const subdirectorySuffix              = L"\\*";                                                                                                                      // ->> Windows-specific character sequence which instructs its filesystem APIs to access sub-directories of a specified path
      BOOL              filesystemRedirectionDisabled   = FALSE;                                                                                                                       // ->> Indicated by `::Wow64DisableWow64FsRedirection(…)`
      PVOID             filesystemRedirection           = NULL;                                                                                                                        // ->> Used      by `::Wow64DisableWow64FsRedirection(…)`
      std::size_t       executableFilePathLength        = MAX_PATH;                                                                                                                    // ->> With NUL terminator(s)
      LPWSTR            executableFilePath              = reinterpret_cast<LPWSTR>(allocator.allocate(executableFilePathLength * sizeof(WCHAR), sizeof(WCHAR), false));                // ->> Avoid renaming executable
      TCHAR       const defaultDirectory[]              = L"." "\\*";                                                                                                                  //
      int               commandLineArgumentsLength      = 0;                                                                                                                           //
      LPWSTR     *const commandLineArguments            = NULL != shell32 and NULL != CommandLineToArgvW ? CommandLineToArgvW(::GetCommandLineW(), &commandLineArgumentsLength) : NULL; // ->> UTF-16 (endian-dependent) encoded version of `arguments`

      // ... ->> Inquire `executableFilePath`
      for (executableFilePath = NULL != executableFilePath ? ::new (executableFilePath) WCHAR[executableFilePathLength] : NULL; NULL != executableFilePath; ) {
        DWORD const fileNameLength = (static_cast<void>(::SetLastError(ERROR_SUCCESS)), ::GetModuleFileNameW(static_cast<HMODULE>(NULL), executableFilePath, static_cast<DWORD>(executableFilePathLength)));

        // ...
        if (FALSE == fileNameLength) {
          // ... ->> Retry `::GetModuleFileName(…)` with ideally no truncation
          if (
            ERROR_INSUFFICIENT_BUFFER == ::GetLastError() ||
            ERROR_SUCCESS             == ::GetLastError() // ->> See previous `::SetLastError(…)` call
          ) {
            bool fileNameInquirable = false;

            // ...
            for (std::size_t length = executableFilePathLength; 0u != length and executableFilePathLength > SIZE_MAX - length; length /= 2u) {
              unsigned char *const reallocation = allocator.reallocate(executableFilePathLength * sizeof(WCHAR), sizeof(WCHAR), (executableFilePathLength + length) * sizeof(WCHAR), sizeof(WCHAR), false);

              // ...
              if (NULL != reallocation) {
                fileNameInquirable        = true;
                executableFilePathLength += length;
                executableFilePath        = ::new (reallocation) WCHAR[executableFilePathLength];

                break;
              }
            }

            if (fileNameInquirable) {
              executableFilePathAllocated = true;
              break;
            }

            (void) allocator.release(executableFilePathLength * sizeof(WCHAR), sizeof(WCHAR)); // --> executableFilePath
            executableFilePath       = NULL;
            executableFilePathLength = 0u;
          }

          // ... ->> Get `executableFilePath` via `extern char *::_pgmptr`, instead
          #ifndef _MSC_VER
            static_cast<void>(NormalizeString);
          #else
            char       *programPointer;
            std::size_t programPointerSize = 0u; // ->> With NUL terminator

            // ...
            if (NULL == executableFilePath) {
              bool fileNameConvertible = (
                0x00 == ::_get_pgmptr(&programPointer) &&
                SUCCEEDED(::StringCbLengthA(static_cast<STRSAFE_PCNZCH>(programPointer), STRSAFE_MAX_CCH * sizeof(char), &programPointerSize)) // ->> Cannot retrieve length alternatively
              );

              // ... ->> Convert `::_pgmptr` to an appropriate `wchar_t[]` string via the `::MultiByteToWideChar(…)` (and `NormalizeString(…)`) function(s)
              if (fileNameConvertible) {
                DWORD flag = MB_ERR_INVALID_CHARS | MB_PRECOMPOSED;
                int   length;

                // ...
                programPointerSize += sizeof(char);

                while (true) {
                  length = ::MultiByteToWideChar(CP_THREAD_ACP, flag, programPointer, static_cast<DWORD>(programPointerSize), static_cast<LPWSTR>(NULL), 0);

                  // ... ->> `programPointer` can be successfully converted
                  if (0 /* --> ERROR_NO_UNICODE_TRANSLATION */ != length) {
                    fileNameConvertible = true;
                    break;
                  }

                  // ... ->> Retry with no conversion flags set
                  if (0x00u != flag and ERROR_INVALID_FLAGS == ::GetLastError()) {
                    flag = 0x00u;
                    continue;
                  }

                  // ...
                  fileNameConvertible = false;
                  break;
                }

                do if (fileNameConvertible) {
                  if (length < 0) { fileNameConvertible = false; break; } // ->> While not explicitly a `::MultiByteToWideChar(…)` fail state, a positive `length` is required regardless
                  executableFilePath = reinterpret_cast<LPWSTR>(allocator.allocate((length + 1) * sizeof(WCHAR), sizeof(WCHAR), false));

                  if (NULL == executableFilePath) { fileNameConvertible = false; break; }
                  executableFilePath       = ::new (executableFilePath) WCHAR[length + 1];
                  executableFilePathLength = length + 1;
                  length                   = ::MultiByteToWideChar(CP_THREAD_ACP, flag, programPointer, static_cast<DWORD>(programPointerSize), executableFilePath, length + 1);

                  if (0 == length) { fileNameConvertible = false; break; }
                  if (NULL != normaliz and NULL != NormalizeString) {
                    LPWSTR denormalized;
                    int    normalizedLength = NormalizeString(::NormalizationC, executableFilePath, length, static_cast<LPWSTR>(NULL), 0);

                    // ...
                    if (0 == normalizedLength) { fileNameConvertible = false; break; }
                    if (normalizedLength < 0) {
                      if (ERROR_INSUFFICIENT_BUFFER == ::GetLastError()) { fileNameConvertible = false; break; }
                      normalizedLength = -normalizedLength; // --> ERROR_NO_UNICODE_TRANSLATION ->> Truncate `executableFilePath` to only contain valid Unicode characters
                    }

                    denormalized = reinterpret_cast<LPWSTR>(allocator.allocate((length + 1u) * sizeof(WCHAR), sizeof(WCHAR), false)); // ->> Unspecified if `NormalizeString(…)` accepts overlapping strings, so we allocate for additional storage instead
                    if (NULL == denormalized) { fileNameConvertible = false; break; }

                    denormalized = ::new (denormalized) WCHAR[length];
                    denormalized = FAILED(::StringCchCopyNExW(static_cast<STRSAFE_LPWSTR>(denormalized), length + 1, executableFilePath, length, static_cast<LPTSTR*>(NULL), static_cast<std::size_t*>(NULL), STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE)) ? std::wcsncpy(denormalized, executableFilePath, length) : denormalized;

                    if (length != normalizedLength) {
                      executableFilePath = reinterpret_cast<LPWSTR>(allocator.reallocate(length * sizeof(WCHAR), sizeof(WCHAR), (normalizedLength + 1) * sizeof(WCHAR), sizeof(WCHAR), false));
                      if (NULL == executableFilePath) { fileNameConvertible = false; break; }

                      executableFilePath       = ::new (executableFilePath) WCHAR[normalizedLength + 1];
                      executableFilePath       = FAILED(::StringCchCopyNExW(static_cast<STRSAFE_LPWSTR>(executableFilePath), normalizedLength + 1, denormalized, length, static_cast<LPTSTR*>(NULL), static_cast<std::size_t*>(NULL), STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE)) ? std::wcsncpy(executableFilePath, denormalized, length) : executableFilePath;
                      executableFilePathLength = normalizedLength + 1;
                    }

                    normalizedLength = NormalizeString(::NormalizationC, denormalized, length, executableFilePath, normalizedLength + 1);
                    (void) allocator.release((length + 1) * sizeof(WCHAR), sizeof(WCHAR)); // --> denormalized ->> Possibly overwritten by storage for `executableFilePath` on failure
                    if (normalizedLength <= 0) { fileNameConvertible = false; break; }
                  }

                  executableFilePath[executableFilePathLength - 1u] = L'\0';
                } while (false);
              }

              // ...
              if (fileNameConvertible) {
                executableFilePathAllocated = true;
                break;
              }

              (void) allocator.release(executableFilePathLength * sizeof(WCHAR), sizeof(WCHAR)); // --> executableFilePath
              executableFilePath       = NULL;
              executableFilePathLength = 0u;
            }
          #endif

          // ... ->> Get `executableFilePath` via `::CommandLineToArgvW(L"", ...)`, instead
          if (NULL == executableFilePath) {
            int           executableCommandLineArgumentsLength = 0u;
            LPWSTR *const executableCommandLineArguments       = NULL != shell32 and NULL != CommandLineToArgvW ? CommandLineToArgvW(L"", &executableCommandLineArgumentsLength) : NULL;

            // ...
            if (NULL != executableCommandLineArguments) {
              executableFilePath       = *executableCommandLineArguments;
              executableFilePathLength = FAILED(::StringCchLengthW(executableFilePath, STRSAFE_MAX_CCH, &executableFilePathLength)) ? std::wcslen(executableFilePath) : executableFilePathLength;

              ++executableFilePathLength;
              (void) ::LocalFree(executableCommandLineArguments); // ->> Ideally successfully evaluates to `NULL`
            }

            break;
          }

          executableFilePath       = NULL;
          executableFilePathLength = 0u;
        }

        if (executableFilePathLength > fileNameLength) {
          unsigned char *const reallocation = allocator.reallocate(executableFilePathLength * sizeof(WCHAR), sizeof(WCHAR), (fileNameLength + 1u) * sizeof(WCHAR), sizeof(WCHAR), false);

          // ...
          executableFilePathAllocated = true;

          if (NULL == reallocation) {
            for (LPWSTR path = executableFilePath + fileNameLength; executableFilePathLength != static_cast<std::size_t>(path - executableFilePath); ++path)
            *path = L'\0'; // ->> Zero out trailing storage with NUL terminators
          }

          else {
            executableFilePath       = reinterpret_cast<LPWSTR>(reallocation); // --> std::launder(reinterpret_cast<LPWSTR>(reallocation) + ...);
            executableFilePathLength = fileNameLength + 1u;
          }

          // ...
          break;
        }
      }

      // ... ->> Update `renameFormat`
      if (NULL != commandLineArguments and static_cast<std::size_t>(commandLineArgumentsLength) > renameFormatIndex and renameFormatIndex != static_cast<std::size_t>(-1)) {
        if (renameFormatAllocated)
          (void) allocator.release((renameFormatLength + 1u) * sizeof(wchar_t), sizeof(wchar_t)); // --> renameFormat

        renameFormat          = commandLineArguments[renameFormatIndex];
        renameFormatAllocated = false;
        renameFormatLength    = FAILED(::StringCchLengthW(renameFormat, STRSAFE_MAX_CCH, &renameFormatLength)) ? std::wcslen(renameFormat) : renameFormatLength;
      }

      // ... ->> Set `renumerationDirectory` to a user-specified command-line argument
      if (NULL != commandLineArguments and commandLineArgumentsLength > 1) {
        std::size_t commandLineArgumentLength = 0u;

        // ...
        commandLineArgumentLength   = FAILED(::StringCchLengthW(commandLineArguments[1], STRSAFE_MAX_CCH, &commandLineArgumentLength)) ? std::wcslen(commandLineArguments[1]) : commandLineArgumentLength;
        renumerationDirectoryLength = commandLineArgumentLength + subdirectorySuffixLength;
        renumerationDirectory       = reinterpret_cast<LPTSTR>(allocator.allocate((renumerationDirectoryLength + 1u) * sizeof(TCHAR), sizeof(TCHAR), false));

        // ...
        if (NULL == renumerationDirectory)
          renumerationDirectoryRecognized = false;

        else {
          renumerationDirectory          = ::new (renumerationDirectory) TCHAR[renumerationDirectoryLength + 1u];
          renumerationDirectory          = FAILED(::StringCchCopyNExW(static_cast<STRSAFE_LPWSTR>(renumerationDirectory), renumerationDirectoryLength, commandLineArguments[1], commandLineArgumentLength + 1u, static_cast<LPTSTR*>(NULL), static_cast<std::size_t*>(NULL), STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE)) ? std::wcsncpy(renumerationDirectory, commandLineArguments[1], commandLineArgumentLength + 1u) : renumerationDirectory;
          renumerationDirectoryAllocated = true;

          (void) ::LocalFree(commandLineArguments); // ->> Ideally successfully evaluates to `NULL`

          // ... ->> Modify `renumerationDirectory` to be a more Windows-compatible directory path
          for (LPTSTR directory = renumerationDirectory; ; ++directory) {
            if (L'\0' == *directory) {
              do {
                if (*--directory != L'\\') {
                  ++directory;

                  if (FAILED(::StringCchCatNExW(directory, (renumerationDirectoryLength + 1u) - (directory - renumerationDirectory), subdirectorySuffix, subdirectorySuffixLength, static_cast<LPTSTR*>(NULL), static_cast<std::size_t*>(NULL), STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE)))
                  (void) std::wcsncat(directory, subdirectorySuffix, subdirectorySuffixLength);

                  // ...
                  break;
                }

                // ... ->> Remove trailing backslashes
                *directory = L'\0';
              } while (directory != renumerationDirectory);

              if (NULL != renumerationDirectory and L'\0' == *renumerationDirectory) {
                (void) allocator.release((renumerationDirectoryLength + 1u) * sizeof(WCHAR), sizeof(WCHAR)); // --> renumerationDirectory

                renumerationDirectory           = NULL;
                renumerationDirectoryRecognized = false;
              }

              break;
            }

            // ... ->> Replace (forward) slashes with backslashes
            if (*directory == L'/')
            *directory = L'\\';
          }
        }
      }

      // ... ->> Set `renumerationDirectory` otherwise to the current working directory
      if (NULL == renumerationDirectory) {
        DWORD const currentDirectoryLength = ::GetCurrentDirectory(0u, static_cast<LPTSTR>(NULL)); // ->> With NUL terminator

        // ...
        if (0u != currentDirectoryLength) {
          renumerationDirectoryLength = (currentDirectoryLength - 1u) + subdirectorySuffixLength;
          renumerationDirectory       = reinterpret_cast<LPTSTR>(allocator.allocate((renumerationDirectoryLength + 1u) * sizeof(WCHAR), sizeof(WCHAR), false));

          // ...
          if (NULL != renumerationDirectory) {
            renumerationDirectory          = ::new (renumerationDirectory) TCHAR[renumerationDirectoryLength + 1u];
            renumerationDirectoryAllocated = true;

            if (0u != ::GetCurrentDirectory(currentDirectoryLength, renumerationDirectory)) {
              if (FAILED(::StringCchCatNExW(renumerationDirectory + (currentDirectoryLength - 1u), renumerationDirectoryLength - (currentDirectoryLength - 1u), subdirectorySuffix, subdirectorySuffixLength, static_cast<LPTSTR*>(NULL), static_cast<std::size_t*>(NULL), STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE)))
              (void) std::wcsncat(renumerationDirectory + (currentDirectoryLength - 1u), subdirectorySuffix, subdirectorySuffixLength);
            }
          }
        }
      }

      // ... ->> Set `renumerationDirectory` otherwise to `defaultDirectory`
      if (NULL == renumerationDirectory) {
        renumerationDirectory          = const_cast<LPTSTR>(defaultDirectory);
        renumerationDirectoryAllocated = false;

        if (FAILED(::StringCchLengthW(renumerationDirectory, STRSAFE_MAX_CCH, &renumerationDirectoryLength)))
        renumerationDirectoryLength = (sizeof(defaultDirectory) / sizeof(TCHAR)) - 1u;
      }

      // ... ->> Canonize `renumerationDirectory` to its absolute path
      renumerationAbsolutePathLength = renumerationDirectoryLength;
      renumerationAbsolutePath       = reinterpret_cast<LPWSTR>(allocator.allocate((renumerationAbsolutePathLength + 1u) * sizeof(WCHAR), sizeof(WCHAR), false));

      if (NULL != renumerationAbsolutePath) {
        DWORD length;

        // ...
        renumerationAbsolutePathAllocated = true;
        renumerationAbsolutePath          = ::new (renumerationAbsolutePath) WCHAR[renumerationAbsolutePathLength + 1u];
        length                            = ::GetFullPathNameW(static_cast<LPCWSTR>(renumerationDirectory), static_cast<DWORD>(((renumerationAbsolutePathLength + 1u) * sizeof(WCHAR)) / sizeof(TCHAR)), renumerationAbsolutePath, static_cast<LPWSTR*>(NULL));

        if (0u != length and length > renumerationAbsolutePathLength * sizeof(WCHAR)) {
          unsigned char *reallocation = allocator.reallocate((renumerationAbsolutePathLength + 1u) * sizeof(WCHAR), sizeof(WCHAR), ((length + 1u) / sizeof(TCHAR)) * sizeof(WCHAR), sizeof(WCHAR), false);

          // ...
          if (NULL != reallocation) {
            DWORD sublength;

            // ...
            renumerationAbsolutePathLength = ((length + 1u) / sizeof(TCHAR)) - 1u;
            renumerationAbsolutePath       = ::new (renumerationAbsolutePath) WCHAR[renumerationAbsolutePathLength + 1u];
            sublength                      = ::GetFullPathNameW(static_cast<LPCWSTR>(renumerationDirectory), static_cast<DWORD>((renumerationAbsolutePathLength + 1u) * sizeof(WCHAR)), renumerationAbsolutePath, static_cast<LPWSTR*>(NULL));

            // ...
            if (0u != sublength and length >= sublength) {
              reallocation = allocator.reallocate((renumerationAbsolutePathLength + 1u) * sizeof(WCHAR), sizeof(WCHAR), (sublength + 1u) * sizeof(WCHAR), sizeof(WCHAR), false);

              if (NULL != reallocation) {
                renumerationAbsolutePath       = renumerationDirectory;
                renumerationAbsolutePathLength = renumerationDirectoryLength;
                renumerationDirectory          = reinterpret_cast<LPWSTR>(reallocation); // --> std::launder(reinterpret_cast<LPWSTR>(reallocation));
                renumerationDirectoryLength    = sublength;
              }
            }
          }
        }
      }

      // ... ->> Enumerate files in `renumerationDirectory`
      filesystemRedirectionDisabled                      = FALSE != ::Wow64DisableWow64FsRedirection(&filesystemRedirection);
      renumerationDirectoryFindHandle                    = ::FindFirstFileExW(renumerationDirectory, ::FindExInfoBasic, static_cast<LPVOID>(&renumerationDirectoryFindData), ::FindExSearchNameMatch, static_cast<LPVOID>(NULL), 0x00u);
      renumerationDirectoryLength                       -= subdirectorySuffixLength; // ->> Memory leak
      renumerationDirectory[renumerationDirectoryLength] = L'\0'; // ->> Truncate appended `subdirectorySuffix`

      // ...
      if (INVALID_HANDLE_VALUE == renumerationDirectoryFindHandle) {
        executableExitCode = EXIT_FAILURE;

        switch (::GetLastError()) {
          case ERROR_DIRECTORY: {
            if (false == quietRequested)
            (void) std::fprintf(stderr, "%1.17s%ls%1.36s", "renum: Expected \"", renumerationDirectory, "\" to be a valid (absolute) directory");
          } break;

          case ERROR_NOT_SUPPORTED: {
            if (false == quietRequested)
            (void) std::fprintf(stderr, renumerationDirectoryAllocated ? "%1.32s%1.21s%ls%c" : "%1.32s%1.25s", "renum: Unable to filter through ", false == renumerationDirectoryAllocated ? "current working directory" : "specified directory \"", renumerationDirectory, '"');
          } break;

          case ERROR_PATH_NOT_FOUND: {
            if (false == quietRequested)
            (void) std::fprintf(stderr, "%1.43s%ls%c", "renum: Could not find specified directory \"", renumerationDirectory, '"');
          } break;

          default:
            goto filePathsNonEnumerable;
        }
      }

      else {
        std::size_t filePathsBufferLength = 0u;

        // ...
        if (false == quietRequested) {
          (void) std::fprintf(stdout, "%1.29s%ls%1.6s", "renum: Enumerating files in \"", renumerationDirectory, "\"..." "\r\n");
          (void) std::fflush(stdout);
        }

        while (true) {
          // ... ->> Stop enumerating
          if (FALSE == ::FindNextFileW(renumerationDirectoryFindHandle, &renumerationDirectoryFindData)) {
            if (ERROR_NO_MORE_FILES == ::GetLastError()) {
              filePaths = reinterpret_cast<wchar_t**>(allocator.allocate(filePathsLength * sizeof(wchar_t*), sizeof(wchar_t*)));

              // ...
              if (NULL == filePaths) allocatorExhausted = true;
              else { filePaths = ::new (filePaths) wchar_t*[filePathsLength]; break; }
            }

            goto filePathsNonEnumerable;
          }

          // ... ->> Enumerate acceptable file
          if (FILE_ATTRIBUTE_NORMAL == renumerationDirectoryFindData.dwFileAttributes or false == (renumerationDirectoryFindData.dwFileAttributes & (
            FILE_ATTRIBUTE_DIRECTORY |
            FILE_ATTRIBUTE_TEMPORARY |
            (enumerateHiddenFiles ? 0x00u : FILE_ATTRIBUTE_HIDDEN) |
            (enumerateSystemFiles ? 0x00u : FILE_ATTRIBUTE_SYSTEM)
          ))) {
            std::size_t fileNameLength       = MAX_PATH;                                                                 // ->> With NUL terminator
            wchar_t    *filePathBuffer       = NULL != filePathsBuffer ? filePathsBuffer + filePathsBufferLength : NULL; //
            std::size_t filePathBufferLength = renumerationDirectoryLength + 1u;                                         // ->> With NUL terminator

            // ...
            ++filePathsLength;

            // ... --> fileNameLength = ...; filePathsBufferLength = ...;
            if (SUCCEEDED(::StringCchLengthW(renumerationDirectoryFindData.cFileName, MAX_PATH, &fileNameLength)))
              ++fileNameLength;

            else { // --> ::wcsnlen(renumerationDirectoryFindData.cFileName, MAX_PATH) + 1u;
              fileNameLength = 0u;

              for (WCHAR *fileName = renumerationDirectoryFindData.cFileName; MAX_PATH != fileNameLength; ++fileName) {
                ++fileNameLength;
                if (L'\0' == *fileName) break;
              }
            }

            filePathBufferLength  += fileNameLength       - 1u;
            filePathsBufferLength += filePathBufferLength + 1u; // ->> With NUL delimiter

            // ... --> filePathsBuffer = ...; filePathsBufferSize = ...;
            if (NULL == filePathsBuffer) {
              filePathsBufferSize = filePathsBufferLength;
              filePathsBuffer     = reinterpret_cast<wchar_t*>(allocator.allocate(filePathsBufferSize * sizeof(wchar_t), sizeof(wchar_t)));

              if (NULL == filePathsBuffer) {
                allocatorExhausted = true;
                goto filePathsNonEnumerable;
              }

              filePathsBuffer = ::new (filePathsBuffer) wchar_t[filePathsBufferSize];
              filePathBuffer  = filePathsBuffer;
            }

            else if (filePathsBufferLength > filePathsBufferSize) {
              unsigned char *reallocation = NULL;
              std::size_t    size         = 1u;

              // ...
              for (; size < filePathsBufferLength; size *= 2u)
              if (size >= SIZE_MAX / 2u) {
                allocatorExhausted = true;
                goto filePathsNonEnumerable;
              }

              while (true) {
                reallocation = allocator.reallocate(filePathsBufferSize * sizeof(wchar_t), sizeof(wchar_t), size * sizeof(wchar_t), sizeof(wchar_t));
                if (NULL != reallocation) break;

                // ...
                std::size_t const delta = (size - filePathsBufferLength) / 2u;

                // ...
                if (allocator.invalidated or 0u == delta) {
                  allocatorExhausted = true;
                  goto filePathsNonEnumerable;
                }

                size -= delta;
              }

              filePathsBuffer = reinterpret_cast<wchar_t*>(reallocation); // --> std::launder(reinterpret_cast<wchar_t*>(reallocation) + ...);

              for (wchar_t *buffer = filePathsBuffer + filePathsBufferSize; filePathsBufferSize != size; ++buffer, ++filePathsBufferSize)
              (void) ::new (buffer) wchar_t;
            }

            // ... --> filePathBuffer = ...;
            filePathBuffer                                   = FAILED(::StringCchCopyNExW(static_cast<STRSAFE_LPWSTR>(filePathBuffer), filePathBufferLength, renumerationDirectory, renumerationDirectoryLength, static_cast<LPTSTR*>(NULL), static_cast<std::size_t*>(NULL), STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE)) ? std::wcsncpy(filePathBuffer, renumerationDirectory, renumerationDirectoryLength) : filePathBuffer;
            filePathBuffer[renumerationDirectoryLength + 0u] = L'\\';
            filePathBuffer[renumerationDirectoryLength + 1u] = L'\0';

            if (FAILED(::StringCchCatNExW(filePathBuffer + renumerationDirectoryLength + 1, fileNameLength, renumerationDirectoryFindData.cFileName, fileNameLength - 1u, static_cast<LPTSTR*>(NULL), static_cast<std::size_t*>(NULL), STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE)))
              (void) std::wcsncat(filePathBuffer + renumerationDirectoryLength + 1, renumerationDirectoryFindData.cFileName, fileNameLength - 1u);

            filePathBuffer[(renumerationDirectoryLength + fileNameLength) + 0u] = L'\0'; // ->> Terminate `filePathBuffer`  with NUL
            filePathBuffer[(renumerationDirectoryLength + fileNameLength) + 1u] = L'\0'; // ->> Delimit   `filePathsBuffer` with NUL
          }
        }
      }

      // ...
      if (false) filePathsNonEnumerable: {
        executableExitCode = EXIT_FAILURE;

        if (false == quietRequested) {
          (void) (renumerationDirectoryAllocated
            ? std::fprintf(stderr, "%1.51s%ls%c", "renum: Unable to enumerate through (all) files in \"", renumerationDirectory, '"')
            : std::fprintf(stderr, "%1.79s",      "renum: Unable to enumerate through (all) files in the current working directory")
          );

          if (allocatorExhausted)
            (void) std::fprintf(stderr, "%1.17s%s%1.6s", " (out of memory: ", allocator.size(), " used)");

          (void) std::fflush(stderr);
        }
      }

      // ... ->> Ideally successfully evaluates not to `FALSE` (for both function calls)
      if (filesystemRedirectionDisabled)                           (void) ::Wow64RevertWow64FsRedirection(filesystemRedirection);
      if (INVALID_HANDLE_VALUE != renumerationDirectoryFindHandle) (void) ::FindClose(renumerationDirectoryFindHandle);
    #elif defined __APPLE__ or defined __unix__
      struct renum /* final */ {
        inline static int sort(void const* const filePathA, void const* const filePathB) /* noexcept */ {
          int const comparison = std::wcscoll(*static_cast<wchar_t* const*>(filePathA), *static_cast<wchar_t* const*>(filePathB));

          // ...
          if (comparison < 0) return -1;
          if (comparison > 0) return +1;

          return 0;
        }
      };

      /* ... */
      char        const defaultDirectory[]                 = "./";     //
      char             *executableFilePath                 = NULL;     //
      uint32_t          executableFilePathLength           = 0u;       // ->> With NUL terminator(s)
      char              renumerationAbsolutePath[PATH_MAX] = {};       //
      std::size_t const renumerationAbsolutePathLength     = PATH_MAX; //
      char             *renumerationDirectory              = NULL;     //
      std::size_t       renumerationDirectoryLength        = 0u;       //

      // ... ->> Inquire `executableFilePath`
      #if defined __APPLE__ or defined __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ or defined __MACH__
        executableFilePath = reinterpret_cast<char*>(allocator.allocate(PATH_MAX * sizeof(char), sizeof(char), false));

        if (NULL != executableFilePath) {
          executableFilePath[PATH_MAX] = '\0';
          executableFilePathLength     = PATH_MAX;

          if (::_NSGetExecutablePath(executableFilePath, &executableFilePathLength) == -1) {
            unsigned char *const reallocation = allocator.reallocate(PATH_MAX * sizeof(char), sizeof(char), executableFilePathLength * sizeof(char), sizeof(char), false);

            // ...
            if (NULL == reallocation) {
              (void) allocator.release(PATH_MAX * sizeof(char), sizeof(char)); // --> executableFilePath
              executableFilePath       = NULL;
              executableFilePathLength = 0u;
            }

            else {
              uint_fast32_t const length = executableFilePathLength;

              // ...
              executableFilePath = reinterpret_cast<char*>(reallocation);

              if (::_NSGetExecutablePath(executableFilePath, &executableFilePathLength) != -1)
                executableFilePathAllocated = true;

              else {
                (void) allocator.release(length * sizeof(char), sizeof(char)); // --> executableFilePath
                executableFilePath       = NULL;
                executableFilePathLength = 0u;
              }
            }
          }
        }
      #elif defined __SUNPRO_C or defined __SUNPRO_CC
        executableFilePath = ::getexecname(); // ->> Absolute executable path would have the current working directory prepended (see `::getcwd(…)`) or begin with `/`

        while ('\0' != *executableFilePath) {
          ++executableFilePath;
          ++executableFilePathLength;
        }

        executableFilePath -= executableFilePathLength;
      #endif

      #if defined __FreeBSD__ or defined __FreeBSD_version or defined __gnu_linux__ or defined linux or defined __linux or defined __linux__ or defined __SUNPRO_C or defined __SUNPRO_CC // --> RIP Open BSD
        #if _BSD_SOURCE or _POSIX_C_SOURCE >= 200112L or _XOPEN_SOURCE >= 500 or (_XOPEN_SOURCE and _XOPEN_SOURCE_EXTENDED)
          if (NULL == executableFilePath)
          do {
            std::time_t const readlinkTimestamp      = std::time(static_cast<std::time_t*>(NULL));
            std::time_t const readlinkRetryTimestamp = static_cast<std::time_t>(3); // ->> Possibly 3 calendar seconds
            char const        directory[]            = "/proc/"
              #if defined __DragonFly__ or defined __FreeBSD__ or defined __FreeBSD_version
                "curproc/file"
              #elif defined __NETBSD__ or defined __NETBSD_version or defined NetBSD0_8 or defined NetBSD0_9 or defined NetBSD1_0
                "curproc/exe"
              #elif defined __SUNPRO_C or defined __SUNPRO_CC
                "self/path/a.out"
              #else
                "self/exe"
              #endif
            ;

            // ...
            executableFilePath = reinterpret_cast<char*>(allocator.allocate(PATH_MAX * sizeof(char), sizeof(char), false));

            if (NULL != executableFilePath) {
              executableFilePathLength = PATH_MAX;

              // ...
              #if defined __SUNPRO_C or defined __SUNPRO_CC
                // ... --> std::sprintf(executableFilePath, "/proc/%li/path/a.out", static_cast<long>(::getpid())) >= 0
                for (char *destination = executableFilePath, *source = const_cast<char*>("/proc/"); ; ++destination, ++source) {
                  if ('\0' == *source) {
                    pid_t const id = ::getpid();

                    // ...
                    for (pid_t value = id; ; value /= 10) {
                      ++destination;
                      if (value < 10) {
                        value = id;

                        for (char *subdestination = destination; ; value /= 10) {
                          *--subdestination = "0123456789"[value % 10];
                          if (value < 10) break;
                        }

                        break;
                      }
                    }

                    for (source = const_cast<char*>("/path/a.out"); ; ++destination, ++source) {
                      *destination = *source;
                      if ('\0' == *source) break;
                    }

                    break;
                  }

                  *destination = *source;
                }

                // ...
                if (::readlink(executableFilePath, executableFilePath, executableFilePathLength) != -1) {
                  executableFilePathAllocated = true;
                  break;
                }
              #endif

              while (NULL != executableFilePath) {
                ssize_t        length       = ::readlink(directory, executableFilePath, executableFilePathLength);
                unsigned char *reallocation = NULL;

                // ...
                if (length == -1)
                switch (errno) {
                  case EACCES: case EIO: case ENOMEM: {
                    std::time_t const timestamp = std::time(static_cast<std::time_t*>(NULL));

                    // ...
                    if (false == (
                      readlinkTimestamp == static_cast<std::time_t>(-1) ||
                      timestamp         == static_cast<std::time_t>(-1) ||
                      readlinkRetryTimestamp < timestamp - readlinkTimestamp
                    )) continue;
                  } // ->> leak into `default` case

                  default: {
                    (void) allocator.release(executableFilePathLength * sizeof(char), sizeof(char)); // --> executableFilePath
                    executableFilePath       = NULL;                                                 // --> break
                    executableFilePathLength = 0u;
                  } continue;
                }

                if (length < executableFilePathLength) {
                  executableFilePathAllocated = true;
                  break;
                }

                for (length = executableFilePathLength; executableFilePathLength <= SIZE_MAX - length and 0 != length; length /= 2) {
                  reallocation = allocator.reallocate(executableFilePathLength * sizeof(char), sizeof(char), (executableFilePathLength + length) * sizeof(char), sizeof(char), false);
                  if (NULL != reallocation) break;
                }

                if (NULL != reallocation) {
                  executableFilePath        = reinterpret_cast<char*>(reallocation);
                  executableFilePathLength += length;
                }

                else {
                  (void) allocator.release(executableFilePathLength * sizeof(char), sizeof(char)); // --> executableFilePath
                  executableFilePath       = NULL;                                                 // --> break
                  executableFilePathLength = 0u;
                }
              }
            }
          } while (false);
        #endif

        #if defined __FreeBSD__ or defined __FreeBSD_version
          if (NULL == executableFilePath) {
            int const managementInformationBaseName[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};

            // ... ->> Ideally `PATH_MAX` is large enough
            executableFilePath       = reinterpret_cast<char*>(allocator.allocate(PATH_MAX * sizeof(char), sizeof(char), false));
            executableFilePathLength = PATH_MAX;

            if (::sysctl(managementInformationBaseName, sizeof(managementInformationBaseName) / sizeof(int), executableFilePath, &executableFilePathLength, static_cast<void const*>(NULL), 0u) != -1) {
              executableFilePath[executableFilePathLength - 1u] = '\0';
              executableFilePathAllocated                       = true;
            }

            else {
              (void) allocator.release(PATH_MAX * sizeof(char), sizeof(char)); // --> executableFilePath
              executableFilePath       = NULL;
              executableFilePathLength = 0u;
            }
          }
        #endif
      #endif

      // ... ->> Canonize `executableFilePath` to its absolute file path
      if (NULL != executableFilePath and *executableFilePath != '/') {
        char executableFileAbsolutePath[PATH_MAX];

        // ...
        if (executableFileAbsolutePath == ::realpath(executableFilePath, executableFileAbsolutePath)) {
          if (false == executableFilePathAllocated or PATH_MAX + 1u > executableFilePathLength) {
            char *path;

            // ...
            path = false == executableFilePathAllocated                ? reinterpret_cast<char*>(allocator.allocate  (executableFilePathLength * sizeof(char), sizeof(char),                                        false)) : executableFilePath;
            path = NULL != path and PATH_MAX > executableFilePathLength ? reinterpret_cast<char*>(allocator.reallocate(executableFilePathLength * sizeof(char), sizeof(char), PATH_MAX * sizeof(char), sizeof(char), false)) : path;

            if (NULL != path) {
              for (char *destination = path, *source = executableFilePath; executableFilePathLength != static_cast<std::size_t>(source - executableFilePath); ++destination, ++source)
                *destination = *source;

              executableFilePath          = path;
              executableFilePathAllocated = true;
              executableFilePathLength    = PATH_MAX > executableFilePathLength ? PATH_MAX : executableFilePathLength;
            }
          }

          if (executableFilePathAllocated)
          for (char *destination = executableFilePath, *source = executableFileAbsolutePath; ; ++destination, ++source) {
            if (PATH_MAX == static_cast<std::size_t>(source - executableFileAbsolutePath)) {
              *destination = '\0';
              break;
            }

            *destination = *source;
            if ('\0' == *source) break;
          }
        }
      }

      // ... ->> Set `renumerationDirectory` to a user-specified command-line argument
      if (count > 1) {
        renumerationDirectory = arguments[1];

        for (char *directory = renumerationDirectory; ; ++directory)
        if ('\0' == *directory) {
          renumerationDirectoryLength = directory - renumerationDirectory;
          break;
        }
      }

      // ... ->> Set `renumerationDirectory` otherwise to the current working directory
      #if _BSD_SOURCE or _DEFAULT_SOURCE or (_XOPEN_SOURCE >= 500 and _POSIX_C_SOURCE < 200809L)
        if (NULL == renumerationDirectory) {
          renumerationDirectoryLength = PATH_MAX - 1u;
          renumerationDirectory       = reinterpret_cast<char*>(allocator.allocate((renumerationDirectoryLength + 1u) * sizeof(char), sizeof(char), false));

          // ...
          if (NULL != renumerationDirectory)
          while (NULL == ::getcwd(renumerationDirectory, renumerationDirectoryLength + 1u)) {
            if (ERANGE == errno) {
              std::size_t    length       = renumerationDirectoryLength + 1u;
              unsigned char *reallocation = NULL;

              // ...
              for (; renumerationDirectoryLength < SIZE_MAX - length and 0u != length; length /= 2u) {
                reallocation = allocator.reallocate((renumerationDirectoryLength + 1u) * sizeof(char), sizeof(char), (length + renumerationDirectoryLength + 1u) * sizeof(char), sizeof(char), false);
                if (NULL != reallocation) break;
              }

              if (NULL != reallocation) {
                renumerationDirectory          = reinterpret_cast<char*>(reallocation);
                renumerationDirectoryAllocated = true;
                renumerationDirectoryLength   += length;

                continue;
              }
            }

            (void) allocator.release((renumerationDirectoryLength + 1u) * sizeof(char), sizeof(char)); // --> renumerationDirectory
            renumerationDirectory          = NULL;
            renumerationDirectoryAllocated = false;
            renumerationDirectoryLength    = 0u;

            break;
          }
        }
      #endif

      // ... ->> Set `renumerationDirectory` otherwise to `defaultDirectory`
      if (NULL == renumerationDirectory) {
        renumerationDirectory       = const_cast<char*>(defaultDirectory);
        renumerationDirectoryLength = (sizeof(defaultDirectory) / sizeof(char)) - 1u;
      }

      // ... ->> Canonize `renumerationDirectory` to its absolute file path
      if (*renumerationDirectory != '/')
      if (renumerationAbsolutePath == ::realpath(renumerationDirectory, renumerationAbsolutePath)) {
        std::size_t length = 0u;
        char       *path   = NULL;

        if (false == renumerationDirectoryAllocated or PATH_MAX + 1u > renumerationDirectoryLength) {
          path = false == renumerationDirectoryAllocated                     ? reinterpret_cast<char*>(allocator.allocate  (renumerationDirectoryLength * sizeof(char), sizeof(char),                                               false)) : renumerationDirectory;
          path = NULL != path and PATH_MAX + 1u > renumerationDirectoryLength ? reinterpret_cast<char*>(allocator.reallocate(renumerationDirectoryLength * sizeof(char), sizeof(char), (PATH_MAX + 1u) * sizeof(char), sizeof(char), false)) : path;

          if (NULL != path) {
            for (char *destination = path, *source = renumerationDirectory; renumerationDirectoryLength != static_cast<std::size_t>(source - renumerationDirectory); ++destination, ++source)
              *destination = *source;

            renumerationDirectory          = path;
            renumerationDirectoryAllocated = true;
            renumerationDirectoryLength    = PATH_MAX + 1u > renumerationDirectoryLength ? PATH_MAX + 1u : renumerationDirectoryLength;
          }
        }

        if (renumerationDirectoryAllocated) {
          for (char *destination = renumerationDirectory, *source = renumerationAbsolutePath; ; ++destination, ++source) {
            if (PATH_MAX == static_cast<std::size_t>(source - renumerationAbsolutePath) or '\0' == *source) {
              *destination = '\0';
              length       = destination - renumerationDirectory;

              break;
            }

            *destination = *source;
          }

          path = reinterpret_cast<char*>(allocator.reallocate(renumerationDirectoryLength * sizeof(char), sizeof(char), length * sizeof(char), sizeof(char), false));
          if (NULL != path) {
            renumerationDirectory       = path;
            renumerationDirectoryLength = length;
          }
        }
      }

      // ... ->> Enumerate files in `renumerationDirectory`
      std::time_t const opendirRetryTimestamp = static_cast<std::time_t>(3); // ->> Possibly 3 calendar seconds
      std::time_t const opendirTimestamp      = std::time(static_cast<std::time_t*>(NULL));

      while (true) {
        DIR        *const renumerationDirectoryAddress = ::opendir(renumerationDirectory);
        std::time_t const timestamp                    = std::time(static_cast<std::time_t*>(NULL));

        // ...
        if (NULL != renumerationDirectoryAddress) {
          std::size_t filePathsBufferLength = 0u;
          struct stat fileStatus;

          // ...
          while (true) {
            std::size_t   fileNameLength;                                                                                          // ->> With NUL terminator
            char          filePath[NAME_MAX + PATH_MAX];                                                                           //
            wchar_t      *filePathBuffer             = NULL != filePathsBuffer ? filePathsBuffer + filePathsBufferLength : NULL;   //
            std::size_t   filePathBufferLength       = renumerationDirectoryLength + 1u;                                           // ->> With NUL terminator
            dirent *const renumerationDirectoryEntry = (static_cast<void>(errno = 0x00), ::readdir(renumerationDirectoryAddress)); //

            // ... ->> No more entries to read
            if (NULL == renumerationDirectoryEntry) {
              if (0x00 == errno) {
                filePaths = reinterpret_cast<wchar_t**>(allocator.allocate(filePathsLength * sizeof(wchar_t*), sizeof(wchar_t*)));

                // ...
                if (NULL == filePaths) allocatorExhausted = true;
                else { filePaths = ::new (filePaths) wchar_t*[filePathsLength]; break; }
              }

              goto filePathsNonEnumerable;
            }

            // ...
            ++filePathsLength;

            // ... --> fileNameLength = ...; filePathsBufferLength = ...;
            fileNameLength         = ::strlen(renumerationDirectoryEntry -> d_name) + 1u;
            filePathBufferLength  += fileNameLength                                 - 1u;
            filePathsBufferLength += filePathBufferLength                           + 1u; // ->> With NUL delimiter

            // ... --> filePathsBuffer = ...; filePathsBufferSize = ...;
            if (NULL == filePathsBuffer) {
              filePathsBufferSize = filePathsBufferLength;
              filePathsBuffer     = reinterpret_cast<wchar_t*>(allocator.allocate(filePathsBufferSize * sizeof(wchar_t), sizeof(wchar_t)));

              if (NULL == filePathsBuffer) {
                allocatorExhausted = true;
                goto filePathsNonEnumerable;
              }

              filePathsBuffer = ::new (filePathsBuffer) wchar_t[filePathsBufferSize];
              filePathBuffer  = filePathsBuffer;
            }

            else if (filePathsBufferLength > filePathsBufferSize) {
              unsigned char *reallocation = NULL;
              std::size_t    size         = 1u;

              // ...
              for (; size < filePathsBufferLength; size *= 2u)
              if (size >= SIZE_MAX / 2u) {
                allocatorExhausted = true;
                goto filePathsNonEnumerable;
              }

              while (true) {
                reallocation = allocator.reallocate(filePathsBufferSize * sizeof(wchar_t), sizeof(wchar_t), size * sizeof(wchar_t), sizeof(wchar_t));
                if (NULL != reallocation) break;

                // ...
                std::size_t const delta = (size - filePathsBufferLength) / 2u;

                // ...
                if (allocator.invalidated or 0u == delta) {
                  allocatorExhausted = true;
                  goto filePathsNonEnumerable;
                }

                size -= delta;
              }

              filePathsBuffer = reinterpret_cast<wchar_t*>(reallocation); // --> std::launder(reinterpret_cast<wchar_t*>(reallocation) + ...);

              for (wchar_t *buffer = filePathsBuffer + filePathsBufferSize; filePathsBufferSize != size; ++buffer, ++filePathsBufferSize)
              (void) ::new (buffer) wchar_t;
            }

            // ... --> filePathBuffer = ...;
            for (std::size_t index = 0u; index != renumerationDirectoryLength; ++index)
              filePathBuffer[index] = renumerationDirectory[index];

            filePathBuffer[renumerationDirectoryLength + 0u] = L'/';
            filePathBuffer[renumerationDirectoryLength + 1u] = L'\0';

            for (std::size_t index = 0u; index != fileNameLength; ++index)
              filePathBuffer[index + renumerationDirectoryLength + 1u] = renumerationDirectoryEntry -> d_name[index];

            filePathBuffer[(renumerationDirectoryLength + fileNameLength) + 0u] = L'\0'; // ->> Terminate `filePathBuffer`  with NUL
            filePathBuffer[(renumerationDirectoryLength + fileNameLength) + 1u] = L'\0'; // ->> Delimit   `filePathsBuffer` with NUL

            // ... --> filePath = std::wcsrtombs(…);
            for (std::size_t index = 0u; index != filePathBufferLength + 1u; ++index)
              filePath[index] = filePathBuffer[index];

            // ... ->> Enumerate acceptable file
            if (0 == ::lstat(filePath, &fileStatus)) {
              if (0 != ((S_ISLNK(fileStatus.st_mode)) | (S_ISREG(fileStatus.st_mode))))
              continue;
            }

            filePathsBufferLength -= filePathBufferLength + 1u;
            --filePathsLength;
          }

          if (false) filePathsNonEnumerable: {
            executableExitCode = EXIT_FAILURE;

            if (false == quietRequested) {
              (void) (renumerationDirectoryAllocated
                ? std::fprintf(stderr, "%1.51s%s%c", "renum: Unable to enumerate through (all) files in \"", renumerationDirectory, '"')
                : std::fprintf(stderr, "%1.79s",     "renum: Unable to enumerate through (all) files in the current working directory")
              );

              if (allocatorExhausted)
                (void) std::fprintf(stderr, "%1.17s%s%1.6s", " (out of memory: ", allocator.size(), " used)");

              (void) std::fflush(stderr);
            }
          }

          (void) ::closedir(renumerationDirectoryAddress);
        }

        else switch (errno) {
          case EACCES: case EMFILE: case ENFILE: case ENOMEM: {
            if (
              opendirTimestamp != static_cast<std::time_t>(-1) &&
              timestamp        != static_cast<std::time_t>(-1) &&
              opendirRetryTimestamp >= timestamp - opendirTimestamp
            ) continue;

            executableExitCode = EXIT_FAILURE;
            (void) (renumerationDirectoryAllocated
              ? std::fprintf(stderr, "%1.51s%1.*s%c", "renum: Unable to enumerate through (all) files in \"", static_cast<int>(renumerationDirectoryLength), renumerationDirectory, '"')
              : std::fprintf(stderr, "%1.79s",        "renum: Unable to enumerate through (all) files in the current working directory")
            );
          } break;

          case ENOENT: {
            executableExitCode = EXIT_FAILURE;

            if (false == quietRequested)
            (void) std::fprintf(stderr, "%1.43s%s%c", "renum: Could not find specified directory \"", renumerationDirectory, '"');
          } break;

          case ENOTDIR: {
            executableExitCode = EXIT_FAILURE;

            if (false == quietRequested)
            (void) std::fprintf(stderr, "%1.17s%s%1.36s", "renum: Expected \"", renumerationDirectory, "\" to be a valid (absolute) directory");
          } break;
        }

        break;
      }

      // ... ->> Alphabetically sort the directory entries (using `::malloc(…)`)
      // --> for (filePathsLength = ::scandir(renumerationDirectory, &dirent{}, reinterpret_cast<int (*)(dirent const*)>(NULL), &::alphasort); static_cast<int>(--filePathsLength) != -1; )
    #else
      struct renum /* final */ {
        inline static int sort(void const* const, void const* const) /* noexcept = delete */;
      };

      /* ... */
      wchar_t const *const executableFilePath             = NULL; // ->> With NUL terminator(s)
      std::size_t    const executableFilePathLength       = 0u;   //
      wchar_t const *const renumerationDirectory          = NULL; //
      std::size_t    const renumerationDirectoryLength    = 0u;   //
      wchar_t const *const renumerationAbsolutePath       = NULL; //
      std::size_t          renumerationAbsolutePathLength = 0u;   //

      // ...
      if (false == quietRequested)
      (void) std::fputs("renum: Unable to enumerate files", stderr);

      // ...
      return EXIT_FAILURE;
    #endif

    // ... ->> Ensure `renameFormat` uses valid character sequences
    if (NULL == renameFormat) {
      executableExitCode = EXIT_FAILURE;
      (void) (renameFormatIndex != static_cast<std::size_t>(-1)
        ? std::fprintf(stderr, "%1.31s%s%1.11s", "renum: Unusable rename format \"", arguments[renameFormatIndex - 2u], "\" specified")
        : std::fputs("renum: Unusable rename format specified", stderr)
      );
    }

    else for (wchar_t const *format = renameFormat, *renumerationSequence = NULL; ; ++format) {
      if (L'\0' == *format) {
        if (NULL == renumerationSequence) {
          executableExitCode = EXIT_FAILURE;

          if (false == quietRequested)
          (void) std::fprintf(stderr, "%1.22s%1.*ls%1.55s", "renum: Rename format \"", static_cast<int>(renameFormatLength), renameFormat, "\" must include either `?` or `??` character sequences" "\r\n");
        }

        break;
      }

      if (*(format + 0) != L'?') continue;
      if (*(format + 1) != L'?') { renumerationSequence = format;   continue; }
      if (*(format + 2) != L'?') { renumerationSequence = format++; continue; }

      format += 2;
    }

    // ... ->> Ensure `renameIndex` does not overflow past its maximum
    if (renameIndex > SIZE_MAX - filePathsLength) {
      executableExitCode = EXIT_FAILURE;

      if (false == quietRequested)
      (void) std::fprintf(stderr, "%1.19s%lu%1.68s%lu%1.34s%lu", "renum: File index (", static_cast<unsigned long>(renameIndex), ") would erroneously overflow given the number of encountered files (", static_cast<unsigned long>(filePathsLength), "); exceeding the maximum index of ", static_cast<unsigned long>(SIZE_MAX));
    }

    // ...
    if (EXIT_FAILURE != executableExitCode) {
      // ... ->> Ensure there are files to enumerate or rename
      if (0u == filePathsLength) {
        if (false == quietRequested)
        (void) std::fputs("\r\n" "renum: No files encountered.", stdout);
      }

      else {
        // ... ->> Conditionally ascertain if the files enumerated from `renumerationDirectory` are intended to be renamed
        if (false == renumerationDirectoryRecognized) {
          (void) (typeid(*renumerationDirectory) == typeid(wchar_t)
            ? std::fprintf(stdout, "%1.28s%ls%1.17s", "       " "Rename the files in \"", reinterpret_cast<wchar_t*>(renumerationDirectory), "\"? [Y]es / [N]o:" " ")
            : std::fprintf(stdout, "%1.28s%s%1.17s",  "       " "Rename the files in \"", reinterpret_cast<char*>   (renumerationDirectory), "\"? [Y]es / [N]o:" " ")
          ), std::fflush(stdout);
        }

        for (unsigned char reprompted = 0u; ; ++reprompted) {
          if (false == renumerationDirectoryRecognized) {
            // ... ->> Re-ascertain ...
            if (0u != reprompted) {
              // ... ->> Stop trying to ascertain ...
              if (reprompted > 10u) {
                (void) std::fputs("renum: Prompt received unrecognized replies. Terminating...", stdout);
                (void) std::fflush(stdout);

                break;
              }

              (void) std::fputs("       " "  please reply 'Y' (for \"Yes\") or 'N' (for \"No\") to rename files or not:" " ", stdout);
              (void) std::fflush(stdout);
            }

            // ... ->> Request prompt
            if (false == promptBuffersNewline)
              promptToken = std::fgetc(stdin);

            else for (bool promptReplied = false; ; promptReplied = true) {
              promptToken = std::fgetc(stdin);

              if (0u == reprompted or promptReplied) break;
              if ('\n' == static_cast<char>(promptToken)) continue;
            }
          }

          // ...
          if ((false == renumerationDirectoryRecognized and EOF == promptToken) or static_cast<char>(promptToken) == 'N' or static_cast<char>(promptToken) == 'n') {
            executableExitCode = EXIT_FAILURE;
            break;
          }

          // ... ->> Prepare to rename all files
          if (renumerationDirectoryRecognized or static_cast<char>(promptToken) == 'Y' or static_cast<char>(promptToken) == 'y' or static_cast<char>(promptToken) == '\n') {
            std::size_t fileExtensionMaximumLength = 0u;
            std::size_t fileNameMaximumLength      = 0u;
            bool        renamable                  = true;
            std::size_t renameIndexLength          = 0u;
            long double renameIndexThreshold       = static_cast<long double>(filePathsLength) / 10.0L;
            bool        renameLogged               = false;
            wchar_t    *renamePath                 = NULL;
            std::size_t renamePathLength           = 1u;
            std::time_t renameTimestamp            = std::time(static_cast<std::time_t*>(NULL));

            // ... --> fileNameMaximumLength = ...; filePaths[] = ...;
            for (wchar_t *buffer = filePathsBuffer, **paths = filePaths; L'\0' != *buffer; ++buffer, ++paths) {
              std::size_t    fileExtensionLength = 0u;
              std::size_t    fileNameLength      = 0u;
              wchar_t *const filePath            = buffer;

              // ... ->> Assume `filePath` is formatted as `renumerationDirectory\fileName.[fileExtension]`
              buffer += std::wcslen(buffer);
              *paths  = filePath;

              // ... --> fileExtensionLength = ...; fileNameLength = ...;
              for (wchar_t *subbuffer = buffer - 1; ; --subbuffer)
              if (filePath == subbuffer or *subbuffer == L'\\') {
                for (subbuffer += filePath != subbuffer; buffer != subbuffer; ++subbuffer) {
                  if (*subbuffer == L'.') {
                    while (buffer != ++subbuffer)
                    ++fileExtensionLength;

                    break;
                  }

                  ++fileNameLength;
                }

                break;
              }

              // ... --> fileExtensionMaximumLength = ...; fileNameMaximumLength = ...;
              if (fileExtensionMaximumLength < fileExtensionLength) fileExtensionMaximumLength = fileExtensionLength;
              if (fileNameMaximumLength      < fileNameLength)      fileNameMaximumLength      = fileNameLength;
            }

            // ... --> renameIndexLength = ...; renamePathLength = ...;
            for (std::size_t length = filePathsLength + renameIndex; 0u != length; length /= 10u)
              ++renameIndexLength;

            for (wchar_t const *format = renameFormat; L'\0' != *format; ++format) {
              if      (*(format + 0) != L'?') { renamePathLength += 1u;                                 }
              else if (*(format + 1) != L'?') { renamePathLength += renameIndexLength;                  }
              else if (*(format + 2) != L'?') { renamePathLength += renameIndexLength;     format += 1; }
              else                            { renamePathLength += fileNameMaximumLength; format += 2; }
            }

            renamePathLength += fileExtensionMaximumLength + renumerationDirectoryLength + 1u;
            renamePath        = reinterpret_cast<wchar_t*>(allocator.allocate((renamePathLength + 1u) * sizeof(wchar_t), sizeof(wchar_t)));

            if (NULL == renamePath) {
              allocatorExhausted = true;
              executableExitCode = EXIT_FAILURE;

              if (false == quietRequested)
              (void) std::fprintf(stderr, "%1.46s%s%1.6s", "renum: Unable to rename files (out of memory: ", allocator.size(), " used)");

              break;
            }

            renamePath = ::new (renamePath) wchar_t[renamePathLength + 1u];

            // ... ->> Lexicographically sort which enumerated files get renamed in order
            if (false == quietRequested) {
              (void) std::fputs("renum: Indexing files..." "\r\n", stdout);
              (void) std::fflush(stdout);
            }

            std::qsort(filePaths, filePathsLength, sizeof(wchar_t*), &renum::sort);

            // ... ->> Ensure that all enumerated files are renamable
            for (wchar_t **paths = filePaths; filePathsLength != static_cast<std::size_t>(paths - filePaths); ++paths) {
              #if defined _WIN32
                // --> break
              #else
                std::size_t renamableFilePathLength = 0u;
                char const *renamableFilePath       = renumerator_convert::renamableof(*paths, &renamableFilePathLength);

                // ...
                if (NULL == renamableFilePath) {
                  if (false == quietRequested) {
                    (void) std::fprintf(stderr, "%1.30s%ls%1.3s", "renum: Unable to rename file \"", *paths, "\"" "\r\n");
                    (void) std::fflush(stderr);
                  }

                  renamable = false;
                }

                else if (0u != renamableFilePathLength)
                  (void) allocator.release(renamableFilePathLength, sizeof(char)); // --> renamableFilePath
              #endif
            }

            if (false == renamable) {
              executableExitCode = EXIT_FAILURE;
              (void) std::fputs("\r\n" "renum: Unable to rename all files.", stderr);

              break;
            }

            // ... ->> Produce faster error diagnostics ideally
            if (false == quietRequested) {
              std::size_t const stderrBufferSize = filePathsLength * (94u + fileExtensionMaximumLength + fileNameMaximumLength + renamePathLength + 1u);
              char       *const stderrBuffer     = reinterpret_cast<char*>(allocator.allocate(stderrBufferSize * sizeof(char), sizeof(char), false));

              // ...
              if (NULL != stderrBuffer) {
                if (0 != std::setvbuf(stderr, stderrBuffer, _IOFBF, BUFSIZ))
                (void) allocator.release(stderrBufferSize * sizeof(char), sizeof(char)); // --> stderrBuffer
              }
            }

            // ... ->> Rename all enumerated files; Errors can not be undone until the loop is completed
            if (false == quietRequested) {
              (void) std::fputs("renum: Renaming files..." "\r\n\n", stdout);
              (void) std::fflush(stdout);
            }

            for (wchar_t **paths = filePaths; filePathsLength != static_cast<std::size_t>(paths - filePaths); ++paths, ++renameIndex) {
              wchar_t const filePathDelimiter =
                #if defined _WIN32
                  L'\\'
                #else
                  L'/'
                #endif
              ;
              wchar_t    *const filePath      = *paths;
              std::size_t const filePathIndex = static_cast<std::size_t>(paths - filePaths) + 1u;
              std::time_t const logTimestamp  = static_cast<std::time_t>(3); // ->> Possibly 3 calendar seconds
              std::time_t const timestamp     = std::time(static_cast<std::time_t*>(NULL));

              // ... ->> Non-IO attempt to not rename executable
              if (NULL != executableFilePath) {
                bool match = false;

                // ...
                for (std::size_t index = 0u; index != executableFilePathLength; ++index) {
                  if (static_cast<std::wint_t>(executableFilePath[index]) != static_cast<std::wint_t>(filePath[index])) break;
                  if (L'\0' == filePath[index] and (index == executableFilePathLength or '\0' == executableFilePath[index])) { match = true; break; }
                }

                if (match) {
                  --renameIndex;
                  continue;
                }
              }

              // ... ->> Format `renamePath`
              for (std::size_t index = 0u; index != renumerationDirectoryLength; ++index)
                renamePath[index] = renumerationDirectory[index]; // --> std::wcsncpy(renamePath, renumerationDirectory, renumerationDirectoryLength);

              renamePath[renumerationDirectoryLength] = filePathDelimiter;
              renamable                               = true;

              for (wchar_t *format = const_cast<wchar_t*>(renameFormat), *path = renamePath + renumerationDirectoryLength + 1; ; ++format) {
                // ... ->> File extension
                if (L'\0' == *format) {
                  wchar_t *subpath = filePath;

                  // ...
                  while (L'\0' != *subpath)
                    ++subpath;

                  while (filePath != --subpath)
                  if (*subpath == L'.') {
                    while (L'\0' != *subpath)
                    *(path++) = *(subpath++);

                    break;
                  }

                  *path = L'\0';
                  break;
                }

                // ... ->> File path
                if (*format != L'?')
                  *(path++) = *format;

                else if (*(format + 1) != L'?') {
                  std::size_t indexLength = 0u;

                  // ...
                  if (0u == renameIndex) indexLength = 1u;
                  else for (std::size_t index = renameIndex; 0u != index; index /= 10u) ++indexLength;

                  for (std::size_t index = renameIndex, length = indexLength; 0u != length; index /= 10u)
                    path[--length] = L"0123456789"[index % 10u];

                  path += indexLength;
                }

                else if (*(format + 2) != L'?') {
                  for (std::size_t index = renameIndex, length = renameIndexLength; 0u != length; index /= 10u)
                    path[--length] = L"0123456789"[index % 10u];

                  ++format;
                  path += renameIndexLength;
                }

                else {
                  wchar_t *subpath = filePath;

                  // ...
                  while (L'\0' != *subpath)
                    ++subpath;

                  while (filePath != --subpath)
                  if (*subpath == filePathDelimiter) break;

                  for (subpath += filePath != subpath; ; ++subpath) {
                    if (L'\0' == *subpath or *subpath == L'.') break;
                    *(path++) = *subpath;
                  }

                  format += 2;
                }
              }

              // ... ->> Rename file
              #if defined _WIN32
                struct renumerator_rename /* final */ {
                  wchar_t const *filePath;
                  wchar_t const *renamePath;
                  std::size_t    filePathIndex;
                  std::size_t    filePathsLength;
                  std::time_t    logTimestamp;
                  std::time_t    renameTimestamp;

                  /* ... */
                  inline static DWORD CALLBACK onprogress(LARGE_INTEGER const fileSize, LARGE_INTEGER const transferSize, LARGE_INTEGER const, LARGE_INTEGER const, DWORD const, DWORD const, HANDLE const, HANDLE const, LPVOID const data) /* noexcept */ {
                    if (false == quietRequested) {
                      renumerator_rename &renameData = *static_cast<renumerator_rename*>(data);
                      std::time_t  const  timestamp  = std::time(static_cast<std::time_t*>(NULL));

                      // ...
                      if (timestamp != static_cast<std::time_t>(-1)) {
                        if (renameData.renameTimestamp == static_cast<std::time_t>(-1))
                          renameData.renameTimestamp = timestamp;

                        else if (renameData.logTimestamp < timestamp - renameData.renameTimestamp) {
                          std::size_t    ratio = 0u;
                          ULARGE_INTEGER size  = {0u, 0u};

                          // ...
                          if (fileSize.HighPart >= 0L and transferSize.HighPart >= 0L)
                          while (SIZE_MAX != ratio) {
                            unsigned char carry                  = 0u;
                            DWORD         sizeSegments        [] = {size.HighPart, size.LowPart};
                            DWORD const   transferSizeSegments[] = {static_cast<DWORD>(transferSize.HighPart), transferSize.LowPart};

                            // ... --> size += transferSize
                            for (unsigned char index = 2u; index--; )
                            for (unsigned char rank = 0u; rank != CHAR_BIT * sizeof(DWORD); ++rank) {
                              unsigned char const sizeDigit         = (sizeSegments        [index] >> rank) & 1u;
                              unsigned char const transferSizeDigit = (transferSizeSegments[index] >> rank) & 1u;

                              // ...
                              if (0u != carry or 0u != sizeDigit or 0u != transferSizeDigit) {
                                carry                = 0u != sizeDigit and 0u != transferSizeDigit;
                                sizeSegments[index] |= (0u == carry) << rank;
                              }
                            }

                            ++ratio;
                            size.HighPart = sizeSegments[0];
                            size.LowPart  = sizeSegments[1];

                            // ...
                            if (0u == size.HighPart and 0u == size.LowPart) {
                              ratio = 0u != transferSize.HighPart or 0u != transferSize.LowPart;
                              break;
                            }

                            if (static_cast<DWORD>(fileSize.HighPart) <= size.HighPart or fileSize.LowPart <= size.LowPart)
                            break;
                          }

                          renameData.renameTimestamp = timestamp;

                          // ...
                          (void) (fileSize.HighPart < 0L or transferSize.HighPart < 0L
                            ? std::fprintf(stdout, "%1.7s%s%1.9s" "%0.2Lf" "%1.3s%lu%1.3s%lu%1.8s%1.*ls%1.6s%1.*ls%c", "renum: ", renameData.filePathIndex == 1u ? "" : "  ", "Renaming ", (0u != ratio ? 1.00L / static_cast<long double>(ratio) : 0.00L) * 100.0L, "% (", static_cast<unsigned long>(renameData.filePathIndex), " / ", static_cast<unsigned long>(renameData.filePathsLength), ") done \"", static_cast<int>(std::wcslen(renameData.filePath) - 2u), renameData.filePath, "\" to \"", static_cast<int>(std::wcslen(renameData.renamePath) - 2u), renameData.renamePath, '"')
                            : std::fprintf(stdout, "%1.7s%s%1.9s" "%1.3s"  "%1.3s%lu%1.3s%lu%1.8s%1.*ls%1.6s%1.*ls%c", "renum: ", renameData.filePathIndex == 1u ? "" : "  ", "Renaming ", "???",                                                                    "% (", static_cast<unsigned long>(renameData.filePathIndex), " / ", static_cast<unsigned long>(renameData.filePathsLength), ") done \"", static_cast<int>(std::wcslen(renameData.filePath) - 2u), renameData.filePath, "\" to \"", static_cast<int>(std::wcslen(renameData.renamePath) - 2u), renameData.renamePath, '"')
                          ), std::fflush(stdout);
                        }
                      }
                    }

                    return PROGRESS_CONTINUE;
                  }
                };

                /* ... */
                std::size_t        filePathLength = 0u;
                renumerator_rename renameData     = {filePath, renamePath, filePathIndex, filePathsLength, renameTimestamp, logTimestamp};

                // .. ->> Does not rename files of equal path
                renamable = FALSE != ::MoveFileWithProgressW(renumerator_convert::renamableof(filePath, &filePathLength), renumerator_convert::renamableof(renamePath, &renamePathLength), static_cast<LPPROGRESS_ROUTINE>(&renumerator_rename::onprogress), static_cast<LPVOID>(&renameData), MOVEFILE_WRITE_THROUGH);
              #else
                std::size_t       renamableFilePathLength   = 0u;
                char const *const renamableFilePath         = renumerator_convert::renamableof(filePath, &renamableFilePathLength);
                std::size_t       renamableRenamePathLength = 0u;
                char const *const renamableRenamePath       = renumerator_convert::renamableof(renamePath, &renamableRenamePathLength);

                // ...
                renamable = NULL != renamableFilePath and NULL != renamableRenamePath;
                renamable = renamable and 0 == std::rename(renamableFilePath, renamableRenamePath);

                if (0u != renamableFilePathLength)   (void) allocator.release((renamableFilePathLength   + 1u) * sizeof(char), sizeof(char)); // --> renamableFilePath
                if (0u != renamableRenamePathLength) (void) allocator.release((renamableRenamePathLength + 1u) * sizeof(char), sizeof(char)); // --> renamableRenamePath
              #endif

              // ... ->> Log progress
              if (
                // ->> Error
                false == renamable ||

                // ->> Progress
                filePathIndex > renameIndexThreshold                    or // ->> Enough files renamed to be logged
                (renameLogged and filePathIndex == filePathsLength - 1u) or // ->> Last file renamed to be logged
                static_cast<std::size_t>(renameIndexThreshold) == 1u    or // ->> Likely to log first file, anyway
                (                                                          // ->> Enough time elapsed to log
                  renameTimestamp != static_cast<std::time_t>(-1) &&
                  timestamp       != static_cast<std::time_t>(-1) &&
                  logTimestamp < timestamp - renameTimestamp
                )
              ) {
                long double const renameCompletionRate = (static_cast<long double>(filePathIndex) / static_cast<long double>(filePathsLength)) * 100.0L;

                // ...
                if (false == quietRequested) (void) (renamable
                  ? std::fflush(stderr), std::fprintf(stdout, "%1.7s%0.2Lf%1.3s%lu%1.3s%lu%1.17s%ls%1.6s%ls%1.3s", "renum: ", renameCompletionRate, "% (", static_cast<unsigned long>(filePathIndex), " / ", static_cast<unsigned long>(filePathsLength), ") done. Renamed \"", filePath, "\" to \"", renamePath, "\"" "\r\n"), std::fflush(stdout)
                  : std::fprintf(stderr, "%1.7s%0.2Lf%1.3s%lu%1.3s%lu%1.26s%ls%1.6s%ls%1.3s", "renum: ", renameCompletionRate, "% (", static_cast<unsigned long>(filePathIndex), " / ", static_cast<unsigned long>(filePathsLength), ") done. Unable to rename \"", filePath, "\" to \"", renamePath, "\"" "\r\n")
                );

                renameIndexThreshold += static_cast<long double>(filePathsLength) / 10.0L;
                renameLogged          = true;
                renameTimestamp       = timestamp;
              }

              // ... ->> Update log timer
              if (renameTimestamp == static_cast<std::time_t>(-1) and timestamp != static_cast<std::time_t>(-1))
              renameTimestamp = timestamp;
            }

            // ...
            if (false == quietRequested) (void) std::fflush(stderr), std::fputs("\r\n" "renum: Files renamed.", stdout);
            if (NULL  != renamePath)     (void) allocator.release((renamePathLength + 1u) * sizeof(char), sizeof(char)); // --> renamePath

            break;
          }
        }
      }
    }

    // ... ->> Release allocated memory
    if (NULL != filePaths)                 (void) allocator.release(filePathsLength                       * sizeof(wchar_t*),                  sizeof(wchar_t*));                  // --> filePaths
    if (NULL != filePathsBuffer)           (void) allocator.release(filePathsBufferSize                   * sizeof(wchar_t),                   sizeof(wchar_t));                   // --> filePathsBuffer
    if (renumerationAbsolutePathAllocated) (void) allocator.release((renumerationAbsolutePathLength + 1u) * sizeof(*renumerationAbsolutePath), sizeof(*renumerationAbsolutePath)); // --> renumerationAbsolutePath
    if (renumerationDirectoryAllocated)    (void) allocator.release((renumerationDirectoryLength + 1u)    * sizeof(*renumerationDirectory),    sizeof(*renumerationDirectory));    // --> renumerationDirectory
    if (executableFilePathAllocated)       (void) allocator.release((executableFilePathLength + 0u)       * sizeof(*executableFilePath),       sizeof(*executableFilePath));       // --> executableFilePath
    if (renameFormatAllocated)             (void) allocator.release((renameFormatLength + 1u)             * sizeof(wchar_t),                   sizeof(wchar_t));                   // --> renameFormat

    // ...
    return executableExitCode;
  }
#endif

