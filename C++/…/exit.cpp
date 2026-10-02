/* POSIX:   rm -f ./close;   clear && clang++ -pedantic-errors -std=c++98 -Wall -Wextra exit.cpp -o close     && ./close;    rm -f ./close */
/* Windows: del close.exe && cls   && clang++ -pedantic-errors -std=c++98 -Wall -Wextra exit.cpp -o close.exe && close.exe & del close.exe */
#include <ciso646> // --> and, not, or
#include <csignal> // --> ::std::sig_atomic_t; SIG_ERR, SIGINT, SIGTERM; ::std::signal(…)
#include <cstddef> // --> NULL
#include <cstdlib> // --> EXIT_FAILURE, EXIT_SUCCESS
#include <cstdio>  // --> stdout; ::std::fwrite(…)
#if defined _WIN32
# include <windows.h> // --> ::BOOL, ::DWORD, ::HANDLE, ::HWND, ::MSG; CREATE_EVENT_MANUAL_RESET, CTRL_BREAK_EVENT, CTRL_CLOSE_EVENT, CTRL_C_EVENT, CTRL_LOGOFF_EVENT, CTRL_SHUTDOWN_EVENT, FALSE, INFINITE, MWMO_INPUTAVAILABLE, PM_REMOVE, QS_ALLINPUT, TRUE, WAIT_FAILED, WAIT_OBJECT_0, WM_CLOSE, WM_DESTROY, WM_ENDSESSION, WM_KEYDOWN, WM_QUERYENDSESSION, WM_QUIT; ::CloseHandle(…), ::CreateEventExW(…), ::DispatchMessageW(…), ::GetMessageW(…), ::PostQuitMessage(…), ::SetConsoleCtrlHandler(…), ::SetEvent(…), ::TranslateMessage(…), ::WaitForSingleObject(…)
#elif defined __APPLE__ or defined __unix__
# include <signal.h> // --> ::sigaction; SIGHUP, SIGQUIT; ::sigaction(…), ::sigemptyset(…)
#endif

/* Application */
static struct program *application = NULL;
#if not defined _WIN32
  extern "C"
#endif
struct program /* final */ {
  #if defined _WIN32
    struct /* final */ { int status; ::HANDLE completed, requested; } termination;
    struct /* final */ { ::MSG message; }                             thread;
    struct /* final */ {
      static ::LRESULT CALLBACK procedure(::HWND const windowHandle, ::UINT const message, ::WPARAM const parameter, ::LPARAM const subparameter) {
        switch (message) {
          case WM_CLOSE:   (void) ::DestroyWindow(windowHandle);                                        return 00L;   break; // --> WM_DESTROY
          case WM_DESTROY:        ::PostQuitMessage(application -> termination.status);                 return 00L;   break; // --> WM_QUIT
          case WM_ENDSESSION: if (FALSE     != parameter) { (void) application -> finish(); }           return 00L;   break; // ->> Session ending (e.g. `winlogon.exe`) due to `ENDSESSION_CLOSEAPP | ENDSESSION_CRITICAL | ENDSESSION_LOGOFF == subparameter`
          case WM_KEYDOWN:    if (VK_ESCAPE == parameter) { if (FALSE != ::DestroyWindow(windowHandle)) return 00L; } break; // --> WM_DESTROY
          case WM_QUERYENDSESSION:                                                                      return TRUE;  break; //
          default:;                                                                                                          //
        }

        return ::DefWindowProcW(windowHandle, message, parameter, subparameter);
      }
    } window;
  #elif defined __APPLE__ or defined __unix__
    struct /* final */ { int status; union { ::sig_atomic_t volatile requested, completed; }; } termination;
  #else
    struct /* final */ { int status; union { ::std::sig_atomic_t volatile requested, completed; }; } termination;
  #endif

