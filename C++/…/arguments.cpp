/* TODO */
#include <cerrno>    // --> EINTR, errno.
#include <climits>   // --> INT_MAX.
#include <cstddef>   // --> ::std::size_t.
#include <cstdio>    // --> ::std::printf(…).
#include <cstdlib>   // --> EXIT_FAILURE, EXIT_SUCCESS.
#include <cstring>   // --> ::std::memcpy(…), ::std::strlen(…).
#include <iso646.h>  // --> and, not, or.
#if defined _WIN32
  #include <shellapi.h> // --> ::CommandLineToArgvW(…).
  #include <windows.h>  // --> GetCommandLine*, GetModuleFileName*, LocalFree, character conversion.
  #if defined _MSC_VER
    #pragma comment(lib, "kernel32.lib")
    #pragma comment(lib, "shell32.lib")
  #endif
#elif defined __APPLE__
  #include <crt_externs.h> // --> ::_NSGetArgc(…), ::_NSGetArgv(…).
  #include <mach-o/dyld.h> // --> ::_NSGetExecutablePath(…).
  #include <wordexp.h>     // --> ::wordexp(…), ::wordfree(…), wordexp_t, WRDE_NOCMD, WRDE_UNDEF.
#elif defined __FreeBSD__
# include <sys/sysctl.h>    // --> CTL_KERN, KERN_PROC*, ::sysctl(…).
# include <sys/syslimits.h> // --> ARG_MAX.
# include <sys/types.h>     // --> FreeBSD system types.
# include <wordexp.h>       // --> ::wordexp(…), ::wordfree(…), wordexp_t, WRDE_NOCMD, WRDE_UNDEF.
#elif defined __ANDROID__ or defined __linux__ or defined __unix__
  #include <fcntl.h>  // --> O_CLOEXEC, O_RDONLY, ::open(…).
  #include <unistd.h> // --> ::close(…), ::read(…), ::readlink(…).
  #if defined __linux__ and not defined __ANDROID__
    #include <wordexp.h> // --> ::wordexp(…), ::wordfree(…), wordexp_t, WRDE_NOCMD, WRDE_UNDEF.
  #endif
#endif

#if defined _WIN32
  static ::std::size_t const COMMAND_LINE_MAXIMUM = 32768u; // ->> CreateProcessW UTF-16 code units including NUL.
#elif defined __FreeBSD__
  static ::std::size_t const COMMAND_LINE_ARGUMENT_MAXIMUM = ARG_MAX; // ->> FreeBSD compile-time exec argument limit.
#endif

enum {
  COMMAND_ARGUMENT_COUNT   = 8u,    // ->> Maximum argument count of the custom literal-word parser only.
  COMMAND_ARGUMENT_MAXIMUM = 4096u  // ->> Maximum bytes per custom-parsed word, including NUL.
};

struct word_list /* final */ {
  char          arguments[COMMAND_ARGUMENT_COUNT][COMMAND_ARGUMENT_MAXIMUM];
  char         *values[COMMAND_ARGUMENT_COUNT + 1u];
  ::std::size_t count;
};

