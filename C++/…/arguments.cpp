/* TODO */
#include <climits>  // --> INT_MAX
#include <cstddef>  // --> NULL; ::std::size_t
#include <cstdio>   // --> ::std::printf(…)
#include <cstdlib>  // --> EXIT_FAILURE, EXIT_SUCCESS; __argc, __argv, __wargv
#include <cstring>  // --> ::std::memcpy(…), ::std::memset(…), ::std::strlen(…)
#include <iso646.h> // --> and, not, or
#if defined _WIN32
# include <windows.h> // --> CP_ACP, CP_UTF8, WC_ERR_INVALID_CHARS; ::DWORD, ::HMODULE, ::LPCCH, ::LPBOOL; ::GetCommandLineA(…), ::GetCommandLineW(…), ::GetModuleFileNameA(…), ::GetModuleFileNameW(…), ::LocalFree(…), ::MultiByteToWideChar(…), ::WideCharToMultiByte(…)
#   include <shellapi.h> // --> ::CommandLineToArgvW(…)
# if defined _MSC_VER
#   pragma comment(lib, "kernel32.lib")
#   pragma comment(lib, "shell32.lib")
# endif
#elif defined __APPLE__
# include <crt_externs.h> // --> ::_NSGetArgc(…), ::_NSGetArgv(…)
# include <mach-o/dyld.h> // --> ::_NSGetExecutablePath(…)
# include <wordexp.h>     // --> WRDE_NOCMD, WRDE_NOSPACE, WRDE_UNDEF; ::wordexp_t; ::wordexp(…), ::wordfree(…)
#elif defined __FreeBSD__
# include <sys/sysctl.h>    // --> CTL_KERN, KERN_PROC, KERN_PROC_ARGS, KERN_PROC_PATHNAME; ::sysctl(…)
# include <sys/syslimits.h> // --> ARG_MAX
# include <sys/types.h>     //
# include <wordexp.h>       // --> WRDE_NOCMD, WRDE_NOSPACE, WRDE_UNDEF; ::wordexp_t; ::wordexp(…), ::wordfree(…)
#elif defined __ANDROID__ or defined __linux__ or defined __unix__
# include <cerrno>   // --> EINTR; errno
# include <fcntl.h>  // --> O_CLOEXEC, O_RDONLY; ::open(…)
# include <unistd.h> // --> ::ssize_t; ::close(…), ::read(…), ::readlink(…)
# if not defined __ANDROID__ and defined __linux__
#   include <wordexp.h> // --> WRDE_NOCMD, WRDE_NOSPACE, WRDE_UNDEF; ::wordexp_t; ::wordexp(…), ::wordfree(…)
# endif
#endif

/* ... */
#if defined _WIN32
  static ::std::size_t const COMMAND_LINE_MAXIMUM = 32768u; // ->> `::CreateProcessW(…)` UTF-16 code units limit (including NUL)
#elif defined __FreeBSD__
  static ::std::size_t const COMMAND_LINE_ARGUMENT_MAXIMUM = ARG_MAX; // ->> `::exec(…)` argument limit --> 0 == ::sysctl(int[] {CTL_KERN, KERN_ARGMAX}, 2zu, &int maximum, &length = sizeof(int), (void*) NULL, 0u) ? length == sizeof(int) and maximum > 0 : false
#endif

enum {
  COMMAND_ARGUMENT_COUNT = 8u // ->> Maximum argument count of the custom literal-word parser only.
};

/* ... */
struct vector /* final */ {
  enum next /* : signed char */ {
    ERROR_ARGUMENT = -1,
    NEXT_ARGUMENT  = +1,
    NO_ARGUMENT    =  0
  };

  enum source /* : unsigned char */ {
    C,                // ->> C runtime argument vector `argv`
    EMULATE_POSIX,    // ->> POSIX-style command-line tokenizer
    EMULATE_UCRT,     // ->> Windows UCRT parser
    EMULATE_WINSHELL, // ->> Windows Shell32 parser
    FREEBSD,          // ->> `::sysctl(…)`        command-line arguments snapshot
    INVALID,          //
    POSIX,            // ->> `/proc/self/cmdline` command-line arguments
    UCRT,             // ->> C runtime `__argv` (or `__wargv`).
    WINSHELL,         // --> CommandLineToArgvW(…)
    WORD_EXP          // --> wordexp(…)
  };

