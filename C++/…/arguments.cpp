#include <cerrno>   // --> EINTR, errno
#include <climits>  // --> INT_MAX
#include <cstddef>  // --> ::std::size_t
#include <cstdio>   // --> ::std::printf(…)
#include <cstdlib>  // --> EXIT_FAILURE, EXIT_SUCCESS
#include <cstring>  // --> ::std::memcpy(…), ::std::strlen(…)
#include <iso646.h> // --> and, not, or
#if defined _WIN32
# include <cstdlib>    // --> __argc, __argv, __wargv
# include <windows.h>  // --> ::GetCommandLineA/W(…), ::GetModuleFileNameA/W(…), ::LocalFree(…), ::MultiByteToWideChar(…), ::WideCharToMultiByte(…)
#   include <shellapi.h> // --> ::CommandLineToArgvW(…)
# if defined _MSC_VER
#   pragma comment(lib, "kernel32.lib")
#   pragma comment(lib, "shell32.lib")
# endif
#elif defined __APPLE__
# include <crt_externs.h> // --> ::_NSGetArgc(…), ::_NSGetArgv(…)
# include <mach-o/dyld.h> // --> ::_NSGetExecutablePath(…)
#elif defined __FreeBSD__
# include <sys/sysctl.h>    // --> CTL_KERN, KERN_PROC, KERN_PROC_ARGS, KERN_PROC_PATHNAME; ::sysctl(…)
# include <sys/syslimits.h> // --> ARG_MAX
# include <sys/types.h>     // --> …
#elif defined __ANDROID__ or defined __linux__ or defined __unix__
# include <fcntl.h>  // --> O_CLOEXEC, O_RDONLY; ::open(…)
# include <unistd.h> // --> ::close(…), ::read(…), ::readlink(…)
#endif

/* ... */
#if defined _WIN32
  static ::std::size_t const COMMAND_LINE_MAXIMUM = 32768u; // ->> CreateProcessW's command-line limit, including NUL.
#elif defined __FreeBSD__
  static ::std::size_t const COMMAND_LINE_ARGUMENT_MAXIMUM = ARG_MAX; // --> 0 == ::sysctl(int[] {CTL_KERN, KERN_ARGMAX}, 2zu, &int maximum, &length = sizeof(int), (void*) NULL, 0u) ? length == sizeof(int) and maximum > 0 : false
#endif

/* ... */
struct vector /* final */ {
  enum next /* : signed char */ {
    NO_ARGUMENT    =  0,
    NEXT_ARGUMENT  = +1,
    ERROR_ARGUMENT = -1
  };

  enum source /* : unsigned char */ {
    C,                // ->> Apple:         C runtime’s argument vector `argv`
    EMULATE_UCRT,     // ->> Windows:       Manual UCRT    command-line parsing
    EMULATE_WINSHELL, // ->> Windows:       Manual Shell32 command-line parsing
    FREEBSD,          // ->> FreeBSD:       `sysctl(KERN_PROC_ARGS)` NUL-separated snapshot
    POSIX,            // ->> Linux/Android: `/proc/self/cmdline`     NUL-separated stream
    UCRT,             // ->> Windows:       C runtime’s `__argv` or `__wargv`
    WINSHELL          // ->> Windows:       `::CommandLineToArgvW(::GetCommandLineW())`
  };

  /* ... */
  enum vector::source method;
  ::std::size_t index;
  #if defined _WIN32
    struct /* final */ { char **narrow; wchar_t **wide; } value;
    wchar_t const                                        *position;
    wchar_t                                               fallback[COMMAND_LINE_MAXIMUM]; // ->> Caller-owned UTF-16LE conversion/ executable path fallback
    int                                                   count;
  #elif defined __APPLE__
    char **value;
    int    count;
  #elif defined __FreeBSD__
    char         *value;
    ::std::size_t length;
    ::std::size_t offset;
  #elif defined __ANDROID__ or defined __linux__ or defined __unix__
    int           descriptor;
    unsigned char chunk[4096];
    ::std::size_t chunk_length;
    ::std::size_t chunk_offset;
  #endif