  /* ... */
  #if defined _WIN32 // --> extern "C"
    static ::BOOL WINAPI onexit(::DWORD const reason) {
      // ->> Invoked from the console control handler thread
      if (NULL != application)
      switch (reason) {
        case CTRL_BREAK_EVENT:    // ->> Control-Break (e.g. `Ctrl`+`Break` or `::GenerateConsoleCtrlEvent(reason, ::DWORD)`)
        case CTRL_CLOSE_EVENT:    // ->> Control Close
        case CTRL_C_EVENT:        // ->> Control-C      (e.g. `Ctrl`+`C` or `::GenerateConsoleCtrlEvent(reason, ::DWORD)`)
        case CTRL_LOGOFF_EVENT:   // ->> Control Logoff (i.e. `winlogon.exe`)
        case CTRL_SHUTDOWN_EVENT: // ->> Control Shutdown
          return FALSE != ::SetEvent(application -> termination.requested) and WAIT_FAILED != ::WaitForSingleObject(application -> termination.completed, /* --> 0xFFFFFFFFu */ INFINITE) ? TRUE : FALSE; // ->> Signal then wait until external timeouts and other threads shutdown
      }

      return FALSE;
    }
  #elif defined __APPLE__ or defined __unix__
    static void onexit(int const reason) {
      if (NULL != application)
      switch (reason) {
        case SIGHUP: case SIGINT: case SIGQUIT: case SIGTERM: application -> termination.requested = static_cast< ::sig_atomic_t>(true); break;
        default:; // --> SIGABRT, SIGBUS, SIGFPE, SIGILL, SIGPIPE, SIGSEGV, …, SIGKILL, SIGSTOP, …
      }
    }
  #else
    static void onexit(int const reason) {
      if (NULL != application)
      switch (reason) {
        case SIGINT: case SIGTERM: application -> termination.requested = static_cast< ::std::sig_atomic_t>(true); break;
        default:; // --> SIGABRT, SIGFPE, SIGILL, SIGSEGV, …
      }
    }
  #endif

  inline int finish()                 const /* noexcept */ { return this -> finish(this -> termination.status); }
  int        finish(int const status) const /* noexcept */ {
    // ->> Close files and sockets and such, flush persistent data, relinquish owned resources, stop workers, …
    #if defined _WIN32
      if (FALSE == ::SetEvent(this -> termination.completed))
      return EXIT_FAILURE;
    #endif

    return status;
  }

  void load(...) /* noexcept */ {
    application = this;

    #if defined _WIN32
      this -> termination.completed = ::CreateEventExW(static_cast< ::LPSECURITY_ATTRIBUTES>(NULL), static_cast< ::LPCWSTR>(NULL), CREATE_EVENT_MANUAL_RESET, /* --> ::SetEvent(…) */ EVENT_MODIFY_STATE | /* --> ::WaitForSingleObject(…) */ SYNCHRONIZE); // --> ::CreateEventW(::LPSECURITY_ATTRIBUTES, ::BOOL manual, ::BOOL initial = FALSE, ::LPCWSTR);
      this -> termination.requested = ::CreateEventExW(static_cast< ::LPSECURITY_ATTRIBUTES>(NULL), static_cast< ::LPCWSTR>(NULL), CREATE_EVENT_MANUAL_RESET, /* --> ::SetEvent(…) */ EVENT_MODIFY_STATE | /* --> ::WaitForSingleObject(…) */ SYNCHRONIZE); // --> ::std::atomic_bool {… ? ::SetEvent(::HANDLE) : ::ResetEvent(::HANDLE)}

      if (
        static_cast< ::HANDLE>(NULL) == this -> termination.completed or
        static_cast< ::HANDLE>(NULL) == this -> termination.requested or
        FALSE == ::SetConsoleCtrlHandler(&this -> onexit, TRUE)
      ) {
        application = NULL;

        if (static_cast< ::HANDLE>(NULL) != this -> termination.completed) { (void) ::CloseHandle(this -> termination.completed); this -> termination.completed = static_cast< ::HANDLE>(NULL); }
        if (static_cast< ::HANDLE>(NULL) != this -> termination.requested) { (void) ::CloseHandle(this -> termination.requested); this -> termination.requested = static_cast< ::HANDLE>(NULL); }
      }
    #elif defined __APPLE__ or defined __unix__
      struct ::sigaction action = {};

      // ... ->> Otherwise use `::signal(…)`
      action.sa_flags               = 0x00;
      action.sa_handler             = &this -> onexit; // ->> `SIG_DFL` is default and `SIG_IGN` does nothing
      this -> termination.requested = static_cast< ::sig_atomic_t>(false);

      if (0 != ::sigemptyset(&action.sa_mask)) // --> ::sigset_t*
        application = NULL;

      else for (struct signal /* final */ { int const id; struct ::sigaction previousAction; } signals[] = {
        {SIGHUP,  {}}, // ->> Hang Up; typically used for disconnects or re-configurations
        {SIGINT,  {}}, // ->> Interrupt                          (e.g. `Ctrl`+`C`)
        {SIGQUIT, {}}, // ->> Quit; typically core dumps instead (e.g. `Ctrl`+`\`)
        {SIGTERM, {}}  // ->> Terminate                          (e.g. `kill [-s SIGTERM|-TERM] <pid>` or `::kill(<pid>, SIGTERM)`)
      }, *signal = signals; signal != &signals[sizeof signals / sizeof(struct signal)]; ++signal)
      if (0 != ::sigaction(signal -> id, &action, &signal -> previousAction)) {
        while (signal != signals)
          (void) --signal, ::sigaction(signal -> id, &signal -> previousAction, static_cast<struct ::sigaction*>(NULL));

        application = NULL;
        signal      = &signals[(sizeof signals / sizeof(struct signal)) - 1u]; // --> break
      }
    #else
      for (struct signal /* final */ { int const id; void (*previousHandler)(int); } signals[] = {
        {SIGINT,  NULL}, // ->> Interrupt
        {SIGTERM, NULL}  // ->> Terminated
      }, *signal = signals; signal != &signals[sizeof signals / sizeof(struct signal)]; ++signal)
      if (SIG_ERR == (signal -> previousHandler = ::std::signal(signal -> id, &this -> onexit))) {
        while (signal != signals)
          (void) --signal, ::std::signal(signal -> id, signal -> previousHandler);

        application = NULL;
        signal      = &signals[(sizeof signals / sizeof(struct signal)) - 1u]; // --> break
      }
    #endif
  }
} program = {
  #if defined _WIN32
    {EXIT_SUCCESS, NULL, NULL}, {}, {}
  #elif defined __APPLE__ or defined __unix__
    {EXIT_SUCCESS, {00}}
  #else
    {EXIT_SUCCESS, {00}}
  #endif
};