extern "C" {
  // ->> Lexical POSIX-style quoting and escaping, bounded to eight arguments.
  // ->> No evaluation or expansion: $, *, ~, parentheses, and shell operators
  // ->> are ordinary text (unless quoted or escaped). Not a full wordexp() replacement.
  // ->> Output is caller-owned; return -1 for malformed quotes/escapes or overflow.
  static int shell_words(char const input[], struct word_list *const output) /* noexcept */ {
    enum quote { UNQUOTED, SINGLE_QUOTED, DOUBLE_QUOTED } state = UNQUOTED;
    ::std::size_t count  = 0u;
    ::std::size_t length = 0u;
    bool active = false;

    if (NULL == input or NULL == output) return -1;
    output -> count = 0u;
    output -> values[0] = NULL;

    while (true) {
      unsigned char const character = static_cast<unsigned char>(*input);

      if ('\0' == character) {
        if (UNQUOTED != state) return -1;
        if (active) {
          if (COMMAND_ARGUMENT_COUNT <= count) return -1;
          output -> arguments[count][length] = '\0';
          output -> values[count] = output -> arguments[count];
          ++count;
        }
        output -> values[count] = NULL;
        output -> count = count;
        return static_cast<int>(count);
      }

      if (UNQUOTED == state) {
        if (' ' == character or '\t' == character) {
          if (active) {
            if (COMMAND_ARGUMENT_COUNT <= count) return -1;
            output -> arguments[count][length] = '\0';
            output -> values[count] = output -> arguments[count];
            ++count;
            length = 0u;
            active = false;
          }
          ++input;
          continue;
        }
        if ('\'' == character or '"' == character) {
          state = '\'' == character ? SINGLE_QUOTED : DOUBLE_QUOTED;
          active = true;
          ++input;
          continue;
        }
        if ('\\' == character) {
          if ('\0' == input[1]) return -1;
          if ('\n' == input[1]) { input += 2; continue; }
          ++input;
        }
      } else if (SINGLE_QUOTED == state) {
        if ('\'' == character) { state = UNQUOTED; ++input; continue; }
      } else {
        if ('"' == character) { state = UNQUOTED; ++input; continue; }
        if ('\\' == character) {
          if ('\0' == input[1]) return -1;
          if ('\n' == input[1]) { input += 2; continue; }
          if ('$' == input[1] or '`' == input[1] or '"' == input[1] or '\\' == input[1]) ++input;
        }
      }

      if (COMMAND_ARGUMENT_COUNT <= count or COMMAND_ARGUMENT_MAXIMUM - 1u <= length) return -1;
      output -> arguments[count][length++] = *input++;
      active = true;
    }
  }
}

struct vector /* final */ {
  enum next /* : signed char */ {
    ERROR_ARGUMENT = -1,
    NEXT_ARGUMENT  = +1,
    NO_ARGUMENT    =  0
  };

  enum source /* : unsigned char */ {
    C,                // ->> Apple CRT argv.
    EMULATE_POSIX,    // ->> Bounded, literal POSIX-style word tokenizer; no expansion.
    EMULATE_UCRT,     // ->> Manual Windows UCRT parser (UTF-16).
    EMULATE_WINSHELL, // ->> Manual Shell32 parser (UTF-16).
    FREEBSD,          // ->> FreeBSD sysctl argument snapshot.
    POSIX,            // ->> Linux/Android /proc/self/cmdline.
    UCRT,             // ->> CRT __argv or __wargv.
    WINSHELL,         // ->> Windows CommandLineToArgvW().
    WORD_EXP          // ->> Native POSIX wordexp() (where available).
  };

  enum source method;
  ::std::size_t index;
  union {
    struct word_list manual;
    #if defined _WIN32
      struct {
        union { char **narrow; wchar_t **wide; } value;
        wchar_t const *text;
        wchar_t buffer[COMMAND_LINE_MAXIMUM];
        int count;
      } windows;
    #elif defined __APPLE__
      struct { char **value; int count; } apple;
    #elif defined __FreeBSD__
      struct {
        char value[COMMAND_LINE_ARGUMENT_MAXIMUM];
        ::std::size_t length;
        ::std::size_t offset;
      } freebsd;
    #elif defined __ANDROID__ or defined __linux__ or defined __unix__
      struct {
        int descriptor;
        unsigned char chunk[4096];
        ::std::size_t chunk_length;
        ::std::size_t chunk_offset;
      } proc;
    #endif
    #if defined __APPLE__ or defined __FreeBSD__ or (defined __linux__ and not defined __ANDROID__)
      ::wordexp_t expansion;
    #endif
  } state;

