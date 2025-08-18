// --> del messages.exe && cls && g++ -pedantic-errors -std=c++98 -Wall -Werror -Wextra messages.cpp -lcomctl32 -o messages.exe && messages.exe & del messages.exe
#include <cstdio>
#
#include <cstddef> // --> NULL; std::size_t
#include <cstdlib> // --> EXIT_FAILURE, EXIT_SUCCESS
#include <new>     // --> delete, new; std::nothrow
#
#if defined _MSC_BUILD or defined _MSC_FULL_VER or defined _MSC_VER
#  pragma comment(lib, "comctl32" ".lib")
#  pragma comment(lib, "kernel32" ".lib")
#  pragma comment(lib, "richedit" ".lib")
#  pragma comment(lib, "user32"   ".lib")
#endif
#define _CRT_SECURE_NO_WARNINGS // ->> C Run-Time security                    --> `https://learn.microsoft.com/en-us/cpp/c-runtime-library/security-features-in-the-crt`
#define CINTERFACE              // ->> C Interface for Component Object Model --> `https://learn.microsoft.com/en-us/windows/win32/com/the-component-object-model`
#define STRICT                  // ->> Strict type checking                   --> `https://learn.microsoft.com/en-us/windows/win32/winprog/enabling-strict`
#define UNICODE                 // ->> Unicode preference                     --> `https://learn.microsoft.com/en-us/windows/win32/learnwin32/working-with-strings`
#ifndef _WIN32_WINNT            //
#  define _WIN32_WINNT 0x0600 // --> _WIN32_WINNT_WINXP+
#endif
#include <windows.h>    // --> CS_HREDRAW, CS_VREDRAW, CW_USEDEFAULT, FALSE, IDC_ARROW, GWL_STYLE, LOWORD(…), MAKELONG(…), MAKELPARAM(…), MAX_PATH, SW_SHOW, WS_CHILD, WS_OVERLAPPEDWINDOW, WS_VISIBLE, ZeroMemory(…), …; HBRUSH, HICON, HINSTANCE, HMODULE, HWND, INT_PTR, LPARAM, LPCWSTR, LPSTR, MSG, POINT, RECT, UINT, WCHAR, WINAPI, WNDCLASSEXW, WORD, WPARAM; ::CreateWindowExW(…), ::DefWindowProcW(…), ::DestroyWindow(…), ::DispatchMessageW(…), ::ExtractIconW(…), ::GetClientRect(…), ::GetCurrentProcess(…), ::GetMessageW(…), ::GetModuleFileNameW(…), ::GetSysColorBrush(…), ::GetWindowLongPtrW(…), ::LoadCursor(…), ::PostQuitMessage(…), ::RegisterClassExW(…), ::SendMessage(…), ::ShowWindow(…), ::TranslateMessage(…), ::UpdateWindow(…), ::UnregisterClassW(…)
#  include <commctrl.h> // --> BTNS_BUTTON, CCS_NOPARENTALIGN, CCS_NORESIZE, HINST_COMMCTRL, I_IMAGENONE, ICC_BAR_CLASSES, ICC_DATE_CLASSES, ICC_USEREX_CLASSES, IDB_STD_SMALL_COLOR, STD_HELP, TB_BUTTONSTRUCTSIZE, TB_ADDBITMAP, TBSTATE_ENABLED, TBSTYLE_WRAPABLE, TOOLBARCLASSNAME; INITCOMMONCONTROLSEX, TBADDBITMAP, TBBUTTON; ::InitCommonControlsEx(…)

