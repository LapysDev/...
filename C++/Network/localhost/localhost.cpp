/* POSIX:      clang++ -std=c++98 -pedantic -O2 -Wall -Wextra -pthread localhost.cpp -o localhost */
/* Windows:    rc /nologo /folocalhost.res localhost.rc && cl /nologo /utf-8 /EHsc /O2 /DUNICODE /D_UNICODE localhost.cpp localhost.res /Fe:localhost.exe /link /SUBSYSTEM:WINDOWS ws2_32.lib shell32.lib user32.lib kernel32.lib && del /q localhost.obj localhost.res */
/* PowerShell: New-NetFirewallRule -Action Allow -Direction Inbound -DisplayName "LapysDev Server" -LocalPort 8000 -Profile Private -Program (Resolve-Path "C:\LapysDev\tools\localhost.exe").Path -Protocol TCP -RemoteAddress LocalSubnet4 */
/* localhost [--bind 127.0.0.1 | --lan] [--help] [--port 8000] [--root "C:/LapysDev"] [--shutdown] */
#include <cctype>   // --> ::std::tolower(…)
#include <cerrno>   // --> EADDRINUSE, EINTR; errno
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
extern "C" __declspec(dllimport) int       WINAPI WideCharToMultiByte (::UINT, ::DWORD, ::LPCWCH, int, ::LPSTR, int, ::LPCCH, ::LPBOOL);800
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
  #error [!]: Unsupported platform
#endif

/* ... */
struct directory /* final */ {
  char *name; // ->> Directory names are stored as UTF-8 bytes on Windows and native bytes on POSIX
  bool  directory;
};

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

  inline void clear() /* noexcept */ {
    this -> length = 0u;
    if (NULL != this -> value) this -> value[0] = '\0';
  }

  inline bool set(char const text[]) /* noexcept */ {
    this -> clear();
    return this -> append(text);
  }
};

/* ... */
enum server_limit {
  ASCII_DELETE                 = 0x7Fu,
  ASCII_SPACE                  = 0x20u,
  CONTROL_BACKLOG              = 4u,
  CONTROL_NAME_MAXIMUM_LENGTH  = 80u,
  HEADER_MAXIMUM_LENGTH        = 16384u,
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

enum http_status /* : ::uint_least16_t : 9 */ {
  HTTP_BAD_REQUEST         = 400u,
  HTTP_FORBIDDEN           = 403u,
  HTTP_METHOD_NOT_ALLOWED  = 405u,
  HTTP_NO_CONTENT          = 204u,
  HTTP_NOT_FOUND           = 404u,
  HTTP_OK                  = 200u,
  HTTP_REDIRECT            = 301u,
  HTTP_REQUEST_TOO_LARGE   = 431u,
  HTTP_REQUEST_TOO_LONG    = 414u,
  HTTP_UNSUPPORTED_METHOD  = 501u,
  HTTP_UNSUPPORTED_VERSION = 505u
};

#if defined _WIN32
  typedef ::SOCKET socket_t; // --> WinSock handle

  static int      const INVALID_COMMAND_LINE  = EXIT_FAILURE + 1;
  static socket_t const INVALID_SOCKET_HANDLE = INVALID_SOCKET;

