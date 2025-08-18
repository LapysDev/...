#include <csignal>
#include <cstdio>
#include <cstdlib>

static void terminate_handler(void) {
  std::puts("[TERMINATED]");
}

static void signal_handler(int const signal) {
  switch (signal) {
    case SIGABRT: std::puts("[ABORTED]");              break;
    case SIGFPE : std::puts("[FLOATING-POINT ERROR]"); break;
    case SIGILL : std::puts("[ILLEGAL INSTRUCTION]");  break;
    case SIGINT : std::puts("[INTERRUPT]");            break;
    case SIGSEGV: std::puts("[SEGMENTATION ERROR]");   break;
    case SIGTERM: std::puts("[TERMINATED]");           break;

    #if false
      case SIGALRM:   /* ->> See `alarm()`; Real-time timer elapses/ expires */ break;
      case SIGBUS:    /* ->> Bus error: invalid memory access (e.g., misaligned access). Usually crashes with a core dump */ break;
      case SIGCHLD:   /* ->> Child process continued, stopped, or terminated */ break;
      case SIGCONT:   /* ->> Continue execution of a stopped process */ break;
      case SIGEMT:    /* ->> Emulation trap (implementation-specific hardware fault). Not commonly used */ break;
      case SIGHUP:    /* ->> Hangup: sent when controlling terminal closes. Often used to tell daemons to reload config */ break;
      case SIGIO:     /* ->> Asynchronous I/O possible on a descriptor */ break;
      case SIGIOT:    /* ->> Obsolete alias for `SIGABRT` */ break;
      case SIGKILL:   /* ->> Uncaught immediate termination of the process */ break;
      case SIGPIPE:   /* ->> Write on a pipe with no readers. Typically terminates the writing process */ break;
      case SIGPOLL:   /* ->> Polled event (System V); similar to `SIGIO` */ break;
      case SIGPROF:   /* ->> Profiling timer expired (often obsolete) */ break;
      case SIGPWR:    /* ->> Power-failure restart or power condition */ break;
      case SIGQUIT:   /* ->> Quit signal; Terminates process and dumps core */ break;
      case SIGRTMAX:  /* ->> Real-time signals reserved for application use */ break;
      case SIGRTMIN:  /* ->> Real-time signals reserved for application use */ break;
      case SIGSTKFLT: /* ->> Stack fault on coprocessor (rare/not used on many systems) */ break;
      case SIGSTOP:   /* ->> Uncaught signal to stop (pause) the process */ break;
      case SIGSYS:    /* ->> Bad system call. Usually causes termination with a core dump */ break;
      case SIGTRAP:   /* ->> Breakpoint/ trace trap (used by debuggers) */ break;
      case SIGTSTP:   /* ->> Terminal stop signal (e.g., `Ctrl-Z`). Can be caught or ignored */ break;
      case SIGTTIN:   /* ->> Background process attempted to read from terminal. Stops the process */ break;
      case SIGTTOU:   /* ->> Background process attempted to write to terminal. Stops the process */ break;
      case SIGURG:    /* ->> Urgent condition on socket (e.g., out-of-band data) */ break;
      case SIGUSR1:   /* ->> User-defined signals for custom purposes */ break;
      case SIGUSR2:   /* ->> User-defined signals for custom purposes */ break;
      case SIGVTALRM: /* ->> Virtual timer elapsed; expired (counts process virtual time) */ break;
      case SIGWINCH:  /* ->> Window size change (e.g., terminal resized) */ break;
      case SIGXCPU:   /* ->> CPU time limit exceeded */ break;
      case SIGXFSZ:   /* ->> File size limit exceeded */ break;

      case SIGCLD:    /* ->> Alias for `SIGCHLD` (System V legacy name) */ break;
      case SIGINFO:   /* ->> Status request (often Ctrl-T) on BSD-like systems; not supported on Linux */ break;
      case SIGLOST:   /* ->> Notification (on some systems like Solaris) that a resource lock has been lost */ break;
      case SIGUNUSED: /* ->> Reserved/ unused in modern systems */ break;
    #endif
  }

  std::exit(EXIT_SUCCESS);
}

/* Main */
int main(void) {
  int *const pointer = NULL;

  std::atexit(&terminate_handler);
  std::signal(SIGABRT, &signal_handler);
  std::signal(SIGFPE, &signal_handler);
  std::signal(SIGILL, &signal_handler);
  std::signal(SIGINT, &signal_handler);
  std::signal(SIGSEGV, &signal_handler);
  std::signal(SIGTERM, &signal_handler);

  std::printf("[...]: %i" "\r\n", *pointer);
  std::puts("[...]");

  return EXIT_SUCCESS;
}