  /* ... */
  bool begin(enum vector::source const method, char storage[], ::std::size_t const capacity, wchar_t const commandLine[] = NULL, char const commandLineANSI[] = NULL) /* noexcept */ {
    this -> method = method;
    this -> index  = 0u;

    #if defined _WIN32
      (void) capacity;
      (void) storage;

      this -> count        = 0u;
      this -> position     = NULL;
      this -> value.narrow = NULL;
      this -> value.wide   = NULL;

      switch (method) {
        case vector::UCRT: /* ->> `main` initializes `__argv` and `wmain` initializes `__wargv` */ {
          if (__argc < 0)
          return false;

          this -> count        = __argc;
          this -> value.narrow = __argv;
          this -> value.wide   = __wargv;
        } return NULL != this -> value.narrow or NULL != this -> value.wide;

        case vector::WINSHELL:
        case vector::EMULATE_UCRT:
        case vector::EMULATE_WINSHELL: {
          this -> position = NULL == commandLineANSI and NULL == this -> position ? ::GetCommandLineW() : commandLine;

          if (NULL == this -> position) {
            char const * const narrow = NULL != commandLineANSI ? commandLineANSI : ::GetCommandLineA();
            if (NULL == narrow) return false;
            // ->> MultiByteToWideChar returns 0 on failure (including insufficient space).
            if (0 == ::MultiByteToWideChar(CP_ACP, 0u, narrow, -1, this -> fallback,
                                         static_cast<int>(COMMAND_LINE_MAXIMUM))) return false;
            this -> position = this -> fallback;
          }
          if (vector::WINSHELL == method) {
            this -> value.wide = ::CommandLineToArgvW(this -> position, &this -> count);
            return NULL != this -> value.wide;
          }
        } return true;

        default:
          return false;
      }
    #elif defined __APPLE__
      (void) storage;
      (void) capacity;
      (void) commandLine;
      (void) commandLineANSI;
      switch (method) {
        case vector::C:
          this -> value = *::_NSGetArgv();
          this -> count = *::_NSGetArgc();
          return 0 <= this -> count and NULL != this -> value;
        default: return false;
      }
    #elif defined __FreeBSD__
      (void) commandLine;
      (void) commandLineANSI;
      switch (method) {
        case vector::FREEBSD: {
          int mib[] = { CTL_KERN, KERN_PROC, KERN_PROC_ARGS, -1 };
          if (NULL == storage) return false;
          this -> value = storage;
          this -> length = capacity;
          this -> offset = 0u;
          return 0 == ::sysctl(mib, 4u, storage, &this -> length, NULL, 0u);
        }
        default: return false;
      }
    #elif defined __ANDROID__ or defined __linux__ or defined __unix__
      (void) storage;
      (void) capacity;
      (void) commandLine;
      (void) commandLineANSI;
      switch (method) {
        case vector::POSIX:
          this -> chunk_length = 0u;
          this -> chunk_offset = 0u;
          #if defined O_CLOEXEC
            this -> descriptor = ::open("/proc/self/cmdline", O_RDONLY | O_CLOEXEC);
          #else
            this -> descriptor = ::open("/proc/self/cmdline", O_RDONLY);
          #endif
          return 0 <= this -> descriptor;
        default: return false;
      }
    #endif
  }

  void end(void) /* noexcept */ {
    switch (this -> method) {
  #if defined _WIN32
      case vector::WINSHELL:
        if (NULL != this -> value.wide) (void) ::LocalFree(this -> value.wide);
        this -> value.wide = NULL;
        break;
  #elif defined __ANDROID__ or defined __linux__ or defined __unix__
      case vector::POSIX:
        if (0 <= this -> descriptor) (void) ::close(this -> descriptor);
        this -> descriptor = -1;
        break;
  #endif
      default: break;
    }
  }

  // ->> vector::NEXT_ARGUMENT for an argument, vector::NO_ARGUMENT on exhaustion, vector::ERROR_ARGUMENT on failure.
  // ->> output == NULL skips data without copying (used by count()).
  // ->> Wide Windows sources yield UTF-8; __argv yields the CRT's narrow encoding.
  enum vector::next next(char output[], ::std::size_t capacity) /* noexcept */ {
    ::std::size_t length = 0u;

    switch (this -> method) {
  #if defined _WIN32
      case vector::UCRT:
      case vector::WINSHELL: {
        if (static_cast< ::std::size_t >(this -> count) <= this -> index) return vector::NO_ARGUMENT;
        if (NULL != this -> value.wide) {
          if (NULL == this -> value.wide[this -> index]) return vector::ERROR_ARGUMENT;
          if (NULL != output) {
            if (0u == capacity or static_cast< ::std::size_t >(INT_MAX) < capacity) return vector::ERROR_ARGUMENT;
            if (0 == ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                                          this -> value.wide[this -> index], -1,
                                          output, static_cast<int>(capacity), NULL, NULL)) return vector::ERROR_ARGUMENT;
          }
        } else {
          if (NULL == this -> value.narrow or NULL == this -> value.narrow[this -> index]) return vector::ERROR_ARGUMENT;
          length = ::std::strlen(this -> value.narrow[this -> index]);
          if (NULL != output) {
            if (capacity <= length) return vector::ERROR_ARGUMENT;
            ::std::memcpy(output, this -> value.narrow[this -> index], length + 1u);
          }
        }
        ++this -> index;
        return vector::NEXT_ARGUMENT;
      }