  ::std::size_t index;
  enum source   method;
  ::std::size_t manual_count;
  char         *manual_values[COMMAND_ARGUMENT_COUNT + 1u];
  union {
    char manual_text[COMMAND_ARGUMENT_COUNT * 4096u];
    #if defined _WIN32
      union { char **narrow; wchar_t **wide; } value;
      wchar_t                                  buffer[COMMAND_LINE_MAXIMUM];
    #elif defined __APPLE__
      char **value;
    #elif defined __FreeBSD__
      char value[COMMAND_LINE_ARGUMENT_MAXIMUM];
    #elif defined __ANDROID__ or defined __linux__ or defined __unix__
      unsigned char chunk[4096];
    #endif
    #if defined __APPLE__ or defined __FreeBSD__ or (defined __linux__ and not defined __ANDROID__)
      ::wordexp_t expansion;
    #endif
  };
  #if defined _WIN32
    wchar_t const *text;
    int argc;
  #elif defined __APPLE__
    int argc;
  #elif defined __FreeBSD__
    ::std::size_t length;
    ::std::size_t offset;
  #elif defined __ANDROID__ or defined __linux__ or defined __unix__
    int descriptor;
    ::std::size_t chunk_length;
    ::std::size_t chunk_offset;
  #endif

  // ->> Lexical POSIX-style quoting and escaping, bounded to eight arguments.
  // ->> No evaluation or expansion: $, *, ~, parentheses, and shell operators
  // ->> are ordinary text (unless quoted or escaped). Not a full wordexp() replacement.
  // ->> Output vector owns the characters; INVALID means malformed input or overflow.
  static bool tokenize(char const input[], struct vector *const output) /* noexcept */ {
    enum quote { UNQUOTED, SINGLE_QUOTED, DOUBLE_QUOTED } state = UNQUOTED;
    ::std::size_t count  = 0u;
    ::std::size_t length = 0u;
    ::std::size_t offset = 0u;
    bool active = false;

    // ...
    if (NULL == output) return false;
    output -> index = 0u;
    output -> manual_count = 0u;
    output -> method = vector::INVALID;
    output -> manual_values[0] = NULL;
    if (NULL == input) return false;

    while (true) {
      unsigned char const character = static_cast<unsigned char>(*input);

      if ('\0' == character) {
        if (UNQUOTED != state) return false;
        if (active) {
          if (COMMAND_ARGUMENT_COUNT <= count) return false;
          if (sizeof output -> manual_text <= offset + length) return false;
          output -> manual_text[offset + length] = '\0';
          offset += length + 1u;
          ++count;
        }
        output -> manual_values[count] = NULL;
        output -> manual_count = count;
        output -> method = vector::EMULATE_POSIX;
        return true;
      }

      if (UNQUOTED == state) {
        if (' ' == character or '\t' == character) {
          if (active) {
            if (COMMAND_ARGUMENT_COUNT <= count) return false;
            if (sizeof output -> manual_text <= offset + length) return false;
            output -> manual_text[offset + length] = '\0';
            offset += length + 1u;
            ++count;
            length = 0u;
            active = false;
          }
          ++input;
          continue;
        }
        if ('\'' == character or '"' == character) {
          state = '\'' == character ? SINGLE_QUOTED : DOUBLE_QUOTED;
          if (false == active) {
            if (COMMAND_ARGUMENT_COUNT <= count) return false;
            output -> manual_values[count] = output -> manual_text + offset;
          }
          active = true;
          ++input;
          continue;
        }
        if ('\\' == character) {
          if ('\0' == input[1]) return false;
          if ('\n' == input[1]) { input += 2; continue; }
          ++input;
        }
      } else if (SINGLE_QUOTED == state) {
        if ('\'' == character) { state = UNQUOTED; ++input; continue; }
      } else {
        if ('"' == character) { state = UNQUOTED; ++input; continue; }
        if ('\\' == character) {
          if ('\0' == input[1]) return false;
          if ('\n' == input[1]) { input += 2; continue; }
          if ('$' == input[1] or '`' == input[1] or '"' == input[1] or '\\' == input[1]) ++input;
        }
      }

      if (COMMAND_ARGUMENT_COUNT <= count or 4096u - 1u <= length or sizeof output -> manual_text <= offset + length) return false;
      if (false == active) output -> manual_values[count] = output -> manual_text + offset;
      output -> manual_text[offset + length++] = *input++;
      active = true;
    }
  }