  static ::HANDLE        localhostMutex    = NULL;
  static ::HANDLE        localhostShutdown = NULL;
  static ::LONG volatile workerCount       = 0L;
#elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
  typedef int socket_t; // --> POSIX descriptor

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
static struct string    root;

extern "C" {
  static bool         append_html        (struct string*, char const[])                                                     /* noexcept */;
  static bool         append_url         (struct string*, char const[])                                                     /* noexcept */;
  static void         close_control      ()                                                                                 /* noexcept */;
  static void         close_socket       (socket_t)                                                                         /* noexcept */;
  static void         finish             ()                                                                                 /* noexcept */;
  static void         handle_request     (socket_t)                                                                         /* noexcept */;
  static bool         is_directory       (char const[])                                                                     /* noexcept */;
  static bool         is_static_file     (char const[], void*)                                                              /* noexcept */;
  static bool         list_directory     (char const[], struct directory*[], ::std::size_t*)                                /* noexcept */;
  static char const*  mime_type          (char const[])                                                                     /* noexcept */;
  static ::std::FILE* open_file          (char const[])                                                                     /* noexcept */;
  static socket_t     open_listener      (unsigned char const)                                                              /* noexcept */;
  static int          request_shutdown   ()                                                                                 /* noexcept */;
  static bool         resolve_path       (char const[], struct string*, struct string*)                                     /* noexcept */;
  static bool         send_all           (socket_t, char const[], ::std::size_t)                                            /* noexcept */;
  static void         send_document      (socket_t, unsigned, char const[], struct string const*, bool const, char const[]) /* noexcept */;
  static void         send_error         (socket_t, unsigned, char const[], bool const)                                     /* noexcept */;
  static void         set_client_timeout (socket_t)                                                                         /* noexcept */;
  static void         start_client       (socket_t)                                                                         /* noexcept */;
  static bool         start_control      ()                                                                                 /* noexcept */;
  static void         worker_end         ()                                                                                 /* noexcept */;
  #if defined _WIN32
    static bool     response_headers(socket_t, unsigned, char const[], char const[], ::ULONGLONG, char const[]) /* noexcept */;
    static wchar_t* widen_path      (char const[])                                                              /* noexcept */;
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    static bool control_name    (struct string*)                                                        /* noexcept */;
    static bool response_headers(socket_t, unsigned, char const[], char const[], ::off_t, char const[]) /* noexcept */;
  #endif
}

/* ... */
static bool append_html(struct string* const html, char const text[]) /* noexcept */ {
  for (::std::size_t index = 0u; '\0' != text[index]; ++index)
  switch (text[index]) {
    case '&':  if (not html -> append("&amp;"))     return false; break;
    case '<':  if (not html -> append("&lt;"))      return false; break;
    case '>':  if (not html -> append("&gt;"))      return false; break;
    case '"':  if (not html -> append("&quot;"))    return false; break;
    case '\'': if (not html -> append("&#x27;"))    return false; break;
    default:   if (not html -> append(text[index])) return false;
  }

  return true;
}

static bool append_url(struct string* const output, char const text[]) /* noexcept */ {
  for (::std::size_t index = 0u; '\0' != text[index]; ++index) {
    unsigned char const character = static_cast<unsigned char>(text[index]);

    // ->> Non-ASCII UTF-8 bytes are percent-encoded byte-by-byte, preserving emoji
    if (character == '-' or character == '_' or character == '.' or character == '~' or (character >= 'a' and character <= 'z') or (character >= 'A' and character <= 'Z') or (character >= '0' and character <= '9')) {
      if (not output -> append(static_cast<char>(character)))
      return false;
    }

    else {
      if (not output -> append('%')) return false;
      if (not output -> append("0123456789ABCDEF"[(character >> 4u) & 0x0Fu])) return false;
      if (not output -> append("0123456789ABCDEF"[(character >> 0u) & 0x0Fu])) return false;
    }
  }

  return true;
}

#if defined _WIN32
  static unsigned __stdcall client_thread(void* const parameter) /* noexcept */ {
    socket_t const client = static_cast<socket_t>(reinterpret_cast< ::UINT_PTR>(parameter));

    // ...
    handle_request(client);
    close_socket(client);
    worker_end();

    return 0u;
  }
#elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
  static void* client_thread(void* const parameter) /* noexcept */ {
    socket_t const client = *static_cast<socket_t*>(parameter);

    // ...
    ::std::free(parameter);
    handle_request(client);
    close_socket(client);
    worker_end();

    return NULL;
  }
#endif

static void close_control() /* noexcept */ {
  #if defined _WIN32
    if (NULL != localhostMutex)    { (void) ::CloseHandle(localhostMutex);    localhostMutex    = NULL; }
    if (NULL != localhostShutdown) { (void) ::CloseHandle(localhostShutdown); localhostShutdown = NULL; }
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    close_socket(control);
    if ('\0' != *controlPath) (void) ::unlink(controlPath);

    control      = INVALID_SOCKET_HANDLE;
    *controlPath = '\0';
  #endif
}

static void close_socket(socket_t const socket) /* noexcept */ {
  if (INVALID_SOCKET_HANDLE != socket)
  #if defined _WIN32
    (void) ::closesocket(socket);
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    (void) ::close(socket);
  #endif
}

#if defined __ANDROID__ or defined __APPLE__ or defined __unix__
  static bool control_name(struct string* const name) /* noexcept */ {
    struct ::stat attributes = {};
    char const   *directory  = ::std::getenv("XDG_RUNTIME_DIR");
    char          suffix[CONTROL_NAME_MAXIMUM_LENGTH];

    // ... ->> An owner-specific local-domain socket lets a second instance request shutdown
    directory = NULL == directory or '\0' == *directory or 0 != ::stat(directory, &attributes) or not S_ISDIR(attributes.st_mode) or 0 != ::access(directory, W_OK | X_OK) ? "/tmp" : directory; // ->> Runtime directory is preferred with `/tmp` as fallback
    (void) ::std::sprintf(suffix, "lapysdev-localhost-%lu.sock", static_cast<unsigned long>(::getuid()));

    if (not name -> append(directory))                                                                     return false;
    if (0u != name -> length and '/' != name -> value[name -> length - 1u]) { if (not name -> append('/')) return false; }
    if (not name -> append(suffix))                                                                        return false;

    return name -> length < sizeof controlPath;
  }
#endif

static void finish() /* noexcept */ {
  for (unsigned char index = 0u; index != sizeof listeners / sizeof(socket_t); ++index) {
    close_socket(listeners[index]);
    listeners[index] = INVALID_SOCKET_HANDLE;
  }

  close_control();

  #if defined _WIN32
    (void) ::WSACleanup();
  #endif
}

static void handle_request(socket_t const client) /* noexcept */ {
  #if defined _WIN32
    ::ULONGLONG size = 0u;
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    ::off_t size = 0;
  #endif
  struct /* final */ {
    ::std::size_t length;
    char          memory[HEADER_MAXIMUM_LENGTH + 1u];
    char         *lineEnd;
    char const   *method;
    char         *spaces[2];
    char const   *target;
    char const   *version;
  }             request   = {0u, {}, NULL, NULL, {NULL, NULL}, NULL, NULL};
  ::std::size_t headerEnd = 0u;

  // Header
  set_client_timeout(client); // ->> One request per connection

  while (HEADER_MAXIMUM_LENGTH > request.length) {
    #if defined _WIN32
      int const received = ::recv(client, &request.memory[request.length], static_cast<int>(HEADER_MAXIMUM_LENGTH - request.length), 0);
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      ::ssize_t const received = ::recv(client, &request.memory[request.length], HEADER_MAXIMUM_LENGTH - request.length, 0);
    #endif

    // ...
    if (received <= 0)
    return;

    request.length                += static_cast< ::std::size_t>(received);
    request.memory[request.length] = '\0';

    if (char *const marker = ::std::strstr(request.memory, "\r\n" "\r\n")) {
      headerEnd = static_cast< ::std::size_t>(marker - request.memory) + 4u;
      break;
    }
    if (char *const marker = ::std::strstr(request.memory, "\n" "\n")) {
      headerEnd = static_cast< ::std::size_t>(marker - request.memory) + 2u;
      break;
    }
  }

  if (0u == headerEnd)
  return send_error(client, HTTP_REQUEST_TOO_LARGE, "Request Header Fields Too Large", false);

  request.lineEnd = ::std::strchr(request.memory, '\n');

  if (NULL == request.lineEnd or REQUEST_LINE_MAXIMUM_LENGTH < request.lineEnd - request.memory)
  return send_error(client, HTTP_REQUEST_TOO_LONG, "URI Too Long", false);

  request.spaces[0]                                                                          =                             ::std::strchr(request.memory,        ' ');
  request.spaces[1]                                                                          = NULL != request.spaces[0] ? ::std::strchr(request.spaces[0] + 1, ' ') : NULL;
  request.lineEnd[request.lineEnd > request.memory and request.lineEnd[-1] == '\r' ? -1 : 0] = '\0';

  if (NULL == request.spaces[0] or NULL == request.spaces[1] or NULL != ::std::strchr(request.spaces[1] + 1, ' '))
  return send_error(client, HTTP_BAD_REQUEST, "Bad Request", false);

  request.method       = request.memory;
  request.spaces[0][0] = '\0';
  request.spaces[1][0] = '\0';
  request.target       = request.spaces[0] + 1;
  request.version      = request.spaces[1] + 1;

  if (0 != ::std::strcmp(request.version, "HTTP/1.0") and 0 != ::std::strcmp(request.version, "HTTP/1.1"))
  return send_error(client, HTTP_UNSUPPORTED_VERSION, "HTTP Version Not Supported", 0 == ::std::strcmp(request.method, "HEAD"));

  if (0 != ::std::strcmp(request.method, "GET") and 0 != ::std::strcmp(request.method, "HEAD") and 0 != ::std::strcmp(request.method, "OPTIONS") and 0 != ::std::strcmp(request.method, "TRACE"))
  return send_error(client, 0 == ::std::strcmp(request.method, "CONNECT") ? HTTP_METHOD_NOT_ALLOWED : HTTP_UNSUPPORTED_METHOD, 0 == ::std::strcmp(request.method, "CONNECT") ? "Method Not Allowed" : "Not Implemented", false); // ->> `CONNECT` needs a bidirectional proxy tunnel, which this static file server does not implement

  for (::std::size_t index = 0u; '\0' != request.target[index]; ++index) {
    if (ASCII_DELETE == request.target[index] or ASCII_SPACE >= static_cast<unsigned char>(request.target[index]))
    return send_error(client, HTTP_BAD_REQUEST, "Bad Request", 0 == ::std::strcmp(request.method, "HEAD"));
  }

  if (request.target[0] != '/' and not (0 == ::std::strcmp(request.method, "OPTIONS") and 0 == ::std::strcmp(request.target, "*")))
  return send_error(client, HTTP_BAD_REQUEST, "Bad Request", 0 == ::std::strcmp(request.method, "HEAD"));

  // Method
  if (0 == ::std::strcmp(request.method, "OPTIONS"))
    (void) response_headers(client, HTTP_NO_CONTENT, "No Content", "text/plain", size, "Allow: GET, HEAD, OPTIONS, TRACE" "\r\n");

  else if (0 == ::std::strcmp(request.method, "TRACE")) /* ->> `TRACE` reflects the request as a message/ HTTP but deliberately removes credential headers */ {
    struct string echo;

    // ...
    if (not echo.append(request.method))  return;
    if (not echo.append(' '))             return;
    if (not echo.append(request.target))  return;
    if (not echo.append(' '))             return;
    if (not echo.append(request.version)) return;
    if (not echo.append("\r\n"))          return;

    for (char *current = request.lineEnd + 1, *const end = &request.memory[headerEnd]; current < end and *current != '\r' and *current != '\n'; ) {
      char const *const blocks[]  = {"authorization" ":", "cookie" ":", "proxy-authorization" ":", "proxy-cookie" ":", "set-cookie" ":"}; // --> ::std::tolower(…)
      char *const       next      = static_cast<char*>(::std::memchr(current, '\n', static_cast< ::std::size_t>(end - current)));
      char              saved     = '\0';
      bool              sensitive = false;
      char             *stop      = next;

      // ...
      if (NULL == next) break;

      stop -= current < stop and stop[-1] == '\r';
      saved = *stop;
      *stop = '\0';

      for (char const *const *block = blocks; block != &blocks[sizeof blocks / sizeof(char const*)]; ++block) {
        ::std::size_t       match = 0u;
        ::std::size_t const width = ::std::strlen(*block);

        // ...
        if (width <= ::std::strlen(current)) {
          while (match != width and (*block)[match] == ::std::tolower(static_cast<unsigned char>(current[match])))
          ++match;

          if (match == width)
          sensitive = true;
        }
      }

      if (false == sensitive) {
        if (not echo.append(current)) return;
        if (not echo.append("\r\n")) return;
      }

      *stop   = saved;
      current = next + 1;
    }

    if (echo.append("\r\n")) {
      #if defined _WIN32
        size = static_cast< ::ULONGLONG>(echo.length);
      #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
        size = static_cast< ::off_t>(echo.length);
      #endif

      if (response_headers(client, HTTP_OK, "OK", "message/http", size, ""))
      (void) send_all(client, echo.value, echo.length);
    }
  }

  else {
    struct string       canonical;
    char const   *const delimiter = ::std::strpbrk(request.target, "?#");
    struct string       path;
    ::std::FILE        *stream = NULL;
    struct string       uri;

    // ...
    if (not uri.append(request.target, delimiter ? static_cast< ::std::size_t>(delimiter - request.target) : ::std::strlen(request.target)))
    return;

    if (not resolve_path(uri.value, &path, &canonical))
    return send_error(client, HTTP_BAD_REQUEST, "Bad Request", 0 == ::std::strcmp(request.method, "HEAD"));

    if (is_directory(path.value)) {
      struct string     candidate; // ->> `index.html` takes precedence over `index.htm` when a directory has both
      char const* const indexFiles[] = {"index.html", "index.htm"};

      // ...
      if (uri.length and uri.value[uri.length - 1u] != '/') {
        struct string document, extra;
        struct string location;

        // ...
        if (not location.append(canonical.value))                                                                  return;
        if (0u != location.length and '/' != location.value[location.length - 1u]) { if (not location.append('/')) return; }
        if (NULL != delimiter and not location.append(delimiter))                                                  return;

        if (not document.append("<!DOCTYPE html>" "<html><body><a href=\""))      return;
        if (not append_html(&document, location.value))                           return;
        if (not document.append("\">Moved Permanently</a></body></html>" "\r\n")) return;

        if (not extra.append("Location: "))   return;
        if (not extra.append(location.value)) return;
        if (not extra.append("\r\n"))         return;

        return send_document(client, HTTP_REDIRECT, "Moved Permanently", &document, 0 == ::std::strcmp(request.method, "HEAD"), extra.value);
      }

      for (char const *const *indexFile = indexFiles; indexFile != &indexFiles[sizeof indexFiles / sizeof(char const*)]; ++indexFile) {
        if (not candidate.set(path.value))                                                                                                                                return;
        if (0u != candidate.length and '/' != candidate.value[candidate.length - 1u] and '\\' != candidate.value[candidate.length - 1u]) { if (not candidate.append('/')) return; }
        if (not candidate.append(*indexFile))                                                                                                                             return;

        if (is_static_file(candidate.value, NULL)) {
          if (not path.set(candidate.value)) return;
          else                               break;
        }
      }

      if (is_directory(path.value)) {
        ::std::size_t     count = 0u;
        struct string     document;
        struct directory *entries = NULL;
        bool              success = true;

        // ...
        if (not list_directory(path.value, &entries, &count))
        return send_error(client, HTTP_FORBIDDEN, "Forbidden", 0 == ::std::strcmp(request.method, "HEAD"));

        if (success) success = document.append("<!DOCTYPE html><html><head><meta charset=\"UTF-8\"/><title>Directory listing for ");
        if (success) success = append_html(&document, uri.value);
        if (success) success = document.append("</title></head><body><h1>Directory listing for ");
        if (success) success = append_html(&document, uri.value);
        if (success) success = document.append("</h1><hr><ul>" "\r\n");

        for (::std::size_t index = 0u; index < count and success; ++index) {
          if (success)                              success = document.append("<li><a href=\"");
          if (success)                              success = append_url(&document, entries[index].name);
          if (success and entries[index].directory) success = document.append('/');
          if (success)                              success = document.append("\">");
          if (success)                              success = append_html(&document, entries[index].name);
          if (success and entries[index].directory) success = document.append('/');
          if (success)                              success = document.append("</a></li>" "\r\n");
        }

        while (count)
          ::std::free(entries[--count].name);
        ::std::free(entries);

        if (success and document.append("</ul><hr></body></html>" "\r\n"))
        return send_document(client, HTTP_OK, "OK", &document, 0 == ::std::strcmp(request.method, "HEAD"), "");
      }
    }

    if (not is_static_file(path.value, &size))
    return send_error(client, HTTP_NOT_FOUND, "File Not Found", 0 == ::std::strcmp(request.method, "HEAD"));

    stream = open_file(path.value);

    if (NULL == stream)
    return send_error(client, HTTP_FORBIDDEN, "Forbidden", 0 == ::std::strcmp(request.method, "HEAD"));

    if (response_headers(client, HTTP_OK, "OK", mime_type(path.value), size, "") and not (0 == ::std::strcmp(request.method, "HEAD"))) {
      unsigned char payload[16384];
      ::std::size_t received = 0u;

      while ((received = ::std::fread(payload, 1u, sizeof payload, stream))) {
        if (not send_all(client, reinterpret_cast<char const*>(payload), received))
        break;
      }
    }

    (void) ::std::fclose(stream);
  }
}

static bool is_directory(char const path[]) /* noexcept */ {
  #if defined _WIN32
    if (wchar_t* const widepath = widen_path(path)) {
      ::DWORD const attributes = ::GetFileAttributesW(widepath);

      ::std::free(widepath);
      return INVALID_FILE_ATTRIBUTES != attributes and 0u != (attributes & FILE_ATTRIBUTE_DIRECTORY);
    }

    return false;
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    struct ::stat attributes = {};
    return ::stat(path, &attributes) == 0 and S_ISDIR(attributes.st_mode);
  #endif
}

static bool is_static_file(char const path[], void* const size) /* noexcept */ {
  #if defined _WIN32
    if (wchar_t* const widepath = widen_path(path)) {
      ::WIN32_FILE_ATTRIBUTE_DATA attributes = {};
      bool const                  found      = 0 != ::GetFileAttributesExW(widepath, GetFileExInfoStandard, &attributes) and 0u == (attributes.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

      // ...
      ::std::free(widepath);
      if (found and NULL != size) *static_cast< ::ULONGLONG*>(size) = (static_cast< ::ULONGLONG>(attributes.nFileSizeHigh) << 32u) | (static_cast< ::ULONGLONG>(attributes.nFileSizeLow) << 0u);

      return found;
    }

    return false;
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    struct ::stat attributes = {};

    // ...
    if (0 == ::stat(path, &attributes) and S_ISREG(attributes.st_mode)) {
      if (NULL != size) *static_cast< ::off_t*>(size) = attributes.st_size;
      return true;
    }

    return false;
  #endif
}

static bool list_directory(char const path[], struct directory* entries[], ::std::size_t* const count) /* noexcept */ {
  bool success = true;
  #if defined _WIN32
    wchar_t           *widepath = widen_path(path);        if (NULL == widepath)                                                                                                                                                  return false;
    ::std::size_t      length   = ::std::wcslen(widepath); if (wchar_t *const wildcard = static_cast<wchar_t*>(::std::realloc(widepath, (length + 3u) * sizeof(wchar_t)))) { widepath = wildcard; } else { ::std::free(widepath); return false; }
    ::HANDLE           search   = NULL;
    ::WIN32_FIND_DATAW entry    = {};

    // ...
    if (0u != length and L'\\' != widepath[length - 1u] and L'/' != widepath[length - 1u]) widepath[length++] = L'\\';
    widepath[length++] = L'*';
    widepath[length++] = L'\0';

    search = ::FindFirstFileW(widepath, &entry);
    ::std::free(widepath);

    if (INVALID_HANDLE_VALUE == search)
    return false;
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    struct ::dirent *entry  = NULL;
    ::DIR *const     search = ::opendir(path);

    // ...
    if (NULL == search)
    return false;
  #endif

  // ...
  *count   = 0u;
  *entries = NULL;

  while (true) {
    #if defined _WIN32
      bool const directory = entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
      int  const length    = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, entry.cFileName, -1, NULL, 0, NULL, NULL);
      char      *name      = NULL;

      // ...
      if (length > 0) {
        name = static_cast<char*>(::std::malloc(static_cast< ::std::size_t>(length)));

        if (0 == ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, entry.cFileName, -1, name, length, NULL, NULL)) {
          ::std::free(name);
          name = NULL;
        }
      }
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      entry = ::readdir(search);
      if (NULL == entry) break;

      bool                directory = false;
      ::std::size_t const length    = ::std::strlen(entry -> d_name);
      char         *const name      = static_cast<char*>(::std::malloc(length + 1u));
      struct string       subpath;

      // ...
      if (NULL != name) (void) ::std::memcpy(name, entry -> d_name, length + 1u);
      if (NULL != name and subpath.append(path)) {
        if (0u != subpath.length and '/' != subpath.value[subpath.length - 1u]) { if (not subpath.append('/')) success = false; }
        if (success and subpath.append(name)) directory = is_directory(subpath.value); else success = false;
      }
    #endif

    // ...
    if      (NULL == name)                                                    { success = false; break; }
    else if (0 == ::std::strcmp(name, ".") or 0 == ::std::strcmp(name, "..")) { ::std::free(name); }
    else if (SIZE_MAX / sizeof(struct directory) == *count)                   { ::std::free(name); success = false; break; }
    else if (void *const allocation = ::std::realloc(*entries, (*count + 1u) * sizeof(struct directory))) {
      *entries                     = static_cast< struct directory*>(allocation);
      (*entries)[*count].name      = name;
      (*entries)[*count].directory = directory;
      *count                      += 1u;
    } else { ::std::free(name); success = false; break; }

    #if defined _WIN32
      if (FALSE == ::FindNextFileW(search, &entry))
      break;
    #endif
  }

