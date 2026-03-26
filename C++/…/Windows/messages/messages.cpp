#undef _CRT_NON_CONFORMING_SWPRINTFS     // ->> `std::swprintf(…)` is weird in Microsoft Windows environments/ runtimes
#undef _CRT_STDIO_LEGACY_WIDE_SPECIFIERS //     ^^^
#if defined _MSC_BUILD || defined _MSC_FULL_VER || defined _MSC_VER
# pragma comment(lib, "comctl32" ".lib")
# pragma comment(lib, "kernel32" ".lib")
# pragma comment(lib, "richedit" ".lib")
# pragma comment(lib, "user32"   ".lib")
#endif
#define _CRT_SECURE_NO_WARNINGS // ->> C Run-Time security                    --> `https://learn.microsoft.com/en-us/cpp/c-runtime-library/security-features-in-the-crt`
#define CINTERFACE              // ->> C Interface for Component Object Model --> `https://learn.microsoft.com/en-us/windows/win32/com/the-component-object-model`
#define STRICT                  // ->> Strict type checking                   --> `https://learn.microsoft.com/en-us/windows/win32/winprog/enabling-strict`
#ifndef UNICODE
# define UNICODE // ->> Unicode preference --> `https://learn.microsoft.com/en-us/windows/win32/learnwin32/working-with-strings`
#endif
#ifndef _WIN32_WINNT
# define _WIN32_WINNT 0x0600 // --> _WIN32_WINNT_WINXP+
#endif
#include <fcntl.h>   // --> _O_WTEXT
#include <io.h>      // --> _setmode(…)
#include <stdio.h>   // --> _fileno(…)
#include <windows.h> // --> CS_HREDRAW, CS_VREDRAW, CW_USEDEFAULT, FALSE, GW_CHILD, GW_ENABLEDPOPUP, GW_HWNDNEXT, HWND_TOPMOST, IDC_ARROW, GWL_STYLE, LOWORD(…), MAKELONG(…), MAKELPARAM(…), MAX_PATH, SW_SHOW, WS_CHILD, WS_OVERLAPPEDWINDOW, WS_VISIBLE, ZeroMemory(…), …; HANDLE, HBRUSH, HICON, HINSTANCE, HMODULE, HWND, INT_PTR, LPARAM, LPCWSTR, LPNMHDR, LPNMTTDISPINFOW, LPPOINT, LPWSTR, MSG, POINT, PWSTR, RECT, UINT, UINT_PTR, WCHAR, WINAPI, WNDCLASSEXW, WORD, WPARAM; ::CreateWindowExW(…), ::DefWindowProcW(…), ::DestroyWindow(…), ::DispatchMessageW(…), ::ExtractIconW(…), ::FreeLibrary(…), ::GetClientRect(…), ::GetCurrentProcess(…), ::GetMessageW(…), ::GetModuleFileNameW(…), ::GetSysColorBrush(…), ::GetWindow(…), ::GetWindowLongPtrW(…), ::LoadCursor(…), ::LoadLibraryExW(…), ::MapWindowPoints(…), ::PostQuitMessage(…), ::RegisterClassExW(…), ::SendMessageW(…), ::ScreenToClient(…), ::ShowWindow(…), ::TranslateMessage(…), ::UpdateWindow(…), ::UnregisterClassW(…)
# include <commctrl.h> // --> BTNS_BUTTON, CCM_GETVERSION, CCS_NOPARENTALIGN, CCS_NORESIZE, HINST_COMMCTRL, I_IMAGENONE, ICC_BAR_CLASSES, ICC_DATE_CLASSES, ICC_USEREX_CLASSES, ICC_WIN95_CLASSES, IDB_STD_SMALL_COLOR, SB_SETPARTS, SB_SETTEXT, SBT_NOBORDERS, SBT_NOTABPARSING, STATUSCLASSNAME, STD_HELP, TB_BUTTONSTRUCTSIZE, TB_ADDBITMAP, TBSTATE_ENABLED, TBSTYLE_WRAPABLE, TOOLBARCLASSNAME, TOOLINFOW, TTM_ADDTOOLW; INITCOMMONCONTROLSEX, TBADDBITMAP, TBBUTTON, TTF_ABSOLUTE, TTF_CENTERTIP, TTF_DI_SETITEM, TTF_IDISHWND, TTF_PARSELINKS, TTF_RTLREADING, TTF_SUBCLASS, TTF_TRACK, TTF_TRANSPARENT, TTN_GETDISPINFOW, TTN_LINKCLICK, TTN_NEEDTEXTW, TTN_POP, TTN_SHOW; ::InitCommonControlsEx(…)
# include <shlwapi.h>  // --> DLLGETVERSIONPROC, DLLVERSIONINFO
# include <winerror.h> // --> SUCCEEDED(…)
#
#include <clocale>  // --> LC_ALL; std::setlocale(…)
#include <cstddef>  // --> NULL; std::size_t
#include <cstdlib>  // --> EXIT_FAILURE, EXIT_SUCCESS; std::div_t
#include <cstdio>   // --> stdout; std::sprintf(…)
#include <cwchar>   // --> std::fwprintf(…), std::swprintf(…)
#include <iso646.h> // --> and, not, or
#include <new>      // --> new
#include <stdint.h> // --> UINTPTR_MAX; intmax_t, uintptr_t