/* Main */
int main(int, char*[]) {
  program.load(NULL); // --> application = …;

  #if defined _WIN32 // ->> See `::RegisterServiceCtrlHandlerExW(…)` for service processes
    for (::DWORD const timeout = /* ->> `INFINITE` to wait until messaged */ 0u; NULL != application; )
    switch (::MsgWaitForMultipleObjectsEx(1u, &application -> termination.requested, timeout, QS_ALLINPUT, MWMO_INPUTAVAILABLE)) /* ->> `::WaitForSingleObject(application -> termination.requested, ::DWORD …)` sans (or explicitly before) GUI/ message-queue update loop */ {
      case WAIT_TIMEOUT: {
        (void) ::std::fwrite("Hello, World!" "\r\n", sizeof(char), 15u, stdout);
      } goto finish; // ->> — or `break` to update loop instead

      case WAIT_OBJECT_0 + 1: /* ->> GUI/ message-queue update loop */ {
        for (::BOOL available; FALSE != (available = ::PeekMessageW(&application -> thread.message, static_cast< ::HWND>(NULL), 0x0u, 0x0u, PM_REMOVE)); ) {
          if (WM_QUIT == application -> thread.message.message or /* --> FALSE < ::GetMessageW(…) */ available == -1) {
            application -> termination.status = application -> thread.message.wParam;
            goto finish;
          }

          (void) ::TranslateMessage(&application -> thread.message);
          (void) ::DispatchMessageW(&application -> thread.message);
        }
      } break;

      case WAIT_OBJECT_0 + 0:                                             goto finish; // --> application -> termination.requested ->> User-requested exit
      case WAIT_FAILED: application -> termination.status = EXIT_FAILURE; goto finish; // --> ::DWORD {0xFFFFFFFFu}                ->> `::GetLastError()` for diagnostic
      default:;         /* Do something… */                                            // --> WAIT_ABANDONED_0, WAIT_IO_COMPLETION, …
    }
  #else // --> defined __APPLE__ or defined __unix__
    while (NULL != application and not application -> termination.requested) {
      (void) ::std::fwrite("Hello, World!" "\r\n", sizeof(char), 15u, stdout);
      goto finish; // ->> — otherwise update loop instead
    }
  #endif
  finish:

  return NULL != application ? application -> finish() : EXIT_FAILURE; // --> ::std::exit(…)
}