  #if defined _WIN32
    (void) ::FindClose(search);
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    (void) ::closedir(search);
  #endif

  if (not success) {
    while (*count)
      ::std::free((*entries)[--*count].name);
    ::std::free(*entries);
    *entries = NULL;

    return false;
  }

  return true;
}

static char const* mime_type(char const path[]) /* noexcept */ {
  if (char const *const extension = ::std::strrchr(path, '.'))
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
    return format -> type;
  }

  return "application/octet-stream"; // ->> Must be contiguously 8-bits despite `CHAR_BIT`
}

static ::std::FILE* open_file(char const path[]) /* noexcept */ {
  #if defined _WIN32
    if (wchar_t *const widepath = widen_path(path)) {
      ::std::FILE *const stream = ::_wfopen(widepath, L"rb");

      ::std::free(widepath);
      return stream;
    }
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    return ::std::fopen(path, "rb");
  #endif

  return NULL;
}

static socket_t open_listener(unsigned char const ipVersion) /* noexcept */ {
  socket_t const server = ::socket(ipVersion == 6u ? AF_INET6 : AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // ...
  if (INVALID_SOCKET_HANDLE == server) return INVALID_SOCKET_HANDLE;
  #if defined _WIN32
    ::BOOL const exclusive = TRUE;
    (void) ::setsockopt(server, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<char const*>(&exclusive), sizeof exclusive);
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    int const reuse = static_cast<int>(true);
    (void) ::setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);
  #endif

  switch (ipVersion) {
    case 6u: {
      struct ::sockaddr_in6 address  = {};
      int const             ipv6only = 1;

      // ...
      #if defined _WIN32
        (void) ::setsockopt(server, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<char const*>(&ipv6only), sizeof ipv6only);
      #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
        (void) ::setsockopt(server, IPPROTO_IPV6, IPV6_V6ONLY, &ipv6only, sizeof ipv6only);
      #endif

      address.sin6_addr   = in6addr_loopback;
      address.sin6_family = AF_INET6;
      address.sin6_port   = ::htons(static_cast<unsigned short>(port));

      if (0 != ::bind(server, reinterpret_cast<struct ::sockaddr const*>(&address), sizeof address) or 0 != ::listen(server, LISTEN_BACKLOG)) {
        close_socket(server);
        return INVALID_SOCKET_HANDLE;
      }
    } break;

    case 4u: default: {
      struct ::sockaddr_in address = {};

      // ...
      address.sin_addr.s_addr = ::htonl(bindIPv4);
      address.sin_family      = AF_INET;
      address.sin_port        = ::htons(static_cast<unsigned short>(port));

      if (0 != ::bind(server, reinterpret_cast<struct ::sockaddr const*>(&address), sizeof address) or 0 != ::listen(server, LISTEN_BACKLOG)) {
        close_socket(server);
        return INVALID_SOCKET_HANDLE;
      }
    } break;
  }

  return server;
}

