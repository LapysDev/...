/* POSIX:      clang++ -std=c++98 -pedantic -O2 -Wall -Wextra -pthread localhost.cpp -o localhost */
/* Windows:    rc /nologo /folocalhost.res localhost.rc && cl /nologo /utf-8 /EHsc /O2 /DUNICODE /D_UNICODE localhost.cpp localhost.res /Fe:localhost.exe /link /SUBSYSTEM:WINDOWS ws2_32.lib shell32.lib user32.lib kernel32.lib && del /q localhost.obj localhost.res */
/*   PowerShell: New-NetFirewallRule -Action Allow -Direction Inbound -DisplayName "LapysDev Server" -LocalPort 8000 -Profile Private -Program (Resolve-Path "C:\LapysDev\tools\localhost.exe").Path -Protocol TCP -RemoteAddress LocalSubnet4 */
/* localhost [--bind 127.0.0.1 | --lan] [--help] [--port 8000] [--root "C:/LapysDev"] [--shutdown] */
#include <cctype>   // --> ::std::tolower(…)
#include <cstddef>  // --> ::std::size_t
#include <cstdio>   // --> ::std::FILE; stdout; ::std::fclose(…), ::std::fopen(…), ::std::fputs(…), ::std::fread(…), ::std::sprintf(…)
#include <cstdlib>  // --> ::std::calloc(…), ::std::free(…), ::std::getenv(…), ::std::malloc(…), ::std::realloc(…)
#include <cstring>  // --> ::std::memchr(…), ::std::memcpy(…), ::std::memmove(…), ::std::strcmp(…), ::std::strchr(…), ::std::strlen(…), ::std::strpbrk(…), ::std::strrchr(…), ::std::strstr(…)
#include <ctime>    // --> ::std::time_t, ::std::tm; ::std::strftime(…), ::std::time(…)
#include <iso646.h> // --> and, not, or
#include <stdint.h> // --> ::uint_least8_t, ::uint_least16_t, ::uint_least32_t, ::uintptr_t
#if defined _WIN32
# define NOMINMAX
# define WIN32_LEAN_AND_MEAN
#
# include <cwchar>     // --> ::std::wcslen(…)
# include <winsock2.h> // --> ::SOCKET, ::WSADATA, ::fd_set, ::sockaddr, ::sockaddr_in, ::timeval; AF_INET, FD_ISSET, FD_SET, FD_ZERO, INADDR_LOOPBACK, INVALID_SOCKET, MAKEWORD, SO_EXCLUSIVEADDRUSE, SO_RCVTIMEO, SO_SNDTIMEO, SOL_SOCKET, SOCK_STREAM; …
# include <ws2tcpip.h> // --> in6addr_loopback, IPPROTO_IPV6, IPPROTO_TCP, IPV6_V6ONLY; ::sockaddr_in6
# include <windows.h>  // --> ATTACH_PARENT_PROCESS, CP_UTF8, ERROR_ALREADY_EXISTS, EVENT_MODIFY_STATE, FALSE, FILE_ATTRIBUTE_DIRECTORY, GetFileExInfoStandard, INVALID_FILE_ATTRIBUTES, INVALID_HANDLE_VALUE, MB_ERR_INVALID_CHARS, STD_OUTPUT_HANDLE, TRUE, WAIT_OBJECT_0, WC_ERR_INVALID_CHARS; ::BOOL, ::DWORD, ::HANDLE, ::HINSTANCE, ::LONG, ::LPWSTR, ::UINT_PTR, ::ULONGLONG, ::WIN32_FILE_ATTRIBUTE_DATA, ::WIN32_FIND_DATAW; ::AttachConsole(…), ::FreeConsole(…); …
#   include <process.h>  // --> ::_beginthreadex(…)
#   include <shellapi.h> // --> ::CommandLineToArgvW(…)
#
# pragma comment(lib, "kernel32.lib")
# pragma comment(lib, "shell32.lib")
# pragma comment(lib, "user32.lib")
# pragma comment(lib, "ws2_32.lib")

extern "C" __declspec(dllimport) ::BOOL    WINAPI AttachConsole       (::DWORD);
extern "C" WINBASEAPI            ::BOOL    WINAPI CloseHandle         (::HANDLE);
extern "C" WINSHELLAPI           ::LPWSTR* WINAPI CommandLineToArgvW  (::LPCWSTR, int*);
extern "C" __declspec(dllimport) ::HANDLE  WINAPI CreateEventW        (::LPSECURITY_ATTRIBUTES, ::BOOL, ::BOOL, ::LPCWSTR);
extern "C" __declspec(dllimport) ::HANDLE  WINAPI CreateMutexW        (::LPSECURITY_ATTRIBUTES, ::BOOL, ::LPCWSTR);
extern "C" __declspec(dllimport) ::BOOL    WINAPI FindClose           (::HANDLE);
extern "C" __declspec(dllimport) ::HANDLE  WINAPI FindFirstFileW      (::LPCWSTR, ::LPWIN32_FIND_DATAW);
extern "C" __declspec(dllimport) ::BOOL    WINAPI FindClose           (::HANDLE);
extern "C" __declspec(dllimport) ::DWORD   WINAPI GetFileAttributesW  (::LPCWSTR);
extern "C" __declspec(dllimport) ::BOOL    WINAPI GetFileAttributesExW(::LPCWSTR, ::GET_FILEEX_INFO_LEVELS, ::LPVOID);
extern "C" __declspec(dllimport) ::DWORD   WINAPI GetLastError        ();
extern "C" __declspec(dllimport) ::HANDLE  WINAPI GetStdHandle        (::DWORD);
extern "C" __declspec(dllimport) ::LPWSTR  WINAPI GetCommandLineW     ();
extern "C" __declspec(dllimport) ::HLOCAL  WINAPI LocalFree           (::HLOCAL);
extern "C" __declspec(dllimport) int       WINAPI MultiByteToWideChar (::UINT, ::DWORD, ::LPCCH, int, ::LPWSTR, int);
extern "C" __declspec(dllimport) ::HANDLE  WINAPI OpenEventW          (::DWORD, ::BOOL, ::LPCWSTR);
extern "C" __declspec(dllimport) ::BOOL    WINAPI SetEvent            (::HANDLE);
extern "C" __declspec(dllimport) ::DWORD   WINAPI WaitForSingleObject (::HANDLE, ::DWORD);
extern "C" __declspec(dllimport) int       WINAPI WideCharToMultiByte (::UINT, ::DWORD, ::LPCWCH, int, ::LPSTR, int, ::LPCCH, ::LPBOOL);
extern "C" __declspec(dllimport) ::BOOL    WINAPI WriteFile           (::HANDLE, ::LPCVOID, ::DWORD, ::LPDWORD, ::LPOVERLAPPED);

extern "C" _ACRTIMP              ::uintptr_t  __cdecl _beginthreadex(void*, unsigned, unsigned (__stdcall*)(void*), void*, unsigned, unsigned*);
extern "C" _ACRTIMP              ::errno_t    __cdecl _gmtime64_s   (struct ::tm*, ::__time64_t const*);
extern "C" _ACRTIMP              ::std::FILE* __cdecl _wfopen       (wchar_t const[], wchar_t const[]);
extern "C" __declspec(dllimport) ::SOCKET     WSAAPI  accept        (::SOCKET, struct ::sockaddr*, int*);
extern "C" __declspec(dllimport) int          WSAAPI  bind          (::SOCKET, struct ::sockaddr const*, int);
extern "C" __declspec(dllimport) int          WSAAPI  closesocket   (::SOCKET);
extern "C" __declspec(dllimport) int          WSAAPI  connect       (::SOCKET, struct ::sockaddr const*, int);
extern "C" __declspec(dllimport) int          WSAAPI  listen        (::SOCKET, int);
extern "C" __declspec(dllimport) int          WSAAPI  recv          (::SOCKET, char*, int, int);
extern "C" __declspec(dllimport) int          WSAAPI  select        (int, ::fd_set*, ::fd_set*, ::fd_set*, ::timeval const*);
extern "C" WINSOCK_API_LINKAGE   int          WSAAPI  send          (::SOCKET, char const[], int, int);
extern "C" __declspec(dllimport) int          WSAAPI  setsockopt    (::SOCKET, int, int, char const*, int);
extern "C" __declspec(dllimport) ::SOCKET     WSAAPI  socket        (int, int, int);
extern "C" __declspec(dllimport) int          WSAAPI  WSACleanup    ();
extern "C" __declspec(dllimport) int          WSAAPI  WSAStartup    (::WORD, ::LPWSADATA);
# if not defined htonl
  extern "C" __declspec(dllimport) ::u_long WSAAPI htonl(::u_long);
