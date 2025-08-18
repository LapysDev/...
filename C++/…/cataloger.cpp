// COMPILE: del cataloger.exe && cls && clang++ -pedantic -std=c++98 -Wall -Wextra -Wno-c++11-long-long cataloger.cpp -o cataloger.exe -ltaskschd && cataloger.exe A C:\Users\oluwa\…\cataloger-clock.dat & del cataloger.exe
// RUN    : cataloger C:/Users/oluwa/OneDrive/Lapys/Catalog C:/Users/oluwa/OneDrive/Lapys/Catalog/cataloger-clock.dat
#include <cerrno>  // --> EILSEQ, errno
#include <ciso646> // --> and, not, or
#include <climits> // --> CHAR_BIT, UCHAR_MAX
#include <clocale> // --> LC_ALL;                                            std::setlocale(…)
#include <csignal> // --> SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM; std::signal(…)
#include <cstddef> // --> NULL; std::ptrdiff_t, std::size_t
#include <cstdio>  // --> _IONBF, EOF, L_tmpnam, stderr, stdin, stdout; std::FILE; std::fflush(…), std::fopen(…, "ccs=UTF-8"), std::fputs(…), std::fwrite(…), std::setvbuf(…), std::tmpfile(…), std::tmpnam(…)
#include <cstdlib> // --> EXIT_FAILURE, EXIT_SUCCESS;                              std::at_quick_exit(…), std::atexit(…), std::exit(…)
#include <ctime>   // --> std::time_t;                                             std::gmtime(…), std::time(…)
#include <cwchar>  // --> std::fgetwc(…), std::fputws(…), std::fwide(…)
#include <new>     // --> delete, new; std:nothrow
#if defined __APPLE__ or defined __bsdi__ or defined __CYGWIN__ or defined __DragonFly__ or defined __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ or defined __FreeBSD__ or defined __FreeBSD_version or defined __gnu_linux__ or defined __linux or defined __linux__ or defined __MACH__ or defined __NETBSD__ or defined __NETBSD_version or defined __OpenBSD__ or defined __OS400__ or defined __QNX__ or defined __QNXNTO__ or defined __sun or defined __SVR4 or defined __svr4__ or defined __sysv__ or defined __unix or defined __unix__ or defined __VMS or defined __VMS_VER or defined _NTO_VERSION or defined _POSIX_SOURCE or defined _SYSTYPE_SVR4 or defined _XOPEN_SOURCE or defined linux or defined NetBSD0_8 or defined NetBSD0_9 or defined NetBSD1_0 or defined OpenBSD2_0 or defined OpenBSD2_1 or defined OpenBSD2_2 or defined OpenBSD2_3 or defined OpenBSD2_4 or defined OpenBSD2_5 or defined OpenBSD2_6 or defined OpenBSD2_7 or defined OpenBSD2_8 or defined OpenBSD2_9 or defined OpenBSD3_0 or defined OpenBSD3_1 or defined OpenBSD3_2 or defined OpenBSD3_3 or defined OpenBSD3_4 or defined OpenBSD3_5 or defined OpenBSD3_6 or defined OpenBSD3_7 or defined OpenBSD3_8 or defined OpenBSD3_9 or defined OpenBSD4_0 or defined OpenBSD4_1 or defined OpenBSD4_2 or defined OpenBSD4_3 or defined OpenBSD4_4 or defined OpenBSD4_5 or defined OpenBSD4_6 or defined OpenBSD4_7 or defined OpenBSD4_8 or defined OpenBSD4_9 or defined sun or defined unix or defined VMS
# include <stdio.h>  // --> ::fileno(…)
# include <unistd.h> // --> ::isatty(…)
#elif defined __NT__ or defined __TOS_WIN__ or defined __WIN32__ or defined __WINDOWS__ or defined _WIN16 or defined _WIN32 or defined _WIN32_WCE or defined _WIN64
# if defined _MSC_BUILD or defined _MSC_FULL_VER or defined _MSC_VER
#   pragma comment(lib, "kernel32.lib")
#   pragma comment(lib, "taskschd.lib")
# endif
# define _CRT_SECURE_NO_WARNINGS // ->> C Run-Time security                    --> `https://learn.microsoft.com/en-us/cpp/c-runtime-library/security-features-in-the-crt`
# define CINTERFACE              // ->> C Interface for Component Object Model --> `https://learn.microsoft.com/en-us/windows/win32/com/the-component-object-model`
# define STRICT                  // ->> Strict type checking                   --> `https://learn.microsoft.com/en-us/windows/win32/winprog/enabling-strict`
# define UNICODE                 // ->> Unicode preference                     --> `https://learn.microsoft.com/en-us/windows/win32/learnwin32/working-with-strings`
# ifndef _WIN32_WINNT            //
#   define _WIN32_WINNT 0x0600 // --> _WIN32_WINNT_WINXP+
# endif
# include <io.h>      // --> ::_isatty(…)
# include <stdio.h>   // --> ::_fileno(…), ::_get_pgmptr(…), ::_get_wpgmptr(…)
# include <windows.h> // --> CP_UTF8, ERROR_CANCELLED, ERROR_INVALID_HANDLE, ERROR_SUCCESS, FOREGROUND_GREEN, FOREGROUND_RED, INFINITE, INVALID_HANDLE_VALUE, MAX_PATH, MB_ERR_INVALID_CHARS, NO_ERROR, SC_MANAGER_ALL_ACCESS, SERVICE_ACCEPT_STOP, SERVICE_ALL_ACCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, SERVICE_RUNNING, SERVICE_STOP_PENDING, SERVICE_STOPPED, SERVICE_WIN32_OWN_PROCESS, STD_ERROR_HANDLE, WINAPI; BOOL, CONSOLE_SCREEN_BUFFER_INFO, DWORD, HANDLE, HLOCAL, HMODULE, LONG, LPBOOL, LPCCH, LPCSTR, LPCVOID, LPCWCH, LPCWSTR, LPDWORD, LPSECURITY_ATTRIBUTES, LPSERVICE_STATUS, LPUNKNOWN, LPVOID, LPWSTR, OLECHAR, PCONSOLE_SCREEN_BUFFER_INFO, PCWSTR, PHANDLE, PSECURITY_DESCRIPTOR, PVOID, PWSTR, REFCLSID, REFIID, SC_HANDLE, SERVICE_STATUS, SERVICE_STATUS_HANDLE, SERVICE_TABLE_ENTRYW, TCHAR, UINT, VARIANTARG, WAIT_FAILED, WAITORTIMERCALLBACK, WCHAR; ::GetModuleHandleExW(…), ::GetProcAddress(…), ::GetSystemDirectoryW(…)
#   include <oaidl.h>         // --> VARIANT, VARIANTARG
#   include <objbase.h>       // --> ::COINIT_MULTITHREADED
#   include <objidl.h>        // --> SOLE_AUTHENTICATION_SERVICE
#   include <rpcdce.h>        // --> RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IMPERSONATE
#   include <wincred.h>       // --> CREDUI_FLAGS_ALWAYS_SHOW_UI, CREDUI_FLAGS_DO_NOT_PERSIST, CREDUI_FLAGS_GENERIC_CREDENTIALS, CREDUI_MAX_PASSWORD_LENGTH, CREDUI_MAX_USERNAME_LENGTH, CREDUIAPI; CREDUI_INFOW, PCtxtHandle
#   include <winerror.h>      // --> FAILED, SUCCEEDED
#   include <winnt.h>         // --> HRESULT
#   include <wtypes.h>        // --> BSTR, ::VT_BSTR
#   include <wtypesbase.h>    // --> ::CLSCTX_INPROC_SERVER
#   if _WIN32_WINNT >= 0x0600 // --> _WIN32_WINNT_VISTA | _WIN32_WINNT_WS08
#     include <taskschd.h> // --> IAction, IActionCollection, IDailyTrigger, IRegisteredTask, IRegistrationInfo, IRepetitionPattern, ITaskDefinition, ITaskFolder, ITaskService, ITrigger, ITriggerCollection; ::CLSID_TaskScheduler, ::IID_IDailyTrigger, ::IID_IExecAction, ::IID_ITaskService, ::TASK_ACTION_EXEC, ::TASK_CREATE_OR_UPDATE, ::TASK_LOGON_PASSWORD, ::TASK_TRIGGER_DAILY
#   endif
#endif