static int request_shutdown() /* noexcept */ {
  #if defined _WIN32
    if (::HANDLE const event = ::OpenEventW(EVENT_MODIFY_STATE, FALSE, L"Local\\LapysDev.Localhost.Shutdown")) {
      bool const success = 0 != ::SetEvent(event);

      (void) ::CloseHandle(event);
      return success ? EXIT_SUCCESS : EXIT_FAILURE;
    }
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    struct string name;

    if (control_name(&name)) {
      struct ::sockaddr_un address = {};
      socket_t const       client  = ::socket(AF_UNIX, SOCK_STREAM, 0);
      bool                 success = false;

      if (INVALID_SOCKET_HANDLE != client) {
        (void) ::std::memcpy(address.sun_path, name.value, name.length + 1u);
        address.sun_family = AF_UNIX;
        success            = 0 == ::connect(client, reinterpret_cast<struct ::sockaddr*>(&address), sizeof address) and send_all(client, "S", 1u);

        if (success) {
          char                   confirmation = '\0';
          struct ::timeval const timeout      = {TIMEOUT_CONTROL_SECONDS, 0u};

          (void) ::setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
          success = ::recv(client, &confirmation, 1, 0) == 1 and confirmation == 'K';
        }

        close_socket(client);
        return success ? EXIT_SUCCESS : EXIT_FAILURE;
      }
    }
  #endif
  return EXIT_FAILURE;
}