# endif
# if not defined htons
  extern "C" __declspec(dllimport) ::u_short WSAAPI htons(::u_short);
# endif
#elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
# include <arpa/inet.h>  // --> htonl(…), htons(…)
# include <cerrno>       // --> EADDRINUSE, EINTR; errno
# include <dirent.h>     // --> ::DIR, ::dirent; ::closedir(…), ::opendir(…), ::readdir(…)
# include <netinet/in.h> // --> INADDR_LOOPBACK, IPPROTO_IPV6, IPPROTO_TCP, IPV6_V6ONLY; ::sockaddr_in, ::sockaddr_in6; in6addr_loopback
# include <pthread.h>    // --> PTHREAD_MUTEX_INITIALIZER; ::pthread_t, ::pthread_mutex_t; ::pthread_create(…), ::pthread_detach(…), ::pthread_mutex_lock(…), ::pthread_mutex_unlock(…)
# include <signal.h>     // --> SIG_IGN, SIGINT, SIGPIPE, SIGTERM; ::sig_atomic_t, ::sigaction; ::sigaction(…), ::sigemptyset(…)
# include <sys/select.h> // --> FD_ISSET, FD_SET, FD_ZERO; ::fd_set, ::timeval; ::select(…).
# include <sys/socket.h> // --> AF_*, MSG_NOSIGNAL, SO_*, SOCK_STREAM; ::socklen_t, ::sockaddr; ::accept(…), ::bind(…), ::connect(…), ::listen(…), ::recv(…), ::send(…), ::setsockopt(…), ::socket(…)
# include <sys/stat.h>   // --> S_ISDIR, S_ISREG, S_ISSOCK; ::mode_t, ::stat; ::lstat(…), ::stat(…), ::umask(…)
# include <sys/types.h>  // --> ::off_t, ::ssize_t, ::uid_t
# include <sys/un.h>     // --> AF_UNIX; ::sockaddr_un
# include <unistd.h>     // --> W_OK, X_OK; ::access(…), ::close(…), ::geteuid(…), ::getuid(…), ::unlink(…)

extern "C" int              accept              (int, struct ::sockaddr*, ::socklen_t*);
extern "C" int              access              (char const[], int)                          /* throw() */;
extern "C" int              bind                (int, struct ::sockaddr const*, ::socklen_t) /* throw() */;
extern "C" int              close               (int);
extern "C" int              closedir            (::DIR*);
extern "C" int              connect             (int, struct ::sockaddr const*, ::socklen_t);
extern "C" ::uid_t          geteuid             ()                              /* throw() */;
extern "C" ::uid_t          getuid              ()                              /* throw() */;
extern "C" struct ::tm*     gmtime_r            (::time_t const*, struct ::tm*) /* throw() */;
extern "C" int              listen              (int, int)                      /* throw() */;
extern "C" int              lstat               (char const[], struct ::stat*)  /* throw() */;
extern "C" ::DIR*           opendir             (char const[]);
extern "C" int              pthread_create      (::pthread_t*, ::pthread_attr_t const*, void* (*)(void*), void*) /* throw() */;
extern "C" int              pthread_detach      (::pthread_t)                                                    /* throw() */;
extern "C" int              pthread_mutex_lock  (::pthread_mutex_t*)                                             /* throw() */;
extern "C" int              pthread_mutex_unlock(::pthread_mutex_t*)                                             /* throw() */;
extern "C" ::ssize_t        recv                (int, void*, ::std::size_t, int);
extern "C" struct ::dirent* readdir             (::DIR*);
extern "C" int              select              (int, ::fd_set*, ::fd_set*, ::fd_set*, struct ::timeval*);
extern "C" ::ssize_t        send                (int, void const*, ::std::size_t, int);
extern "C" int              setsockopt          (int, int, int, void const*, ::socklen_t)             /* throw() */;
extern "C" int              sigaction           (int, struct ::sigaction const*, struct ::sigaction*) /* throw() */;
extern "C" int              sigemptyset         (::sigset_t*)                                         /* throw() */;
extern "C" int              socket              (int, int, int)                                       /* throw() */;
extern "C" int              stat                (char const[], struct ::stat*)                        /* throw() */;
extern "C" ::mode_t         umask               (::mode_t)                                            /* throw() */;
extern "C" int              unlink              (char const[])                                        /* throw() */;
#else
# error [!]: Unsupported platform
#endif

/* ... */
enum /* : unsigned … */ {
  ASCII_DELETE                 = 0x7Fu,
  ASCII_SPACE                  = 0x20u,
  CONTROL_BACKLOG              = 4u,
  CONTROL_NAME_MAXIMUM_LENGTH  = 80u,
  HEADER_MAXIMUM_LENGTH        = 16384u,
  HTTP_BAD_REQUEST             = 400u, // --> ::uint_least16_t:9
  HTTP_FORBIDDEN               = 403u, //
  HTTP_METHOD_NOT_ALLOWED      = 405u, //
  HTTP_NO_CONTENT              = 204u, //
  HTTP_NOT_FOUND               = 404u, //
  HTTP_OK                      = 200u, //
  HTTP_REDIRECT                = 301u, //
  HTTP_REQUEST_TOO_LARGE       = 431u, //
  HTTP_REQUEST_TOO_LONG        = 414u, //
  HTTP_UNSUPPORTED_METHOD      = 501u, //
  HTTP_UNSUPPORTED_VERSION     = 505u, //
  LISTEN_BACKLOG               = 64u,
  PORT_MAXIMUM                 = 65535u,
  REQUEST_LINE_MAXIMUM_LENGTH  = 8192u,
  SOCKET_SEND_LIMIT            = 32768u,
  TIMEOUT_CLIENT_SECONDS       = 10u,
  TIMEOUT_CONTROL_MICROSECONDS = 250000u,
  TIMEOUT_CONTROL_SECONDS      = 1u,
  TIMEOUT_SELECT_MICROSECONDS  = 200000u,
  TIMEOUT_SOCKET_MILLISECONDS  = TIMEOUT_CLIENT_SECONDS * 1000u,
  WORKER_MAXIMUM               = 128u
};

#if defined _WIN32
  typedef ::SOCKET socket_t; // --> WinSock handle
  typedef unsigned thread_t; // --> thread::client(…)

  static int      const INVALID_COMMAND_LINE  = EXIT_FAILURE + 1;
  static socket_t const INVALID_SOCKET_HANDLE = INVALID_SOCKET;

  static ::HANDLE        localhostMutex    = NULL;
  static ::HANDLE        localhostShutdown = NULL;
  static ::LONG volatile workerCount       = 0L;
#elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
  typedef int   socket_t; // --> POSIX descriptor
  typedef void* thread_t; // --> thread::client(…)

  static int      const INVALID_COMMAND_LINE  = EXIT_FAILURE + 1;
  static socket_t const INVALID_SOCKET_HANDLE = -1;

  static socket_t                control                                                                  = INVALID_SOCKET_HANDLE;
  static char                    controlPath[sizeof static_cast<struct ::sockaddr_un*>(NULL) -> sun_path] = {};
  static ::sig_atomic_t volatile localhostShutdown                                                        = static_cast< ::sig_atomic_t>(false);
  static long                    workerCount                                                              = 0L;
  static ::pthread_mutex_t       workerMutex                                                              = PTHREAD_MUTEX_INITIALIZER;
#endif
static socket_t         listeners[2] = {INVALID_SOCKET_HANDLE, INVALID_SOCKET_HANDLE};
static ::uint_least32_t bindIPv4     = INADDR_LOOPBACK; // ->> Host-order IPv4; `--bind` selects a particular network interface
static ::uint_least16_t port         = 8000u;           // ->> HTTP/TCP ports fit in at least 16 bits
struct string /* final */ {
  char         *value;
  ::std::size_t capacity;
  ::std::size_t length;

  /* ... */
  inline  string() /* noexcept */ : value(NULL), capacity(0u), length(0u) {}
  inline ~string() /* noexcept */ { ::std::free(value); }

  /* ... */
  inline bool append(char const character) /* noexcept */ { return this -> append(&character, 1u); }
  inline bool append(char const text[])    /* noexcept */ { return this -> append(text, ::std::strlen(text)); }