/* Main */
int WINAPI WinMain(HINSTANCE const instanceHandle, HINSTANCE const, LPSTR const, int const windowAppearance) {
  static struct windowbase *window;
  struct windowbase /* final */ {
    struct info /* final */ {
      WNDCLASSEXW          classInformation;
      INITCOMMONCONTROLSEX commonControlsInitializer;
      HWND                 handle;
    };

    struct children /* final */ {
      struct windowbase::info *information;
      std::size_t              count;
    };

    /* ... */
    struct /* final */ {
      enum /* : std::size_t */ { count = 15u };
      static struct windowbase::children windowbase:: *const (&enumerate())[count + 1u] /* ->> Null-terminated pointer-to-member access array of all collections */ { static struct windowbase::children windowbase:: *const enumeration[] = {&windowbase::buttons, &windowbase::checkboxes, &windowbase::content, &windowbase::datetimes, &windowbase::dropdowns, &windowbase::edits, &windowbase::headers, &windowbase::progressbars, &windowbase::richedits, &windowbase::scrollbars, &windowbase::spinners, &windowbase::statusbars, &windowbase::tabControls, &windowbase::toolbars, &windowbase::tooltips, NULL}; return enumeration; }
    }                                                collections;
    struct windowbase::info                          information;
    struct windowbase::children windowbase:: *const *layout; // ->> Null-terminated ordering of `windowbase::children`
    struct windowbase::children                      buttons, checkboxes, content, datetimes, dropdowns, edits, headers, progressbars, richedits, scrollbars, spinners, statusbars, tabControls, toolbars, tooltips;

    /* ... */
    static LRESULT CALLBACK procedure(HWND windowHandle, UINT message, WPARAM parameter, LPARAM subparameter) {
      switch (message) {
        case WM_COMMAND: {
          for (std::size_t index = window -> toolbars.count; index--; )
          if (window -> toolbars.information[index].handle == reinterpret_cast<HWND>(subparameter)) /* ->> `subparameter` otherwise null when originated from an accelerator or menu */ {
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
  }                                               w                        = {{}, {{}, {sizeof(INITCOMMONCONTROLSEX), ICC_BAR_CLASSES | ICC_DATE_CLASSES | ICC_USEREX_CLASSES}, static_cast<HWND>(NULL)}, NULL, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}, {NULL, 0u}};
  struct windowbase::children windowbase:: *const wl[]                     = {&windowbase::statusbars, &windowbase::toolbars, NULL};
  MSG                                             threadMessage            = {};
  WCHAR                                           moduleFileName[MAX_PATH] = L"\0"; // ->> See use of `::ExtractIconW(…)`

  // ...
  window           = &w;
  window -> layout = wl;

  if (FALSE != ::InitCommonControlsEx(&window -> information.commonControlsInitializer)) {
    window -> information.classInformation.cbClsExtra    = 0;
    window -> information.classInformation.cbSize        = sizeof(WNDCLASSEX);
    window -> information.classInformation.cbWndExtra    = 0;
    window -> information.classInformation.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); // --> ::GetSysColorBrush(COLOR_WINDOW);
    window -> information.classInformation.hCursor       = ::LoadCursor  (static_cast<HINSTANCE>(NULL), IDC_ARROW); // --> static_cast<HCURSOR>(::LoadImage(NULL, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED));
    window -> information.classInformation.hIcon         = 0x00u != ::GetModuleFileNameW(static_cast<HMODULE>(NULL), moduleFileName, MAX_PATH) ? ::ExtractIconW(static_cast<HINSTANCE>(::GetCurrentProcess()), moduleFileName, 0u) : static_cast<HICON>(NULL);
    window -> information.classInformation.hIconSm       = static_cast<HICON>(NULL);
    window -> information.classInformation.hInstance     = instanceHandle;
    window -> information.classInformation.lpfnWndProc   = &window -> procedure;
    window -> information.classInformation.lpszClassName = L"window";
    window -> information.classInformation.lpszMenuName  = static_cast<LPCWSTR>(NULL);
    window -> information.classInformation.style         = CS_HREDRAW | CS_VREDRAW;
    threadMessage.hwnd                                   = NULL; // --> window -> information.handle
    threadMessage.lParam                                 = 0x00L;
    threadMessage.message                                = 0x00u;
    threadMessage.pt                                     = POINT();
    threadMessage.pt.x                                   = 0L;
    threadMessage.pt.y                                   = 0L;
    threadMessage.time                                   = 0u;
    threadMessage.wParam                                 = EXIT_SUCCESS;

    if (0x0000u != ::RegisterClassExW(&window -> information.classInformation)) {
      RECT windowChildBounds = {0L, 0L, 0L, 0L};
      union {
        std::size_t              windowChildCount;
        struct windowbase::info *windowChildren;
      };

      // ...
      window -> information.handle = ::CreateWindowExW(0x00u, window -> information.classInformation.lpszClassName, L"Windows Event Messages", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 480, 320, static_cast<HWND>(NULL), static_cast<HMENU>(NULL), window -> information.classInformation.hInstance, static_cast<LPVOID>(NULL));
      windowChildren               = NULL;

      if (NULL != window -> information.handle) {
        struct windowbase::children windowbase:: *const (&windowCollections)     [window -> collections.count + 1u] = window -> collections.enumerate();
        std::size_t                                       windowCollectionsCounts[window -> collections.count + 0u];

        // ... --> window.[buttons].count = windowChildCount = windowCollectionsCounts = …
        windowChildCount = 0u;

        for (struct windowbase::children windowbase:: *const *layout = window -> layout; NULL != *layout; ++layout) {
          ++(window ->* *layout).count;
          ++windowChildCount;

          for (std::size_t index = 0u; ; ++index)
          if (*layout == windowCollections[index]) {
            windowCollectionsCounts[index] = (window ->* *layout).count;
            break;
          }
        }

        // ...
        windowChildren = 0u != windowChildCount ? new (std::nothrow) struct windowbase::info[windowChildCount] : NULL;

        if (NULL != windowChildren) {
          for (struct /* final */ {
            struct windowbase::info                         *children;
            struct windowbase::children windowbase:: *const *collection;
          } iterator = {windowChildren, windowCollections}; NULL != *iterator.collection; ) {
            (window ->* *iterator.collection).information = iterator.children; // --> window -> [buttons].information = windowChildren[a..b]

            iterator.children += (window ->* *iterator.collection).count;
            iterator.collection++;
          }

          for (struct windowbase::children windowbase:: *const *layout = window -> layout; NULL != *layout; ++layout) {
            struct windowbase::info *information;
            std::size_t              windowCollectionsIndex = 0u;

            // ...
            while (*layout != windowCollections[windowCollectionsIndex])
              ++windowCollectionsIndex;

            information = (window ->* *layout).information + --windowCollectionsCounts[windowCollectionsIndex];

            // ...
            if (false) {}
            else if (*layout == &windowbase::buttons) {}
            else if (*layout == &windowbase::checkboxes) {}
            else if (*layout == &windowbase::content) {}
            else if (*layout == &windowbase::datetimes) {}
            else if (*layout == &windowbase::dropdowns) {}
            else if (*layout == &windowbase::edits) {}
            else if (*layout == &windowbase::headers) {}
            else if (*layout == &windowbase::progressbars) {}
            else if (*layout == &windowbase::richedits) {}
            else if (*layout == &windowbase::scrollbars) {}
            else if (*layout == &windowbase::spinners) {}
            else if (*layout == &windowbase::statusbars) {
              HWND hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL,
                  WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
                  0, 0, 0, 0, hwnd, (HMENU)(IDC_BASE + 9100), hInst, NULL);
              int parts[] = {150, -1};
              SendMessage(hStatus, SB_SETPARTS, 2, (LPARAM)parts);
              SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)L"Ready");
            }

            else if (*layout == &windowbase::tabControls) {}
            else if (*layout == &windowbase::toolbars) {
              RECT         windowClientBounds                 = {0L, 0L, 480L, 320L};
              HWND         toolbar                            = NULL;
              TBADDBITMAP  toolbarAddedBitmaps                = {};
              RECT         toolbarBounds                      = {};
              WPARAM const toolbarButtonCount                 = 2u;
              TBBUTTON     toolbarButtons[toolbarButtonCount] = {};
              WORD const   toolbarButtonHeight                = 42u; // ->> 24×22 pixels by default --- CITE (Lapys) -> https://learn.microsoft.com/en-us/windows/win32/controls/toolbar-controls-overview
              WORD const   toolbarButtonWidth                 = 42u;

              // ...
              ZeroMemory(toolbarButtons, sizeof toolbarButtons);

              (void) ::GetClientRect(window -> information.handle, &windowClientBounds);

              information -> handle       = ::CreateWindowExW(0x00u, TOOLBARCLASSNAME, static_cast<LPCWSTR>(NULL), (CCS_NOPARENTALIGN | CCS_NORESIZE) /* ->> Automatic layout otherwise */ | TBSTYLE_WRAPABLE | WS_CHILD | WS_VISIBLE, windowChildBounds.left, windowChildBounds.top, windowClientBounds.right, toolbarButtonHeight, window -> information.handle, reinterpret_cast<HMENU>(0) /* ->> Arbitrary integer identifier */, window -> information.classInformation.hInstance, static_cast<LPVOID>(NULL));
              toolbar                     = information -> handle;
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

              if (NULL != toolbar) {
                (void) ::GetWindowRect(toolbar, &toolbarBounds);
                (void) ::SendMessage  (toolbar, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON),   0x00L);
                (void) ::SendMessage  (toolbar, TB_ADDBITMAP,        0x00u,              reinterpret_cast<LPARAM>(&toolbarAddedBitmaps));
                (void) ::SendMessage  (toolbar, TB_ADDBUTTONS,       toolbarButtonCount, reinterpret_cast<LPARAM>(&toolbarButtons));

                if (0x00L != (::GetWindowLongPtrW(toolbar, GWL_STYLE) & CCS_NORESIZE))
                  (void) ::SendMessage(toolbar, TB_SETBUTTONSIZE, 0x00u, MAKELPARAM(toolbarButtonWidth, toolbarButtonHeight)); // ->> `TB_SETPADDING` and 16×15-default `TB_SETBITMAPSIZE` also available

                (void) ::SendMessage(toolbar, TB_AUTOSIZE, 0x00u, 0x00L);
                (void) ::ShowWindow (toolbar, SW_NORMAL /* --> SW_SHOWNORMAL */);

                windowChildBounds.bottom += toolbarBounds.bottom - toolbarBounds.top;
                windowChildBounds.left    = 0L;
                windowChildBounds.right   = toolbarBounds.right  - toolbarBounds.left;
                windowChildBounds.top    += toolbarBounds.bottom - toolbarBounds.top;
              }
            }

            else if (*layout == &windowbase::tooltips) {}

            // ... ->> `windowbase::children` not created successfully
            if (NULL == information -> handle) {
              ++windowCollectionsCounts[windowCollectionsIndex];

              if (FALSE != ::DestroyWindow(window -> information.handle)) {
                window -> information.handle = NULL;
                break;
              }
            }
          }

          // HWND hTooltip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
          //     WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
          //     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
          //     hwnd, NULL, hInst, NULL);
          // SetWindowPos(hTooltip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
          // TOOLINFO ti = { sizeof(TOOLINFO), TTF_SUBCLASS, hwnd, 0, NULL, L"Tooltip text" };
          // GetClientRect(hwnd, &ti.rect);
          // SendMessage(hTooltip, TTM_ADDTOOL, 0, (LPARAM)&ti);

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
      }

      // ...
      if (NULL != window -> information.handle) {
        (void) ::ShowWindow  (window -> information.handle, SW_SHOW | (windowAppearance & SW_SHOW));
        (void) ::UpdateWindow(window -> information.handle);

        while (::GetMessageW(&threadMessage, static_cast<HWND>(NULL), 0x0u, 0x0u) > FALSE) {
          (void) ::TranslateMessage(&threadMessage);
          (void) ::DispatchMessageW(&threadMessage); // --> window.procedure(…)
        }
      } else threadMessage.wParam = EXIT_FAILURE;

      (void) ::UnregisterClassW(window -> information.classInformation.lpszClassName, window -> information.classInformation.hInstance);
      delete[] windowChildren;

      // ...
      return threadMessage.wParam;
    }
  }

  return EXIT_FAILURE;
}