static bool resolve_path(char const url[], struct string* const path, struct string* const canonical) /* noexcept */ {
  struct string decoded;

  // ... ->> Percent-decode before removing dot segments so encoded traversal cannot escape `root` directory
  if (NULL == url or *url != '/')
  return false;

  if (not canonical -> set("/") or not path -> set(root.value))
  return false;

  for (::std::size_t index = 0u; '\0' != url[index]; ++index) {
    signed char   digits[2] = {-1, -1};
    unsigned char value     = static_cast<unsigned char>(url[index]);

    // ...
    if (value == '%') {
      for (unsigned char subindex = 0u; subindex != sizeof digits / sizeof(digits[0]); ++subindex) {
        char const character = url[index + subindex + 1u];

        // ...
        if ('\0' == character)
        return false;

        if      (character >= '0' and character <= '9') digits[subindex] = character - '0';
        else if (character >= 'A' and character <= 'F') digits[subindex] = character - 'A' + 10;
        else if (character >= 'a' and character <= 'f') digits[subindex] = character - 'a' + 10;

        if (digits[subindex] < 0)
        return false;
      }

      index += 2u;
      value  = static_cast<unsigned char>((digits[0] << 4u) | (digits[1] << 0u));
    }

    if (ASCII_DELETE == value or ASCII_SPACE > value or '\0' == value or value == '\\') return false;
    if (not decoded.append(static_cast<char>(value)))                                   return false;
  }

  for (::std::size_t index = 0u; decoded.length > index; ) {
    char const *segment = "";
    char        separator;

    // ...
    while (decoded.length != index and decoded.value[index] == '/') ++index;
    if    (decoded.length == index)                                   break;
    segment = &decoded.value[index];

    while (decoded.length != index and decoded.value[index] != '/') ++index;
    separator            = decoded.value[index];
    decoded.value[index] = '\0';

    if (0 == ::std::strcmp(segment, "..")) {
      /* ... */ {
        while (path -> length > root.length and path -> value[path -> length - 1u] != '/' and path -> value[path -> length - 1u] != '\\') --path -> length;
        if    (path -> length > root.length)                                                                                              --path -> length;

        path -> value[path -> length] = '\0';
      }

      if (canonical -> length > 1u) {
        while (canonical -> length > 1u and canonical -> value[canonical -> length - 1u] != '/') --canonical -> length;
        if    (canonical -> length > 1u)                                                         --canonical -> length;

        canonical -> value[canonical -> length] = '\0';
      }
    }

    else if (0 != ::std::strcmp(segment, ".") and '\0' != *segment) {
      #if defined _WIN32 // ->> Reject Windows device separators and trailing aliases to avoid ambiguous paths
        ::std::size_t const length = ::std::strlen(segment);

        if (segment[length - 1u] == '.' or segment[length - 1u] == ' ' or NULL != ::std::strpbrk(segment, ":*?\"<>|"))
        return false;
      #endif

      if (0u != path -> length and '/' != path -> value[path -> length - 1u] and '\\' != path -> value[path -> length - 1u]) {
        if (not path -> append('/')) return false;
      }
      if (not path -> append(segment)) return false;
      if (1u < canonical -> length and not canonical -> append('/')) return false;
      if (not append_url(canonical, segment)) return false;
    }

    decoded.value[index] = separator;
  }

  if (decoded.length and decoded.value[decoded.length - 1u] == '/' and canonical -> value[canonical -> length - 1u] != '/' and not canonical -> append('/'))
  return false;

  return true;
}