  bool append(char const text[], ::std::size_t const count) /* noexcept */ {
    if (SIZE_MAX - this -> length - 1u < count)
    return false;

    if (capacity < count + length + 1u) {
      ::std::size_t nextCapacity = 0u == this -> capacity ? 64u : this -> capacity;

      // ...
      for (::std::size_t const required = count + length + 1u; nextCapacity < required; nextCapacity *= 2u)
      if (SIZE_MAX / 2u < nextCapacity) {
        nextCapacity = required;
        break;
      }

      if (void *const allocation = ::std::realloc(this -> value, nextCapacity)) {
        this -> capacity = nextCapacity;
        this -> value    = static_cast<char*>(allocation);
      } else return false;
    }

    if (count) { (void) ::std::memmove(&this -> value[this -> length], text, count); }
    value[length += count] = '\0';

    return true;
  }

  #if defined __ANDROID__ or defined __APPLE__ or defined __unix__
    bool appendControlName() /* noexcept */ {
      struct ::stat attributes = {};
      char const   *directory  = ::std::getenv("XDG_RUNTIME_DIR");
      char          suffix[CONTROL_NAME_MAXIMUM_LENGTH];

      // ... ->> An owner-specific local-domain socket lets a second instance request shutdown
      directory = NULL == directory or '\0' == *directory or 0 != ::stat(directory, &attributes) or not S_ISDIR(attributes.st_mode) or 0 != ::access(directory, W_OK | X_OK) ? "/tmp" : directory; // ->> Runtime directory is preferred with `/tmp` as fallback
      (void) ::std::sprintf(suffix, "lapysdev-localhost-%lu.sock", static_cast<unsigned long>(::getuid()));

      if (not   this -> append(directory))                                                                   return false;
      if (0u != this -> length and '/' != this -> value[this -> length - 1u]) { if (not this -> append('/')) return false; }
      if (not   this -> append(suffix))                                                                      return false;

      return this -> length < sizeof controlPath;
    }
  #endif

  bool appendHTML(char const text[]) /* noexcept */ {
    for (::std::size_t index = 0u; '\0' != text[index]; ++index)
    switch (text[index]) {
      case '&':  if (not this -> append("&amp;"))     return false; break;
      case '<':  if (not this -> append("&lt;"))      return false; break;
      case '>':  if (not this -> append("&gt;"))      return false; break;
      case '"':  if (not this -> append("&quot;"))    return false; break;
      case '\'': if (not this -> append("&#x27;"))    return false; break;
      default:   if (not this -> append(text[index])) return false; // ->> +2226 others
    }

    return true;
  }

  bool appendURL(char const text[]) /* noexcept */ {
    for (::std::size_t index = 0u; '\0' != text[index]; ++index) {
      unsigned char const character = static_cast<unsigned char>(text[index]);

      // ->> Non-ASCII UTF-8 bytes are percent-encoded byte-by-byte, preserving emoji
      if (character == '-' or character == '_' or character == '.' or character == '~' or (character >= 'a' and character <= 'z') or (character >= 'A' and character <= 'Z') or (character >= '0' and character <= '9')) {
        if (not this -> append(static_cast<char>(character)))
        return false;
      }

      else {
        if (not this -> append('%'))                                           return false;
        if (not this -> append("0123456789ABCDEF"[(character >> 4u) & 0x0Fu])) return false;
        if (not this -> append("0123456789ABCDEF"[(character >> 0u) & 0x0Fu])) return false;
      }
    }

    return true;
  }

  inline void clear() /* noexcept */ {
    this -> length = 0u;
    if (NULL != this -> value) this -> value[0] = '\0';
  }

  #if defined _WIN32 // ->> Convert multibyte text to the allocated environment-native format
    static wchar_t* raw(char const text[]) /* noexcept */ {
      int      length  = 0;
      wchar_t *rawtext = NULL;

      // ...
      length = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, NULL, 0);
      if (length <= 0) return NULL;

      rawtext = static_cast<wchar_t*>(::std::malloc(static_cast< ::std::size_t>(length) * sizeof(wchar_t)));
      if (not rawtext) return NULL;

      if (length != ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, -1, rawtext, length)) {
        ::std::free(rawtext);
        return NULL;
      }

      return rawtext;
    }
  #endif

  inline bool set(char const text[]) /* noexcept */ {
    this -> clear();
    return this -> append(text);
  }
} root;

/* ... */
struct directory /* final */ {
  char *name; // ->> UTF-8 bytes on Windows and native bytes on POSIX
  bool  directory;

  /* ... */
  static bool isDirectory(char const path[]) /* noexcept */ {
    #if defined _WIN32
      if (wchar_t* const widepath = string::raw(path)) {
        ::DWORD const attributes = ::GetFileAttributesW(widepath);

        ::std::free(widepath);
        return INVALID_FILE_ATTRIBUTES != attributes and 0u != (attributes & FILE_ATTRIBUTE_DIRECTORY);
      }

      return false;
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      struct ::stat attributes = {};
      return 0 == ::stat(path, &attributes) and S_ISDIR(attributes.st_mode);
    #endif
  }

  static bool isFile(char const path[], void* size) /* noexcept */ {
    // ->> Assert ordinary static files rather than directories, optionally obtaining their size.
    #if defined _WIN32
      if (wchar_t *const widepath = string::raw(path)) {
        ::WIN32_FILE_ATTRIBUTE_DATA attributes = {};

        if (0 != ::GetFileAttributesExW(widepath, ::GetFileExInfoStandard, &attributes) and 0u == (attributes.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
          *static_cast< ::ULONGLONG*>(NULL == size ? &const_cast< ::ULONGLONG&>(static_cast< ::ULONGLONG const&>(::ULARGE_INTEGER().QuadPart)) : size) = (static_cast< ::ULONGLONG>(attributes.nFileSizeHigh) << 32u) | (static_cast< ::ULONGLONG>(attributes.nFileSizeLow) << 0u);
          return ::std::free(widepath), true;
        } ::std::free(widepath);
      }

      return false;
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      /* ... */ {
        struct ::stat attributes = {};

        if (0 == ::stat(path, &attributes) and S_ISREG(attributes.st_mode)) {
          *static_cast< ::off_t*>(NULL == size ? &attributes.st_size : size) = attributes.st_size;
          return true;
        }
      }

      return false;
    #endif
  }
};

struct socket /* final */ {
  static void close(socket_t const socket) /* noexcept */ {
    if (INVALID_SOCKET_HANDLE != socket)
    #if defined _WIN32
      (void) ::closesocket(socket);
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      (void) ::close(socket);
    #endif
  }

  static bool headers(
    socket_t const client,
    unsigned const status,
    char     const label[],
    char     const type [],
    #if defined _WIN32
      ::ULONGLONG const size,
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      ::off_t const size,
    #endif
    char const extra[]
  ) /* noexcept */ {
    ::std::size_t    count      = 0u;
    char             date  [64] = "Thu, 01 Jan 1970 00:00:00 GMT";
    char             digits[64] = "0";
    char             number[64] = "???.??";
    struct string    response;
    struct ::std::tm utc = {};
    #if defined _WIN32
      ::ULONGLONG value = size;
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      ::off_t value = size;
    #endif

    // ...
    #if defined _WIN32
      if (0 == ::_gmtime64_s(&utc, &static_cast< ::__time64_t const&>(static_cast< ::__time64_t>(::std::time(NULL)))))
      (void) ::std::strftime(date, sizeof date, "%a, %d %b %Y %H:%M:%S GMT", &utc);
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      if (NULL != ::gmtime_r(&static_cast< ::std::time_t const&>(::std::time(NULL)), &utc))
      (void) ::std::strftime(date, sizeof date, "%a, %d %b %Y %H:%M:%S GMT", &utc);
    #endif

    (void) ::std::sprintf(number, "%u", status);

    if (not response.append("HTTP/1.0 "))                                return false;
    if (not response.append(number))                                     return false;
    if (not response.append(' '))                                        return false;
    if (not response.append(label))                                      return false;
    if (not response.append("\r\n" "Server: Localhost" "\r\n" "Date: ")) return false;
    if (not response.append(date))                                       return false;
    if (not response.append("\r\n" "Content-Type: "))                    return false;
    if (not response.append(type))                                       return false;
    if (not response.append("\r\n" "Content-Length: "))                  return false;

    do {
      digits[count++] = static_cast<char>('0' + (value % 10u));
      value          /= 10u;
    } while (count < sizeof digits and value);

    while (count) {
      if (not response.append(digits[--count]))
      return false;
    }

    if (not response.append("\r\n" "X-Content-Type-Options: nosniff" "\r\n" "Connection: close" "\r\n")) return false;
    if (not response.append(extra))                                                                      return false;
    if (not response.append("\r\n"))                                                                     return false;

    return socket::send(client, response.value, response.length);
  }