      case vector::EMULATE_UCRT:
      case vector::EMULATE_WINSHELL: {
        wchar_t buffer[COMMAND_LINE_MAXIMUM];
        wchar_t const *position = this -> position;
        bool in_quotes = false;
        unsigned quotes = 0u;

        if (NULL == position) return vector::ERROR_ARGUMENT;
        if (0u == this -> index and L'\0' == *position) {
          if (vector::EMULATE_WINSHELL == this -> method) {
            ::DWORD const obtained = ::GetModuleFileNameW(NULL, buffer,
                                                           static_cast< ::DWORD >(COMMAND_LINE_MAXIMUM));
            if (0u == obtained or COMMAND_LINE_MAXIMUM <= obtained) {
              char fallback[COMMAND_LINE_MAXIMUM];
              ::DWORD const bytes = ::GetModuleFileNameA(NULL, fallback, sizeof fallback);
              if (0u == bytes or sizeof fallback <= bytes) return vector::ERROR_ARGUMENT;
              if (0 == ::MultiByteToWideChar(CP_ACP, 0u, fallback, -1, buffer,
                                            static_cast<int>(COMMAND_LINE_MAXIMUM))) return vector::ERROR_ARGUMENT;
            }
            if (NULL != output) {
              if (0u == capacity or static_cast< ::std::size_t >(INT_MAX) < capacity) return vector::ERROR_ARGUMENT;
              if (0 == ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, buffer, -1,
                                            output, static_cast<int>(capacity), NULL, NULL)) return vector::ERROR_ARGUMENT;
            }
            ++this -> index;
            return vector::NEXT_ARGUMENT;
          }
          // ->> vector::UCRT parses the returned executable path as a replacement command line.
          ::DWORD const obtained = ::GetModuleFileNameW(NULL, this -> fallback,
                                                         static_cast< ::DWORD >(COMMAND_LINE_MAXIMUM));
          if (0u == obtained or COMMAND_LINE_MAXIMUM <= obtained) {
            char fallback[COMMAND_LINE_MAXIMUM];
            ::DWORD const bytes = ::GetModuleFileNameA(NULL, fallback, sizeof fallback);
            if (0u == bytes or sizeof fallback <= bytes) return vector::ERROR_ARGUMENT;
            if (0 == ::MultiByteToWideChar(CP_ACP, 0u, fallback, -1, this -> fallback,
                                          static_cast<int>(COMMAND_LINE_MAXIMUM))) return vector::ERROR_ARGUMENT;
          }
          this -> position = this -> fallback;
          position = this -> position;
        }

        if (0u != this -> index) {
          while (L' ' == *position or L'\t' == *position) ++position;
          if (L'\0' == *position) return vector::NO_ARGUMENT;
        }

        if (vector::EMULATE_WINSHELL == this -> method) {
          if (0u == this -> index) {
            // ->> Shell32: argv[0] ends at the next quote or whitespace.
            if (L'"' == *position) {
              ++position;
              while (L'\0' != *position and L'"' != *position) {
                if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                buffer[length++] = *position++;
              }
              if (L'"' == *position) ++position;
            } else {
              while (L'\0' != *position and L' ' != *position and L'\t' != *position) {
                if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                buffer[length++] = *position++;
              }
            }
          } else {
            while (L'\0' != *position) {
              if (0u == quotes and (L' ' == *position or L'\t' == *position)) break;
              if (L'\\' == *position) {
                unsigned slashes = 0u;
                while (L'\\' == *position) { ++slashes; ++position; }
                if (L'"' == *position) {
                  for (unsigned index = 0u; index < slashes / 2u; ++index) {
                    if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                    buffer[length++] = L'\\';
                  }
                  if (0u == (slashes % 2u)) ++quotes;
                  else {
                    if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                    buffer[length++] = L'"';
                  }
                  ++position;
                } else {
                  for (unsigned index = 0u; index < slashes; ++index) {
                    if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                    buffer[length++] = L'\\';
                  }
                  continue;
                }
              } else if (L'"' == *position) {
                ++quotes;
                ++position;
              } else {
                if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                buffer[length++] = *position++;
                continue;
              }
              while (L'"' == *position) {
                if (3u == ++quotes) {
                  if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                  buffer[length++] = L'"';
                  quotes = 0u;
                }
                ++position;
              }
              if (2u == quotes) quotes = 0u;
            }
          }
        } else {
          // ->> The vector::UCRT's wide parser; no DBCS lead-byte treatment is needed.
          if (0u == this -> index) {
            while (L'\0' != *position) {
              if (L'"' == *position) {
                in_quotes = not in_quotes;
                ++position;
                continue;
              }
              if (not in_quotes and (L' ' == *position or L'\t' == *position)) break;
              if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
              buffer[length++] = *position++;
            }
          } else {
            while (L'\0' != *position) {
              unsigned slashes = 0u;
              bool copy_quote = true;
              while (L'\\' == *position) { ++slashes; ++position; }
              if (L'"' == *position) {
                if (0u == (slashes % 2u)) {
                  if (in_quotes and L'"' == position[1]) ++position;
                  else { in_quotes = not in_quotes; copy_quote = false; }
                }
                slashes /= 2u;
              }
              for (unsigned index = 0u; index < slashes; ++index) {
                if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                buffer[length++] = L'\\';
              }
              if (L'\0' == *position or (not in_quotes and (L' ' == *position or L'\t' == *position))) break;
              if (copy_quote) {
                if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                buffer[length++] = *position++;
              } else ++position;
            }
          }
        }