static bool response_headers(
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

  return send_all(client, response.value, response.length);
}

static bool send_all(socket_t const client, char const data[], ::std::size_t length) /* noexcept */ {
  // ... ->> `send(…)` may write fewer bytes than requested, so retry until all data is sent
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

      // ...
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

static void send_document(socket_t const client, unsigned const status, char const label[], struct string const* const document, bool const head, char const extra[]) /* noexcept */ {
  #if defined _WIN32
    ::ULONGLONG const size = static_cast< ::ULONGLONG>(document -> length);
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    ::off_t const size = static_cast< ::off_t>(document -> length);
  #endif

  if (response_headers(client, status, label, "text/html; charset=utf-8", size, extra) and not head)
  (void) send_all(client, document -> value, document -> length);
}

static void send_error(socket_t const client, unsigned const status, char const label[], bool const head) /* noexcept */ {
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

  send_document(client, status, label, &document, head, HTTP_METHOD_NOT_ALLOWED == status ? "Allow: GET, HEAD, OPTIONS, TRACE" "\r\n" : "");
}

static void set_client_timeout(socket_t const client) /* noexcept */ {
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
}

static void start_client(socket_t const client) /* noexcept */ {
  // ... ->> Active worker counts are bounded and guarded by platform-specific atomics/ mutexes
  #if defined _WIN32
    ::LONG current = 0L;

    // ...
    do {
      current = ::InterlockedCompareExchange(&workerCount, 0L, 0L);

      if (WORKER_MAXIMUM <= current)
      return close_socket(client);
    } while (current != ::InterlockedCompareExchange(&workerCount, current + 1L, current));

    if (::HANDLE const thread = reinterpret_cast< ::HANDLE>(::_beginthreadex(NULL, 0u, &client_thread, reinterpret_cast<void*>(static_cast< ::UINT_PTR>(client)), 0u, NULL)))
    return (void) ::CloseHandle(thread);

    close_socket(client);
    worker_end  ();
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    bool accepted = false;

    // ...
    (void) ::pthread_mutex_lock(&workerMutex);
      accepted     = WORKER_MAXIMUM > workerCount;
      workerCount += accepted;
    (void) ::pthread_mutex_unlock(&workerMutex);

    if (not accepted)
    return close_socket(client);

    if (socket_t* const parameter = static_cast<socket_t*>(::std::malloc(sizeof(socket_t)))) {
      ::pthread_t thread;

      // ...
      *parameter = client;

      if (0 == ::pthread_create(&thread, NULL, &client_thread, parameter))
      return (void) ::pthread_detach(thread);

      ::std::free(parameter);
    }

    close_socket(client);
    worker_end  ();
  #endif
}

static bool start_control() /* noexcept */ {
  // ->> The control endpoint enforces single-instance startup and private shutdown
  #if defined _WIN32
    localhostMutex = ::CreateMutexW(NULL, FALSE, L"Local\\LapysDev.Localhost.Instance");

    if (NULL == localhostMutex)
    return false;

    if (ERROR_ALREADY_EXISTS == ::GetLastError() or NULL == (localhostShutdown = ::CreateEventW(NULL, TRUE, FALSE, L"Local\\LapysDev.Localhost.Shutdown"))) {
      (void) ::CloseHandle(localhostMutex);
      localhostMutex = NULL;

      return false;
    }

    return true;
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    struct ::sockaddr_un address = {};
    int                  bound   = static_cast<int>(false);
    ::mode_t             mask    = 0000;
    struct string        name;

    // ...
    if (not control_name(&name))
    return false;

    address.sun_family = AF_UNIX;
    (void) ::std::memcpy(controlPath,      name.value,  name.length + 1u);
    (void) ::std::memcpy(address.sun_path, controlPath, name.length + 1u);
    control = ::socket(AF_UNIX, SOCK_STREAM, 0);

    if (INVALID_SOCKET_HANDLE == control)
    return false;

    mask  = ::umask(0077u);
    bound = ::bind(control, reinterpret_cast<struct ::sockaddr*>(&address), sizeof address);

    if (0 != bound and EADDRINUSE == errno) {
      socket_t const probe   = ::socket(AF_UNIX, SOCK_STREAM, 0);
      bool     const running = INVALID_SOCKET_HANDLE != probe and 0 == ::connect(probe, reinterpret_cast<struct ::sockaddr*>(&address), sizeof address);

      // ...
      close_socket(probe);

      if (not running) {
        struct ::stat entry = {};
        if (0 == ::lstat(controlPath, &entry) and S_ISSOCK(entry.st_mode) and entry.st_uid == ::geteuid()) {
          (void) ::unlink(controlPath);
          bound = ::bind(control, reinterpret_cast<struct ::sockaddr*>(&address), sizeof address);
        }
      }
    }

    (void) ::umask(mask);
    if (0 != bound or 0 != ::listen(control, CONTROL_BACKLOG)) {
      close_socket(control);

      control        = INVALID_SOCKET_HANDLE;
      controlPath[0] = '\0';

      return false;
    }

    return true;
  #endif
}

#if defined _WIN32
  static wchar_t* widen_path(char const path[]) /* noexcept */ {
    int      length   = 0;
    wchar_t *widepath = NULL;

    // ...
    length = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path, -1, NULL, 0);
    if (length <= 0) return NULL;

    widepath = static_cast<wchar_t*>(::std::malloc(static_cast< ::std::size_t>(length) * sizeof(wchar_t)));
    if (not widepath) return NULL;

    if (length != ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path, -1, widepath, length)) {
      ::std::free(widepath);
      return NULL;
    }

    return widepath;
  }