  static socket_t listen(unsigned char const ipVersion) /* noexcept */ {
    socket_t const server = ::socket(ipVersion == 6u ? AF_INET6 : AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // ...
    if (INVALID_SOCKET_HANDLE == server)
    return INVALID_SOCKET_HANDLE;

    #if defined _WIN32
      (void) ::setsockopt(server, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<char const*>(&static_cast< ::BOOL const&>(TRUE)), sizeof(::BOOL)); // ->> Exclusive
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      (void) ::setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &static_cast<int const&>(true), sizeof(int)); // ->> Reuse
    #endif

    switch (ipVersion) {
      case 6u: {
        struct ::sockaddr_in6 address = {};

        // ...
        #if defined _WIN32
          (void) ::setsockopt(server, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<char const*>(&static_cast<int const&>(true)), sizeof(int));
        #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
          (void) ::setsockopt(server, IPPROTO_IPV6, IPV6_V6ONLY, &static_cast<int const&>(true), sizeof(int));
        #endif

        address.sin6_addr   = in6addr_loopback;
        address.sin6_family = AF_INET6;
        address.sin6_port   = ::htons(static_cast<unsigned short>(port));

        if (0 != ::bind(server, reinterpret_cast<struct ::sockaddr const*>(&address), sizeof address) or 0 != ::listen(server, LISTEN_BACKLOG))
        return socket::close(server), INVALID_SOCKET_HANDLE;
      } break;

      case 4u: default: {
        struct ::sockaddr_in address = {};

        // ...
        address.sin_addr.s_addr = ::htonl(bindIPv4);
        address.sin_family      = AF_INET;
        address.sin_port        = ::htons(static_cast<unsigned short>(port));

        if (0 != ::bind(server, reinterpret_cast<struct ::sockaddr const*>(&address), sizeof address) or 0 != ::listen(server, LISTEN_BACKLOG))
        return socket::close(server), INVALID_SOCKET_HANDLE;
      } break;
    }

    return server;
  }

  static bool send(socket_t const client, char const data[], ::std::size_t length) /* noexcept */ {
    while (length) {
      ::std::size_t const amount = SOCKET_SEND_LIMIT < length ? static_cast< ::std::size_t>(SOCKET_SEND_LIMIT) : length;
      #if defined _WIN32
        int const written = ::send(client, data, static_cast<int>(amount), 0);
      #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
        #if defined MSG_NOSIGNAL
          ::ssize_t const written = ::send(client, data, amount, MSG_NOSIGNAL);
        #else
          ::ssize_t const written = ::send(client, data, amount, 0x00);
        #endif
      #endif

      // ... ->> `send(…)` may write fewer bytes than requested, so retry until all data is sent
      if (written <= 0) {
        #if defined __ANDROID__ or defined __APPLE__ or defined __unix__
          if (EINTR == errno)
          continue;
        #endif
        return false;
      }

      data   += written;
      length -= static_cast< ::std::size_t>(written);
    }

    return true;
  }

  static void sendDocument(socket_t const client, unsigned const status, char const label[], struct string const* const document, bool const head, char const extra[]) /* noexcept */ {
    #if defined _WIN32
      ::ULONGLONG const size = static_cast< ::ULONGLONG>(document -> length);
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      ::off_t const size = static_cast< ::off_t>(document -> length);
    #endif

    if (not head and socket::headers(client, status, label, "text/html; charset=utf-8", size, extra))
    (void) socket::send(client, document -> value, document -> length);
  }

  static void sendError(socket_t const client, unsigned const status, char const label[], bool const head) /* noexcept */ {
    struct string document;
    char          number[64] = "???.??";

    // ...
    (void) ::std::sprintf(number, "%u", status);

    if (not document.append("<!DOCTYPE html>" "<html><head><meta charset=\"UTF-8\"/><title>")) return;
    if (not document.append(number))                                                           return;
    if (not document.append(' '))                                                              return;
    if (not document.append(label))                                                            return;
    if (not document.append("</title></head><body><h1>"))                                      return;
    if (not document.append(number))                                                           return;
    if (not document.append(' '))                                                              return;
    if (not document.append(label))                                                            return;
    if (not document.append("</h1></body></html>" "\r\n"))                                     return;

    socket::sendDocument(client, status, label, &document, head, HTTP_METHOD_NOT_ALLOWED == status ? "Allow: GET, HEAD, OPTIONS, TRACE" "\r\n" : "");
  }

  static void workerEnd() /* noexcept */ {
    #if defined _WIN32
      (void) ::InterlockedDecrement(&workerCount);
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      (void) ::pthread_mutex_lock(&workerMutex);
      --workerCount;
      (void) ::pthread_mutex_unlock(&workerMutex);
    #endif
  }
};

struct thread /* final */ {
  #if defined _WIN32
    typedef unsigned /* __stdcall */ thread_t;
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    typedef void *thread_t;
  #endif

  /* ... */
  static thread_t client(void* const parameter) /* noexcept */ {
    #if defined _WIN32
      socket_t const client = static_cast<socket_t>(reinterpret_cast< ::UINT_PTR>(parameter));
      ::ULONGLONG    size   = 0u;
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      socket_t const client = *static_cast<socket_t*>(parameter); ::std::free(parameter);
      ::off_t        size   = 0;
    #endif
    struct /* final */ {
      ::std::size_t length;
      char          memory[HEADER_MAXIMUM_LENGTH + 1u];
      char         *lineEnd;
      char const   *method;
      char         *spaces[2];
      char const   *target;
      char const   *version;
    }             request       = {0u, {}, NULL, NULL, {NULL, NULL}, NULL, NULL};
    ::std::size_t headerEnd     = 0u;
    bool          invalidTarget = false;
    bool          fileEnd       = false;

    // ... ->> One request per connection; socket timeouts bound worker occupancy
    #if defined _WIN32
      ::DWORD const timeout = TIMEOUT_SOCKET_MILLISECONDS;

      (void) ::setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char const*>(&timeout), sizeof timeout);
      (void) ::setsockopt(client, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char const*>(&timeout), sizeof timeout);
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      struct ::timeval const timeout = {TIMEOUT_CLIENT_SECONDS, 0};

      (void) ::setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
      (void) ::setsockopt(client, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof timeout);
      #if defined SO_NOSIGPIPE
        (void) ::setsockopt(client, SOL_SOCKET, SO_NOSIGPIPE, &static_cast<int const&>(1), sizeof(int));
      #endif
    #endif