  // ->> Native process backends ignore supplied text; WORD_EXP and EMULATE_POSIX require it.
  bool begin(enum source const method, char const commandLine[] = NULL, wchar_t const commandLineUnicode[] = NULL) /* noexcept */ {
    this -> method = method;
    this -> index  = 0u;
    (void) commandLineUnicode;

    switch (method) {
      case vector::EMULATE_POSIX:
        return vector::tokenize(commandLine, this);

      #if defined __APPLE__ or defined __FreeBSD__ or (defined __linux__ and not defined __ANDROID__)
        case vector::WORD_EXP: {
          int result = 0;
          if (NULL == commandLine) return false;
          ::std::memset(&this -> expansion, 0, sizeof this -> expansion);
          result = ::wordexp(commandLine, &this -> expansion, WRDE_NOCMD | WRDE_UNDEF);
          if (0 != result) {
            if (WRDE_NOSPACE == result) (void) ::wordfree(&this -> expansion);
            return false;
          }
          return true;
        }
      #endif

      #if defined _WIN32
        case vector::EMULATE_UCRT:
        case vector::EMULATE_WINSHELL:
        case vector::WINSHELL: {
          this -> argc = 0;
          this -> text  = NULL;
          this -> value.wide = NULL;

          if (NULL != commandLineUnicode) this -> text = commandLineUnicode;
          else if (NULL != commandLine) {
            if (0 == ::MultiByteToWideChar(CP_ACP, 0u, commandLine, -1,
                                          this -> buffer, static_cast<int>(COMMAND_LINE_MAXIMUM))) return false;
            this -> text = this -> buffer;
          } else this -> text = ::GetCommandLineW();

          if (NULL == this -> text) {
            char const *const narrow = ::GetCommandLineA();
            if (NULL == narrow or 0 == ::MultiByteToWideChar(CP_ACP, 0u, narrow, -1,
                                                            this -> buffer, static_cast<int>(COMMAND_LINE_MAXIMUM))) return false;
            this -> text = this -> buffer;
          }

          if (vector::WINSHELL == method) {
            this -> value.wide = ::CommandLineToArgvW(this -> text, &this -> argc);
            return NULL != this -> value.wide;
          }
        } return true;
        case vector::UCRT:
          if (0 > __argc) return false;
          this -> argc = __argc;
          if (NULL != __wargv) this -> value.wide = __wargv;
          else if (NULL != __argv) this -> value.narrow = __argv;
          else return false;
          return true;
      #elif defined __APPLE__
        case vector::C:
          this -> argc = *::_NSGetArgc();
          this -> value = *::_NSGetArgv();
          return 0 <= this -> argc and NULL != this -> value;
      #elif defined __FreeBSD__
        case vector::FREEBSD: {
          int const managementInformationBase[] = {CTL_KERN, KERN_PROC, KERN_PROC_ARGS, -1};
          this -> length = sizeof this -> value;
          this -> offset = 0u;
          return 0 == ::sysctl(managementInformationBase, 4u, this -> value,
                               &this -> length, static_cast<void*>(NULL), 0u);
        }
      #elif defined __ANDROID__ or defined __linux__ or defined __unix__
        case vector::POSIX:
          this -> chunk_length = 0u;
          this -> chunk_offset = 0u;
          #if defined O_CLOEXEC
            this -> descriptor = ::open("/proc/self/cmdline", O_RDONLY | O_CLOEXEC);
          #else
            this -> descriptor = ::open("/proc/self/cmdline", O_RDONLY);
          #endif
          return 0 <= this -> descriptor;
      #endif
      default: return false;
    }
  }