#endif

static void worker_end() /* noexcept */ {
  #if defined _WIN32
    (void) ::InterlockedDecrement(&workerCount);
  #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
    (void) ::pthread_mutex_lock(&workerMutex);
    --workerCount;
    (void) ::pthread_mutex_unlock(&workerMutex);
  #endif
}

/* Main -|> Consider dynamic API support (e.g. `api/status`), PHP CGI, or SQL database clients. */
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
  for (unsigned index = 1u; count > index; ++index) {
    char const* const option = arguments[index];

    // ...
    if (0 == ::std::strcmp(option, "--shutdown")) {
      if (count != 2)
      return INVALID_COMMAND_LINE;

      shutdown = true;
      break;
    }

    else if (0 == ::std::strcmp(option, "--root") and index + 1 < count) {
      if (not root.set(arguments[++index]))
      return EXIT_FAILURE;
    }

    else if (0 == ::std::strcmp(option, "--port") and index + 1 < count) {
      char const      *value  = arguments[++index];
      ::uint_least16_t number = 0000u;

      // ...
      if ('\0' == *value)
      return INVALID_COMMAND_LINE;

      for (; '\0' != *value; ++value) {
        if (*value < '0' or *value > '9')
        return INVALID_COMMAND_LINE;

        number = (number * 10uL) + static_cast< ::uint_least16_t>(*value - '0');

        if (PORT_MAXIMUM < number)
        return INVALID_COMMAND_LINE;
      }

      if (0000u == number)
      return INVALID_COMMAND_LINE;

      port = number;
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
          hasDigit     = true;
          octets[part] = (octets[part] * 10uL) + static_cast<::uint_least8_t>(*address - '0');

          if (octets[part] > 255uL)
          return INVALID_COMMAND_LINE;
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

      bindIPv4 = static_cast< ::uint_least32_t>((octets[0] << (8u * 3u)) | (octets[1] << (8u * 2u)) | (octets[2] << (8u * 1u)) | (octets[3] << (8u * 0u)));
    }

    else
    return INVALID_COMMAND_LINE;
  }

  // ->> Received HTTP bytes are not converted through the process locale.
  if (help or shutdown) {
    char const* message = NULL;

    // ->> Continue only while each filesystem and conversion operation succeeds.
    if (shutdown) {
      status  = request_shutdown();
      message = EXIT_SUCCESS == status ? "[.]: Localhost shutdown requested." "\r\n" : "[.]: Localhost is not running." "\r\n";
    } else message = "[?]: localhost [--bind IPv4 | --lan] [--port 1..65535] [--root \"DIRECTORY\"] | --shutdown | --help" "\r\n";

    #if defined _WIN32
      ::BOOL const   attached = ::AttachConsole(ATTACH_PARENT_PROCESS);
      ::HANDLE const stream   = ::GetStdHandle(STD_OUTPUT_HANDLE);

      // ...
      if (INVALID_HANDLE_VALUE != stream and NULL != stream) {
        ::DWORD written = 0u;
        (void) ::WriteFile(stream, message, static_cast< ::DWORD>(::std::strlen(message)), &written, NULL);
      }

      if (FALSE != attached)
      (void) ::FreeConsole(); // ->> The Windows-subsystem executable temporarily borrows the calling shell’s console only to report command results
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      (void) ::std::fputs(message, stdout);
    #endif

    return status;
  }

  if (not is_directory(root.value))
  return EXIT_FAILURE;

  #if defined _WIN32
    ::WSADATA winsock = {};

    if (0 != ::WSAStartup(MAKEWORD(2, 2), &winsock))
    return EXIT_FAILURE;
  #endif

  if (not start_control()) {
    #if defined _WIN32
      (void) ::WSACleanup();
    #endif

    return EXIT_FAILURE;
  }

  listeners[0] = open_listener(4u);
  listeners[1] = open_listener(6u);

  if (INVALID_SOCKET_HANDLE == listeners[0] or INVALID_SOCKET_HANDLE == listeners[1]) {
    finish();
    return EXIT_FAILURE;
  }

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

  for (;;) {
    #if defined _WIN32
      if (WAIT_OBJECT_0 == ::WaitForSingleObject(localhostShutdown, 0u)) break;
    #elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
      if (0 != localhostShutdown) break;
    #endif
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
            (void) send_all(client, "K", 1u);
            localhostShutdown = static_cast< ::sig_atomic_t>(true);
          }

          close_socket(client);
        }
      }

      if (localhostShutdown)
      break;
    #endif

    for (unsigned char index = 0u; index < sizeof listeners / sizeof(socket_t); ++index)
    if (FD_ISSET(listeners[index], &incoming)) {
      socket_t client = ::accept(listeners[index], NULL, NULL);

      if (INVALID_SOCKET_HANDLE != client)
      start_client(client);
    }
  }

  finish();
  return EXIT_SUCCESS;
}