        buffer[length] = L'\0';
        if (NULL != output) {
          if (0u == capacity or static_cast< ::std::size_t >(INT_MAX) < capacity) return vector::ERROR_ARGUMENT;
          if (0 == ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, buffer, -1,
                                        output, static_cast<int>(capacity), NULL, NULL)) return vector::ERROR_ARGUMENT;
        }
        this -> position = position;
        ++this -> index;
        return vector::NEXT_ARGUMENT;
      }
  #endif
  #if defined __APPLE__
      case vector::C: {
        if (static_cast< ::std::size_t >(this -> count) <= this -> index) return vector::NO_ARGUMENT;
        if (NULL == this -> value[this -> index]) return vector::ERROR_ARGUMENT;
        length = ::std::strlen(this -> value[this -> index]);
        if (NULL != output) {
          if (capacity <= length) return vector::ERROR_ARGUMENT;
          ::std::memcpy(output, this -> value[this -> index], length + 1u);
        }
        ++this -> index;
        return vector::NEXT_ARGUMENT;
      }
  #endif
  #if defined __FreeBSD__
      case vector::FREEBSD: {
        ::std::size_t const start = this -> offset;
        if (this -> length == start) return vector::NO_ARGUMENT;
        while (this -> offset < this -> length and '\0' != this -> value[this -> offset])
          ++this -> offset;
        if (this -> length == this -> offset) return vector::ERROR_ARGUMENT;
        length = this -> offset - start;
        if (NULL != output) {
          if (capacity <= length) {
            this -> offset = start;
            return vector::ERROR_ARGUMENT;
          }
          ::std::memcpy(output, this -> value + start, length + 1u);
        }
        ++this -> offset;
        ++this -> index;
        return vector::NEXT_ARGUMENT;
      }
  #endif
  #if (defined __ANDROID__ or defined __linux__ or defined __unix__) and not defined _WIN32 and not defined __APPLE__ and not defined __FreeBSD__
      case vector::POSIX: {
        while (true) {
          if (this -> chunk_length == this -> chunk_offset) {
            ::ssize_t received = 0;
            do {
              received = ::read(this -> descriptor, this -> chunk, sizeof this -> chunk);
            } while (0 > received and EINTR == errno);
            if (0 > received) return vector::ERROR_ARGUMENT;
            if (0 == received) return 0u == length ? vector::NO_ARGUMENT : vector::ERROR_ARGUMENT;
            this -> chunk_length = static_cast< ::std::size_t >(received);
            this -> chunk_offset = 0u;
          }
          unsigned char const character = this -> chunk[this -> chunk_offset++];
          if (0u == character) {
            if (NULL != output) {
              if (capacity <= length) return vector::ERROR_ARGUMENT;
              output[length] = '\0';
            }
            ++this -> index;
            return vector::NEXT_ARGUMENT;
          }
          if (NULL != output and 0u < capacity and length < capacity - 1u)
            output[length] = static_cast<char>(character);
          ++length;
        }
      }
  #endif
      default: return vector::ERROR_ARGUMENT;
    }
  }
};

