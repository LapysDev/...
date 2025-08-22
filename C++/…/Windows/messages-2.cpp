// --> del messages.exe && cls && g++ -pedantic-errors -std=c++98 -Wall -Werror -Wextra messages.cpp -lcomctl32 -o messages.exe && messages.exe & del messages.exe
#include <ciso646> // --> and, or
#include <cstddef> // --> NULL
#include <cstdlib> // --> EXIT_FAILURE, EXIT_SUCCESS
#include <cstdio>  // --> std::printf(…)
#
#if defined _MSC_BUILD or defined _MSC_FULL_VER or defined _MSC_VER
# pragma comment(lib, "comctl32" ".lib")
# pragma comment(lib, "kernel32" ".lib")
# pragma comment(lib, "richedit" ".lib")
# pragma comment(lib, "user32"   ".lib")
#endif
#define _CRT_SECURE_NO_WARNINGS // ->> C Run-Time security                    --> `https://learn.microsoft.com/en-us/cpp/c-runtime-library/security-features-in-the-crt`
#define CINTERFACE              // ->> C Interface for Component Object Model --> `https://learn.microsoft.com/en-us/windows/win32/com/the-component-object-model`
#define STRICT                  // ->> Strict type checking                   --> `https://learn.microsoft.com/en-us/windows/win32/winprog/enabling-strict`
#define UNICODE                 // ->> Unicode preference                     --> `https://learn.microsoft.com/en-us/windows/win32/learnwin32/working-with-strings`
#ifndef _WIN32_WINNT            //
# define _WIN32_WINNT 0x0600 // --> _WIN32_WINNT_WINXP+
#endif
#include <windows.h> // --> CS_HREDRAW, CS_VREDRAW, CW_USEDEFAULT, FALSE, GW_CHILD, GW_HWNDNEXT, HWND_TOPMOST, IDC_ARROW, GWL_STYLE, LOWORD(…), MAKELONG(…), MAKELPARAM(…), MAX_PATH, SW_SHOW, WS_CHILD, WS_OVERLAPPEDWINDOW, WS_VISIBLE, ZeroMemory(…), …; HANDLE, HBRUSH, HICON, HINSTANCE, HMODULE, HWND, INT_PTR, LPARAM, LPCWSTR, LPPOINT, LPSTR, MSG, POINT, RECT, UINT, UINT_PTR, WCHAR, WINAPI, WNDCLASSEXW, WORD, WPARAM; ::CreateWindowExW(…), ::DefWindowProcW(…), ::DestroyWindow(…), ::DispatchMessageW(…), ::ExtractIconW(…), ::FreeLibrary(…), ::GetClientRect(…), ::GetCurrentProcess(…), ::GetMessageW(…), ::GetModuleFileNameW(…), ::GetSysColorBrush(…), ::GetWindow(…), ::GetWindowLongPtrW(…), ::LoadCursor(…), ::LoadLibraryExW(…), ::MapWindowPoints(…), ::PostQuitMessage(…), ::RegisterClassExW(…), ::SendMessageW(…), ::ScreenToClient(…), ::ShowWindow(…), ::TranslateMessage(…), ::UpdateWindow(…), ::UnregisterClassW(…)
# include <commctrl.h> // --> BTNS_BUTTON, CCM_GETVERSION, CCS_NOPARENTALIGN, CCS_NORESIZE, HINST_COMMCTRL, I_IMAGENONE, ICC_BAR_CLASSES, ICC_DATE_CLASSES, ICC_USEREX_CLASSES, ICC_WIN95_CLASSES, IDB_STD_SMALL_COLOR, SB_SETPARTS, SB_SETTEXT, SBT_NOBORDERS, SBT_NOTABPARSING, STATUSCLASSNAME, STD_HELP, TB_BUTTONSTRUCTSIZE, TB_ADDBITMAP, TBSTATE_ENABLED, TBSTYLE_WRAPABLE, TOOLBARCLASSNAME, TOOLINFOW, TTM_ADDTOOLW; INITCOMMONCONTROLSEX, TBADDBITMAP, TBBUTTON; ::InitCommonControlsEx(…)
# include <shlwapi.h>  // --> DLLGETVERSIONPROC, DLLVERSIONINFO
# include <winerror.h> // --> SUCCEEDED(…)