    do {
      while (HEADER_MAXIMUM_LENGTH > request.length) {
        #if defined _WIN32
          int const received = ::recv(client, &request.memory[request.length], static_cast<int>(HEADER_MAXIMUM_LENGTH - request.length), 0);
        #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
          ::ssize_t const received = ::recv(client, &request.memory[request.length], HEADER_MAXIMUM_LENGTH - request.length, 0);
        #endif

        // ...
        if (received <= 0) {
          fileEnd = true;
          break;
        }

        request.length                += static_cast< ::std::size_t>(received);
        request.memory[request.length] = '\0';

        if (char *const marker = ::std::strstr(request.memory, "\r\n" "\r\n")) { headerEnd = static_cast< ::std::size_t>(marker - request.memory) + 4u; break; }
        if (char *const marker = ::std::strstr(request.memory, "\n"   "\n"))   { headerEnd = static_cast< ::std::size_t>(marker - request.memory) + 2u; break; }
      }

      if (fileEnd)
      break;

      if (0u == headerEnd) {
        socket::sendError(client, HTTP_REQUEST_TOO_LARGE, "Request Header Fields Too Large", false);
        break;
      }

      request.lineEnd = ::std::strchr(request.memory, '\n');
      if (NULL == request.lineEnd or REQUEST_LINE_MAXIMUM_LENGTH < request.lineEnd - request.memory) {
        socket::sendError(client, HTTP_REQUEST_TOO_LONG, "URI Too Long", false);
        break;
      }

      request.spaces[0]                                                                          =                             ::std::strchr(request.memory,        ' ');
      request.spaces[1]                                                                          = NULL != request.spaces[0] ? ::std::strchr(request.spaces[0] + 1, ' ') : NULL;
      request.lineEnd[request.lineEnd > request.memory and request.lineEnd[-1] == '\r' ? -1 : 0] = '\0';
      if (NULL == request.spaces[0] or NULL == request.spaces[1] or NULL != ::std::strchr(request.spaces[1] + 1, ' ')) {
        socket::sendError(client, HTTP_BAD_REQUEST, "Bad Request", false);
        break;
      }

      request.method       = request.memory;
      request.spaces[0][0] = '\0';
      request.spaces[1][0] = '\0';
      request.target       = request.spaces[0] + 1;
      request.version      = request.spaces[1] + 1;

      if (0 != ::std::strcmp(request.version, "HTTP/1.0") and 0 != ::std::strcmp(request.version, "HTTP/1.1")) {
        socket::sendError(client, HTTP_UNSUPPORTED_VERSION, "HTTP Version Not Supported", 0 == ::std::strcmp(request.method, "HEAD"));
        break;
      }

      if (0 != ::std::strcmp(request.method, "GET") and 0 != ::std::strcmp(request.method, "HEAD") and 0 != ::std::strcmp(request.method, "OPTIONS") and 0 != ::std::strcmp(request.method, "TRACE")) {
        socket::sendError(client, 0 == ::std::strcmp(request.method, "CONNECT") ? HTTP_METHOD_NOT_ALLOWED : HTTP_UNSUPPORTED_METHOD, 0 == ::std::strcmp(request.method, "CONNECT") ? "Method Not Allowed" : "Not Implemented", false);
        break;
      }

      for (::std::size_t index = 0u; not invalidTarget and '\0' != request.target[index]; ++index)
        invalidTarget = ASCII_DELETE == request.target[index] or ASCII_SPACE >= static_cast<unsigned char>(request.target[index]);
      if (invalidTarget or (request.target[0] != '/' and not (0 == ::std::strcmp(request.method, "OPTIONS") and 0 == ::std::strcmp(request.target, "*")))) {
        socket::sendError(client, HTTP_BAD_REQUEST, "Bad Request", 0 == ::std::strcmp(request.method, "HEAD"));
        break;
      }

      // ...
      if (0 == ::std::strcmp(request.method, "OPTIONS"))
        (void) socket::headers(client, HTTP_NO_CONTENT, "No Content", "text/plain", size, "Allow: GET, HEAD, OPTIONS, TRACE" "\r\n");

      else if (0 == ::std::strcmp(request.method, "TRACE")) /* ->> Strip credential headers from reflected requests */ {
        struct string echo;
        bool          success = true;

        // ...
        if (success) success = echo.append(request.method);
        if (success) success = echo.append(' ');
        if (success) success = echo.append(request.target);
        if (success) success = echo.append(' ');
        if (success) success = echo.append(request.version);
        if (success) success = echo.append("\r\n");

        for (char *current = request.lineEnd + 1, *const end = &request.memory[headerEnd]; success and current < end and *current != '\r' and *current != '\n'; ) {
          char const *const blocks[]  = {"authorization" ":", "cookie" ":", "proxy-authorization" ":", "proxy-cookie" ":", "set-cookie" ":"};
          char *const       next      = static_cast<char*>(::std::memchr(current, '\n', static_cast< ::std::size_t>(end - current)));
          char              saved     = '\0';
          bool              sensitive = false;
          char             *stop      = next;

          // ...
          if (NULL == next)
          break;

          stop -= current < stop and stop[-1] == '\r';
          saved = *stop;
          *stop = '\0';

          for (char const *const *block = blocks; block != &blocks[sizeof blocks / sizeof(char const*)]; ++block) {
            ::std::size_t       match = 0u;
            ::std::size_t const width = ::std::strlen(*block);

            // ...
            if (width <= ::std::strlen(current)) {
              while (match != width and (*block)[match] == ::std::tolower(static_cast<unsigned char>(current[match]))) match++;
              if    (match == width)                                                                                   sensitive = true;
            }
          }

          success = sensitive ? success : echo.append(current) and echo.append("\r\n");
          *stop   = saved;
          current = next + 1;
        }

        if (success and echo.append("\r\n")) {
          size = echo.length;

          if (socket::headers(client, HTTP_OK, "OK", "message/http", size, ""))
          (void) socket::send(client, echo.value, echo.length);
        }
      }

      else {
        struct string       canonical;
        struct string       decoded; // ->> Decode and normalize path segments before filesystem access
        char const   *const delimiter = ::std::strpbrk(request.target, "?#");
        char const         *mime      = "application/octet-stream";
        struct string       path;
        ::std::FILE        *stream = NULL;
        struct string       uri;
        bool                resolved = true;

        // ...
        do {
          if (uri.append(request.target, NULL != delimiter ? static_cast< ::std::size_t>(delimiter - request.target) : ::std::strlen(request.target)))
          break;

          if (NULL == uri.value or '/' != *uri.value or not canonical.set("/") or not path.set(root.value))
          resolved = false;

          for (::std::size_t index = 0u; resolved and '\0' != uri.value[index]; ++index) {
            signed char   digits[2] = {-1, -1};
            unsigned char value     = static_cast<unsigned char>(uri.value[index]);

            // ...
            if ('%' == value) {
              for (unsigned char subindex = 0u; subindex != sizeof digits / sizeof(signed char); ++subindex) {
                char const character = uri.value[index + subindex + 1u];

                // ...
                if ('\0' == character) {
                  resolved = false;
                  break;
                }

                if      (character >= '0' and character <= '9') digits[subindex] = character - '0';
                else if (character >= 'A' and character <= 'F') digits[subindex] = character - 'A' + 10;
                else if (character >= 'a' and character <= 'f') digits[subindex] = character - 'a' + 10;

                if (digits[subindex] < 0) {
                  resolved = false;
                  break;
                }
              }

              if (not resolved)
              break;

              index += 2u;
              value  = static_cast<unsigned char>((digits[0] << 4u) | (digits[1] << 0u));
            }

            if (ASCII_DELETE == value or ASCII_SPACE > value or '\0' == value or '\\' == value or not decoded.append(static_cast<char>(value))) {
              resolved = false;
              break;
            }
          }

          for (::std::size_t index = 0u; resolved and decoded.length > index; ) {
            char const *segment = "";
            char        separator;

            // ...
            while (decoded.length != index and decoded.value[index] == '/') ++index;
            if    (decoded.length == index) break;

            segment = &decoded.value[index];

            while (decoded.length != index and decoded.value[index] != '/') ++index;
            separator            = decoded.value[index];
            decoded.value[index] = '\0';

            if (0 == ::std::strcmp(segment, "..")) {
              /* ... */ {
                while (path.length > root.length and path.value[path.length - 1u] != '/' and path.value[path.length - 1u] != '\\') --path.length;
                if    (path.length > root.length)                                                                                  --path.length;
                path.value[path.length] = '\0';
              }

              if (canonical.length > 1u) {
                while (canonical.length > 1u and canonical.value[canonical.length - 1u] != '/') --canonical.length;
                if    (canonical.length > 1u)                                                   --canonical.length;
                canonical.value[canonical.length] = '\0';
              }
            }

            else if (0 != ::std::strcmp(segment, ".") and '\0' != *segment) {
              #if defined _WIN32
                ::std::size_t const length = ::std::strlen(segment);
                if (resolved) resolved = segment[length - 1u] != '.' and segment[length - 1u] != ' ' and NULL == ::std::strpbrk(segment, ":*?\"<>|");
              #endif

              if (resolved and 0u != path.length and '/' != path.value[path.length - 1u] and '\\' != path.value[path.length - 1u]) resolved = path     .append   ('/');
              if (resolved)                                                                                                        resolved = path     .append   (segment);
              if (resolved and 1u < canonical.length)                                                                              resolved = canonical.append   ('/');
              if (resolved)                                                                                                        resolved = canonical.appendURL(segment);
            }

            decoded.value[index] = separator;
          }

          if (resolved and decoded.length and decoded.value[decoded.length - 1u] == '/' and canonical.value[canonical.length - 1u] != '/')
          resolved = canonical.append('/');

          if (not resolved) {
            socket::sendError(client, HTTP_BAD_REQUEST, "Bad Request", 0 == ::std::strcmp(request.method, "HEAD"));
            break;
          }

          // ...
          if (directory::isDirectory(path.value)) {
            struct string     candidate; // ->> `index.html` takes precedence over `index.htm`.
            char const* const indexFiles[] = {"index.html", "index.htm"};
            bool              success      = true;

            // ...
            if (uri.length and uri.value[uri.length - 1u] != '/') {
              struct string document, extra;
              struct string location;

              // ...
              success = location.append(canonical.value);
              if (success and 0u != location.length and '/' != location.value[location.length - 1u]) success = location.append    ('/');
              if (success and NULL != delimiter)                                                     success = location.append    (delimiter);
              if (success)                                                                           success = document.append    ("<!DOCTYPE html>" "<html><body><a href=\"");
              if (success)                                                                           success = document.appendHTML(location.value);
              if (success)                                                                           success = document.append    ("\">Moved Permanently</a></body></html>" "\r\n");
              if (success)                                                                           success = extra   .append    ("Location: ");
              if (success)                                                                           success = extra   .append    (location.value);
              if (success)                                                                           success = extra   .append    ("\r\n");

              if (success)
              socket::sendDocument(client, HTTP_REDIRECT, "Moved Permanently", &document, 0 == ::std::strcmp(request.method, "HEAD"), extra.value);

              break;
            }

            for (char const *const *indexFile = indexFiles; success and indexFile != &indexFiles[sizeof indexFiles / sizeof(char const*)]; ++indexFile) {
              success = candidate.set(path.value);

              if (success and 0u != candidate.length and '/' != candidate.value[candidate.length - 1u] and '\\' != candidate.value[candidate.length - 1u]) success = candidate.append('/');
              if (success)                                                                                                                                 success = candidate.append(*indexFile);

              if (success and directory::isFile(candidate.value, NULL)) {
                success = path.set(candidate.value);
                break;
              }
            }

            if (not success)
            break;

            if (directory::isDirectory(path.value)) {
              ::std::size_t     count = 0u;
              struct string     document;
              struct directory *entries = NULL;
              bool              listed  = true;

              // ... ->> Enumerate directly; partial allocations are freed before any exit
              #if defined _WIN32
                ::HANDLE           search = INVALID_HANDLE_VALUE;
                ::WIN32_FIND_DATAW entry  = {};

                // ...
                listed = false;

                if (wchar_t *widepath = string::raw(path.value)) {
                  ::std::size_t length = ::std::wcslen(widepath);

                  // ...
                  if (void *const extension = ::std::realloc(widepath, (length + 3u) * sizeof(wchar_t))) {
                    widepath = static_cast<wchar_t*>(extension);

                    if (length and widepath[length - 1u] != L'\\' and widepath[length - 1u] != L'/')
                      widepath[length++] = L'\\';

                    widepath[length++] = L'*';
                    widepath[length++] = L'\0';
                    search             = ::FindFirstFileW(widepath, &entry);
                    listed             = INVALID_HANDLE_VALUE != search;
                  }

                  ::std::free(widepath);
                }
              #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
                struct ::dirent *entry  = NULL;
                ::DIR *const     search = ::opendir(path.value);

                listed = NULL != search;
              #endif

              while (listed) {
                bool  isDirectory = false;
                char *name        = NULL;
                long  length      = 0;

                // ...
                #if defined _WIN32
                  isDirectory = 0u != (entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
                  length      = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, entry.cFileName, -1, NULL, 0, NULL, NULL);

                  if (length > 0) {
                    name = static_cast<char*>(::std::malloc(length * sizeof(char)));

                    if (NULL != name and 0 == ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, entry.cFileName, -1, name, length, NULL, NULL)) {
                      ::std::free(name);
                      name = NULL;
                    }
                  }
                #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
                  entry  = ::readdir(search);
                  length = NULL != entry ? ::std::strlen(entry -> d_name) : -1;

                  if (length > 0) {
                    name = static_cast<char*>(::std::malloc((length + 1u) * sizeof(char)));

                    if (NULL != name) {
                      struct string directory;

                      if (listed)                                                                        listed      = directory.append(path.value);
                      if (listed and directory.length and directory.value[directory.length - 1u] != '/') listed      = directory.append('/');
                      if (listed)                                                                        listed      = directory.append(::std::memcpy(name, entry -> d_name, length + 1u));
                      if (listed)                                                                        isDirectory = directory::isDirectory(directory.value);
                    }
                  }
                #endif

                if      (SIZE_MAX / sizeof(struct directory) == count or not listed or NULL == name) { ::std::free(name); listed = false; break; }
                else if (0 == ::std::strcmp(name, ".") or 0 == ::std::strcmp(name, ".."))            { ::std::free(name); }
                else if (void *const allocation = ::std::realloc(entries, (count + 1u) * sizeof(struct directory))) {
                  entries                  = static_cast<struct directory*>(allocation);
                  entries[count].name      = name;
                  entries[count].directory = isDirectory;
                  count++;
                } else { ::std::free(name); listed = false; break; }

                #if defined _WIN32
                  if (FALSE == ::FindNextFileW(search, &entry))
                  break;
                #endif
              }

              #if defined _WIN32
                if (INVALID_HANDLE_VALUE != search)
                (void) ::FindClose(search);
              #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
                if (NULL != search)
                (void) ::closedir(search);
              #endif

              if (not listed) {
                while (count)
                  ::std::free(entries[--count].name);

                ::std::free(entries);
                socket::sendError(client, HTTP_FORBIDDEN, "Forbidden", 0 == ::std::strcmp(request.method, "HEAD"));

                entries = NULL;
                break;
              }

              if (success) success = document.append    ("<!DOCTYPE html>" "<html><head><meta charset=\"UTF-8\"/><title>Directory listing for ");
              if (success) success = document.appendHTML(uri.value);
              if (success) success = document.append    ("</title></head><body><h1>Directory listing for ");
              if (success) success = document.appendHTML(uri.value);
              if (success) success = document.append    ("</h1><hr><ul>" "\r\n");

              for (::std::size_t index = 0u; index < count and success; ++index) {
                if (success)                              success = document.append    ("<li><a href=\"");
                if (success)                              success = document.appendURL (entries[index].name);
                if (success and entries[index].directory) success = document.append    ('/');
                if (success)                              success = document.append    ("\">");
                if (success)                              success = document.appendHTML(entries[index].name);
                if (success and entries[index].directory) success = document.append    ('/');
                if (success)                              success = document.append    ("</a></li>" "\r\n");
              }

              while (count)
                ::std::free(entries[--count].name);
              ::std::free(entries);

              if (success and document.append("</ul><hr></body></html>" "\r\n"))
              socket::sendDocument(client, HTTP_OK, "OK", &document, 0 == ::std::strcmp(request.method, "HEAD"), "");

              break;
            }
          }

          if (not directory::isFile(path.value, &size)) {
            socket::sendError(client, HTTP_NOT_FOUND, "File Not Found", 0 == ::std::strcmp(request.method, "HEAD"));
            break;
          }

          // ...
          #if defined _WIN32
            if (wchar_t *const widepath = string::raw(path.value)) {
              stream = ::_wfopen(widepath, L"rb");
              ::std::free(widepath);
            }
          #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
            stream = ::std::fopen(path.value, "rb");
          #endif

          if (NULL == stream) {
            socket::sendError(client, HTTP_FORBIDDEN, "Forbidden", 0 == ::std::strcmp(request.method, "HEAD"));
            break;
          }

          if (char const *const extension = ::std::strrchr(path.value, '.'))
          for (struct format /* final */ { char const extension[7], *type /* [31] */; } const formats[] = {
            {".avif", "image/avif"}, {".bmp", "image/bmp"},
            {".cpp",  "text/plain"}, {".css", "text/css; charset=utf-8"}, {".csv", "text/csv"},
            {".gif",  "image/gif"},
            {".h",    "text/plain"}, {".htm", "text/html; charset=utf-8"}, {".html", "text/html; charset=utf-8"},
            {".ico",  "image/x-icon"},
            {".jpeg", "image/jpeg"}, {".jpg", "image/jpeg"}, {".js", "text/javascript; charset=utf-8"}, {".json", "application/json; charset=utf-8"},
            {".log",  "text/plain"},
            {".map",  "application/json"}, {".mjs", "text/javascript; charset=utf-8"}, {".mp3", "audio/mpeg"}, {".mp4", "video/mp4"},
            {".otf",  "font/otf"},
            {".pdf",  "application/pdf"},  {".png",  "image/png"}, {".py", "text/plain"},
            {".svg",  "image/svg+xml"},    {".txt",  "text/plain; charset=utf-8"},
            {".wasm", "application/wasm"}, {".webp", "image/webp"}, {".woff", "font/woff"}, {".woff2", "font/woff2"},
            {".xml",  "application/xml"},  {".zip",   "application/zip"}
          }, *format = formats; format != &formats[sizeof formats / sizeof(struct format)]; ++format) {
            ::std::size_t index = 0u;

            // ...
            for (; '\0' != extension[index] and '\0' != format -> extension[index]; ++index) {
              if (format -> extension[index] != ::std::tolower(static_cast<unsigned char>(extension[index])))
              break;
            }

            if ('\0' == extension[index] and '\0' == format -> extension[index])
            mime = format -> type;
          }

          if (socket::headers(client, HTTP_OK, "OK", mime, size, "") and not (0 == ::std::strcmp(request.method, "HEAD")))
          for (unsigned char payload[16384]; ; ) {
            ::std::size_t const received = ::std::fread(payload, 1u, sizeof payload, stream);

            if (not received or not socket::send(client, reinterpret_cast<char const*>(payload), received))
            break;
          }

          (void) ::std::fclose(stream);
        } while (false);
      }
    } while (false);