extern "C" {
  static int count(enum vector::source method, char storage[], ::std::size_t storage_capacity,
                   wchar_t const commandLine[] = NULL, char const commandLineANSI[] = NULL) /* noexcept */ {
    struct vector arguments;
    int result = 0;
    enum vector::next status = vector::NO_ARGUMENT;
    if (not arguments.begin(method, storage, storage_capacity, commandLine, commandLineANSI)) return -1;
    while (vector::NEXT_ARGUMENT == (status = arguments.next(NULL, 0u))) {
      if (INT_MAX == result) { status = vector::ERROR_ARGUMENT; break; }
      ++result;
    }
    arguments.end();
    return vector::NO_ARGUMENT == status ? result : -1;
  }

  // ->> Retrieves the running executable's real module pathname, not argv[0].
  static bool executable(char output[], ::std::size_t capacity) /* noexcept */ {
    if (NULL == output or 0u == capacity) return false;
  #if defined _WIN32
    wchar_t path[COMMAND_LINE_MAXIMUM];
    ::DWORD const obtained = ::GetModuleFileNameW(NULL, path, static_cast< ::DWORD >(COMMAND_LINE_MAXIMUM));
    if (0u < obtained and COMMAND_LINE_MAXIMUM > obtained) {
      if (static_cast< ::std::size_t >(INT_MAX) < capacity) return false;
      return 0 != ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, path, -1,
                                       output, static_cast<int>(capacity), NULL, NULL);
    }
    // ->> Legacy ANSI fallback. Result is in the active Windows ANSI code page, not UTF-8.
    if (static_cast< ::std::size_t >(static_cast< ::DWORD >(-1)) < capacity) return false;
    ::DWORD const bytes = ::GetModuleFileNameA(NULL, output, static_cast< ::DWORD >(capacity));
    return 0u < bytes and bytes < capacity;
  #elif defined __APPLE__
    if (static_cast< ::std::size_t >(static_cast<unsigned>(-1)) < capacity) return false;
    unsigned length = static_cast<unsigned>(capacity);
    return 0 == ::_NSGetExecutablePath(output, &length);
  #elif defined __FreeBSD__
    int mib[] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
    ::std::size_t length = capacity;
    return 0 == ::sysctl(mib, 4u, output, &length, NULL, 0u) and
           0u < length and length <= capacity and '\0' == output[length - 1u];
  #elif defined __ANDROID__ or defined __linux__ or defined __unix__
    ::ssize_t const length = ::readlink("/proc/self/exe", output, capacity);
    if (0 >= length or capacity <= static_cast< ::std::size_t >(length)) return false;
    output[length] = '\0';
    return true;
  #endif
  }
}

/* Main */
int main(void) {
  #if defined _WIN32
    enum vector::source const method      = vector::EMULATE_UCRT; // ->> UTF-16LE format by default
    char               *const commandLine = NULL;
    ::std::size_t       const capacity    = 0u;
  #elif defined __APPLE__
    enum vector::source const method      = vector::C;
    char               *const commandLine = NULL;
    ::std::size_t       const capacity    = 0u;
  #elif defined __FreeBSD__
    enum vector::source const method                                     = vector::FREEBSD;
    static char               commandLine[COMMAND_LINE_ARGUMENT_MAXIMUM] = "";
    ::std::size_t const       capacity                                   = sizeof commandLine;
  #elif defined __ANDROID__ or defined __linux__ or defined __unix__
    enum vector::source const method      = vector::POSIX;
    char               *const commandLine = NULL;
    ::std::size_t       const capacity    = 0u;
  #endif
  enum vector::next status           = vector::ERROR_ARGUMENT;
  char              executable[4096] = ".";
  int const         count            = ::count(method, commandLine, capacity);
  struct vector     arguments        = {};

  // ...
  if (count < 0)                                       return EXIT_FAILURE;
  if (not ::executable(executable, sizeof executable)) return EXIT_FAILURE;

  (void) ::std::printf("executable" " = " "\"%s\"" "\r\n", executable);
  (void) ::std::printf("argc"       " = " "%i"     "\r\n", count);

  if (arguments.begin(method, commandLine, capacity)) {
    for (char argument[4096] = ""; vector::NEXT_ARGUMENT == (status = arguments.next(argument, sizeof argument)); ) {
      (void) ::std::printf("argv[%lu]" " = " "\"", static_cast<unsigned long>(arguments.index - 1u));
      (void) ::std::printf("%.4096s", argument);
      (void) ::std::printf("\"" "\r\n");
    }

    arguments.end();
  }

  return vector::NO_ARGUMENT == status ? EXIT_SUCCESS : EXIT_FAILURE;
}