/* Main */
int WINAPI WinMain(HINSTANCE const instanceHandle, HINSTANCE const, LPSTR const, int const windowAppearance) {
  struct /* final */ {
    HWND                 handle;
    INITCOMMONCONTROLSEX commonControlsInitializer;
    RECT                 clientBounds;
    WNDCLASSEXW          classInformation;

    /* ... */
    static LRESULT CALLBACK procedure(HWND windowHandle, UINT message, WPARAM parameter, LPARAM subparameter) /* noexcept */ {
      switch (message) {
        case WM_COMMAND: {
          // ->> Prefer `::EnumChildWindows(…)` for robust handling instead
          for (HWND windowChildHandle = ::GetWindow(windowHandle, GW_CHILD); NULL != windowChildHandle; windowChildHandle = ::GetWindow(windowChildHandle, GW_HWNDNEXT))
          if (windowChildHandle == reinterpret_cast<HWND>(subparameter)) /* ->> `subparameter` otherwise null when originated from an accelerator or menu */ {
            (void) std::printf("Command recieved: %i" "\r\n", static_cast<int>(LOWORD(parameter)));
            break;
          }
        } break;
      }

      // ...
      switch (message) {
        case WM_CLOSE: case WM_DESTROY:              ::PostQuitMessage(EXIT_SUCCESS); break;
        case WM_KEYDOWN: if (VK_ESCAPE == parameter) ::PostQuitMessage(EXIT_SUCCESS); break;
        case WM_QUIT:                         (void) ::DestroyWindow  (windowHandle); break;
      }

      return ::DefWindowProcW(windowHandle, message, parameter, subparameter);
    }
  }     window                   = {};
  MSG   threadMessage            = {};
  POINT position                 = {0L, 0L};
  WCHAR moduleFileName[MAX_PATH] = L"\0"; // ->> See use of `::ExtractIconW(…)` (or `window.classInformation.hIcon`)

  // ...
  threadMessage.hwnd                      = NULL; // --> window.handle
  threadMessage.lParam                    = 0x00L;
  threadMessage.message                   = 0x00u;
  threadMessage.pt.x                      = 0L;
  threadMessage.pt.y                      = 0L;
  threadMessage.time                      = 0u;
  threadMessage.wParam                    = EXIT_SUCCESS;
  window.classInformation.cbClsExtra      = 0;
  window.classInformation.cbSize          = sizeof(WNDCLASSEX);
  window.classInformation.cbWndExtra      = 0;
  window.classInformation.hbrBackground   = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); // --> ::GetSysColorBrush(COLOR_WINDOW);
  window.classInformation.hCursor         = ::LoadCursor(static_cast<HINSTANCE>(NULL), IDC_ARROW); // --> static_cast<HCURSOR>(::LoadImage(NULL, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED));
  window.classInformation.hIcon           = 0x00u != ::GetModuleFileNameW(static_cast<HMODULE>(NULL), moduleFileName, MAX_PATH) ? ::ExtractIconW(static_cast<HINSTANCE>(::GetCurrentProcess()), moduleFileName, 0u) : static_cast<HICON>(NULL);
  window.classInformation.hIconSm         = static_cast<HICON>(NULL);
  window.classInformation.hInstance       = instanceHandle;
  window.classInformation.lpfnWndProc     = &window.procedure;
  window.classInformation.lpszClassName   = L"window";
  window.classInformation.lpszMenuName    = static_cast<LPCWSTR>(NULL);
  window.classInformation.style           = CS_HREDRAW | CS_VREDRAW;
  window.clientBounds.left                = 0L;
  window.clientBounds.top                 = 0L;
  window.commonControlsInitializer.dwICC  = ICC_BAR_CLASSES | ICC_DATE_CLASSES | ICC_USEREX_CLASSES | ICC_WIN95_CLASSES;
  window.commonControlsInitializer.dwSize = sizeof(INITCOMMONCONTROLSEX);

  if (FALSE != ::InitCommonControlsEx(&window.commonControlsInitializer) and 0x0000u != ::RegisterClassExW(&window.classInformation)) {
    window.handle = ::CreateWindowExW(0x00u, window.classInformation.lpszClassName, L"Windows Event Messages", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 480, 320, static_cast<HWND>(NULL), static_cast<HMENU>(NULL), window.classInformation.hInstance, static_cast<LPVOID>(NULL));

    // ... ->> Add child windows to interpret more event messages
    #if true
      if (NULL != window.handle) {
        if (FALSE == ::GetClientRect(window.handle, &window.clientBounds)) {
          window.clientBounds.bottom = 320L; // ->> Client Height
          window.clientBounds.right  = 480L; // ->> Client Width
        }

        for (enum /* : unsigned char */ {
          BUTTON,
          CHECKBOX, CONTENT,
          DATETIME, DROPDOWN,
          EDIT,
          HEADER,
          PROGRESS,
          RICHEDIT,
          SCROLLBAR, SPINNER, STATUSBAR,
          TABCONTROL, TOOLBAR, TOOLTIP
        } const components[] = {STATUSBAR, TOOLBAR, TOOLTIP}, *component = components + (sizeof components / sizeof *components); component-- != components; ) {
          struct subwindow /* final */ {
            HWND handle;
            union { RECT bounds; POINT position; };

            /* ... */
            static bool measure(void* const window, struct subwindow* const subwindow) /* noexcept */ {
              if (FALSE != ::GetWindowRect(subwindow -> handle, &subwindow -> bounds)) {
                POINT      subwindowClientBoundsMaximum = {subwindow -> bounds.right, subwindow -> bounds.bottom};
                HWND const windowHandle                 = *static_cast<HWND*>(window);

                // ...
                if (FALSE != ::ScreenToClient(windowHandle, &subwindowClientBoundsMaximum) and FALSE != ::ScreenToClient(windowHandle, &subwindow -> position)) {
                  subwindow -> bounds.bottom = subwindowClientBoundsMaximum.y;
                  subwindow -> bounds.right  = subwindowClientBoundsMaximum.x;

                  return true;
                }
              }

              return false;
            }
          } subwindow = {NULL, {0L, 0L, 0L, 0L}};

          // ...
          switch (*component) {
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
              struct subwindow &statusbar        = subwindow;
              int const         statusbarParts[] = {50, 150, -1}; // ->> Right bound of each slice of the bar

              // ...
              statusbar.handle = ::CreateWindowExW(0x00u, STATUSCLASSNAME, static_cast<LPCWSTR>(NULL), (CCS_NOPARENTALIGN | CCS_NORESIZE) /* ->> Automatic layout otherwise */ | SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE, position.x, position.y, window.clientBounds.right, 69, window.handle, reinterpret_cast<HMENU>(0) /* ->> Arbitrary integer identifier */, window.classInformation.hInstance, static_cast<LPVOID>(NULL));

              if (NULL != statusbar.handle) {
                if (subwindow::measure(&window, &statusbar)) {
                  (void) ::MoveWindow(statusbar.handle, statusbar.position.x, statusbar.position.y, statusbar.bounds.right - statusbar.bounds.left, statusbar.bounds.bottom - statusbar.bounds.top, TRUE);

                  position.x  = 0L;
                  position.y += statusbar.bounds.bottom - statusbar.bounds.top;
                }

                if (FALSE != ::SendMessageW(statusbar.handle, SB_SETPARTS, 3u /* ->> `<= 256u` */, reinterpret_cast<LPARAM>(statusbarParts))) {
                  (void) ::SendMessageW(statusbar.handle, SB_SETTEXT, MAKEWPARAM(0u, SBT_NOBORDERS | SBT_NOTABPARSING /* | … */), reinterpret_cast<LPARAM>(L"Hello")); // --> FALSE, TRUE ->> See `SBT_OWNERDRAW` too
                  (void) ::SendMessageW(statusbar.handle, SB_SETTEXT, MAKEWPARAM(1u, 0x00u),                                      reinterpret_cast<LPARAM>(L"World!"));
                }
              }
            } break;

            case TABCONTROL: break;

            case TOOLBAR: {
              WORD const        toolbarButtonWidth                 = 42u; // ->> 24×22 pixels by default --- CITE (Lapys) -> https://learn.microsoft.com/en-us/windows/win32/controls/toolbar-controls-overview
              WPARAM const      toolbarButtonCount                 = 2u;
              TBBUTTON          toolbarButtons[toolbarButtonCount] = {};
              WORD const        toolbarButtonHeight                = 42u;
              TBADDBITMAP       toolbarAddedBitmaps                = {}; // ->> Always `mutable`
              struct subwindow &toolbar                            = subwindow;

              // ...
              ZeroMemory(toolbarButtons, sizeof toolbarButtons); // ->> Zero-initialize reserved members

              toolbar.handle              = ::CreateWindowExW(0x00u, TOOLBARCLASSNAME, static_cast<LPCWSTR>(NULL), (CCS_NOPARENTALIGN | CCS_NORESIZE) /* ->> Automatic layout otherwise */ | TBSTYLE_WRAPABLE | WS_CHILD | WS_VISIBLE, position.x, position.y, window.clientBounds.right, toolbarButtonHeight, window.handle, reinterpret_cast<HMENU>(0) /* ->> Arbitrary integer identifier */, window.classInformation.hInstance, static_cast<LPVOID>(NULL));
              toolbarAddedBitmaps.hInst   = HINST_COMMCTRL;      // ->> System-defined button bitmaps; Otherwise specify `{static_cast<HINSTANCE>(NULL), reinterpret_cast<UINT_PTR>(static_cast<HBITMAP>(…))}`
              toolbarAddedBitmaps.nID     = IDB_STD_SMALL_COLOR; // ->> Image Data Bitmap Standard Small Colored
              toolbarButtons[0].fsState   = TBSTATE_ENABLED;
              toolbarButtons[0].fsStyle   = BTNS_AUTOSIZE | BTNS_BUTTON;          // --> TBSTYLE_AUTOSIZE | TBSTYLE_BUTTON
              toolbarButtons[0].iBitmap   = I_IMAGENONE;                          // ->> Index of `IDB_STD_SMALL_COLOR`
              toolbarButtons[0].idCommand = 42;                                   // ->> Arbitrary positive (given `WPARAM` representation)
              toolbarButtons[0].iString   = reinterpret_cast<INT_PTR>(L"Window"); // ->> Alternatively the zero-based index of its button string
              toolbarButtons[1].fsState   = TBSTATE_ENABLED;
              toolbarButtons[1].fsStyle   = BTNS_AUTOSIZE | BTNS_BUTTON;
              toolbarButtons[1].iBitmap   = MAKELONG(static_cast<WORD>(STD_HELP), 0x00u); // CITE (Lapys) -> https://learn.microsoft.com/en-us/windows/win32/controls/toolbar-standard-button-image-index-values
              toolbarButtons[1].idCommand = 69;
              toolbarButtons[1].iString   = reinterpret_cast<INT_PTR>(L"Help");

              if (NULL != toolbar.handle) {
                if (subwindow::measure(&window, &toolbar)) {
                  position.x  = 0L;
                  position.y += toolbar.bounds.bottom - toolbar.bounds.top;
                }

                (void) ::SendMessageW(toolbar.handle, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON),                   0x00L);
                (void) ::SendMessageW(toolbar.handle, TB_ADDBITMAP,        0x00u /* --> toolbarBitmapCount */, reinterpret_cast<LPARAM>(&toolbarAddedBitmaps)); // ->> Index of first `toolbarAddedBitmaps` or `-1L`
                (void) ::SendMessageW(toolbar.handle, TB_ADDBUTTONS,       toolbarButtonCount,                 reinterpret_cast<LPARAM>(&toolbarButtons));      // --> FALSE, TRUE

                if (0x00L != (::GetWindowLongPtrW(toolbar.handle, GWL_STYLE) & CCS_NORESIZE))
                  (void) ::SendMessageW(toolbar.handle, TB_SETBUTTONSIZE, 0x00u, MAKELPARAM(toolbarButtonWidth, toolbarButtonHeight)); // --> FALSE, TRUE ->> `TB_SETBITMAPSIZE` (16×15 pixels by default) and `TB_SETPADDING` also available

                (void) ::SendMessageW(toolbar.handle, TB_AUTOSIZE, 0x00u, 0x00L);
                (void) ::ShowWindow (toolbar.handle, SW_NORMAL /* --> SW_SHOWNORMAL */);
              }
            } break;

            case TOOLTIP: {
              struct subwindow &tooltip            = subwindow;
              TTTOOLINFOW       tooltipInformation = {};
              WCHAR             tooltipText[]      = L"Lorem \nipsum"; // ->> Used to return values pending `TTF_IDISHWND`; Maximum valid length between 80–4096–∞

              // ...
              tooltip.handle = ::CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, static_cast<LPCWSTR>(NULL), /* TTS_BALLOON | */ TTS_ALWAYSTIP | TTS_NOPREFIX | WS_POPUP, position.x, position.y, window.clientBounds.right / 3, 24, window.handle, static_cast<HMENU>(NULL), window.classInformation.hInstance, static_cast<LPVOID>(NULL));

              if (NULL != tooltip.handle) {
                tooltipInformation.cbSize   = sizeof(TOOLINFOW);                 // ->> `TTTOOLINFOW_V3_SIZE` at time of development
                tooltipInformation.hinst    = window.classInformation.hInstance; // ->> Unused unless `.lpszText` specifies a string resource identifier
                tooltipInformation.hwnd     = window.handle;
                tooltipInformation.lParam   = 420L; // ->> Arbitrary
                tooltipInformation.lpszText = tooltipText;
                tooltipInformation.rect     = window.clientBounds;
                tooltipInformation.uFlags   = TTF_SUBCLASS /* | TTF_ABSOLUTE | TTF_MULTILINETIP | TTF_TRACK */ | TTF_TRANSPARENT; // ->> Intercepts mouse event messages unless `TTF_TRANSPARENT`; Also see `TTF_IDISHWND`
                tooltipInformation.uId      = reinterpret_cast<UINT_PTR>(window.handle);                                          // ->> Alternatively `tooltip.handle`, or any arbitrary unique identifier

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
                    tooltipInformation.cbSize = TTTOOLINFOW_V2_SIZE;
                  }

                  else if (::SendMessageW(tooltip.handle, CCM_GETVERSION, 0x00u, 0x00L) < 6u) // ->> Common Controls version 6.0; Wary of prior `CCM_SETVERSION` messages
                    tooltipInformation.cbSize = TTTOOLINFOW_V2_SIZE;

                  // ...
                  if (NULL != commonControlsModuleHandle)
                  (void) ::FreeLibrary(commonControlsModuleHandle);
                #endif

                if (subwindow::measure(&window, &tooltip)) {
                  position.x  = 0L;
                  position.y += tooltip.bounds.bottom - tooltip.bounds.top;
                }

                (void) ::SetWindowPos(tooltip.handle, HWND_TOPMOST, tooltip.position.x, tooltip.position.y, tooltip.bounds.right - tooltip.bounds.left, tooltip.bounds.bottom - tooltip.bounds.top, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE); // ->> Ignores the `.bounds` and `.position` due to `SWP_NOSIZE` and `SWP_NOMOVE` respectively

                if (FALSE != ::SendMessageW(tooltip.handle, TTM_ADDTOOLW, 0x00u, reinterpret_cast<LPARAM>(&tooltipInformation))) {
                  RECT windowClientBounds = window.clientBounds;

                  // ...
                  (void) ::SendMessageW(tooltip.handle, TTM_ACTIVATE, TRUE,  0x00L);
                  (void) ::SendMessageW(tooltip.handle, TTM_POPUP,    0x00u, 0x00L); // ->> Displays at the coordinates of the last mouse event message

                  if (0x00u != (tooltipInformation.uFlags & (TTF_ABSOLUTE | TTF_TRACK)) and 0 != ::MapWindowPoints(window.handle, static_cast<HWND>(NULL) /* --> HWND_DESKTOP */, reinterpret_cast<LPPOINT>(&windowClientBounds), 2u)) {
                    (void) ::SendMessageW(tooltip.handle, TTM_TRACKPOSITION, 0x00u, MAKELONG(windowClientBounds.left, windowClientBounds.top));
                    (void) ::SendMessageW(tooltip.handle, TTM_TRACKACTIVATE, TRUE,  reinterpret_cast<LPARAM>(&tooltipInformation));
                  }
                }
              }
            } break;
          }

          if (NULL == subwindow.handle)
          if (FALSE != ::DestroyWindow(window.handle)) {
            window.handle = NULL;
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
      }
    #endif

    // ... ->> Event message loop
    if (NULL != window.handle) {
      (void) ::ShowWindow  (window.handle, SW_SHOW | (windowAppearance & SW_SHOW));
      (void) ::UpdateWindow(window.handle);

      while (::GetMessageW(&threadMessage, static_cast<HWND>(NULL), 0x0u, 0x0u) > FALSE) {
        (void) ::TranslateMessage(&threadMessage); // --> FALSE, …
        (void) ::DispatchMessageW(&threadMessage); // --> window.procedure(…)
      }
    } else threadMessage.wParam = EXIT_FAILURE;

    (void) ::UnregisterClassW(window.classInformation.lpszClassName, window.classInformation.hInstance);
    return threadMessage.wParam;
  }

  return EXIT_FAILURE;
}