  // ->> Native process backends ignore supplied text; WORD_EXP and EMULATE_POSIX require it.
  bool begin(enum source const method, char const commandLine[] = NULL, wchar_t const commandLineUnicode[] = NULL) /* noexcept */ {
    this -> method = method;
    this -> index  = 0u;
    (void) commandLineUnicode;

    switch (method) {
      case vector::EMULATE_POSIX:
        return 0 <= ::shell_words(commandLine, &this -> state.manual);

      #if defined __APPLE__ or defined __FreeBSD__ or (defined __linux__ and not defined __ANDROID__)
        case vector::WORD_EXP: {
          int result = 0;
          if (NULL == commandLine) return false;
          ::std::memset(&this -> state.expansion, 0, sizeof this -> state.expansion);
          result = ::wordexp(commandLine, &this -> state.expansion, WRDE_NOCMD | WRDE_UNDEF);
          if (0 != result) {
            if (WRDE_NOSPACE == result) (void) ::wordfree(&this -> state.expansion);
            return false;
          }
          return true;
        }
      #endif

      #if defined _WIN32
        case vector::EMULATE_UCRT:
        case vector::EMULATE_WINSHELL:
        case vector::WINSHELL: {
          this -> state.windows.count = 0;
          this -> state.windows.text  = NULL;
          this -> state.windows.value.wide = NULL;

          if (NULL != commandLineUnicode) this -> state.windows.text = commandLineUnicode;
          else if (NULL != commandLine) {
            if (0 == ::MultiByteToWideChar(CP_ACP, 0u, commandLine, -1,
                                          this -> state.windows.buffer, static_cast<int>(COMMAND_LINE_MAXIMUM))) return false;
            this -> state.windows.text = this -> state.windows.buffer;
          } else this -> state.windows.text = ::GetCommandLineW();

          if (NULL == this -> state.windows.text) {
            char const *const narrow = ::GetCommandLineA();
            if (NULL == narrow or 0 == ::MultiByteToWideChar(CP_ACP, 0u, narrow, -1,
                                                            this -> state.windows.buffer, static_cast<int>(COMMAND_LINE_MAXIMUM))) return false;
            this -> state.windows.text = this -> state.windows.buffer;
          }

          if (vector::WINSHELL == method) {
            this -> state.windows.value.wide = ::CommandLineToArgvW(this -> state.windows.text, &this -> state.windows.count);
            return NULL != this -> state.windows.value.wide;
          }
          return true;
        }
        case vector::UCRT:
          if (0 > ::__argc) return false;
          this -> state.windows.count = ::__argc;
          if (NULL != ::__wargv) this -> state.windows.value.wide = ::__wargv;
          else if (NULL != ::__argv) this -> state.windows.value.narrow = ::__argv;
          else return false;
          return true;
      #elif defined __APPLE__
        case vector::C:
          this -> state.apple.count = *::_NSGetArgc();
          this -> state.apple.value = *::_NSGetArgv();
          return 0 <= this -> state.apple.count and NULL != this -> state.apple.value;
      #elif defined __FreeBSD__
        case vector::FREEBSD: {
          int const managementInformationBase[] = { CTL_KERN, KERN_PROC, KERN_PROC_ARGS, -1 };
          this -> state.freebsd.length = sizeof this -> state.freebsd.value;
          this -> state.freebsd.offset = 0u;
          return 0 == ::sysctl(managementInformationBase, 4u, this -> state.freebsd.value,
                               &this -> state.freebsd.length, NULL, 0u);
        }
      #elif defined __ANDROID__ or defined __linux__ or defined __unix__
        case vector::POSIX:
          this -> state.proc.chunk_length = 0u;
          this -> state.proc.chunk_offset = 0u;
          #if defined O_CLOEXEC
            this -> state.proc.descriptor = ::open("/proc/self/cmdline", O_RDONLY | O_CLOEXEC);
          #else
            this -> state.proc.descriptor = ::open("/proc/self/cmdline", O_RDONLY);
          #endif
          return 0 <= this -> state.proc.descriptor;
      #endif
      default: return false;
    }
  }