    socket::close(client);
    socket::workerEnd();

    return static_cast<thread_t>(0);
  }
};

/* Main -|> Consider dynamic API support (e.g. `api/status`), PHP CGI, or SQL database clients */
static int comain(int const count, char const* const arguments[]) /* noexcept */ {
  bool help     = false;
  bool shutdown = false;
  int  status   = EXIT_SUCCESS;

  // ...
  #if defined _WIN32
    if (not root.set("C:\\LapysDev"))
    return EXIT_FAILURE;
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    if (not root.set("."))
    return EXIT_FAILURE;
  #endif

  // ->> Parse before initializing sockets: Invalid command lines do not launch the server
  for (int index = 1; count > index; ++index) {
    char const *const option = arguments[index];

    // ...
    if (0 == ::std::strcmp(option, "--shutdown")) {
      if (count != 2)
      return INVALID_COMMAND_LINE;

      shutdown = true;
      break;
    }

    else if (0 == ::std::strcmp(option, "--root") and count > index + 1) {
      if (not root.set(arguments[++index]))
      return EXIT_FAILURE;
    }

    else if (0 == ::std::strcmp(option, "--port") and count > index + 1) {
      ::uint_least16_t number = 0000u;

      // ...
      if ('\0' == arguments[++index][0])
      return INVALID_COMMAND_LINE;

      for (char const *value = arguments[index]; '\0' != *value; ++value) {
        number = (number * 10u) + static_cast< ::uint_least16_t>(*value - '0');

        if (PORT_MAXIMUM < number or not (*value >= '0' and *value <= '9'))
        return INVALID_COMMAND_LINE;
      }

      if (0000u == number)
      return INVALID_COMMAND_LINE;

      port = static_cast< ::uint_least16_t>(number);
    }

    else if (0 == ::std::strcmp(option, "--lan"))
      bindIPv4 = INADDR_ANY; // ->> All IPv4 interfaces; restrict access using the host firewall

    else if (0 == ::std::strcmp(option, "--help") or 0 == ::std::strcmp(option, "-h")) {
      if (count != 2)
      return INVALID_COMMAND_LINE;

      help = true;
      break;
    }

    else if (0 == ::std::strcmp(option, "--bind") and index + 1 < count) {
      char const     *address   = arguments[++index];
      bool            hasDigit  = false;
      ::uint_least8_t octets[4] = {0u, 0u, 0u, 0u};
      unsigned char   part      = 0u; /* 0–3 */

      // ...
      for (; '\0' != *address; ++address) {
        if (*address >= '0' and *address <= '9') {
          ::uint_least8_t const number = (octets[part] * 10uL) + static_cast< ::uint_least8_t>(*address - '0');

          // ...
          if (number > 255u)
          return INVALID_COMMAND_LINE;

          hasDigit     = true;
          octets[part] = static_cast< ::uint_least8_t>(number);
        }

        else if ('.' == *address and hasDigit and part < 3u) {
          hasDigit = false;
          part++;
        }

        else
        return INVALID_COMMAND_LINE;
      }

      if (not hasDigit or part != 3u)
      return INVALID_COMMAND_LINE;

      bindIPv4 = (static_cast< ::uint_least32_t>(octets[0]) << (8u * 3u)) | (static_cast< ::uint_least32_t>(octets[1]) << (8u * 2u)) | (static_cast< ::uint_least32_t>(octets[2]) << (8u * 1u)) | (static_cast< ::uint_least32_t>(octets[3]) << (8u * 0u));
    }

    else
    return INVALID_COMMAND_LINE;
  }

  // ->> Received HTTP bytes are not converted through the process locale.
  if (help or shutdown) {
    char const *message = NULL;

    // ->> Continue only while each filesystem and conversion operation succeeds.
    if (shutdown) {
      status = EXIT_FAILURE;

      #if defined _WIN32
        if (::HANDLE const event = ::OpenEventW(EVENT_MODIFY_STATE, FALSE, L"Local\\LapysDev.Localhost.Shutdown")) {
          status = 0 != ::SetEvent(event) ? EXIT_SUCCESS : status;
          (void) ::CloseHandle(event);
        }
      #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
        struct string name;

        if (name.appendControlName()) {
          struct ::sockaddr_un address = {};
          socket_t const       client  = ::socket(AF_UNIX, SOCK_STREAM, 0);

          if (INVALID_SOCKET_HANDLE != client) {
            bool success = false;

            // ...
            (void) ::std::memcpy(address.sun_path, name.value, name.length + 1u);
            address.sun_family = AF_UNIX;
            success            = 0 == ::connect(client, reinterpret_cast<struct ::sockaddr*>(&address), sizeof address) and socket::send(client, "S", 1u);

            if (success) {
              char                   confirmation = '\0';
              struct ::timeval const timeout      = {TIMEOUT_CONTROL_SECONDS, 0u};

              // ...
              (void) ::setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
              success = 1 == ::recv(client, &confirmation, 1, 0) and 'K' == confirmation;
            }

            socket::close(client);
            status = success ? EXIT_SUCCESS : status;
          }
        }
      #endif

      message = EXIT_SUCCESS == status ? "[.]: Localhost shutdown requested." "\r\n" : "[.]: Localhost is not running." "\r\n";
    } else message = "[?]: localhost [--bind IPv4 | --lan] [--port 1..65535] [--root \"DIRECTORY\"] | --shutdown | --help" "\r\n";

    #if defined _WIN32
      ::BOOL   const consoleAttached = ::AttachConsole(ATTACH_PARENT_PROCESS);
      ::HANDLE const stream          = ::GetStdHandle(STD_OUTPUT_HANDLE);

      // ...
      if (INVALID_HANDLE_VALUE != stream and NULL != stream)
      (void) ::WriteFile(stream, message, static_cast< ::DWORD>(::std::strlen(message)), &reinterpret_cast< ::DWORD&>(const_cast< ::ULARGE_INTEGER&>(static_cast< ::ULARGE_INTEGER const&>(::ULARGE_INTEGER()))), NULL);

      if (FALSE != consoleAttached)
      (void) ::FreeConsole(); // ->> The Windows-subsystem executable temporarily borrows the calling shell’s console only to report command results
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      (void) ::std::fputs(message, stdout);
    #endif

    return status;
  }

  if (not directory::isDirectory(root.value))
  return EXIT_FAILURE;

  #if defined _WIN32
    ::WSADATA windowsSockets = {};

    if (0 != ::WSAStartup(MAKEWORD(2, 2), &windowsSockets))
    return EXIT_FAILURE;
  #endif

  // ->> The control endpoint enforces single-instance startup and private shutdown.
  bool controlStarted = false;
  #if defined _WIN32
    localhostMutex = ::CreateMutexW(NULL, FALSE, L"Local\\LapysDev.Localhost.Instance");

    if (NULL != localhostMutex) {
      localhostShutdown = ERROR_ALREADY_EXISTS != ::GetLastError() ? ::CreateEventW(NULL, TRUE, FALSE, L"Local\\LapysDev.Localhost.Shutdown") : localhostShutdown;
      controlStarted    = NULL != localhostShutdown;

      if (not controlStarted) {
        (void) ::CloseHandle(localhostMutex);
        localhostMutex = NULL;
      }
    }
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    struct string name;

    if (name.appendControlName()) {
      struct ::sockaddr_un address = {};
      int                  bound   = static_cast<int>(false);
      ::mode_t             mask    = 0000;

      // ...
      address.sun_family = AF_UNIX;
      (void) ::std::memcpy(controlPath,      name.value,  name.length + 1u);
      (void) ::std::memcpy(address.sun_path, controlPath, name.length + 1u);
      control = ::socket(AF_UNIX, SOCK_STREAM, 0);

      if (INVALID_SOCKET_HANDLE != control) {
        mask  = ::umask(0077u);
        bound = ::bind(control, reinterpret_cast<struct ::sockaddr*>(&address), sizeof address);

        if (0 != bound and EADDRINUSE == errno) {
          socket_t const probe   = ::socket(AF_UNIX, SOCK_STREAM, 0);
          bool     const running = INVALID_SOCKET_HANDLE != probe and 0 == ::connect(probe, reinterpret_cast<struct ::sockaddr*>(&address), sizeof address);

          // ...
          socket::close(probe);

          if (not running) {
            struct ::stat entry = {};
            if (0 == ::lstat(controlPath, &entry) and S_ISSOCK(entry.st_mode) and entry.st_uid == ::geteuid()) {
              (void) ::unlink(controlPath);
              bound = ::bind(control, reinterpret_cast<struct ::sockaddr*>(&address), sizeof address);
            }
          }
        }

        (void) ::umask(mask);

        if (0 == bound and 0 == ::listen(control, CONTROL_BACKLOG))
          controlStarted = true;

        else {
          socket::close(control);
          control = INVALID_SOCKET_HANDLE;
        }
      }

      if (not controlStarted)
      controlPath[0] = '\0';
    }
  #endif

  if (not controlStarted) {
    #if defined _WIN32
      (void) ::WSACleanup();
    #endif
    return EXIT_FAILURE;
  }

  listeners[0] = socket::listen(4u);
  listeners[1] = socket::listen(6u);

  if (INVALID_SOCKET_HANDLE == listeners[0] or INVALID_SOCKET_HANDLE == listeners[1])
    status = EXIT_FAILURE;

  else {
    status = EXIT_FAILURE;

    #if defined __ANDROID__ or defined __APPLE__ or defined __unix__
      struct /* final */ {
        struct ::sigaction action;
        struct ::sigaction ignored;

        static void stop(int) /* noexcept */ {
          localhostShutdown = static_cast< ::sig_atomic_t>(true);
        }
      } signal = {};

      // ...
      signal.action.sa_handler = &signal.stop;
      (void) ::sigemptyset(&signal.action.sa_mask);
      (void) ::sigaction  (SIGINT,  &signal.action, NULL);
      (void) ::sigaction  (SIGTERM, &signal.action, NULL);

      signal.ignored.sa_handler = SIG_IGN;
      (void) ::sigemptyset(&signal.ignored.sa_mask);
      (void) ::sigaction  (SIGPIPE, &signal.ignored, NULL);
    #endif

    while (
      #if defined _WIN32
        WAIT_OBJECT_0 != ::WaitForSingleObject(localhostShutdown, 0u)
      #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
        static_cast< ::sig_atomic_t>(false) == localhostShutdown
      #endif
    ) {
      ::fd_set         incoming;
      struct ::timeval timeout = {0, TIMEOUT_SELECT_MICROSECONDS};

      // ...
      FD_ZERO(&incoming);
      FD_SET (listeners[0], &incoming);
      FD_SET (listeners[1], &incoming);
      #if defined __ANDROID__ or defined __APPLE__ or defined __unix__
        FD_SET(control, &incoming);
      #endif

      #if defined _WIN32
        int const selected = ::select(0, &incoming, NULL, NULL, &timeout);
      #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
        int       largest  = listeners[0] > listeners[1] ? listeners[0] : listeners[1];
        int const selected = ::select((control > largest ? control : largest) + 1, &incoming, NULL, NULL, &timeout);
      #endif

      if (selected <= 0)
      continue;

      #if defined __ANDROID__ or defined __APPLE__ or defined __unix__
        if (FD_ISSET(control, &incoming)) {
          socket_t client = ::accept(control, NULL, NULL);

          if (INVALID_SOCKET_HANDLE != client) {
            char                   command        = '\0';
            struct ::timeval const controlTimeout = {0, TIMEOUT_CONTROL_MICROSECONDS};

            // ...
            (void) ::setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &controlTimeout, sizeof controlTimeout);

            if (::recv(client, &command, 1, 0) == 1 and command == 'S') {
              (void) socket::send(client, "K", 1u);
              localhostShutdown = static_cast< ::sig_atomic_t>(true);
            }

            socket::close(client);
          }
        }

        if (localhostShutdown)
        break;
      #endif

      for (unsigned char index = 0u; index != sizeof listeners / sizeof(socket_t); ++index)
      if (FD_ISSET(listeners[index], &incoming)) {
        socket_t const client = ::accept(listeners[index], NULL, NULL);

        if (INVALID_SOCKET_HANDLE != client) {
          #if defined _WIN32
            bool   accepted = false;
            ::LONG current  = 0L;

            // ...
            do {
              current = ::InterlockedCompareExchange(&workerCount, 0L, 0L);
              if (WORKER_MAXIMUM <= current) break;
              accepted = current == ::InterlockedCompareExchange(&workerCount, current + 1L, current);
            } while (not accepted);

            if (accepted) {
              if (::HANDLE const handle = reinterpret_cast< ::HANDLE>(::_beginthreadex(NULL, 0u, &thread::client, reinterpret_cast<void*>(static_cast< ::UINT_PTR>(client)), 0u, NULL)))
                (void) ::CloseHandle(handle);
              else {
                socket::close(client);
                socket::workerEnd();
              }
            } else socket::close(client);
          #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
            bool accepted = false;
            bool launched = false;

            // ...
            (void) ::pthread_mutex_lock(&workerMutex);
              accepted     = WORKER_MAXIMUM > workerCount;
              workerCount += accepted;
            (void) ::pthread_mutex_unlock(&workerMutex);

            if (accepted) {
              if (socket_t *const parameter = static_cast<socket_t*>(::std::malloc(sizeof(socket_t)))) {
                ::pthread_t created;

                // ...
                *parameter = client;

                if (0 == ::pthread_create(&created, NULL, &thread::client, parameter)) {
                  launched = true;
                  (void) ::pthread_detach(created);
                } else ::std::free(parameter);
              }

              if (not launched) {
                socket::close(client);
                socket::workerEnd();
              }
            } else socket::close(client);
          #endif
        }
      }
    }
  }

  // ->> Listener and control ownership ends here; workers close their own clients.
  for (unsigned char index = 0u; index != sizeof listeners / sizeof(socket_t); ++index) {
    socket::close(listeners[index]);
    listeners[index] = INVALID_SOCKET_HANDLE;
  }

  #if defined _WIN32
    if (NULL != localhostMutex)    { (void) ::CloseHandle(localhostMutex);    localhostMutex    = NULL; }
    if (NULL != localhostShutdown) { (void) ::CloseHandle(localhostShutdown); localhostShutdown = NULL; }
    (void) ::WSACleanup();
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    socket::close(control);
    if ('\0' != *controlPath) (void) ::unlink(controlPath);
    control      = INVALID_SOCKET_HANDLE;
    *controlPath = '\0';
  #endif
  return status;
}