#if defined _WIN32
  int WINAPI wWinMain(::HINSTANCE, ::HINSTANCE, ::LPWSTR, int) {
    int       count     = 0;
    int       result    = INVALID_COMMAND_LINE;
    ::LPWSTR* arguments = ::CommandLineToArgvW(::GetCommandLineW(), &count);

    // ...
    if (NULL == arguments or 1 > count)
    return INVALID_COMMAND_LINE;

    char** const values = static_cast<char**>(::std::calloc(static_cast< ::std::size_t>(count), sizeof(char*)));
    if (NULL != values) {
      bool ready = true;

      // ...
      for (int index = 0; index < count; ++index) {
        int const length = ::WideCharToMultiByte(CP_UTF8, 0u, arguments[index], -1, NULL, 0, NULL, NULL);

        // ...
        if (length <= 0) {
          ready = false;
          break;
        }

        values[index] = static_cast<char*>(::std::malloc(static_cast< ::std::size_t>(length)));

        if (NULL == values[index] or length != ::WideCharToMultiByte(CP_UTF8, 0u, arguments[index], -1, values[index], length, NULL, NULL)) {
          ready = false;
          break;
        }
      }

      if (ready)
      result = comain(count, const_cast<char const* const*>(values));

      for (int index = 0; index < count; ++index) ::std::free(values[index]);
      ::std::free(values);
    }

    (void) ::LocalFree(arguments);
    return result;
  }
#elif defined __ANDROID__ or defined __APPLE__ or defined __unix__
  int main(int count, char* arguments[]) {
    return comain(count, const_cast<char const* const*>(arguments));
  }
#endif