/* Main --> del messages.exe messages.res && cls && windres messages.rc -O coff -o messages.res && g++ -DNDEBUG -municode -mwindows -pedantic-errors -std=c++98 -Wall -Werror -Wextra messages.cpp messages.res -lcomctl32 -o messages.exe && messages.exe */
int WINAPI wWinMain(HINSTANCE const instanceHandle, HINSTANCE const, PWSTR const, int const windowAppearance) {
  typedef struct allocator align, rate;
  static  struct allocator /* final */ {
    union allocate {
      struct header /* final */ {
        unsigned char *buffer; //                    ->> Reallocation purposes; See `allocated` parameter in `::allocate(…)`
        unsigned short amount; // --> uint_least16_t ->> `short` has a minimum 16 bit-width — enough to represent `::maximum`
      };
    };

    enum /* : bool */        { previous, next };   // --> ::align(…, ::align::next)
    enum /* : std::size_t */ { maximum = 65535u }; // ->> Arbitrary 64KiB based on (C++11) minimum 17 bit-wide width of `std::ptrdiff_t`
    union { struct allocate::header const _; union { char (allocator:: *const a)(), allocator:: *const b, (*const c)(), *const d; intmax_t const e; long double const f; }; } alignment;
    mutable unsigned char buffer[maximum];                   // --> alignas(struct allocate::header | std::max_align_t) ->> Non-extensible
    std::div_t const      aggressive, basic, exact, passive; // --> ::allocate(…, ::rate::exact, …)

    /* ... */
    void* align(void* const address, std::size_t const alignment, std::size_t const size, signed char const direction) const /* noexcept */ /* --> [[assume(address >= ::buffer and address <= &::buffer[maximum] and alignment and 0u == (alignment & (alignment - 1u)) and maximum >= size)]] */ {
      #if defined UINTPTR_MAX // ->> Total ordering
        uintptr_t const offset  = reinterpret_cast<uintptr_t>(address);
        uintptr_t const aligned = (offset + (direction ? alignment - 1u : 0u)) & ~(alignment - 1u);

        if (aligned <= reinterpret_cast<uintptr_t>(&this -> buffer[maximum]) - size and (direction ? UINTPTR_MAX - offset >= alignment - 1u : aligned >= reinterpret_cast<uintptr_t>(this -> buffer)))
        return &static_cast<unsigned char*>(address)[direction ? aligned - offset : offset - aligned]; // ->> `address` was offset not `::buffer`
      #endif

      if (alignment < /* --> alignof(::buffer) */ sizeof this -> alignment) /* ->> Relative */ {
        std::size_t const offset  = static_cast<unsigned char*>(address) - this -> buffer;
        std::size_t const aligned = (offset + (direction ? alignment - 1u : 0u)) & ~(alignment - 1u);

        if (aligned <= maximum - size)
        return &static_cast<unsigned char*>(address)[direction ? &this -> buffer[aligned] - static_cast<unsigned char*>(address) : static_cast<unsigned char*>(address) - &this -> buffer[aligned]]; // ->> `address` has more provenance than `::buffer`
      }

      return NULL;
    }

    void* allocate(void* const allocated, std::size_t count, std::size_t size, std::div_t const rate, std::size_t* const capacity = NULL) const /* noexcept */ {
      std::size_t const HEADER_SIZE       = sizeof(struct allocate::header);
      std::size_t const HEADER_ALIGNMENT  = HEADER_SIZE; // --> alignof(…)
      std::div_t  const EXPANSION_MINIMUM = {1, 2};      // ->> Threshold for reallocations to expand rather than reuse existing storage
      std::div_t  const EXPANSION_MAXIMUM = {1, 1};
      std::size_t const END               = maximum;

      std::size_t /* --> std::align_val_t */ alignment  = 1u;
      struct allocate::header               *allocation = NULL;
      unsigned char                         *available  = this -> buffer;
      static struct allocate::header        *headers    = NULL; // ->> Stored toward the end of `::buffer`

      // ...
      capacity = NULL == capacity ? &count : capacity; // ->> Reuse unneeded `count` as fallback storage for `capacity`

      while (alignment < size) /* ->> Estimate alignment presuming lack of `alignof(…)` */ {
        if (0u == alignment) { return NULL; } // ->> Unsigned overflow i.e. no valid alignment
        alignment *= 2u;                      // ->> Ensure squared alignment à la C++ constraints
      }

      if (count <= maximum / size and rate.quot > 0 and rate.rem > 0)
      for (std::size_t preamount = 0u, extent = count * size, amount = rate.quot <= rate.rem ? extent : rate.rem; amount != preamount; ) {
        preamount = amount;      // ->> Avoid non-growing `rate`s
        amount   += amount % 2u; // ->> Ensure `amount` (’s parity) is arbitrarily even

        if (amount < extent) /* ... --> amount ·= rate */ {
          if      (amount < (maximum / rate.quot))            amount = (amount * rate.quot) / rate.rem;
          else if (amount < (maximum / rate.quot) * rate.rem) amount = (amount / rate.rem) * rate.quot;
          else if (amount <= maximum - rate.quot) {
            if (std::size_t const _ = (extent - amount) / rate.quot) amount += rate.quot * _;
            else                                                     amount += rate.quot;
          }
        } else {
          do {
            // ->> Acquire appropriate `allocation`
            if (NULL != headers) // --> available = …
            for (struct allocate::header *header = headers; HEADER_SIZE <= &this -> buffer[END] - reinterpret_cast<unsigned char*>(header); available += header++ -> amount) {
              if (not allocated ? NULL == header -> buffer and (NULL == allocation or allocation -> amount > header -> amount) : allocated == header -> buffer)
              allocation = header;
            }

            // ... ->> Provide new `allocation`; Assumed `allocation` would otherwise be a valid `struct ::header`
            if (not allocated or (
              amount >= (allocation -> amount * EXPANSION_MINIMUM.quot) / EXPANSION_MINIMUM.rem or
              amount <  (allocation -> amount * EXPANSION_MAXIMUM.quot) / EXPANSION_MAXIMUM.rem
            )) {
              if (NULL == allocation or NULL == headers) {
                struct allocate::header *const offset = static_cast<struct allocate::header*>(this -> align(&this -> buffer[END - HEADER_SIZE], HEADER_ALIGNMENT, HEADER_SIZE, this -> align::previous));
                if (NULL == offset) return NULL; // ->> No alignment for `headers`

                std::size_t const length = NULL != headers ? &offset[1] - headers : 0u;
                if (&available[0] > offset - (length * 2u) or &available[amount] > offset - (length * 1u)) {
                  if (allocated and EXPANSION_MINIMUM)
                  INLINE: FIND SMALLER SPACE INSTEAD

                  break; // ->> Unable to lengthen `headers`
                }

                struct allocate::header *const preheaders = ::new (offset - (length * 2u)) struct allocate::header[length + 0u];

                // ...
                for (std::size_t index = length; index--; ) { preheaders[index] = headers[index]; }
                headers = ::new (offset - (length * 1u)) struct allocate::header[length + 1u];

                for (std::size_t index = length; index--; ) { headers[index] = preheaders[index]; }
                headers[length].amount = amount;
                headers[length].buffer = available;

                if (allocated) {
                  COPY OVER OLD DATA?
                }

                allocation = &headers[length];
              }

              return allocation -> buffer;
            }

            // ... ->> Reapportion existing `allocation`
            TODO
          } while (false);

          amount -= ((amount - extent) / rate.quot) * rate.quot; // ->> Constrain `amount >= extent`
        }
      }

      return NULL;
    }
  }                   allocator                = {{}, {}, {2, 1}, {3, 2}, {1, 1}, {6, 5}};
  WCHAR               moduleFileName[MAX_PATH] = L"";      // ->> See use of `::ExtractIconW(…)` (or `window -> classInformation.hIcon`)
  POINT               position                 = {0L, 0L}; // ->> For positioning controls
  MSG                 threadMessage            = {};
  static struct main *window                   = NULL;
  static struct main /* final */ {
    struct app /* final */ {
      static wchar_t const* alias(HINSTANCE const instanceHandle) /* noexcept */ {
        std::size_t const    aliasCapacity              = /* --> L"@" */ 1u + /* --> … */ (sizeof(HINSTANCE) * 2u) + /* --> " “" */ 2u + /* --> instanceHandle */ MAX_PATH + /* --> "”" */ 1u;
        static wchar_t       alias[aliasCapacity]       = L"";
        std::size_t          capacity                   = aliasCapacity;
        wchar_t const *const fallback                   = L"...";
        WCHAR                instanceFileName[MAX_PATH] = L"";
        DWORD const          instanceFileNameLength     = ::GetModuleFileNameW(static_cast<HMODULE>(instanceHandle), instanceFileName, MAX_PATH); // ->> `::LoadStringW(instanceHandle, …, instanceFileName, …)` to extract resource ID-specific text

        // ...
        if (0x00u == instanceFileNameLength)
        return fallback;

        capacity -= MAX_PATH;
        capacity += instanceFileNameLength + /* ->> NUL */ 1u;

        if (std::swprintf(alias, aliasCapacity, L"%.1s" "%*.s" "%.2ls" "%.*ls" "%.1ls", "@", static_cast<unsigned>(sizeof(HWND) * 2u), "", L" “", static_cast<unsigned>(instanceFileNameLength), instanceFileName, L"”") < static_cast<int>(capacity - 1u))
        return fallback;

        for (std::size_t index = sizeof(HINSTANCE); index--; ) {
          if (std::swprintf(&alias[1u + ((sizeof(HINSTANCE) - index - 1u) * 2u)], /* --> aliasCapacity - … */ 3u + /* ->> ??? */ 1u, L"%02hX", static_cast<unsigned short>(reinterpret_cast<unsigned char const*>(&instanceHandle)[index])) < 2)
          return fallback;
        }

        alias[1u + (sizeof(HWND) * 2u)] = L' '; // ->> `std::swprintf(…)` NUL-terminates

        while (capacity--)
        switch (alias[capacity]) {
          case L'\u0009': case L'\u000A': case L'\u000C': case L'\u000D': case L'\u0085':
          case L'\u2028': case L'\u2029':
          case L'\uFFF9': case L'\uFFFA': case L'\uFFFB':
            alias[capacity] = L'·';
        }

        return alias;
      }

      static wchar_t const* alias(/* [[nonnull]] */ HWND const windowHandle) /* noexcept */ {
        static LPWSTR        alias         = NULL;    //
        static std::size_t   aliasCapacity = 0u;      //
        unsigned             capacity;                // --> … < INT_MAX
        wchar_t const *const fallback       = L"..."; //
        static std::size_t   recursionDepth = 0u;     //
        int                  windowTextLength;        // --> ::GetWindowTextLengthW(…)

        // ... ->> Non-idempotent
        if (0u != recursionDepth++) {
          recursionDepth -= 0u != recursionDepth;
          return L"<>";
        }

        // ... ->> Length
        ::SetLastError(ERROR_SUCCESS);
        windowTextLength = ::GetWindowTextLengthW(windowHandle); // --> WM_GETTEXTLENGTH ->> Typically `MAX_PATH` for top-level captioned windows

        if (0 == windowTextLength and ERROR_SUCCESS != ::GetLastError()) {
          recursionDepth -= 0u != recursionDepth;
          return fallback;
        }

        capacity = /* --> L"@" */ 1u + /* --> … */ (sizeof(HWND) * 2u) + /* --> " “" */ 2u + /* --> … */ windowTextLength + /* --> "”" */ 2u;

        // ... ->> Allocation
        if (aliasCapacity < capacity) {
          void *const allocation = allocator.allocate(alias, capacity * sizeof(WCHAR), allocator.rate::basic, &aliasCapacity);

          // ...
          if (NULL == allocation) {
            recursionDepth -= 0u != recursionDepth;
            return fallback;
          }

          alias = static_cast<LPWSTR>(allocation);
        }

        // ... ->> Value
        if (std::swprintf(alias, aliasCapacity + /* ->> NUL */ 1u, L"%.1s" "%*.s" "%.2ls" "%*.s" "%.1ls", "@", static_cast<unsigned>(sizeof(HWND) * 2u), "", L" “", windowTextLength, "", L"”") < static_cast<int>(capacity - 1u)) {
          recursionDepth -= 0u != recursionDepth;
          return fallback;
        }

        for (std::size_t index = sizeof(HWND); index--; )
        if (std::swprintf(&alias[1u + ((sizeof(HWND) - index - 1u) * 2u)], 3u /* --> aliasCapacity - … */ + /* ->> ??? */ 1u, L"%02hX", static_cast<unsigned short>(reinterpret_cast<unsigned char const*>(&windowHandle)[index])) < 2) {
          recursionDepth -= 0u != recursionDepth;
          return fallback;
        }

        ::SetLastError(ERROR_SUCCESS);
        windowTextLength = ::GetWindowTextW(windowHandle, &alias[1u + (sizeof(HWND) * 2u) + 2u], windowTextLength + 1 /* --> aliasCapacity - … */); // --> WM_GETTEXT

        if (0 == windowTextLength and ERROR_SUCCESS != ::GetLastError()) {
          recursionDepth -= 0u != recursionDepth;
          return fallback;
        }

        alias[1u + (sizeof(HWND) * 2u)]                         = L' '; // ->> `std::swprintf   (…)` NUL-terminates
        alias[1u + (sizeof(HWND) * 2u) + 2u + windowTextLength] = L'”'; // ->> `::GetWindowTextW(…)` NUL-terminates

        while (capacity--)
        switch (alias[capacity]) {
          case L'\u0009': case L'\u000A': case L'\u000C': case L'\u000D': case L'\u0085':
          case L'\u2028': case L'\u2029':
          case L'\uFFF9': case L'\uFFFA': case L'\uFFFB':
            alias[capacity] = L'·';
        }

        recursionDepth -= 0u != recursionDepth;
        return alias;
      }

      static wchar_t const* alias(UINT const message) /* noexcept */ {
        std::size_t const    aliasCapacity        = /* --> L"WM" ":" */ 3u + /* --> message */ (sizeof(UINT) * 2u) + /* --> … */ 1u;
        static wchar_t       alias[aliasCapacity] = L"";
        wchar_t const *const fallback             = L"WM" ":" "...";

        // ...
        switch (message) {
          case WM_COMMAND:    return L"WM" ":" "COMMAND"; break;
          case WM_DPICHANGED: return L"WM" ":" "DPICHANGED"; break;
          case WM_NOTIFY:     return L"WM" ":" "NOTIFY";  break;
        }

        std::wprintf(L"\r\n" "FUCK: %i" "\r\n", std::swprintf(alias, 5 + 1, L"Hello"));
        return std::swprintf(alias, aliasCapacity, L"%.3s" "%0*X", "WM" ":", static_cast<unsigned>(sizeof(UINT) * 2u), message) < 0 ? fallback : alias;
      }

      static bool contains(HWND const windowHandle, UINT const relationship, HWND const subwindowHandle) /* noexcept */ {
        for (HWND windowChildHandle = ::GetWindow(windowHandle, relationship); NULL != windowChildHandle; windowChildHandle = ::GetWindow(windowChildHandle, GW_HWNDNEXT)) {
          if (subwindowHandle == windowChildHandle)
          return true; // ->> Prefer `::EnumChildWindows(…)` for robust handling instead
        }

        return false;
      }

      static std::size_t count(char    const value[])                          /* noexcept */ { if (NULL == value) return 0u; for (std::size_t length = 0u; ; ++length, value += 1)     { if ('\0'  == *value) return length; } }
      static std::size_t count(wchar_t const value[])                          /* noexcept */ { if (NULL == value) return 0u; for (std::size_t length = 0u; ; ++length, value += 1)     { if (L'\0' == *value) return length; } } // --> std::wcslen([[nonnull]] value)
      static std::size_t count(long          value, unsigned char radix = 10u) /* noexcept */ { if (0    == value) return 1u; for (std::size_t length = 0u; ; ++length, value /= radix) { if (0     ==  value) return length; } } // ->> Counts digits only, not signedness

      static char const* enumerate(intmax_t value, char const* names, intmax_t enumerator1 = 0x00L, intmax_t enumerator2 = 0x00L, intmax_t enumerator3 = 0x00L, intmax_t enumerator4 = 0x00L, intmax_t enumerator5 = 0x00L, intmax_t enumerator6 = 0x00L, intmax_t enumerator7 = 0x00L, intmax_t enumerator8 = 0x00L, intmax_t enumerator9 = 0x00L, intmax_t enumerator10 = 0x00L, intmax_t enumerator11 = 0x00L, intmax_t enumerator12 = 0x00L, intmax_t enumerator13 = 0x00L, intmax_t enumerator14 = 0x00L, intmax_t enumerator15 = 0x00L, intmax_t enumerator16 = 0x00L, intmax_t enumerator17 = 0x00L, intmax_t enumerator18 = 0x00L, intmax_t enumerator19 = 0x00L, intmax_t enumerator20 = 0x00L, intmax_t enumerator21 = 0x00L, intmax_t enumerator22 = 0x00L, intmax_t enumerator23 = 0x00L, intmax_t enumerator24 = 0x00L, intmax_t enumerator25 = 0x00L, intmax_t enumerator26 = 0x00L, intmax_t enumerator27 = 0x00L, intmax_t enumerator28 = 0x00L, intmax_t enumerator29 = 0x00L, intmax_t enumerator30 = 0x00L, intmax_t enumerator31 = 0x00L, intmax_t enumerator32 = 0x00L, intmax_t enumerator33 = 0x00L, intmax_t enumerator34 = 0x00L, intmax_t enumerator35 = 0x00L, intmax_t enumerator36 = 0x00L, intmax_t enumerator37 = 0x00L, intmax_t enumerator38 = 0x00L, intmax_t enumerator39 = 0x00L, intmax_t enumerator40 = 0x00L, intmax_t enumerator41 = 0x00L, intmax_t enumerator42 = 0x00L, intmax_t enumerator43 = 0x00L, intmax_t enumerator44 = 0x00L, intmax_t enumerator45 = 0x00L, intmax_t enumerator46 = 0x00L, intmax_t enumerator47 = 0x00L, intmax_t enumerator48 = 0x00L, intmax_t enumerator49 = 0x00L, intmax_t enumerator50 = 0x00L, intmax_t enumerator51 = 0x00L, intmax_t enumerator52 = 0x00L, intmax_t enumerator53 = 0x00L, intmax_t enumerator54 = 0x00L, intmax_t enumerator55 = 0x00L, intmax_t enumerator56 = 0x00L, intmax_t enumerator57 = 0x00L, intmax_t enumerator58 = 0x00L, intmax_t enumerator59 = 0x00L, intmax_t enumerator60 = 0x00L, intmax_t enumerator61 = 0x00L, intmax_t enumerator62 = 0x00L, intmax_t enumerator63 = 0x00L, intmax_t enumerator64 = 0x00L, intmax_t enumerator65 = 0x00L, intmax_t enumerator66 = 0x00L, intmax_t enumerator67 = 0x00L, intmax_t enumerator68 = 0x00L, intmax_t enumerator69 = 0x00L, intmax_t enumerator70 = 0x00L, intmax_t enumerator71 = 0x00L, intmax_t enumerator72 = 0x00L, intmax_t enumerator73 = 0x00L, intmax_t enumerator74 = 0x00L, intmax_t enumerator75 = 0x00L, intmax_t enumerator76 = 0x00L, intmax_t enumerator77 = 0x00L, intmax_t enumerator78 = 0x00L, intmax_t enumerator79 = 0x00L, intmax_t enumerator80 = 0x00L, intmax_t enumerator81 = 0x00L, intmax_t enumerator82 = 0x00L, intmax_t enumerator83 = 0x00L, intmax_t enumerator84 = 0x00L, intmax_t enumerator85 = 0x00L, intmax_t enumerator86 = 0x00L, intmax_t enumerator87 = 0x00L, intmax_t enumerator88 = 0x00L, intmax_t enumerator89 = 0x00L, intmax_t enumerator90 = 0x00L, intmax_t enumerator91 = 0x00L, intmax_t enumerator92 = 0x00L, intmax_t enumerator93 = 0x00L, intmax_t enumerator94 = 0x00L, intmax_t enumerator95 = 0x00L, intmax_t enumerator96 = 0x00L, intmax_t enumerator97 = 0x00L, intmax_t enumerator98 = 0x00L, intmax_t enumerator99 = 0x00L, intmax_t enumerator100 = 0x00L, intmax_t enumerator101 = 0x00L, intmax_t enumerator102 = 0x00L, intmax_t enumerator103 = 0x00L, intmax_t enumerator104 = 0x00L, intmax_t enumerator105 = 0x00L, intmax_t enumerator106 = 0x00L, intmax_t enumerator107 = 0x00L, intmax_t enumerator108 = 0x00L, intmax_t enumerator109 = 0x00L, intmax_t enumerator110 = 0x00L, intmax_t enumerator111 = 0x00L, intmax_t enumerator112 = 0x00L, intmax_t enumerator113 = 0x00L, intmax_t enumerator114 = 0x00L, intmax_t enumerator115 = 0x00L, intmax_t enumerator116 = 0x00L, intmax_t enumerator117 = 0x00L, intmax_t enumerator118 = 0x00L, intmax_t enumerator119 = 0x00L, intmax_t enumerator120 = 0x00L, intmax_t enumerator121 = 0x00L, intmax_t enumerator122 = 0x00L, intmax_t enumerator123 = 0x00L, intmax_t enumerator124 = 0x00L, ...) /* noexcept */ {
        struct enumerator /* final */ {
          char const          *name;
          std::size_t          nameLength;
          intmax_t /* const */ value; // --> `va_arg(…, T)` does not robustly handle `enum` types (`intmax_t` doesn’t either, but it’s more straightforward than `template` substitutions)
          bool                 enumerated;
        }                  enumerators[]          = {{NULL, 0u, enumerator1, false}, {NULL, 0u, enumerator2, false}, {NULL, 0u, enumerator3, false}, {NULL, 0u, enumerator4, false}, {NULL, 0u, enumerator5, false}, {NULL, 0u, enumerator6, false}, {NULL, 0u, enumerator7, false}, {NULL, 0u, enumerator8, false}, {NULL, 0u, enumerator9, false}, {NULL, 0u, enumerator10, false}, {NULL, 0u, enumerator11, false}, {NULL, 0u, enumerator12, false}, {NULL, 0u, enumerator13, false}, {NULL, 0u, enumerator14, false}, {NULL, 0u, enumerator15, false}, {NULL, 0u, enumerator16, false}, {NULL, 0u, enumerator17, false}, {NULL, 0u, enumerator18, false}, {NULL, 0u, enumerator19, false}, {NULL, 0u, enumerator20, false}, {NULL, 0u, enumerator21, false}, {NULL, 0u, enumerator22, false}, {NULL, 0u, enumerator23, false}, {NULL, 0u, enumerator24, false}, {NULL, 0u, enumerator25, false}, {NULL, 0u, enumerator26, false}, {NULL, 0u, enumerator27, false}, {NULL, 0u, enumerator28, false}, {NULL, 0u, enumerator29, false}, {NULL, 0u, enumerator30, false}, {NULL, 0u, enumerator31, false}, {NULL, 0u, enumerator32, false}, {NULL, 0u, enumerator33, false}, {NULL, 0u, enumerator34, false}, {NULL, 0u, enumerator35, false}, {NULL, 0u, enumerator36, false}, {NULL, 0u, enumerator37, false}, {NULL, 0u, enumerator38, false}, {NULL, 0u, enumerator39, false}, {NULL, 0u, enumerator40, false}, {NULL, 0u, enumerator41, false}, {NULL, 0u, enumerator42, false}, {NULL, 0u, enumerator43, false}, {NULL, 0u, enumerator44, false}, {NULL, 0u, enumerator45, false}, {NULL, 0u, enumerator46, false}, {NULL, 0u, enumerator47, false}, {NULL, 0u, enumerator48, false}, {NULL, 0u, enumerator49, false}, {NULL, 0u, enumerator50, false}, {NULL, 0u, enumerator51, false}, {NULL, 0u, enumerator52, false}, {NULL, 0u, enumerator53, false}, {NULL, 0u, enumerator54, false}, {NULL, 0u, enumerator55, false}, {NULL, 0u, enumerator56, false}, {NULL, 0u, enumerator57, false}, {NULL, 0u, enumerator58, false}, {NULL, 0u, enumerator59, false}, {NULL, 0u, enumerator60, false}, {NULL, 0u, enumerator61, false}, {NULL, 0u, enumerator62, false}, {NULL, 0u, enumerator63, false}, {NULL, 0u, enumerator64, false}, {NULL, 0u, enumerator65, false}, {NULL, 0u, enumerator66, false}, {NULL, 0u, enumerator67, false}, {NULL, 0u, enumerator68, false}, {NULL, 0u, enumerator69, false}, {NULL, 0u, enumerator70, false}, {NULL, 0u, enumerator71, false}, {NULL, 0u, enumerator72, false}, {NULL, 0u, enumerator73, false}, {NULL, 0u, enumerator74, false}, {NULL, 0u, enumerator75, false}, {NULL, 0u, enumerator76, false}, {NULL, 0u, enumerator77, false}, {NULL, 0u, enumerator78, false}, {NULL, 0u, enumerator79, false}, {NULL, 0u, enumerator80, false}, {NULL, 0u, enumerator81, false}, {NULL, 0u, enumerator82, false}, {NULL, 0u, enumerator83, false}, {NULL, 0u, enumerator84, false}, {NULL, 0u, enumerator85, false}, {NULL, 0u, enumerator86, false}, {NULL, 0u, enumerator87, false}, {NULL, 0u, enumerator88, false}, {NULL, 0u, enumerator89, false}, {NULL, 0u, enumerator90, false}, {NULL, 0u, enumerator91, false}, {NULL, 0u, enumerator92, false}, {NULL, 0u, enumerator93, false}, {NULL, 0u, enumerator94, false}, {NULL, 0u, enumerator95, false}, {NULL, 0u, enumerator96, false}, {NULL, 0u, enumerator97, false}, {NULL, 0u, enumerator98, false}, {NULL, 0u, enumerator99, false}, {NULL, 0u, enumerator100, false}, {NULL, 0u, enumerator101, false}, {NULL, 0u, enumerator102, false}, {NULL, 0u, enumerator103, false}, {NULL, 0u, enumerator104, false}, {NULL, 0u, enumerator105, false}, {NULL, 0u, enumerator106, false}, {NULL, 0u, enumerator107, false}, {NULL, 0u, enumerator108, false}, {NULL, 0u, enumerator109, false}, {NULL, 0u, enumerator110, false}, {NULL, 0u, enumerator111, false}, {NULL, 0u, enumerator112, false}, {NULL, 0u, enumerator113, false}, {NULL, 0u, enumerator114, false}, {NULL, 0u, enumerator115, false}, {NULL, 0u, enumerator116, false}, {NULL, 0u, enumerator117, false}, {NULL, 0u, enumerator118, false}, {NULL, 0u, enumerator119, false}, {NULL, 0u, enumerator120, false}, {NULL, 0u, enumerator121, false}, {NULL, 0u, enumerator122, false}, {NULL, 0u, enumerator123, false}, {NULL, 0u, enumerator124, false}};
        std::size_t        capacity               = /* --> "enum<" */ 5u + /* --> " | ..." ">" */ 8u; //
        std::size_t        count                  = 0u;                                               // ->> Number of `names` matching `value`
        char     const     delimiter[]            = " | ";                                            //
        unsigned const     delimiterCapacity      = sizeof delimiter / sizeof(char);                  //
        unsigned           delimiterLength        = 0u;                                               //
        intmax_t           enumerated             = 0x00L;                                            // ->> Disjunction of `enumerators` matching `value`
        static char       *enumeration            = NULL;                                             //
        static std::size_t enumerationCapacity    = 0u;                                               //
        std::size_t        enumerationLength      = 0u;                                               //
        char const *const  fallback               = "enum<" "..." ">";                                //
        std::size_t        total                  = 0u;                                               // ->> Number of `names` or `enumeration` items
        std::size_t        valueDecimalLength     = 0u;                                               //
        std::size_t        valueHexadecimalLength = 0u;                                               //

        // ... ->> Setup
        for (char const *name = names; ; ++name) {
          bool const ended = total == sizeof enumerators / sizeof(struct enumerator) or '\0' == *name;

          // ...
          if (ended or *name == ' ') {
            enumerators[total].name       = names;
            enumerators[total].nameLength = name - names;
            names                        += enumerators[total++].nameLength + 1u;

            if (ended)
            break;
          }
        }

        // ... ->> Match
        for (struct enumerator *enumerator = enumerators; total--; ++enumerator)
        if (enumerator -> value == value or (value > 0x00L and enumerator -> value == (value & enumerator -> value))) /* --> or 0x00L == enumerator -> value */ {
          capacity                += enumerator -> nameLength + (0u != count ? delimiterCapacity - 1u : 0u); // ->> Allocation
          enumerated              |= enumerator -> value;
          enumerator -> enumerated = true;
          enumerators[count++]     = *enumerator;
        }

        value &= ~enumerated;

        if (0x00L != value) {
          valueHexadecimalLength = app::count(value, 16u);
          valueHexadecimalLength = valueHexadecimalLength > 8u ? valueHexadecimalLength : 8u; // --> (std::size_t) std::fminl(…, 8zu)
          valueDecimalLength     = app::count(value, 10u);
          capacity              += (0u != count ? delimiterCapacity - 1u : 0u) + /* --> "0x" */ 2u + /* --> value */ valueHexadecimalLength + /* --> " (" */ 2u + /* --> "-" */ (value < 0L ? 1u : 0u) + /* --> value */ valueDecimalLength + /* --> ")" */ 1u;
        }

        // ... ->> Allocation
        if (capacity > enumerationCapacity) {
          void *const allocation = allocator.allocate(enumeration, capacity * sizeof(char), allocator.rate::passive, &enumerationCapacity);

          if (NULL == allocation) return fallback;
          enumeration = static_cast<char*>(allocation);
        }

        // ... ->> Value
        if (std::sprintf(&enumeration[enumerationLength], "%.5s", "enum<") < static_cast<int>(5))
        return fallback;

        enumerationLength = 5u; // --> "enum<"
        total             = 0u;

        for (struct enumerator *enumerator = enumerators; count--; ++enumerator) {
          delimiterLength = 0u != total ? delimiterCapacity - 1u : 0u;

          if (std::sprintf(&enumeration[enumerationLength], "%.*s" "%.*s", delimiterLength, delimiter, static_cast<unsigned>(enumerator -> nameLength), enumerator -> name) < static_cast<int>(delimiterLength + enumerator -> nameLength))
          continue;

          enumerationLength += delimiterLength + enumerator -> nameLength;
          total++;
        }

        if (0x00L != value) do {
          delimiterLength = 0u != total ? delimiterCapacity - 1u : 0u;

          if (std::sprintf(&enumeration[enumerationLength], "%.*s" "%.2s%08lX" "%.2s%li%c", delimiterLength, delimiter, "0x", value, " (", value, ')') < static_cast<int>(delimiterLength + /* --> "0x" */ 2u + /* --> value */ valueHexadecimalLength + /* --> " (" */ 2u + /* --> "-" */ (value < 0L ? 1u : 0u) + /* --> value */ valueDecimalLength + /* --> ")" */ 1u))
          continue;

          enumerationLength += delimiterLength + 2u + valueHexadecimalLength + 2u + (value < 0L ? 1u : 0u) + valueDecimalLength + 1u;
          total++;
          value = 0x00L;
        } while (false);

        if (0u == total or 0x00L != value) {
          delimiterLength = 0u != total ? delimiterCapacity - 1u : 0u;

          if (std::sprintf(&enumeration[enumerationLength], "%.*s" "%.3s", delimiterLength, delimiter, "...") < static_cast<int>(delimiterLength + 3u))
          return fallback;

          enumerationLength += delimiterLength + 3u;
        }

        enumeration[enumerationLength++] = '>';
        enumeration[enumerationLength++] = '\0';

        // ...
        return enumeration;
      }
    };

    /* ... */
    HWND                                                                  handle, tooltipHandle;
    struct /* final */ { std::size_t capacity, length; UINT *processed; } messages;
    INITCOMMONCONTROLSEX                                                  commonControlsInitializer;
    RECT                                                                  clientBounds;
    WNDCLASSEXW                                                           classInformation;

    static LRESULT CALLBACK procedure(HWND windowHandle, UINT message, WPARAM parameter, LPARAM subparameter) {
      struct /* final */ {
        LRESULT value; bool processable, processed;
        void operator =(LRESULT const value) /* noexcept */ { this -> processed = true; this -> value = value; }
      } result = {0x00L, true, false};

      // ... ->> Allocation
      if (window -> messages.capacity == window -> messages.length) {
        void *const allocation = allocator.allocate(window -> messages.processed, window -> messages.length + 1u, allocator.rate::aggressive, &window -> messages.capacity);

        if (NULL == allocation) result.processable = false;
        else          window -> messages.processed = static_cast<UINT*>(allocation);
      }

      // ... -> Processing
      if (result.processable) {
        window -> messages.processed[window -> messages.length] = message;
        window -> messages.length++;

        #if true
          for (std::size_t index = (std::fwprintf(stdout, L"[…]: (%lu) {", static_cast<unsigned long>(window -> messages.length)), 0u); index != window -> messages.length; ++index)
          (void) std::fwprintf(stdout, L"%ls%s", app::alias(window -> messages.processed[index]), index != window -> messages.length - 1u ? ", " : "}" "\r\n");
        #endif

        if (wchar_t const *const messageName = app::alias(message))
        switch (message) {
          case WM_COMMAND: /* ->> `user32` Controls */ {
            if (app::contains(windowHandle, GW_CHILD, reinterpret_cast<HWND>(subparameter))) // ->> `subparameter` otherwise null when originated from accelerators or menus
            (void) std::fwprintf(stdout, L"[%ls]: Command recieved: %i" "\r\n", messageName, static_cast<int>(LOWORD(parameter)));
          } break;

          case WM_NOTIFY: /* ->> `commctrl` Common Controls */ {
            LPNMHDR const notificationMessageHeader = reinterpret_cast<LPNMHDR>(subparameter); // --> NMHDR*

            // ...
            std::wprintf(L"\r\n" "[#A]: Notifying" "\r\n");
            if (window -> messages.length > 1u and WM_NOTIFY == window -> messages.processed[window -> messages.length - 2u]) {
              if (notificationMessageHeader -> code == TTN_NEEDTEXTW) {
                std::wprintf(L"[#1]: Duplicate inquiry" "\r\n");
                reinterpret_cast<LPNMTTDISPINFOW>(subparameter) -> hinst    = static_cast<HINSTANCE>(NULL); // ->> `::lpszText` is not a resource ID
                reinterpret_cast<LPNMTTDISPINFOW>(subparameter) -> lpszText = (wchar_t*) L"BRUH";
              } // TODO: Fix all swprintf

              std::wprintf(L"[#2]: Duplicate" "\r\n");
              result.processable = false;
              break;
            }

            // ... ->> Some `NMHDR::code`s may be intercepted by tooltip `TTTOOLINFOW::uFlags`; `GW_ENABLEDPOPUP` applies after `TTN_SHOW` and ends with `TTN_POP`
            std::wprintf(L"[#B]: Notified" "\r\n");
            (void) std::fwprintf(stdout, L"[%ls]: 0x%08lX; NMHDR {hwndFrom: %ls, idFrom: 0x%08X, code: %s}" "\r\n", messageName, static_cast<unsigned long>(parameter), app::alias(notificationMessageHeader -> hwndFrom), notificationMessageHeader -> idFrom, app::enumerate(notificationMessageHeader -> code, "TTN_GETDISPINFOW TTN_LINKCLICK TTN_NEEDTEXTW TTN_POP TTN_SHOW", TTN_GETDISPINFOW, TTN_LINKCLICK, TTN_NEEDTEXTW, TTN_POP, TTN_SHOW));

            if (notificationMessageHeader -> hwndFrom == window -> tooltipHandle or app::contains(windowHandle, GW_ENABLEDPOPUP, notificationMessageHeader -> hwndFrom))
            switch (notificationMessageHeader -> code) {
              case TTN_GETDISPINFOW | TTN_NEEDTEXTW: {
                LPNMTTDISPINFOW const notificationMessageTooltipDisplayInformation             = reinterpret_cast<LPNMTTDISPINFOW>(subparameter);
                std::size_t     const notificationMessageTooltipDisplayInformationTextCapacity = sizeof notificationMessageTooltipDisplayInformation -> szText / sizeof(WCHAR); // ->> NUL included
                UINT            const notificationMessageTooltipId                             = notificationMessageTooltipDisplayInformation -> hdr.idFrom;
                std::size_t           notificationMessageTooltipIdLength                       = app::count(notificationMessageTooltipId, 16u);
                HWND           const  notificationMessageTooltipWindowHandle                   = 0x00u != (notificationMessageTooltipDisplayInformation -> uFlags & TTF_IDISHWND) ? reinterpret_cast<HWND>(notificationMessageTooltipId) : NULL;
                wchar_t const *const  notificationMessageTooltipWindowAlias                    = NULL != notificationMessageTooltipWindowHandle ? L"HWND" /*app::alias(notificationMessageTooltipWindowHandle)*/ : NULL;
                std::size_t    const  notificationMessageTooltipWindowAliasLength              = NULL != notificationMessageTooltipWindowAlias  ? app::count(notificationMessageTooltipWindowAlias)  : 0u;

                // ...
                notificationMessageTooltipIdLength = /* --> L"0x" */ 2u + /* --> notificationMessageTooltipId */ (notificationMessageTooltipIdLength > 8u ? notificationMessageTooltipIdLength : 8u); // --> … + (std::size_t) std::fminl(…, 8zu)

                do {
                  if (
                    NULL != notificationMessageTooltipWindowHandle                                                         and
                    notificationMessageTooltipDisplayInformationTextCapacity > notificationMessageTooltipWindowAliasLength and
                    std::swprintf(notificationMessageTooltipDisplayInformation -> szText, notificationMessageTooltipDisplayInformationTextCapacity, L"%.*ls", notificationMessageTooltipWindowAliasLength, notificationMessageTooltipWindowAlias) >= static_cast<int>(notificationMessageTooltipWindowAliasLength)
                  ) break;

                  if (
                    notificationMessageTooltipDisplayInformationTextCapacity > notificationMessageTooltipIdLength and
                    std::swprintf(notificationMessageTooltipDisplayInformation -> szText, notificationMessageTooltipDisplayInformationTextCapacity, L"0x%08X", notificationMessageTooltipId) >= static_cast<int>(notificationMessageTooltipIdLength)
                  ) break;

                  if (std::swprintf(notificationMessageTooltipDisplayInformation -> szText, notificationMessageTooltipDisplayInformationTextCapacity, L"%.21ls", L"Dynamic \ntooltip text") >= 21)
                  break;

                  notificationMessageTooltipDisplayInformation -> szText[0] = L'\0';
                } while (false);

                notificationMessageTooltipDisplayInformation -> hinst    = static_cast<HINSTANCE>(NULL); // ->> `::lpszText` is not a resource ID
                notificationMessageTooltipDisplayInformation -> lpszText = notificationMessageTooltipDisplayInformation -> szText;
                notificationMessageTooltipDisplayInformation -> uFlags  |= TTF_DI_SETITEM; // ->> Retain tooltip display/ text information (prevents successive `TTN_GETDISPINFOW | TTN_NEEDTEXTW` paths)

                std::wprintf(L"[#C]: Inquired" "\r\n");
                // (void) std::fwprintf(stdout, L"  " "Inquiring tooltip display/ text from" "\r\n" "  " "`NMTTDISPINFOW {hdr: {hwndFrom: %ls, idFrom: 0x%08X, code: %s}, hinst: %ls, lpszText: ", app::alias(notificationMessageTooltipDisplayInformation -> hdr.hwndFrom), notificationMessageTooltipDisplayInformation -> hdr.idFrom, app::enumerate(notificationMessageTooltipDisplayInformation -> hdr.code, "TTN_GETDISPINFOW TTN_LINKCLICK TTN_NEEDTEXTW TTN_POP TTN_SHOW", TTN_GETDISPINFOW, TTN_LINKCLICK, TTN_NEEDTEXTW, TTN_POP, TTN_SHOW), app::alias(notificationMessageTooltipDisplayInformation -> hinst));
                // (void)   std::fwprintf(stdout, notificationMessageTooltipDisplayInformation -> lpszText != notificationMessageTooltipDisplayInformation -> szText ? L"“%ls”" : L".szText", notificationMessageTooltipDisplayInformation -> lpszText);
                // (void)   std::fwprintf(stdout, L", szText: “%.*ls”, uFlags: %s, lParam: %li}`" "\r\n", static_cast<unsigned>(notificationMessageTooltipDisplayInformationTextCapacity - 1u), notificationMessageTooltipDisplayInformation -> szText, app::enumerate(notificationMessageTooltipDisplayInformation -> uFlags, "TTF_ABSOLUTE TTF_CENTERTIP TTF_IDISHWND TTF_PARSELINKS TTF_RTLREADING TTF_SUBCLASS TTF_TRACK TTF_TRANSPARENT", TTF_ABSOLUTE, TTF_CENTERTIP, TTF_IDISHWND, TTF_PARSELINKS, TTF_RTLREADING, TTF_SUBCLASS, TTF_TRACK, TTF_TRANSPARENT), notificationMessageTooltipDisplayInformation -> lParam);
              } break;

              case TTN_LINKCLICK: (void) std::fwprintf(stdout, L"  " "Hyperlinked tooltip" "\r\n");                 break;
              case TTN_POP:       (void) std::fwprintf(stdout, L"  " "Hiding tooltip"      "\r\n");                 break;
              case TTN_SHOW:      (void) std::fwprintf(stdout, L"  " "Showing tooltip"     "\r\n"); result = FALSE; break; // ->> `TRUE` if manually re-positioned via `::SetWindowPos(…, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER)` (and `::SendMessage(…, TTM_ADJUSTRECT, TRUE, (LPARAM) …)`)
            }
          } break;

          // case WM_DPICHANGED: {
          //   // 1. The new DPI is in the low word of wParam
          //   UINT newDpi = LOWORD(wParam);

          //   // 2. Windows provides a suggested new size/position in lParam
          //   // This RECT is calculated by Windows to keep the window
          //   // visually the same size relative to the new monitor.
          //   RECT* const prcNewWindow = (RECT*)lParam;

          //   // 3. Resize and reposition the window based on Windows' suggestion
          //   SetWindowPos(hwnd,
          //       NULL,
          //       prcNewWindow->left,
          //       prcNewWindow->top,
          //       prcNewWindow->right - prcNewWindow->left,
          //       prcNewWindow->bottom - prcNewWindow->top,
          //       SWP_NOZORDER | SWP_NOACTIVATE);
          // } break;
        }

        window -> messages.length--;
      }

      if (not result.processable)
      if (wchar_t const *const messageName = app::alias(message)) {
        // (void) std::fwprintf(stderr, L"[%ls]: Defaulted" "\r\n", messageName);
      }

      switch (message) {
        case WM_CLOSE: case WM_DESTROY:              ::PostQuitMessage(EXIT_SUCCESS); break;
        case WM_KEYDOWN: if (VK_ESCAPE == parameter) ::PostQuitMessage(EXIT_SUCCESS); break;
        case WM_QUIT:                         (void) ::DestroyWindow  (windowHandle); break;
      }

      return result.processed ? result.value : ::DefWindowProcW(windowHandle, message, parameter, subparameter);
    }
  } w = {};

  // ...
  (void) std::setlocale(LC_ALL, ".UTF8");                                                                                   // ->> Presumed successful
  (void) _setmode(_fileno(stdout) /* ->> POSIX’s `STDOUT_FILENO` */, _O_WTEXT /* ->> `_O_U16TEXT` with Byte-Order Mark */); // --> std::fwide(stdout, 0x1)

  threadMessage.hwnd                        = static_cast<HWND>(NULL); // --> window -> handle
  threadMessage.lParam                      = 0x00L;
  threadMessage.message                     = 0x00u;
  threadMessage.pt.x                        = 0L;
  threadMessage.pt.y                        = 0L;
  threadMessage.time                        = 0u;
  threadMessage.wParam                      = EXIT_SUCCESS;
  window                                    = &w;
  window -> classInformation.cbClsExtra     = 0;
  window -> classInformation.cbSize         = sizeof(WNDCLASSEX);
  window -> classInformation.cbWndExtra     = 0;
  window -> classInformation.hbrBackground  = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); // --> ::GetSysColorBrush(COLOR_WINDOW);
  window -> classInformation.hCursor        = ::LoadCursor(static_cast<HINSTANCE>(NULL), IDC_ARROW); // --> static_cast<HCURSOR>(::LoadImage(NULL, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED));
  window -> classInformation.hIcon          = 0x00u != ::GetModuleFileNameW(static_cast<HMODULE>(NULL), moduleFileName, MAX_PATH) ? ::ExtractIconW(static_cast<HINSTANCE>(::GetCurrentProcess()), moduleFileName, 0u) : static_cast<HICON>(NULL);
  window -> classInformation.hIconSm        = static_cast<HICON>(NULL);
  window -> classInformation.hInstance      = instanceHandle;
  window -> classInformation.lpfnWndProc    = &window -> procedure;
  window -> classInformation.lpszClassName  = L"window";
  window -> classInformation.lpszMenuName   = static_cast<LPCWSTR>(NULL);
  window -> classInformation.style          = CS_HREDRAW | CS_VREDRAW;
  window -> clientBounds.left               = 0L;
  window -> clientBounds.top                = 0L;
  window -> commonControlsInitializer.dwICC =
    ICC_ANIMATE_CLASS      | // ->> Animation controls
    ICC_BAR_CLASSES        | // ->> Toolbars, Status Bars, and Track Bars
    ICC_COOL_CLASSES       | // ->> Rebars
    ICC_DATE_CLASSES       | // ->> Calendars and Date Pickers
    ICC_HOTKEY_CLASS       | // ->> Hotkey Inputs
    ICC_INTERNET_CLASSES   | // ->> IP Address controls
    ICC_LINK_CLASS         | // ->> Hyperlink Texts
    ICC_LISTVIEW_CLASSES   | // ->> List View and Headers
    ICC_NATIVEFNTCTL_CLASS | // ->> Native Font Managers
    ICC_PAGESCROLLER_CLASS | // ->> Pager controls
    ICC_PROGRESS_CLASS     | // ->> Progress Bars
    ICC_STANDARD_CLASSES   | // ->> Buttons, Combo Boxes, Edits, List Boxes, Scrollbars, Statics, …
    ICC_TAB_CLASSES        | // ->> Tab Controls and Tooltips
    ICC_TREEVIEW_CLASSES   | // ->> Tree View and Tooltips
    ICC_UPDOWN_CLASS       | // ->> Up-Down Spinners
    ICC_USEREX_CLASSES     | // ->> Extended Combo Boxes
    ICC_WIN95_CLASSES;
  window -> commonControlsInitializer.dwSize = sizeof(INITCOMMONCONTROLSEX);
  window -> messages.capacity                = 0u;
  window -> messages.length                  = 0u;
  window -> messages.processed               = static_cast<UINT*>(NULL);
  window -> tooltipHandle                    = static_cast<HWND> (NULL);

  if (FALSE != ::InitCommonControlsEx(&window -> commonControlsInitializer) and 0x0000u != ::RegisterClassExW(&window -> classInformation)) {
    window -> handle = ::CreateWindowExW(0x00u, window -> classInformation.lpszClassName, L"Windows Event Messages", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 480, 320, static_cast<HWND>(NULL), static_cast<HMENU>(NULL), window -> classInformation.hInstance, static_cast<LPVOID>(NULL));

    // ... ->> Add child windows to interpret more event messages
    #if true
      if (NULL != window -> handle) {
        if (FALSE == ::GetClientRect(window -> handle, &window -> clientBounds)) {
          window -> clientBounds.bottom = 320L; // ->> Client Height
          window -> clientBounds.right  = 480L; // ->> Client Width
        }

        for (enum /* : unsigned char */ {
          BUTTON,
          CHECKBOX, COMBOBOX, CONTENT,
          DATETIME, DROPDOWN,
          EDIT,
          HEADER,
          LISTBOX, LISTVIEW,
          MDICLIENT,
          PROGRESS,
          RICHEDIT,
          SCROLLBAR, SPINNER, STATIC, STATUSBAR,
          TABCONTROL, TOOLBAR, TOOLTIP, TREEVIEW
          // #32768
          // The system class designated for menus.

          // #32769
          // The system class for the desktop window itself.

          // #32770
          // The predefined class for dialog boxes.

          // #32771
          // The class for the task-switch (Alt+Tab) window.

          // #32772
          // The class for icon titles.

          // ComboLBox
          // The internal list box utilized within a combo box.

          // Message
          // A class for message-only windows, which are invisible and used for inter-process communication.
        } const controlKinds[] = {STATUSBAR, TOOLBAR, TOOLTIP}, *controlKind = controlKinds + (sizeof controlKinds / sizeof *controlKinds); controlKind-- != controlKinds; ) {
          struct control /* final */ {
            HWND handle;
            union { RECT bounds; POINT position; };

            /* ... */
            static bool measure(void* const window, struct control* const control) /* noexcept */ {
              if (FALSE != ::GetWindowRect(control -> handle, &control -> bounds)) {
                POINT      clientBoundsMaximum = {control -> bounds.right, control -> bounds.bottom};
                HWND const windowHandle        = *static_cast<HWND*>(window);

                // ...
                if (FALSE != ::ScreenToClient(windowHandle, &clientBoundsMaximum) and FALSE != ::ScreenToClient(windowHandle, &control -> position)) {
                  control -> bounds.bottom = clientBoundsMaximum.y;
                  control -> bounds.right  = clientBoundsMaximum.x;

                  return true;
                }
              }

              return false;
            }
          } control = {NULL, {0L, 0L, 0L, 0L}};

          // ...
          switch (*controlKind) {
            case BUTTON: break;
            case CHECKBOX: break;
            case CONTENT: break;
            case DATETIME: break;
            case DROPDOWN: break;
            case EDIT: break;
            case HEADER: break;
            case PROGRESS: break;
            case RICHEDIT: break;
            case SCROLLBAR: break;
            case SPINNER: break;
            case STATUSBAR: {
              struct statusbar /* final */ {
                enum /* : unsigned char */ { count = 3u }; // --> <= 256u

                RECT &bounds; HWND &handle; POINT &position;
                int const parts[statusbar::count]; // ->> Right bound of each slice of the bar
              } const statusbar = {control.bounds, control.handle, control.position, {50, 150, -1}};

              // ...
              statusbar.handle = ::CreateWindowExW(0x00u, STATUSCLASSNAME, static_cast<LPCWSTR>(NULL), (CCS_NOPARENTALIGN | CCS_NORESIZE) /* ->> Automatic layout otherwise */ | SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE, position.x, position.y, window -> clientBounds.right, 69, window -> handle, reinterpret_cast<HMENU>(0) /* ->> Arbitrary integer ID */, window -> classInformation.hInstance, static_cast<LPVOID>(NULL));

              if (NULL != statusbar.handle) {
                if (control::measure(window, &control)) {
                  (void) ::MoveWindow(statusbar.handle, statusbar.position.x, statusbar.position.y, statusbar.bounds.right - statusbar.bounds.left, statusbar.bounds.bottom - statusbar.bounds.top, TRUE);

                  position.x  = 0L;
                  position.y += statusbar.bounds.bottom - statusbar.bounds.top;
                }

                if (FALSE != ::SendMessageW(statusbar.handle, SB_SETPARTS, statusbar::count, reinterpret_cast<LPARAM>(statusbar.parts))) {
                  (void) ::SendMessageW(statusbar.handle, SB_SETTEXT, MAKEWPARAM(0u, SBT_NOBORDERS | SBT_NOTABPARSING /* | … */), reinterpret_cast<LPARAM>(L"Hello")); // --> FALSE, TRUE ->> See `SBT_OWNERDRAW` too
                  (void) ::SendMessageW(statusbar.handle, SB_SETTEXT, MAKEWPARAM(1u, 0x00u),                                      reinterpret_cast<LPARAM>(L"World!"));
                }
              }
            } break;

            case TABCONTROL: break;

            case TOOLBAR: {
              struct toolbar /* final */ {
                struct buttons /* final */ {
                  enum /* : const WPARAM */ { count = 2u };
                  enum /* : const WORD */   { height = 42u, width = 42u }; // ->> 24×22 pixels by default --- CITE (Lapys) -> https://learn.microsoft.com/en-us/windows/win32/controls/toolbar-controls-overview
                };

                RECT &bounds; HWND &handle; POINT &position;
                TBADDBITMAP bitmaps; // ->> Always `mutable`
                TBBUTTON    buttons[toolbar::buttons::count];
              } toolbar = {control.bounds, control.handle, control.position, {}, {}};

              // ...
              ZeroMemory(toolbar.buttons, sizeof toolbar.buttons); // ->> Zero-initialize reserved members

              toolbar.handle              = ::CreateWindowExW(0x00u, TOOLBARCLASSNAME, static_cast<LPCWSTR>(NULL), (CCS_NOPARENTALIGN | CCS_NORESIZE) /* ->> Automatic layout otherwise */ | TBSTYLE_WRAPABLE | WS_CHILD | WS_VISIBLE, position.x, position.y, window -> clientBounds.right, toolbar::buttons::height, window -> handle, reinterpret_cast<HMENU>(0) /* ->> Arbitrary integer ID */, window -> classInformation.hInstance, static_cast<LPVOID>(NULL));
              toolbar.bitmaps.hInst        = HINST_COMMCTRL;      // ->> System-defined button bitmaps; Otherwise specify `{static_cast<HINSTANCE>(NULL), reinterpret_cast<UINT_PTR>(static_cast<HBITMAP>(…))}`
              toolbar.bitmaps.nID          = IDB_STD_SMALL_COLOR; // ->> Image Data Bitmap Standard Small Colored
              toolbar.buttons[0].fsState   = TBSTATE_ENABLED;
              toolbar.buttons[0].fsStyle   = BTNS_AUTOSIZE | BTNS_BUTTON;          // --> TBSTYLE_AUTOSIZE | TBSTYLE_BUTTON
              toolbar.buttons[0].iBitmap   = I_IMAGENONE;                          // ->> Index of `IDB_STD_SMALL_COLOR`
              toolbar.buttons[0].idCommand = 420;                                  // ->> Arbitrary positive (given `WPARAM` representation)
              toolbar.buttons[0].iString   = reinterpret_cast<INT_PTR>(L"Window"); // ->> Alternatively the zero-based index of its button string
              toolbar.buttons[1].fsState   = TBSTATE_ENABLED;
              toolbar.buttons[1].fsStyle   = BTNS_AUTOSIZE | BTNS_BUTTON;
              toolbar.buttons[1].iBitmap   = MAKELONG(static_cast<WORD>(STD_HELP), 0x00u); // CITE (Lapys) -> https://learn.microsoft.com/en-us/windows/win32/controls/toolbar-standard-button-image-index-values
              toolbar.buttons[1].idCommand = 69;
              toolbar.buttons[1].iString   = reinterpret_cast<INT_PTR>(L"Help");

              if (NULL != toolbar.handle) {
                if (control::measure(window, &control)) {
                  position.x  = 0L;
                  position.y += toolbar.bounds.bottom - toolbar.bounds.top;
                }

                (void) ::SendMessageW(toolbar.handle, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON),                   0x00L);
                (void) ::SendMessageW(toolbar.handle, TB_ADDBITMAP,        0x00u /* --> toolbarBitmapCount */, reinterpret_cast<LPARAM>(&toolbar.bitmaps)); // ->> Index of first `toolbar.bitmaps` or `-1L`
                (void) ::SendMessageW(toolbar.handle, TB_ADDBUTTONS,       toolbar::buttons::count,            reinterpret_cast<LPARAM>(&toolbar.buttons)); // --> FALSE, TRUE

                if (0x00L != (::GetWindowLongPtrW(toolbar.handle, GWL_STYLE) & CCS_NORESIZE))
                  (void) ::SendMessageW(toolbar.handle, TB_SETBUTTONSIZE, 0x00u, MAKELPARAM(toolbar::buttons::width, toolbar::buttons::height)); // --> FALSE, TRUE ->> `TB_SETBITMAPSIZE` (16×15 pixels by default) and `TB_SETPADDING` also available

                (void) ::SendMessageW(toolbar.handle, TB_AUTOSIZE, 0x00u, 0x00L);
                (void) ::ShowWindow  (toolbar.handle, SW_NORMAL /* --> SW_SHOWNORMAL */);
              }
            } break;

            case TOOLTIP: /* ->> Shows wherever `control`s aren’t within the `window` */ {
              struct /* final */ {
                RECT &bounds; HWND &handle; POINT &position;
                TTTOOLINFOW information;
                WCHAR       text[21]; // ->> Used to return values pending `TTF_IDISHWND`; Maximum valid length between 80–4096–∞
              } tooltip = {control.bounds, control.handle, control.position, {}, L"Static \ntooltip text"};

              // ...
              tooltip.handle          = ::CreateWindowExW(WS_EX_TOPMOST /* | WS_EX_TOOLWINDOW */, TOOLTIPS_CLASS, static_cast<LPCWSTR>(NULL), WS_POPUP /* | TTS_ALWAYSTIP | TTS_BALLOON | TTS_NOPREFIX */, position.x, position.y, window -> clientBounds.right / 3, 24, window -> handle, static_cast<HMENU>(NULL), window -> classInformation.hInstance, static_cast<LPVOID>(NULL));
              window -> tooltipHandle = tooltip.handle;

              if (NULL != tooltip.handle) {
                tooltip.information.cbSize   = sizeof(TOOLINFOW);                    // ->> `TTTOOLINFOW_V3_SIZE` at time of development
                tooltip.information.hinst    = window -> classInformation.hInstance; // ->> Unused unless `.lpszText` specifies a string resource identifier
                tooltip.information.hwnd     = window -> handle;
                tooltip.information.lParam   = 420L;                // ->> Arbitrary
                tooltip.information.lpszText = LPSTR_TEXTCALLBACKW; // ->> `LPSTR_TEXTCALLBACKW` to handle `TTN_GETDISPINFOW | TTN_NEEDTEXTW` through its `::hwnd`’s window procedure, otherwise `tooltip.text`
                tooltip.information.rect     = window -> clientBounds;
                tooltip.information.uFlags   = TTF_IDISHWND | TTF_SUBCLASS /* | TTF_ABSOLUTE | TTF_MULTILINETIP | TTF_TRACK */ | TTF_TRANSPARENT; // ->> Intercepts mouse event messages unless `TTF_TRANSPARENT`
                tooltip.information.uId      = reinterpret_cast<UINT_PTR>(window -> handle);                                                      // ->> Alternatively `tooltip.handle`, or any arbitrary unique identifier if not `TTF_IDISHWND`

                // ...
                #ifdef TTTOOLINFOW_V2_SIZE // ->> `TTTOOLINFOW_V*_SIZE` (starting from 1) also exist
                  DLLVERSIONINFO commonControlsDLLVersionInformation = {};
                  HMODULE const  commonControlsModuleHandle          = ::LoadLibraryExW(L"comctl32" ".dll", static_cast<HANDLE>(NULL), 0x00000000u);
                  #if __GNUC__
                  # pragma GCC diagnostic push
                  # pragma GCC diagnostic ignored "-Wcast-function-type"
                  #endif
                    DLLGETVERSIONPROC const DllGetVersion = NULL != commonControlsModuleHandle ? reinterpret_cast<DLLGETVERSIONPROC>(::GetProcAddress(commonControlsModuleHandle, "DllGetVersion")) : NULL;
                  #if __GNUC__
                  # pragma GCC diagnostic pop
                  #endif

                  // ...
                  commonControlsDLLVersionInformation.cbSize = sizeof(DLLVERSIONINFO);

                  if (NULL != DllGetVersion and SUCCEEDED(DllGetVersion(&commonControlsDLLVersionInformation))) {
                    if (MAKELONG(commonControlsDLLVersionInformation.dwMinorVersion, commonControlsDLLVersionInformation.dwMajorVersion) < MAKELONG(0u, 6u)) // ->> Common Controls version 6.0
                    tooltip.information.cbSize = TTTOOLINFOW_V2_SIZE;
                  }

                  else if (::SendMessageW(tooltip.handle, CCM_GETVERSION, 0x00u, 0x00L) < 6u) // ->> Common Controls version 6.0; Wary of prior `CCM_SETVERSION` messages
                    tooltip.information.cbSize = TTTOOLINFOW_V2_SIZE;

                  // ...
                  if (NULL != commonControlsModuleHandle)
                  (void) ::FreeLibrary(commonControlsModuleHandle);
                #endif

                if (control::measure(window, &control)) {
                  position.x  = 0L;
                  position.y += tooltip.bounds.bottom - tooltip.bounds.top;
                }

                (void) ::SetWindowPos(tooltip.handle, HWND_TOPMOST, tooltip.position.x, tooltip.position.y, tooltip.bounds.right - tooltip.bounds.left, tooltip.bounds.bottom - tooltip.bounds.top, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE); // ->> Ignores the `.bounds` and `.position` due to `SWP_NOSIZE` and `SWP_NOMOVE` respectively

                if (FALSE != ::SendMessageW(tooltip.handle, TTM_ADDTOOLW, 0x00u, reinterpret_cast<LPARAM>(&tooltip.information))) {
                  RECT windowClientBounds = window -> clientBounds;

                  // ...
                  (void) ::SendMessageW(tooltip.handle, TTM_ACTIVATE, TRUE,  0x00L);
                  (void) ::SendMessageW(tooltip.handle, TTM_POPUP,    0x00u, 0x00L); // ->> Displays at the coordinates of the last mouse event message

                  if (0x00u != (tooltip.information.uFlags & (TTF_ABSOLUTE | TTF_TRACK)) and 0 != ::MapWindowPoints(window -> handle, static_cast<HWND>(NULL) /* --> HWND_DESKTOP */, reinterpret_cast<LPPOINT>(&windowClientBounds), 2u)) {
                    (void) ::SendMessageW(tooltip.handle, TTM_TRACKPOSITION, 0x00u, MAKELONG(windowClientBounds.left, windowClientBounds.top));
                    (void) ::SendMessageW(tooltip.handle, TTM_TRACKACTIVATE, TRUE,  reinterpret_cast<LPARAM>(&tooltip.information));
                  }
                }
              }
            } break;
          }

          if (NULL == control.handle)
          if (FALSE != ::DestroyWindow(window -> handle)) {
            window -> handle = NULL;
            break;
          }
        }

        // CreateWindowEx(0, WC_BUTTON, L"Btn", WS_CHILD | WS_VISIBLE,
        //     20 + (int)i * 80, y, 75, 23, hwnd, (HMENU)(IDC_BASE + i), hInst, NULL);
        // CreateWindowEx(0, WC_BUTTON, L"Check", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        //     120 + (int)i * 80, y, 75, 23, hwnd, (HMENU)(IDC_BASE + 100 + i), hInst, NULL);
        // CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
        //     220 + (int)i * 80, y, 75, 23, hwnd, (HMENU)(IDC_BASE + 200 + i), hInst, NULL);
        // CreateWindowEx(0, DATETIMEPICK_CLASS, NULL, WS_CHILD | WS_VISIBLE | DTS_SHORTDATEFORMAT,
        //     320 + (int)i * 80, y, 120, 23, hwnd, (HMENU)(IDC_BASE + 300 + i), hInst, NULL);
        // CreateWindowEx(0, WC_EDIT, L"Edit", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        //     460 + (int)i * 120, y, 120, 23, hwnd, (HMENU)(IDC_BASE + 400 + i), hInst, NULL);
        // CreateWindowEx(0, WC_HEADER, NULL, WS_CHILD | WS_VISIBLE | HDS_BUTTONS,
        //     600, y, 120, 23, hwnd, (HMENU)(IDC_BASE + 500 + i), hInst, NULL);
        // CreateWindowEx(0, WC_SCROLLBAR, NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
        //     20 + (int)i * 120, y + dy * 3, 100, 17, hwnd, (HMENU)(IDC_BASE + 600 + i), hInst, NULL);
        // CreateWindowEx(0, UPDOWN_CLASS, NULL, WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT,
        //     140 + (int)i * 120, y + dy * 3, 17, 23, hwnd, (HMENU)(IDC_BASE + 700 + i), hInst, NULL);
        // CreateWindowEx(0, WC_STATIC, L"Text", WS_CHILD | WS_VISIBLE,
        //     180 + (int)i * 120, y + dy * 3, 80, 23, hwnd, (HMENU)(IDC_BASE + 800 + i), hInst, NULL);
        // (GetDpiForWindow(…) * width) / 96 // where `96` is the base logical DPI Windows uses since 1980 for 100% scaling
      }
    #endif

    // ... ->> Event message loop
    if (NULL != window -> handle) {
      (void) ::ShowWindow  (window -> handle, SW_SHOW | (windowAppearance & SW_SHOW));
      (void) ::UpdateWindow(window -> handle);

      while (::GetMessageW(&threadMessage, static_cast<HWND>(NULL), 0x0u, 0x0u) > FALSE) {
        (void) ::TranslateMessage(&threadMessage); // --> FALSE, …
        (void) ::DispatchMessageW(&threadMessage); // --> window -> procedure(…)
      }
    } else threadMessage.wParam = EXIT_FAILURE;

    (void) ::UnregisterClassW(window -> classInformation.lpszClassName, window -> classInformation.hInstance);
    return threadMessage.wParam;
  }

  return EXIT_FAILURE;
}