#if defined _WIN32
  int WINAPI wWinMain(::HINSTANCE, ::HINSTANCE, ::LPWSTR, int) {
    int             count         = 0;
    int             status        = INVALID_COMMAND_LINE;
    ::LPWSTR *const wideArguments = ::CommandLineToArgvW(::GetCommandLineW(), &count);

    // ...
    if (count < 1 or NULL == wideArguments)
    return INVALID_COMMAND_LINE;

    if (char **const arguments = static_cast<char**>(::std::calloc(static_cast< ::std::size_t>(count), sizeof(char*)))) {
      bool ready = true;

      // ...
      for (int index = 0; index < count; ++index) {
        int const length = ::WideCharToMultiByte(CP_UTF8, 0u, wideArguments[index], -1, NULL, 0, NULL, NULL);

        // ...
        if (length <= 0) {
          ready = false;
          break;
        }

        arguments[index] = static_cast<char*>(::std::malloc(static_cast< ::std::size_t>(length)));

        if (NULL == arguments[index] or length != ::WideCharToMultiByte(CP_UTF8, 0u, wideArguments[index], -1, arguments[index], length, NULL, NULL)) {
          ready = false;
          break;
        }
      }

      status = ready ? comain(count, const_cast<char const* const*>(arguments)) : status;

      while (count)
        ::std::free(arguments[--count]);
      ::std::free(arguments);
    }

    (void) ::LocalFree(wideArguments);
    return status;
  }
#elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
  int main(int count, char* arguments[]) {
    return comain(count, const_cast<char const* const*>(arguments));
  }
#endif