/* Main --> cataloger [log_directory] [clock_path] [rerun] */
int main(int count, char* arguments[]) /* noexcept */ {
  /* ... -> Vendor setup */
  #if defined __NT__ or defined __TOS_WIN__ or defined __WIN32__ or defined __WINDOWS__ or defined _WIN16 or defined _WIN32 or defined _WIN32_WCE or defined _WIN64
    enum {
      advapi32, credui, kernel32, ole32, oleaut32, shell32, taskschd,
      librariesLength // --> std::size_t{…}
    };

    static struct libraryinfo *libraries = NULL;
    static struct libraryinfo /* final */ {
      enum      { nameLengthMaximum = /* ->> Arbitrarily-long */ 12u }; // --> std::size_t
      WCHAR const name[nameLengthMaximum + /* ->> NUL terminator */ 1u];
      BOOL      (*moduleUnloader)(HMODULE);
      HMODULE     moduleHandle;

      static void exit() /* extern "C" */ {
        for (struct libraryinfo *library = libraries; library != libraries + (NULL == libraries ? 0u : librariesLength); ++library) {
          if (NULL != library -> moduleHandle and NULL != library -> moduleUnloader)
          library -> moduleUnloader(library -> moduleHandle); // --> FreeLibrary(…)
        }
      }
    } l[librariesLength] = {
      {L"advapi32" ".dll", NULL, NULL},
      {L"credui"   ".dll", NULL, NULL},
      {L"kernel32" ".dll", NULL, NULL},
      {L"ole32"    ".dll", NULL, NULL},
      {L"oleaut32" ".dll", NULL, NULL},
      {L"shell32"  ".dll", NULL, NULL},
      {L"taskschd" ".dll", NULL, NULL}
    };
    WCHAR      librariesPath[MAX_PATH + libraryinfo::nameLengthMaximum + /* ->> NUL terminator */ 1u]; // --> ::GetSystemDirectoryW(…) + '\\' + libraries[kernel32].name
    UINT const librariesPathLength = (::GetSystemDirectoryW(librariesPath, sizeof librariesPath / sizeof(TCHAR)) * sizeof(TCHAR)) / sizeof(WCHAR);

    // ...
    libraries                          = l;
    librariesPath[librariesPathLength] = L'\\';

    for (std::size_t index = 0u; ; ++index) {
      librariesPath[index + librariesPathLength + 1u] = libraries[kernel32].name[index];
      if (L'\0' == libraries[kernel32].name[index]) break;
    }

    (void) std::atexit(&libraries -> exit);
    #if __cplusplus >= 201103L
      (void) std::at_quick_exit(&libraries -> exit);
    #endif

    if (0u != librariesPathLength) {
      BOOL    (*FreeLibrary)             (HMODULE)                = NULL;
      HMODULE (*LoadLibraryExW)          (LPCWSTR, HANDLE, DWORD) = NULL;
      BOOL    (*SetDefaultDllDirectories)(DWORD)                  = NULL;

      // ... ->> Load the userland `kernel32` library
      if (FALSE != ::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, librariesPath, &libraries[kernel32].moduleHandle))
      if (NULL != libraries[kernel32].moduleHandle) {
        FreeLibrary              = reinterpret_cast<BOOL    (*)(HMODULE)>               (::GetProcAddress(libraries[kernel32].moduleHandle, "FreeLibrary"));              // --> <windows.h>
        LoadLibraryExW           = reinterpret_cast<HMODULE (*)(LPCWSTR, HANDLE, DWORD)>(::GetProcAddress(libraries[kernel32].moduleHandle, "LoadLibraryExW"));           // --> <windows.h>
        SetDefaultDllDirectories = reinterpret_cast<BOOL    (*)(DWORD)>                 (::GetProcAddress(libraries[kernel32].moduleHandle, "SetDefaultDllDirectories")); // --> <windows.h>
      }

      if (NULL != SetDefaultDllDirectories)
      (void) SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_SYSTEM32); // --> %SystemRoot%\System32

      // ... ->> Load other used Windows API libraries —
      for (struct libraryinfo *library = libraries; library != libraries + librariesLength; ++library)
      if (library != libraries + kernel32) { // ->> — except the already loaded userland `kernel32` library
        for (std::size_t index = 0u; ; ++index) {
          librariesPath[index + librariesPathLength + 1u] = library -> name[index];
          if (L'\0' == library -> name[index]) break;
        }

        if (FALSE != ::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, librariesPath, &library -> moduleHandle)) {
          if (NULL != library -> moduleHandle)
          continue; // --> library -> moduleUnloader = NULL;
        }

        library -> moduleHandle   = LoadLibraryExW(librariesPath, static_cast<HANDLE>(NULL), LOAD_WITH_ALTERED_SEARCH_PATH);
        library -> moduleUnloader = NULL == library -> moduleHandle ? NULL : FreeLibrary;
      }
    }
  #endif

  /* ... ->> Application `start()` */
  static struct cataloger *catalog = NULL;
  static struct cataloger /* final */ {
    enum /* : std::size_t */     { MESSAGE_MAXIMUM_LENGTH = 256u };                                                                                         //
    union  formatinfo            { unsigned char metadata[32]; };                                                                                           // ->> Arbitrarily-sized
    enum   formatstyle           { FORMAT_INITIAL = 0x00u, FORMAT_ERROR, FORMAT_WARN };                                                                     // ->> Mutually-exclusive
    enum   parsediag             { PARSED, PARSE_NAME, PARSE_NOT_NAME, PARSE_NOT_TAG, PARSE_NOT_VALUE, PARSE_TAG, PARSE_VALUE, REPARSED, REPARSE_COMMAND }; // ->> Command-line option   parsing
    enum   parsestyle            { PARSE_INITIAL = 0x00u, PARSE_TRIMMED = 0x01u, PARSE_UNCASED = 0x02u };                                                   // ->> Command-line argument interpretation
    struct parsecmd  /* final */ {                                                                                                                          //
      /* ... ->> `NULL` `::…name`s represent unrecognized command-line arguments */
      enum command {                                                                                     //
        ABOUT,                                                                                           //   ->> “About” command prompted
        CLOCK,                                                                                           //   ->> Filesystem location for serializing Catalog clock; Assume NUL-terminated
        HELP,                                                                                            //   ->> “Help” command prompted
        LOG,                                                                                             //   ->> Filesystem location for storing catalogs; Assume NUL-terminated
        RERUN                                                                                            //   ->> Catalog is re-executed
      }                   /* const */ command;                                                           // ->> Command-line option identifier
      union { char const */* const */ text,      *const name; };                                         // ->> Multi-byte NUL-terminated text representing the                                                          name of the command-line option; Optionally `NULL`
      char const         */* const */ longName;                                                          // ->> Multi-byte NUL-terminated text representing the        long  (prefixed by `::commandLineOptionLongTag`)  name of the command-line option; Optionally `NULL`
      union { char const */* const */ shortName, *const alias; };                                        // ->> Multi-byte NUL-terminated text representing the alias/ short (prefixed by `::commandLineOptionShortTag`) name of the command-line option; Optionally `NULL`
      enum parsestyle     /* const */ style;                                                             // ->> Determines the textual interpretation of `::argument`
      std::size_t         /* const */ count;                                                             // ->> Number of command-line arguments (after `::argument`/ `::value`) evaluated; Non-zero invalidates use of value tags within the same `::argument`/ `::value`
      void                           *metadata;                                                          //
      bool                          (*diagnosis)(enum parsediag, char[], struct parsecmd const*, void*); // ->> Predicate function determining the validity of the command-line option; Called through command-line `parse()`; Optionally `NULL`
      struct parse /* final */ {                                                                         // ->> Set through command-line `parse()`
        char                                                                              *argument;     //   ->> Command-line argument representing the command-line option; Initially `NULL`
        struct /* final */ { union { std::ptrdiff_t name, text; }; std::ptrdiff_t value; } offsets;      //   ->> Offsets denoting particular attributes of the command-line option
      } parsed;                                                                                          //
    };

    /* ... */
    static void error(int const signal) /* [[noreturn]] extern "C" */ {
      if (NULL != catalog)
      switch (signal) {
        case SIGABRT: (void) catalog -> exit((catalog -> warn("Cataloger stopped by abormal termination"),                   EXIT_FAILURE)); break;
        case SIGFPE : (void) catalog -> exit((catalog -> warn("Cataloger stopped by a floating-point exception"),            EXIT_FAILURE)); break;
        case SIGILL : (void) catalog -> exit((catalog -> warn("Cataloger stopped by an invalid executable instruction"),     EXIT_FAILURE)); break;
        case SIGINT : (void) catalog -> exit((catalog -> warn("Cataloger stopped by an external interrupt") ? EXIT_SUCCESS : EXIT_FAILURE)); break;
        case SIGSEGV: (void) catalog -> exit((catalog -> warn("Cataloger stopped by impermissible memory access"),           EXIT_FAILURE)); break;
        case SIGTERM: (void) catalog -> exit((catalog -> warn("Cataloger stopped by a termination request"),                 EXIT_FAILURE)); break;

        /* case SIGALRM, SIGBUS, SIGCHLD, SIGCONT, SIGEMT, SIGHUP,    SIGIO, SIGIOT, SIGKILL, SIGPIPE, SIGPOLL, SIGPROF, SIGPWR, SIGQUIT, SIGRTMAX, SIGRTMIN, SIGSTKFLT, SIGSTOP, SIGSYS, SIGTRAP, SIGTSTP, SIGTTIN, SIGTTOU, SIGURG, SIGUSR1, SIGUSR2, SIGVTALRM, SIGXCPU, SIGXFSZ, SIGWINCH: break; */
        /* case SIGCLD,  SIGEMT, SIGINFO, SIGLOST, SIGPWR, SIGSTKFLT, SIGUNUSED                                                                                                                                                                                                              : break; */
        default: (void) catalog -> exit((catalog -> warn("Cataloger stopped"), EXIT_FAILURE)); break;
      }

      catalog = NULL;
      std::exit(EXIT_FAILURE);
    }

    static int exit(int const code) /* [[noreturn]] */ {
      if (NULL != catalog) {
        if         (catalog -> clockPathAllocated) ::delete[] catalog -> clockPath;
        if (NULL != catalog -> clockFileStream) std::fclose  (catalog -> clockFileStream);

        catalog -> applicationExitCode = code;
        catalog -> clockFileStream     = NULL;
      }

      catalog = NULL;
      std::exit(code);

      return code;
    }

    static bool format(std::FILE* const stream, enum formatstyle const style, union formatinfo* const information = NULL) {
      std::size_t const informationMetadataSize = sizeof information -> metadata / sizeof(unsigned char);
      bool              styleSupported          = false;
      #if defined __APPLE__ or defined __bsdi__ or defined __CYGWIN__ or defined __DragonFly__ or defined __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ or defined __FreeBSD__ or defined __FreeBSD_version or defined __gnu_linux__ or defined __linux or defined __linux__ or defined __MACH__ or defined __NETBSD__ or defined __NETBSD_version or defined __OpenBSD__ or defined __OS400__ or defined __QNX__ or defined __QNXNTO__ or defined __sun or defined __SVR4 or defined __svr4__ or defined __sysv__ or defined __unix or defined __unix__ or defined __VMS or defined __VMS_VER or defined _NTO_VERSION or defined _POSIX_SOURCE or defined _SYSTYPE_SVR4 or defined _XOPEN_SOURCE or defined linux or defined NetBSD0_8 or defined NetBSD0_9 or defined NetBSD1_0 or defined OpenBSD2_0 or defined OpenBSD2_1 or defined OpenBSD2_2 or defined OpenBSD2_3 or defined OpenBSD2_4 or defined OpenBSD2_5 or defined OpenBSD2_6 or defined OpenBSD2_7 or defined OpenBSD2_8 or defined OpenBSD2_9 or defined OpenBSD3_0 or defined OpenBSD3_1 or defined OpenBSD3_2 or defined OpenBSD3_3 or defined OpenBSD3_4 or defined OpenBSD3_5 or defined OpenBSD3_6 or defined OpenBSD3_7 or defined OpenBSD3_8 or defined OpenBSD3_9 or defined OpenBSD4_0 or defined OpenBSD4_1 or defined OpenBSD4_2 or defined OpenBSD4_3 or defined OpenBSD4_4 or defined OpenBSD4_5 or defined OpenBSD4_6 or defined OpenBSD4_7 or defined OpenBSD4_8 or defined OpenBSD4_9 or defined sun or defined unix or defined VMS
        int const streamDescriptor = ::fileno(stream);

        // ...
        styleSupported = streamDescriptor != -1 and ::isatty(streamDescriptor) == 1;
      #elif defined __NT__ or defined __TOS_WIN__ or defined __WIN32__ or defined __WINDOWS__ or defined _WIN16 or defined _WIN32 or defined _WIN32_WCE or defined _WIN64
        BOOL   WINAPI      (*const GetConsoleMode)            (HANDLE, LPDWORD)                     = NULL == libraries[kernel32].moduleHandle ? NULL : reinterpret_cast<BOOL   WINAPI (*)(HANDLE, LPDWORD)>                    (::GetProcAddress(libraries[kernel32].moduleHandle, "GetConsoleMode"));             // --> <windows.h>
        BOOL   WINAPI      (*const GetConsoleScreenBufferInfo)(HANDLE, PCONSOLE_SCREEN_BUFFER_INFO) = NULL == libraries[kernel32].moduleHandle ? NULL : reinterpret_cast<BOOL   WINAPI (*)(HANDLE, PCONSOLE_SCREEN_BUFFER_INFO)>(::GetProcAddress(libraries[kernel32].moduleHandle, "GetConsoleScreenBufferInfo")); // --> <windows.h>
        HANDLE WINAPI      (*const GetStdHandle)              (DWORD)                               = NULL == libraries[kernel32].moduleHandle ? NULL : reinterpret_cast<HANDLE WINAPI (*)(DWORD)>                              (::GetProcAddress(libraries[kernel32].moduleHandle, "GetStdHandle"));               // --> <windows.h>
        BOOL   WINAPI      (*const SetConsoleMode)            (HANDLE, DWORD)                       = NULL == libraries[kernel32].moduleHandle ? NULL : reinterpret_cast<BOOL   WINAPI (*)(HANDLE, DWORD)>                      (::GetProcAddress(libraries[kernel32].moduleHandle, "SetConsoleMode"));             // --> <windows.h>
        BOOL   WINAPI      (*const SetConsoleTextAttribute)   (HANDLE, WORD)                        = NULL == libraries[kernel32].moduleHandle ? NULL : reinterpret_cast<BOOL   WINAPI (*)(HANDLE, WORD)>                       (::GetProcAddress(libraries[kernel32].moduleHandle, "SetConsoleTextAttribute"));    // --> <windows.h>
        DWORD                      streamConsoleMode                                                = 0x00u;
        CONSOLE_SCREEN_BUFFER_INFO streamConsoleScreenBufferInformation                             = {};
        HANDLE                     streamStandardDeviceHandle                                       = NULL != GetStdHandle ? GetStdHandle(STD_ERROR_HANDLE) : NULL;

        // ...
        styleSupported = INVALID_HANDLE_VALUE != streamStandardDeviceHandle and NULL != streamStandardDeviceHandle;
      #endif

      // ...
      (void) informationMetadataSize;
      (void) stream;

      if (styleSupported)
      switch (style) {
        case FORMAT_ERROR:
        case FORMAT_WARN : {
          #if defined __APPLE__ or defined __bsdi__ or defined __CYGWIN__ or defined __DragonFly__ or defined __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ or defined __FreeBSD__ or defined __FreeBSD_version or defined __gnu_linux__ or defined __linux or defined __linux__ or defined __MACH__ or defined __NETBSD__ or defined __NETBSD_version or defined __OpenBSD__ or defined __OS400__ or defined __QNX__ or defined __QNXNTO__ or defined __sun or defined __SVR4 or defined __svr4__ or defined __sysv__ or defined __unix or defined __unix__ or defined __VMS or defined __VMS_VER or defined _NTO_VERSION or defined _POSIX_SOURCE or defined _SYSTYPE_SVR4 or defined _XOPEN_SOURCE or defined linux or defined NetBSD0_8 or defined NetBSD0_9 or defined NetBSD1_0 or defined OpenBSD2_0 or defined OpenBSD2_1 or defined OpenBSD2_2 or defined OpenBSD2_3 or defined OpenBSD2_4 or defined OpenBSD2_5 or defined OpenBSD2_6 or defined OpenBSD2_7 or defined OpenBSD2_8 or defined OpenBSD2_9 or defined OpenBSD3_0 or defined OpenBSD3_1 or defined OpenBSD3_2 or defined OpenBSD3_3 or defined OpenBSD3_4 or defined OpenBSD3_5 or defined OpenBSD3_6 or defined OpenBSD3_7 or defined OpenBSD3_8 or defined OpenBSD3_9 or defined OpenBSD4_0 or defined OpenBSD4_1 or defined OpenBSD4_2 or defined OpenBSD4_3 or defined OpenBSD4_4 or defined OpenBSD4_5 or defined OpenBSD4_6 or defined OpenBSD4_7 or defined OpenBSD4_8 or defined OpenBSD4_9 or defined sun or defined unix or defined VMS
            /* Don’t repeat yourself… */
            switch (style) {
              case FORMAT_ERROR: return std::fwrite("\x1B" "[31m", sizeof(char), 5u, stream) == 5u;
              case FORMAT_WARN : return std::fwrite("\x1B" "[33m", sizeof(char), 5u, stream) == 5u;
              default:           return true;
            }
          #elif defined __NT__ or defined __TOS_WIN__ or defined __WIN32__ or defined __WINDOWS__ or defined _WIN16 or defined _WIN32 or defined _WIN32_WCE or defined _WIN64
            if (NULL != information and NULL != GetConsoleMode and NULL != SetConsoleMode) {
              if (FALSE != GetConsoleMode(streamStandardDeviceHandle, &streamConsoleMode))
              if (FALSE != SetConsoleMode(streamStandardDeviceHandle, streamConsoleMode | /* --> ENABLE_VIRTUAL_TERMINAL_PROCESSING */ 0x0004u)) {
                information -> metadata[0] = true;

                /* Don’t repeat yourself… */
                switch (style) {
                  case FORMAT_ERROR: return std::fwrite("\x1B" "[31m", sizeof(char), 5u, stream) == 5u;
                  case FORMAT_WARN : return std::fwrite("\x1B" "[33m", sizeof(char), 5u, stream) == 5u;
                  default:           return true;
                }
              }
            }

            if (NULL != information and NULL != GetConsoleScreenBufferInfo and NULL != SetConsoleTextAttribute)
            if (FALSE != GetConsoleScreenBufferInfo(streamStandardDeviceHandle, &streamConsoleScreenBufferInformation)) {
              information -> metadata[0] = false;

              if (informationMetadataSize >= sizeof streamConsoleScreenBufferInformation.wAttributes) {
                for (std::size_t index = 0u; index != sizeof streamConsoleScreenBufferInformation.wAttributes; streamConsoleScreenBufferInformation.wAttributes >>= CHAR_BIT)
                information -> metadata[++index] = static_cast<unsigned char>(streamConsoleScreenBufferInformation.wAttributes & UCHAR_MAX);
              }

              switch (style) {
                case FORMAT_ERROR: return FALSE != SetConsoleTextAttribute(streamStandardDeviceHandle, FOREGROUND_RED);
                case FORMAT_WARN : return FALSE != SetConsoleTextAttribute(streamStandardDeviceHandle, FOREGROUND_GREEN | FOREGROUND_RED);
                default:           return true;
              }
            }
          #endif
        } break;

        case FORMAT_INITIAL: {
          #if defined __APPLE__ or defined __bsdi__ or defined __CYGWIN__ or defined __DragonFly__ or defined __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ or defined __FreeBSD__ or defined __FreeBSD_version or defined __gnu_linux__ or defined __linux or defined __linux__ or defined __MACH__ or defined __NETBSD__ or defined __NETBSD_version or defined __OpenBSD__ or defined __OS400__ or defined __QNX__ or defined __QNXNTO__ or defined __sun or defined __SVR4 or defined __svr4__ or defined __sysv__ or defined __unix or defined __unix__ or defined __VMS or defined __VMS_VER or defined _NTO_VERSION or defined _POSIX_SOURCE or defined _SYSTYPE_SVR4 or defined _XOPEN_SOURCE or defined linux or defined NetBSD0_8 or defined NetBSD0_9 or defined NetBSD1_0 or defined OpenBSD2_0 or defined OpenBSD2_1 or defined OpenBSD2_2 or defined OpenBSD2_3 or defined OpenBSD2_4 or defined OpenBSD2_5 or defined OpenBSD2_6 or defined OpenBSD2_7 or defined OpenBSD2_8 or defined OpenBSD2_9 or defined OpenBSD3_0 or defined OpenBSD3_1 or defined OpenBSD3_2 or defined OpenBSD3_3 or defined OpenBSD3_4 or defined OpenBSD3_5 or defined OpenBSD3_6 or defined OpenBSD3_7 or defined OpenBSD3_8 or defined OpenBSD3_9 or defined OpenBSD4_0 or defined OpenBSD4_1 or defined OpenBSD4_2 or defined OpenBSD4_3 or defined OpenBSD4_4 or defined OpenBSD4_5 or defined OpenBSD4_6 or defined OpenBSD4_7 or defined OpenBSD4_8 or defined OpenBSD4_9 or defined sun or defined unix or defined VMS
            /* Don’t repeat yourself… */
            return std::fwrite("\x1B" "[00m", sizeof(char), 5u, stream) == 5u;
          #elif defined __NT__ or defined __TOS_WIN__ or defined __WIN32__ or defined __WINDOWS__ or defined _WIN16 or defined _WIN32 or defined _WIN32_WCE or defined _WIN64
            /* Don’t repeat yourself… */
            if (NULL != information and information -> metadata[0] == static_cast<unsigned char>(true))
            return std::fwrite("\x1B" "[00m", sizeof(char), 5u, stream) == 5u;

            if (NULL != information and information -> metadata[0] == static_cast<unsigned char>(false) and NULL != SetConsoleTextAttribute) {
              if (informationMetadataSize >= sizeof streamConsoleScreenBufferInformation.wAttributes) {
                for (std::size_t index = sizeof streamConsoleScreenBufferInformation.wAttributes; index; --index)
                streamConsoleScreenBufferInformation.wAttributes = information -> metadata[index] | (streamConsoleScreenBufferInformation.wAttributes << CHAR_BIT);
              }

              return FALSE != SetConsoleTextAttribute(streamStandardDeviceHandle, streamConsoleScreenBufferInformation.wAttributes);
            }
          #endif
        } break;

        default:;
      }

      return false;
    }

    static bool log(void const* const message, std::FILE* const stream, std::size_t const stride) {
      typedef union _ {
        static void spanof(unsigned char const character[], std::size_t const stride, std::size_t* const begin, std::size_t* const end) {
          // ... ->> Ignores `character` endianness, leading bytes, and trailing bytes (assumes it is a distinct set of `stride` code units)
          for (; *begin != stride; ++*begin) {
            if (0x00u != character[*begin])
            break;
          }

          for (std::size_t index = *begin, redundant = false; ; ++index) {
            *end = index;

            if (index == stride or redundant)
            break;

            for (++*end, redundant = true; *end != stride; ++*end)
            if (0x00u != character[*end]) {
              redundant = false;
              break;
            }
          }
        }
      } character;

      std::size_t       messageBegin              = 0u;                                           //
      std::size_t       messageEnd                = 0u;                                           //
      std::size_t       messageLength             = 0u;                                           //
      std::size_t       messageOutputLength       = 0u;                                           //
      bool              messageOutputted          = false;                                        //
      char const        messageOverflow[]         = "…" "\r\n";                                   // ->> Source literal encoding to multi-byte NUL-terminated string
      bool              messageOverflowed         = false;                                        //
      std::size_t const messageOverflowLength     = (sizeof messageOverflow / sizeof(char)) - 1u; //
      std::size_t       messagePrefixLength       = 0u;                                           //
      std::size_t       messageSuffixLength       = 0u;                                           //
      bool              messageSuffixSignificance = true;                                         // ->> `message` ends with `::messageSuffix` followed by whitespace
      char              messageOutput[MESSAGE_MAXIMUM_LENGTH + messageOverflowLength + /* ->> NUL terminator */ 1u];

      // ...
      if (NULL == message or NULL == stream) return true;
      if (MESSAGE_MAXIMUM_LENGTH < stride)   return false;

      // ... --> messageLength = …;
      for (std::size_t index = 0u; ; index += stride) {
        unsigned char const *character      = static_cast<unsigned char const*>(message) + index;
        bool                 characterIsNUL = true;

        // ... ->> Assumes NUL `character` is a complete set of zeroed code units
        for (std::size_t subindex = 0u; stride != subindex; ++subindex)
        if (0x00u != character[subindex]) {
          characterIsNUL = false;
          break;
        }

        if (characterIsNUL or MESSAGE_MAXIMUM_LENGTH + 1u == index) {
          messageLength = index;
          break;
        }
      }

      // ... --> messageBegin = …; messageEnd = …;
      messageBegin = 0u;
      messageEnd   = messageLength;

      // ... ->> Truncate (non-whitespace leading) `::messagePrefix` repeats
      if (NULL != catalog -> messagePrefix) {
        // ... --> messagePrefixLength = …;
        while (MESSAGE_MAXIMUM_LENGTH + 1u != messagePrefixLength and '\0' != catalog -> messagePrefix[messagePrefixLength])
        ++messagePrefixLength;

        // ...
        for (std::size_t messageIndex = 0u, messagePrefixed = true, messagePrefixIndex = 0u; messageIndex != messageLength and messagePrefixed; messageIndex += stride) {
          unsigned char const *const character      = static_cast<unsigned char const*>(message) + messageIndex;
          std::size_t                characterBegin = 0u;
          std::size_t                characterEnd   = stride;

          // ... --> messageBegin = …;
          //     ->> Assume first occurrence of `::messagePrefix` begins the `message`
          character::spanof(character, stride, &characterBegin, &characterEnd);

          for (std::size_t subindex = characterBegin; characterEnd != subindex; ) {
            // ... ->> Follow iteratively next character/ code unit for `::messagePrefix`
            if (catalog -> messagePrefix[messagePrefixIndex++] != character[subindex++]) {
              messagePrefixed = false;
              break;
            }

            // ... ->> Found `::messagePrefix`
            if (messagePrefixIndex == messagePrefixLength) {
              // ... ->> Truncate `message` by this (repeat) instance of `::messagePrefix`
              messageBegin       = messageIndex + subindex;
              messagePrefixIndex = 0u;
            }
          }
        }
      }

      // ... ->> Truncate (whitespace trailing) `::messageSuffix` repeats
      if (NULL != catalog -> messageSuffix) {
        // ... --> messageSuffixLength = …;
        while (MESSAGE_MAXIMUM_LENGTH + 1u != messageSuffixLength and '\0' != catalog -> messageSuffix[messageSuffixLength])
        ++messageSuffixLength;

        // ...
        for (std::size_t messageIndex = messageLength, messageSuffixed = false, messageSuffixIndex = messageSuffixLength, messageTrimIndex = messageIndex, messageTrimSubindex = stride; messageIndex and messageSuffixSignificance; messageIndex -= stride) {
          unsigned char const *const character      = static_cast<unsigned char const*>(message) + (messageIndex - stride);
          std::size_t                characterBegin = 0u;
          std::size_t                characterEnd   = stride;

          // ... --> messageEnd = …;
          //     ->> Search last occurrence of `::messageSuffix`
          character::spanof(character, stride, &characterBegin, &characterEnd);

          for (std::size_t subindex = characterEnd; characterBegin != subindex; ) {
            // ... ->> Search iteratively next character/ code unit for `::messageSuffix`
            if (messageSuffixIndex == messageSuffixLength) {
              messageTrimIndex    = messageIndex - stride;
              messageTrimSubindex = subindex     - 1u;
            }

            if (catalog -> messageSuffix[--messageSuffixIndex] != character[--subindex]) {
              messageSuffixIndex = messageSuffixLength;
              continue;
            }

            // ... ->> Found `::messageSuffix`
            if (0u == messageSuffixIndex) {
              // ... ->> Determine `::messageSuffix` significance i.e.: trailed by whitespace
              if (not messageSuffixed)
              for (bool suffixed = true; messageEnd != messageTrimIndex and messageSuffixSignificance; messageTrimIndex += stride) {
                unsigned char const *const character      = static_cast<unsigned char const*>(message) + messageTrimIndex;
                std::size_t                characterBegin = 0u;
                std::size_t                characterEnd   = stride;

                // ... --> messageSuffixSignificance = …;
                character::spanof(character, stride, &characterBegin, &characterEnd);

                characterBegin      = 0u == messageTrimSubindex ? characterBegin : messageTrimSubindex;
                messageTrimSubindex = 0u;

                for (std::size_t subindex = characterBegin; characterEnd != subindex and messageSuffixSignificance; ++subindex, suffixed = false)
                if (not suffixed) { // ->> First code unit is the end of `::messageSuffix`, so it is ignored
                  switch (static_cast<char>(character[subindex])) {
                    case '\f': case '\n': case '\r': case '\t': case '\v': case ' ':
                    continue;
                  }

                  messageSuffixSignificance = false;
                }
              }

              // ... ->> `::messageSuffix` must be significant to be truncated off `message`
              if (not messageSuffixSignificance)
              break;

              // ... ->> Truncate `message` by tracking the first instance in the last set of contiguous `::messageSuffix` repeats
              messageEnd         = subindex + (messageIndex - stride);
              messageSuffixed    = true; // ->> For symmetry with `messagePrefixed` only
              messageSuffixIndex = messageSuffixLength;
            }
          }
        }
      }

      // ... --> messageOutput[messageOutputLength++…] = …;
      for (std::size_t index = 0u; index != messagePrefixLength and not messageOverflowed; ++index) {
        // ... --> … = ::messagePrefix;
        messageOutput[messageOutputLength++] = catalog -> messagePrefix[index];

        if (MESSAGE_MAXIMUM_LENGTH == messageOutputLength)
        messageOverflowed = true;
      }

      for (std::size_t index = 0u; index != messageLength and not messageOverflowed; index += stride) {
        unsigned char const *const character      = static_cast<unsigned char const*>(message) + index;
        std::size_t                characterBegin = 0u;
        std::size_t                characterEnd   = stride;

        // ... --> … = message;
        character::spanof(character, stride, &characterBegin, &characterEnd);

        for (std::size_t subindex = 0u; characterEnd != subindex; ++subindex)
        if (messageBegin <= index + subindex and messageEnd > index + subindex) {
          messageOutput[messageOutputLength++] = static_cast<char>(character[subindex]);

          if (MESSAGE_MAXIMUM_LENGTH == messageOutputLength) {
            messageOverflowed = true;
            break;
          }
        }
      }

      for (std::size_t index = 0u; index != messageSuffixLength and not messageOverflowed; ++index) {
        // ... --> … = ::messageSuffix;
        messageOutput[messageOutputLength++] = catalog -> messageSuffix[index];

        if (MESSAGE_MAXIMUM_LENGTH == messageOutputLength) {
          messageOutputLength -= index + 1u;
          messageOverflowed    = true;
        }
      }

      if (messageOverflowed) {
        // ... --> … = messageOverflow;
        for (std::size_t index = 0u; index != messageOverflowLength; ++index)
        messageOutput[messageOutputLength++] = messageOverflow[index];
      }

      messageOutput[messageOutputLength] = '\0';

      // ...
      (void) std::fflush (stream); // ->> Ensure the `stream` delivers the `messageOutput` immediately
      (void) std::setvbuf(stream, static_cast<char*>(NULL), _IONBF, 0u);

      messageOutputted = EOF != std::fputs(messageOutput, stream);
      messageOutputted ? (void) std::fflush(stream) : std::clearerr(stream);

      // ...
      return messageOutputted;
    }
      static std::FILE* log()                                                         { return stdout; }
      static bool       log(char    const message[], std::FILE* const stream = log()) { return catalog -> log(message, stream, sizeof(char)); }
      static bool       log(wchar_t const message[], std::FILE* const stream = log()) { return catalog -> log(message, stream, sizeof(wchar_t)); }

    static bool parse(char* arguments[], std::size_t const argumentsLength, struct parsecmd options[], std::size_t const optionsLength, bool (*const undiagnosis)(char[]) = NULL) {
      typedef union parseinfo {
        static char encase(char const value) {
          return value;
        }

        static struct parsecmd* info(struct parsecmd const option, union parseinfo const& information = parseinfo()) {
          information.option = option;
          return &information.option;
        }

        static char** info(char argumentBegin[], char argumentEnd[], union parseinfo const& information = parseinfo()) {
          information.argument[0] = argumentBegin;
          information.argument[1] = argumentEnd;

          return information.argument;
        }

        static char* nextchar(char* argument) {
          for (; '\0' != *argument; ++argument)
          switch (*argument) {
            case '\f': case '\n': case '\r': case '\t': case '\v': case ' ': continue;
            default: return argument;
          }

          return argument;
        }

        static char nocase(char const value) {
          switch (value) {
            case 'A': return 'a'; case 'B': return 'b'; case 'C': return 'c'; case 'D': return 'd'; case 'E': return 'e';
            case 'F': return 'f'; case 'G': return 'g'; case 'H': return 'h'; case 'I': return 'i'; case 'J': return 'j';
            case 'K': return 'k'; case 'L': return 'l'; case 'M': return 'm'; case 'N': return 'n'; case 'O': return 'o';
            case 'P': return 'p'; case 'Q': return 'q'; case 'R': return 'r'; case 'S': return 's'; case 'T': return 't';
            case 'U': return 'u'; case 'V': return 'v'; case 'W': return 'w'; case 'X': return 'x'; case 'Y': return 'y';
            case 'Z': return 'z';
          }

          return encase(value);
        }

        static bool nodiagnosis(enum parsediag const, char[], struct parsecmd const* const, void* const) {
          return true;
        }

        /* ... */
        void                   *value;
        mutable char           *argument[2];
        mutable struct parsecmd option;
      } argument, option;

      // ...
      for (std::size_t optionIndex = 0u; optionIndex != optionsLength; ++optionIndex)
      options[optionIndex].parsed.argument = NULL;

      for (std::size_t argumentIndex = 0u; argumentIndex != argumentsLength; ++argumentIndex) {
        char            *const argument       = arguments[argumentIndex];
        std::size_t            argumentLength = 0u;
        struct parsecmd const *argumentOption = NULL;

        // ... --> argumentLength = …;
        while ('\0' != argument[argumentLength])
        ++argumentLength;

        // ...
        if (NULL == argumentOption)
        for (struct parsecmd *option = options; option != options + optionsLength; ++option) {
          bool          (*const diagnosis)(enum parsediag, char[], struct parsecmd const*, void*) = NULL == option -> diagnosis ? &option::nodiagnosis : option -> diagnosis;
          bool                  diagnostic                                                        = true;
          char          (*const recase)(char)                                                     = option -> style & PARSE_UNCASED ? &option::nocase : &option::encase;
          struct parsecmd const reset                                                             = *option;

          // ... ->> Match based on the `option` `name`
          typedef struct parsecmd parsecmd;

          for (char const *const parsecmd:: *const names[] = {&parsecmd::text, &parsecmd::longName, &parsecmd::shortName}, *const parsecmd:: *const *name = names; diagnostic and name != names + (sizeof names / sizeof(char const* parsecmd::*)); ++name)
          if (NULL != option ->* *name) {
            char const *const   notag[2]    = {"", NULL}; // ->> Key-value token pair
            char               *subargument = argument;
            char const *const (*tags)[2]    = &notag;
            std::size_t         tagsLength  = 1u;

            // ...
            if (&parsecmd::longName  == *name) { tags = catalog -> commandLineOptionLongTags;  tagsLength = catalog -> commandLineOptionLongTagsLength;  }
            if (&parsecmd::shortName == *name) { tags = catalog -> commandLineOptionShortTags; tagsLength = catalog -> commandLineOptionShortTagsLength; }

            subargument = option -> style & PARSE_TRIMMED or &notag != tags ? argument::nextchar(subargument) : subargument;

            // ... ->> Match based on the `option` `tags`
            for (std::size_t tagIndex = 0u; diagnostic and tagIndex != tagsLength; ++tagIndex) {
              char                    *argumentName  = NULL;
              char                    *argumentValue = NULL;
              char const *const *const tag           = tags[tagIndex]; // --> char const *const (&)[2]
              char const        *const tagName       = tag [0];
              char const        *const tagValue      = 0u != option -> count ? NULL : tag[1];

              // ... ->> Validate based on `tagName`
              for (std::size_t index = 0u; ; ++index) {
                if ('\0' == tagName[index]) {
                  argumentName = subargument + index;
                  argumentName = &notag == tags ? argumentName : argument::nextchar(argumentName);
                  diagnostic   = diagnostic and diagnosis(PARSE_TAG, argument, option, /* --> tagName */ option::info(subargument, argumentName));

                  break;
                }

                if ('\0' == subargument[index] or recase(subargument[index]) != recase(tagName[index])) {
                  diagnostic = diagnostic and diagnosis(PARSE_NOT_TAG, argument, option, /* --> argument */ option::info(argument, argument + argumentLength));
                  break;
                }
              }

              if (NULL == argumentName)
              continue;

              // ... ->> Validate based on `argument` name (or optionally `tagValue`)
              for (std::size_t index = 0u; ; ++index) {
                char *const subargument = argument::nextchar(argumentName + index);

                // ...
                for (std::size_t subindex = 0u; '\0' != subargument[subindex] and NULL != tagValue; ++subindex) {
                  if ('\0' == tagValue[subindex]) {
                    argumentValue = subargument + subindex;
                    break;
                  }

                  if (recase(subargument[subindex]) != recase(tagValue[subindex]))
                  break;
                }

                if (NULL != argumentValue or '\0' == (option ->* *name)[index]) {
                  if (NULL != argumentValue or '\0' == *(PARSE_TRIMMED != option -> style and &notag == tags ? argumentName + index : subargument)) {
                    diagnostic = diagnostic and diagnosis(PARSE_NAME,                                            argument, option, /* --> argumentName */                 option::info(argumentName,                                     argumentName + index));
                    diagnostic = diagnostic and diagnosis(NULL == argumentValue ? PARSE_NOT_VALUE : PARSE_VALUE, argument, option, /* --> … ? argument : argumentValue */ option::info(NULL == argumentValue ? argument : argumentValue, argument     + argumentLength));

                    /* Don’t repeat yourself… */
                    for (std::size_t optionIndex = 0u; optionIndex != optionsLength; ++optionIndex) {
                      if (option != options + optionIndex and option -> command == options[optionIndex].command and NULL != options[optionIndex].parsed.argument)
                      diagnostic = diagnostic and diagnosis(REPARSE_COMMAND, argument, option, options + optionIndex);
                    }

                    option -> parsed.argument      = argument;
                    option -> parsed.offsets.name  = NULL != argumentName  ? argumentName  - option -> parsed.argument : -1;
                    option -> parsed.offsets.value = NULL != argumentValue ? argumentValue - option -> parsed.argument : -1;
                    diagnostic                     = diagnostic and (NULL == reset.parsed.argument or diagnosis(REPARSED, argument, option, option::info(reset)));
                    diagnostic                     = diagnostic and                                   diagnosis(PARSED,   argument, option, option::info(reset));
                    argumentOption                 = option;

                    // ...
                    if (not diagnostic) {
                      option -> parsed = reset.parsed;
                      argumentOption   = NULL;

                      break;
                    }
                  }

                  break;
                }

                if ('\0' == argumentName[index] or recase(argumentName[index]) != recase((option ->* *name)[index])) {
                  diagnostic = diagnostic and diagnosis(PARSE_NOT_NAME, argument, option, /* --> argument */ option::info(argument, argument + argumentLength));
                  break;
                }
              }

              // ...
              if (NULL != argumentOption)
              break;
            }

            // ...
            if (NULL != argumentOption)
            break;
          }

          // ...
          if (NULL != argumentOption and diagnostic)
          break;
        }

        if (NULL == argumentOption)
        for (struct parsecmd *option = options; option != options + optionsLength; ++option) {
          bool          (*const diagnosis)(enum parsediag, char[], struct parsecmd const*, void*) = NULL == option -> diagnosis ? &option::nodiagnosis : option -> diagnosis;
          bool                  diagnostic                                                        = true;
          struct parsecmd const reset                                                             = *option;

          // ... ->> Match the `option` automatically
          if (NULL == option -> longName and NULL == option -> shortName and NULL == option -> text) {
            /* Don’t repeat yourself… */
            for (std::size_t optionIndex = 0u; optionIndex != optionsLength; ++optionIndex) {
              if (option != options + optionIndex and option -> command == options[optionIndex].command and NULL != options[optionIndex].parsed.argument)
              diagnostic = diagnostic and diagnosis(REPARSE_COMMAND, argument, option, options + optionIndex);
            }

            option -> parsed.argument     = argument;
            option -> parsed.offsets.text = 0;
            diagnostic                    = diagnostic and (NULL == reset.parsed.argument or diagnosis(REPARSED, argument, option, option::info(reset)));
            diagnostic                    = diagnostic and                                   diagnosis(PARSED,   argument, option, option::info(reset));
            argumentOption                = option;

            if (NULL != argumentOption and diagnostic)
              break;

            // ...
            option -> parsed = reset.parsed;
            argumentOption   = NULL;
          }

          // ...
          if (NULL != argumentOption)
          break;
        }

        // ... ->> Matched against no `option`
        if (NULL == argumentOption) {
          if (NULL != undiagnosis) {
            if (not undiagnosis(argument))
            return false;
          }

          continue;
        }

        // ... ->> Matched against a valid `option`
        if (NULL != argumentOption) {
          // ... ->> Onto the next `argument`
          if (argumentOption -> count > argumentsLength - argumentIndex)
          break;

          argumentIndex += argumentOption -> count;
        }
      }

      return true;
    }

    static void quick_exit() /* [[noreturn]] extern "C" */ {
      if (NULL != catalog)
        (void) catalog -> exit(catalog -> applicationExitCode);

      // ... ->> Cleanup regardless
      std::exit(EXIT_SUCCESS);
    }

    static void start(char* arguments[], std::size_t length) {
      typedef union _ {
        static bool diagnose(enum parsediag const diagnosis, char argument[], struct parsecmd const* const option, void* const information) {
          bool                   const repeated = REPARSED == diagnosis or REPARSE_COMMAND == diagnosis;
          struct parsecmd const *const sequence = static_cast<struct parsecmd*>(option -> metadata);

          // ... ->> Track repeated command-line options
          if (repeated) {
            bool                 (*log)(char const[], std::FILE*)                                      = NULL;
            char const            *message                                                             = argument;
            char                   messageOutput[MESSAGE_MAXIMUM_LENGTH + /* ->> NUL terminator */ 1u] = "";
            std::size_t            messageOutputLength                                                 = 0u;
            char const            *messagePrefix                                                       = "";
            char const            *messageSuffix                                                       = "";
            struct parsecmd *const repeat                                                              = static_cast<struct parsecmd*>(information);
            std::FILE*           (*stream)()                                                           = NULL;

            // ... ->> Do not override/ repeat these commands
            if (NULL != repeat -> parsed.argument and (parsecmd::CLOCK == repeat -> command or parsecmd::LOG == repeat -> command))
            return false;

            // ...
            switch (option -> command) {
              case parsecmd::CLOCK: log = &(bool (&)(char const[], std::FILE*)) catalog -> warn; stream = &(std::FILE* (&)()) catalog -> warn; messagePrefix = "Multiple directories for the catalog's internal clock specified; Using duplicate `"; messageSuffix = "`"; break;
              case parsecmd::LOG  : log = &(bool (&)(char const[], std::FILE*)) catalog -> warn; stream = &(std::FILE* (&)()) catalog -> warn; messagePrefix = "Multiple directories for logs specified; Using duplicate `";                         messageSuffix = "`"; break;
              case parsecmd::RERUN: log = &(bool (&)(char const[], std::FILE*)) catalog -> log;  stream = &(std::FILE* (&)()) catalog -> log;  message       = "Okay! Okay! Restarting, alright?";                                                                        break;
              default:;
            }

            if (NULL != log) {
              for (char const *const texts[] = {messagePrefix, message, messageSuffix}, *const *text = texts; text != texts + (sizeof texts / sizeof(char const*)); ++text) {
                for (std::size_t index = 0u; MESSAGE_MAXIMUM_LENGTH != messageOutputLength and '\0' != (*text)[index]; ++index)
                messageOutput[messageOutputLength++] = (*text)[index];
              }

              messageOutput[messageOutputLength] = '\0';
              (void) log(messageOutput, stream());
            }

            // ... ->> Ignore previous repeated commands
            repeat -> parsed.argument = NULL;
          }

          // ... ->> Waits for its possibly `sequence`d command-line option before a successful `parse()`
          return NULL == sequence or NULL != sequence -> parsed.argument;
        }

        static char const* format(enum parsecmd::command const command) {
          switch (command) {
            case parsecmd::ABOUT: return "about";
            case parsecmd::CLOCK: return "clock";
            case parsecmd::HELP : return "help";
            case parsecmd::LOG  : return "log";
            case parsecmd::RERUN: return "rerun";
          }

          return "";
        }

        static bool undiagnose(char argument[], struct parsecmd const options[], std::size_t const optionsLength) {
          char        messageOutput[MESSAGE_MAXIMUM_LENGTH + /* ->> NUL terminator */ 1u] = "";
          std::size_t messageOutputLength                                                 = 0u;

          // TODO (Lapys) -> Support "????…??" as help command
          // TODO (Lapys) -> Consider Levenstein distance checking for closest-matching argument `https://www.geeksforgeeks.org/edit-distance-dp-5/`
          (void) options;

          // ...
          for (char const *const texts[] = {"Unrecognized argument `", argument, "`, use `--help` for supported commands\0;Did you mean `…`?"}, *const *text = texts; text != texts + (sizeof texts / sizeof(char const*)); ++text) {
            for (std::size_t index = 0u; MESSAGE_MAXIMUM_LENGTH != messageOutputLength and '\0' != (*text)[index]; ++index)
            messageOutput[messageOutputLength++] = (*text)[index];
          }

          messageOutput[messageOutputLength] = '\0';
          catalog -> warn(messageOutput);

          // ...
          return false;
        }
      } parse;

      struct parsecmd commandLineOptions[] = {
        {parsecmd::ABOUT, {"?"},  "about", {"?"},  PARSE_TRIMMED, 0u, NULL, &parse::diagnose, {}}, // --> 0
        {parsecmd::CLOCK, {NULL}, NULL,    {NULL}, PARSE_INITIAL, 0u, NULL, &parse::diagnose, {}}, // --> 1
        {parsecmd::CLOCK, {NULL}, "clock", {"c"},  PARSE_INITIAL, 0u, NULL, &parse::diagnose, {}}, // --> 2
        {parsecmd::CLOCK, {NULL}, "clock", {"c"},  PARSE_INITIAL, 1u, NULL, &parse::diagnose, {}}, // --> 3
        {parsecmd::CLOCK, {NULL}, NULL,    {"t"},  PARSE_INITIAL, 0u, NULL, &parse::diagnose, {}}, // --> 4
        {parsecmd::CLOCK, {NULL}, NULL,    {"t"},  PARSE_INITIAL, 1u, NULL, &parse::diagnose, {}}, // --> 5
        {parsecmd::HELP,  {NULL}, "help",  {"h"},  PARSE_INITIAL, 0u, NULL, &parse::diagnose, {}}, // --> 6
        {parsecmd::LOG,   {NULL}, NULL,    {NULL}, PARSE_INITIAL, 0u, NULL, &parse::diagnose, {}}, // --> 7
        {parsecmd::RERUN, {NULL}, "rerun", {"r"},  PARSE_INITIAL, 0u, NULL, &parse::diagnose, {}}  // --> 8
      };
      char const *const commandLineOptionLongTags [][2] = {{"--", "="}, {"/", ":"}};
      char const *const commandLineOptionShortTags[][2] = {{"-",  "="}, {"/", ":"}};
      std::size_t const commandLineOptionsLength        = sizeof commandLineOptions / sizeof *commandLineOptions;

      // ...
      catalog -> about                            = false;                                                            // --> parsecmd::ABOUT
      catalog -> applicationExitCode              = EXIT_SUCCESS;                                                     // ->> Used by `::quick_exit(…)` primarily
      catalog -> applicationName                  = "Cataloger";                                                      //
      catalog -> clocked                          = false;                                                            // ->> Catalog's internal clock is active
      catalog -> clockFileStream                  = NULL;                                                             // ->> File stream representing the Catalog's internal clock
      catalog -> clockInterval                    = 86400000u;                                                        // ->> Milliseconds
      catalog -> clockPath                        = NULL;                                                             // --> parsecmd::CLOCK
      catalog -> clockPathAllocated               = false;                                                            //
      catalog -> commandLineArguments             = arguments;                                                        // ->> Useful for determining how the standard C/ C++ command-line `arguments` were evaluated; Optionally `NULL`-terminated
      catalog -> commandLineArgumentsLength       = length;                                                           //
      catalog -> commandLineOptionLongTags        = commandLineOptionLongTags;                                        // ->> Prefix that denotes long command-line options interchangeably; Optionally `NULL`
      catalog -> commandLineOptionLongTagsLength  = sizeof commandLineOptionLongTags / sizeof(char const* const[2]);  //
      catalog -> commandLineOptionShortTags       = commandLineOptionShortTags;                                       // ->> Prefix that denotes short command-line options interchangeably; Optionally `NULL`
      catalog -> commandLineOptionShortTagsLength = sizeof commandLineOptionShortTags / sizeof(char const* const[2]); //
      catalog -> help                             = false;                                                            // --> parsecmd::HELP
      catalog -> logDirectoryPath                 = NULL;                                                             // --> parsecmd::LOG
      catalog -> messagePrefix                    = "cataloger" ": ";                                                 // ->> Used by `::log(…)` primarily; Optionally `NULL`
      catalog -> messageSuffix                    = "\r\n";                                                           // ->> Used by `::log(…)` primarily; Optionally `NULL`
      catalog -> rerun                            = false;                                                            // --> parsecmd::RERUN

      for (std::size_t index = 0u; commandLineOptionsLength != index; ++index)
        commandLineOptions[index].metadata = NULL;

      commandLineOptions[1].metadata = commandLineOptions + 7;

      #if defined __APPLE__ or defined __bsdi__ or defined __CYGWIN__ or defined __DragonFly__ or defined __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ or defined __FreeBSD__ or defined __FreeBSD_version or defined __gnu_linux__ or defined __linux or defined __linux__ or defined __MACH__ or defined __NETBSD__ or defined __NETBSD_version or defined __OpenBSD__ or defined __OS400__ or defined __QNX__ or defined __QNXNTO__ or defined __sun or defined __SVR4 or defined __svr4__ or defined __sysv__ or defined __unix or defined __unix__ or defined __VMS or defined __VMS_VER or defined _NTO_VERSION or defined _POSIX_SOURCE or defined _SYSTYPE_SVR4 or defined _XOPEN_SOURCE or defined linux or defined NetBSD0_8 or defined NetBSD0_9 or defined NetBSD1_0 or defined OpenBSD2_0 or defined OpenBSD2_1 or defined OpenBSD2_2 or defined OpenBSD2_3 or defined OpenBSD2_4 or defined OpenBSD2_5 or defined OpenBSD2_6 or defined OpenBSD2_7 or defined OpenBSD2_8 or defined OpenBSD2_9 or defined OpenBSD3_0 or defined OpenBSD3_1 or defined OpenBSD3_2 or defined OpenBSD3_3 or defined OpenBSD3_4 or defined OpenBSD3_5 or defined OpenBSD3_6 or defined OpenBSD3_7 or defined OpenBSD3_8 or defined OpenBSD3_9 or defined OpenBSD4_0 or defined OpenBSD4_1 or defined OpenBSD4_2 or defined OpenBSD4_3 or defined OpenBSD4_4 or defined OpenBSD4_5 or defined OpenBSD4_6 or defined OpenBSD4_7 or defined OpenBSD4_8 or defined OpenBSD4_9 or defined sun or defined unix or defined VMS
        /* ... ->> `dlsym()` */
      #elif defined __NT__ or defined __TOS_WIN__ or defined __WIN32__ or defined __WINDOWS__ or defined _WIN16 or defined _WIN32 or defined _WIN32_WCE or defined _WIN64
        LPWSTR* (*const CommandLineToArgvW) (LPCWSTR, int*)                                       = NULL == libraries[shell32] .moduleHandle ? NULL : reinterpret_cast<LPWSTR* (*)(LPCWSTR, int*)>                                      (::GetProcAddress(libraries[shell32] .moduleHandle, "CommandLineToArgvW"));  // --> <shellapi.h>
        LPWSTR  (*const GetCommandLineW)    ()                                                    = NULL == libraries[kernel32].moduleHandle ? NULL : reinterpret_cast<LPWSTR  (*)()>                                                   (::GetProcAddress(libraries[kernel32].moduleHandle, "GetCommandLineW"));     // --> <windows.h> → <processenv.h>
        BOOL    (*const GetModuleHandleExA) (DWORD, LPCSTR, HMODULE*)                             = NULL == libraries[kernel32].moduleHandle ? NULL : reinterpret_cast<BOOL    (*)(DWORD, LPCSTR, HMODULE*)>                            (::GetProcAddress(libraries[kernel32].moduleHandle, "GetModuleHandleExA"));  // --> <windows.h>
        HLOCAL  (*const LocalFree)          (HLOCAL)                                              = NULL == libraries[kernel32].moduleHandle ? NULL : reinterpret_cast<HLOCAL  (*)(HLOCAL)>                                             (::GetProcAddress(libraries[kernel32].moduleHandle, "LocalFree"));           // --> <windows.h>
        int     (*const WideCharToMultiByte)(UINT, DWORD, LPCWCH, int, LPSTR, int, LPCCH, LPBOOL) = NULL == libraries[kernel32].moduleHandle ? NULL : reinterpret_cast<int     (*)(UINT, DWORD, LPCWCH, int, LPSTR, int, LPCCH, LPBOOL)>(::GetProcAddress(libraries[kernel32].moduleHandle, "WideCharToMultiByte")); // --> <windows.h>

        // ... ->> Normalize standard C/ C++ command-line `arguments` to vendor-specific Windows `commandLineArguments`
        if (NULL != CommandLineToArgvW and NULL != GetCommandLineW) {
          int           commandLineArgumentsLength = 0;
          LPWSTR *const commandLineArguments       = CommandLineToArgvW(GetCommandLineW(), &commandLineArgumentsLength);

          // ...
          if (NULL != WideCharToMultiByte)
          if (NULL != commandLineArguments and static_cast<unsigned>(commandLineArgumentsLength) <= length) {
            char       *allocation = NULL, *iterator = NULL;
            bool        normalized = false;
            std::size_t size       = 0u;

            // ...
            for (int index = 0; commandLineArgumentsLength != index; ++index) {
              int const commandLineArgumentSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, commandLineArguments[index], -1, static_cast<LPSTR>(NULL), 0u, static_cast<LPCCH>(NULL), static_cast<LPBOOL>(NULL));

              // ...
              if (0 == commandLineArgumentSize) { size = 0u; break; }
              size += commandLineArgumentSize;
            }

            allocation = size ? /* ->> Likely memory leak */ ::new (std::nothrow) char[size = /* ->> Non-NUL characters */ (size / sizeof(char)) + /* ->> NUL terminators */ (commandLineArgumentsLength * sizeof(char))] : NULL;
            iterator   = allocation;

            // ... --> catalog -> commandLineArguments[…] = …;
            if (NULL != allocation)
            for (int index = 0; commandLineArgumentsLength != index; ++index) {
              int const commandLineArgumentSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, commandLineArguments[index], -1, iterator, size, static_cast<LPCCH>(NULL), static_cast<LPBOOL>(NULL));

              if (0 != commandLineArgumentSize) {
                catalog -> commandLineArguments[index] = iterator; // ->> Normalize standard C/ C++ command-line argument
                iterator                              += commandLineArgumentSize;
                normalized                             = true;
              }
            }

            if (not normalized)
            ::delete[] allocation;
          }

          if (NULL != LocalFree)
          (void) LocalFree(commandLineArguments);
        }

        // ... ->> Ignore the standard C/ C++ command-line string used to execute this application
        if (NULL != GetModuleHandleExA) {
          HMODULE applicationModuleHandle = NULL;
          HMODULE argumentModuleHandle    = NULL;

          // ...
          if (
            FALSE !=   GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, catalog -> commandLineArguments[0], &argumentModuleHandle) and
            FALSE != ::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, static_cast<LPWSTR>(NULL),          &applicationModuleHandle)
          ) {
            catalog -> commandLineArguments       += applicationModuleHandle == argumentModuleHandle;
            catalog -> commandLineArgumentsLength -= applicationModuleHandle == argumentModuleHandle;
          }
        }
      #endif

      // ... ->> Parse then interpret command-line options
      //     TODO (Lapys) -> Why the heck does `--rerun` not work?!
      if (not catalog -> parse(catalog -> commandLineArguments, catalog -> commandLineArgumentsLength, commandLineOptions, commandLineOptionsLength, &parse::undiagnose))
      catalog -> exit(EXIT_FAILURE);

      for (struct options /* final */ {
        enum { valued, unvalued };

        char const    *warningPrefix;
        std::ptrdiff_t indexes[6];
      } const options[] = {
        {"Expected value on ",   {2, 4,          -1}}, // ->> `options::valued`   command-line option
        {"Unexpected value on ", {0, 3, 5, 6, 8, -1}}  // ->> `options::unvalued` command-line option
      }, *option = options; option != options + (sizeof options / sizeof *options); ++option) {
        for (std::ptrdiff_t const *index = option -> indexes; index != option -> indexes + (sizeof option -> indexes / sizeof(std::ptrdiff_t)) and *index != -1; ++index)
        if (NULL != commandLineOptions[*index].parsed.argument and (
          option == options + options::valued   ? commandLineOptions[*index].parsed.offsets.value <= -1 :
          option == options + options::unvalued ? commandLineOptions[*index].parsed.offsets.value >  -1 :
          false
        )) {
          char              warningOutput[MESSAGE_MAXIMUM_LENGTH + /* ->> NUL terminator */ 1u];
          char       *const warningArgument     = commandLineOptions[*index].parsed.argument;
          char const *const warningCommand      = parse::format(commandLineOptions[*index].command);
          std::size_t       warningOutputLength = 0u;
          char const        warningSuffix[]     = "`";

          // ...
          for (char const *const texts[] = {option -> warningPrefix, warningCommand, " option `", warningArgument, warningSuffix}, *const *text = texts; text != texts + (sizeof texts / sizeof(char const*)); ++text) {
            for (std::size_t subindex = 0u; MESSAGE_MAXIMUM_LENGTH != warningOutputLength and '\0' != (*text)[subindex]; ++subindex)
            warningOutput[warningOutputLength++] = (*text)[subindex];
          }

          warningOutput[warningOutputLength] = '\0';
          (void) catalog -> warn(warningOutput);
          (void) catalog -> exit(EXIT_FAILURE);
        }
      }

      for (std::size_t index = 0u; commandLineOptionsLength != index; ++index)
      if (NULL != commandLineOptions[index].parsed.argument) {
        switch (commandLineOptions[index].command) {
          case parsecmd::ABOUT: catalog -> about            = true;                                      break;
          case parsecmd::HELP : catalog -> help             = true;                                      break;
          case parsecmd::LOG  : catalog -> logDirectoryPath = commandLineOptions[index].parsed.argument; break;
          case parsecmd::RERUN: catalog -> rerun            = true;                                      break;

          case parsecmd::CLOCK: {
            catalog -> clockPath = commandLineOptions[index].parsed.argument + (commandLineOptions[index].parsed.offsets.value > -1 ? commandLineOptions[index].parsed.offsets.value : 0);

            if (0u != commandLineOptions[index].count) {
              std::size_t const count = commandLineOptions[index].count; // ->> Assume `1zu`

              // ...
              for (std::size_t subindex = 0u; catalog -> commandLineArgumentsLength != subindex; ++subindex)
              if (catalog -> commandLineArguments[subindex] == commandLineOptions[index].parsed.argument) {
                catalog -> clockPath = catalog -> commandLineArgumentsLength > subindex + count ? catalog -> commandLineArguments[subindex + count] : NULL;
                break;
              }
            }

            // ... ->> Assert the [clock_path] immediately proceeds the [log_directory] (or has a value)
            for (std::size_t subindex = 0u; catalog -> commandLineArgumentsLength != subindex; ++subindex)
            if (catalog -> commandLineArguments[subindex] == commandLineOptions[index].parsed.argument) {
              if (catalog -> commandLineArgumentsLength <= subindex + commandLineOptions[index].count) {
                (void) catalog -> warn("Expected value for the catalog's internal clock directory");
                catalog -> clockPath = NULL;
              }

              if (0u == subindex) {
                (void) catalog -> warn("Expected directory for logs before directory for the catalog's internal clock");
                catalog -> clockPath = NULL;
              }

              else
                for (std::size_t subsubindex = 0u; commandLineOptionsLength != subsubindex; ++subsubindex)
                if (parsecmd::LOG == commandLineOptions[subsubindex].command and catalog -> commandLineArguments[subindex - 1u] != commandLineOptions[subsubindex].parsed.argument) {
                  (void) catalog -> warn("Expected directory for logs immediately before directory for the catalog's internal clock");
                  catalog -> clockPath = NULL;
                }

              // ...
              break;
            }
          } break;
        }
      }

      std::printf("[..]: {about: %4.5s, clock: \"%s\", help: %4.5s, log: \"%s\", rerun: %4.5s}" "\r\n",
        catalog -> about ? "true" : "false",
        NULL == catalog -> clockPath ? "..." : catalog -> clockPath,
        catalog -> help ? "true" : "false",
        NULL == catalog -> logDirectoryPath ? "..." : catalog -> logDirectoryPath,
        catalog -> rerun ? "true" : "false"
      );

      // ... ->> Handle unexpected application `error()`
      (void) std::signal   (SIGTERM, &catalog -> error); //
      (void) std::signal   (SIGSEGV, &catalog -> error); // ->> ¯\_(ツ)_/¯
      (void) std::signal   (SIGINT,  &catalog -> error); // ->> Handle possible user interrupts
      (void) std::signal   (SIGABRT, &catalog -> error); // ->> Handle `std::abort(…)` and `std::terminate(…)` (and `throw` expression) invocations
      (void) std::setlocale(LC_ALL,  "C.UTF-8");         //
      #if __cplusplus >= 201103L                         //
        (void) std::at_quick_exit(&catalog -> quick_exit);
      #endif
    }

    static void update() {
      // ... ->> Ideally runs every `::clockInterval` elapsed
      return;
    }

    static bool warn(void const* const message, std::FILE* const stream, std::size_t const stride) {
      union formatinfo messageFormatInformation = {};
      bool             messageFormatted         = false;
      bool             messageOutputted         = false;

      // ...
      messageFormatted = catalog -> format(stream, FORMAT_ERROR, &messageFormatInformation);
      messageOutputted = catalog -> log   (message, stream, stride);

      if (messageFormatted)
      (void) catalog -> format(stream, FORMAT_INITIAL, &messageFormatInformation);

      // ...
      return messageOutputted;
    }
      static std::FILE* warn()                                                          { return stderr; }
      static bool       warn(char    const message[], std::FILE* const stream = warn()) { return catalog -> warn(message, stream, sizeof(char)); }
      static bool       warn(wchar_t const message[], std::FILE* const stream = warn()) { return catalog -> warn(message, stream, sizeof(wchar_t)); }

    /* ... */
    bool                about;
    int                 applicationExitCode;
    char const         *applicationName;
    bool                clocked;
    std::FILE          *clockFileStream;
    std::size_t         clockInterval;
    char               *clockPath;
    bool                clockPathAllocated;
    char              **commandLineArguments;
    std::size_t         commandLineArgumentsLength;
    char const *const (*commandLineOptionLongTags)[2];
    std::size_t         commandLineOptionLongTagsLength;
    char const *const (*commandLineOptionShortTags)[2];
    std::size_t         commandLineOptionShortTagsLength;
    bool                help;
    char const         *logDirectoryPath;
    char const         *messagePrefix;
    char const         *messageSuffix;
    bool                rerun;
  } c = {};

  catalog = &c;                       //
  catalog -> start(arguments, count); // ->> Initialization
  return EXIT_SUCCESS;

  // ... --> catalog -> logDirectoryPath = …;
  if (NULL == catalog -> logDirectoryPath) {
    (void) catalog -> log("No directory for logs specified; Defaulting to the current working directory instead");
    catalog -> logDirectoryPath = "./";
  }

  // ... --> catalog -> clockFileStream = …;
  if (NULL == catalog -> clockPath) {
    (void) catalog -> log("No directory for the catalog's internal clock specified; Defaulting to fallback instead");
    catalog -> clockFileStream = std::tmpfile();
  } else catalog -> clockFileStream = std::fopen(catalog -> clockPath, "wb+");

  if (NULL == catalog -> clockFileStream and NULL != catalog -> clockPath) {
    char        catalogClockFileNameFallback[L_tmpnam + 1u] = {};    //
    std::size_t catalogClockFileNameFallbackLength          = 0u;    //
    char       *catalogClockPathBuffer                      = NULL;  // ->> Non-null when `::clockPathAllocated` and assigned to `::clockPath` afterward
    std::size_t catalogClockPathLength                      = 0u;    //
    char const *catalogClockPathPrefix                      = "";    //
    bool        catalogClockPathPrefixed                    = false; // ->> Determines `::clockPath` (significantly) begins with `catalogClockPathPrefix`
    std::size_t catalogClockPathPrefixLength                = 0u;    //
    bool        catalogClockPathSuffixed                    = false; // ->> Determines `::clockPath` (significantly) ends   with `pathDelimiters[…]`
    char const  pathDelimiters[]                            = {'/'
      #if defined __NT__ or defined __TOS_WIN__ or defined __WIN32__ or defined __WINDOWS__ or defined _WIN16 or defined _WIN32 or defined _WIN32_WCE or defined _WIN64
        , '\\'
      #endif
    };
    std::size_t const pathDelimitersLength = sizeof pathDelimiters / sizeof(char);

    // ... --> catalogClockFileNameFallback       = std::strrchr(std::tmpnam(…) ?? …, '/' | …);
    //         catalogClockFileNameFallbackLength = std::strlen (catalogClockFileNameFallback);
    if (catalogClockFileNameFallback != std::tmpnam(catalogClockFileNameFallback)) {
      char        const fallback[]     = "cataloger-clock.dat";
      std::size_t const fallbackLength = (sizeof fallback / sizeof(char)) - 1u;

      // ...
      for (std::size_t index = 0u; ; ++index) {
        catalogClockFileNameFallback[index] = fallback[index];

        if (fallbackLength == index or index == (sizeof catalogClockFileNameFallback / sizeof(char)) - 1u) {
          catalogClockFileNameFallback[index] = '\0';
          break;
        }
      }
    }

    while ('\0' != catalogClockFileNameFallback[catalogClockFileNameFallbackLength])
    ++catalogClockFileNameFallbackLength;

    for (std::size_t index = catalogClockFileNameFallbackLength; index--; ) {
      // ... ->> Unique names only, rather than unique file paths
      for (std::size_t subindex = 0u; pathDelimitersLength != subindex; ++subindex)
      if (catalogClockFileNameFallback[index] == pathDelimiters[subindex]) {
        catalogClockFileNameFallbackLength -= ++index;

        for (subindex = 0u; subindex <= catalogClockFileNameFallbackLength; ++index, ++subindex)
        catalogClockFileNameFallback[subindex] = catalogClockFileNameFallback[index];

        index = 0u;
        break;
      }
    }

    // ... --> catalogClockPathLength   = std::strlen(catalog -> clockPath | …);
    //         catalogClockPathSuffixed = …;
    for (; '\0' != catalog -> clockPath[catalogClockPathLength]; ++catalogClockPathLength) {
      for (std::size_t subindex = 0u; pathDelimitersLength != subindex; ++subindex)
      if (catalog -> clockPath[catalogClockPathLength] == pathDelimiters[subindex]) {
        for (char *iterator = catalog -> clockPath + (catalogClockPathLength + 1u); ; ++iterator) {
          switch (*iterator) {
            case '\f': case '\n': case '\r': case '\t': case '\v': case ' ':
            continue;
          }

          catalogClockPathSuffixed = '\0' == *iterator;
          break;
        }
      }

      if (catalogClockPathSuffixed)
      break;
    }

    // ... --> catalogClockPathPrefix… = …;
    #if defined __NT__ or defined __TOS_WIN__ or defined __WIN32__ or defined __WINDOWS__ or defined _WIN16 or defined _WIN32 or defined _WIN32_WCE or defined _WIN64
      catalogClockPathPrefix       = "\\\\?\\";
      catalogClockPathPrefixLength = MAX_PATH < catalogClockPathLength + /* ->> Path delimiter */ 1u + catalogClockFileNameFallbackLength + /* ->> NUL terminator */ 1u ? 3u : 0u;
    #endif

    // ... --> catalogClockPathPrefixed = …;
    for (std::size_t clockIndex = 0u, prefixIndex = 0u; ; ++clockIndex) {
      if (catalogClockPathPrefixLength == prefixIndex) {
        catalogClockPathPrefixed     = true;
        catalogClockPathPrefixLength = 0u;

        break;
      }

      switch (catalog -> clockPath[clockIndex]) {
        case '\f': case '\n': case '\r': case '\t': case '\v': case ' ':
        continue;
      }

      if (catalog -> clockPath[clockIndex] != catalogClockPathPrefix[prefixIndex++])
      break;
    }

    // ... --> catalogClockPathBuffer = …; ->> Just resizing
    catalogClockPathBuffer = ::new (std::nothrow) char[catalogClockPathPrefixLength + catalogClockPathLength + /* ->> Path delimiter */ 1u + catalogClockFileNameFallbackLength + /* ->> NUL terminator */ 1u];

    if (NULL == catalogClockPathBuffer) {
      if (not catalogClockPathPrefixed) {
        catalogClockPathBuffer             = ::new (std::nothrow) char[catalogClockPathPrefixLength + catalogClockPathLength + /* ->> NUL terminator */ 1u];
        catalogClockFileNameFallbackLength = 0u;
      }

      else if (not catalogClockPathSuffixed) {
        (void) catalog -> warn("Cataloger exited; No file for the catalog's internal clock specified");
        return catalog -> exit(EXIT_FAILURE);
      }

      else
        catalogClockPathBuffer = ::new (std::nothrow) char[catalogClockPathLength + /* ->> Path delimiter */ 1u + catalogClockFileNameFallbackLength + /* ->> NUL terminator */ 1u];
    }

    if (NULL == catalogClockPathBuffer) {
      catalogClockPathBuffer       = ::new (std::nothrow) char[catalogClockPathLength + /* ->> NUL terminator */ 1u];
      catalogClockPathPrefixLength = 0u;

      if (NULL == catalogClockPathBuffer) {
        (void) catalog -> warn("Cataloger exited; No file for the catalog's internal clock specified");
        return catalog -> exit(EXIT_FAILURE);
      }
    }

    catalogClockPathBuffer[catalogClockPathLength] = '\0';

    for (std::size_t index = 0u; index != catalogClockPathLength; ++index)
    catalogClockPathBuffer[index] = catalog -> clockPath[index] != '/' ? catalog -> clockPath[index] : '\\';

    // ... --> catalog -> clockFileStream = …;
    //         catalog -> clockPath…      = …;
    catalog -> clockPathAllocated = true;
    catalog -> clockPath          = catalogClockPathBuffer;
    catalog -> clockFileStream    = std::fopen(catalog -> clockPath, "wb+");

    if (NULL == catalog -> clockFileStream)
    if (0u != catalogClockPathPrefixLength) {
      for (std::size_t index = catalogClockPathLength; index--; )
        catalog -> clockPath[catalogClockPathPrefixLength + index] = catalog -> clockPath[index];

      for (std::size_t index = catalogClockPathPrefixLength; index--; )
        catalog -> clockPath[index] = catalogClockPathPrefix[index];

      catalogClockPathLength    += catalogClockPathPrefixLength;
      catalog -> clockFileStream = std::fopen(catalog -> clockPath, "wb+");
    }

    if (NULL == catalog -> clockFileStream)
    if (0u != catalogClockFileNameFallbackLength) {
      catalog -> clockPath[catalogClockPathLength]                                           = '\\';
      catalog -> clockPath[catalogClockPathLength + catalogClockFileNameFallbackLength + 1u] = '\0';

      for (std::size_t index = 0u; index != catalogClockFileNameFallbackLength; ++index)
        catalog -> clockPath[catalogClockPathLength + index + 1u] = catalogClockFileNameFallback[index];

      catalogClockPathLength    += catalogClockFileNameFallbackLength + 1u;
      catalog -> clockFileStream = std::fopen(catalog -> clockPath, "wb+");
    }
  }

  // ...
  if (NULL == catalog -> clockFileStream) {
    (void) catalog -> warn("Cataloger exited; No file for the catalog's internal clock specified");
    return catalog -> exit(EXIT_FAILURE);
  }

  /* ... ->> Execute internal clock */
  if (catalog -> rerun)
    catalog -> update();

  else {
  #if false
    #if defined __APPLE__ or defined __bsdi__ or defined __CYGWIN__ or defined __DragonFly__ or defined __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ or defined __FreeBSD__ or defined __FreeBSD_version or defined __gnu_linux__ or defined __linux or defined __linux__ or defined __MACH__ or defined __NETBSD__ or defined __NETBSD_version or defined __OpenBSD__ or defined __OS400__ or defined __QNX__ or defined __QNXNTO__ or defined __sun or defined __SVR4 or defined __svr4__ or defined __sysv__ or defined __unix or defined __unix__ or defined __VMS or defined __VMS_VER or defined _NTO_VERSION or defined _POSIX_SOURCE or defined _SYSTYPE_SVR4 or defined _XOPEN_SOURCE or defined linux or defined NetBSD0_8 or defined NetBSD0_9 or defined NetBSD1_0 or defined OpenBSD2_0 or defined OpenBSD2_1 or defined OpenBSD2_2 or defined OpenBSD2_3 or defined OpenBSD2_4 or defined OpenBSD2_5 or defined OpenBSD2_6 or defined OpenBSD2_7 or defined OpenBSD2_8 or defined OpenBSD2_9 or defined OpenBSD3_0 or defined OpenBSD3_1 or defined OpenBSD3_2 or defined OpenBSD3_3 or defined OpenBSD3_4 or defined OpenBSD3_5 or defined OpenBSD3_6 or defined OpenBSD3_7 or defined OpenBSD3_8 or defined OpenBSD3_9 or defined OpenBSD4_0 or defined OpenBSD4_1 or defined OpenBSD4_2 or defined OpenBSD4_3 or defined OpenBSD4_4 or defined OpenBSD4_5 or defined OpenBSD4_6 or defined OpenBSD4_7 or defined OpenBSD4_8 or defined OpenBSD4_9 or defined sun or defined unix or defined VMS
      // TODO (Lapys) -> Linux service → `usleep(…)` → `nanosleep(…)`
    #elif defined __NT__ or defined __TOS_WIN__ or defined __WIN32__ or defined __WINDOWS__ or defined _WIN16 or defined _WIN32 or defined _WIN32_WCE or defined _WIN64
      DWORD     (*const GetModuleFileNameW) (HMODULE, LPWSTR, DWORD)               = NULL == libraries[kernel32].moduleHandle ? NULL : reinterpret_cast<DWORD (*)(HMODULE, LPWSTR, DWORD)>              (::GetProcAddress(libraries[kernel32].moduleHandle, "GetModuleFileNameW"));  // --> <windows.h>
      int       (*const MultiByteToWideChar)(UINT, DWORD, LPCCH, int, LPWSTR, int) = NULL == libraries[kernel32].moduleHandle ? NULL : reinterpret_cast<int   (*)(UINT, DWORD, LPCCH, int, LPWSTR, int)>(::GetProcAddress(libraries[kernel32].moduleHandle, "MultiByteToWideChar")); // --> <windows.h>
      WCHAR             applicationFilePath[MAX_PATH + 1u]                         = {};
      std::size_t const applicationFilePathLength                                  = sizeof applicationFilePath / sizeof(WCHAR);
      static WCHAR      applicationName[]                                          = L"Cataloger"; // --> ::MultiByteToWideChar(…, catalog -> applicationName, …)
      char             *programPointer                                             = NULL; // --> ::_pgmptr
      wchar_t          *programPointerW                                            = NULL; // --> ::_wpgmptr

      // ... --> programPointer… = ::_get_…pgmptr(…);
      programPointer  = 0 != ::_get_pgmptr (&programPointer)  ? arguments + 1 == catalog -> commandLineArguments ? arguments[0] : NULL : programPointer;
      programPointerW = 0 != ::_get_wpgmptr(&programPointerW) ?                                                                   NULL : programPointerW;

      // ... --> applicationFilePath = …;
      if (NULL == GetModuleFileNameW or FALSE == GetModuleFileNameW(static_cast<HMODULE>(NULL), applicationFilePath, applicationFilePathLength)) {
        applicationFilePath[0] = L'\0';

        // ...
        if (NULL != programPointerW)
        for (std::size_t index = 0u; ; ++index) {
          applicationFilePath[index] = programPointerW[index];
          if (L'\0' == programPointerW[index]) break;
        }

        else if (NULL != programPointer) {
          if (NULL != MultiByteToWideChar and applicationFilePathLength >= static_cast<unsigned>(MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, programPointer, -1, static_cast<LPWSTR>(NULL), 0))) {
            if (0 == MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, programPointer, -1, applicationFilePath, applicationFilePathLength))
            applicationFilePath[0] = L'\0';
          }
        }
      }

      applicationFilePath[MAX_PATH] = L'\0';

      /* ... ->> Windows Task Scheduler --- TODO (Lapys) -> Remove `false` condition */
      #if _WIN32_WINNT >= 0x0600
        if (false and not catalog -> clocked)
        if (NULL != libraries[credui].moduleHandle and NULL != libraries[kernel32].moduleHandle and NULL != libraries[ole32].moduleHandle and NULL != libraries[oleaut32].moduleHandle and NULL != libraries[taskschd].moduleHandle) {
          HRESULT (*const CoCreateInstance)                  (REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID)                                                         = reinterpret_cast<HRESULT         (*)(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID)>                                                        (::GetProcAddress(libraries[ole32]   .moduleHandle, "CoCreateInstance"));                   // --> <objbase.h>
          HRESULT (*const CoInitializeEx)                    (LPVOID, DWORD)                                                                                      = reinterpret_cast<HRESULT         (*)(LPVOID, DWORD)>                                                                                     (::GetProcAddress(libraries[ole32]   .moduleHandle, "CoInitializeEx"));                     // --> <objbase.h>
          HRESULT (*const CoInitializeSecurity)              (PSECURITY_DESCRIPTOR, LONG, SOLE_AUTHENTICATION_SERVICE*, void*, DWORD, DWORD, void*, DWORD, void*) = reinterpret_cast<HRESULT         (*)(PSECURITY_DESCRIPTOR, LONG, SOLE_AUTHENTICATION_SERVICE*, void*, DWORD, DWORD, void*, DWORD, void*)>(::GetProcAddress(libraries[ole32]   .moduleHandle, "CoInitializeSecurity"));               // --> <objbase.h>
          void    (*const CoUninitialize)                    ()                                                                                                   = reinterpret_cast<void            (*)()>                                                                                                  (::GetProcAddress(libraries[ole32]   .moduleHandle, "CoUninitialize"));                     // --> <objbase.h>
          DWORD   (*const CredUIPromptForCredentialsW)       (PCREDUI_INFOW, PCWSTR, PCtxtHandle, DWORD, PWSTR, ULONG, PWSTR, ULONG, BOOL*, DWORD)                = reinterpret_cast<CREDUIAPI DWORD (*)(PCREDUI_INFOW, PCWSTR, PCtxtHandle, DWORD, PWSTR, ULONG, PWSTR, ULONG, BOOL*, DWORD)>               (::GetProcAddress(libraries[credui]  .moduleHandle, "CredUIPromptForCredentialsW"));        // --> <wincred.h>
          DWORD   (*const CredUIPromptForWindowsCredentialsW)(PCREDUI_INFOW, DWORD, ULONG*, LPCVOID, ULONG, LPVOID*, ULONG*, BOOL*, DWORD)                        = reinterpret_cast<CREDUIAPI DWORD (*)(PCREDUI_INFOW, DWORD, ULONG*, LPCVOID, ULONG, LPVOID*, ULONG*, BOOL*, DWORD)>                       (::GetProcAddress(libraries[credui]  .moduleHandle, "CredUIPromptForWindowsCredentialsW")); // --> <wincred.h>
          BOOL    (*const CredUnPackAuthenticationBufferW)   (DWORD, PVOID, DWORD, LPWSTR, DWORD*, LPWSTR, DWORD*, LPWSTR, DWORD*)                                = reinterpret_cast<CREDUIAPI BOOL  (*)(DWORD, PVOID, DWORD, LPWSTR, DWORD*, LPWSTR, DWORD*, LPWSTR, DWORD*)>                               (::GetProcAddress(libraries[credui]  .moduleHandle, "CredUnPackAuthenticationBufferW"));    // --> <wincred.h>
          LPWSTR  (*const GetCommandLineW)                   ()                                                                                                   = reinterpret_cast<LPWSTR          (*)()>                                                                                                  (::GetProcAddress(libraries[kernel32].moduleHandle, "GetCommandLineW"));                    // --> <windows.h> → <processenv.h>
          BSTR    (*const SysAllocString)                    (OLECHAR const*)                                                                                     = reinterpret_cast<BSTR            (*)(OLECHAR const*)>                                                                                    (::GetProcAddress(libraries[oleaut32].moduleHandle, "SysAllocString"));                     // --> <oleauto.h>
          void    (*const SysFreeString)                     (BSTR)                                                                                               = reinterpret_cast<void            (*)(BSTR)>                                                                                              (::GetProcAddress(libraries[oleaut32].moduleHandle, "SysFreeString"));                      // --> <oleauto.h>
          HRESULT (*const VariantClear)                      (VARIANTARG*)                                                                                        = reinterpret_cast<HRESULT         (*)(VARIANTARG*)>                                                                                       (::GetProcAddress(libraries[oleaut32].moduleHandle, "VariantClear"));                       // --> <oleauto.h>
          void    (*const VariantInit)                       (VARIANTARG*)                                                                                        = reinterpret_cast<void            (*)(VARIANTARG*)>                                                                                       (::GetProcAddress(libraries[oleaut32].moduleHandle, "VariantInit"));                        // --> <oleauto.h>

          // ... -> Derived from `https://learn.microsoft.com/en-us/windows/win32/taskschd/daily-trigger-example--c---`
          if (NULL != CoCreateInstance and NULL != CoInitializeEx and NULL != CoInitializeSecurity and NULL != CoUninitialize and NULL != SysAllocString and NULL != VariantInit and (NULL != CredUIPromptForCredentialsW or (NULL != CredUIPromptForWindowsCredentialsW and NULL != CredUnPackAuthenticationBufferW)))
          if (SUCCEEDED(CoInitializeEx(static_cast<LPVOID>(NULL), ::COINIT_MULTITHREADED))) {
            #ifdef __DATE__ // --> "Mmm DD YYYY"
              char const DATE[12] = __DATE__; // --> std::asctime(…)
            #else
              char const DATE[] = "Jan 00 2024";
            #endif
            #ifdef __TIME__ // --> "HH:MM:SS"
              char const TIME[9] = __TIME__;
            #else
              char const TIME[] = "12:00:00";
            #endif
            LPVOID              credentialsAuthenticationBuffer                             = NULL;                                                   // --> CredUIPromptForWindowsCredentialsW(…)
            ULONG               credentialsAuthenticationBufferSize                         = 0u;                                                     // --> CredUIPromptForWindowsCredentialsW(…)
            ULONG               credentialsAuthenticationPackage                            = 0x00u;                                                  // --> CredUIPromptForWindowsCredentialsW(…)
            WCHAR               credentialsDomainName[CREDUI_MAX_DOMAIN_TARGET_LENGTH + 1u] = L"";                                                    // --> CredUnPackAuthenticationBufferW   (…)
            DWORD               credentialsDomainNameSize                                   = sizeof credentialsDomainName / sizeof(WCHAR);           // --> CredUnPackAuthenticationBufferW   (…)
            WCHAR               credentialsPassword[CREDUI_MAX_PASSWORD_LENGTH + 1u]        = L"";                                                    // --> CredUIPromptForCredentialsW(…) or CredUnPackAuthenticationBufferW(…)
            DWORD               credentialsPasswordSize                                     = sizeof credentialsPassword / sizeof(WCHAR);             // --> CredUIPromptForCredentialsW(…) or CredUnPackAuthenticationBufferW(…)
            WCHAR const *const  credentialsPromptCaption                                    = applicationName;                                        // --> WCHAR[CREDUI_MAX_CAPTION_LENGTH]
            WCHAR const         credentialsPromptMessage[CREDUI_MAX_MESSAGE_LENGTH]         = L"Set up internal clock via the Task Scheduler ⏱️";     //
            WCHAR               credentialsUsername[CREDUI_MAX_USERNAME_LENGTH + 1u]        = L"";                                                    // --> CredUIPromptForCredentialsW(…) or CredUnPackAuthenticationBufferW(…)
            DWORD               credentialsUsernameSize                                     = sizeof credentialsUsername / sizeof(WCHAR);             // --> CredUIPromptForCredentialsW(…) or CredUnPackAuthenticationBufferW(…)
            CREDUI_INFOW        credentialsUIInformation                                    = {};                                                     //
            IRegisteredTask    *registeredTask                                              = NULL;                                                   // ->> Must remain `NULL`
            int                 standardInputDescriptor                                     = -1;                                                     //
            bool                standardInputIsTeletype                                     = false;                                                  //
            bool                standardInputNewlined                                       = true;                                                   //
            IAction            *taskAction                                                  = NULL;                                                   //
            IActionCollection  *taskActionCollection                                        = NULL;                                                   //
            BSTR const          taskAuthor                                                  = SysAllocString(L"LapysDev");                            //
            BSTR const          taskDescription                                             = SysAllocString(L"Cataloger internal clock ⏱️");         //
            void               *taskDailyTrigger                                            = NULL;                                                   // --> IDailyTrigger*
            short const         taskDailyTriggerInterval                                    = 1;                                                      //
            BSTR  const         taskDailyTriggerName                                        = SysAllocString(L"checkup");                             //
            BSTR                taskDailyTriggerTimeEnd                                     = NULL;                                                   //
            WCHAR               taskDailyTriggerTimeEndBuffer[]                             = L"20\0\0-01-01" "T" "\0\0:\0\0:\0\0";                   // --> YYYY-MM-DDTHH:MM:SS(+-)(timezone)
            BSTR                taskDailyTriggerTimeStart                                   = NULL;                                                   //
            WCHAR               taskDailyTriggerTimeStartBuffer[]                           = L"20\0\0-01-01" "T" "\0\0:\0\0:\0\0";                   // --> YYYY-MM-DDTHH:MM:SS(+-)(timezone)
            ITaskDefinition    *taskDefinition                                              = NULL;                                                   //
            void               *taskExecutableAction                                        = NULL;                                                   // --> IExecAction*
            BSTR                taskExecutableArguments                                     = NULL;                                                   // --> GetCommandLineW(…)
            WCHAR              *taskExecutableArgumentsBuffer                               = NULL;                                                   //
            BSTR                taskExecutablePath                                          = NULL;                                                   // --> GetModuleFileNameW(…)
            ITaskFolder        *taskFolder                                                  = NULL;                                                   //
            VARIANT             taskFolderPassword                                          = {};                                                     //
            BSTR const          taskFolderPath                                              = SysAllocString(L"\\");                                  // ->> Root folder
            VARIANT             taskFolderSecurityDescriptor                                = {};                                                     //
            VARIANT             taskFolderUsername                                          = {};                                                     //
            BSTR const          taskName                                                    = SysAllocString(applicationName);                        //
            IRegistrationInfo  *taskRegistrationInformation                                 = NULL;                                                   //
            IRepetitionPattern *taskRepititionPattern                                       = NULL;                                                   //
            BSTR const          taskRepititionPatternDuration                               = SysAllocString(L"PT4M");                                // ->> `https://learn.microsoft.com/en-us/windows/win32/taskschd/taskschedulerschema-duration-repetitiontype-element`
            BSTR const          taskRepititionPatternInterval                               = SysAllocString(L"PT1M");                                // ->> `https://learn.microsoft.com/en-us/windows/win32/taskschd/taskschedulerschema-interval-repetitiontype-element`
            void               *taskService                                                 = NULL;                                                   // --> ITaskService*
            VARIANT             taskServiceDomain                                           = {};                                                     //
            VARIANT             taskServicePassword                                         = {};                                                     //
            VARIANT             taskServiceServerName                                       = {};                                                     //
            VARIANT             taskServiceUser                                             = {};                                                     //
            ITrigger           *taskTrigger                                                 = NULL;                                                   //
            ITriggerCollection *taskTriggerCollection                                       = NULL;                                                   //
            struct {
              std::time_t const value;                //
              unsigned int      year;                 // --> YYYY
              unsigned int      hour, minute, second; // --> HH:MM:SS
            } time = {std::time(static_cast<std::time_t*>(NULL)), 0000u, 00u, 00u, 00u};

            // ... --> time = …;
            for (struct { char const *text; unsigned int *time; std::size_t begin, end; } const indexes[] = {
              {TIME, &time.hour,   0u, 1u},
              {TIME, &time.minute, 3u, 4u},
              {TIME, &time.second, 6u, 7u},
              {DATE, &time.year,   7u, 10u}
            }, *index = indexes; index != indexes + (sizeof indexes / sizeof *indexes); ++index) {
              for (std::size_t subindex = index -> end + 1u; index -> begin != subindex--; )
              switch (index -> text[subindex]) {
                case '0': *(index -> time) = (*(index -> time) * 10u) + 0u; break;
                case '1': *(index -> time) = (*(index -> time) * 10u) + 1u; break;
                case '2': *(index -> time) = (*(index -> time) * 10u) + 2u; break;
                case '3': *(index -> time) = (*(index -> time) * 10u) + 3u; break;
                case '4': *(index -> time) = (*(index -> time) * 10u) + 4u; break;
                case '5': *(index -> time) = (*(index -> time) * 10u) + 5u; break;
                case '6': *(index -> time) = (*(index -> time) * 10u) + 6u; break;
                case '7': *(index -> time) = (*(index -> time) * 10u) + 7u; break;
                case '8': *(index -> time) = (*(index -> time) * 10u) + 8u; break;
                case '9': *(index -> time) = (*(index -> time) * 10u) + 9u; break;
              }
            }

            if (time.value != static_cast<std::time_t>(-1)) {
              struct std::tm *const calenderTime = std::gmtime(&time.value);

              if (NULL != calenderTime) {
                time.hour   = calenderTime -> tm_hour;
                time.minute = calenderTime -> tm_min;
                time.second = calenderTime -> tm_sec  % /* ->> Correct for C89 leap-second defect */ 61;
                time.year   = calenderTime -> tm_year + 1900;
              }
            }

            // ... --> taskExecutableArguments… = …;
            if (NULL != GetCommandLineW) {
              LPWSTR const      commandLine                    = GetCommandLineW();
              std::size_t       commandLineLength              = 0u;
              WCHAR       const commandLineRerunArgument[]     = L"--rerun";
              std::size_t const commandLineRerunArgumentLength = (sizeof commandLineRerunArgument / sizeof(WCHAR)) - 1u;

              // ...
              while (L'\0' != commandLine[commandLineLength])
              ++commandLineLength;

              taskExecutableArgumentsBuffer = ::new (std::nothrow) WCHAR[commandLineLength + commandLineRerunArgumentLength + /* ->> NUL terminator */ 1u];

              if (NULL != taskExecutableArgumentsBuffer) {
                taskExecutableArgumentsBuffer[commandLineLength + commandLineRerunArgumentLength] = L'\0';

                for (std::size_t index = 0u; index != commandLineLength; ++index)
                taskExecutableArgumentsBuffer[index] = commandLine[index];

                for (std::size_t index = 0u; index != commandLineRerunArgumentLength; ++index)
                taskExecutableArgumentsBuffer[commandLineLength + index] = commandLineRerunArgument[index];
              }
            }

            // ... --> taskDailyTriggerTime…Buffer[…] = …;
            for (struct { WCHAR *text; unsigned int time; std::size_t begin, end; } indexes[] = {
              {taskDailyTriggerTimeEndBuffer,   time.hour,        11u, 12u},
              {taskDailyTriggerTimeEndBuffer,   time.minute,      14u, 15u},
              {taskDailyTriggerTimeEndBuffer,   time.second,      17u, 18u},
              {taskDailyTriggerTimeEndBuffer,   time.year + 100u, 0u,  3u},
              {taskDailyTriggerTimeStartBuffer, time.hour,        11u, 12u},
              {taskDailyTriggerTimeStartBuffer, time.minute,      14u, 15u},
              {taskDailyTriggerTimeStartBuffer, time.second,      17u, 18u},
              {taskDailyTriggerTimeStartBuffer, time.year,        0u,  3u},
            }, *index = indexes; index != indexes + (sizeof indexes / sizeof *indexes); ++index) {
              for (std::size_t subindex = index -> end + 1u; index -> begin != subindex--; index -> time /= 10u)
              index -> text[subindex] = "0123456789"[index -> time % 10u];
            }

            // ...
            VariantInit(&taskFolderPassword);
            VariantInit(&taskFolderSecurityDescriptor);
            VariantInit(&taskFolderUsername);
            VariantInit(&taskServiceDomain);
            VariantInit(&taskServicePassword);
            VariantInit(&taskServiceServerName);
            VariantInit(&taskServiceUser);

            credentialsPassword[credentialsPasswordSize - 1u] = L'\0';
            credentialsUsername[credentialsUsernameSize - 1u] = L'\0';
            credentialsUIInformation.cbSize                   = sizeof(CREDUI_INFOW);
            credentialsUIInformation.hbmBanner                = static_cast<HBITMAP>(NULL);
            credentialsUIInformation.hwndParent               = static_cast<HWND>   (NULL);
            credentialsUIInformation.pszCaptionText           = credentialsPromptCaption;
            credentialsUIInformation.pszMessageText           = credentialsPromptMessage;
            taskDailyTriggerTimeEnd                           = SysAllocString(taskDailyTriggerTimeEndBuffer);
            taskDailyTriggerTimeStart                         = SysAllocString(taskDailyTriggerTimeStartBuffer);
            taskFolderPassword          .vt                   = ::VT_BSTR;
            taskFolderSecurityDescriptor.bstrVal              = NULL;
            taskFolderSecurityDescriptor.vt                   = ::VT_BSTR;
            taskFolderUsername          .vt                   = ::VT_BSTR;
            taskExecutableArguments                           = NULL != taskExecutableArgumentsBuffer ? SysAllocString(taskExecutableArgumentsBuffer) : NULL;
            taskExecutablePath                                = SysAllocString(applicationFilePath);

            ::delete[] taskExecutableArgumentsBuffer;

            if (
              NULL  != taskAuthor                    and
              NULL  != taskDailyTriggerName          and
              NULL  != taskDailyTriggerTimeEnd       and
              NULL  != taskDailyTriggerTimeStart     and
              NULL  != taskExecutableArguments       and
              NULL  != taskExecutablePath            and
              L'\0' != applicationFilePath[0]   and
              NULL  != taskFolderPath                and
              NULL  != taskName                      and
              NULL  != taskRepititionPatternDuration and
              NULL  != taskRepititionPatternInterval
            ) do {
              if (FAILED(CoInitializeSecurity(static_cast<PSECURITY_DESCRIPTOR>(NULL), -1L, static_cast<SOLE_AUTHENTICATION_SERVICE*>(NULL), static_cast<void*>(NULL), RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IMPERSONATE, static_cast<void*>(NULL), 0x00u, static_cast<void*>(NULL)))) break;
              if (FAILED(CoCreateInstance    (::CLSID_TaskScheduler, static_cast<LPUNKNOWN>(NULL), ::CLSCTX_INPROC_SERVER, ::IID_ITaskService, &taskService)))                                                                                                                                  break;
              if (NULL == taskService)                                                                                                                                                                                                                                                          break;

              if (FAILED(static_cast<ITaskService*>(taskService) -> lpVtbl -> Connect  (static_cast<ITaskService*>(taskService), taskServiceServerName, taskServiceUser, taskServiceDomain, taskServicePassword))) break;
              if (FAILED(static_cast<ITaskService*>(taskService) -> lpVtbl -> GetFolder(static_cast<ITaskService*>(taskService), taskFolderPath, &taskFolder)))                                                    break;
              if (NULL == taskFolder)                                                                                                                                                                              break;

              if (FAILED(static_cast<ITaskService*>(taskService) -> lpVtbl -> NewTask(static_cast<ITaskService*>(taskService), 0x00u, &taskDefinition))) break;
              if (NULL == taskDefinition)                                                                                                                break;

              if (FAILED(taskDefinition -> lpVtbl -> get_RegistrationInfo(taskDefinition, &taskRegistrationInformation))) break;
              if (NULL == taskRegistrationInformation)                                                                    break;

              if (FAILED(taskRegistrationInformation -> lpVtbl -> put_Description(taskRegistrationInformation, taskDescription))) { /* Do nothing… */ }
              if (FAILED(taskRegistrationInformation -> lpVtbl -> put_Author     (taskRegistrationInformation, taskAuthor)))            break;
              if (FAILED(taskDefinition              -> lpVtbl -> get_Triggers   (taskDefinition,             &taskTriggerCollection))) break;
              if (NULL == taskTriggerCollection)                                                                                        break;

              if (FAILED(taskTriggerCollection -> lpVtbl -> Create(taskTriggerCollection, ::TASK_TRIGGER_DAILY, &taskTrigger))) break;
              if (NULL == taskTrigger)                                                                                          break;

              if (FAILED(taskTrigger -> lpVtbl -> QueryInterface(taskTrigger, ::IID_IDailyTrigger, &taskDailyTrigger))) break;
              if (NULL == taskDailyTrigger)                                                                             break;

              if (FAILED(static_cast<IDailyTrigger*>(taskDailyTrigger) -> lpVtbl -> put_DaysInterval (static_cast<IDailyTrigger*>(taskDailyTrigger),  taskDailyTriggerInterval)))  break;
              if (FAILED(static_cast<IDailyTrigger*>(taskDailyTrigger) -> lpVtbl -> put_EndBoundary  (static_cast<IDailyTrigger*>(taskDailyTrigger),  taskDailyTriggerTimeEnd)))   break;
              if (FAILED(static_cast<IDailyTrigger*>(taskDailyTrigger) -> lpVtbl -> put_Id           (static_cast<IDailyTrigger*>(taskDailyTrigger),  taskDailyTriggerName)))      break;
              if (FAILED(static_cast<IDailyTrigger*>(taskDailyTrigger) -> lpVtbl -> put_StartBoundary(static_cast<IDailyTrigger*>(taskDailyTrigger),  taskDailyTriggerTimeStart))) break;
              if (FAILED(static_cast<IDailyTrigger*>(taskDailyTrigger) -> lpVtbl -> get_Repetition   (static_cast<IDailyTrigger*>(taskDailyTrigger), &taskRepititionPattern)))     break;
              if (NULL == taskRepititionPattern)                                                                                                                                   break;

              if (FAILED(taskRepititionPattern -> lpVtbl -> put_Duration(taskRepititionPattern, taskRepititionPatternDuration))) break;
              if (FAILED(taskRepititionPattern -> lpVtbl -> put_Interval(taskRepititionPattern, taskRepititionPatternInterval))) break;
              if (FAILED(taskDefinition        -> lpVtbl -> get_Actions (taskDefinition,       &taskActionCollection)))          break;
              if (NULL == taskActionCollection)                                                                                  break;

              if (FAILED(taskActionCollection -> lpVtbl -> Create(taskActionCollection, ::TASK_ACTION_EXEC, &taskAction))) break;
              if (NULL == taskAction)                                                                                      break;

              if (FAILED(taskAction -> lpVtbl -> QueryInterface(taskAction, ::IID_IExecAction, &taskExecutableAction))) break;
              if (NULL == taskExecutableAction)                                                                         break;

              if (FAILED(static_cast<IExecAction*>(taskExecutableAction) -> lpVtbl -> put_Arguments(static_cast<IExecAction*>(taskExecutableAction), taskExecutableArguments))) break;
              if (FAILED(static_cast<IExecAction*>(taskExecutableAction) -> lpVtbl -> put_Path     (static_cast<IExecAction*>(taskExecutableAction), taskExecutablePath)))      break;

              // ... --> credentialsPassword = …; credentialsUsername = …;
              (void) std::fwide(stdin, +1);

              standardInputDescriptor = ::_fileno(stdin);
              standardInputIsTeletype = standardInputDescriptor != -1 and 0 != ::_isatty(standardInputDescriptor);

              do {
                // --> catalog -> log(credentialsPromptMessage);
                if (EOF == std::fputs(catalog -> messagePrefix, stdout))
                break;

                (void) std::fputws(credentialsPromptMessage, stdout);
                (void) std::fputs (catalog -> messageSuffix, stdout);
              } while (false);

              // ...
              if (standardInputIsTeletype) { // --> std::fgetws(…)
                credentialsAuthenticationBuffer = &credentialsAuthenticationBuffer;

                for (struct { wchar_t const detailMessage[14u + /* ->> NUL terminator */ 1u]; WCHAR *detail; std::size_t detailLength; } const credentials[] = {
                  {L"\r\n" "  " "Username: ", credentialsUsername, credentialsUsernameSize - 1u},
                  {L"\r\n" "  " "Password: ", credentialsPassword, credentialsPasswordSize - 1u}
                }, *credential = credentials; credential != credentials + (sizeof credentials / sizeof *credentials); ++credential) {
                  if (NULL == credentialsAuthenticationBuffer)
                  break;

                  // ...
                  if (EOF == std::fputws(credential -> detailMessage + (standardInputIsTeletype and standardInputNewlined ? /* ->> CRLF newline */ 2u : 0u), stdout))
                    credentialsAuthenticationBuffer = NULL;

                  else for (std::size_t length = 0u; ; ++length) {
                    std::wint_t character = WEOF;

                    // ...
                    if (credential -> detailLength <= length) {
                      (void) catalog -> warn("Unable to set up internal clock via the Task Scheduler; Insufficient storage");
                      credentialsAuthenticationBuffer = NULL;

                      break;
                    }

                    character             = std::fgetwc(stdin);
                    standardInputNewlined = L'\n' == static_cast<wchar_t>(character);

                    if (WEOF == character or standardInputNewlined) {
                      // credentialsAuthenticationBuffer = EILSEQ == errno ? NULL : credentialsAuthenticationBuffer;
                      credential -> detail[length] = L'\0';
                      break;
                    }

                    credential -> detail[length] = character;
                  }
                }
              }

              if (NULL == credentialsAuthenticationBuffer) // --> CredUIPromptForWindowsCredentialsW(…)
              if (NULL != CredUIPromptForWindowsCredentialsW and NULL != CredUnPackAuthenticationBufferW) {
                DWORD const credentialsPrompted = CredUIPromptForWindowsCredentialsW(&credentialsUIInformation, 0x00u, &credentialsAuthenticationPackage, static_cast<LPCVOID>(NULL), 0u, &credentialsAuthenticationBuffer, &credentialsAuthenticationBufferSize, static_cast<BOOL*>(NULL), CREDUIWIN_ENUMERATE_CURRENT_USER | CREDUIWIN_GENERIC);

                // ...
                if (ERROR_CANCELLED == credentialsPrompted) {
                  (void) catalog -> warn("Rejected internal clock set up");
                  break;
                }

                else if (ERROR_SUCCESS == credentialsPrompted) {
                  if (FALSE == CredUnPackAuthenticationBufferW(0x1u /* --> CRED_PACK_PROTECTED_CREDENTIALS */, credentialsAuthenticationBuffer, credentialsAuthenticationBufferSize, credentialsUsername, &credentialsUsernameSize, credentialsDomainName, &credentialsDomainNameSize, credentialsPassword, &credentialsPasswordSize))
                  credentialsAuthenticationBuffer = NULL;
                }
              }

              if (NULL == credentialsAuthenticationBuffer) // --> CredUIPromptForCredentialsW(…)
              if (NULL != CredUIPromptForCredentialsW) {
                DWORD const credentialsPrompted = CredUIPromptForCredentialsW(&credentialsUIInformation, L"", static_cast<PCtxtHandle>(NULL), 0x00u, credentialsUsername, credentialsUsernameSize, credentialsPassword, credentialsPasswordSize, static_cast<BOOL*>(NULL), CREDUI_FLAGS_ALWAYS_SHOW_UI | CREDUI_FLAGS_DO_NOT_PERSIST | CREDUI_FLAGS_GENERIC_CREDENTIALS);

                // ...
                if (ERROR_CANCELLED == credentialsPrompted) {
                  (void) catalog -> warn("Rejected internal clock set up");
                  break;
                }
              }

              (void) taskFolder -> lpVtbl -> DeleteTask(taskFolder, taskName, 0x00L);
              taskFolderPassword.bstrVal = SysAllocString(credentialsPassword);
              taskFolderUsername.bstrVal = SysAllocString(credentialsUsername);

              if (FAILED(taskFolder -> lpVtbl -> RegisterTaskDefinition(taskFolder, taskName, taskDefinition, ::TASK_CREATE_OR_UPDATE, taskFolderUsername, taskFolderPassword, ::TASK_LOGON_PASSWORD, taskFolderSecurityDescriptor, &registeredTask))) {
                (void) catalog -> warn("Unable to set up internal clock via the Task Scheduler; Failed task registration");
                break;
              }

              // catalog -> clocked = true;
            } while (false);

            // ...
            if (NULL != VariantClear) {
              (void) VariantClear(&taskFolderUsername);
              (void) VariantClear(&taskServiceDomain);
              (void) VariantClear(&taskServicePassword);
              (void) VariantClear(&taskServiceServerName);
              (void) VariantClear(&taskServiceUser);
            }

            if (NULL != taskAction)                  (void) taskAction                              -> lpVtbl -> Release(taskAction);
            if (NULL != taskActionCollection)        (void) taskActionCollection                    -> lpVtbl -> Release(taskActionCollection);
            if (NULL != taskRepititionPattern)       (void) taskRepititionPattern                   -> lpVtbl -> Release(taskRepititionPattern);
            if (NULL != taskTrigger)                 (void) taskTrigger                             -> lpVtbl -> Release(taskTrigger);
            if (NULL != taskTriggerCollection)       (void) taskTriggerCollection                   -> lpVtbl -> Release(taskTriggerCollection);
            if (NULL != taskRegistrationInformation) (void) taskRegistrationInformation             -> lpVtbl -> Release(taskRegistrationInformation);
            if (NULL != taskDefinition)              (void) taskDefinition                          -> lpVtbl -> Release(taskDefinition);
            if (NULL != taskFolder)                  (void) taskFolder                              -> lpVtbl -> Release(taskFolder);
            if (NULL != taskService)                 (void) static_cast<ITaskService*>(taskService) -> lpVtbl -> Release(static_cast<ITaskService*>(taskService));

            if (NULL != SysFreeString) {
              SysFreeString(taskAuthor);
              SysFreeString(taskDailyTriggerName);
              SysFreeString(taskDailyTriggerTimeEnd);
              SysFreeString(taskDailyTriggerTimeStart);
              SysFreeString(taskDescription);
              SysFreeString(taskExecutableArguments);
              SysFreeString(taskExecutablePath);
              SysFreeString(taskFolderPath);
              SysFreeString(taskName);
              SysFreeString(taskRepititionPatternDuration);
              SysFreeString(taskRepititionPatternInterval);
            }

            CoUninitialize();
          }
        }
      #endif

      /* ... ->> Windows Services --- TODO (Lapys) -> Get it working for basic test */
      if (not catalog -> clocked)
      if (NULL != libraries[advapi32].moduleHandle and NULL != libraries[kernel32].moduleHandle) {
        SC_HANDLE          (*const CreateServiceW)             (SC_HANDLE, LPCWSTR, LPCWSTR, DWORD, DWORD, DWORD, DWORD, LPCWSTR, LPCWSTR, LPDWORD, LPCWSTR, LPCWSTR, LPCWSTR) = reinterpret_cast<SC_HANDLE (*)(SC_HANDLE, LPCWSTR, LPCWSTR, DWORD, DWORD, DWORD, DWORD, LPCWSTR, LPCWSTR, LPDWORD, LPCWSTR, LPCWSTR, LPCWSTR)>(::GetProcAddress(libraries[advapi32].moduleHandle, "CreateServiceW"));              // --> <windows.h>
        SC_HANDLE          (*const OpenSCManagerW)             (LPCWSTR, LPCWSTR, DWORD)                                                                                       = reinterpret_cast<SC_HANDLE (*)(LPCWSTR, LPCWSTR, DWORD)>                                                                                      (::GetProcAddress(libraries[advapi32].moduleHandle, "OpenSCManagerW"));              // --> <windows.h>
        BOOL               (*const StartServiceCtrlDispatcherW)(SERVICE_TABLE_ENTRYW const*)                                                                                   = reinterpret_cast<BOOL      (*)(SERVICE_TABLE_ENTRYW const*)>                                                                                  (::GetProcAddress(libraries[advapi32].moduleHandle, "StartServiceCtrlDispatcherW")); // --> <windows.h>
        SC_HANDLE                  serviceControlManagerHandle                                                                                                                 = NULL;                                                                                                                                                                                                                              //
        SC_HANDLE                  serviceControlServiceHandle                                                                                                                 = NULL;                                                                                                                                                                                                                              //
        SERVICE_TABLE_ENTRYW       serviceEntries[]                                                                                                                            = {{NULL, NULL}, {NULL, NULL}};                                                                                                                                                                                                      //
        static struct serviceinfo *service                                                                                                                                     = NULL;                                                                                                                                                                                                                              //
        static struct serviceinfo /* final */ {
          static void CALLBACK callback(PVOID const /* parameter */, BOOLEAN const /* triggered */) {
            catalog -> update();
          }

          static DWORD WINAPI control(DWORD const code, DWORD const /* event */, LPVOID const /* eventData */, LPVOID const /* parameter */) {
            BOOL (*const SetEvent)        (HANDLE)                                  = reinterpret_cast<BOOL (*)(HANDLE)>                                 (::GetProcAddress(libraries[kernel32].moduleHandle, "SetEvent"));         // --> <windows.h> → <synchapi.h>
            BOOL (*const SetServiceStatus)(SERVICE_STATUS_HANDLE, LPSERVICE_STATUS) = reinterpret_cast<BOOL (*)(SERVICE_STATUS_HANDLE, LPSERVICE_STATUS)>(::GetProcAddress(libraries[advapi32].moduleHandle, "SetServiceStatus")); // --> <windows.h>

            // ...
            if (NULL != SetEvent and NULL != SetServiceStatus and NULL != service)
            if (SERVICE_CONTROL_STOP == code and SERVICE_RUNNING == service -> status.dwCurrentState) {
              service -> status.dwCurrentState = SERVICE_STOP_PENDING;

              (void) SetServiceStatus(service -> statusHandle, &service -> status);
              (void) SetEvent        (service -> stopEvent);
            }

            return NO_ERROR;
          }

          static void WINAPI main(DWORD const /* count */, LPWSTR /* arguments */ []) {
            BOOL                  (*const CloseHandle)                  (HANDLE)                                                           = reinterpret_cast<BOOL                  (*)(HANDLE)>                                                          (::GetProcAddress(libraries[kernel32].moduleHandle, "CloseHandle"));                   // --> <windows.h>
            HANDLE                (*const CreateEventW)                 (LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCWSTR)                       = reinterpret_cast<HANDLE                (*)(LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCWSTR)>                      (::GetProcAddress(libraries[kernel32].moduleHandle, "CreateEventW"));                  // --> <windows.h> → <synchapi.h>
            HANDLE                (*const CreateTimerQueue)             ()                                                                 = reinterpret_cast<HANDLE                (*)()>                                                                (::GetProcAddress(libraries[kernel32].moduleHandle, "CreateTimerQueue"));              // --> <threadpoollegacyapiset.h>
            BOOL                  (*const CreateTimerQueueTimer)        (PHANDLE, HANDLE, WAITORTIMERCALLBACK, PVOID, DWORD, DWORD, ULONG) = reinterpret_cast<BOOL                  (*)(PHANDLE, HANDLE, WAITORTIMERCALLBACK, PVOID, DWORD, DWORD, ULONG)>(::GetProcAddress(libraries[kernel32].moduleHandle, "CreateTimerQueueTimer"));         // --> <threadpoollegacyapiset.h>
            BOOL                  (*const DeleteTimerQueue)             (HANDLE)                                                           = reinterpret_cast<BOOL                  (*)(HANDLE)>                                                          (::GetProcAddress(libraries[kernel32].moduleHandle, "DeleteTimerQueue"));              // --> <threadpoollegacyapiset.h>
            BOOL                  (*const DeleteTimerQueueEx)           (HANDLE, HANDLE)                                                   = reinterpret_cast<BOOL                  (*)(HANDLE, HANDLE)>                                                  (::GetProcAddress(libraries[kernel32].moduleHandle, "DeleteTimerQueueEx"));            // --> <threadpoollegacyapiset.h>
            DWORD                 (*const GetLastError)                 ()                                                                 = reinterpret_cast<DWORD                 (*)()>                                                                (::GetProcAddress(libraries[kernel32].moduleHandle, "GetLastError"));                  // --> <windows.h>
            SERVICE_STATUS_HANDLE (*const RegisterServiceCtrlHandlerExW)(LPCWSTR, LPHANDLER_FUNCTION_EX, LPVOID)                           = reinterpret_cast<SERVICE_STATUS_HANDLE (*)(LPCWSTR, LPHANDLER_FUNCTION_EX, LPVOID)>                          (::GetProcAddress(libraries[advapi32].moduleHandle, "RegisterServiceCtrlHandlerExW")); // --> <windows.h>
            BOOL                  (*const SetServiceStatus)             (SERVICE_STATUS_HANDLE, LPSERVICE_STATUS)                          = reinterpret_cast<BOOL                  (*)(SERVICE_STATUS_HANDLE, LPSERVICE_STATUS)>                         (::GetProcAddress(libraries[advapi32].moduleHandle, "SetServiceStatus"));              // --> <windows.h>
            DWORD                 (*const WaitForSingleObject)          (HANDLE, DWORD)                                                    = reinterpret_cast<DWORD                 (*)(HANDLE, DWORD)>                                                   (::GetProcAddress(libraries[kernel32].moduleHandle, "WaitForSingleObject"));           // --> <windows.h> → <synchapi.h>
            HANDLE                        clockHandle                                                                                      = NULL;                                                                                                                                                                                               //
            HANDLE                        clockQueueHandle                                                                                 = NULL;                                                                                                                                                                                               //

            // ...
            return;
            if (NULL == CreateEventW or NULL == CreateTimerQueue or NULL == CreateTimerQueueTimer or NULL == RegisterServiceCtrlHandlerExW or NULL == SetServiceStatus or NULL == WaitForSingleObject) {
              catalog -> clocked = false;
              return;
            }

            catalog -> clocked                          = false;
            service -> status.dwCheckPoint              = 0u;
            service -> status.dwControlsAccepted        = SERVICE_ACCEPT_STOP;
            service -> status.dwCurrentState            = SERVICE_RUNNING;
            service -> status.dwServiceSpecificExitCode = 0x00u; // ->> Ignored unless `ERROR_SERVICE_SPECIFIC_ERROR == ::dwWin32ExitCode`
            service -> status.dwServiceType             = SERVICE_WIN32_OWN_PROCESS;
            service -> status.dwWin32ExitCode           = NO_ERROR;
            service -> statusHandle                     = RegisterServiceCtrlHandlerExW(applicationName, &service -> control, static_cast<LPVOID>(service));
            service -> stopEvent                        = CreateEventW(static_cast<LPSECURITY_ATTRIBUTES>(NULL), TRUE, FALSE, static_cast<LPCWSTR>(NULL)); // ->> `CloseHandle(…)` (if non-`NULL`) on program termination
            clockQueueHandle                            = CreateTimerQueue();

            do {
              if (NULL == service -> statusHandle or NULL == clockQueueHandle)
              break;

              if (NULL == service -> stopEvent or FALSE == CreateTimerQueueTimer(&clockHandle, clockQueueHandle, &service -> callback, static_cast<PVOID>(service), 0u, catalog -> clockInterval, 0x00u)) {
                service -> status.dwCurrentState  = SERVICE_STOPPED;
                service -> status.dwWin32ExitCode = NULL == GetLastError ? ERROR_INVALID_HANDLE : GetLastError();
                (void) SetServiceStatus(service -> statusHandle, &service -> status);

                break;
              }

              if (FALSE       == SetServiceStatus   (service -> statusHandle, &service -> status)) break;
              if (WAIT_FAILED == WaitForSingleObject(service -> stopEvent,    INFINITE))           break;

              // ...
              catalog -> clocked = true;
            } while (false);

            if (INVALID_HANDLE_VALUE != service -> stopEvent and NULL != service -> stopEvent and NULL != CloseHandle) (void) CloseHandle(service -> stopEvent);
            if (NULL != clockQueueHandle and NULL != DeleteTimerQueueEx) { (void) DeleteTimerQueueEx(clockQueueHandle, static_cast<HANDLE>(NULL)); clockQueueHandle = NULL; }
            if (NULL != clockQueueHandle and NULL != DeleteTimerQueue)   { (void) DeleteTimerQueue  (clockQueueHandle);                            clockQueueHandle = NULL; }

            service -> status.dwCurrentState = SERVICE_STOPPED;
            (void) SetServiceStatus(service -> statusHandle, &service -> status);
          }

          /* ... */
          SERVICE_STATUS        status;
          SERVICE_STATUS_HANDLE statusHandle;
          HANDLE                stopEvent;
        } s = {};

        // ...
        service                         = &s;
        service        -> statusHandle  = NULL;
        service        -> stopEvent     = INVALID_HANDLE_VALUE;
        serviceEntries -> lpServiceName = applicationName;
        serviceEntries -> lpServiceProc = &service -> main; // --> LPSERVICE_MAIN_FUNCTIONW

        if (
          NULL != ::GetProcAddress(libraries[kernel32].moduleHandle, "CreateEventW")                  and // --> <windows.h> → <synchapi.h>
          NULL != ::GetProcAddress(libraries[kernel32].moduleHandle, "CreateTimerQueue")              and // --> <threadpoollegacyapiset.h>
          NULL != ::GetProcAddress(libraries[kernel32].moduleHandle, "CreateTimerQueueTimer")         and // --> <threadpoollegacyapiset.h>
          NULL != ::GetProcAddress(libraries[advapi32].moduleHandle, "RegisterServiceCtrlHandlerExW") and // --> <windows.h>
          NULL != ::GetProcAddress(libraries[kernel32].moduleHandle, "SetEvent")                      and // --> <windows.h> → <synchapi.h>
          NULL != ::GetProcAddress(libraries[advapi32].moduleHandle, "SetServiceStatus")              and // --> <windows.h>
          NULL != ::GetProcAddress(libraries[kernel32].moduleHandle, "WaitForSingleObject")           and // --> <windows.h> → <synchapi.h>
          NULL != CreateServiceW and NULL != OpenSCManagerW and NULL != StartServiceCtrlDispatcherW
        ) {
          serviceControlManagerHandle = OpenSCManagerW(static_cast<LPCWSTR>(NULL), static_cast<LPCWSTR>(NULL), SC_MANAGER_ALL_ACCESS);

          if (NULL != serviceControlManagerHandle) {
            serviceControlServiceHandle = CreateServiceW(serviceControlManagerHandle, /* --> WCHAR[256] */ L"cataloger", /* --> WCHAR[256] */ applicationName, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
              L"c:\\abc.exe",            // path to service's binary
              NULL,                      // no load ordering group
              NULL,                      // no tag identifier
              NULL,                      // no dependencies
              L"NT AUTHORITY\\NetworkService",
              L"");                     // no password
            catalog -> clocked          = FALSE != StartServiceCtrlDispatcherW(serviceEntries);
          }
        }
      }

      // TODO (Lapys) -> `WaitForSingleObject(…)` → `SetTimer(…)` → `Sleep(…)`
      if (not catalog -> clocked) {}
    #endif

    // MANUAL METHOD
  #endif
  }

  // GET CURRENT DATE/ TIME
  // CHECK IF TIME ELAPSED OR IF RUN FIRST TIME
  // SPAWN LOG not as child process; format: 2023–07–07.log
  // HELP SCREEN?
  // ABOUT SCREEN

  std::printf("[...]: 0x%02lX {allocated: %4.5s, name: \"%s\"}" "\r\n", (unsigned long) reinterpret_cast<uintptr_t>(catalog -> clockFileStream), catalog -> clockPathAllocated ? "true" : "false", catalog -> clockPath);

  // ...
  return catalog -> exit(EXIT_SUCCESS);
}

// int __cdecl _tmain(int argc, TCHAR* argv[])
// {
//     const SERVICE_TABLE_ENTRY dispatchTable[] = {
//         {serviceName, static_cast<LPSERVICE_MAIN_FUNCTION>(ServiceMainCallback)},
//         {NULL, NULL}
//     };

//     if (!StartServiceCtrlDispatcher(dispatchTable)) {
//         // if registration fails with ERROR_FAILED_SERVICE_CONTROLLER_CONNECT
//         // this usually means we're running as console application
//         if (GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT) {
//             return CommonEntry(argc, argv, true);
//         }
//         return -1;
//     }

//     return 0;
// }
// this is what my entry point looks like
// for reference
// // callback registered with and called by the Service Control Manager
// VOID WINAPI ServiceMainCallback(DWORD argc, LPTSTR* argv)
// {
//     CommonEntry(argc, argv);
// }