  void end(void) /* noexcept */ {
    switch (this -> method) {
      #if defined _WIN32
        case vector::WINSHELL:
          if (NULL != this -> value.wide) (void) ::LocalFree(this -> value.wide);
          this -> value.wide = NULL;
          break;
      #endif
      #if defined __APPLE__ or defined __FreeBSD__ or (defined __linux__ and not defined __ANDROID__)
        case vector::WORD_EXP:
          (void) ::wordfree(&this -> expansion);
          break;
      #endif
      #if (defined __ANDROID__ or defined __linux__ or defined __unix__) and not defined __APPLE__ and not defined __FreeBSD__ and not defined _WIN32
        case vector::POSIX:
          if (0 <= this -> descriptor) (void) ::close(this -> descriptor);
          this -> descriptor = -1;
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
        if (this -> manual_count <= this -> index) return vector::NO_ARGUMENT;
        char const *const argument = this -> manual_values[this -> index];
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
          if (this -> expansion.we_wordc <= this -> index) return vector::NO_ARGUMENT;
          char const *const argument = this -> expansion.we_wordv[this -> index];
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
          if (static_cast< ::std::size_t >(this -> argc) <= this -> index) return vector::NO_ARGUMENT;
          if (vector::WINSHELL == this -> method or NULL != ::__wargv) {
            if (NULL == this -> value.wide[this -> index]) return vector::ERROR_ARGUMENT;
            if (NULL != output) {
              if (0u == capacity or static_cast< ::std::size_t >(INT_MAX) < capacity) return vector::ERROR_ARGUMENT;
              if (0 == ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                                            this -> value.wide[this -> index], -1,
                                            output, static_cast<int>(capacity), static_cast< ::LPCCH >(NULL), static_cast< ::LPBOOL >(NULL))) return vector::ERROR_ARGUMENT;
            }
          } else {
            if (NULL == this -> value.narrow[this -> index]) return vector::ERROR_ARGUMENT;
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
        wchar_t const *text = this -> text;
        bool in_quotes = false;
        unsigned quotes = 0u;

        if (NULL == text) return vector::ERROR_ARGUMENT;
        if (0u == this -> index and L'\0' == *text) {
          if (vector::EMULATE_WINSHELL == this -> method) {
            ::DWORD const obtained = ::GetModuleFileNameW(static_cast< ::HMODULE >(NULL), buffer,
                                                           static_cast< ::DWORD >(COMMAND_LINE_MAXIMUM));
            if (0u == obtained or COMMAND_LINE_MAXIMUM <= obtained) {
              char narrow[COMMAND_LINE_MAXIMUM];
              ::DWORD const bytes = ::GetModuleFileNameA(static_cast< ::HMODULE >(NULL), narrow, sizeof narrow);
              if (0u == bytes or sizeof narrow <= bytes) return vector::ERROR_ARGUMENT;
              if (0 == ::MultiByteToWideChar(CP_ACP, 0u, narrow, -1, buffer,
                                            static_cast<int>(COMMAND_LINE_MAXIMUM))) return vector::ERROR_ARGUMENT;
            }
            if (NULL != output) {
              if (0u == capacity or static_cast< ::std::size_t >(INT_MAX) < capacity) return vector::ERROR_ARGUMENT;
              if (0 == ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, buffer, -1,
                                            output, static_cast<int>(capacity), static_cast< ::LPCCH >(NULL), static_cast< ::LPBOOL >(NULL))) return vector::ERROR_ARGUMENT;
            }
            ++this -> index;
            return vector::NEXT_ARGUMENT;
          }
          // ->> vector::UCRT parses the returned executable path as a replacement command line.
          ::DWORD const obtained = ::GetModuleFileNameW(static_cast< ::HMODULE >(NULL), this -> buffer,
                                                         static_cast< ::DWORD >(COMMAND_LINE_MAXIMUM));
          if (0u == obtained or COMMAND_LINE_MAXIMUM <= obtained) {
            char narrow[COMMAND_LINE_MAXIMUM];
            ::DWORD const bytes = ::GetModuleFileNameA(static_cast< ::HMODULE >(NULL), narrow, sizeof narrow);
            if (0u == bytes or sizeof narrow <= bytes) return vector::ERROR_ARGUMENT;
            if (0 == ::MultiByteToWideChar(CP_ACP, 0u, narrow, -1, this -> buffer,
                                          static_cast<int>(COMMAND_LINE_MAXIMUM))) return vector::ERROR_ARGUMENT;
          }
          this -> text = this -> buffer;
          text = this -> text;
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
                                        output, static_cast<int>(capacity), static_cast< ::LPCCH >(NULL), static_cast< ::LPBOOL >(NULL))) return vector::ERROR_ARGUMENT;
        }
        this -> text = text;
        ++this -> index;
        return vector::NEXT_ARGUMENT;
      }
      #endif
      #if defined __APPLE__
        case vector::C: {
          if (static_cast< ::std::size_t >(this -> argc) <= this -> index) return vector::NO_ARGUMENT;
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
            if (capacity <= length) { this -> offset = start; return vector::ERROR_ARGUMENT; }
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
            if (NULL != output and 0u < capacity and length < capacity - 1u) output[length] = static_cast<char>(character);
            ++length;
          }
        }
      #endif
      default: return vector::ERROR_ARGUMENT;
    }
  }

  static int count(enum vector::source const method, char const commandLine[] = NULL, wchar_t const commandLineUnicode[] = NULL) /* noexcept */ {
    struct vector arguments;
    int result = 0;
    enum vector::next status = vector::NO_ARGUMENT;

    // ...
    if (not arguments.begin(method, commandLine, commandLineUnicode))
    return -1;

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
      ::DWORD const obtained = ::GetModuleFileNameW(static_cast< ::HMODULE >(NULL), path, static_cast< ::DWORD >(COMMAND_LINE_MAXIMUM));
      if (0u < obtained and COMMAND_LINE_MAXIMUM > obtained) {
        if (static_cast< ::std::size_t >(INT_MAX) < capacity) return false;
        return 0 != ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, path, -1, output, static_cast<int>(capacity), static_cast< ::LPCCH >(NULL), static_cast< ::LPBOOL >(NULL));
      }
      // ->> ANSI fallback returns the active code page, not UTF-8.
      if (static_cast< ::std::size_t >(static_cast< ::DWORD >(-1)) < capacity) return false;
      ::DWORD const bytes = ::GetModuleFileNameA(static_cast< ::HMODULE >(NULL), output, static_cast< ::DWORD >(capacity));
      return 0u < bytes and bytes < capacity;
    #elif defined __APPLE__
      if (static_cast< ::std::size_t >(static_cast<unsigned>(-1)) < capacity) return false;
      unsigned length = static_cast<unsigned>(capacity);
      return 0 == ::_NSGetExecutablePath(output, &length);
    #elif defined __FreeBSD__
      int managementInformationBase[] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
      ::std::size_t length = capacity;
      return 0 == ::sysctl(managementInformationBase, 4u, output, &length, static_cast<void*>(NULL), 0u) and
             0u < length and length <= capacity and '\0' == output[length - 1u];
    #elif defined __ANDROID__ or defined __linux__ or defined __unix__
      ::ssize_t const length = ::readlink("/proc/self/exe", output, capacity);
      if (0 >= length or capacity <= static_cast< ::std::size_t >(length)) return false;
      output[length] = '\0';
      return true;
    #endif
  }
};

/* Main */
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
  int const         count            = vector::count(method);
  struct vector     arguments        = {};

  // ...
  if (count < 0 or not vector::executable(executable, sizeof executable))
  return EXIT_FAILURE;

  (void) ::std::printf("executable = \"%s\"\r\n", executable);
  (void) ::std::printf("argc = %d\r\n", count);

  if (arguments.begin(method)) {
    for (char argument[4096] = ""; vector::NEXT_ARGUMENT == (status = arguments.next(argument, sizeof argument)); )
    (void) ::std::printf("argv[%lu] = \"%s\"\r\n", static_cast<unsigned long>(arguments.index - 1u), argument);

    arguments.end();
  }

  return vector::NO_ARGUMENT == status ? EXIT_SUCCESS : EXIT_FAILURE;
}