  void end(void) /* noexcept */ {
    switch (this -> method) {
      #if defined _WIN32
        case vector::WINSHELL:
          if (NULL != this -> state.windows.value.wide) (void) ::LocalFree(this -> state.windows.value.wide);
          this -> state.windows.value.wide = NULL;
          break;
      #endif
      #if defined __APPLE__ or defined __FreeBSD__ or (defined __linux__ and not defined __ANDROID__)
        case vector::WORD_EXP:
          (void) ::wordfree(&this -> state.expansion);
          break;
      #endif
      #if (defined __ANDROID__ or defined __linux__ or defined __unix__) and not defined __APPLE__ and not defined __FreeBSD__ and not defined _WIN32
        case vector::POSIX:
          if (0 <= this -> state.proc.descriptor) (void) ::close(this -> state.proc.descriptor);
          this -> state.proc.descriptor = -1;
          break;
      #endif
        default: break;
    }
  }

  // ->> The output buffer is UTF-8 for wide Windows input, otherwise source bytes.
  // ->> NULL output counts/skips an argument without copying.
  enum next next(char output[], ::std::size_t capacity) /* noexcept */ {
    ::std::size_t length = 0u;

    switch (this -> method) {
      case vector::EMULATE_POSIX: {
        if (this -> state.manual.count <= this -> index) return vector::NO_ARGUMENT;
        char const *const argument = this -> state.manual.values[this -> index];
        length = ::std::strlen(argument);
        if (NULL != output) {
          if (capacity <= length) return vector::ERROR_ARGUMENT;
          ::std::memcpy(output, argument, length + 1u);
        }
        ++this -> index;
        return vector::NEXT_ARGUMENT;
      }
      #if defined __APPLE__ or defined __FreeBSD__ or (defined __linux__ and not defined __ANDROID__)
        case vector::WORD_EXP: {
          if (this -> state.expansion.we_wordc <= this -> index) return vector::NO_ARGUMENT;
          char const *const argument = this -> state.expansion.we_wordv[this -> index];
          if (NULL == argument) return vector::ERROR_ARGUMENT;
          length = ::std::strlen(argument);
          if (NULL != output) {
            if (capacity <= length) return vector::ERROR_ARGUMENT;
            ::std::memcpy(output, argument, length + 1u);
          }
          ++this -> index;
          return vector::NEXT_ARGUMENT;
        }
      #endif
      #if defined _WIN32
        case vector::UCRT:
        case vector::WINSHELL: {
          if (static_cast< ::std::size_t >(this -> state.windows.count) <= this -> index) return vector::NO_ARGUMENT;
          if (vector::WINSHELL == this -> method or NULL != ::__wargv) {
            if (NULL == this -> state.windows.value.wide[this -> index]) return vector::ERROR_ARGUMENT;
            if (NULL != output) {
              if (0u == capacity or static_cast< ::std::size_t >(INT_MAX) < capacity) return vector::ERROR_ARGUMENT;
              if (0 == ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                                            this -> state.windows.value.wide[this -> index], -1,
                                            output, static_cast<int>(capacity), NULL, NULL)) return vector::ERROR_ARGUMENT;
            }
          } else {
            if (NULL == this -> state.windows.value.narrow[this -> index]) return vector::ERROR_ARGUMENT;
            length = ::std::strlen(this -> state.windows.value.narrow[this -> index]);
            if (NULL != output) {
              if (capacity <= length) return vector::ERROR_ARGUMENT;
              ::std::memcpy(output, this -> state.windows.value.narrow[this -> index], length + 1u);
            }
          }
          ++this -> index;
          return vector::NEXT_ARGUMENT;
        }
      case vector::EMULATE_UCRT:
      case vector::EMULATE_WINSHELL: {
        wchar_t buffer[COMMAND_LINE_MAXIMUM];
        wchar_t const *text = this -> state.windows.text;
        bool in_quotes = false;
        unsigned quotes = 0u;

        if (NULL == text) return vector::ERROR_ARGUMENT;
        if (0u == this -> index and L'\0' == *text) {
          if (vector::EMULATE_WINSHELL == this -> method) {
            ::DWORD const obtained = ::GetModuleFileNameW(NULL, buffer,
                                                           static_cast< ::DWORD >(COMMAND_LINE_MAXIMUM));
            if (0u == obtained or COMMAND_LINE_MAXIMUM <= obtained) {
              char narrow[COMMAND_LINE_MAXIMUM];
              ::DWORD const bytes = ::GetModuleFileNameA(NULL, narrow, sizeof narrow);
              if (0u == bytes or sizeof narrow <= bytes) return vector::ERROR_ARGUMENT;
              if (0 == ::MultiByteToWideChar(CP_ACP, 0u, narrow, -1, buffer,
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
          ::DWORD const obtained = ::GetModuleFileNameW(NULL, this -> state.windows.buffer,
                                                         static_cast< ::DWORD >(COMMAND_LINE_MAXIMUM));
          if (0u == obtained or COMMAND_LINE_MAXIMUM <= obtained) {
            char narrow[COMMAND_LINE_MAXIMUM];
            ::DWORD const bytes = ::GetModuleFileNameA(NULL, narrow, sizeof narrow);
            if (0u == bytes or sizeof narrow <= bytes) return vector::ERROR_ARGUMENT;
            if (0 == ::MultiByteToWideChar(CP_ACP, 0u, narrow, -1, this -> state.windows.buffer,
                                          static_cast<int>(COMMAND_LINE_MAXIMUM))) return vector::ERROR_ARGUMENT;
          }
          this -> state.windows.text = this -> state.windows.buffer;
          text = this -> state.windows.text;
        }

        if (0u != this -> index) {
          while (L' ' == *text or L'\t' == *text) ++text;
          if (L'\0' == *text) return vector::NO_ARGUMENT;
        }

        if (vector::EMULATE_WINSHELL == this -> method) {
          if (0u == this -> index) {
            // ->> Shell32: argv[0] ends at the next quote or whitespace.
            if (L'"' == *text) {
              ++text;
              while (L'\0' != *text and L'"' != *text) {
                if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                buffer[length++] = *text++;
              }
              if (L'"' == *text) ++text;
            } else {
              while (L'\0' != *text and L' ' != *text and L'\t' != *text) {
                if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                buffer[length++] = *text++;
              }
            }
          } else {
            while (L'\0' != *text) {
              if (0u == quotes and (L' ' == *text or L'\t' == *text)) break;
              if (L'\\' == *text) {
                unsigned slashes = 0u;
                while (L'\\' == *text) { ++slashes; ++text; }
                if (L'"' == *text) {
                  for (unsigned index = 0u; index < slashes / 2u; ++index) {
                    if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                    buffer[length++] = L'\\';
                  }
                  if (0u == (slashes % 2u)) ++quotes;
                  else {
                    if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                    buffer[length++] = L'"';
                  }
                  ++text;
                } else {
                  for (unsigned index = 0u; index < slashes; ++index) {
                    if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                    buffer[length++] = L'\\';
                  }
                  continue;
                }
              } else if (L'"' == *text) {
                ++quotes;
                ++text;
              } else {
                if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                buffer[length++] = *text++;
                continue;
              }
              while (L'"' == *text) {
                if (3u == ++quotes) {
                  if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                  buffer[length++] = L'"';
                  quotes = 0u;
                }
                ++text;
              }
              if (2u == quotes) quotes = 0u;
            }
          }
        } else {
          // ->> The UCRT-style wide parser; no DBCS lead-byte treatment is needed.
          if (0u == this -> index) {
            while (L'\0' != *text) {
              if (L'"' == *text) {
                in_quotes = not in_quotes;
                ++text;
                continue;
              }
              if (not in_quotes and (L' ' == *text or L'\t' == *text)) break;
              if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
              buffer[length++] = *text++;
            }
          } else {
            while (L'\0' != *text) {
              unsigned slashes = 0u;
              bool copy_quote = true;
              while (L'\\' == *text) { ++slashes; ++text; }
              if (L'"' == *text) {
                if (0u == (slashes % 2u)) {
                  if (in_quotes and L'"' == text[1]) ++text;
                  else { in_quotes = not in_quotes; copy_quote = false; }
                }
                slashes /= 2u;
              }
              for (unsigned index = 0u; index < slashes; ++index) {
                if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                buffer[length++] = L'\\';
              }
              if (L'\0' == *text or (not in_quotes and (L' ' == *text or L'\t' == *text))) break;
              if (copy_quote) {
                if (COMMAND_LINE_MAXIMUM - 1u <= length) return vector::ERROR_ARGUMENT;
                buffer[length++] = *text++;
              } else ++text;
            }
          }
        }

        buffer[length] = L'\0';
        if (NULL != output) {
          if (0u == capacity or static_cast< ::std::size_t >(INT_MAX) < capacity) return vector::ERROR_ARGUMENT;
          if (0 == ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, buffer, -1,
                                        output, static_cast<int>(capacity), NULL, NULL)) return vector::ERROR_ARGUMENT;
        }
        this -> state.windows.text = text;
        ++this -> index;
        return vector::NEXT_ARGUMENT;
      }
      #endif
      #if defined __APPLE__
        case vector::C: {
          if (static_cast< ::std::size_t >(this -> state.apple.count) <= this -> index) return vector::NO_ARGUMENT;
          if (NULL == this -> state.apple.value[this -> index]) return vector::ERROR_ARGUMENT;
          length = ::std::strlen(this -> state.apple.value[this -> index]);
          if (NULL != output) {
            if (capacity <= length) return vector::ERROR_ARGUMENT;
            ::std::memcpy(output, this -> state.apple.value[this -> index], length + 1u);
          }
          ++this -> index;
          return vector::NEXT_ARGUMENT;
        }
      #endif
      #if defined __FreeBSD__
        case vector::FREEBSD: {
          ::std::size_t const start = this -> state.freebsd.offset;
          if (this -> state.freebsd.length == start) return vector::NO_ARGUMENT;
          while (this -> state.freebsd.offset < this -> state.freebsd.length and '\0' != this -> state.freebsd.value[this -> state.freebsd.offset])
            ++this -> state.freebsd.offset;
          if (this -> state.freebsd.length == this -> state.freebsd.offset) return vector::ERROR_ARGUMENT;
          length = this -> state.freebsd.offset - start;
          if (NULL != output) {
            if (capacity <= length) { this -> state.freebsd.offset = start; return vector::ERROR_ARGUMENT; }
            ::std::memcpy(output, this -> state.freebsd.value + start, length + 1u);
          }
          ++this -> state.freebsd.offset;
          ++this -> index;
          return vector::NEXT_ARGUMENT;
        }
      #endif
      #if (defined __ANDROID__ or defined __linux__ or defined __unix__) and not defined _WIN32 and not defined __APPLE__ and not defined __FreeBSD__
        case vector::POSIX: {
          while (true) {
            if (this -> state.proc.chunk_length == this -> state.proc.chunk_offset) {
              ::ssize_t received = 0;
              do {
                received = ::read(this -> state.proc.descriptor, this -> state.proc.chunk, sizeof this -> state.proc.chunk);
              } while (0 > received and EINTR == errno);
              if (0 > received) return vector::ERROR_ARGUMENT;
              if (0 == received) return 0u == length ? vector::NO_ARGUMENT : vector::ERROR_ARGUMENT;
              this -> state.proc.chunk_length = static_cast< ::std::size_t >(received);
              this -> state.proc.chunk_offset = 0u;
            }
            unsigned char const character = this -> state.proc.chunk[this -> state.proc.chunk_offset++];
            if (0u == character) {
              if (NULL != output) {
                if (capacity <= length) return vector::ERROR_ARGUMENT;
                output[length] = '\0';
              }
              ++this -> index;
              return vector::NEXT_ARGUMENT;
            }
            if (NULL != output and 0u < capacity and length < capacity - 1u) output[length] = static_cast<char>(character);
            ++length;
          }
        }
      #endif
      default: return vector::ERROR_ARGUMENT;
    }
  }
};

extern "C" {
  static int count(enum vector::source method, char const commandLine[] = NULL, wchar_t const commandLineUnicode[] = NULL) /* noexcept */ {
    struct vector arguments;
    int result = 0;
    enum vector::next status = vector::NO_ARGUMENT;
    if (false == arguments.begin(method, commandLine, commandLineUnicode)) return -1;
    while (vector::NEXT_ARGUMENT == (status = arguments.next(NULL, 0u))) {
      if (INT_MAX == result) { status = vector::ERROR_ARGUMENT; break; }
      ++result;
    }
    arguments.end();
    return vector::NO_ARGUMENT == status ? result : -1;
  }

  // ->> Current executable path rather than argv[0].
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
      // ->> ANSI fallback returns the active code page, not UTF-8.
      if (static_cast< ::std::size_t >(static_cast< ::DWORD >(-1)) < capacity) return false;
      ::DWORD const bytes = ::GetModuleFileNameA(NULL, output, static_cast< ::DWORD >(capacity));
      return 0u < bytes and bytes < capacity;
    #elif defined __APPLE__
      if (static_cast< ::std::size_t >(static_cast<unsigned>(-1)) < capacity) return false;
      unsigned length = static_cast<unsigned>(capacity);
      return 0 == ::_NSGetExecutablePath(output, &length);
    #elif defined __FreeBSD__
      int managementInformationBase[] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
      ::std::size_t length = capacity;
      return 0 == ::sysctl(managementInformationBase, 4u, output, &length, NULL, 0u) and
             0u < length and length <= capacity and '\0' == output[length - 1u];
    #elif defined __ANDROID__ or defined __linux__ or defined __unix__
      ::ssize_t const length = ::readlink("/proc/self/exe", output, capacity);
      if (0 >= length or capacity <= static_cast< ::std::size_t >(length)) return false;
      output[length] = '\0';
      return true;
    #endif
  }
}

#if not defined PROCESS_ARGUMENTS_NO_MAIN
int main(void) {
  #if defined _WIN32
    enum vector::source const method = vector::EMULATE_UCRT;
  #elif defined __APPLE__
    enum vector::source const method = vector::C;
  #elif defined __FreeBSD__
    enum vector::source const method = vector::FREEBSD;
  #elif defined __ANDROID__ or defined __linux__ or defined __unix__
    enum vector::source const method = vector::POSIX;
  #endif
  enum vector::next status           = vector::ERROR_ARGUMENT;
  char              executable[4096] = ".";
  int const         total            = ::count(method);
  struct vector     arguments        = {};

  if (0 > total or false == ::executable(executable, sizeof executable)) return EXIT_FAILURE;
  (void) ::std::printf("executable = \"%s\"\r\n", executable);
  (void) ::std::printf("argc = %d\r\n", total);
  if (arguments.begin(method)) {
    for (char argument[4096] = ""; vector::NEXT_ARGUMENT == (status = arguments.next(argument, sizeof argument)); ) {
      (void) ::std::printf("argv[%lu] = \"%s\"\r\n", static_cast<unsigned long>(arguments.index - 1u), argument);
    }
    arguments.end();
  }
  return vector::NO_ARGUMENT == status ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif
