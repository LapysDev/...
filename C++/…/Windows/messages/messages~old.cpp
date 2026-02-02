/* ... */
#define  UNICODE            // ->> Windows API
#define _UNICODE            //
#define _WIN32_WINNT 0x0501 // --> _WIN32_WINNT_WINXP

/* Import */
#include <ciso646>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cwchar>
#include <stdint.h>
#include <windows.h>
# include <commctrl.h>
# include <ddraw.h>
# include <richedit.h>
# include <shlobj.h>
# include <vfw.h>
# include <winuser.h>
# include <wow64apiset.h>
# ifdef _MSC_VER
#   include <inked.h>
# endif

/* Main */
int WinMain(HINSTANCE const instanceHandle, HINSTANCE const, LPSTR const, int const windowAppearance) {
  WCHAR moduleFileName[MAX_PATH] = L"\0";
  MSG   threadMessage            = {};
  struct /* final */ {
    WNDCLASSEXW classInformation;
    HWND        handle;

    /* ... */
    static LRESULT CALLBACK procedure(HWND const windowHandle, UINT const message, WPARAM const parameter, LPARAM const subparameter) /* noexcept */ {
      struct log /* final */ {
        /* ... ->> Used only for `log::format(…)` */
        struct number_layout {
          uintmax_t   value;
          bool        signedness : 1;
          std::size_t length     : 6; // ->> Count of digits within `number::value`; Arbitrary bit width
        };

        struct number : public number_layout {
          inline number(DWORD const value) /* noexcept */ { this -> length = sizeof value; this -> signedness = false;      this -> value = value; }
          inline number(INT   const value) /* noexcept */ { this -> length = sizeof value; this -> signedness = value < 0;  this -> value = value; }
          inline number(LONG  const value) /* noexcept */ { this -> length = sizeof value; this -> signedness = value < 0L; this -> value = value; }
          inline number(UINT  const value) /* noexcept */ { this -> length = sizeof value; this -> signedness = false;      this -> value = value; }
          inline number(WORD  const value) /* noexcept */ { this -> length = sizeof value; this -> signedness = false;      this -> value = value; }
          #ifdef _WIN64
            inline number(LPARAM const value) /* noexcept */ { this -> length = sizeof value; this -> signedness = value < 0L; this -> value = value; }
            inline number(WPARAM const value) /* noexcept */ { this -> length = sizeof value; this -> signedness = false;      this -> value = value; }
          #endif
        };
          struct special   : public number { explicit inline special  (number const& value) /* noexcept */ : number::number(value) {} };
          struct undefined : public number { explicit inline undefined(number const& value) /* noexcept */ : number::number(value) {} };
          struct unused    : public number { explicit inline unused   (number const& value) /* noexcept */ : number::number(value) {} };

        struct any /* final */ {
          union {
            unsigned char : 1;                                                           //
            void const          *const address;                                          // ->> Reference or character sequence
            bool                 const binary;                                           //
            char                       character[(MB_LEN_MAX | sizeof(uintmax_t)) + 1u]; // ->> `char8_t[]` sequence
            struct number_layout const number;                                           // --> struct number | special | undefined | unused
          };

          enum /* : unsigned char */ {
            addressable,
            boolean,
            functional,
            nullable,
            numerable,
            specialized,
            subtextual,
            textual_default,
            textual_locale,
            undefinable,
            unusable
          } const type : 4;

          /* ... */
          inline any()                                      /* noexcept */ :                                              type(any::nullable)        {}
          inline any(void const volatile* const  address)   /* noexcept */ : address  (const_cast<void const*>(address)), type(any::addressable)     {}
          inline any(bool                 const  boolean)   /* noexcept */ : binary   (boolean),                          type(any::boolean)         {}
          inline any(log::number          const& number)    /* noexcept */ : number   (number),                           type(any::numerable)       {}
          inline any(log::special         const& special)   /* noexcept */ : number   (special),                          type(any::specialized)     {}
          inline any(char                 const  text)      /* noexcept */ : character(""),                               type(any::subtextual)      { this -> character[0] = text; this -> character[1] = '\0'; }
          inline any(char                 const  text[])    /* noexcept */ : address  (text),                             type(any::textual_default) {}
          inline any(wchar_t              const  text)      /* noexcept */ : character(""),                               type(any::subtextual)      { int const length = std::wctomb(this -> character, text); this -> character[length != -1 ? length : 0] = '\0'; }
          inline any(wchar_t              const  text[])    /* noexcept */ : address  (text),                             type(any::textual_locale)  {}
          inline any(log::undefined       const& undefined) /* noexcept */ : number   (undefined),                        type(any::undefinable)     {}
          inline any(log::unused          const& unused)    /* noexcept */ : number   (unused),                           type(any::unusable)        {}
        };

        /* ... */
        inline static bool compare(char const stringA[], char const stringB[]) /* noexcept */ {
          return log::compare(stringA, stringB, sizeof(char));
        }

        inline static bool compare(char const* stringA, wchar_t const* stringB) /* noexcept */ {
          char const    *endA  = stringA;
          std::mbstate_t state = std::mbstate_t();

          // ...
          if (static_cast<void const*>(stringA) == static_cast<void const*>(stringB))
          return true;

          if (NULL == stringA or NULL == stringB)
          return false;

          while ('\0' != *endA)
            ++endA;

          while (true) {
            wchar_t           characterA = L'\0';
            wchar_t     const characterB = *stringB;
            std::size_t const count      = std::mbrtowc(&characterA, stringA, endA - stringA, &state);

            // ...
            if (characterA != characterB or count == static_cast<std::size_t>(-1) or count == static_cast<std::size_t>(-2))
            return false;

            if (0u == count or L'\0' == characterA or L'\0' == characterB)
            break;

            stringA += count;
            stringB += 1;
          }

          return true;
        }

        inline static bool compare(wchar_t const stringA[], wchar_t const stringB[]) /* noexcept */ {
          return log::compare(stringA, stringB, sizeof(wchar_t));
        }

        inline static bool compare(wchar_t const stringA[], char const stringB[]) /* noexcept */ {
          return log::compare(stringB, stringA);
        }

        inline static bool compare(void const* const stringA, void const* const stringB, std::size_t const size) /* noexcept */ {
          if (stringA == stringB)
          return true;

          if (NULL == stringA or NULL == stringB)
          return false;

          for (unsigned char const *iteratorA = static_cast<unsigned char const*>(stringA), *iteratorB = static_cast<unsigned char const*>(stringB); ; ) {
            uintmax_t characterA = 0x00u;
            uintmax_t characterB = 0x00u;

            // ...
            for (std::size_t count = size; count--; ) {
              characterA = (characterA << CHAR_BIT * sizeof(unsigned char)) | *(iteratorA++);
              characterB = (characterB << CHAR_BIT * sizeof(unsigned char)) | *(iteratorB++);
            }

            if (characterA != characterB)
            return false;

            if (0x00u == characterA or 0x00u == characterB)
            return true;
          }
        }

        // ... ->> Generate formatted text (maximum of 127 messages per `format(…)`)
        inline static char const* format(any const message1 = any(), any const message2 = any(), any const message3 = any(), any const message4 = any(), any const message5 = any(), any const message6 = any(), any const message7 = any(), any const message8 = any(), any const message9 = any(), any const message10 = any(), any const message11 = any(), any const message12 = any(), any const message13 = any(), any const message14 = any(), any const message15 = any(), any const message16 = any(), any const message17 = any(), any const message18 = any(), any const message19 = any(), any const message20 = any(), any const message21 = any(), any const message22 = any(), any const message23 = any(), any const message24 = any(), any const message25 = any(), any const message26 = any(), any const message27 = any(), any const message28 = any(), any const message29 = any(), any const message30 = any(), any const message31 = any(), any const message32 = any(), any const message33 = any(), any const message34 = any(), any const message35 = any(), any const message36 = any(), any const message37 = any(), any const message38 = any(), any const message39 = any(), any const message40 = any(), any const message41 = any(), any const message42 = any(), any const message43 = any(), any const message44 = any(), any const message45 = any(), any const message46 = any(), any const message47 = any(), any const message48 = any(), any const message49 = any(), any const message50 = any(), any const message51 = any(), any const message52 = any(), any const message53 = any(), any const message54 = any(), any const message55 = any(), any const message56 = any(), any const message57 = any(), any const message58 = any(), any const message59 = any(), any const message60 = any(), any const message61 = any(), any const message62 = any(), any const message63 = any(), any const message64 = any(), any const message65 = any(), any const message66 = any(), any const message67 = any(), any const message68 = any(), any const message69 = any(), any const message70 = any(), any const message71 = any(), any const message72 = any(), any const message73 = any(), any const message74 = any(), any const message75 = any(), any const message76 = any(), any const message77 = any(), any const message78 = any(), any const message79 = any(), any const message80 = any(), any const message81 = any(), any const message82 = any(), any const message83 = any(), any const message84 = any(), any const message85 = any(), any const message86 = any(), any const message87 = any(), any const message88 = any(), any const message89 = any(), any const message90 = any(), any const message91 = any(), any const message92 = any(), any const message93 = any(), any const message94 = any(), any const message95 = any(), any const message96 = any(), any const message97 = any(), any const message98 = any(), any const message99 = any(), any const message100 = any(), any const message101 = any(), any const message102 = any(), any const message103 = any(), any const message104 = any(), any const message105 = any(), any const message106 = any(), any const message107 = any(), any const message108 = any(), any const message109 = any(), any const message110 = any(), any const message111 = any(), any const message112 = any(), any const message113 = any(), any const message114 = any(), any const message115 = any(), any const message116 = any(), any const message117 = any(), any const message118 = any(), any const message119 = any(), any const message120 = any(), any const message121 = any(), any const message122 = any(), any const message123 = any(), any const message124 = any(), any const message125 = any(), any const message126 = any(), any const message127 = any()) /* noexcept */ {
          static struct /* final */ {
            std::size_t length;
            char        value[4096]; // --> char8_t

            inline bool apply(any const& message) /* noexcept */ {
              switch (message.type) {
                case any::addressable: {
                  if (NULL == message.address) return this -> apply("NULL");

                  if (not this -> apply('@'))                                                                  return false;
                  if (not this -> apply(static_cast<undefined>(reinterpret_cast<uintptr_t>(message.address)))) return false;
                } break;

                case any::boolean: {
                  if (not this -> apply(message.binary ? "true" : "false"))
                  return false;
                } break;

                case any::functional: {
                  std::size_t length = 0u; // --> log16(UCHAR_MAX)

                  // ...
                  if (NULL == message.address) return this -> apply("NULL");
                  if (not this -> apply("0x")) return false;

                  for (std::size_t characteristics = UCHAR_MAX; characteristics; characteristics /= 16u)
                    ++length;

                  for (unsigned char const *iterator = static_cast<unsigned char const*>(message.address), *const end = static_cast<unsigned char const*>(message.address) + sizeof(void (*)(...)); end != iterator; ++iterator) {
                    if (length + this -> length > sizeof this -> value / sizeof(char))
                    return false;

                    for (std::size_t byte = *iterator, sublength = length; sublength--; byte /= 16u)
                    this -> value[this -> length++] = "0123456789ABCDEF"[byte % 16u];
                  }
                } break;

                case any::nullable: break;

                case any::numerable:
                case any::specialized:
                case any::undefinable:
                case any::unusable: {
                  uintmax_t         value           = message.number.signedness ? -static_cast<intmax_t>(message.number.value) : message.number.value;
                  std::size_t const radix           = message.type == any::numerable ? 10u : 16u;
                  std::size_t       length          = 0u; // --> logN(characteristics, radix)
                  uintmax_t         characteristics = message.type == any::undefinable ? (((static_cast<std::size_t>(1u) << ((CHAR_BIT * message.number.length) - 1u)) - 1u) << 1u) + 1u : value ? value : 1u;

                  // ...
                  if (not this -> apply(message.number.signedness ? "-" : ""))                               return false;
                  if (not this -> apply(radix == 2u ? "0b" : radix == 8u ? "0o" : radix == 16u ? "0x" : "")) return false;
                  if (not this -> apply(message.type == any::unusable ? "..." : ""))                         return false;

                  for (; characteristics; characteristics /= radix) {
                    if (length++ + this -> length == sizeof this -> value / sizeof(char))
                    return false;
                  }

                  for (char *iterator = this -> value + (length + this -> length); length--; ++(this -> length), value /= radix)
                  *--iterator = "0123456789ABCDEF"[value % radix];
                } break;

                case any::subtextual: {
                  if (not this -> apply(message.character))
                  return false;
                } break;

                case any::textual_default: {
                  for (char *destination = this -> value + this -> length, *source = const_cast<char*>(static_cast<char const*>(message.address)); '\0' != *source; ++destination, ++source) {
                    if (destination == this -> value + ((sizeof this -> value / sizeof(char)) - 1u))
                    return false;

                    *destination = *source;
                    ++(this -> length);
                  }
                } break;

                case any::textual_locale: {
                  wchar_t const    *text   = static_cast<wchar_t const*>(message.address);
                  std::mbstate_t    state  = std::mbstate_t();
                  std::size_t const length = std::wcsrtombs(NULL, &text, 0u, &state);

                  // ...
                  if (length + this -> length > sizeof this -> value / sizeof(char))
                  return false;

                  if (std::wcsrtombs(this -> value + this -> length, &text, length, &state) == static_cast<std::size_t>(-1))
                  return false;

                  this -> length += length;
                } break;
              }

              return true;
            }
          } translation = {0u, {'\0'}};

          // ...
          translation.length = 0u;

          for (any const *const messages[] = {&message1, &message2, &message3, &message4, &message5, &message6, &message7, &message8, &message9, &message10, &message11, &message12, &message13, &message14, &message15, &message16, &message17, &message18, &message19, &message20, &message21, &message22, &message23, &message24, &message25, &message26, &message27, &message28, &message29, &message30, &message31, &message32, &message33, &message34, &message35, &message36, &message37, &message38, &message39, &message40, &message41, &message42, &message43, &message44, &message45, &message46, &message47, &message48, &message49, &message50, &message51, &message52, &message53, &message54, &message55, &message56, &message57, &message58, &message59, &message60, &message61, &message62, &message63, &message64, &message65, &message66, &message67, &message68, &message69, &message70, &message71, &message72, &message73, &message74, &message75, &message76, &message77, &message78, &message79, &message80, &message81, &message82, &message83, &message84, &message85, &message86, &message87, &message88, &message89, &message90, &message91, &message92, &message93, &message94, &message95, &message96, &message97, &message98, &message99, &message100, &message101, &message102, &message103, &message104, &message105, &message106, &message107, &message108, &message109, &message110, &message111, &message112, &message113, &message114, &message115, &message116, &message117, &message118, &message119, &message120, &message121, &message122, &message123, &message124, &message125, &message126, &message127}, *const *message = messages; message != messages + (sizeof(messages) / sizeof(any const*)); ++message) {
            if (not translation.apply(**message))
            return NULL;
          }

          if (translation.length == sizeof translation.value / sizeof(char))
          return NULL;

          translation.value[translation.length] = '\0';

          return translation.value;
        }

        // ... ->> Print NUL-terminated text
        inline static bool output(char const text[]) /* noexcept */ {
          return NULL != text ? EOF != std::fputs(text, stdout) : false;
        }

        // ... ->> Generate `message` text; `NULL` if `message` unrecognized, otherwise `""` if `message` untranslatable, otherwise `"NMSP_CONSTANT" "\0" "Namespace" "\0" "Event Action"`
        inline static char const* translate(UINT const message) /* noexcept */ {
          static struct /* final */ {
            std::size_t length;
            char        value[4096];

            inline bool apply(char const* messageConstant, char const* messageType, char const* messageEvent) /* noexcept */ {
              for (char const *const messages[] = {messageConstant, messageType, messageEvent}, *const *message = messages; message != messages + (sizeof messages / sizeof(char const*)); ++message) {
                for (char *destination = this -> value + this -> length, *source = const_cast<char*>(*message); '\0' != *source; ++destination, ++source, ++(this -> length)) {
                  if (this -> length == sizeof this -> value / sizeof(char))
                  break;

                  *destination = *source;
                }

                if (this -> length == sizeof this -> value / sizeof(char))
                break;

                this -> value[this -> length++] = '\0';
              }

              if (this -> length == sizeof this -> value / sizeof(char))
              return false;

              this -> value[this -> length] = '\0';

              return true;
            }
          } translation = {0u, {'\0'}};

          /* ...
            --- CITE ---
              #Lapys:
              - https://docs.microsoft.com/en-us/windows/win32/winmsg/about-messages-and-message-queues#system-defined-messages
              - https://github.com/LapysDev/Windows-API-Events
              - https://wiki.winehq.org/List_Of_Windows_Messages
          */
          translation.length = 0u;

          #ifdef ACM_OPENA
            if (not (ACM_OPENA == message ? translation.apply("ACM_OPENA", "Animation Control", "Open") : true))
            return "";
          #endif

          #ifdef ACM_OPENW
            if (not (ACM_OPENW == message ? translation.apply("ACM_OPENW", "Animation Control", "Open") : true))
            return "";
          #endif

          #ifdef ACM_PLAY
            if (not (ACM_PLAY == message ? translation.apply("ACM_PLAY", "Animation Control", "Play") : true))
            return "";
          #endif

          #ifdef ACM_STOP
            if (not (ACM_STOP == message ? translation.apply("ACM_STOP", "Animation Control", "Stop") : true))
            return "";
          #endif

          #ifdef BFFM_ENABLEOK
            if (not (BFFM_ENABLEOK == message ? translation.apply("BFFM_ENABLEOK", "Browse for Folder", "Enable OK") : true))
            return "";
          #endif

          #ifdef BFFM_SETSELECTIONA
            if (not (BFFM_SETSELECTIONA == message ? translation.apply("BFFM_SETSELECTIONA", "Browse for Folder", "Set Selection") : true))
            return "";
          #endif

          #ifdef BFFM_SETSELECTIONW
            if (not (BFFM_SETSELECTIONW == message ? translation.apply("BFFM_SETSELECTIONW", "Browse for Folder", "Set Selection") : true))
            return "";
          #endif

          #ifdef BFFM_SETSTATUSTEXTA
            if (not (BFFM_SETSTATUSTEXTA == message ? translation.apply("BFFM_SETSTATUSTEXTA", "Browse for Folder", "Set Status Text") : true))
            return "";
          #endif

          #ifdef BFFM_SETSTATUSTEXTW
            if (not (BFFM_SETSTATUSTEXTW == message ? translation.apply("BFFM_SETSTATUSTEXTW", "Browse for Folder", "Set Status Text") : true))
            return "";
          #endif

          #ifdef BM_CLICK
            if (not (BM_CLICK == message ? translation.apply("BM_CLICK", "Button", "Click") : true))
            return "";
          #endif

          #ifdef BM_GETCHECK
            if (not (BM_GETCHECK == message ? translation.apply("BM_GETCHECK", "Button", "Get Check") : true))
            return "";
          #endif

          #ifdef BM_GETIMAGE
            if (not (BM_GETIMAGE == message ? translation.apply("BM_GETIMAGE", "Button", "Get Image") : true))
            return "";
          #endif

          #ifdef BM_GETSTATE
            if (not (BM_GETSTATE == message ? translation.apply("BM_GETSTATE", "Button", "Get State") : true))
            return "";
          #endif

          #ifdef BM_SETCHECK
            if (not (BM_SETCHECK == message ? translation.apply("BM_SETCHECK", "Button", "Set Check") : true))
            return "";
          #endif

          #ifdef BM_SETDONTCLICK
            if (not (BM_SETDONTCLICK == message ? translation.apply("BM_SETDONTCLICK", "Button", "Set Don't Click") : true))
            return "";
          #endif

          #ifdef BM_SETIMAGE
            if (not (BM_SETIMAGE == message ? translation.apply("BM_SETIMAGE", "Button", "Set Image") : true))
            return "";
          #endif

          #ifdef BM_SETSTATE
            if (not (BM_SETSTATE == message ? translation.apply("BM_SETSTATE", "Button", "Set State") : true))
            return "";
          #endif

          #ifdef BM_SETSTYLE
            if (not (BM_SETSTYLE == message ? translation.apply("BM_SETSTYLE", "Button", "Set Style") : true))
            return "";
          #endif

          #ifdef CBEM_GETCOMBOCONTROL
            if (not (CBEM_GETCOMBOCONTROL == message ? translation.apply("CBEM_GETCOMBOCONTROL", "Combination Box", "Get Combo. Control") : true))
            return "";
          #endif

          #ifdef CBEM_GETEDITCONTROL
            if (not (CBEM_GETEDITCONTROL == message ? translation.apply("CBEM_GETEDITCONTROL", "Combination Box", "Get Edit Control") : true))
            return "";
          #endif

          #ifdef CBEM_GETEXSTYLE
            if (not (CBEM_GETEXSTYLE == message ? translation.apply("CBEM_GETEXSTYLE", "Combination Box", "Get Extended Style") : true))
            return "";
          #endif

          #ifdef CBEM_GETEXTENDEDSTYLE
            if (not (CBEM_GETEXTENDEDSTYLE == message ? translation.apply("CBEM_GETEXTENDEDSTYLE", "Combination Box", "Get Extended Style") : true))
            return "";
          #endif

          #ifdef CBEM_GETIMAGELIST
            if (not (CBEM_GETIMAGELIST == message ? translation.apply("CBEM_GETIMAGELIST", "Combination Box", "Get Image List") : true))
            return "";
          #endif

          #ifdef CBEM_GETITEMA
            if (not (CBEM_GETITEMA == message ? translation.apply("CBEM_GETITEMA", "Combination Box", "Get Item") : true))
            return "";
          #endif

          #ifdef CBEM_GETITEMW
            if (not (CBEM_GETITEMW == message ? translation.apply("CBEM_GETITEMW", "Combination Box", "Get Item") : true))
            return "";
          #endif

          #ifdef CBEM_HASEDITCHANGED
            if (not (CBEM_HASEDITCHANGED == message ? translation.apply("CBEM_HASEDITCHANGED", "Combination Box", "Has Edit Changed") : true))
            return "";
          #endif

          #ifdef CBEM_INSERTITEMA
            if (not (CBEM_INSERTITEMA == message ? translation.apply("CBEM_INSERTITEMA", "Combination Box", "Insert Item") : true))
            return "";
          #endif

          #ifdef CBEM_INSERTITEMW
            if (not (CBEM_INSERTITEMW == message ? translation.apply("CBEM_INSERTITEMW", "Combination Box", "Insert Item") : true))
            return "";
          #endif

          #ifdef CBEM_SETEXTENDEDSTYLE
            if (not (CBEM_SETEXTENDEDSTYLE == message ? translation.apply("CBEM_SETEXTENDEDSTYLE", "Combination Box", "Set Extended Style") : true))
            return "";
          #endif

          #ifdef CBEM_SETIMAGELIST
            if (not (CBEM_SETIMAGELIST == message ? translation.apply("CBEM_SETIMAGELIST", "Combination Box", "Set Image List") : true))
            return "";
          #endif

          #ifdef CBEM_SETITEMA
            if (not (CBEM_SETITEMA == message ? translation.apply("CBEM_SETITEMA", "Combination Box", "Set Item") : true))
            return "";
          #endif

          #ifdef CBEM_SETITEMW
            if (not (CBEM_SETITEMW == message ? translation.apply("CBEM_SETITEMW", "Combination Box", "Set Item") : true))
            return "";
          #endif

          #ifdef CDM_FIRST
            if (not (CDM_FIRST == message ? translation.apply("CDM_FIRST", "Common Dialog", "1st") : true))
            return "";
          #endif

          #ifdef CDM_GETFILEPATH
            if (not (CDM_GETFILEPATH == message ? translation.apply("CDM_GETFILEPATH", "Common Dialog", "Get File Path") : true))
            return "";
          #endif

          #ifdef CDM_GETFOLDERIDLIST
            if (not (CDM_GETFOLDERIDLIST == message ? translation.apply("CDM_GETFOLDERIDLIST", "Common Dialog", "Get Folder ID List") : true))
            return "";
          #endif

          #ifdef CDM_GETFOLDERPATH
            if (not (CDM_GETFOLDERPATH == message ? translation.apply("CDM_GETFOLDERPATH", "Common Dialog", "Get Folder Path") : true))
            return "";
          #endif

          #ifdef CDM_GETSPEC
            if (not (CDM_GETSPEC == message ? translation.apply("CDM_GETSPEC", "Common Dialog", "Get Specifications") : true))
            return "";
          #endif

          #ifdef CDM_HIDECONTROL
            if (not (CDM_HIDECONTROL == message ? translation.apply("CDM_HIDECONTROL", "Common Dialog", "Hide Control") : true))
            return "";
          #endif

          #ifdef CDM_LAST
            if (not (CDM_LAST == message ? translation.apply("CDM_LAST", "Common Dialog", "nth") : true))
            return "";
          #endif

          #ifdef CDM_SETCONTROLTEXT
            if (not (CDM_SETCONTROLTEXT == message ? translation.apply("CDM_SETCONTROLTEXT", "Common Dialog", "Set Control Text") : true))
            return "";
          #endif

          #ifdef CDM_SETDEFEXT
            if (not (CDM_SETDEFEXT == message ? translation.apply("CDM_SETDEFEXT", "Common Dialog", "Set Default Text") : true))
            return "";
          #endif

          #ifdef DDM_BEGIN
            if (not (DDM_BEGIN == message ? translation.apply("DDM_BEGIN", "Direct Draw", "Begin") : true))
            return "";
          #endif

          #ifdef DDM_CLOSE
            if (not (DDM_CLOSE == message ? translation.apply("DDM_CLOSE", "Direct Draw", "Close") : true))
            return "";
          #endif

          #ifdef DDM_DRAW
            if (not (DDM_DRAW == message ? translation.apply("DDM_DRAW", "Direct Draw", "Draw") : true))
            return "";
          #endif

          #ifdef DDM_END
            if (not (DDM_END == message ? translation.apply("DDM_END", "Direct Draw", "End") : true))
            return "";
          #endif

          #ifdef DDM_SETFMT
            if (not (DDM_SETFMT == message ? translation.apply("DDM_SETFMT", "Direct Draw", "Set Format") : true))
            return "";
          #endif

          #ifdef DL_BEGINDRAG
            if (not (DL_BEGINDRAG == message ? translation.apply("DL_BEGINDRAG", "Drag List Box", "Begin Drag") : true))
            return "";
          #endif

          #ifdef DL_CANCELDRAG
            if (not (DL_CANCELDRAG == message ? translation.apply("DL_CANCELDRAG", "Drag List Box", "Cancel Drag") : true))
            return "";
          #endif

          #ifdef DL_DRAGGING
            if (not (DL_DRAGGING == message ? translation.apply("DL_DRAGGING", "Drag List Box", "Dragging") : true))
            return "";
          #endif

          #ifdef DL_DROPPED
            if (not (DL_DROPPED == message ? translation.apply("DL_DROPPED", "Drag List Box", "Dropped") : true))
            return "";
          #endif

          #ifdef DM_GETDEFID
            if (not (DM_GETDEFID == message ? translation.apply("DM_GETDEFID", "Dialog", "Get Default Identity") : true))
            return "";
          #endif

          #ifdef DM_REPOSITION
            if (not (DM_REPOSITION == message ? translation.apply("DM_REPOSITION", "Dialog", "Reposition") : true))
            return "";
          #endif

          #ifdef DM_SETDEFID
            if (not (DM_SETDEFID == message ? translation.apply("DM_SETDEFID", "Dialog", "Set Default Identity") : true))
            return "";
          #endif

          #ifdef EM_AUTOURLDETECT
            if (not (EM_AUTOURLDETECT == message ? translation.apply("EM_AUTOURLDETECT", "Rich Edit", "Automatic URL (Uniform Resource Locator) Detect") : true))
            return "";
          #endif

          #ifdef EM_CANPASTE
            if (not (EM_CANPASTE == message ? translation.apply("EM_CANPASTE", "Rich Edit", "Can Paste") : true))
            return "";
          #endif

          #ifdef EM_CANREDO
            if (not (EM_CANREDO == message ? translation.apply("EM_CANREDO", "Rich Edit", "Can Redo") : true))
            return "";
          #endif

          #ifdef EM_CANUNDO
            if (not (EM_CANUNDO == message ? translation.apply("EM_CANUNDO", "Rich Edit", "Can Undo") : true))
            return "";
          #endif

          #ifdef EM_CHARFROMPOS
            if (not (EM_CHARFROMPOS == message ? translation.apply("EM_CHARFROMPOS", "Rich Edit", "Character from Position") : true))
            return "";
          #endif

          #ifdef EM_CONVPOSITION
            if (not (EM_CONVPOSITION == message ? translation.apply("EM_CONVPOSITION", "Rich Edit", "Conv... Position") : true))
            return "";
          #endif

          #ifdef EM_DISPLAYBAND
            if (not (EM_DISPLAYBAND == message ? translation.apply("EM_DISPLAYBAND", "Rich Edit", "Display Band") : true))
            return "";
          #endif

          #ifdef EM_EMPTYUNDOBUFFER
            if (not (EM_EMPTYUNDOBUFFER == message ? translation.apply("EM_EMPTYUNDOBUFFER", "Rich Edit", "Empty Undo Buffer") : true))
            return "";
          #endif

          #ifdef EM_EXGETSEL
            if (not (EM_EXGETSEL == message ? translation.apply("EM_EXGETSEL", "Rich Edit", "Extend Get Selection") : true))
            return "";
          #endif

          #ifdef EM_EXLIMITTEXT
            if (not (EM_EXLIMITTEXT == message ? translation.apply("EM_EXLIMITTEXT", "Rich Edit", "Extend Limit Text") : true))
            return "";
          #endif

          #ifdef EM_EXLINEFROMCHAR
            if (not (EM_EXLINEFROMCHAR == message ? translation.apply("EM_EXLINEFROMCHAR", "Rich Edit", "Extend Line From Character") : true))
            return "";
          #endif

          #ifdef EM_EXSETSEL
            if (not (EM_EXSETSEL == message ? translation.apply("EM_EXSETSEL", "Rich Edit", "Extend Set Selection") : true))
            return "";
          #endif

          #ifdef EM_FINDTEXT
            if (not (EM_FINDTEXT == message ? translation.apply("EM_FINDTEXT", "Rich Edit", "Find Text") : true))
            return "";
          #endif

          #ifdef EM_FINDTEXTEX
            if (not (EM_FINDTEXTEX == message ? translation.apply("EM_FINDTEXTEX", "Rich Edit", "Find Text (Extended)") : true))
            return "";
          #endif

          #ifdef EM_FINDTEXTEXW
            if (not (EM_FINDTEXTEXW == message ? translation.apply("EM_FINDTEXTEXW", "Rich Edit", "Find Text (Extended)") : true))
            return "";
          #endif

          #ifdef EM_FINDTEXTW
            if (not (EM_FINDTEXTW == message ? translation.apply("EM_FINDTEXTW", "Rich Edit", "Find Text") : true))
            return "";
          #endif

          #ifdef EM_FINDWORDBREAK
            if (not (EM_FINDWORDBREAK == message ? translation.apply("EM_FINDWORDBREAK", "Rich Edit", "Find Word Break") : true))
            return "";
          #endif

          #ifdef EM_FMTLINES
            if (not (EM_FMTLINES == message ? translation.apply("EM_FMTLINES", "Rich Edit", "Format Lines") : true))
            return "";
          #endif

          #ifdef EM_FORMATRANGE
            if (not (EM_FORMATRANGE == message ? translation.apply("EM_FORMATRANGE", "Rich Edit", "Format Range") : true))
            return "";
          #endif

          #ifdef EM_GETAUTOURLDETECT
            if (not (EM_GETAUTOURLDETECT == message ? translation.apply("EM_GETAUTOURLDETECT", "Rich Edit", "Get Automatic URL (Uniform Resource Locator) Detect") : true))
            return "";
          #endif

          #ifdef EM_GETBIDIOPTIONS
            if (not (EM_GETBIDIOPTIONS == message ? translation.apply("EM_GETBIDIOPTIONS", "Rich Edit", "Get Bi-Directional Options") : true))
            return "";
          #endif

          #ifdef EM_GETCHARFORMAT
            if (not (EM_GETCHARFORMAT == message ? translation.apply("EM_GETCHARFORMAT", "Rich Edit", "Get Character Format") : true))
            return "";
          #endif

          #ifdef EM_GETCTFMODEBIAS
            if (not (EM_GETCTFMODEBIAS == message ? translation.apply("EM_GETCTFMODEBIAS", "Rich Edit", "Get CTF Mode Bias") : true))
            return "";
          #endif

          #ifdef EM_GETCTFOPENSTATUS
            if (not (EM_GETCTFOPENSTATUS == message ? translation.apply("EM_GETCTFOPENSTATUS", "Rich Edit", "Get CTF Open Status") : true))
            return "";
          #endif

          #ifdef EM_GETEDITSTYLE
            if (not (EM_GETEDITSTYLE == message ? translation.apply("EM_GETEDITSTYLE", "Rich Edit", "Get Edit Style") : true))
            return "";
          #endif

          #ifdef EM_GETEVENTMASK
            if (not (EM_GETEVENTMASK == message ? translation.apply("EM_GETEVENTMASK", "Rich Edit", "Get Event Mask") : true))
            return "";
          #endif

          #ifdef EM_GETFIRSTVISIBLELINE
            if (not (EM_GETFIRSTVISIBLELINE == message ? translation.apply("EM_GETFIRSTVISIBLELINE", "Rich Edit", "Get First Visible Line") : true))
            return "";
          #endif

          #ifdef EM_GETHANDLE
            if (not (EM_GETHANDLE == message ? translation.apply("EM_GETHANDLE", "Rich Edit", "Get Handle") : true))
            return "";
          #endif

          #ifdef EM_GETHYPHENATEINFO
            if (not (EM_GETHYPHENATEINFO == message ? translation.apply("EM_GETHYPHENATEINFO", "Rich Edit", "Get Hyphenate Information") : true))
            return "";
          #endif

          #ifdef EM_GETIMECOLOR
            if (not (EM_GETIMECOLOR == message ? translation.apply("EM_GETIMECOLOR", "Rich Edit", "Get IME (Input Method Encoding) Color") : true))
            return "";
          #endif

          #ifdef EM_GETIMECOMPMODE
            if (not (EM_GETIMECOMPMODE == message ? translation.apply("EM_GETIMECOMPMODE", "Rich Edit", "Get IME (Input Method Encoding) Composition Mode") : true))
            return "";
          #endif

          #ifdef EM_GETIMECOMPTEXT
            if (not (EM_GETIMECOMPTEXT == message ? translation.apply("EM_GETIMECOMPTEXT", "Rich Edit", "Get IME (Input Method Encoding) Composition Text") : true))
            return "";
          #endif

          #ifdef EM_GETIMEMODEBIAS
            if (not (EM_GETIMEMODEBIAS == message ? translation.apply("EM_GETIMEMODEBIAS", "Rich Edit", "Get IME (Input Method Encoding) Mode Bias") : true))
            return "";
          #endif

          #ifdef EM_GETIMEOPTIONS
            if (not (EM_GETIMEOPTIONS == message ? translation.apply("EM_GETIMEOPTIONS", "Rich Edit", "Get IME (Input Method Encoding) Options") : true))
            return "";
          #endif

          #ifdef EM_GETIMEPROPERTY
            if (not (EM_GETIMECOMPTEXT == message ? translation.apply("EM_GETIMEPROPERTY", "Rich Edit", "Get IME (Input Method Encoding) Property") : true))
            return "";
          #endif

          #ifdef EM_GETIMESTATUS
            if (not (EM_GETIMESTATUS == message ? translation.apply("EM_GETIMESTATUS", "Rich Edit", "Get IME (Input Method Encoding) Status") : true))
            return "";
          #endif

          #ifdef EM_GETLANGOPTIONS
            if (not (EM_GETLANGOPTIONS == message ? translation.apply("EM_GETLANGOPTIONS", "Rich Edit", "Get Language Options") : true))
            return "";
          #endif

          #ifdef EM_GETLIMITTEXT
            if (not (EM_GETLIMITTEXT == message ? translation.apply("EM_GETLIMITTEXT", "Rich Edit", "Get Limit Text") : true))
            return "";
          #endif

          #ifdef EM_GETLINE
            if (not (EM_GETLINE == message ? translation.apply("EM_GETLINE", "Rich Edit", "Get Line") : true))
            return "";
          #endif

          #ifdef EM_GETLINECOUNT
            if (not (EM_GETLINECOUNT == message ? translation.apply("EM_GETLINECOUNT", "Rich Edit", "Get Line Count") : true))
            return "";
          #endif

          #ifdef EM_GETMARGINS
            if (not (EM_GETMARGINS == message ? translation.apply("EM_GETMARGINS", "Rich Edit", "Get Margins") : true))
            return "";
          #endif

          #ifdef EM_GETMODIFY
            if (not (EM_GETMODIFY == message ? translation.apply("EM_GETMODIFY", "Rich Edit", "Get Modify") : true))
            return "";
          #endif

          #ifdef EM_GETOLEINTERFACE
            if (not (EM_GETOLEINTERFACE == message ? translation.apply("EM_GETOLEINTERFACE", "Rich Edit", "Get OLE (Object Linking & Embedding) Interface") : true))
            return "";
          #endif

          #ifdef EM_GETOPTIONS
            if (not (EM_GETOPTIONS == message ? translation.apply("EM_GETOPTIONS", "Rich Edit", "Get Options") : true))
            return "";
          #endif

          #ifdef EM_GETPAGE
            if (not (EM_GETPAGE == message ? translation.apply("EM_GETPAGE", "Rich Edit", "Get Page") : true))
            return "";
          #endif

          #ifdef EM_GETPAGEROTATE
            if (not (EM_GETPAGEROTATE == message ? translation.apply("EM_GETPAGEROTATE", "Rich Edit", "Get Page Rotate") : true))
            return "";
          #endif

          #ifdef EM_GETPARAFORMAT
            if (not (EM_GETPARAFORMAT == message ? translation.apply("EM_GETPARAFORMAT", "Rich Edit", "Get Paragraph Format") : true))
            return "";
          #endif

          #ifdef EM_GETPASSWORDCHAR
            if (not (EM_GETPASSWORDCHAR == message ? translation.apply("EM_GETPASSWORDCHAR", "Rich Edit", "Get Password Character") : true))
            return "";
          #endif

          #ifdef EM_GETPUNCTUATION
            if (not (EM_GETPUNCTUATION == message ? translation.apply("EM_GETPUNCTUATION", "Rich Edit", "Get Punctuation") : true))
            return "";
          #endif

          #ifdef EM_GETRECT
            if (not (EM_GETRECT == message ? translation.apply("EM_GETRECT", "Rich Edit", "Get Rectangle") : true))
            return "";
          #endif

          #ifdef EM_GETREDONAME
            if (not (EM_GETREDONAME == message ? translation.apply("EM_GETREDONAME", "Rich Edit", "Get Redo Name") : true))
            return "";
          #endif

          #ifdef EM_GETSCROLLPOS
            if (not (EM_GETSCROLLPOS == message ? translation.apply("EM_GETSCROLLPOS", "Rich Edit", "Get Scroll Position") : true))
            return "";
          #endif

          #ifdef EM_GETSEL
            if (not (EM_GETSEL == message ? translation.apply("EM_GETSEL", "Rich Edit", "Get Selection") : true))
            return "";
          #endif

          #ifdef EM_GETSELTEXT
            if (not (EM_GETSELTEXT == message ? translation.apply("EM_GETSELTEXT", "Rich Edit", "Get Selection Text") : true))
            return "";
          #endif

          #ifdef EM_GETTEXTEX
            if (not (EM_GETTEXTEX == message ? translation.apply("EM_GETTEXTEX", "Rich Edit", "Get Text (Extended)") : true))
            return "";
          #endif

          #ifdef EM_GETTEXTLENGTHEX
            if (not (EM_GETTEXTLENGTHEX == message ? translation.apply("EM_GETTEXTLENGTHEX", "Rich Edit", "Get Text Length (Extended)") : true))
            return "";
          #endif

          #ifdef EM_GETTEXTMODE
            if (not (EM_GETTEXTMODE == message ? translation.apply("EM_GETTEXTMODE", "Rich Edit", "Get Text Mode") : true))
            return "";
          #endif

          #ifdef EM_GETTEXTRANGE
            if (not (EM_GETTEXTRANGE == message ? translation.apply("EM_GETTEXTRANGE", "Rich Edit", "Get Text Range") : true))
            return "";
          #endif

          #ifdef EM_GETTHUMB
            if (not (EM_GETTHUMB == message ? translation.apply("EM_GETTHUMB", "Rich Edit", "Get Thumb") : true))
            return "";
          #endif

          #ifdef EM_GETTYPOGRAPHYOPTIONS
            if (not (EM_GETTYPOGRAPHYOPTIONS == message ? translation.apply("EM_GETTYPOGRAPHYOPTIONS", "Rich Edit", "Get Typography Options") : true))
            return "";
          #endif

          #ifdef EM_GETUNDONAME
            if (not (EM_GETUNDONAME == message ? translation.apply("EM_GETUNDONAME", "Rich Edit", "Get Undo Name") : true))
            return "";
          #endif

          #ifdef EM_GETVIEWKIND
            if (not (EM_GETVIEWKIND == message ? translation.apply("EM_GETVIEWKIND", "Rich Edit", "Get View Kind") : true))
            return "";
          #endif

          #ifdef EM_GETWORDBREAKPROC
            if (not (EM_GETWORDBREAKPROC == message ? translation.apply("EM_GETWORDBREAKPROC", "Rich Edit", "Get Word Break Procedure") : true))
            return "";
          #endif

          #ifdef EM_GETWORDBREAKPROCEX
            if (not (EM_GETWORDBREAKPROCEX == message ? translation.apply("EM_GETWORDBREAKPROCEX", "Rich Edit", "Get Word Break Procedure (Extended)") : true))
            return "";
          #endif

          #ifdef EM_GETWORDWRAPMODE
            if (not (EM_GETWORDWRAPMODE == message ? translation.apply("EM_GETWORDWRAPMODE", "Rich Edit", "Get Word Wrap Mode") : true))
            return "";
          #endif

          #ifdef EM_GETZOOM
            if (not (EM_GETZOOM == message ? translation.apply("EM_GETZOOM", "Rich Edit", "Get Zoom") : true))
            return "";
          #endif

          #ifdef EM_HIDESELECTION
            if (not (EM_HIDESELECTION == message ? translation.apply("EM_HIDESELECTION", "Rich Edit", "Hide Selection") : true))
            return "";
          #endif

          #ifdef EM_ISIME
            if (not (EM_ISIME == message ? translation.apply("EM_ISIME", "Rich Edit", "Is IME (Input Method Encoding)") : true))
            return "";
          #endif

          #ifdef EM_LIMITTEXT
            if (not (EM_LIMITTEXT == message ? translation.apply("EM_LIMITTEXT", "Rich Edit", "Limit Text") : true))
            return "";
          #endif

          #ifdef EM_LINEFROMCHAR
            if (not (EM_LINEFROMCHAR == message ? translation.apply("EM_LINEFROMCHAR", "Rich Edit", "Line From Character") : true))
            return "";
          #endif

          #ifdef EM_LINEINDEX
            if (not (EM_LINEINDEX == message ? translation.apply("EM_LINEINDEX", "Rich Edit", "Line Index") : true))
            return "";
          #endif

          #ifdef EM_LINELENGTH
            if (not (EM_LINELENGTH == message ? translation.apply("EM_LINELENGTH", "Rich Edit", "Line Length") : true))
            return "";
          #endif

          #ifdef EM_LINESCROLL
            if (not (EM_LINESCROLL == message ? translation.apply("EM_LINESCROLL", "Rich Edit", "Line Scroll") : true))
            return "";
          #endif

          #ifdef EM_OUTLINE
            if (not (EM_OUTLINE == message ? translation.apply("EM_OUTLINE", "Rich Edit", "Line Outline") : true))
            return "";
          #endif

          #ifdef EM_PASTESPECIAL
            if (not (EM_PASTESPECIAL == message ? translation.apply("EM_PASTESPECIAL", "Rich Edit", "Line Paste Special") : true))
            return "";
          #endif

          #ifdef EM_POSFROMCHAR
            if (not (EM_POSFROMCHAR == message ? translation.apply("EM_POSFROMCHAR", "Rich Edit", "Line Position from Character") : true))
            return "";
          #endif

          #ifdef EM_RECONVERSION
            if (not (EM_RECONVERSION == message ? translation.apply("EM_RECONVERSION", "Rich Edit", "Reconversion") : true))
            return "";
          #endif

          #ifdef EM_REDO
            if (not (EM_REDO == message ? translation.apply("EM_REDO", "Rich Edit", "Redo") : true))
            return "";
          #endif

          #ifdef EM_REPLACESEL
            if (not (EM_REPLACESEL == message ? translation.apply("EM_REPLACESEL", "Rich Edit", "Replace Selection") : true))
            return "";
          #endif

          #ifdef EM_REQUESTRESIZE
            if (not (EM_REQUESTRESIZE == message ? translation.apply("EM_REQUESTRESIZE", "Rich Edit", "Request Resize") : true))
            return "";
          #endif

          #ifdef EM_SCROLL
            if (not (EM_SCROLL == message ? translation.apply("EM_SCROLL", "Rich Edit", "Scroll") : true))
            return "";
          #endif

          #ifdef EM_SCROLLCARET
            if (not (EM_SCROLLCARET == message ? translation.apply("EM_SCROLLCARET", "Rich Edit", "Scroll Caret") : true))
            return "";
          #endif

          #ifdef EM_SELECTIONTYPE
            if (not (EM_SELECTIONTYPE == message ? translation.apply("EM_SELECTIONTYPE", "Rich Edit", "Selection Type") : true))
            return "";
          #endif

          #ifdef EM_SETBIDIOPTIONS
            if (not (EM_SETBIDIOPTIONS == message ? translation.apply("EM_SETBIDIOPTIONS", "Rich Edit", "Set Bi-Directional Options") : true))
            return "";
          #endif

          #ifdef EM_SETBKGNDCOLOR
            if (not (EM_SETBKGNDCOLOR == message ? translation.apply("EM_SETBKGNDCOLOR", "Rich Edit", "Set Background Color") : true))
            return "";
          #endif

          #ifdef EM_SETCHARFORMAT
            if (not (EM_SETCHARFORMAT == message ? translation.apply("EM_SETCHARFORMAT", "Rich Edit", "Set Character Format") : true))
            return "";
          #endif

          #ifdef EM_SETCTFMODEBIAS
            if (not (EM_SETCTFMODEBIAS == message ? translation.apply("EM_SETCTFMODEBIAS", "Rich Edit", "Set CTF Mode Bias") : true))
            return "";
          #endif

          #ifdef EM_SETCTFOPENSTATUS
            if (not (EM_SETCTFOPENSTATUS == message ? translation.apply("EM_SETCTFOPENSTATUS", "Rich Edit", "Set CTF Open Status") : true))
            return "";
          #endif

          #ifdef EM_SETEDITSTYLE
            if (not (EM_SETEDITSTYLE == message ? translation.apply("EM_SETEDITSTYLE", "Rich Edit", "Set Edit Style") : true))
            return "";
          #endif

          #ifdef EM_SETEVENTMASK
            if (not (EM_SETEVENTMASK == message ? translation.apply("EM_SETEVENTMASK", "Rich Edit", "Set Event Mask") : true))
            return "";
          #endif

          #ifdef EM_SETFONT
            if (not (EM_SETFONT == message ? translation.apply("EM_SETFONT", "Rich Edit", "Set Font") : true))
            return "";
          #endif

          #ifdef EM_SETFONTSIZE
            if (not (EM_SETFONTSIZE == message ? translation.apply("EM_SETFONTSIZE", "Rich Edit", "Set Font Size") : true))
            return "";
          #endif

          #ifdef EM_SETHANDLE
            if (not (EM_SETHANDLE == message ? translation.apply("EM_SETHANDLE", "Rich Edit", "Set Handle") : true))
            return "";
          #endif

          #ifdef EM_SETHYPHENATEINFO
            if (not (EM_SETHYPHENATEINFO == message ? translation.apply("EM_SETHYPHENATEINFO", "Rich Edit", "Set Hyphenate Information") : true))
            return "";
          #endif

          #ifdef EM_SETIMECOLOR
            if (not (EM_SETIMECOLOR == message ? translation.apply("IPM_SETFOCUS", "Internet Protocol", "Set Focus") : true))
            return "";
          #endif

          #ifdef EM_SETIMEMODEBIAS
            if (not (EM_SETIMEMODEBIAS == message ? translation.apply("EM_SETIMEMODEBIAS", "Rich Edit", "Set IME (Input Method Encoding) Mode Bias") : true))
            return "";
          #endif

          #ifdef EM_SETIMEOPTIONS
            if (not (EM_SETIMEOPTIONS == message ? translation.apply("EM_SETIMEOPTIONS", "Rich Edit", "Set IME (Input Method Encoding) Options") : true))
            return "";
          #endif

          #ifdef EM_SETIMESTATUS
            if (not (EM_SETIMESTATUS == message ? translation.apply("EM_SETIMESTATUS", "Rich Edit", "Set Time Status") : true))
            return "";
          #endif

          #ifdef EM_SETLANGOPTIONS
            if (not (EM_SETLANGOPTIONS == message ? translation.apply("EM_SETLANGOPTIONS", "Rich Edit", "Set Language Options") : true))
            return "";
          #endif

          #ifdef EM_SETLIMITTEXT
            if (not (EM_SETLIMITTEXT == message ? translation.apply("EM_SETLIMITTEXT", "Rich Edit", "Set Limit Text") : true))
            return "";
          #endif

          #ifdef EM_SETMARGINS
            if (not (EM_SETMARGINS == message ? translation.apply("EM_SETMARGINS", "Rich Edit", "Set Margins") : true))
            return "";
          #endif

          #ifdef EM_SETMODIFY
            if (not (EM_SETMODIFY == message ? translation.apply("EM_SETMODIFY", "Rich Edit", "Set Modify") : true))
            return "";
          #endif

          #ifdef EM_SETOLECALLBACK
            if (not (EM_SETOLECALLBACK == message ? translation.apply("EM_SETOLECALLBACK", "Rich Edit", "Set OLE (Object Linking & Embedding) Callback") : true))
            return "";
          #endif

          #ifdef EM_SETOPTIONS
            if (not (EM_SETOPTIONS == message ? translation.apply("EM_SETOPTIONS", "Rich Edit", "Set Options") : true))
            return "";
          #endif

          #ifdef EM_SETPAGE
            if (not (EM_SETPAGE == message ? translation.apply("EM_SETPAGE", "Rich Edit", "Set Page") : true))
            return "";
          #endif

          #ifdef EM_SETPAGEROTATE
            if (not (EM_SETPAGEROTATE == message ? translation.apply("EM_SETPAGEROTATE", "Rich Edit", "Set Page Rotate") : true))
            return "";
          #endif

          #ifdef EM_SETPALETTE
            if (not (EM_SETPALETTE == message ? translation.apply("EM_SETPALETTE", "Rich Edit", "Set Palette") : true))
            return "";
          #endif

          #ifdef EM_SETPARAFORMAT
            if (not (EM_SETPARAFORMAT == message ? translation.apply("EM_SETPARAFORMAT", "Rich Edit", "Set Paragraph Format") : true))
            return "";
          #endif

          #ifdef EM_SETPASSWORDCHAR
            if (not (EM_SETPASSWORDCHAR == message ? translation.apply("EM_SETPASSWORDCHAR", "Rich Edit", "Set Password Character") : true))
            return "";
          #endif

          #ifdef EM_SETPUNCTUATION
            if (not (EM_SETPUNCTUATION == message ? translation.apply("IPM_CLEARADDRESS", "Internet Protocol", "Clear Address") : true))
            return "";
          #endif

          #ifdef EM_SETQUERYRTFOBJ
            if (not (EM_SETQUERYRTFOBJ == message ? translation.apply("EM_SETQUERYRTFOBJ", "Rich Edit", "Set Query Rich-Text Format Object") : true))
            return "";
          #endif

          #ifdef EM_SETREADONLY
            if (not (EM_SETREADONLY == message ? translation.apply("EM_SETREADONLY", "Rich Edit", "Set Read-Only") : true))
            return "";
          #endif

          #ifdef EM_SETRECT
            if (not (EM_SETRECT == message ? translation.apply("EM_SETRECT", "Rich Edit", "Set Rectangle") : true))
            return "";
          #endif

          #ifdef EM_SETRECTNP
            if (not (EM_SETRECTNP == message ? translation.apply("EM_SETRECTNP", "Rich Edit", "Set Rectangle New Position") : true))
            return "";
          #endif

          #ifdef EM_SETSCROLLPOS
            if (not (EM_SETSCROLLPOS == message ? translation.apply("EM_SETSCROLLPOS", "Rich Edit", "Set Scroll Position") : true))
            return "";
          #endif

          #ifdef EM_SETSEL
            if (not (EM_SETSEL == message ? translation.apply("EM_SETSEL", "Rich Edit", "Set Selection") : true))
            return "";
          #endif

          #ifdef EM_SETTABSTOPS
            if (not (EM_SETTABSTOPS == message ? translation.apply("EM_SETTABSTOPS", "Rich Edit", "Set Absolute Stops") : true))
            return "";
          #endif

          #ifdef EM_SETTARGETDEVICE
            if (not (EM_SETTARGETDEVICE == message ? translation.apply("EM_SETTARGETDEVICE", "Rich Edit", "Set Target Device") : true))
            return "";
          #endif

          #ifdef EM_SETTEXTEX
            if (not (EM_SETTEXTEX == message ? translation.apply("EM_SETTEXTEX", "Rich Edit", "Set Text (Extended)") : true))
            return "";
          #endif

          #ifdef EM_SETTEXTMODE
            if (not (EM_SETTEXTMODE == message ? translation.apply("EM_SETTEXTMODE", "Rich Edit", "Set Text Mode") : true))
            return "";
          #endif

          #ifdef EM_SETTYPOGRAPHYOPTIONS
            if (not (EM_SETTYPOGRAPHYOPTIONS == message ? translation.apply("EM_SETTYPOGRAPHYOPTIONS", "Rich Edit", "Set Typography Options") : true))
            return "";
          #endif

          #ifdef EM_SETUNDOLIMIT
            if (not (EM_SETUNDOLIMIT == message ? translation.apply("EM_SETUNDOLIMIT", "Rich Edit", "Set Undo Limit") : true))
            return "";
          #endif

          #ifdef EM_SETVIEWKIND
            if (not (EM_SETVIEWKIND == message ? translation.apply("EM_SETVIEWKIND", "Rich Edit", "Set View Kind") : true))
            return "";
          #endif

          #ifdef EM_SETWORDBREAK
            if (not (EM_SETWORDBREAK == message ? translation.apply("EM_SETWORDBREAK", "Rich Edit", "Set Word Break") : true))
            return "";
          #endif

          #ifdef EM_SETWORDBREAKPROC
            if (not (EM_SETWORDBREAKPROC == message ? translation.apply("EM_SETWORDBREAKPROC", "Rich Edit", "Set Word Break Procedure") : true))
            return "";
          #endif

          #ifdef EM_SETWORDBREAKPROCEX
            if (not (EM_SETWORDBREAKPROCEX == message ? translation.apply("EM_SETWORDBREAKPROCEX", "Rich Edit", "Set Word Break Procedure (Extended)") : true))
            return "";
          #endif

          #ifdef EM_SETWORDWRAPMODE
            if (not (EM_SETWORDWRAPMODE == message ? translation.apply("EM_SETWORDWRAPMODE", "Rich Edit", "Set Word Wrap Mode") : true))
            return "";
          #endif

          #ifdef EM_SETZOOM
            if (not (EM_SETZOOM == message ? translation.apply("EM_SETZOOM", "Rich Edit", "Set Zoom") : true))
            return "";
          #endif

          #ifdef EM_SHOWSCROLLBAR
            if (not (EM_SHOWSCROLLBAR == message ? translation.apply("EM_SHOWSCROLLBAR", "Rich Edit", "Show Scrollbar") : true))
            return "";
          #endif

          #ifdef EM_STOPGROUPTYPING
            if (not (EM_STOPGROUPTYPING == message ? translation.apply("EM_STOPGROUPTYPING", "Rich Edit", "Stop Group Typing") : true))
            return "";
          #endif

          #ifdef EM_STREAMIN
            if (not (EM_STREAMIN == message ? translation.apply("EM_STREAMIN", "Rich Edit", "Stream Input") : true))
            return "";
          #endif

          #ifdef EM_STREAMOUT
            if (not (EM_STREAMOUT == message ? translation.apply("EM_STREAMOUT", "Rich Edit", "Stream Output") : true))
            return "";
          #endif

          #ifdef EM_UNDO
            if (not (EM_UNDO == message ? translation.apply("EM_UNDO", "Rich Edit", "Undo") : true))
            return "";
          #endif

          #ifdef FM_GETDRIVEINFOA
            if (not (FM_GETDRIVEINFOA == message ? translation.apply("FM_GETDRIVEINFOA", "File Manager", "Get Drive Information") : true))
            return "";
          #endif

          #ifdef FM_GETDRIVEINFOW
            if (not (FM_GETDRIVEINFOW == message ? translation.apply("FM_GETDRIVEINFOW", "File Manager", "Get Drive Information") : true))
            return "";
          #endif

          #ifdef FM_GETFILESELA
            if (not (FM_GETFILESELA == message ? translation.apply("FM_GETFILESELA", "File Manager", "Get File Selection") : true))
            return "";
          #endif

          #ifdef FM_GETFILESELLFNA
            if (not (FM_GETFILESELLFNA == message ? translation.apply("FM_GETFILESELLFNA", "File Manager", "Get File Selection Long File Name") : true))
            return "";
          #endif

          #ifdef FM_GETFILESELLFNW
            if (not (FM_GETFILESELLFNW == message ? translation.apply("FM_GETFILESELLFNW", "File Manager", "Get File Selection Long File Name") : true))
            return "";
          #endif

          #ifdef FM_GETFILESELW
            if (not (FM_GETFILESELW == message ? translation.apply("FM_GETFILESELW", "File Manager", "Get File Selection") : true))
            return "";
          #endif

          #ifdef FM_GETFOCUS
            if (not (FM_GETFOCUS == message ? translation.apply("FM_GETFOCUS", "File Manager", "Get Focus") : true))
            return "";
          #endif

          #ifdef FM_GETSELCOUNT
            if (not (FM_GETSELCOUNT == message ? translation.apply("FM_GETSELCOUNT", "File Manager", "Get Selection Count") : true))
            return "";
          #endif

          #ifdef FM_GETSELCOUNTLFN
            if (not (FM_GETSELCOUNTLFN == message ? translation.apply("FM_GETSELCOUNTLFN", "File Manager", "Get Selection Count Long File Name") : true))
            return "";
          #endif

          #ifdef FM_REFRESH_WINDOWS
            if (not (FM_REFRESH_WINDOWS == message ? translation.apply("FM_REFRESH_WINDOWS", "File Manager", "Refresh Windows") : true))
            return "";
          #endif

          #ifdef FM_RELOAD_EXTENSIONS
            if (not (FM_RELOAD_EXTENSIONS == message ? translation.apply("FM_RELOAD_EXTENSIONS", "File Manager", "Reload Extensions") : true))
            return "";
          #endif

          #ifdef HKM_GETHOTKEY
            if (not (HKM_GETHOTKEY == message ? translation.apply("HKM_GETHOTKEY", "Hotkey", "Get Hotkey") : true))
            return "";
          #endif

          #ifdef HKM_SETHOTKEY
            if (not (HKM_SETHOTKEY == message ? translation.apply("HKM_SETHOTKEY", "Hotkey", "Set Hotkey") : true))
            return "";
          #endif

          #ifdef HKM_SETRULES
            if (not (HKM_SETRULES == message ? translation.apply("HKM_SETRULES", "Hotkey", "Set Rules") : true))
            return "";
          #endif

          #ifdef IE_DOCOMMAND
            if (not (IE_DOCOMMAND == message ? translation.apply("IE_DOCOMMAND", "Ink Edit", "Do Command") : true))
            return "";
          #endif

          #ifdef IE_GETAPPDATA
            if (not (IE_GETAPPDATA == message ? translation.apply("IE_GETAPPDATA", "Ink Edit", "Get Application Data") : true))
            return "";
          #endif

          #ifdef IE_GETBKGND
            if (not (IE_GETBKGND == message ? translation.apply("IE_GETBKGND", "Ink Edit", "Get Background") : true))
            return "";
          #endif

          #ifdef IE_GETCOMMAND
            if (not (IE_GETCOMMAND == message ? translation.apply("IE_GETCOMMAND", "Ink Edit", "Get Command") : true))
            return "";
          #endif

          #ifdef IE_GETCOUNT
            if (not (IE_GETCOUNT == message ? translation.apply("IE_GETCOUNT", "Ink Edit", "Get Count") : true))
            return "";
          #endif

          #ifdef IE_GETDRAWOPTS
            if (not (IE_GETDRAWOPTS == message ? translation.apply("IE_GETDRAWOPTS", "Ink Edit", "Get Draw Options") : true))
            return "";
          #endif

          #ifdef IE_GETERASERTIP
            if (not (IE_GETERASERTIP == message ? translation.apply("IE_GETERASERTIP", "Ink Edit", "Get Eraser Tip") : true))
            return "";
          #endif

          #ifdef IE_GETFORMAT
            if (not (IE_GETFORMAT == message ? translation.apply("IE_GETFORMAT", "Ink Edit", "Get Format") : true))
            return "";
          #endif

          #ifdef IE_GETGESTURE
            if (not (IE_GETGESTURE == message ? translation.apply("IE_GETGESTURE", "Ink Edit", "Get Gesture") : true))
            return "";
          #endif

          #ifdef IE_GETGRIDORIGIN
            if (not (IE_GETGRIDORIGIN == message ? translation.apply("IE_GETGRIDORIGIN", "Ink Edit", "Get Grid Origin") : true))
            return "";
          #endif

          #ifdef IE_GETGRIDPEN
            if (not (IE_GETGRIDPEN == message ? translation.apply("IE_GETGRIDPEN", "Ink Edit", "Get Grid Pen") : true))
            return "";
          #endif

          #ifdef IE_GETGRIDSIZE
            if (not (IE_GETGRIDSIZE == message ? translation.apply("IE_GETGRIDSIZE", "Ink Edit", "Get Grid Size") : true))
            return "";
          #endif

          #ifdef IE_GETINK
            if (not (IE_GETINK == message ? translation.apply("IE_GETINK", "Ink Edit", "Get Ink") : true))
            return "";
          #endif

          #ifdef IE_GETINKINPUT
            if (not (IE_GETINKINPUT == message ? translation.apply("IE_GETINKINPUT", "Ink Edit", "Get Ink Input") : true))
            return "";
          #endif

          #ifdef IE_GETMENU
            if (not (IE_GETMENU == message ? translation.apply("IE_GETMENU", "Ink Edit", "Get Menu") : true))
            return "";
          #endif

          #ifdef IE_GETMODE
            if (not (IE_GETMODE == message ? translation.apply("IE_GETMODE", "Ink Edit", "Get Mode") : true))
            return "";
          #endif

          #ifdef IE_GETNOTIFY
            if (not (IE_GETNOTIFY == message ? translation.apply("IE_GETNOTIFY", "Ink Edit", "Get Notify") : true))
            return "";
          #endif

          #ifdef IE_GETPAINTDC
            if (not (IE_GETPAINTDC == message ? translation.apply("IE_GETPAINTDC", "Ink Edit", "Get Paint Device Context") : true))
            return "";
          #endif

          #ifdef IE_GETPDEVENT
            if (not (IE_GETPDEVENT == message ? translation.apply("IE_GETPDEVENT", "Ink Edit", "Get PD Event") : true))
            return "";
          #endif

          #ifdef IE_GETPENTIP
            if (not (IE_GETPENTIP == message ? translation.apply("IE_GETPENTIP", "Ink Edit", "Get Pen Tip") : true))
            return "";
          #endif

          #ifdef IE_GETRECOG
            if (not (IE_GETRECOG == message ? translation.apply("IE_GETRECOG", "Ink Edit", "Get Recognition") : true))
            return "";
          #endif

          #ifdef IE_GETSECURITY
            if (not (IE_GETSECURITY == message ? translation.apply("IE_GETSECURITY", "Ink Edit", "Get Security") : true))
            return "";
          #endif

          #ifdef IE_GETSEL
            if (not (IE_GETSEL == message ? translation.apply("IE_GETSEL", "Ink Edit", "Get Selection") : true))
            return "";
          #endif

          #ifdef IE_GETSELCOUNT
            if (not (IE_GETSELCOUNT == message ? translation.apply("IE_GETSELCOUNT", "Ink Edit", "Get Selection Count") : true))
            return "";
          #endif

          #ifdef IE_GETSELITEMS
            if (not (IE_GETSELITEMS == message ? translation.apply("IE_GETSELITEMS", "Ink Edit", "Get Selection Items") : true))
            return "";
          #endif

          #ifdef IE_GETSTYLE
            if (not (IE_GETSTYLE == message ? translation.apply("IE_GETSTYLE", "Ink Edit", "Get Style") : true))
            return "";
          #endif

          #ifdef IE_MSGFIRST
            if (not (IE_MSGFIRST == message ? translation.apply("IE_MSGFIRST", "Ink Edit", "Message 1st") : true))
            return "";
          #endif

          #ifdef IE_SETAPPDATA
            if (not (IE_SETAPPDATA == message ? translation.apply("IE_SETAPPDATA", "Ink Edit", "Set Application Data") : true))
            return "";
          #endif

          #ifdef IE_SETBKGND
            if (not (IE_SETBKGND == message ? translation.apply("IE_SETBKGND", "Ink Edit", "Set Background") : true))
            return "";
          #endif

          #ifdef IE_SETDRAWOPTS
            if (not (IE_SETDRAWOPTS == message ? translation.apply("IE_SETDRAWOPTS", "Ink Edit", "Set Draw Options") : true))
            return "";
          #endif

          #ifdef IE_SETERASERTIP
            if (not (IE_SETERASERTIP == message ? translation.apply("IE_SETERASERTIP", "Ink Edit", "Set Eraser Tip") : true))
            return "";
          #endif

          #ifdef IE_SETFORMAT
            if (not (IE_SETFORMAT == message ? translation.apply("IE_SETFORMAT", "Ink Edit", "Set Format") : true))
            return "";
          #endif

          #ifdef IE_SETGRIDORIGIN
            if (not (IE_SETGRIDORIGIN == message ? translation.apply("IE_SETGRIDORIGIN", "Ink Edit", "Set Grid Origin") : true))
            return "";
          #endif

          #ifdef IE_SETGRIDPEN
            if (not (IE_SETGRIDPEN == message ? translation.apply("IE_SETGRIDPEN", "Ink Edit", "Set Grid Pen") : true))
            return "";
          #endif

          #ifdef IE_SETGRIDSIZE
            if (not (IE_SETGRIDSIZE == message ? translation.apply("IE_SETGRIDSIZE", "Ink Edit", "Set Grid Size") : true))
            return "";
          #endif

          #ifdef IE_SETINK
            if (not (IE_SETINK == message ? translation.apply("IE_SETINK", "Ink Edit", "Set Ink") : true))
            return "";
          #endif

          #ifdef IE_SETINKINPUT
            if (not (IE_SETINKINPUT == message ? translation.apply("IE_SETINKINPUT", "Ink Edit", "Set Ink Input") : true))
            return "";
          #endif

          #ifdef IE_SETMODE
            if (not (IE_SETMODE == message ? translation.apply("IE_SETMODE", "Ink Edit", "Set Mode") : true))
            return "";
          #endif

          #ifdef IE_SETNOTIFY
            if (not (IE_SETNOTIFY == message ? translation.apply("IE_SETNOTIFY", "Ink Edit", "Set Notify") : true))
            return "";
          #endif

          #ifdef IE_SETPENTIP
            if (not (IE_SETPENTIP == message ? translation.apply("IE_SETPENTIP", "Ink Edit", "Set Pen Tip") : true))
            return "";
          #endif

          #ifdef IE_SETRECOG
            if (not (IE_SETRECOG == message ? translation.apply("IE_SETRECOG", "Ink Edit", "Set Recognition") : true))
            return "";
          #endif

          #ifdef IE_SETSECURITY
            if (not (IE_SETSECURITY == message ? translation.apply("IE_SETSECURITY", "Ink Edit", "Set Security") : true))
            return "";
          #endif

          #ifdef IE_SETSEL
            if (not (IE_SETSEL == message ? translation.apply("IE_SETSEL", "Ink Edit", "Set Selection") : true))
            return "";
          #endif

          #ifdef IPM_CLEARADDRESS
            if (not (IPM_CLEARADDRESS == message ? translation.apply("EM_SETPUNCTUATION", "Rich Edit", "Set Punctuation") : true))
            return "";
          #endif

          #ifdef IPM_GETADDRESS
            if (not (IPM_GETADDRESS == message ? translation.apply("IPM_GETADDRESS", "Internet Protocol", "Get Address") : true))
            return "";
          #endif

          #ifdef IPM_ISBLANK
            if (not (IPM_ISBLANK == message ? translation.apply("IPM_ISBLANK", "Internet Protocol", "Is Blank") : true))
            return "";
          #endif

          #ifdef IPM_SETADDRESS
            if (not (IPM_SETADDRESS == message ? translation.apply("IPM_SETADDRESS", "Internet Protocol", "Set Address") : true))
            return "";
          #endif

          #ifdef IPM_SETFOCUS
            if (not (IPM_SETFOCUS == message ? translation.apply("PSM_CHANGED", "Prop Sheet", "Changed") : true))
            return "";
          #endif

          #ifdef IPM_SETRANGE
            if (not (IPM_SETRANGE == message ? translation.apply("IPM_SETRANGE", "Internet Protocol", "Set Range") : true))
            return "";
          #endif

          #ifdef LVM_ARRANGE
            if (not (LVM_ARRANGE == message ? translation.apply("LVM_ARRANGE", "List View", "Arrange") : true))
            return "";
          #endif

          #ifdef LVM_CANCELEDITLABEL
            if (not (LVM_CANCELEDITLABEL == message ? translation.apply("LVM_CANCELEDITLABEL", "List View", "Cancel Edit Label") : true))
            return "";
          #endif

          #ifdef LVM_CREATEDRAGIMAGE
            if (not (LVM_CREATEDRAGIMAGE == message ? translation.apply("LVM_CREATEDRAGIMAGE", "List View", "Create Drag Image") : true))
            return "";
          #endif

          #ifdef LVM_DELETEALLITEMS
            if (not (LVM_DELETEALLITEMS == message ? translation.apply("LVM_DELETEALLITEMS", "List View", "Delete All Items") : true))
            return "";
          #endif

          #ifdef LVM_DELETECOLUMN
            if (not (LVM_DELETECOLUMN == message ? translation.apply("LVM_DELETECOLUMN", "List View", "Delete Column") : true))
            return "";
          #endif

          #ifdef LVM_DELETEITEM
            if (not (LVM_DELETEITEM == message ? translation.apply("LVM_DELETEITEM", "List View", "Delete Item") : true))
            return "";
          #endif

          #ifdef LVM_EDITLABELA
            if (not (LVM_EDITLABELA == message ? translation.apply("LVM_EDITLABELA", "List View", "Edit Label") : true))
            return "";
          #endif

          #ifdef LVM_EDITLABELW
            if (not (LVM_EDITLABELW == message ? translation.apply("LVM_EDITLABELW", "List View", "Edit Label") : true))
            return "";
          #endif

          #ifdef LVM_ENABLEGROUPVIEW
            if (not (LVM_ENABLEGROUPVIEW == message ? translation.apply("LVM_ENABLEGROUPVIEW", "List View", "Enable Group View") : true))
            return "";
          #endif

          #ifdef LVM_ENSUREVISIBLE
            if (not (LVM_ENSUREVISIBLE == message ? translation.apply("LVM_ENSUREVISIBLE", "List View", "Ensure Visible") : true))
            return "";
          #endif

          #ifdef LVM_FINDITEMA
            if (not (LVM_FINDITEMA == message ? translation.apply("LVM_FINDITEMA", "List View", "Find Item") : true))
            return "";
          #endif

          #ifdef LVM_FINDITEMW
            if (not (LVM_FINDITEMW == message ? translation.apply("LVM_FINDITEMW", "List View", "Find Item") : true))
            return "";
          #endif

          #ifdef LVM_FIRST
            if (not (LVM_FIRST == message ? translation.apply("LVM_FIRST", "List View", "1st") : true))
            return "";
          #endif

          #ifdef LVM_GETBKCOLOR
            if (not (LVM_GETBKCOLOR == message ? translation.apply("LVM_GETBKCOLOR", "List View", "Get Background Color") : true))
            return "";
          #endif

          #ifdef LVM_GETBKIMAGEA
            if (not (LVM_GETBKIMAGEA == message ? translation.apply("LVM_GETBKIMAGEA", "List View", "Get Background Image") : true))
            return "";
          #endif

          #ifdef LVM_GETBKIMAGEW
            if (not (LVM_GETBKIMAGEW == message ? translation.apply("LVM_GETBKIMAGEW", "List View", "Get Background Image") : true))
            return "";
          #endif

          #ifdef LVM_GETCALLBACKMASK
            if (not (LVM_GETCALLBACKMASK == message ? translation.apply("LVM_GETCALLBACKMASK", "List View", "Get Call Back Mask") : true))
            return "";
          #endif

          #ifdef LVM_GETCOLUMNA
            if (not (LVM_GETCOLUMNA == message ? translation.apply("LVM_GETCOLUMNA", "List View", "Get Column") : true))
            return "";
          #endif

          #ifdef LVM_GETCOLUMNORDERARRAY
            if (not (LVM_GETCOLUMNORDERARRAY == message ? translation.apply("LVM_GETCOLUMNORDERARRAY", "List View", "Get Column Order Array") : true))
            return "";
          #endif

          #ifdef LVM_GETCOLUMNW
            if (not (LVM_GETCOLUMNW == message ? translation.apply("LVM_GETCOLUMNW", "List View", "Get Column") : true))
            return "";
          #endif

          #ifdef LVM_GETCOLUMNWIDTH
            if (not (LVM_GETCOLUMNWIDTH == message ? translation.apply("LVM_GETCOLUMNWIDTH", "List View", "Get Column Width") : true))
            return "";
          #endif

          #ifdef LVM_GETCOUNTPERPAGE
            if (not (LVM_GETCOUNTPERPAGE == message ? translation.apply("LVM_GETCOUNTPERPAGE", "List View", "Get Count per Page") : true))
            return "";
          #endif

          #ifdef LVM_GETEDITCONTROL
            if (not (LVM_GETEDITCONTROL == message ? translation.apply("LVM_GETEDITCONTROL", "List View", "Get Edit Control") : true))
            return "";
          #endif

          #ifdef LVM_GETEXTENDEDLISTVIEWSTYLE
            if (not (LVM_GETEXTENDEDLISTVIEWSTYLE == message ? translation.apply("LVM_GETEXTENDEDLISTVIEWSTYLE", "List View", "Get Extended List View Style") : true))
            return "";
          #endif

          #ifdef LVM_GETGROUPINFO
            if (not (LVM_GETGROUPINFO == message ? translation.apply("LVM_GETGROUPINFO", "List View", "Get Group Information") : true))
            return "";
          #endif

          #ifdef LVM_GETGROUPMETRICS
            if (not (LVM_GETGROUPMETRICS == message ? translation.apply("LVM_GETGROUPMETRICS", "List View", "Get Group Metrics") : true))
            return "";
          #endif

          #ifdef LVM_GETHEADER
            if (not (LVM_GETHEADER == message ? translation.apply("LVM_GETHEADER", "List View", "Get Header") : true))
            return "";
          #endif

          #ifdef LVM_GETHOTCURSOR
            if (not (LVM_GETHOTCURSOR == message ? translation.apply("LVM_GETHOTCURSOR", "List View", "Get Hot Cursor") : true))
            return "";
          #endif

          #ifdef LVM_GETHOTITEM
            if (not (LVM_GETHOTITEM == message ? translation.apply("LVM_GETHOTITEM", "List View", "Get Hover Time") : true))
            return "";
          #endif

          #ifdef LVM_GETHOVERTIME
            if (not (LVM_GETHOVERTIME == message ? translation.apply("LVM_GETHOVERTIME", "List View", "Get Image List") : true))
            return "";
          #endif

          #ifdef LVM_GETIMAGELIST
            if (not (LVM_GETIMAGELIST == message ? translation.apply("LVM_GETIMAGELIST", "List View", "Get Image List") : true))
            return "";
          #endif

          #ifdef LVM_GETINSERTMARK
            if (not (LVM_GETINSERTMARK == message ? translation.apply("LVM_GETINSERTMARK", "List View", "Get Insert Mark") : true))
            return "";
          #endif

          #ifdef LVM_GETINSERTMARKCOLOR
            if (not (LVM_GETINSERTMARKCOLOR == message ? translation.apply("LVM_GETINSERTMARKCOLOR", "List View", "Get Insert Mark Color") : true))
            return "";
          #endif

          #ifdef LVM_GETINSERTMARKRECT
            if (not (LVM_GETINSERTMARKRECT == message ? translation.apply("LVM_GETINSERTMARKRECT", "List View", "Get Insert Mark Rectangle") : true))
            return "";
          #endif

          #ifdef LVM_GETISEARCHSTRINGA
            if (not (LVM_GETISEARCHSTRINGA == message ? translation.apply("LVM_GETISEARCHSTRINGA", "List View", "Get Incremental Search String") : true))
            return "";
          #endif

          #ifdef LVM_GETISEARCHSTRINGW
            if (not (LVM_GETISEARCHSTRINGW == message ? translation.apply("LVM_GETISEARCHSTRINGW", "List View", "Get Incremental Search String") : true))
            return "";
          #endif

          #ifdef LVM_GETITEMA
            if (not (LVM_GETITEMA == message ? translation.apply("LVM_GETITEMA", "List View", "Get Item") : true))
            return "";
          #endif

          #ifdef LVM_GETITEMCOUNT
            if (not (LVM_GETITEMCOUNT == message ? translation.apply("LVM_GETITEMCOUNT", "List View", "Get Item Count") : true))
            return "";
          #endif

          #ifdef LVM_GETITEMPOSITION
            if (not (LVM_GETITEMPOSITION == message ? translation.apply("LVM_GETITEMPOSITION", "List View", "Get Item Position") : true))
            return "";
          #endif

          #ifdef LVM_GETITEMRECT
            if (not (LVM_GETITEMRECT == message ? translation.apply("LVM_GETITEMRECT", "List View", "Get Item Rectangle") : true))
            return "";
          #endif

          #ifdef LVM_GETITEMSPACING
            if (not (LVM_GETITEMSPACING == message ? translation.apply("LVM_GETITEMSPACING", "List View", "Get Item Spacing") : true))
            return "";
          #endif

          #ifdef LVM_GETITEMSTATE
            if (not (LVM_GETITEMSTATE == message ? translation.apply("LVM_GETITEMSTATE", "List View", "Get Item State") : true))
            return "";
          #endif

          #ifdef LVM_GETITEMTEXTA
            if (not (LVM_GETITEMTEXTA == message ? translation.apply("LVM_GETITEMTEXTA", "List View", "Get Item Text") : true))
            return "";
          #endif

          #ifdef LVM_GETITEMTEXTW
            if (not (LVM_GETITEMTEXTW == message ? translation.apply("LVM_GETITEMTEXTW", "List View", "Get Item Text") : true))
            return "";
          #endif

          #ifdef LVM_GETITEMW
            if (not (LVM_GETITEMW == message ? translation.apply("LVM_GETITEMW", "List View", "Get Item") : true))
            return "";
          #endif

          #ifdef LVM_GETNEXTITEM
            if (not (LVM_GETNEXTITEM == message ? translation.apply("LVM_GETNEXTITEM", "List View", "Get Next Item") : true))
            return "";
          #endif

          #ifdef LVM_GETNUMBEROFWORKAREAS
            if (not (LVM_GETNUMBEROFWORKAREAS == message ? translation.apply("LVM_GETNUMBEROFWORKAREAS", "List View", "Get Number of Work Areas") : true))
            return "";
          #endif

          #ifdef LVM_GETORIGIN
            if (not (LVM_GETORIGIN == message ? translation.apply("LVM_GETORIGIN", "List View", "Get Origin") : true))
            return "";
          #endif

          #ifdef LVM_GETOUTLINECOLOR
            if (not (LVM_GETOUTLINECOLOR == message ? translation.apply("LVM_GETOUTLINECOLOR", "List View", "Get Outline Color") : true))
            return "";
          #endif

          #ifdef LVM_GETSELECTEDCOLUMN
            if (not (LVM_GETSELECTEDCOLUMN == message ? translation.apply("LVM_GETSELECTEDCOLUMN", "List View", "Get Selected Column") : true))
            return "";
          #endif

          #ifdef LVM_GETSELECTEDCOUNT
            if (not (LVM_GETSELECTEDCOUNT == message ? translation.apply("LVM_GETSELECTEDCOUNT", "List View", "Get Selected Count") : true))
            return "";
          #endif

          #ifdef LVM_GETSELECTIONMARK
            if (not (LVM_GETSELECTIONMARK == message ? translation.apply("LVM_GETSELECTIONMARK", "List View", "Get Selection Mark") : true))
            return "";
          #endif

          #ifdef LVM_GETSTRINGWIDTHA
            if (not (LVM_GETSTRINGWIDTHA == message ? translation.apply("LVM_GETSTRINGWIDTHA", "List View", "Get String Width") : true))
            return "";
          #endif

          #ifdef LVM_GETSTRINGWIDTHW
            if (not (LVM_GETSTRINGWIDTHW == message ? translation.apply("LVM_GETSTRINGWIDTHW", "List View", "Get String Width") : true))
            return "";
          #endif

          #ifdef LVM_GETSUBITEMRECT
            if (not (LVM_GETSUBITEMRECT == message ? translation.apply("LVM_GETSUBITEMRECT", "List View", "Get Sub-Item Rectangle") : true))
            return "";
          #endif

          #ifdef LVM_GETTEXTBKCOLOR
            if (not (LVM_GETTEXTBKCOLOR == message ? translation.apply("LVM_GETTEXTBKCOLOR", "List View", "Get Text Background Color") : true))
            return "";
          #endif

          #ifdef LVM_GETTEXTCOLOR
            if (not (LVM_GETTEXTCOLOR == message ? translation.apply("LVM_GETTEXTCOLOR", "List View", "Get Text Color") : true))
            return "";
          #endif

          #ifdef LVM_GETTILEINFO
            if (not (LVM_GETTILEINFO == message ? translation.apply("LVM_GETTILEINFO", "List View", "Get Tile Information") : true))
            return "";
          #endif

          #ifdef LVM_GETTILEVIEWINFO
            if (not (LVM_GETTILEVIEWINFO == message ? translation.apply("LVM_GETTILEVIEWINFO", "List View", "Get Tile View Information") : true))
            return "";
          #endif

          #ifdef LVM_GETTOOLTIPS
            if (not (LVM_GETTOOLTIPS == message ? translation.apply("LVM_GETTOOLTIPS", "List View", "Get Tooltips") : true))
            return "";
          #endif

          #ifdef LVM_GETTOPINDEX
            if (not (LVM_GETTOPINDEX == message ? translation.apply("LVM_GETTOPINDEX", "List View", "Get Top Index") : true))
            return "";
          #endif

          #ifdef LVM_GETUNICODEFORMAT
            if (not (LVM_GETUNICODEFORMAT == message ? translation.apply("LVM_GETUNICODEFORMAT", "List View", "Get Unicode Format") : true))
            return "";
          #endif

          #ifdef LVM_GETVIEW
            if (not (LVM_GETVIEW == message ? translation.apply("LVM_GETVIEW", "List View", "Get View") : true))
            return "";
          #endif

          #ifdef LVM_GETVIEWRECT
            if (not (LVM_GETVIEWRECT == message ? translation.apply("LVM_GETVIEWRECT", "List View", "Get View Rectangle") : true))
            return "";
          #endif

          #ifdef LVM_GETWORKAREAS
            if (not (LVM_GETWORKAREAS == message ? translation.apply("LVM_GETWORKAREAS", "List View", "Get Work Areas") : true))
            return "";
          #endif

          #ifdef LVM_HASGROUP
            if (not (LVM_HASGROUP == message ? translation.apply("LVM_HASGROUP", "List View", "Has Group") : true))
            return "";
          #endif

          #ifdef LVM_HITTEST
            if (not (LVM_HITTEST == message ? translation.apply("LVM_HITTEST", "List View", "Hit Test") : true))
            return "";
          #endif

          #ifdef LVM_INSERTCOLUMNA
            if (not (LVM_INSERTCOLUMNA == message ? translation.apply("LVM_INSERTCOLUMNA", "List View", "Insert Column") : true))
            return "";
          #endif

          #ifdef LVM_INSERTCOLUMNW
            if (not (LVM_INSERTCOLUMNW == message ? translation.apply("LVM_INSERTCOLUMNW", "List View", "Insert Column") : true))
            return "";
          #endif

          #ifdef LVM_INSERTGROUP
            if (not (LVM_INSERTGROUP == message ? translation.apply("LVM_INSERTGROUP", "List View", "Insert Group") : true))
            return "";
          #endif

          #ifdef LVM_INSERTGROUPSORTED
            if (not (LVM_INSERTGROUPSORTED == message ? translation.apply("LVM_INSERTGROUPSORTED", "List View", "Insert Group Sorted") : true))
            return "";
          #endif

          #ifdef LVM_INSERTITEMA
            if (not (LVM_INSERTITEMA == message ? translation.apply("LVM_INSERTITEMA", "List View", "Insert Item") : true))
            return "";
          #endif

          #ifdef LVM_INSERTITEMW
            if (not (LVM_INSERTITEMW == message ? translation.apply("LVM_INSERTITEMW", "List View", "Insert Item") : true))
            return "";
          #endif

          #ifdef LVM_INSERTMARKHITTEST
            if (not (LVM_INSERTMARKHITTEST == message ? translation.apply("LVM_INSERTMARKHITTEST", "List View", "Insert Mark Hit Test") : true))
            return "";
          #endif

          #ifdef LVM_ISGROUPVIEWENABLED
            if (not (LVM_ISGROUPVIEWENABLED == message ? translation.apply("LVM_ISGROUPVIEWENABLED", "List View", "Is Group View Enabled") : true))
            return "";
          #endif

          #ifdef LVM_ISITEMVISIBLE
            if (not (LVM_ISITEMVISIBLE == message ? translation.apply("LVM_ISITEMVISIBLE", "List View", "Is Item Visible") : true))
            return "";
          #endif

          #ifdef LVM_MAPIDTOINDEX
            if (not (LVM_MAPIDTOINDEX == message ? translation.apply("LVM_MAPIDTOINDEX", "List View", "Map Identity to Index") : true))
            return "";
          #endif

          #ifdef LVM_MAPINDEXTOID
            if (not (LVM_MAPINDEXTOID == message ? translation.apply("LVM_MAPINDEXTOID", "List View", "Map Index to Identity") : true))
            return "";
          #endif

          #ifdef LVM_MOVEGROUP
            if (not (LVM_MOVEGROUP == message ? translation.apply("LVM_MOVEGROUP", "List View", "Move Group") : true))
            return "";
          #endif

          #ifdef LVM_MOVEITEMTOGROUP
            if (not (LVM_MOVEITEMTOGROUP == message ? translation.apply("LVM_MOVEITEMTOGROUP", "List View", "Move Item to Group") : true))
            return "";
          #endif

          #ifdef LVM_REDRAWITEMS
            if (not (LVM_REDRAWITEMS == message ? translation.apply("LVM_REDRAWITEMS", "List View", "Redraw Items") : true))
            return "";
          #endif

          #ifdef LVM_REMOVEALLGROUPS
            if (not (LVM_REMOVEALLGROUPS == message ? translation.apply("LVM_REMOVEALLGROUPS", "List View", "Remove all Groups") : true))
            return "";
          #endif

          #ifdef LVM_REMOVEGROUP
            if (not (LVM_REMOVEGROUP == message ? translation.apply("LVM_REMOVEGROUP", "List View", "Remove Group") : true))
            return "";
          #endif

          #ifdef LVM_SCROLL
            if (not (LVM_SCROLL == message ? translation.apply("LVM_SCROLL", "List View", "Scroll") : true))
            return "";
          #endif

          #ifdef LVM_SETBKCOLOR
            if (not (LVM_SETBKCOLOR == message ? translation.apply("LVM_SETBKCOLOR", "List View", "Set Background Color") : true))
            return "";
          #endif

          #ifdef LVM_SETBKIMAGEA
            if (not (LVM_SETBKIMAGEA == message ? translation.apply("LVM_SETBKIMAGEA", "List View", "Set Background Image") : true))
            return "";
          #endif

          #ifdef LVM_SETCALLBACKMASK
            if (not (LVM_SETCALLBACKMASK == message ? translation.apply("LVM_SETCALLBACKMASK", "List View", "Set Callback Mask") : true))
            return "";
          #endif

          #ifdef LVM_SETCOLUMNA
            if (not (LVM_SETCOLUMNA == message ? translation.apply("LVM_SETCOLUMNA", "List View", "Set Column") : true))
            return "";
          #endif

          #ifdef LVM_SETCOLUMNORDERARRAY
            if (not (LVM_SETCOLUMNORDERARRAY == message ? translation.apply("LVM_SETCOLUMNORDERARRAY", "List View", "Set Column Order Array") : true))
            return "";
          #endif

          #ifdef LVM_SETCOLUMNW
            if (not (LVM_SETCOLUMNW == message ? translation.apply("LVM_SETCOLUMNW", "List View", "Set Column") : true))
            return "";
          #endif

          #ifdef LVM_SETCOLUMNWIDTH
            if (not (LVM_SETCOLUMNWIDTH == message ? translation.apply("LVM_SETCOLUMNWIDTH", "List View", "Set Column Width") : true))
            return "";
          #endif

          #ifdef LVM_SETEXTENDEDLISTVIEWSTYLE
            if (not (LVM_SETEXTENDEDLISTVIEWSTYLE == message ? translation.apply("LVM_SETEXTENDEDLISTVIEWSTYLE", "List View", "Set Extended List View Style") : true))
            return "";
          #endif

          #ifdef LVM_SETGROUPINFO
            if (not (LVM_SETGROUPINFO == message ? translation.apply("LVM_SETGROUPINFO", "List View", "Set Group Information") : true))
            return "";
          #endif

          #ifdef LVM_SETGROUPMETRICS
            if (not (LVM_SETGROUPMETRICS == message ? translation.apply("LVM_SETGROUPMETRICS", "List View", "Set Group Metrics") : true))
            return "";
          #endif

          #ifdef LVM_SETHOTCURSOR
            if (not (LVM_SETHOTCURSOR == message ? translation.apply("LVM_SETHOTCURSOR", "List View", "Set Hot Cursor") : true))
            return "";
          #endif

          #ifdef LVM_SETHOTITEM
            if (not (LVM_SETHOTITEM == message ? translation.apply("LVM_SETHOTITEM", "List View", "Set Hot Item") : true))
            return "";
          #endif

          #ifdef LVM_SETHOVERTIME
            if (not (LVM_SETHOVERTIME == message ? translation.apply("LVM_SETHOVERTIME", "List View", "Set Hover Time") : true))
            return "";
          #endif

          #ifdef LVM_SETICONSPACING
            if (not (LVM_SETICONSPACING == message ? translation.apply("LVM_SETICONSPACING", "List View", "Set Icon Spacing") : true))
            return "";
          #endif

          #ifdef LVM_SETIMAGELIST
            if (not (LVM_SETIMAGELIST == message ? translation.apply("LVM_SETIMAGELIST", "List View", "Set Image List") : true))
            return "";
          #endif

          #ifdef LVM_SETINFOTIP
            if (not (LVM_SETINFOTIP == message ? translation.apply("LVM_SETINFOTIP", "List View", "Set Information Tip") : true))
            return "";
          #endif

          #ifdef LVM_SETINSERTMARK
            if (not (LVM_SETINSERTMARK == message ? translation.apply("LVM_SETINSERTMARK", "List View", "Set Insert Mark") : true))
            return "";
          #endif

          #ifdef LVM_SETINSERTMARKCOLOR
            if (not (LVM_SETINSERTMARKCOLOR == message ? translation.apply("LVM_SETINSERTMARKCOLOR", "List View", "Set Insert Mark Color") : true))
            return "";
          #endif

          #ifdef LVM_SETITEMA
            if (not (LVM_SETITEMA == message ? translation.apply("LVM_SETITEMA", "List View", "Set Item") : true))
            return "";
          #endif

          #ifdef LVM_SETITEMCOUNT
            if (not (LVM_SETITEMCOUNT == message ? translation.apply("LVM_SETITEMCOUNT", "List View", "Set Item Count") : true))
            return "";
          #endif

          #ifdef LVM_SETITEMPOSITION
            if (not (LVM_SETITEMPOSITION == message ? translation.apply("LVM_SETITEMPOSITION", "List View", "Set Item Position") : true))
            return "";
          #endif

          #ifdef LVM_SETITEMPOSITION32
            if (not (LVM_SETITEMPOSITION32 == message ? translation.apply("LVM_SETITEMPOSITION32", "List View", "Set Item Position (32-Bit)") : true))
            return "";
          #endif

          #ifdef LVM_SETITEMSTATE
            if (not (LVM_SETITEMSTATE == message ? translation.apply("LVM_SETITEMSTATE", "List View", "Set Item State") : true))
            return "";
          #endif

          #ifdef LVM_SETITEMTEXTA
            if (not (LVM_SETITEMTEXTA == message ? translation.apply("LVM_SETITEMTEXTA", "List View", "Set Item Text") : true))
            return "";
          #endif

          #ifdef LVM_SETITEMTEXTW
            if (not (LVM_SETITEMTEXTW == message ? translation.apply("LVM_SETITEMTEXTW", "List View", "Set Item Text") : true))
            return "";
          #endif

          #ifdef LVM_SETITEMW
            if (not (LVM_SETITEMW == message ? translation.apply("LVM_SETITEMW", "List View", "Set Item") : true))
            return "";
          #endif

          #ifdef LVM_SETOUTLINECOLOR
            if (not (LVM_SETOUTLINECOLOR == message ? translation.apply("LVM_SETOUTLINECOLOR", "List View", "Set Outline Color") : true))
            return "";
          #endif

          #ifdef LVM_SETSELECTEDCOLUMN
            if (not (LVM_SETSELECTEDCOLUMN == message ? translation.apply("LVM_SETSELECTEDCOLUMN", "List View", "Set Selected Column") : true))
            return "";
          #endif

          #ifdef LVM_SETSELECTIONMARK
            if (not (LVM_SETSELECTIONMARK == message ? translation.apply("LVM_SETSELECTIONMARK", "List View", "Set Selection Mark") : true))
            return "";
          #endif

          #ifdef LVM_SETTEXTBKCOLOR
            if (not (LVM_SETTEXTBKCOLOR == message ? translation.apply("LVM_SETTEXTBKCOLOR", "List View", "Set Text Background Color") : true))
            return "";
          #endif

          #ifdef LVM_SETTEXTCOLOR
            if (not (LVM_SETTEXTCOLOR == message ? translation.apply("LVM_SETTEXTCOLOR", "List View", "Set Text Color") : true))
            return "";
          #endif

          #ifdef LVM_SETTILEINFO
            if (not (LVM_SETTILEINFO == message ? translation.apply("LVM_SETTILEINFO", "List View", "Set Tile Information") : true))
            return "";
          #endif

          #ifdef LVM_SETTILEVIEWINFO
            if (not (LVM_SETTILEVIEWINFO == message ? translation.apply("LVM_SETTILEVIEWINFO", "List View", "Set Tile View Information") : true))
            return "";
          #endif

          #ifdef LVM_SETTILEWIDTH
            if (not (LVM_SETTILEWIDTH == message ? translation.apply("LVM_SETTILEWIDTH", "List View", "Set Tile Width") : true))
            return "";
          #endif

          #ifdef LVM_SETTOOLTIPS
            if (not (LVM_SETTOOLTIPS == message ? translation.apply("LVM_SETTOOLTIPS", "List View", "Set Tooltips") : true))
            return "";
          #endif

          #ifdef LVM_SETUNICODEFORMAT
            if (not (LVM_SETUNICODEFORMAT == message ? translation.apply("LVM_SETUNICODEFORMAT", "List View", "Set Unicode Format") : true))
            return "";
          #endif

          #ifdef LVM_SETVIEW
            if (not (LVM_SETVIEW == message ? translation.apply("LVM_SETVIEW", "List View", "Set View") : true))
            return "";
          #endif

          #ifdef LVM_SETWORKAREAS
            if (not (LVM_SETWORKAREAS == message ? translation.apply("LVM_SETWORKAREAS", "List View", "Set WorkaAreas") : true))
            return "";
          #endif

          #ifdef LVM_SORTGROUPS
            if (not (LVM_SORTGROUPS == message ? translation.apply("LVM_SORTGROUPS", "List View", "Sort Groups") : true))
            return "";
          #endif

          #ifdef LVM_SORTITEMS
            if (not (LVM_SORTITEMS == message ? translation.apply("LVM_SORTITEMS", "List View", "Sort Items") : true))
            return "";
          #endif

          #ifdef LVM_SUBITEMHITTEST
            if (not (LVM_SUBITEMHITTEST == message ? translation.apply("LVM_SUBITEMHITTEST", "List View", "Sub-Item Hit Test") : true))
            return "";
          #endif

          #ifdef LVM_UPDATE
            if (not (LVM_UPDATE == message ? translation.apply("LVM_UPDATE", "List View", "Update") : true))
            return "";
          #endif

          #ifdef MCIWNDM_CAN_CONFIG
            if (not (MCIWNDM_CAN_CONFIG == message ? translation.apply("MCIWNDM_CAN_CONFIG", "MCI (Media Control Interface) Window", "Can Configure") : true))
            return "";
          #endif

          #ifdef MCIWNDM_CAN_EJECT
            if (not (MCIWNDM_CAN_EJECT == message ? translation.apply("MCIWNDM_CAN_EJECT", "MCI (Media Control Interface) Window", "Can Eject") : true))
            return "";
          #endif

          #ifdef MCIWNDM_CAN_PLAY
            if (not (MCIWNDM_CAN_PLAY == message ? translation.apply("MCIWNDM_CAN_PLAY", "MCI (Media Control Interface) Window", "Can Play") : true))
            return "";
          #endif

          #ifdef MCIWNDM_CAN_RECORD
            if (not (MCIWNDM_CAN_RECORD == message ? translation.apply("MCIWNDM_CAN_RECORD", "MCI (Media Control Interface) Window", "Can Record") : true))
            return "";
          #endif

          #ifdef MCIWNDM_CAN_SAVE
            if (not (MCIWNDM_CAN_SAVE == message ? translation.apply("MCIWNDM_CAN_SAVE", "MCI (Media Control Interface) Window", "Can Save") : true))
            return "";
          #endif

          #ifdef MCIWNDM_CAN_WINDOW
            if (not (MCIWNDM_CAN_WINDOW == message ? translation.apply("MCIWNDM_CAN_WINDOW", "MCI (Media Control Interface) Window", "Can Window") : true))
            return "";
          #endif

          #ifdef MCIWNDM_GET_DEST
            if (not (MCIWNDM_GET_DEST == message ? translation.apply("MCIWNDM_GET_DEST", "MCI (Media Control Interface) Window", "Get Destination") : true))
            return "";
          #endif

          #ifdef MCIWNDM_GET_SOURCE
            if (not (MCIWNDM_GET_SOURCE == message ? translation.apply("MCIWNDM_GET_SOURCE", "MCI (Media Control Interface) Window", "Get Source") : true))
            return "";
          #endif

          #ifdef MCIWNDM_GETDEVICEA
            if (not (MCIWNDM_GETDEVICEA == message ? translation.apply("MCIWNDM_GETDEVICEA", "MCI (Media Control Interface) Window", "Get Device") : true))
            return "";
          #endif

          #ifdef MCIWNDM_GETDEVICEW
            if (not (MCIWNDM_GETDEVICEW == message ? translation.apply("MCIWNDM_GETDEVICEW", "MCI (Media Control Interface) Window", "Get Device") : true))
            return "";
          #endif

          #ifdef MCIWNDM_GETERROR
            if (not (MCIWNDM_GETERROR == message ? translation.apply("MCIWNDM_GETERROR", "MCI (Media Control Interface) Window", "Get Error") : true))
            return "";
          #endif

          #ifdef MCIWNDM_GETERRORA
            if (not (MCIWNDM_GETERRORA == message ? translation.apply("MCIWNDM_GETERRORA", "MCI (Media Control Interface) Window", "Get Error") : true))
            return "";
          #endif

          #ifdef MCIWNDM_GETFILENAMEA
            if (not (MCIWNDM_GETFILENAMEA == message ? translation.apply("MCIWNDM_GETFILENAMEA", "MCI (Media Control Interface) Window", "Get File Name") : true))
            return "";
          #endif

          #ifdef MCIWNDM_GETFILENAMEW
            if (not (MCIWNDM_GETFILENAMEW == message ? translation.apply("MCIWNDM_GETFILENAMEW", "MCI (Media Control Interface) Window", "Get File Name") : true))
            return "";
          #endif

          #ifdef MCIWNDM_GETINACTIVETIMER
            if (not (MCIWNDM_GETINACTIVETIMER == message ? translation.apply("MCIWNDM_GETINACTIVETIMER", "MCI (Media Control Interface) Window", "User") : true))
            return "";
          #endif

          #ifdef MCIWNDM_GETPALETTE
            if (not (MCIWNDM_GETPALETTE == message ? translation.apply("MCIWNDM_GETPALETTE", "MCI (Media Control Interface) Window", "Get Palette") : true))
            return "";
          #endif

          #ifdef MCIWNDM_GETTIMEFORMATA
            if (not (MCIWNDM_GETTIMEFORMATA == message ? translation.apply("MCIWNDM_GETTIMEFORMATA", "MCI (Media Control Interface) Window", "Get Time Format") : true))
            return "";
          #endif

          #ifdef MCIWNDM_NOTIFYERROR
            if (not (MCIWNDM_NOTIFYERROR == message ? translation.apply("MCIWNDM_NOTIFYERROR", "MCI (Media Control Interface) Window", "Notify Error") : true))
            return "";
          #endif

          #ifdef MCIWNDM_NOTIFYMEDIA
            if (not (MCIWNDM_NOTIFYMEDIA == message ? translation.apply("MCIWNDM_NOTIFYMEDIA", "MCI (Media Control Interface) Window", "Notify Media") : true))
            return "";
          #endif

          #ifdef MCIWNDM_NOTIFYMODE
            if (not (MCIWNDM_NOTIFYMODE == message ? translation.apply("MCIWNDM_NOTIFYMODE", "MCI (Media Control Interface) Window", "Notify Mode") : true))
            return "";
          #endif

          #ifdef MCIWNDM_PALETTEKICK
            if (not (MCIWNDM_PALETTEKICK == message ? translation.apply("MCIWNDM_PALETTEKICK", "MCI (Media Control Interface) Window", "Palette Kick") : true))
            return "";
          #endif

          #ifdef MCIWNDM_PLAYTO
            if (not (MCIWNDM_PLAYTO == message ? translation.apply("MCIWNDM_PLAYTO", "MCI (Media Control Interface) Window", "Play To") : true))
            return "";
          #endif

          #ifdef MCIWNDM_PUT_DEST
            if (not (MCIWNDM_PUT_DEST == message ? translation.apply("MCIWNDM_PUT_DEST", "MCI (Media Control Interface) Window", "Put Destination") : true))
            return "";
          #endif

          #ifdef MCIWNDM_PUT_SOURCE
            if (not (MCIWNDM_PUT_SOURCE == message ? translation.apply("MCIWNDM_PUT_SOURCE", "MCI (Media Control Interface) Window", "Put Source") : true))
            return "";
          #endif

          #ifdef MCIWNDM_REALIZE
            if (not (MCIWNDM_REALIZE == message ? translation.apply("MCIWNDM_REALIZE", "MCI (Media Control Interface) Window", "Realize") : true))
            return "";
          #endif

          #ifdef MCIWNDM_SETINACTIVETIMER
            if (not (MCIWNDM_SETINACTIVETIMER == message ? translation.apply("MCIWNDM_SETINACTIVETIMER", "MCI (Media Control Interface) Window", "Set Inactive Timer") : true))
            return "";
          #endif

          #ifdef MCIWNDM_SETPALETTE
            if (not (MCIWNDM_SETPALETTE == message ? translation.apply("MCIWNDM_SETPALETTE", "MCI (Media Control Interface) Window", "Set Palette") : true))
            return "";
          #endif

          #ifdef MCIWNDM_SETTIMEFORMATA
            if (not (MCIWNDM_SETTIMEFORMATA == message ? translation.apply("MCIWNDM_SETTIMEFORMATA", "MCI (Media Control Interface) Window", "Set Time Format") : true))
            return "";
          #endif

          #ifdef MCIWNDM_SETTIMEFORMATW
            if (not (MCIWNDM_SETTIMEFORMATW == message ? translation.apply("MCIWNDM_SETTIMEFORMATW", "MCI (Media Control Interface) Window", "Set Time Format") : true))
            return "";
          #endif

          #ifdef MCIWNDM_VALIDATEMEDIA
            if (not (MCIWNDM_VALIDATEMEDIA == message ? translation.apply("MCIWNDM_VALIDATEMEDIA", "MCI (Media Control Interface) Window", "Validate Media") : true))
            return "";
          #endif

          #ifdef MSG_FTS_JUMP_QWORD
            if (not (MSG_FTS_JUMP_QWORD == message ? translation.apply("MSG_FTS_JUMP_QWORD", "Message Full-Text Search", "Jump Quad-Word") : true))
            return "";
          #endif

          #ifdef MSG_FTS_JUMP_VA
            if (not (MSG_FTS_JUMP_VA == message ? translation.apply("MSG_FTS_JUMP_VA", "Message Full-Text Search", "Jump Variably") : true))
            return "";
          #endif

          #ifdef MSG_FTS_WHERE_IS_IT
            if (not (MSG_FTS_WHERE_IS_IT == message ? translation.apply("MSG_FTS_WHERE_IS_IT", "Message Full-Text Search", "Where Is It") : true))
            return "";
          #endif

          #ifdef MSG_GET_DEFFONT
            if (not (MSG_GET_DEFFONT == message ? translation.apply("MSG_GET_DEFFONT", "Message", "Get Default Font") : true))
            return "";
          #endif

          #ifdef MSG_REINDEX_REQUEST
            if (not (MSG_REINDEX_REQUEST == message ? translation.apply("MSG_REINDEX_REQUEST", "Message", "Re-Index Request") : true))
            return "";
          #endif

          #ifdef NIN_SELECT
            if (not (NIN_SELECT == message ? translation.apply("NIN_SELECT", "Notify Icon", "Select") : true))
            return "";
          #endif

          #ifdef OCM__BASE
            if (not (OCM__BASE == message ? translation.apply("OCM__BASE", "Object Linking & Embedding Control", "Base") : true))
            return "";
          #endif

          #ifdef OCM_CHARTOITEM
            if (not (OCM_CHARTOITEM == message ? translation.apply("OCM_CHARTOITEM", "Object Linking & Embedding Control", "Character To Item") : true))
            return "";
          #endif

          #ifdef OCM_COMMAND
            if (not (OCM_COMMAND == message ? translation.apply("OCM_COMMAND", "Object Linking & Embedding Control", "Command") : true))
            return "";
          #endif

          #ifdef OCM_COMPAREITEM
            if (not (OCM_COMPAREITEM == message ? translation.apply("OCM_COMPAREITEM", "Object Linking & Embedding Control", "Compare Item") : true))
            return "";
          #endif

          #ifdef OCM_CTLCOLOR
            if (not (OCM_CTLCOLOR == message ? translation.apply("OCM_CTLCOLOR", "Object Linking & Embedding Control", "Control Color") : true))
            return "";
          #endif

          #ifdef OCM_CTLCOLORBTN
            if (not (OCM_CTLCOLORBTN == message ? translation.apply("OCM_CTLCOLORBTN", "Object Linking & Embedding Control", "Control Color Button") : true))
            return "";
          #endif

          #ifdef OCM_CTLCOLORDLG
            if (not (OCM_CTLCOLORDLG == message ? translation.apply("OCM_CTLCOLORDLG", "Object Linking & Embedding Control", "Control Color Dialog") : true))
            return "";
          #endif

          #ifdef OCM_CTLCOLOREDIT
            if (not (OCM_CTLCOLOREDIT == message ? translation.apply("OCM_CTLCOLOREDIT", "Object Linking & Embedding Control", "Control Color Edit") : true))
            return "";
          #endif

          #ifdef OCM_CTLCOLORLISTBOX
            if (not (OCM_CTLCOLORLISTBOX == message ? translation.apply("OCM_CTLCOLORLISTBOX", "Object Linking & Embedding Control", "Control Color List Box") : true))
            return "";
          #endif

          #ifdef OCM_CTLCOLORMSGBOX
            if (not (OCM_CTLCOLORMSGBOX == message ? translation.apply("OCM_CTLCOLORMSGBOX", "Object Linking & Embedding Control", "Control Color Message Box") : true))
            return "";
          #endif

          #ifdef OCM_CTLCOLORSCROLLBAR
            if (not (OCM_CTLCOLORSCROLLBAR == message ? translation.apply("OCM_CTLCOLORSCROLLBAR", "Object Linking & Embedding Control", "Control Color Scrollbar") : true))
            return "";
          #endif

          #ifdef OCM_CTLCOLORSTATIC
            if (not (OCM_CTLCOLORSTATIC == message ? translation.apply("OCM_CTLCOLORSTATIC", "Object Linking & Embedding Control", "Control Color Static") : true))
            return "";
          #endif

          #ifdef OCM_DELETEITEM
            if (not (OCM_DELETEITEM == message ? translation.apply("OCM_DELETEITEM", "Object Linking & Embedding Control", "Delete Item") : true))
            return "";
          #endif

          #ifdef OCM_DRAWITEM
            if (not (OCM_DRAWITEM == message ? translation.apply("OCM_DRAWITEM", "Object Linking & Embedding Control", "Draw Item") : true))
            return "";
          #endif

          #ifdef OCM_HSCROLL
            if (not (OCM_HSCROLL == message ? translation.apply("OCM_HSCROLL", "Object Linking & Embedding Control", "Horizontal Scroll") : true))
            return "";
          #endif

          #ifdef OCM_MEASUREITEM
            if (not (OCM_MEASUREITEM == message ? translation.apply("OCM_MEASUREITEM", "Object Linking & Embedding Control", "Measure Item") : true))
            return "";
          #endif

          #ifdef OCM_NOTIFY
            if (not (OCM_NOTIFY == message ? translation.apply("OCM_NOTIFY", "Object Linking & Embedding Control", "Notify") : true))
            return "";
          #endif

          #ifdef OCM_PARENTNOTIFY
            if (not (OCM_PARENTNOTIFY == message ? translation.apply("OCM_PARENTNOTIFY", "Object Linking & Embedding Control", "Parent Notify") : true))
            return "";
          #endif

          #ifdef OCM_VKEYTOITEM
            if (not (OCM_VKEYTOITEM == message ? translation.apply("OCM_VKEYTOITEM", "Object Linking & Embedding Control", "Virtual Key to Item") : true))
            return "";
          #endif

          #ifdef OCM_VSCROLL
            if (not (OCM_VSCROLL == message ? translation.apply("OCM_VSCROLL", "Object Linking & Embedding Control", "Vertical Scroll") : true))
            return "";
          #endif

          #ifdef PBM_DELTAPOS
            if (not (PBM_DELTAPOS == message ? translation.apply("PBM_SETBARCOLOR", "Progress Bar", "Delta Position") : true))
            return "";
          #endif

          #ifdef PBM_GETPOS
            if (not (PBM_GETPOS == message ? translation.apply("PBM_GETPOS", "Progress Bar", "Get Position") : true))
            return "";
          #endif

          #ifdef PBM_GETRANGE
            if (not (PBM_GETRANGE == message ? translation.apply("PBM_GETRANGE", "Progress Bar", "Get Range") : true))
            return "";
          #endif

          #ifdef PBM_SETBARCOLOR
            if (not (PBM_SETBARCOLOR == message ? translation.apply("PBM_SETBARCOLOR", "Progress Bar", "Set Bar Color") : true))
            return "";
          #endif

          #ifdef PBM_SETPOS
            if (not (PBM_SETPOS == message ? translation.apply("PBM_SETPOS", "Progress Bar", "Set Position") : true))
            return "";
          #endif

          #ifdef PBM_SETRANGE
            if (not (PBM_SETRANGE == message ? translation.apply("PBM_SETRANGE", "Progress Bar", "Set Range") : true))
            return "";
          #endif

          #ifdef PBM_SETRANGE32
            if (not (PBM_SETRANGE32 == message ? translation.apply("PBM_SETRANGE32", "Progress Bar", "Set Range") : true))
            return "";
          #endif

          #ifdef PBM_SETSTEP
            if (not (PBM_SETSTEP == message ? translation.apply("PBM_SETSTEP", "Progress Bar", "Set Step") : true))
            return "";
          #endif

          #ifdef PBM_STEPIT
            if (not (PBM_STEPIT == message ? translation.apply("PBM_STEPIT", "Progress Bar", "Step It") : true))
            return "";
          #endif

          #ifdef PSM_ADDPAGE
            if (not (PSM_ADDPAGE == message ? translation.apply("PSM_ADDPAGE", "Prop Sheet", "Add Page") : true))
            return "";
          #endif

          #ifdef PSM_APPLY
            if (not (PSM_APPLY == message ? translation.apply("PSM_APPLY", "Prop Sheet", "Apply") : true))
            return "";
          #endif

          #ifdef PSM_CANCELTOCLOSE
            if (not (PSM_CANCELTOCLOSE == message ? translation.apply("PSM_CANCELTOCLOSE", "Prop Sheet", "Cancel to Close") : true))
            return "";
          #endif

          #ifdef PSM_CHANGED
            if (not (PSM_CHANGED == message ? translation.apply("EM_SETIMECOLOR", "Rich Edit", "Set IME (Input Method Encoding) Color") : true))
            return "";
          #endif

          #ifdef PSM_GETCURRENTPAGEHWND
            if (not (PSM_GETCURRENTPAGEHWND == message ? translation.apply("PSM_GETCURRENTPAGEHWND", "Prop Sheet", "Get Current Page Window Handle") : true))
            return "";
          #endif

          #ifdef PSM_GETRESULT
            if (not (PSM_GETRESULT == message ? translation.apply("PSM_GETRESULT", "Prop Sheet", "Get Result") : true))
            return "";
          #endif

          #ifdef PSM_GETTABCONTROL
            if (not (PSM_GETTABCONTROL == message ? translation.apply("PSM_GETTABCONTROL", "Prop Sheet", "Get Tab Control") : true))
            return "";
          #endif

          #ifdef PSM_HWNDTOINDEX
            if (not (PSM_HWNDTOINDEX == message ? translation.apply("PSM_HWNDTOINDEX", "Prop Sheet", "Window Handle to Index") : true))
            return "";
          #endif

          #ifdef PSM_IDTOINDEX
            if (not (PSM_IDTOINDEX == message ? translation.apply("PSM_IDTOINDEX", "Prop Sheet", "ID to Index") : true))
            return "";
          #endif

          #ifdef PSM_INDEXTOHWND
            if (not (PSM_INDEXTOHWND == message ? translation.apply("PSM_INDEXTOHWND", "Prop Sheet", "Index to Window Handle") : true))
            return "";
          #endif

          #ifdef PSM_INDEXTOID
            if (not (PSM_INDEXTOID == message ? translation.apply("PSM_INDEXTOID", "Prop Sheet", "Index to ID") : true))
            return "";
          #endif

          #ifdef PSM_INDEXTOPAGE
            if (not (PSM_INDEXTOPAGE == message ? translation.apply("PSM_INDEXTOPAGE", "Prop Sheet", "Index to Page") : true))
            return "";
          #endif

          #ifdef PSM_ISDIALOGMESSAGE
            if (not (PSM_ISDIALOGMESSAGE == message ? translation.apply("PSM_ISDIALOGMESSAGE", "Prop Sheet", "Is Dialog Message") : true))
            return "";
          #endif

          #ifdef PSM_PAGETOINDEX
            if (not (PSM_PAGETOINDEX == message ? translation.apply("PSM_PAGETOINDEX", "Prop Sheet", "Insert Page") : true))
            return "";
          #endif

          #ifdef PSM_PAGETOINDEX
            if (not (PSM_PAGETOINDEX == message ? translation.apply("PSM_PAGETOINDEX", "Prop Sheet", "Page to Index") : true))
            return "";
          #endif

          #ifdef PSM_PRESSBUTTON
            if (not (PSM_PRESSBUTTON == message ? translation.apply("PSM_PRESSBUTTON", "Prop Sheet", "Press Button") : true))
            return "";
          #endif

          #ifdef PSM_QUERYSIBLINGS
            if (not (PSM_QUERYSIBLINGS == message ? translation.apply("PSM_QUERYSIBLINGS", "Prop Sheet", "Query Siblings") : true))
            return "";
          #endif

          #ifdef PSM_REBOOTSYSTEM
            if (not (PSM_REBOOTSYSTEM == message ? translation.apply("PSM_REBOOTSYSTEM", "Prop Sheet", "Reboot System") : true))
            return "";
          #endif

          #ifdef PSM_RECALCPAGESIZES
            if (not (PSM_RECALCPAGESIZES == message ? translation.apply("PSM_RECALCPAGESIZES", "Prop Sheet", "Recalculate Page Sizes") : true))
            return "";
          #endif

          #ifdef PSM_REMOVEPAGE
            if (not (PSM_REMOVEPAGE == message ? translation.apply("PSM_REMOVEPAGE", "Prop Sheet", "Remove Page") : true))
            return "";
          #endif

          #ifdef PSM_RESTARTWINDOWS
            if (not (PSM_RESTARTWINDOWS == message ? translation.apply("PSM_RESTARTWINDOWS", "Prop Sheet", "Restart Windows") : true))
            return "";
          #endif

          #ifdef PSM_SETCURSEL
            if (not (PSM_SETCURSEL == message ? translation.apply("PSM_SETCURSEL", "Prop Sheet", "Set Cursor Selection") : true))
            return "";
          #endif

          #ifdef PSM_SETCURSELID
            if (not (PSM_SETCURSELID == message ? translation.apply("PSM_SETCURSELID", "Prop Sheet", "Set Current Selection ID") : true))
            return "";
          #endif

          #ifdef PSM_SETFINISHTEXTA
            if (not (PSM_SETFINISHTEXTA == message ? translation.apply("PSM_SETFINISHTEXTA", "Prop Sheet", "Set Finish Text") : true))
            return "";
          #endif

          #ifdef PSM_SETFINISHTEXTA
            if (not (PSM_SETFINISHTEXTA == message ? translation.apply("PSM_SETFINISHTEXTA", "Prop Sheet", "Set Finish Text") : true))
            return "";
          #endif

          #ifdef PSM_SETHEADERSUBTITLEA
            if (not (PSM_SETHEADERSUBTITLEA == message ? translation.apply("PSM_SETHEADERSUBTITLEA", "Prop Sheet", "Set Header Subtitle") : true))
            return "";
          #endif

          #ifdef PSM_SETHEADERSUBTITLEW
            if (not (PSM_SETHEADERSUBTITLEW == message ? translation.apply("PSM_SETHEADERSUBTITLEW", "Prop Sheet", "Set Header Subtitle") : true))
            return "";
          #endif

          #ifdef PSM_SETHEADERTITLEA
            if (not (PSM_SETHEADERTITLEA == message ? translation.apply("PSM_SETHEADERTITLEA", "Prop Sheet", "Set Header Title") : true))
            return "";
          #endif

          #ifdef PSM_SETHEADERTITLEW
            if (not (PSM_SETHEADERTITLEW == message ? translation.apply("PSM_SETHEADERTITLEW", "Prop Sheet", "Set Header Title") : true))
            return "";
          #endif

          #ifdef PSM_SETTITLEA
            if (not (PSM_SETTITLEA == message ? translation.apply("PSM_SETTITLEA", "Prop Sheet", "Set Title") : true))
            return "";
          #endif

          #ifdef PSM_SETTITLEW
            if (not (PSM_SETTITLEW == message ? translation.apply("PSM_SETTITLEW", "Prop Sheet", "Set Title") : true))
            return "";
          #endif

          #ifdef PSM_SETWIZBUTTONS
            if (not (PSM_SETWIZBUTTONS == message ? translation.apply("PSM_SETWIZBUTTONS", "Prop Sheet", "Set Wizard Buttons") : true))
            return "";
          #endif

          #ifdef PSM_UNCHANGED
            if (not (PSM_UNCHANGED == message ? translation.apply("PSM_UNCHANGED", "Prop Sheet", "Unchanged") : true))
            return "";
          #endif

          #ifdef RB_BEGINDRAG
            if (not (RB_BEGINDRAG == message ? translation.apply("RB_BEGINDRAG", "Rebar", "Begin Drag") : true))
            return "";
          #endif

          #ifdef RB_DELETEBAND
            if (not (RB_DELETEBAND == message ? translation.apply("RB_DELETEBAND", "Rebar", "Delete Band") : true))
            return "";
          #endif

          #ifdef RB_DRAGMOVE
            if (not (RB_DRAGMOVE == message ? translation.apply("RB_DRAGMOVE", "Rebar", "Drag Move") : true))
            return "";
          #endif

          #ifdef RB_ENDDRAG
            if (not (RB_ENDDRAG == message ? translation.apply("RB_ENDDRAG", "Rebar", "End Drag") : true))
            return "";
          #endif

          #ifdef RB_GETBANDBORDERS
            if (not (RB_GETBANDBORDERS == message ? translation.apply("RB_GETBANDBORDERS", "Rebar", "Get Band Borders") : true))
            return "";
          #endif

          #ifdef RB_GETBANDCOUNT
            if (not (RB_GETBANDCOUNT == message ? translation.apply("RB_GETBANDCOUNT", "Rebar", "Get Band Count") : true))
            return "";
          #endif

          #ifdef RB_GETBANDINFOA
            if (not (RB_GETBANDINFOA == message ? translation.apply("RB_GETBANDINFOA", "Rebar", "Get Band Information") : true))
            return "";
          #endif

          #ifdef RB_GETBANDINFOW
            if (not (RB_GETBANDINFOW == message ? translation.apply("RB_GETBANDINFOW", "Rebar", "Get Band Information") : true))
            return "";
          #endif

          #ifdef RB_GETBARHEIGHT
            if (not (RB_GETBARHEIGHT == message ? translation.apply("RB_GETBARHEIGHT", "Rebar", "Get Bar Height") : true))
            return "";
          #endif

          #ifdef RB_GETBARINFO
            if (not (RB_GETBARINFO == message ? translation.apply("RB_GETBARINFO", "Rebar", "Get Bar Information") : true))
            return "";
          #endif

          #ifdef RB_GETBKCOLOR
            if (not (RB_GETBKCOLOR == message ? translation.apply("RB_GETBKCOLOR", "Rebar", "Get Background Color") : true))
            return "";
          #endif

          #ifdef RB_GETPALETTE
            if (not (RB_GETPALETTE == message ? translation.apply("RB_GETPALETTE", "Rebar", "Get Palette") : true))
            return "";
          #endif

          #ifdef RB_GETRECT
            if (not (RB_GETRECT == message ? translation.apply("RB_GETRECT", "Rebar", "Get Rectangle") : true))
            return "";
          #endif

          #ifdef RB_GETROWCOUNT
            if (not (RB_GETROWCOUNT == message ? translation.apply("RB_GETROWCOUNT", "Rebar", "Get Row Count") : true))
            return "";
          #endif

          #ifdef RB_GETROWHEIGHT
            if (not (RB_GETROWHEIGHT == message ? translation.apply("RB_GETROWHEIGHT", "Rebar", "Get Row Height") : true))
            return "";
          #endif

          #ifdef RB_GETTEXTCOLOR
            if (not (RB_GETTEXTCOLOR == message ? translation.apply("RB_GETTEXTCOLOR", "Rebar", "Get Text Color") : true))
            return "";
          #endif

          #ifdef RB_GETTOOLTIPS
            if (not (RB_GETTOOLTIPS == message ? translation.apply("RB_GETTOOLTIPS", "Rebar", "Get Tooltips") : true))
            return "";
          #endif

          #ifdef RB_HITTEST
            if (not (RB_HITTEST == message ? translation.apply("RB_HITTEST", "Rebar", "Hit Test") : true))
            return "";
          #endif

          #ifdef RB_IDTOINDEX
            if (not (RB_IDTOINDEX == message ? translation.apply("RB_IDTOINDEX", "Rebar", "ID to Index") : true))
            return "";
          #endif

          #ifdef RB_INSERTBANDA
            if (not (RB_INSERTBANDA == message ? translation.apply("RB_INSERTBANDA", "Rebar", "Insert Band") : true))
            return "";
          #endif

          #ifdef RB_INSERTBANDW
            if (not (RB_INSERTBANDW == message ? translation.apply("RB_INSERTBANDW", "Rebar", "Insert Band") : true))
            return "";
          #endif

          #ifdef RB_MAXIMIZEBAND
            if (not (RB_MAXIMIZEBAND == message ? translation.apply("RB_MAXIMIZEBAND", "Rebar", "Maximize Band") : true))
            return "";
          #endif

          #ifdef RB_MINIMIZEBAND
            if (not (RB_MINIMIZEBAND == message ? translation.apply("RB_MINIMIZEBAND", "Rebar", "Minimize Band") : true))
            return "";
          #endif

          #ifdef RB_MOVEBAND
            if (not (RB_MOVEBAND == message ? translation.apply("RB_MOVEBAND", "Rebar", "Move Band") : true))
            return "";
          #endif

          #ifdef RB_PUSHCHEVRON
            if (not (RB_PUSHCHEVRON == message ? translation.apply("RB_PUSHCHEVRON", "Rebar", "Push Chevron") : true))
            return "";
          #endif

          #ifdef RB_SETBANDINFOA
            if (not (RB_SETBANDINFOA == message ? translation.apply("RB_SETBANDINFOA", "Rebar", "Set Band Information") : true))
            return "";
          #endif

          #ifdef RB_SETBANDINFOW
            if (not (RB_SETBANDINFOW == message ? translation.apply("RB_SETBANDINFOW", "Rebar", "Set Band Information") : true))
            return "";
          #endif

          #ifdef RB_SETBARINFO
            if (not (RB_SETBARINFO == message ? translation.apply("RB_SETBARINFO", "Rebar", "Set Bar Information") : true))
            return "";
          #endif

          #ifdef RB_SETBKCOLOR
            if (not (RB_SETBKCOLOR == message ? translation.apply("RB_SETBKCOLOR", "Rebar", "Set Background Color") : true))
            return "";
          #endif

          #ifdef RB_SETPALETTE
            if (not (RB_SETPALETTE == message ? translation.apply("RB_SETPALETTE", "Rebar", "Set Palette") : true))
            return "";
          #endif

          #ifdef RB_SETPARENT
            if (not (RB_SETPARENT == message ? translation.apply("RB_SETPARENT", "Rebar", "Set Parent") : true))
            return "";
          #endif

          #ifdef RB_SETTEXTCOLOR
            if (not (RB_SETTEXTCOLOR == message ? translation.apply("RB_SETTEXTCOLOR", "Rebar", "Set Text Color") : true))
            return "";
          #endif

          #ifdef RB_SETTOOLTIPS
            if (not (RB_SETTOOLTIPS == message ? translation.apply("RB_SETTOOLTIPS", "Rebar", "Set Tooltips") : true))
            return "";
          #endif

          #ifdef RB_SHOWBAND
            if (not (RB_SHOWBAND == message ? translation.apply("RB_SHOWBAND", "Rebar", "Show Band") : true))
            return "";
          #endif

          #ifdef RB_SIZETORECT
            if (not (RB_SIZETORECT == message ? translation.apply("RB_SIZETORECT", "Rebar", "Size to Rectangle") : true))
            return "";
          #endif

          #ifdef SB_GETBORDERS
            if (not (SB_GETBORDERS == message ? translation.apply("SB_GETBORDERS", "Status Bar", "Get Borders") : true))
            return "";
          #endif

          #ifdef SB_GETPARTS
            if (not (SB_GETPARTS == message ? translation.apply("SB_GETPARTS", "Status Bar", "Get Parts") : true))
            return "";
          #endif

          #ifdef SB_GETRECT
            if (not (SB_GETRECT == message ? translation.apply("SB_GETRECT", "Status Bar", "Get Rectangle") : true))
            return "";
          #endif

          #ifdef SB_GETTEXTA
            if (not (SB_GETTEXTA == message ? translation.apply("SB_GETTEXTA", "Status Bar", "Get Text") : true))
            return "";
          #endif

          #ifdef SB_GETTEXTLENGTHA
            if (not (SB_GETTEXTLENGTHA == message ? translation.apply("SB_GETTEXTLENGTHA", "Status Bar", "Get Text Length") : true))
            return "";
          #endif

          #ifdef SB_GETTEXTLENGTHW
            if (not (SB_GETTEXTLENGTHW == message ? translation.apply("SB_GETTEXTLENGTHW", "Status Bar", "Get Text Length") : true))
            return "";
          #endif

          #ifdef SB_GETTEXTW
            if (not (SB_GETTEXTW == message ? translation.apply("SB_GETTEXTW", "Status Bar", "Get Text") : true))
            return "";
          #endif

          #ifdef SB_GETTIPTEXTA
            if (not (SB_GETTIPTEXTA == message ? translation.apply("SB_GETTIPTEXTA", "Status Bar", "Get Tip Text") : true))
            return "";
          #endif

          #ifdef SB_GETTIPTEXTW
            if (not (SB_GETTIPTEXTW == message ? translation.apply("SB_GETTIPTEXTW", "Status Bar", "Get Tip Text") : true))
            return "";
          #endif

          #ifdef SB_ISSIMPLE
            if (not (SB_ISSIMPLE == message ? translation.apply("SB_ISSIMPLE", "Status Bar", "Is Simple") : true))
            return "";
          #endif

          #ifdef SB_SETICON
            if (not (SB_SETICON == message ? translation.apply("SB_SETICON", "Status Bar", "Set Icon") : true))
            return "";
          #endif

          #ifdef SB_SETMINHEIGHT
            if (not (SB_SETMINHEIGHT == message ? translation.apply("SB_SETMINHEIGHT", "Status Bar", "Set Minimum Height") : true))
            return "";
          #endif

          #ifdef SB_SETPARTS
            if (not (SB_SETPARTS == message ? translation.apply("SB_SETPARTS", "Status Bar", "Set Parts") : true))
            return "";
          #endif

          #ifdef SB_SETTEXTW
            if (not (SB_SETTEXTW == message ? translation.apply("SB_SETTEXTW", "Status Bar", "Set Text") : true))
            return "";
          #endif

          #ifdef SB_SETTIPTEXTA
            if (not (SB_SETTIPTEXTA == message ? translation.apply("SB_SETTIPTEXTA", "Status Bar", "Set Tip Text") : true))
            return "";
          #endif

          #ifdef SB_SETTIPTEXTW
            if (not (SB_SETTIPTEXTW == message ? translation.apply("SB_SETTIPTEXTW", "Status Bar", "Set Tip Text") : true))
            return "";
          #endif

          #ifdef SB_SIMPLE
            if (not (SB_SIMPLE == message ? translation.apply("SB_SIMPLE", "Status Bar", "Simple") : true))
            return "";
          #endif

          #ifdef SBM_ENABLE_ARROWS
            if (not (SBM_ENABLE_ARROWS == message ? translation.apply("SBM_ENABLE_ARROWS", "Scrollbar", "Enable Arrows") : true))
            return "";
          #endif

          #ifdef SBM_GETPOS
            if (not (SBM_GETPOS == message ? translation.apply("SBM_GETPOS", "Scrollbar", "Get Position") : true))
            return "";
          #endif

          #ifdef SBM_GETRANGE
            if (not (SBM_GETRANGE == message ? translation.apply("SBM_GETRANGE", "Scrollbar", "Get Range") : true))
            return "";
          #endif

          #ifdef SBM_GETSCROLLINFO
            if (not (SBM_GETSCROLLINFO == message ? translation.apply("SBM_GETSCROLLINFO", "Scrollbar", "Get Scroll Information") : true))
            return "";
          #endif

          #ifdef SBM_SETPOS
            if (not (SBM_SETPOS == message ? translation.apply("SBM_SETPOS", "Scrollbar", "Set Position") : true))
            return "";
          #endif

          #ifdef SBM_SETRANGE
            if (not (SBM_SETRANGE == message ? translation.apply("SBM_SETRANGE", "Scrollbar", "Set Range") : true))
            return "";
          #endif

          #ifdef SBM_SETRANGEREDRAW
            if (not (SBM_SETRANGEREDRAW == message ? translation.apply("SBM_SETRANGEREDRAW", "Scrollbar", "Set Range Redraw") : true))
            return "";
          #endif

          #ifdef SBM_SETSCROLLINFO
            if (not (SBM_SETSCROLLINFO == message ? translation.apply("SBM_SETSCROLLINFO", "Scrollbar", "Set Scroll Information") : true))
            return "";
          #endif

          #ifdef SM_GETCURFOCUSA
            if (not (SM_GETCURFOCUSA == message ? translation.apply("SM_GETCURFOCUSA", "Storage Management", "Get Current Focus") : true))
            return "";
          #endif

          #ifdef SM_GETCURFOCUSW
            if (not (SM_GETCURFOCUSW == message ? translation.apply("SM_GETCURFOCUSW", "Storage Management", "Get Current Focus") : true))
            return "";
          #endif

          #ifdef SM_GETOPTIONS
            if (not (SM_GETOPTIONS == message ? translation.apply("SM_GETOPTIONS", "Storage Management", "Get Options") : true))
            return "";
          #endif

          #ifdef SM_GETSELCOUNT
            if (not (SM_GETSELCOUNT == message ? translation.apply("SM_GETSELCOUNT", "Storage Management", "Get Selection Count") : true))
            return "";
          #endif

          #ifdef SM_GETSERVERSELA
            if (not (SM_GETSERVERSELA == message ? translation.apply("SM_GETSERVERSELA", "Storage Management", "Get Server Selection") : true))
            return "";
          #endif

          #ifdef SM_GETSERVERSELW
            if (not (SM_GETSERVERSELW == message ? translation.apply("SM_GETSERVERSELW", "Storage Management", "Get Server Selection") : true))
            return "";
          #endif

          #ifdef TAPI_REPLY
            if (not (TAPI_REPLY == message ? translation.apply("TAPI_REPLY", "Telephony API", "Reply") : true))
            return "";
          #endif

          #ifdef TB_ADDBITMAP
            if (not (TB_ADDBITMAP == message ? translation.apply("TB_ADDBITMAP", "Toolbar", "Add Bitmap") : true))
            return "";
          #endif

          #ifdef TB_ADDBUTTONSA
            if (not (TB_ADDBUTTONSA == message ? translation.apply("TB_ADDBUTTONSA", "Toolbar", "Add Buttons") : true))
            return "";
          #endif

          #ifdef TB_ADDBUTTONSW
            if (not (TB_ADDBUTTONSW == message ? translation.apply("TB_ADDBUTTONSW", "Toolbar", "Add Buttons") : true))
            return "";
          #endif

          #ifdef TB_ADDSTRINGA
            if (not (TB_ADDSTRINGA == message ? translation.apply("TB_ADDSTRINGA", "Toolbar", "Add String") : true))
            return "";
          #endif

          #ifdef TB_ADDSTRINGW
            if (not (TB_ADDSTRINGW == message ? translation.apply("TB_ADDSTRINGW", "Toolbar", "Add String") : true))
            return "";
          #endif

          #ifdef TB_AUTOSIZE
            if (not (TB_AUTOSIZE == message ? translation.apply("TB_AUTOSIZE", "Toolbar", "Auto Size") : true))
            return "";
          #endif

          #ifdef TB_BUTTONCOUNT
            if (not (TB_BUTTONCOUNT == message ? translation.apply("TB_BUTTONCOUNT", "Toolbar", "Button Count") : true))
            return "";
          #endif

          #ifdef TB_BUTTONSTRUCTSIZE
            if (not (TB_BUTTONSTRUCTSIZE == message ? translation.apply("TB_BUTTONSTRUCTSIZE", "Toolbar", "Button Structure Size") : true))
            return "";
          #endif

          #ifdef TB_CHANGEBITMAP
            if (not (TB_CHANGEBITMAP == message ? translation.apply("TB_CHANGEBITMAP", "Toolbar", "Change Bitmap") : true))
            return "";
          #endif

          #ifdef TB_CHECKBUTTON
            if (not (TB_CHECKBUTTON == message ? translation.apply("TB_CHECKBUTTON", "Toolbar", "Check Button") : true))
            return "";
          #endif

          #ifdef TB_COMMANDTOINDEX
            if (not (TB_COMMANDTOINDEX == message ? translation.apply("TB_COMMANDTOINDEX", "Toolbar", "Command to Index") : true))
            return "";
          #endif

          #ifdef TB_CUSTOMIZE
            if (not (TB_CUSTOMIZE == message ? translation.apply("TB_CUSTOMIZE", "Toolbar", "Customize") : true))
            return "";
          #endif

          #ifdef TB_DELETEBUTTON
            if (not (TB_DELETEBUTTON == message ? translation.apply("TB_DELETEBUTTON", "Toolbar", "Delete Button") : true))
            return "";
          #endif

          #ifdef TB_ENABLEBUTTON
            if (not (TB_ENABLEBUTTON == message ? translation.apply("TB_ENABLEBUTTON", "Toolbar", "Enable Button") : true))
            return "";
          #endif

          #ifdef TB_GETANCHORHIGHLIGHT
            if (not (TB_GETANCHORHIGHLIGHT == message ? translation.apply("TB_GETANCHORHIGHLIGHT", "Toolbar", "Get Anchor Highlight") : true))
            return "";
          #endif

          #ifdef TB_GETBITMAP
            if (not (TB_GETBITMAP == message ? translation.apply("TB_GETBITMAP", "Toolbar", "Get Bitmap") : true))
            return "";
          #endif

          #ifdef TB_GETBITMAPFLAGS
            if (not (TB_GETBITMAPFLAGS == message ? translation.apply("TB_GETBITMAPFLAGS", "Toolbar", "Get Bitmap Flags") : true))
            return "";
          #endif

          #ifdef TB_GETBUTTON
            if (not (TB_GETBUTTON == message ? translation.apply("TB_GETBUTTON", "Toolbar", "Get Button") : true))
            return "";
          #endif

          #ifdef TB_GETBUTTONINFOA
            if (not (TB_GETBUTTONINFOA == message ? translation.apply("TB_GETBUTTONINFOA", "Toolbar", "Get Button Information") : true))
            return "";
          #endif

          #ifdef TB_GETBUTTONINFOW
            if (not (TB_GETBUTTONINFOW == message ? translation.apply("TB_GETBUTTONINFOW", "Toolbar", "Get Button Information") : true))
            return "";
          #endif

          #ifdef TB_GETBUTTONSIZE
            if (not (TB_GETBUTTONSIZE == message ? translation.apply("TB_GETBUTTONSIZE", "Toolbar", "Get Button Size") : true))
            return "";
          #endif

          #ifdef TB_GETBUTTONTEXTA
            if (not (TB_GETBUTTONTEXTA == message ? translation.apply("TB_GETBUTTONTEXTA", "Toolbar", "Get Button Text") : true))
            return "";
          #endif

          #ifdef TB_GETBUTTONTEXTW
            if (not (TB_GETBUTTONTEXTW == message ? translation.apply("TB_GETBUTTONTEXTW", "Toolbar", "Get Button Text") : true))
            return "";
          #endif

          #ifdef TB_GETDISABLEDIMAGELIST
            if (not (TB_GETDISABLEDIMAGELIST == message ? translation.apply("TB_GETDISABLEDIMAGELIST", "Toolbar", "Get Disabled Image List") : true))
            return "";
          #endif

          #ifdef TB_GETEXTENDEDSTYLE
            if (not (TB_GETEXTENDEDSTYLE == message ? translation.apply("TB_GETEXTENDEDSTYLE", "Toolbar", "Get Extended Style") : true))
            return "";
          #endif

          #ifdef TB_GETHOTIMAGELIST
            if (not (TB_GETHOTIMAGELIST == message ? translation.apply("TB_GETHOTIMAGELIST", "Toolbar", "Get Hot Image List") : true))
            return "";
          #endif

          #ifdef TB_GETHOTITEM
            if (not (TB_GETHOTITEM == message ? translation.apply("TB_GETHOTITEM", "Toolbar", "Get Hot Item") : true))
            return "";
          #endif

          #ifdef TB_GETIMAGELIST
            if (not (TB_GETIMAGELIST == message ? translation.apply("TB_GETIMAGELIST", "Toolbar", "Get Image List") : true))
            return "";
          #endif

          #ifdef TB_GETINSERTMARK
            if (not (TB_GETINSERTMARK == message ? translation.apply("TB_GETINSERTMARK", "Toolbar", "Get Insert Mark") : true))
            return "";
          #endif

          #ifdef TB_GETINSERTMARKCOLOR
            if (not (TB_GETINSERTMARKCOLOR == message ? translation.apply("TB_GETINSERTMARKCOLOR", "Toolbar", "Get Insert Mark Color") : true))
            return "";
          #endif

          #ifdef TB_GETITEMRECT
            if (not (TB_GETITEMRECT == message ? translation.apply("TB_GETITEMRECT", "Toolbar", "Get Item Rectangle") : true))
            return "";
          #endif

          #ifdef TB_GETMAXSIZE
            if (not (TB_GETMAXSIZE == message ? translation.apply("TB_GETMAXSIZE", "Toolbar", "Get Maximum Size") : true))
            return "";
          #endif

          #ifdef TB_GETOBJECT
            if (not (TB_GETOBJECT == message ? translation.apply("TB_GETOBJECT", "Toolbar", "Get Object") : true))
            return "";
          #endif

          #ifdef TB_GETPADDING
            if (not (TB_GETPADDING == message ? translation.apply("TB_GETPADDING", "Toolbar", "Get Padding") : true))
            return "";
          #endif

          #ifdef TB_GETRECT
            if (not (TB_GETRECT == message ? translation.apply("TB_GETRECT", "Toolbar", "Get Rectangle") : true))
            return "";
          #endif

          #ifdef TB_GETROWS
            if (not (TB_GETROWS == message ? translation.apply("TB_GETROWS", "Toolbar", "Get Rows") : true))
            return "";
          #endif

          #ifdef TB_GETSTATE
            if (not (TB_GETSTATE == message ? translation.apply("TB_GETSTATE", "Toolbar", "Get State") : true))
            return "";
          #endif

          #ifdef TB_GETSTRINGA
            if (not (TB_GETSTRINGA == message ? translation.apply("TB_GETSTRINGA", "Toolbar", "Get String") : true))
            return "";
          #endif

          #ifdef TB_GETSTRINGW
            if (not (TB_GETSTRINGW == message ? translation.apply("TB_GETSTRINGW", "Toolbar", "Get String") : true))
            return "";
          #endif

          #ifdef TB_GETSTYLE
            if (not (TB_GETSTYLE == message ? translation.apply("TB_GETSTYLE", "Toolbar", "Get Style") : true))
            return "";
          #endif

          #ifdef TB_GETTEXTROWS
            if (not (TB_GETTEXTROWS == message ? translation.apply("TB_GETTEXTROWS", "Toolbar", "Get Text Rows") : true))
            return "";
          #endif

          #ifdef TB_GETTOOLTIPS
            if (not (TB_GETTOOLTIPS == message ? translation.apply("TB_GETTOOLTIPS", "Toolbar", "Get Tooltips") : true))
            return "";
          #endif

          #ifdef TB_HIDEBUTTON
            if (not (TB_HIDEBUTTON == message ? translation.apply("TB_HIDEBUTTON", "Toolbar", "Hide Button") : true))
            return "";
          #endif

          #ifdef TB_HITTEST
            if (not (TB_HITTEST == message ? translation.apply("TB_HITTEST", "Toolbar", "Hit Test") : true))
            return "";
          #endif

          #ifdef TB_INDETERMINATE
            if (not (TB_INDETERMINATE == message ? translation.apply("TB_INDETERMINATE", "Toolbar", "Indeterminate") : true))
            return "";
          #endif

          #ifdef TB_INSERTBUTTONA
            if (not (TB_INSERTBUTTONA == message ? translation.apply("TB_INSERTBUTTONA", "Toolbar", "Insert Button") : true))
            return "";
          #endif

          #ifdef TB_INSERTBUTTONW
            if (not (TB_INSERTBUTTONW == message ? translation.apply("TB_INSERTBUTTONW", "Toolbar", "Insert Button") : true))
            return "";
          #endif

          #ifdef TB_INSERTMARKHITTEST
            if (not (TB_INSERTMARKHITTEST == message ? translation.apply("TB_INSERTMARKHITTEST", "Toolbar", "Insert Mark Hit Test") : true))
            return "";
          #endif

          #ifdef TB_ISBUTTONCHECKED
            if (not (TB_ISBUTTONCHECKED == message ? translation.apply("TB_ISBUTTONCHECKED", "Toolbar", "Is Button Checked") : true))
            return "";
          #endif

          #ifdef TB_ISBUTTONENABLED
            if (not (TB_ISBUTTONENABLED == message ? translation.apply("TB_ISBUTTONENABLED", "Toolbar", "Is Button Enabled") : true))
            return "";
          #endif

          #ifdef TB_ISBUTTONHIDDEN
            if (not (TB_ISBUTTONHIDDEN == message ? translation.apply("TB_ISBUTTONHIDDEN", "Toolbar", "Is Button Hidden") : true))
            return "";
          #endif

          #ifdef TB_ISBUTTONHIGHLIGHTED
            if (not (TB_ISBUTTONHIGHLIGHTED == message ? translation.apply("TB_ISBUTTONHIGHLIGHTED", "Toolbar", "Is Button Highlighted") : true))
            return "";
          #endif

          #ifdef TB_ISBUTTONINDETERMINATE
            if (not (TB_ISBUTTONINDETERMINATE == message ? translation.apply("TB_ISBUTTONINDETERMINATE", "Toolbar", "Is Button Indeterminate") : true))
            return "";
          #endif

          #ifdef TB_ISBUTTONPRESSED
            if (not (TB_ISBUTTONPRESSED == message ? translation.apply("TB_ISBUTTONPRESSED", "Toolbar", "Is Button Pressed") : true))
            return "";
          #endif

          #ifdef TB_LOADIMAGES
            if (not (TB_LOADIMAGES == message ? translation.apply("TB_LOADIMAGES", "Toolbar", "Load Images") : true))
            return "";
          #endif

          #ifdef TB_MAPACCELERATORA
            if (not (TB_MAPACCELERATORA == message ? translation.apply("TB_MAPACCELERATORA", "Toolbar", "Map Accelerator") : true))
            return "";
          #endif

          #ifdef TB_MAPACCELERATORW
            if (not (TB_MAPACCELERATORW == message ? translation.apply("TB_MAPACCELERATORW", "Toolbar", "Map Acceleration") : true))
            return "";
          #endif

          #ifdef TB_MARKBUTTON
            if (not (TB_MARKBUTTON == message ? translation.apply("TB_MARKBUTTON", "Toolbar", "Mark Button") : true))
            return "";
          #endif

          #ifdef TB_MOVEBUTTON
            if (not (TB_MOVEBUTTON == message ? translation.apply("TB_MOVEBUTTON", "Toolbar", "Move Button") : true))
            return "";
          #endif

          #ifdef TB_PRESSBUTTON
            if (not (TB_PRESSBUTTON == message ? translation.apply("TB_PRESSBUTTON", "Toolbar", "Press Button") : true))
            return "";
          #endif

          #ifdef TB_REPLACEBITMAP
            if (not (TB_REPLACEBITMAP == message ? translation.apply("TB_REPLACEBITMAP", "Toolbar", "Replace Bitmap") : true))
            return "";
          #endif

          #ifdef TB_SAVERESTOREA
            if (not (TB_SAVERESTOREA == message ? translation.apply("TB_SAVERESTOREA", "Toolbar", "Save Restore") : true))
            return "";
          #endif

          #ifdef TB_SAVERESTOREW
            if (not (TB_SAVERESTOREW == message ? translation.apply("TB_SAVERESTOREW", "Toolbar", "Save Restore") : true))
            return "";
          #endif

          #ifdef TB_SETANCHORHIGHLIGHT
            if (not (TB_SETANCHORHIGHLIGHT == message ? translation.apply("TB_SETANCHORHIGHLIGHT", "Toolbar", "Set Anchor Highlight") : true))
            return "";
          #endif

          #ifdef TB_SETBITMAPSIZE
            if (not (TB_SETBITMAPSIZE == message ? translation.apply("TB_SETBITMAPSIZE", "Toolbar", "Set Bitmap Size") : true))
            return "";
          #endif

          #ifdef TB_SETBUTTONINFOA
            if (not (TB_SETBUTTONINFOA == message ? translation.apply("TB_SETBUTTONINFOA", "Toolbar", "Set Button Information") : true))
            return "";
          #endif

          #ifdef TB_SETBUTTONINFOW
            if (not (TB_SETBUTTONINFOW == message ? translation.apply("TB_SETBUTTONINFOW", "Toolbar", "Set Button Information") : true))
            return "";
          #endif

          #ifdef TB_SETBUTTONSIZE
            if (not (TB_SETBUTTONSIZE == message ? translation.apply("TB_SETBUTTONSIZE", "Toolbar", "Set Button Size") : true))
            return "";
          #endif

          #ifdef TB_SETBUTTONWIDTH
            if (not (TB_SETBUTTONWIDTH == message ? translation.apply("TB_SETBUTTONWIDTH", "Toolbar", "Set Button Width") : true))
            return "";
          #endif

          #ifdef TB_SETCMDID
            if (not (TB_SETCMDID == message ? translation.apply("TB_SETCMDID", "Toolbar", "Set Command Identifier") : true))
            return "";
          #endif

          #ifdef TB_SETDISABLEDIMAGELIST
            if (not (TB_SETDISABLEDIMAGELIST == message ? translation.apply("TB_SETDISABLEDIMAGELIST", "Toolbar", "Set Disabled Image List") : true))
            return "";
          #endif

          #ifdef TB_SETDRAWTEXTFLAGS
            if (not (TB_SETDRAWTEXTFLAGS == message ? translation.apply("TB_SETDRAWTEXTFLAGS", "Toolbar", "Set Draw Text Flags") : true))
            return "";
          #endif

          #ifdef TB_SETEXTENDEDSTYLE
            if (not (TB_SETEXTENDEDSTYLE == message ? translation.apply("TB_SETEXTENDEDSTYLE", "Toolbar", "Set Extended Style") : true))
            return "";
          #endif

          #ifdef TB_SETHOTIMAGELIST
            if (not (TB_SETHOTIMAGELIST == message ? translation.apply("TB_SETHOTIMAGELIST", "Toolbar", "Set Hot Image List") : true))
            return "";
          #endif

          #ifdef TB_SETHOTITEM
            if (not (TB_SETHOTITEM == message ? translation.apply("TB_SETHOTITEM", "Toolbar", "Set Hot Item") : true))
            return "";
          #endif

          #ifdef TB_SETIMAGELIST
            if (not (TB_SETIMAGELIST == message ? translation.apply("TB_SETIMAGELIST", "Toolbar", "Set Image List") : true))
            return "";
          #endif

          #ifdef TB_SETINDENT
            if (not (TB_SETINDENT == message ? translation.apply("TB_SETINDENT", "Toolbar", "Set Indentation") : true))
            return "";
          #endif

          #ifdef TB_SETINSERTMARK
            if (not (TB_SETINSERTMARK == message ? translation.apply("TB_SETINSERTMARK", "Toolbar", "Set Insert Mark") : true))
            return "";
          #endif

          #ifdef TB_SETINSERTMARKCOLOR
            if (not (TB_SETINSERTMARKCOLOR == message ? translation.apply("TB_SETINSERTMARKCOLOR", "Toolbar", "Set Insert Mark Color") : true))
            return "";
          #endif

          #ifdef TB_SETMAXTEXTROWS
            if (not (TB_SETMAXTEXTROWS == message ? translation.apply("TB_SETMAXTEXTROWS", "Toolbar", "Set Maximum Text Rows") : true))
            return "";
          #endif

          #ifdef TB_SETPADDING
            if (not (TB_SETPADDING == message ? translation.apply("TB_SETPADDING", "Toolbar", "Set Padding") : true))
            return "";
          #endif

          #ifdef TB_SETPARENT
            if (not (TB_SETPARENT == message ? translation.apply("TB_SETPARENT", "Toolbar", "Set Parent") : true))
            return "";
          #endif

          #ifdef TB_SETROWS
            if (not (TB_SETROWS == message ? translation.apply("TB_SETROWS", "Toolbar", "Set Rows") : true))
            return "";
          #endif

          #ifdef TB_SETSTATE
            if (not (TB_SETSTATE == message ? translation.apply("TB_SETSTATE", "Toolbar", "Set State") : true))
            return "";
          #endif

          #ifdef TB_SETSTYLE
            if (not (TB_SETSTYLE == message ? translation.apply("TB_SETSTYLE", "Toolbar", "Set Style") : true))
            return "";
          #endif

          #ifdef TB_SETTOOLTIPS
            if (not (TB_SETTOOLTIPS == message ? translation.apply("TB_SETTOOLTIPS", "Toolbar", "Set Tooltips") : true))
            return "";
          #endif

          #ifdef TBM_CLEARSEL
            if (not (TBM_CLEARSEL == message ? translation.apply("TBM_CLEARSEL", "Trackbar", "Clear Selection") : true))
            return "";
          #endif

          #ifdef TBM_CLEARTICS
            if (not (TBM_CLEARTICS == message ? translation.apply("TBM_CLEARTICS", "Trackbar", "Clear Ticks") : true))
            return "";
          #endif

          #ifdef TBM_GETBUDDY
            if (not (TBM_GETBUDDY == message ? translation.apply("TBM_GETBUDDY", "Trackbar", "Get Buddy") : true))
            return "";
          #endif

          #ifdef TBM_GETCHANNELRECT
            if (not (TBM_GETCHANNELRECT == message ? translation.apply("TBM_GETCHANNELRECT", "Trackbar", "Get Channel Rectangle") : true))
            return "";
          #endif

          #ifdef TBM_GETLINESIZE
            if (not (TBM_GETLINESIZE == message ? translation.apply("TBM_GETLINESIZE", "Trackbar", "Get Line Size") : true))
            return "";
          #endif

          #ifdef TBM_GETNUMTICS
            if (not (TBM_GETNUMTICS == message ? translation.apply("TBM_GETNUMTICS", "Trackbar", "Get Number (of) Ticks") : true))
            return "";
          #endif

          #ifdef TBM_GETPAGESIZE
            if (not (TBM_GETPAGESIZE == message ? translation.apply("TBM_GETPAGESIZE", "Trackbar", "Get Page Size") : true))
            return "";
          #endif

          #ifdef TBM_GETPOS
            if (not (TBM_GETPOS == message ? translation.apply("TBM_GETPOS", "Trackbar", "Get Position") : true))
            return "";
          #endif

          #ifdef TBM_GETPTICS
            if (not (TBM_GETPTICS == message ? translation.apply("TBM_GETPTICS", "Trackbar", "Get Position (of) Ticks") : true))
            return "";
          #endif

          #ifdef TBM_GETRANGEMAX
            if (not (TBM_GETRANGEMAX == message ? translation.apply("TBM_GETRANGEMAX", "Trackbar", "Get Range Maximum") : true))
            return "";
          #endif

          #ifdef TBM_GETRANGEMIN
            if (not (TBM_GETRANGEMIN == message ? translation.apply("TBM_GETRANGEMIN", "Trackbar", "Get Range") : true))
            return "";
          #endif

          #ifdef TBM_GETSELEND
            if (not (TBM_GETSELEND == message ? translation.apply("TBM_GETSELEND", "Trackbar", "Get Selection End") : true))
            return "";
          #endif

          #ifdef TBM_GETSELSTART
            if (not (TBM_GETSELSTART == message ? translation.apply("TBM_GETSELSTART", "Trackbar", "Get Selection Start") : true))
            return "";
          #endif

          #ifdef TBM_GETTHUMBLENGTH
            if (not (TBM_GETTHUMBLENGTH == message ? translation.apply("TBM_GETTHUMBLENGTH", "Trackbar", "Get Thumb Length") : true))
            return "";
          #endif

          #ifdef TBM_GETTHUMBRECT
            if (not (TBM_GETTHUMBRECT == message ? translation.apply("TBM_GETTHUMBRECT", "Trackbar", "Get Thumb Rectangle") : true))
            return "";
          #endif

          #ifdef TBM_GETTIC
            if (not (TBM_GETTIC == message ? translation.apply("TBM_GETTIC", "Trackbar", "Get Tick") : true))
            return "";
          #endif

          #ifdef TBM_GETTICPOS
            if (not (TBM_GETTICPOS == message ? translation.apply("TBM_GETTICPOS", "Trackbar", "Get Tick (Mark) Position") : true))
            return "";
          #endif

          #ifdef TBM_GETTOOLTIPS
            if (not (TBM_GETTOOLTIPS == message ? translation.apply("TBM_GETTOOLTIPS", "Trackbar", "Get Tooltips") : true))
            return "";
          #endif

          #ifdef TBM_SETBUDDY
            if (not (TBM_SETBUDDY == message ? translation.apply("TBM_SETBUDDY", "Trackbar", "Set Buddy") : true))
            return "";
          #endif

          #ifdef TBM_SETLINESIZE
            if (not (TBM_SETLINESIZE == message ? translation.apply("TBM_SETLINESIZE", "Trackbar", "Set Line Size") : true))
            return "";
          #endif

          #ifdef TBM_SETPAGESIZE
            if (not (TBM_SETPAGESIZE == message ? translation.apply("TBM_SETPAGESIZE", "Trackbar", "Set Page Size") : true))
            return "";
          #endif

          #ifdef TBM_SETPOS
            if (not (TBM_SETPOS == message ? translation.apply("TBM_SETPOS", "Trackbar", "Set Position") : true))
            return "";
          #endif

          #ifdef TBM_SETRANGE
            if (not (TBM_SETRANGE == message ? translation.apply("TBM_SETRANGE", "Trackbar", "Set Range") : true))
            return "";
          #endif

          #ifdef TBM_SETRANGEMAX
            if (not (TBM_SETRANGEMAX == message ? translation.apply("TBM_SETRANGEMAX", "Trackbar", "Set Range Maximum") : true))
            return "";
          #endif

          #ifdef TBM_SETRANGEMIN
            if (not (TBM_SETRANGEMIN == message ? translation.apply("TTM_RELAYEVENT", "Tooltip", "Relay Event") : true))
            return "";
          #endif

          #ifdef TBM_SETSEL
            if (not (TBM_SETSEL == message ? translation.apply("TBM_SETSEL", "Trackbar", "Set Selection") : true))
            return "";
          #endif

          #ifdef TBM_SETSELEND
            if (not (TBM_SETSELEND == message ? translation.apply("TBM_SETSELEND", "Trackbar", "Set Selection End") : true))
            return "";
          #endif

          #ifdef TBM_SETSELSTART
            if (not (TBM_SETSELSTART == message ? translation.apply("TBM_SETSELSTART", "Trackbar", "Set Selection Start") : true))
            return "";
          #endif

          #ifdef TBM_SETTHUMBLENGTH
            if (not (TBM_SETTHUMBLENGTH == message ? translation.apply("TBM_SETTHUMBLENGTH", "Trackbar", "Set Thumb Length") : true))
            return "";
          #endif

          #ifdef TBM_SETTIC
            if (not (TBM_SETTIC == message ? translation.apply("TBM_SETTIC", "Trackbar", "Set Tick") : true))
            return "";
          #endif

          #ifdef TBM_SETTICFREQ
            if (not (TBM_SETTICFREQ == message ? translation.apply("TBM_SETTICFREQ", "Trackbar", "Set Tick (Mark) Frequency") : true))
            return "";
          #endif

          #ifdef TBM_SETTIPSIDE
            if (not (TBM_SETTIPSIDE == message ? translation.apply("TBM_SETTIPSIDE", "Trackbar", "Set Tip Side") : true))
            return "";
          #endif

          #ifdef TBM_SETTOOLTIPS
            if (not (TBM_SETTOOLTIPS == message ? translation.apply("TBM_SETTOOLTIPS", "Trackbar", "Set Tooltips") : true))
            return "";
          #endif

          #ifdef TTM_ACTIVATE
            if (not (TTM_ACTIVATE == message ? translation.apply("TTM_ACTIVATE", "Tooltip", "Activate") : true))
            return "";
          #endif

          #ifdef TTM_ADDTOOLA
            if (not (TTM_ADDTOOLA == message ? translation.apply("TTM_ADDTOOLA", "Tooltip", "Add Tool") : true))
            return "";
          #endif

          #ifdef TTM_ADJUSTRECT
            if (not (TTM_ADJUSTRECT == message ? translation.apply("TTM_ADJUSTRECT", "Tooltip", "Adjust Rectangle") : true))
            return "";
          #endif

          #ifdef TTM_DELTOOLA
            if (not (TTM_DELTOOLA == message ? translation.apply("TTM_DELTOOLA", "Tooltip", "Delete Tool") : true))
            return "";
          #endif

          #ifdef TTM_DELTOOLW
            if (not (TTM_DELTOOLW == message ? translation.apply("TTM_DELTOOLW", "Tooltip", "Delete Tool") : true))
            return "";
          #endif

          #ifdef TTM_ENUMTOOLSA
            if (not (TTM_ENUMTOOLSA == message ? translation.apply("TTM_ENUMTOOLSA", "Tooltip", "Enumerate Tools") : true))
            return "";
          #endif

          #ifdef TTM_ENUMTOOLSW
            if (not (TTM_ENUMTOOLSW == message ? translation.apply("TTM_ENUMTOOLSW", "Tooltip", "Enumerate Tools") : true))
            return "";
          #endif

          #ifdef TTM_GETBUBBLESIZE
            if (not (TTM_GETBUBBLESIZE == message ? translation.apply("TTM_GETBUBBLESIZE", "Tooltip", "Get Bubble Size") : true))
            return "";
          #endif

          #ifdef TTM_GETCURRENTTOOLA
            if (not (TTM_GETCURRENTTOOLA == message ? translation.apply("TTM_GETCURRENTTOOLA", "Tooltip", "Get Current Tool") : true))
            return "";
          #endif

          #ifdef TTM_GETCURRENTTOOLW
            if (not (TTM_GETCURRENTTOOLW == message ? translation.apply("TTM_GETCURRENTTOOLW", "Tooltip", "Get Current Tool") : true))
            return "";
          #endif

          #ifdef TTM_GETDELAYTIME
            if (not (TTM_GETDELAYTIME == message ? translation.apply("TTM_GETDELAYTIME", "Tooltip", "Get Delay Time") : true))
            return "";
          #endif

          #ifdef TTM_GETMARGIN
            if (not (TTM_GETMARGIN == message ? translation.apply("TTM_GETMARGIN", "Tooltip", "Get Margin") : true))
            return "";
          #endif

          #ifdef TTM_GETMAXTIPWIDTH
            if (not (TTM_GETMAXTIPWIDTH == message ? translation.apply("TTM_GETMAXTIPWIDTH", "Tooltip", "Get Maximum Tip Width") : true))
            return "";
          #endif

          #ifdef TTM_GETTEXTA
            if (not (TTM_GETTEXTA == message ? translation.apply("TTM_GETTEXTA", "Tooltip", "Get Text") : true))
            return "";
          #endif

          #ifdef TTM_GETTEXTW
            if (not (TTM_GETTEXTW == message ? translation.apply("TTM_GETTEXTW", "Tooltip", "Get Text") : true))
            return "";
          #endif

          #ifdef TTM_GETTIPBKCOLOR
            if (not (TTM_GETTIPBKCOLOR == message ? translation.apply("TTM_GETTIPBKCOLOR", "Tooltip", "Get Tip Background Color") : true))
            return "";
          #endif

          #ifdef TTM_GETTIPTEXTCOLOR
            if (not (TTM_GETTIPTEXTCOLOR == message ? translation.apply("TTM_GETTIPTEXTCOLOR", "Tooltip", "Get Tip Text Color") : true))
            return "";
          #endif

          #ifdef TTM_GETTOOLCOUNT
            if (not (TTM_GETTOOLCOUNT == message ? translation.apply("TTM_GETTOOLCOUNT", "Tooltip", "Get Tool Count") : true))
            return "";
          #endif

          #ifdef TTM_GETTOOLINFOA
            if (not (TTM_GETTOOLINFOA == message ? translation.apply("TTM_GETTOOLINFOA", "Tooltip", "Get Tool Information") : true))
            return "";
          #endif

          #ifdef TTM_GETTOOLINFOW
            if (not (TTM_GETTOOLINFOW == message ? translation.apply("TTM_GETTOOLINFOW", "Tooltip", "Get Tool Information") : true))
            return "";
          #endif

          #ifdef TTM_HITTESTA
            if (not (TTM_HITTESTA == message ? translation.apply("TTM_HITTESTA", "Tooltip", "Hit Test") : true))
            return "";
          #endif

          #ifdef TTM_HITTESTW
            if (not (TTM_HITTESTW == message ? translation.apply("TTM_HITTESTW", "Tooltip", "Hit Test") : true))
            return "";
          #endif

          #ifdef TTM_NEWTOOLRECTA
            if (not (TTM_NEWTOOLRECTA == message ? translation.apply("TTM_NEWTOOLRECTA", "Tooltip", "New Tool Rectangle") : true))
            return "";
          #endif

          #ifdef TTM_NEWTOOLRECTW
            if (not (TTM_NEWTOOLRECTW == message ? translation.apply("TTM_NEWTOOLRECTW", "Tooltip", "New Tool Rectangle") : true))
            return "";
          #endif

          #ifdef TTM_POP
            if (not (TTM_POP == message ? translation.apply("TTM_POP", "Tooltip", "Pop") : true))
            return "";
          #endif

          #ifdef TTM_RELAYEVENT
            if (not (TTM_RELAYEVENT == message ? translation.apply("TBM_SETRANGEMIN", "Trackbar", "Set Range Minimum") : true))
            return "";
          #endif

          #ifdef TTM_SETDELAYTIME
            if (not (TTM_SETDELAYTIME == message ? translation.apply("TTM_SETDELAYTIME", "Tooltip", "Set Delay Time") : true))
            return "";
          #endif

          #ifdef TTM_SETMARGIN
            if (not (TTM_SETMARGIN == message ? translation.apply("TTM_SETMARGIN", "Tooltip", "Get Channel Rectangle") : true))
            return "";
          #endif

          #ifdef TTM_SETMAXTIPWIDTH
            if (not (TTM_SETMAXTIPWIDTH == message ? translation.apply("TTM_SETMAXTIPWIDTH", "Tooltip", "Set Maximum Tip Width") : true))
            return "";
          #endif

          #ifdef TTM_SETTIPBKCOLOR
            if (not (TTM_SETTIPBKCOLOR == message ? translation.apply("TTM_SETTIPBKCOLOR", "Tooltip", "Set Tip Background Color") : true))
            return "";
          #endif

          #ifdef TTM_SETTIPTEXTCOLOR
            if (not (TTM_SETTIPTEXTCOLOR == message ? translation.apply("TTM_SETTIPTEXTCOLOR", "Tooltip", "Set Tip Text Color") : true))
            return "";
          #endif

          #ifdef TTM_SETTITLEA
            if (not (TTM_SETTITLEA == message ? translation.apply("TTM_SETTITLEA", "Tooltip", "Set Title") : true))
            return "";
          #endif

          #ifdef TTM_SETTITLEW
            if (not (TTM_SETTITLEW == message ? translation.apply("TTM_SETTITLEW", "Tooltip", "Set Title") : true))
            return "";
          #endif

          #ifdef TTM_SETTOOLINFOA
            if (not (TTM_SETTOOLINFOA == message ? translation.apply("TTM_SETTOOLINFOA", "Tooltip", "Set Tool Information") : true))
            return "";
          #endif

          #ifdef TTM_SETTOOLINFOW
            if (not (TTM_SETTOOLINFOW == message ? translation.apply("TTM_SETTOOLINFOW", "Tooltip", "Set Tool Information") : true))
            return "";
          #endif

          #ifdef TTM_TRACKACTIVATE
            if (not (TTM_TRACKACTIVATE == message ? translation.apply("TTM_TRACKACTIVATE", "Tooltip", "Track Activate") : true))
            return "";
          #endif

          #ifdef TTM_TRACKPOSITION
            if (not (TTM_TRACKPOSITION == message ? translation.apply("TTM_TRACKPOSITION", "Tooltip", "Track Position") : true))
            return "";
          #endif

          #ifdef TTM_UPDATE
            if (not (TTM_UPDATE == message ? translation.apply("TTM_UPDATE", "Tooltip", "Update") : true))
            return "";
          #endif

          #ifdef TTM_UPDATETIPTEXTA
            if (not (TTM_UPDATETIPTEXTA == message ? translation.apply("TTM_UPDATETIPTEXTA", "Tooltip", "Update Tip Text") : true))
            return "";
          #endif

          #ifdef TTM_UPDATETIPTEXTW
            if (not (TTM_UPDATETIPTEXTW == message ? translation.apply("TTM_UPDATETIPTEXTW", "Tooltip", "Update Tip Text") : true))
            return "";
          #endif

          #ifdef TTM_WINDOWFROMPOINT
            if (not (TTM_WINDOWFROMPOINT == message ? translation.apply("TTM_WINDOWFROMPOINT", "Tooltip", "Window from Point") : true))
            return "";
          #endif

          #ifdef UDM_GETACCEL
            if (not (UDM_GETACCEL == message ? translation.apply("UDM_GETACCEL", "Up-Down Control", "Get Acceleration") : true))
            return "";
          #endif

          #ifdef UDM_GETBASE
            if (not (UDM_GETBASE == message ? translation.apply("UDM_GETBASE", "Up-Down Control", "Get Base") : true))
            return "";
          #endif

          #ifdef UDM_GETBUDDY
            if (not (UDM_GETBUDDY == message ? translation.apply("UDM_GETBUDDY", "Up-Down Control", "Get Buddy") : true))
            return "";
          #endif

          #ifdef UDM_GETPOS
            if (not (UDM_GETPOS == message ? translation.apply("UDM_GETPOS", "Up-Down Control", "Get Position") : true))
            return "";
          #endif

          #ifdef UDM_GETPOS32
            if (not (UDM_GETPOS32 == message ? translation.apply("UDM_GETPOS32", "Up-Down Control", "Get Position") : true))
            return "";
          #endif

          #ifdef UDM_GETRANGE
            if (not (UDM_GETRANGE == message ? translation.apply("UDM_GETRANGE", "Up-Down Control", "Get Range") : true))
            return "";
          #endif

          #ifdef UDM_GETRANGE32
            if (not (UDM_GETRANGE32 == message ? translation.apply("UDM_GETRANGE32", "Up-Down Control", "Get Range") : true))
            return "";
          #endif

          #ifdef UDM_SETACCEL
            if (not (UDM_SETACCEL == message ? translation.apply("UDM_SETACCEL", "Up-Down Control", "Set Acceleration") : true))
            return "";
          #endif

          #ifdef UDM_SETBASE
            if (not (UDM_SETBASE == message ? translation.apply("UDM_SETBASE", "Up-Down Control", "Set Base") : true))
            return "";
          #endif

          #ifdef UDM_SETBUDDY
            if (not (UDM_SETBUDDY == message ? translation.apply("UDM_SETBUDDY", "Up-Down Control", "Set Buddy") : true))
            return "";
          #endif

          #ifdef UDM_SETPOS
            if (not (UDM_SETPOS == message ? translation.apply("UDM_SETPOS", "Up-Down Control", "Set Range") : true))
            return "";
          #endif

          #ifdef UDM_SETPOS32
            if (not (UDM_SETPOS32 == message ? translation.apply("UDM_SETPOS32", "Up-Down Control", "Set Position") : true))
            return "";
          #endif

          #ifdef UDM_SETRANGE
            if (not (UDM_SETRANGE == message ? translation.apply("UDM_SETRANGE", "Up-Down Control", "Set Range") : true))
            return "";
          #endif

          #ifdef UDM_SETRANGE32
            if (not (UDM_SETRANGE32 == message ? translation.apply("UDM_SETRANGE32", "Up-Down Control", "Set Range") : true))
            return "";
          #endif

          #ifdef UM_GETCURFOCUSA
            if (not (UM_GETCURFOCUSA == message ? translation.apply("UM_GETCURFOCUSA", "User Manager", "Get Current Focus") : true))
            return "";
          #endif

          #ifdef UM_GETCURFOCUSW
            if (not (UM_GETCURFOCUSW == message ? translation.apply("UM_GETCURFOCUSW", "User Manager", "Get Current Focus") : true))
            return "";
          #endif

          #ifdef UM_GETGROUPSELA
            if (not (UM_GETGROUPSELA == message ? translation.apply("UM_GETGROUPSELA", "User Manager", "Get Group Selection") : true))
            return "";
          #endif

          #ifdef UM_GETGROUPSELW
            if (not (UM_GETGROUPSELW == message ? translation.apply("UM_GETGROUPSELW", "User Manager", "Get Group Selection") : true))
            return "";
          #endif

          #ifdef UM_GETOPTIONS
            if (not (UM_GETOPTIONS == message ? translation.apply("UM_GETOPTIONS", "User Manager", "Get Options") : true))
            return "";
          #endif

          #ifdef UM_GETOPTIONS2
            if (not (UM_GETOPTIONS2 == message ? translation.apply("UM_GETOPTIONS2", "User Manager", "Get Options (Structure)") : true))
            return "";
          #endif

          #ifdef UM_GETSELCOUNT
            if (not (UM_GETSELCOUNT == message ? translation.apply("UM_GETSELCOUNT", "User Manager", "Get Selection Count") : true))
            return "";
          #endif

          #ifdef UM_GETUSERSELA
            if (not (UM_GETUSERSELA == message ? translation.apply("UM_GETUSERSELA", "User Manager", "Get User Selection") : true))
            return "";
          #endif

          #ifdef UM_GETUSERSELW
            if (not (UM_GETUSERSELW == message ? translation.apply("UM_GETUSERSELW", "User Manager", "Get User Selection") : true))
            return "";
          #endif

          #ifdef WIZ_NEXT
            if (not (WIZ_NEXT == message ? translation.apply("WIZ_NEXT", "Wizard", "Next") : true))
            return "";
          #endif

          #ifdef WIZ_PREV
            if (not (WIZ_PREV == message ? translation.apply("WIZ_PREV", "Wizard", "Previous") : true))
            return "";
          #endif

          #ifdef WIZ_QUERYNUMPAGES
            if (not (WIZ_QUERYNUMPAGES == message ? translation.apply("WIZ_QUERYNUMPAGES", "Wizard", "Query Number of Pages") : true))
            return "";
          #endif

          #ifdef WLX_WM_SAS
            if (not (WLX_WM_SAS == message ? translation.apply("WLX_WM_SAS", "Windows Live Sync", "Statistical Analysis Suite Message") : true))
            return "";
          #endif

          #ifdef WM_ACTIVATE
            if (not (WM_ACTIVATE == message ? translation.apply("WM_ACTIVATE", "Windows", "Activate") : true))
            return "";
          #endif

          #ifdef WM_ACTIVATEAPP
            if (not (WM_ACTIVATEAPP == message ? translation.apply("WM_ACTIVATEAPP", "Windows", "Activate Application") : true))
            return "";
          #endif

          #ifdef WM_AFXFIRST
            if (not (WM_AFXFIRST == message ? translation.apply("WM_AFXFIRST", "Windows", "Application Framework Extensions [FIRST]") : true))
            return "";
          #endif

          #ifdef WM_AFXLAST
            if (not (WM_AFXLAST == message ? translation.apply("WM_AFXLAST", "Windows", "Application Framework Extensions [LAST]") : true))
            return "";
          #endif

          #ifdef WM_APP
            if (not (WM_APP == message ? translation.apply("WM_APP", "Windows", "Application") : true))
            return "";
          #endif

          #ifdef WM_APPCOMMAND
            if (not (WM_APPCOMMAND == message ? translation.apply("WM_APPCOMMAND", "Windows", "Application Command") : true))
            return "";
          #endif

          #ifdef WM_ASKCBFORMATNAME
            if (not (WM_ASKCBFORMATNAME == message ? translation.apply("WM_ASKCBFORMATNAME", "Windows", "Ask Clipboard Format Name") : true))
            return "";
          #endif

          #ifdef WM_CANCELJOURNAL
            if (not (WM_CANCELJOURNAL == message ? translation.apply("WM_CANCELJOURNAL", "Windows", "Cancel Journal") : true))
            return "";
          #endif

          #ifdef WM_CANCELMODE
            if (not (WM_CANCELMODE == message ? translation.apply("WM_CANCELMODE", "Windows", "Cancel Mode") : true))
            return "";
          #endif

          #ifdef WM_CAP_DRIVER_GET_NAMEW
            if (not (WM_CAP_DRIVER_GET_NAMEW == message ? translation.apply("WM_CAP_DRIVER_GET_NAMEW", "Windows Capture", "Driver Get Name") : true))
            return "";
          #endif

          #ifdef WM_CAP_DRIVER_GET_VERSIONW
            if (not (WM_CAP_DRIVER_GET_VERSIONW == message ? translation.apply("WM_CAP_DRIVER_GET_VERSIONW", "Windows Capture", "Driver Get Version") : true))
            return "";
          #endif

          #ifdef WM_CAP_FILE_GET_CAPTURE_FILEW
            if (not (WM_CAP_FILE_GET_CAPTURE_FILEW == message ? translation.apply("WM_CAP_FILE_GET_CAPTURE_FILEW", "Up-Down Control", "File Get Capture") : true))
            return "";
          #endif

          #ifdef WM_CAP_FILE_SAVEASW
            if (not (WM_CAP_FILE_SAVEASW == message ? translation.apply("WM_CAP_FILE_SAVEASW", "Windows Capture", "File Save As") : true))
            return "";
          #endif

          #ifdef WM_CAP_FILE_SAVEDIBW
            if (not (WM_CAP_FILE_SAVEDIBW == message ? translation.apply("WM_CAP_FILE_SAVEDIBW", "Windows Capture", "File Save Device-Independent Bitmap") : true))
            return "";
          #endif

          #ifdef WM_CAP_FILE_SET_CAPTURE_FILEW
            if (not (WM_CAP_FILE_SET_CAPTURE_FILEW == message ? translation.apply("WM_CAP_FILE_SET_CAPTURE_FILEW", "Windows Capture", "File Set Capture File") : true))
            return "";
          #endif

          #ifdef WM_CAP_GET_MCI_DEVICEW
            if (not (WM_CAP_GET_MCI_DEVICEW == message ? translation.apply("WM_CAP_GET_MCI_DEVICEW", "Windows Capture", "Get Media Control Interface Device") : true))
            return "";
          #endif

          #ifdef WM_CAP_PAL_OPENW
            if (not (WM_CAP_PAL_OPENW == message ? translation.apply("WM_CAP_PAL_OPENW", "Windows Capture", "Palette Open") : true))
            return "";
          #endif

          #ifdef WM_CAP_PAL_SAVEW
            if (not (WM_CAP_PAL_SAVEW == message ? translation.apply("WM_CAP_PAL_SAVEW", "Windows Capture", "Palette Save") : true))
            return "";
          #endif

          #ifdef WM_CAP_SET_CALLBACK_ERRORW
            if (not (WM_CAP_SET_CALLBACK_ERRORW == message ? translation.apply("WM_CAP_SET_CALLBACK_ERRORW", "Windows Capture", "Set Callback Error") : true))
            return "";
          #endif

          #ifdef WM_CAP_SET_CALLBACK_STATUSW
            if (not (WM_CAP_SET_CALLBACK_STATUSW == message ? translation.apply("WM_CAP_SET_CALLBACK_STATUSW", "Windows Capture", "Set Callback Status") : true))
            return "";
          #endif

          #ifdef WM_CAP_SET_MCI_DEVICEW
            if (not (WM_CAP_SET_MCI_DEVICEW == message ? translation.apply("WM_CAP_SET_MCI_DEVICEW", "Windows Capture", "Set MCI (Media Control Interface) Device") : true))
            return "";
          #endif

          #ifdef WM_CAP_UNICODE_START
            if (not (WM_CAP_UNICODE_START == message ? translation.apply("WM_CAP_UNICODE_START", "Windows Capture", "Unicode Start") : true))
            return "";
          #endif

          #ifdef WM_CAPTURECHANGED
            if (not (WM_CAPTURECHANGED == message ? translation.apply("WM_CAPTURECHANGED", "Windows Capture", "Changed") : true))
            return "";
          #endif

          #ifdef WM_CHANGECBCHAIN
            if (not (WM_CHANGECBCHAIN == message ? translation.apply("WM_CHANGECBCHAIN", "Windows", "Change Control Box Chain") : true))
            return "";
          #endif

          #ifdef WM_CHANGEUISTATE
            if (not (WM_CHANGEUISTATE == message ? translation.apply("WM_CHANGEUISTATE", "Windows", "Change User-Interface State") : true))
            return "";
          #endif

          #ifdef WM_CHAR
            if (not (WM_CHAR == message ? translation.apply("WM_CHAR", "Windows", "Character") : true))
            return "";
          #endif

          #ifdef WM_CHARTOITEM
            if (not (WM_CHARTOITEM == message ? translation.apply("WM_CHARTOITEM", "Windows", "Character to Item") : true))
            return "";
          #endif

          #ifdef WM_CHILDACTIVATE
            if (not (WM_CHILDACTIVATE == message ? translation.apply("WM_CHILDACTIVATE", "Windows", "Child Activate") : true))
            return "";
          #endif

          #ifdef WM_CHOOSEFONT_GETLOGFONT
            if (not (WM_CHOOSEFONT_GETLOGFONT == message ? translation.apply("WM_CHOOSEFONT_GETLOGFONT", "Windows", "Choose Font (Get Log Font)") : true))
            return "";
          #endif

          #ifdef WM_CHOOSEFONT_SETFLAGS
            if (not (WM_CHOOSEFONT_SETFLAGS == message ? translation.apply("WM_CHOOSEFONT_SETFLAGS", "Windows", "Choose Font (Set Flags)") : true))
            return "";
          #endif

          #ifdef WM_CHOOSEFONT_SETLOGFONT
            if (not (WM_CHOOSEFONT_SETLOGFONT == message ? translation.apply("WM_CHOOSEFONT_SETLOGFONT", "Windows", "Choose Font (Set Log Font)") : true))
            return "";
          #endif

          #ifdef WM_CLEAR
            if (not (WM_CLEAR == message ? translation.apply("WM_CLEAR", "Windows", "Clear") : true))
            return "";
          #endif

          #ifdef WM_CLOSE
            if (not (WM_CLOSE == message ? translation.apply("WM_CLOSE", "Windows", "Close") : true))
            return "";
          #endif

          #ifdef WM_CLOSE
            if (not (WM_CLOSE == message ? translation.apply("WM_CLOSE", "Windows", "Close") : true))
            return "";
          #endif

          #ifdef WM_COMMAND
            if (not (WM_COMMAND == message ? translation.apply("WM_COMMAND", "Windows", "Command") : true))
            return "";
          #endif

          #ifdef WM_COMMNOTIFY
            if (not (WM_COMMNOTIFY == message ? translation.apply("WM_COMMNOTIFY", "Windows", "Command Notify") : true))
            return "";
          #endif

          #ifdef WM_COMPACTING
            if (not (WM_COMPACTING == message ? translation.apply("WM_COMPACTING", "Windows", "Compacting") : true))
            return "";
          #endif

          #ifdef WM_COMPAREITEM
            if (not (WM_COMPAREITEM == message ? translation.apply("WM_COMPAREITEM", "Windows", "Compare Item") : true))
            return "";
          #endif

          #ifdef WM_CONTEXTMENU
            if (not (WM_CONTEXTMENU == message ? translation.apply("WM_CONTEXTMENU", "Windows", "Context Menu") : true))
            return "";
          #endif

          #ifdef WM_CONVERTREQUEST
            if (not (WM_CONVERTREQUEST == message ? translation.apply("WM_CONVERTREQUEST", "Windows", "Convert Request") : true))
            return "";
          #endif

          #ifdef WM_CONVERTRESULT
            if (not (WM_CONVERTRESULT == message ? translation.apply("WM_CONVERTRESULT", "Windows", "Convert Result") : true))
            return "";
          #endif

          #ifdef WM_COPY
            if (not (WM_COPY == message ? translation.apply("WM_COPY", "Windows", "Copy") : true))
            return "";
          #endif

          #ifdef WM_COPYDATA
            if (not (WM_COPYDATA == message ? translation.apply("WM_COPYDATA", "Windows", "Copy Data") : true))
            return "";
          #endif

          #ifdef WM_COPYGLOBALDATA
            if (not (WM_COPYGLOBALDATA == message ? translation.apply("WM_COPYGLOBALDATA", "Windows", "Copy Global Data") : true))
            return "";
          #endif

          #ifdef WM_CPL_LAUNCH
            if (not (WM_CPL_LAUNCH == message ? translation.apply("WM_CPL_LAUNCH", "Windows", "Control Panel Launch") : true))
            return "";
          #endif

          #ifdef WM_CPL_LAUNCHED
            if (not (WM_CPL_LAUNCHED == message ? translation.apply("WM_CPL_LAUNCHED", "Windows", "Control Panel Launched") : true))
            return "";
          #endif

          #ifdef WM_CREATE
            if (not (WM_CREATE == message ? translation.apply("WM_CREATE", "Windows", "Create") : true))
            return "";
          #endif

          #ifdef WM_CTLCOLOR
            if (not (WM_CTLCOLOR == message ? translation.apply("WM_CTLCOLOR", "Windows", "Control Color") : true))
            return "";
          #endif

          #ifdef WM_CTLCOLORBTN
            if (not (WM_CTLCOLORBTN == message ? translation.apply("WM_CTLCOLORBTN", "Windows", "Control Color Button") : true))
            return "";
          #endif

          #ifdef WM_CTLCOLORDLG
            if (not (WM_CTLCOLORDLG == message ? translation.apply("WM_CTLCOLORDLG", "Windows", "Control Color Dialog") : true))
            return "";
          #endif

          #ifdef WM_CTLCOLOREDIT
            if (not (WM_CTLCOLOREDIT == message ? translation.apply("WM_CTLCOLOREDIT", "Windows", "Control Color Editor") : true))
            return "";
          #endif

          #ifdef WM_CTLCOLORLISTBOX
            if (not (WM_CTLCOLORLISTBOX == message ? translation.apply("WM_CTLCOLORLISTBOX", "Windows", "Control Color List Box") : true))
            return "";
          #endif

          #ifdef WM_CTLCOLORMSGBOX
            if (not (WM_CTLCOLORMSGBOX == message ? translation.apply("WM_CTLCOLORMSGBOX", "Windows", "Control Color Message Box") : true))
            return "";
          #endif

          #ifdef WM_CTLCOLORSCROLLBAR
            if (not (WM_CTLCOLORSCROLLBAR == message ? translation.apply("WM_CTLCOLORSCROLLBAR", "Windows", "Control Color Scrollbar") : true))
            return "";
          #endif

          #ifdef WM_CTLCOLORSTATIC
            if (not (WM_CTLCOLORSTATIC == message ? translation.apply("WM_CTLCOLORSTATIC", "Windows", "Control Color Static") : true))
            return "";
          #endif

          #ifdef WM_CTLINIT
            if (not (WM_CTLINIT == message ? translation.apply("WM_CTLINIT", "Windows", "Control Initiation") : true))
            return "";
          #endif

          #ifdef WM_CUT
            if (not (WM_CUT == message ? translation.apply("WM_CUT", "Windows", "Cut") : true))
            return "";
          #endif

          #ifdef WM_DEADCHAR
            if (not (WM_DEADCHAR == message ? translation.apply("WM_DEADCHAR", "Windows", "Dead Character") : true))
            return "";
          #endif

          #ifdef WM_DELETEITEM
            if (not (WM_DELETEITEM == message ? translation.apply("WM_DELETEITEM", "Windows", "Delete Item") : true))
            return "";
          #endif

          #ifdef WM_DESTROY
            if (not (WM_DESTROY == message ? translation.apply("WM_DESTROY", "Windows", "Destroy") : true))
            return "";
          #endif

          #ifdef WM_DESTROY
            if (not (WM_DESTROY == message ? translation.apply("WM_DESTROY", "Windows", "Destroy") : true))
            return "";
          #endif

          #ifdef WM_DESTROYCLIPBOARD
            if (not (WM_DESTROYCLIPBOARD == message ? translation.apply("WM_DESTROYCLIPBOARD", "Windows", "Destroy Clipboard") : true))
            return "";
          #endif

          #ifdef WM_DEVICECHANGE
            if (not (WM_DEVICECHANGE == message ? translation.apply("WM_DEVICECHANGE", "Windows", "Device Change") : true))
            return "";
          #endif

          #ifdef WM_DEVMODECHANGE
            if (not (WM_DEVMODECHANGE == message ? translation.apply("WM_DEVMODECHANGE", "Windows", "Device Mode Change") : true))
            return "";
          #endif

          #ifdef WM_DISPLAYCHANGE
            if (not (WM_DISPLAYCHANGE == message ? translation.apply("WM_DISPLAYCHANGE", "Windows", "Display Change") : true))
            return "";
          #endif

          #ifdef WM_DRAWCLIPBOARD
            if (not (WM_DRAWCLIPBOARD == message ? translation.apply("WM_DRAWCLIPBOARD", "Windows", "Draw Clipboard") : true))
            return "";
          #endif

          #ifdef WM_DRAWITEM
            if (not (WM_DRAWITEM == message ? translation.apply("WM_DRAWITEM", "Windows", "Draw Item") : true))
            return "";
          #endif

          #ifdef WM_DROPFILES
            if (not (WM_DROPFILES == message ? translation.apply("WM_DROPFILES", "Windows", "Drop Files") : true))
            return "";
          #endif

          #ifdef WM_ENABLE
            if (not (WM_ENABLE == message ? translation.apply("WM_ENABLE", "Windows", "Enable") : true))
            return "";
          #endif

          #ifdef WM_ENDSESSION
            if (not (WM_ENDSESSION == message ? translation.apply("WM_ENDSESSION", "Windows", "End Session") : true))
            return "";
          #endif

          #ifdef WM_ENTERIDLE
            if (not (WM_ENTERIDLE == message ? translation.apply("WM_ENTERIDLE", "Windows", "Enter Idle") : true))
            return "";
          #endif

          #ifdef WM_ENTERMENULOOP
            if (not (WM_ENTERMENULOOP == message ? translation.apply("WM_ENTERMENULOOP", "Windows", "Enter Menu Loop") : true))
            return "";
          #endif

          #ifdef WM_ENTERSIZEMOVE
            if (not (WM_ENTERSIZEMOVE == message ? translation.apply("WM_ENTERSIZEMOVE", "Windows", "Enter Size Move") : true))
            return "";
          #endif

          #ifdef WM_ERASEBKGND
            if (not (WM_ERASEBKGND == message ? translation.apply("WM_ERASEBKGND", "Windows", "Erase Background") : true))
            return "";
          #endif

          #ifdef WM_EXITMENULOOP
            if (not (WM_EXITMENULOOP == message ? translation.apply("WM_EXITMENULOOP", "Windows", "Exit Menu Loop") : true))
            return "";
          #endif

          #ifdef WM_EXITSIZEMOVE
            if (not (WM_EXITSIZEMOVE == message ? translation.apply("WM_EXITSIZEMOVE", "Windows", "Exit Size Move") : true))
            return "";
          #endif

          #ifdef WM_FONTCHANGE
            if (not (WM_FONTCHANGE == message ? translation.apply("WM_FONTCHANGE", "Windows", "Font Change") : true))
            return "";
          #endif

          #ifdef WM_GETDLGCODE
            if (not (WM_GETDLGCODE == message ? translation.apply("WM_GETDLGCODE", "Windows", "Get Dialog Code") : true))
            return "";
          #endif

          #ifdef WM_GETFONT
            if (not (WM_GETFONT == message ? translation.apply("WM_GETFONT", "Windows", "Get Font") : true))
            return "";
          #endif

          #ifdef WM_GETHOTKEY
            if (not (WM_GETHOTKEY == message ? translation.apply("WM_GETHOTKEY", "Windows", "Get Hotkey") : true))
            return "";
          #endif

          #ifdef WM_GETICON
            if (not (WM_GETICON == message ? translation.apply("WM_GETICON", "Windows", "Get Icon") : true))
            return "";
          #endif

          #ifdef WM_GETMINMAXINFO
            if (not (WM_GETMINMAXINFO == message ? translation.apply("WM_GETMINMAXINFO", "Windows", "Get Min-Maximum Information") : true))
            return "";
          #endif

          #ifdef WM_GETOBJECT
            if (not (WM_GETOBJECT == message ? translation.apply("WM_GETOBJECT", "Windows", "Get Object") : true))
            return "";
          #endif

          #ifdef WM_GETTEXT
            if (not (WM_GETTEXT == message ? translation.apply("WM_GETTEXT", "Windows", "Get Text") : true))
            return "";
          #endif

          #ifdef WM_GETTEXTLENGTH
            if (not (WM_GETTEXTLENGTH == message ? translation.apply("WM_GETTEXTLENGTH", "Windows", "Get Text Length") : true))
            return "";
          #endif

          #ifdef WM_GLOBALRCCHANGE
            if (not (WM_GLOBALRCCHANGE == message ? translation.apply("WM_GLOBALRCCHANGE", "Windows", "Global RC Change") : true))
            return "";
          #endif

          #ifdef WM_HANDHELDFIRST
            if (not (WM_HANDHELDFIRST == message ? translation.apply("WM_HANDHELDFIRST", "Windows", "Hand-Held [FIRST]") : true))
            return "";
          #endif

          #ifdef WM_HANDHELDLAST
            if (not (WM_HANDHELDLAST == message ? translation.apply("WM_HANDHELDLAST", "Windows", "Hand-Held [LAST]") : true))
            return "";
          #endif

          #ifdef WM_HEDITCTL
            if (not (WM_HEDITCTL == message ? translation.apply("WM_HEDITCTL", "Windows", "Hand-Editing Control") : true))
            return "";
          #endif

          #ifdef WM_HELP
            if (not (WM_HELP == message ? translation.apply("WM_HELP", "Windows", "Help") : true))
            return "";
          #endif

          #ifdef WM_HOOKRCRESULT
            if (not (WM_HOOKRCRESULT == message ? translation.apply("WM_HOOKRCRESULT", "Windows", "Hook RC Result") : true))
            return "";
          #endif

          #ifdef WM_HOTKEY
            if (not (WM_HOTKEY == message ? translation.apply("WM_HOTKEY", "Windows", "Hotkey") : true))
            return "";
          #endif

          #ifdef WM_HSCROLL
            if (not (WM_HSCROLL == message ? translation.apply("WM_HSCROLL", "Windows", "Horizontal Scroll") : true))
            return "";
          #endif

          #ifdef WM_HSCROLLCLIPBOARD
            if (not (WM_HSCROLLCLIPBOARD == message ? translation.apply("WM_HSCROLLCLIPBOARD", "Windows", "Horizontal Scroll Clipboard") : true))
            return "";
          #endif

          #ifdef WM_ICONERASEBKGND
            if (not (WM_ICONERASEBKGND == message ? translation.apply("WM_ICONERASEBKGND", "Windows", "Icon Erase Background") : true))
            return "";
          #endif

          #ifdef WM_IME_CHAR
            if (not (WM_IME_CHAR == message ? translation.apply("WM_IME_CHAR", "Windows", "IME (Input Method Encoding) Character") : true))
            return "";
          #endif

          #ifdef WM_IME_COMPOSITION
            if (not (WM_IME_COMPOSITION == message ? translation.apply("WM_IME_COMPOSITION", "Windows", "IME (Input Method Encoding) Composition [(Key) LAST]") : true))
            return "";
          #endif

          #ifdef WM_IME_COMPOSITIONFULL
            if (not (WM_IME_COMPOSITIONFULL == message ? translation.apply("WM_IME_COMPOSITIONFULL", "Windows", "IME (Input Method Encoding) Composition Full") : true))
            return "";
          #endif

          #ifdef WM_IME_CONTROL
            if (not (WM_IME_CONTROL == message ? translation.apply("WM_IME_CONTROL", "Windows", "IME (Input Method Encoding) Control") : true))
            return "";
          #endif

          #ifdef WM_IME_ENDCOMPOSITION
            if (not (WM_IME_ENDCOMPOSITION == message ? translation.apply("WM_IME_ENDCOMPOSITION", "Windows", "IME (Input Method Encoding) End Composition") : true))
            return "";
          #endif

          #ifdef WM_IME_NOTIFY
            if (not (WM_IME_NOTIFY == message ? translation.apply("WM_IME_NOTIFY", "Windows", "IME (Input Method Encoding) Notify") : true))
            return "";
          #endif

          #ifdef WM_IME_REPORT
            if (not (WM_IME_REPORT == message ? translation.apply("WM_IME_REPORT", "Windows", "IME (Input Method Encoding) Report") : true))
            return "";
          #endif

          #ifdef WM_IME_REQUEST
            if (not (WM_IME_REQUEST == message ? translation.apply("WM_IME_REQUEST", "Windows", "IME (Input Method Encoding) Request") : true))
            return "";
          #endif

          #ifdef WM_IME_SELECT
            if (not (WM_IME_SELECT == message ? translation.apply("WM_IME_SELECT", "Windows", "IME (Input Method Encoding) Select") : true))
            return "";
          #endif

          #ifdef WM_IME_SETCONTEXT
            if (not (WM_IME_SETCONTEXT == message ? translation.apply("WM_IME_SETCONTEXT", "Windows", "IME (Input Method Encoding) Set Context") : true))
            return "";
          #endif

          #ifdef WM_IME_STARTCOMPOSITION
            if (not (WM_IME_STARTCOMPOSITION == message ? translation.apply("WM_IME_STARTCOMPOSITION", "Windows", "IME (Input Method Encoding) Start Composition") : true))
            return "";
          #endif

          #ifdef WM_IMEKEYDOWN
            if (not (WM_IMEKEYDOWN == message ? translation.apply("WM_IMEKEYDOWN", "Windows", "IME (Input Method Encoding) Key Down") : true))
            return "";
          #endif

          #ifdef WM_IMEKEYUP
            if (not (WM_IMEKEYUP == message ? translation.apply("WM_IMEKEYUP", "Windows", "IME (Input Method Encoding) Key Up") : true))
            return "";
          #endif

          #ifdef WM_INITDIALOG
            if (not (WM_INITDIALOG == message ? translation.apply("WM_INITDIALOG", "Windows", "Initiate Dialog") : true))
            return "";
          #endif

          #ifdef WM_INITMENU
            if (not (WM_INITMENU == message ? translation.apply("WM_INITMENU", "Windows", "Initiate Menu") : true))
            return "";
          #endif

          #ifdef WM_INITMENUPOPUP
            if (not (WM_INITMENUPOPUP == message ? translation.apply("WM_INITMENUPOPUP", "Windows", "Initiate Menu Pop-Up") : true))
            return "";
          #endif

          #ifdef WM_INPUT
            if (not (WM_INPUT == message ? translation.apply("WM_INPUT", "Windows", "Input") : true))
            return "";
          #endif

          #ifdef WM_INPUTLANGCHANGE
            if (not (WM_INPUTLANGCHANGE == message ? translation.apply("WM_INPUTLANGCHANGE", "Windows", "Input Language Change") : true))
            return "";
          #endif

          #ifdef WM_INPUTLANGCHANGEREQUEST
            if (not (WM_INPUTLANGCHANGEREQUEST == message ? translation.apply("WM_INPUTLANGCHANGEREQUEST", "Windows", "Input Language Change Request") : true))
            return "";
          #endif

          #ifdef WM_INTERIM
            if (not (WM_INTERIM == message ? translation.apply("WM_INTERIM", "Windows", "Interim") : true))
            return "";
          #endif

          #ifdef WM_KEYDOWN
            if (not (WM_KEYDOWN == message ? translation.apply("WM_KEYDOWN", "Windows", "Key Down") : true))
            return "";
          #endif

          #ifdef WM_KEYFIRST
            if (not (WM_KEYFIRST == message ? translation.apply("WM_KEYFIRST", "Windows", "Key 1st") : true))
            return "";
          #endif

          #ifdef WM_KEYLAST
            if (not (WM_KEYLAST == message ? translation.apply("WM_KEYLAST", "Windows", "Key nth") : true))
            return "";
          #endif

          #ifdef WM_KEYUP
            if (not (WM_KEYUP == message ? translation.apply("WM_KEYUP", "Windows", "Key Up") : true))
            return "";
          #endif

          #ifdef WM_KILLFOCUS
            if (not (WM_KILLFOCUS == message ? translation.apply("WM_KILLFOCUS", "Windows", "Kill Focus") : true))
            return "";
          #endif

          #ifdef WM_LBUTTONDBLCLK
            if (not (WM_LBUTTONDBLCLK == message ? translation.apply("WM_LBUTTONDBLCLK", "Windows", "Left (Mouse) Button Double-Click") : true))
            return "";
          #endif

          #ifdef WM_LBUTTONDOWN
            if (not (WM_LBUTTONDOWN == message ? translation.apply("WM_LBUTTONDOWN", "Windows", "Left (Mouse) Button Down") : true))
            return "";
          #endif

          #ifdef WM_LBUTTONUP
            if (not (WM_LBUTTONUP == message ? translation.apply("WM_LBUTTONUP", "Windows", "Left (Mouse) Button Up") : true))
            return "";
          #endif

          #ifdef WM_MBUTTONDBLCLK
            if (not (WM_MBUTTONDBLCLK == message ? translation.apply("WM_MBUTTONDBLCLK", "Windows", "Middle (Mouse) Button Double-Click") : true))
            return "";
          #endif

          #ifdef WM_MBUTTONDOWN
            if (not (WM_MBUTTONDOWN == message ? translation.apply("WM_MBUTTONDOWN", "Windows", "Middle (Mouse) Button Down") : true))
            return "";
          #endif

          #ifdef WM_MBUTTONUP
            if (not (WM_MBUTTONUP == message ? translation.apply("WM_MBUTTONUP", "Windows", "Middle (Mouse) Button Up") : true))
            return "";
          #endif

          #ifdef WM_MDIACTIVATE
            if (not (WM_MDIACTIVATE == message ? translation.apply("WM_MDIACTIVATE", "Windows", "Multiple-Document Interface Activate") : true))
            return "";
          #endif

          #ifdef WM_MDICASCADE
            if (not (WM_MDICASCADE == message ? translation.apply("WM_MDICASCADE", "Windows", "Multiple-Document Interface Cascade") : true))
            return "";
          #endif

          #ifdef WM_MDICREATE
            if (not (WM_MDICREATE == message ? translation.apply("WM_MDICREATE", "Windows", "Multiple-Document Interface Create") : true))
            return "";
          #endif

          #ifdef WM_MDIDESTROY
            if (not (WM_MDIDESTROY == message ? translation.apply("WM_MDIDESTROY", "Windows", "Multiple-Document Interface Destroy") : true))
            return "";
          #endif

          #ifdef WM_MDIGETACTIVE
            if (not (WM_MDIGETACTIVE == message ? translation.apply("WM_MDIGETACTIVE", "Windows", "Multiple-Document Interface Get Active") : true))
            return "";
          #endif

          #ifdef WM_MDIICONARRANGE
            if (not (WM_MDIICONARRANGE == message ? translation.apply("WM_MDIICONARRANGE", "Windows", "Multiple-Document Interface Icon Arrange") : true))
            return "";
          #endif

          #ifdef WM_MDIMAXIMIZE
            if (not (WM_MDIMAXIMIZE == message ? translation.apply("WM_MDIMAXIMIZE", "Windows", "Multiple-Document Interface Maximize") : true))
            return "";
          #endif

          #ifdef WM_MDINEXT
            if (not (WM_MDINEXT == message ? translation.apply("WM_MDINEXT", "Windows", "Multiple-Document Interface Next") : true))
            return "";
          #endif

          #ifdef WM_MDIREFRESHMENU
            if (not (WM_MDIREFRESHMENU == message ? translation.apply("WM_MDIREFRESHMENU", "Windows", "Multiple-Document Interface Refresh Menu") : true))
            return "";
          #endif

          #ifdef WM_MDIRESTORE
            if (not (WM_MDIRESTORE == message ? translation.apply("WM_MDIRESTORE", "Windows", "Multiple-Document Interface Restore") : true))
            return "";
          #endif

          #ifdef WM_MDISETMENU
            if (not (WM_MDISETMENU == message ? translation.apply("WM_MDISETMENU", "Windows", "Multiple-Document Interface Set Menu") : true))
            return "";
          #endif

          #ifdef WM_MDITILE
            if (not (WM_MDITILE == message ? translation.apply("WM_MDITILE", "Windows", "Multiple-Document Interface Tile") : true))
            return "";
          #endif

          #ifdef WM_MEASUREITEM
            if (not (WM_MEASUREITEM == message ? translation.apply("WM_MEASUREITEM", "Windows", "Measure Item") : true))
            return "";
          #endif

          #ifdef WM_MENUCHAR
            if (not (WM_MENUCHAR == message ? translation.apply("WM_MENUCHAR", "Windows", "Menu Character") : true))
            return "";
          #endif

          #ifdef WM_MENUCOMMAND
            if (not (WM_MENUCOMMAND == message ? translation.apply("WM_MENUCOMMAND", "Windows", "Menu Command") : true))
            return "";
          #endif

          #ifdef WM_MENUDRAG
            if (not (WM_MENUDRAG == message ? translation.apply("WM_MENUDRAG", "Windows", "Menu Drag") : true))
            return "";
          #endif

          #ifdef WM_MENUGETOBJECT
            if (not (WM_MENUGETOBJECT == message ? translation.apply("WM_MENUGETOBJECT", "Windows", "Menu Get to Object") : true))
            return "";
          #endif

          #ifdef WM_MENURBUTTONUP
            if (not (WM_MENURBUTTONUP == message ? translation.apply("WM_MENURBUTTONUP", "Windows", "Menu Right (Mouse) Button Up") : true))
            return "";
          #endif

          #ifdef WM_MENUSELECT
            if (not (WM_MENUSELECT == message ? translation.apply("WM_MENUSELECT", "Windows", "Menu Select") : true))
            return "";
          #endif

          #ifdef WM_MOUSEACTIVATE
            if (not (WM_MOUSEACTIVATE == message ? translation.apply("WM_MOUSEACTIVATE", "Windows", "Mouse Activate") : true))
            return "";
          #endif

          #ifdef WM_MOUSEFIRST
            if (not (WM_MOUSEFIRST == message ? translation.apply("WM_MOUSEFIRST", "Windows", "Mouse 1st") : true))
            return "";
          #endif

          #ifdef WM_MOUSEHOVER
            if (not (WM_MOUSEHOVER == message ? translation.apply("WM_MOUSEHOVER", "Windows", "Mouse Hover") : true))
            return "";
          #endif

          #ifdef WM_MOUSELAST
            if (not (WM_MOUSELAST == message ? translation.apply("WM_MOUSELAST", "Windows", "Mouse nth") : true))
            return "";
          #endif

          #ifdef WM_MOUSELEAVE
            if (not (WM_MOUSELEAVE == message ? translation.apply("WM_MOUSELEAVE", "Windows", "Mouse Leave") : true))
            return "";
          #endif

          #ifdef WM_MOUSEWHEEL
            if (not (WM_MOUSEWHEEL == message ? translation.apply("WM_MOUSEWHEEL", "Windows", "Mouse Wheel") : true))
            return "";
          #endif

          #ifdef WM_MOVE
            if (not (WM_MOVE == message ? translation.apply("WM_MOVE", "Windows", "Move") : true))
            return "";
          #endif

          #ifdef WM_MOVING
            if (not (WM_MOVING == message ? translation.apply("WM_MOVING", "Windows", "Moving") : true))
            return "";
          #endif

          #ifdef WM_NCACTIVATE
            if (not (WM_NCACTIVATE == message ? translation.apply("WM_NCACTIVATE", "Windows", "Non-Client Activate") : true))
            return "";
          #endif

          #ifdef WM_NCCALCSIZE
            if (not (WM_NCCALCSIZE == message ? translation.apply("WM_NCCALCSIZE", "Windows", "Non-Client Calculate Size") : true))
            return "";
          #endif

          #ifdef WM_NCCREATE
            if (not (WM_NCCREATE == message ? translation.apply("WM_NCCREATE", "Windows", "Non-Client Create") : true))
            return "";
          #endif

          #ifdef WM_NCDESTROY
            if (not (WM_NCDESTROY == message ? translation.apply("WM_NCDESTROY", "Windows", "Non-Client Destroy") : true))
            return "";
          #endif

          #ifdef WM_NCHITTEST
            if (not (WM_NCHITTEST == message ? translation.apply("WM_NCHITTEST", "Windows", "Non-Client Hit-Test") : true))
            return "";
          #endif

          #ifdef WM_NCLBUTTONDBLCLK
            if (not (WM_NCLBUTTONDBLCLK == message ? translation.apply("WM_NCLBUTTONDBLCLK", "Windows", "Non-Client Left (Mouse) Button Double-Click") : true))
            return "";
          #endif

          #ifdef WM_NCLBUTTONDOWN
            if (not (WM_NCLBUTTONDOWN == message ? translation.apply("WM_NCLBUTTONDOWN", "Windows", "Non-Client Left (Mouse) Button Down") : true))
            return "";
          #endif

          #ifdef WM_NCLBUTTONUP
            if (not (WM_NCLBUTTONUP == message ? translation.apply("WM_NCLBUTTONUP", "Windows", "Non-Client Left (Mouse) Button Up") : true))
            return "";
          #endif

          #ifdef WM_NCMBUTTONDBLCLK
            if (not (WM_NCMBUTTONDBLCLK == message ? translation.apply("WM_NCMBUTTONDBLCLK", "Windows", "Non-Client Middle (Mouse) Button Double-Click") : true))
            return "";
          #endif

          #ifdef WM_NCMBUTTONDOWN
            if (not (WM_NCMBUTTONDOWN == message ? translation.apply("WM_NCMBUTTONDOWN", "Windows", "Non-Client Middle (Mouse) Button Down") : true))
            return "";
          #endif

          #ifdef WM_NCMBUTTONUP
            if (not (WM_NCMBUTTONUP == message ? translation.apply("WM_NCMBUTTONUP", "Windows", "Non-Client Middle (Mouse) Button Up") : true))
            return "";
          #endif

          #ifdef WM_NCMOUSEHOVER
            if (not (WM_NCMOUSEHOVER == message ? translation.apply("WM_NCMOUSEHOVER", "Windows", "Non-Client Mouse Hover") : true))
            return "";
          #endif

          #ifdef WM_NCMOUSELEAVE
            if (not (WM_NCMOUSELEAVE == message ? translation.apply("WM_NCMOUSELEAVE", "Windows", "Non-Client Mouse Leave") : true))
            return "";
          #endif

          #ifdef WM_NCMOUSEMOVE
            if (not (WM_NCMOUSEMOVE == message ? translation.apply("WM_NCMOUSEMOVE", "Windows", "Non-Client Mouse Mouse") : true))
            return "";
          #endif

          #ifdef WM_NCPAINT
            if (not (WM_NCPAINT == message ? translation.apply("WM_NCPAINT", "Windows", "Non-Client Paint") : true))
            return "";
          #endif

          #ifdef WM_NCRBUTTONDBLCLK
            if (not (WM_NCRBUTTONDBLCLK == message ? translation.apply("WM_NCRBUTTONDBLCLK", "Windows", "Non-Client Right (Mouse) Button Double-Click") : true))
            return "";
          #endif

          #ifdef WM_NCRBUTTONDOWN
            if (not (WM_NCRBUTTONDOWN == message ? translation.apply("WM_NCRBUTTONDOWN", "Windows", "Non-Client Right (Mouse) Button Down") : true))
            return "";
          #endif

          #ifdef WM_NCRBUTTONUP
            if (not (WM_NCRBUTTONUP == message ? translation.apply("WM_NCRBUTTONUP", "Windows", "Non-Client Right (Mouse) Button Up") : true))
            return "";
          #endif

          #ifdef WM_NCXBUTTONDBLCLK
            if (not (WM_NCXBUTTONDBLCLK == message ? translation.apply("WM_NCXBUTTONDBLCLK", "Windows", "Non-Client X (Mouse) Button Double-Click") : true))
            return "";
          #endif

          #ifdef WM_NCXBUTTONDOWN
            if (not (WM_NCXBUTTONDOWN == message ? translation.apply("WM_NCXBUTTONDOWN", "Windows", "Non-Client X (Mouse) Button Down") : true))
            return "";
          #endif

          #ifdef WM_NCXBUTTONUP
            if (not (WM_NCXBUTTONUP == message ? translation.apply("WM_NCXBUTTONUP", "Windows", "Non-Client X (Mouse) Button Up") : true))
            return "";
          #endif

          #ifdef WM_NEXTDLGCTL
            if (not (WM_NEXTDLGCTL == message ? translation.apply("WM_NEXTDLGCTL", "Windows", "Next Dialog Control") : true))
            return "";
          #endif

          #ifdef WM_NEXTMENU
            if (not (WM_NEXTMENU == message ? translation.apply("WM_NEXTMENU", "Windows", "Next Menu") : true))
            return "";
          #endif

          #ifdef WM_NOTIFY
            if (not (WM_NOTIFY == message ? translation.apply("WM_NOTIFY", "Windows", "Notify") : true))
            return "";
          #endif

          #ifdef WM_NOTIFYFORMAT
            if (not (WM_NOTIFYFORMAT == message ? translation.apply("WM_NOTIFYFORMAT", "Windows", "Notify Format") : true))
            return "";
          #endif

          #ifdef WM_NULL
            if (not (WM_NULL == message ? translation.apply("WM_NULL", "Windows", "[NULL]") : true))
            return "";
          #endif

          #ifdef WM_PAINT
            if (not (WM_PAINT == message ? translation.apply("WM_PAINT", "Windows", "Paint") : true))
            return "";
          #endif

          #ifdef WM_PAINTCLIPBOARD
            if (not (WM_PAINTCLIPBOARD == message ? translation.apply("WM_PAINTCLIPBOARD", "Windows", "Paint Clipboard") : true))
            return "";
          #endif

          #ifdef WM_PAINTICON
            if (not (WM_PAINTICON == message ? translation.apply("WM_PAINTICON", "Windows", "Paint Icon") : true))
            return "";
          #endif

          #ifdef WM_PALETTECHANGED
            if (not (WM_PALETTECHANGED == message ? translation.apply("WM_PALETTECHANGED", "Windows", "Palette Changed") : true))
            return "";
          #endif

          #ifdef WM_PALETTEISCHANGING
            if (not (WM_PALETTEISCHANGING == message ? translation.apply("WM_PALETTEISCHANGING", "Windows", "Palette is Changing") : true))
            return "";
          #endif

          #ifdef WM_PARENTNOTIFY
            if (not (WM_PARENTNOTIFY == message ? translation.apply("WM_PARENTNOTIFY", "Windows", "Parent Notify") : true))
            return "";
          #endif

          #ifdef WM_PASTE
            if (not (WM_PASTE == message ? translation.apply("WM_PASTE", "Windows", "Paste") : true))
            return "";
          #endif

          #ifdef WM_PENCTL
            if (not (WM_PENCTL == message ? translation.apply("WM_PENCTL", "Windows", "Pen Control") : true))
            return "";
          #endif

          #ifdef WM_PENEVENT
            if (not (WM_PENEVENT == message ? translation.apply("WM_PENEVENT", "Windows", "Pen Event") : true))
            return "";
          #endif

          #ifdef WM_PENMISC
            if (not (WM_PENMISC == message ? translation.apply("WM_PENMISC", "Windows", "Pen Miscellaneous") : true))
            return "";
          #endif

          #ifdef WM_PENMISCINFO
            if (not (WM_PENMISCINFO == message ? translation.apply("WM_PENMISCINFO", "Windows", "Pen Miscellaneous Information") : true))
            return "";
          #endif

          #ifdef WM_PENWINFIRST
            if (not (WM_PENWINFIRST == message ? translation.apply("WM_PENWINFIRST", "Windows", "Pen [FIRST]") : true))
            return "";
          #endif

          #ifdef WM_PENWINLAST
            if (not (WM_PENWINLAST == message ? translation.apply("WM_PENWINLAST", "Windows", "Pen [LAST]") : true))
            return "";
          #endif

          #ifdef WM_POWER
            if (not (WM_POWER == message ? translation.apply("WM_POWER", "Windows", "Power") : true))
            return "";
          #endif

          #ifdef WM_POWERBROADCAST
            if (not (WM_POWERBROADCAST == message ? translation.apply("WM_POWERBROADCAST", "Windows", "Power Broadcast") : true))
            return "";
          #endif

          #ifdef WM_PRINT
            if (not (WM_PRINT == message ? translation.apply("WM_PRINT", "Windows", "Print") : true))
            return "";
          #endif

          #ifdef WM_PRINTCLIENT
            if (not (WM_PRINTCLIENT == message ? translation.apply("WM_PRINTCLIENT", "Windows", "Print Client") : true))
            return "";
          #endif

          #ifdef WM_PSD_ENVSTAMPRECT
            if (not (WM_PSD_ENVSTAMPRECT == message ? translation.apply("WM_PSD_ENVSTAMPRECT", "Windows Page Setup Dialog", "Envelope Stamp Rectangle") : true))
            return "";
          #endif

          #ifdef WM_PSD_FULLPAGERECT
            if (not (WM_PSD_FULLPAGERECT == message ? translation.apply("WM_PSD_FULLPAGERECT", "Windows Page Setup Dialog", "Full Page Rectangle") : true))
            return "";
          #endif

          #ifdef WM_PSD_GREEKTEXTRECT
            if (not (WM_PSD_GREEKTEXTRECT == message ? translation.apply("WM_PSD_GREEKTEXTRECT", "Windows Page Setup Dialog", "Greek Text Rectangle") : true))
            return "";
          #endif

          #ifdef WM_PSD_MARGINRECT
            if (not (WM_PSD_MARGINRECT == message ? translation.apply("WM_PSD_MARGINRECT", "Windows Page Setup Dialog", "Margin Rectangle") : true))
            return "";
          #endif

          #ifdef WM_PSD_MINMARGINRECT
            if (not (WM_PSD_MINMARGINRECT == message ? translation.apply("WM_PSD_MINMARGINRECT", "Windows Page Setup Dialog", "Minimum Margin Rectangle") : true))
            return "";
          #endif

          #ifdef WM_PSD_PAGESETUPDLG
            if (not (WM_PSD_PAGESETUPDLG == message ? translation.apply("WM_PSD_PAGESETUPDLG", "Windows Page Setup Dialog", "Page Setup Dialog") : true))
            return "";
          #endif

          #ifdef WM_PSD_YAFULLPAGERECT
            if (not (WM_PSD_YAFULLPAGERECT == message ? translation.apply("WM_PSD_YAFULLPAGERECT", "Windows Page Setup Dialog", "Full Page Rectangle") : true))
            return "";
          #endif

          #ifdef WM_QUERYDRAGICON
            if (not (WM_QUERYDRAGICON == message ? translation.apply("WM_QUERYDRAGICON", "Windows", "Query Drag Icon") : true))
            return "";
          #endif

          #ifdef WM_QUERYENDSESSION
            if (not (WM_QUERYENDSESSION == message ? translation.apply("WM_QUERYENDSESSION", "Windows", "Query End Session") : true))
            return "";
          #endif

          #ifdef WM_QUERYNEWPALETTE
            if (not (WM_QUERYNEWPALETTE == message ? translation.apply("WM_QUERYNEWPALETTE", "Windows", "Query New Palette") : true))
            return "";
          #endif

          #ifdef WM_QUERYOPEN
            if (not (WM_QUERYOPEN == message ? translation.apply("WM_QUERYOPEN", "Windows", "Query Open") : true))
            return "";
          #endif

          #ifdef WM_QUERYUISTATE
            if (not (WM_QUERYUISTATE == message ? translation.apply("WM_QUERYUISTATE", "Windows", "Query User-Interface State") : true))
            return "";
          #endif

          #ifdef WM_QUEUESYNC
            if (not (WM_QUEUESYNC == message ? translation.apply("WM_QUEUESYNC", "Windows", "Queue Synchronization") : true))
            return "";
          #endif

          #ifdef WM_QUIT
            if (not (WM_QUIT == message ? translation.apply("WM_QUIT", "Windows", "Quit") : true))
            return "";
          #endif

          #ifdef WM_RASDIALEVENT
            if (not (WM_RASDIALEVENT == message ? translation.apply("WM_RASDIALEVENT", "Windows", "Routing & Remote Access Service Dial Event") : true))
            return "";
          #endif

          #ifdef WM_RBUTTONDBLCLK
            if (not (WM_RBUTTONDBLCLK == message ? translation.apply("WM_RBUTTONDBLCLK", "Windows", "Right (Mouse) Button Double-Click") : true))
            return "";
          #endif

          #ifdef WM_RBUTTONDOWN
            if (not (WM_RBUTTONDOWN == message ? translation.apply("WM_RBUTTONDOWN", "Windows", "Right (Mouse) Button Down") : true))
            return "";
          #endif

          #ifdef WM_RBUTTONUP
            if (not (WM_RBUTTONUP == message ? translation.apply("WM_RBUTTONUP", "Windows", "Right (Mouse) Button Up") : true))
            return "";
          #endif

          #ifdef WM_RCRESULT
            if (not (WM_RCRESULT == message ? translation.apply("WM_RCRESULT", "Windows", "RC Result") : true))
            return "";
          #endif

          #ifdef WM_RENDERALLFORMATS
            if (not (WM_RENDERALLFORMATS == message ? translation.apply("WM_RENDERALLFORMATS", "Windows", "Render All Formats") : true))
            return "";
          #endif

          #ifdef WM_RENDERFORMAT
            if (not (WM_RENDERFORMAT == message ? translation.apply("WM_RENDERFORMAT", "Windows", "Render Format") : true))
            return "";
          #endif

          #ifdef WM_SETCURSOR
            if (not (WM_SETCURSOR == message ? translation.apply("WM_SETCURSOR", "Windows", "Set Cursor") : true))
            return "";
          #endif

          #ifdef WM_SETFOCUS
            if (not (WM_SETFOCUS == message ? translation.apply("WM_SETFOCUS", "Windows", "Set Focus") : true))
            return "";
          #endif

          #ifdef WM_SETFONT
            if (not (WM_SETFONT == message ? translation.apply("WM_SETFONT", "Windows", "Set Font") : true))
            return "";
          #endif

          #ifdef WM_SETHOTKEY
            if (not (WM_SETHOTKEY == message ? translation.apply("WM_SETHOTKEY", "Windows", "Set Hotkey") : true))
            return "";
          #endif

          #ifdef WM_SETICON
            if (not (WM_SETICON == message ? translation.apply("WM_SETICON", "Windows", "Set Icon") : true))
            return "";
          #endif

          #ifdef WM_SETREDRAW
            if (not (WM_SETREDRAW == message ? translation.apply("WM_SETREDRAW", "Windows", "Set Redraw") : true))
            return "";
          #endif

          #ifdef WM_SETTEXT
            if (not (WM_SETTEXT == message ? translation.apply("WM_SETTEXT", "Windows", "Set Text") : true))
            return "";
          #endif

          #ifdef WM_SHOWWINDOW
            if (not (WM_SHOWWINDOW == message ? translation.apply("WM_SHOWWINDOW", "Windows", "Show Window") : true))
            return "";
          #endif

          #ifdef WM_SIZE
            if (not (WM_SIZE == message ? translation.apply("WM_SIZE", "Windows", "Size") : true))
            return "";
          #endif

          #ifdef WM_SIZECLIPBOARD
            if (not (WM_SIZECLIPBOARD == message ? translation.apply("WM_SIZECLIPBOARD", "Windows", "Size Clipboard") : true))
            return "";
          #endif

          #ifdef WM_SIZING
            if (not (WM_SIZING == message ? translation.apply("WM_SIZING", "Windows", "Sizing") : true))
            return "";
          #endif

          #ifdef WM_SKB
            if (not (WM_SKB == message ? translation.apply("WM_SKB", "Windows", "SketchUp Backup") : true))
            return "";
          #endif

          #ifdef WM_SPOOLERSTATUS
            if (not (WM_SPOOLERSTATUS == message ? translation.apply("WM_SPOOLERSTATUS", "Windows", "Spooler Status") : true))
            return "";
          #endif

          #ifdef WM_STYLECHANGED
            if (not (WM_STYLECHANGED == message ? translation.apply("WM_STYLECHANGED", "Windows", "Style Changed") : true))
            return "";
          #endif

          #ifdef WM_STYLECHANGING
            if (not (WM_STYLECHANGING == message ? translation.apply("WM_STYLECHANGING", "Windows", "Style Changing") : true))
            return "";
          #endif

          #ifdef WM_SYNCPAINT
            if (not (WM_SYNCPAINT == message ? translation.apply("WM_SYNCPAINT", "Windows", "Synchronized Paint") : true))
            return "";
          #endif

          #ifdef WM_SYSCHAR
            if (not (WM_SYSCHAR == message ? translation.apply("WM_SYSCHAR", "Windows", "System Character") : true))
            return "";
          #endif

          #ifdef WM_SYSCOLORCHANGE
            if (not (WM_SYSCOLORCHANGE == message ? translation.apply("WM_SYSCOLORCHANGE", "Windows", "System Color Change") : true))
            return "";
          #endif

          #ifdef WM_SYSCOMMAND
            if (not (WM_SYSCOMMAND == message ? translation.apply("WM_SYSCOMMAND", "Windows", "System Command") : true))
            return "";
          #endif

          #ifdef WM_SYSDEADCHAR
            if (not (WM_SYSDEADCHAR == message ? translation.apply("WM_SYSDEADCHAR", "Windows", "System Dead Character") : true))
            return "";
          #endif

          #ifdef WM_SYSKEYDOWN
            if (not (WM_SYSKEYDOWN == message ? translation.apply("WM_SYSKEYDOWN", "Windows", "System Key Down") : true))
            return "";
          #endif

          #ifdef WM_SYSKEYUP
            if (not (WM_SYSKEYUP == message ? translation.apply("WM_SYSKEYUP", "Windows", "System Key Up") : true))
            return "";
          #endif

          #ifdef WM_SYSTIMER
            if (not (WM_SYSTIMER == message ? translation.apply("WM_SYSTIMER", "Windows", "System Timer") : true))
            return "";
          #endif

          #ifdef WM_TCARD
            if (not (WM_TCARD == message ? translation.apply("WM_TCARD", "Windows", "Text Card") : true))
            return "";
          #endif

          #ifdef WM_TIMECHANGE
            if (not (WM_TIMECHANGE == message ? translation.apply("WM_TIMECHANGE", "Windows", "Time Change") : true))
            return "";
          #endif

          #ifdef WM_TIMER
            if (not (WM_TIMER == message ? translation.apply("WM_TIMER", "Windows", "Timer") : true))
            return "";
          #endif

          #ifdef WM_UNDO
            if (not (WM_UNDO == message ? translation.apply("WM_UNDO", "Windows", "Undo") : true))
            return "";
          #endif

          #ifdef WM_UNICHAR
            if (not (WM_UNICHAR == message ? translation.apply("WM_UNICHAR", "Windows", "Unicode Character") : true))
            return "";
          #endif

          #ifdef WM_UNINITMENUPOPUP
            if (not (WM_UNINITMENUPOPUP == message ? translation.apply("WM_UNINITMENUPOPUP", "Windows", "Uninitiate Menu Pop-Up") : true))
            return "";
          #endif

          #ifdef WM_UPDATEUISTATE
            if (not (WM_UPDATEUISTATE == message ? translation.apply("WM_UPDATEUISTATE", "Windows", "Update UI State") : true))
            return "";
          #endif

          #ifdef WM_USER
            if (not (WM_USER == message ? translation.apply("WM_USER", "Windows", "User") : true))
            return "";
          #endif

          #ifdef WM_USERCHANGED
            if (not (WM_USERCHANGED == message ? translation.apply("WM_USERCHANGED", "Windows", "User Changed") : true))
            return "";
          #endif

          #ifdef WM_VKEYTOITEM
            if (not (WM_VKEYTOITEM == message ? translation.apply("WM_VKEYTOITEM", "Windows", "Virtual Key to Item") : true))
            return "";
          #endif

          #ifdef WM_VSCROLL
            if (not (WM_VSCROLL == message ? translation.apply("WM_VSCROLL", "Windows", "Vertical Scroll") : true))
            return "";
          #endif

          #ifdef WM_VSCROLLCLIPBOARD
            if (not (WM_VSCROLLCLIPBOARD == message ? translation.apply("WM_VSCROLLCLIPBOARD", "Windows", "Vertical Scroll Clipboard") : true))
            return "";
          #endif

          #ifdef WM_WINDOWPOSCHANGED
            if (not (WM_WINDOWPOSCHANGED == message ? translation.apply("WM_WINDOWPOSCHANGED", "Windows", "Window Position Changed") : true))
            return "";
          #endif

          #ifdef WM_WINDOWPOSCHANGING
            if (not (WM_WINDOWPOSCHANGING == message ? translation.apply("WM_WINDOWPOSCHANGING", "Windows", "Window Position Changing") : true))
            return "";
          #endif

          #ifdef WM_WININICHANGE
            if (not (WM_WININICHANGE == message ? translation.apply("WM_WININICHANGE", "Windows", "Windows INI Change") : true))
            return "";
          #endif

          #ifdef WM_WNT_CONVERTREQUESTEX
            if (not (WM_WNT_CONVERTREQUESTEX == message ? translation.apply("WM_WNT_CONVERTREQUESTEX", "Windows", "Convert Request") : true))
            return "";
          #endif

          #ifdef WM_XBUTTONDOWN
            if (not (WM_XBUTTONDOWN == message ? translation.apply("WM_XBUTTONDOWN", "Windows", "Extended Button Down") : true))
            return "";
          #endif

          #ifdef WM_XBUTTONUP
            if (not (WM_XBUTTONUP == message ? translation.apply("WM_XBUTTONUP", "Windows", "Extended Button Up") : true))
            return "";
          #endif

          if (0u == translation.length or translation.length == sizeof translation.value / sizeof(char))
            return NULL;

          translation.value[++translation.length] = '\0';
          return translation.value;
        }
      };

      typedef log::number    number;
      typedef log::special   special;
      typedef log::undefined undefined;
      typedef log::unused    unused;

      char const   *messageTranslation                = log::translate(message);
      bool          messageTranslationMultilineFormat = false;
      static LPWSTR windowText                        = NULL; // ->> Ideally `HeapFree(…)`-ed on program termination
      bool          windowTextAcquired                = false;
      static SIZE_T windowTextCapacity                = 0L;
      static HANDLE windowTextHeap                    = NULL; // ->> Ideally `HeapDestroy(…)`-ed on program termination
      LRESULT       windowTextLength                  = 0L;   // --> int
      LRESULT const windowTextLengthMaximum           = (static_cast<std::size_t>(1u) << ((CHAR_BIT * sizeof(LRESULT)) - 1u)) - 1u;
      SIZE_T        windowTextSize                    = 0u;
      bool const    windowUsesProcedure               = &procedure == reinterpret_cast<WNDPROC>(::GetWindowLongPtrW(windowHandle, GWLP_WNDPROC));

      // ... --> windowText… = …;
      if (NULL != windowHandle) {
        windowTextHeap = NULL == windowTextHeap ? ::HeapCreate(HEAP_NO_SERIALIZE, 0u, 0u) : windowTextHeap;
        windowTextHeap = NULL == windowTextHeap ? ::GetProcessHeap()                      : windowTextHeap;

        // ... ->> Get `windowText` via `::SendMessage(…)` (or `::DefWindowProcW(…)`) requests
        if (not windowTextAcquired and NULL != windowTextHeap) {
          windowTextLength = (windowUsesProcedure ? ::DefWindowProcW : ::SendMessage)(windowHandle, WM_GETTEXTLENGTH, 0x00u, 0x00L); // ->> NUL character omitted

          if (ERROR_ACCESS_DENIED != ::GetLastError())
          if (windowTextLength < windowTextLengthMaximum - 1L) {
            windowTextSize = static_cast<std::size_t>(windowTextLength + 1L);

            if (windowTextCapacity < windowTextSize) {
              LPVOID const allocation = NULL == windowText ? ::HeapAlloc(windowTextHeap, HEAP_ZERO_MEMORY, static_cast<SIZE_T>(windowTextSize) /* ->> Less than `0x7FFF8u`? */) : ::HeapReAlloc(windowTextHeap, HEAP_ZERO_MEMORY, windowText, static_cast<SIZE_T>(windowTextSize));

              // ...
              if (NULL != allocation) {
                windowText         = static_cast<LPWSTR>(allocation);
                windowTextCapacity = windowTextSize;
              }
            }

            // ...
            if (windowTextCapacity >= windowTextSize)
            windowTextAcquired = windowTextLength >= (windowUsesProcedure ? ::DefWindowProcW : ::SendMessage)(windowHandle, WM_GETTEXT, static_cast<WPARAM>(windowTextCapacity), static_cast<LPARAM>(reinterpret_cast<uintptr_t>(static_cast<void*>(windowText)))) ? ERROR_ACCESS_DENIED != ::GetLastError() : false;
          }
        }

        // ... ->> Get `windowText` via `::GetWindowText…(…)` functions
        if (not windowTextAcquired and NULL != windowTextHeap) {
          ::SetLastError(0x0000u /* --> ERROR_SUCCESS */);
          windowTextLength = not windowUsesProcedure ? ::GetWindowTextLengthW(windowHandle) : 0; // ->> Presumably NUL character omitted

          if (0 != windowTextLength and 0x0000u /* --> ERROR_SUCCESS */ == ::GetLastError())
          if (windowTextLength < windowTextLengthMaximum - 1L) {
            windowTextSize = static_cast<std::size_t>(windowTextLength + 1);

            if (windowTextCapacity < windowTextSize) {
              LPVOID const allocation = NULL == windowText ? ::HeapAlloc(windowTextHeap, HEAP_ZERO_MEMORY, static_cast<SIZE_T>(windowTextSize) /* ->> Less than `0x7FFF8u`? */) : ::HeapReAlloc(windowTextHeap, HEAP_ZERO_MEMORY, windowText, static_cast<SIZE_T>(windowTextSize));

              // ...
              if (NULL != allocation) {
                windowText         = static_cast<LPWSTR>(allocation);
                windowTextCapacity = windowTextSize;
              }
            }

            // ...
            if (windowTextCapacity >= windowTextSize)
            windowTextAcquired = not windowUsesProcedure ? 0 != ::GetWindowTextW(windowHandle, windowText, static_cast<int>(windowTextCapacity)) : false;
          }
        }
      }

      if (windowTextAcquired) {
        // ... ->> Replace common ASCII control characters within `windowText` to `L'\\'`
        for (LPWSTR iterator = windowText; L'\0' != *iterator; ++iterator)
        switch (*iterator) { case L'\b': case L'\n': case L'\r': case L'\v': *iterator = L'\\'; break; }

        // ... ->> Truncate arbitrarily long `windowText` --> windowText[64..] = L"...";
        if (windowTextLength > 64L - 3L) {
          windowText[64 - 3] = L'.';
          windowText[64 - 2] = L'.';
          windowText[64 - 1] = L'.';
          windowText[64 - 0] = L'\0';
        }
      }

      // ... ->> Log the `message`
      if (NULL == messageTranslation)
        (void) log::output(log::format(static_cast<undefined>(message), " {"
          "window"    ": ", windowHandle, windowTextAcquired ? " \"" : "", windowTextAcquired ? windowText : L"", windowTextAcquired ? "\"" : "", ", "
          "arguments" ": [", static_cast<undefined>(parameter), ", ", static_cast<undefined>(subparameter), "]"
        "}" "\r\n"));

      else while ('\0' != *messageTranslation) {
        char const *const messageConstantTranslation = messageTranslation;
        char const       *messageEventTranslation    = NULL;
        char const       *messageTypeTranslation     = NULL;

        // ... --> messageEventTranslation = …; messageTypeTranslation = …;
        for (messageTypeTranslation  = messageConstantTranslation; '\0' != *(messageTypeTranslation++); )  continue;
        for (messageEventTranslation = messageTypeTranslation;     '\0' != *(messageEventTranslation++); ) continue;

        // ...
        if (log::output(log::format(
          static_cast<undefined>(message), " {"
            "window"    ": ",  windowHandle, windowTextAcquired ? " \"" : "", windowTextAcquired ? windowText : L"", windowTextAcquired ? "\"" : "", ", "
            "name"      ": `", messageConstantTranslation, "` \"", messageTypeTranslation, ": ", messageEventTranslation, "\""                       ", "
            "arguments" ": "
        ))) {
          if (false) {
            /* Do nothing… */
          }

          else if (WM_ACTIVATE == message) {
            WORD const minimizationState = LOWORD(parameter);
            BOOL const minimized         = HIWORD(parameter);
            HWND const windowHandle      = static_cast<HWND>(reinterpret_cast<void*>(static_cast<uintptr_t>(subparameter)));

            messageTranslationMultilineFormat = not log::output(log::format('[', static_cast<bool>(minimized), " {state: ", static_cast<special>(minimizationState),
              WA_ACTIVE      == minimizationState ? " " "`WA_ACTIVE`"      : // ->>
              WA_CLICKACTIVE == minimizationState ? " " "`WA_CLICKACTIVE`" : // ->>
              WA_INACTIVE    == minimizationState ? " " "`WA_INACTIVE`"    : // ->>
              "",
            "}, ", windowHandle, ']'));
          }

          else if (WM_ACTIVATEAPP == message) {
            BOOL  const minimized = static_cast<BOOL> (parameter);
            DWORD const threadID  = static_cast<DWORD>(subparameter);

            messageTranslationMultilineFormat = not log::output(log::format('[', static_cast<bool>(minimized), ", ", static_cast<special>(threadID), ']'));
          }

          else if (WM_CAPTURECHANGED == message) {
            HWND const capturingWindowHandle = static_cast<HWND>(reinterpret_cast<void*>(static_cast<uintptr_t>(subparameter)));
            messageTranslationMultilineFormat = not log::output(log::format('[', static_cast<unused>(parameter), ", ", capturingWindowHandle, ']'));
          }

          else if (
            WM_CLOSE         == message or
            WM_DESTROY       == message or
            WM_GETTEXTLENGTH == message or
            WM_NCDESTROY     == message
          ) messageTranslationMultilineFormat = not log::output(log::format('[', static_cast<unused>(parameter), ", ", static_cast<unused>(subparameter), ']'));

          else if (WM_ERASEBKGND == message) {
            HDC const deviceContextHandle = static_cast<HDC>(reinterpret_cast<void*>(static_cast<uintptr_t>(parameter)));
            messageTranslationMultilineFormat = not log::output(log::format('[', deviceContextHandle, ", ", static_cast<unused>(subparameter), ']'));
          }

          else if (WM_GETICON == message) {
            LPARAM const iconDPI  = subparameter;
            WPARAM const iconType = parameter;

            messageTranslationMultilineFormat = not log::output(log::format('[', static_cast<special>(iconType),
              ICON_BIG    == iconType ? " " "`ICON_BIG`"    : // ->> Window's large icon
              ICON_SMALL  == iconType ? " " "`ICON_SMALL`"  : // ->> Window's small icon
              ICON_SMALL2 == iconType ? " " "`ICON_SMALL2`" : // ->> Window's small or otherwise system-generated icon
              "",
            ", ", (number) iconDPI, ']'));
          }

          else if (WM_GETMINMAXINFO == message) {
            MINMAXINFO *const minimumMaximumInformation = static_cast<MINMAXINFO*>(reinterpret_cast<void*>(static_cast<uintptr_t>(subparameter))); // --> std::launder(…)

            // ...
            messageTranslationMultilineFormat = true;

            (void) log::output(log::format(
              ""   "[", static_cast<unused>(parameter), ", ", minimumMaximumInformation, " {"                                                                                 "\r\n"
              "  " "maximumPosition : {x: ", (number) minimumMaximumInformation -> ptMaxPosition .x, ", y: ", (number) minimumMaximumInformation -> ptMaxPosition .y, "}" "," "\r\n"
              "  " "maximumSize     : {x: ", (number) minimumMaximumInformation -> ptMaxSize     .x, ", y: ", (number) minimumMaximumInformation -> ptMaxSize     .y, "}" "," "\r\n"
              "  " "maximumTrackSize: {x: ", (number) minimumMaximumInformation -> ptMaxTrackSize.x, ", y: ", (number) minimumMaximumInformation -> ptMaxTrackSize.y, "}" "," "\r\n"
              "  " "minimumTrackSize: {x: ", (number) minimumMaximumInformation -> ptMinTrackSize.x, ", y: ", (number) minimumMaximumInformation -> ptMinTrackSize.y, "}"     "\r\n"
              ""   "}]"
            ));
          }

          else if (WM_GETOBJECT == message) {
            DWORD const flags    = static_cast<WPARAM>(static_cast<DWORD>(parameter));
            DWORD const objectID = static_cast<LPARAM>(static_cast<DWORD>(subparameter)); // ->> Must be casted to `DWORD` to prevent integer sign-extension issues

            // ...
            (void) flags;
            messageTranslationMultilineFormat = not log::output(log::format('[', static_cast<special>(flags), ", ", static_cast<special>(objectID),
              OBJID_ALERT             == objectID ? " " "`OBJID_ALERT`"             : // ->> Application or window's alert
              OBJID_CARET             == objectID ? " " "`OBJID_CARET`"             : // ->> Window's caret
              OBJID_CLIENT            == objectID ? " " "`OBJID_CLIENT`"            : // ->> Window's client area
              OBJID_CURSOR            == objectID ? " " "`OBJID_CURSOR`"            : // ->> Mouse pointer
              OBJID_HSCROLL           == objectID ? " " "`OBJID_HSCROLL`"           : // ->> Window's horizontal scroll bar
              OBJID_NATIVEOM          == objectID ? " " "`OBJID_NATIVEOM`"          : // ->> Third-party object model
              OBJID_MENU              == objectID ? " " "`OBJID_MENU`"              : // ->> Window's menu bar
              OBJID_QUERYCLASSNAMEIDX == objectID ? " " "`OBJID_QUERYCLASSNAMEIDX`" : //
              OBJID_SIZEGRIP          == objectID ? " " "`OBJID_SIZEGRIP`"          : // ->> Window's size grip
              OBJID_SOUND             == objectID ? " " "`OBJID_SOUND`"             : // ->> Application's sound object
              OBJID_SYSMENU           == objectID ? " " "`OBJID_SYSMENU`"           : // ->> Window's ystem menu
              OBJID_TITLEBAR          == objectID ? " " "`OBJID_TITLEBAR`"          : // ->> Window's title bar
              OBJID_VSCROLL           == objectID ? " " "`OBJID_VSCROLL`"           : // ->> Window's vertical scroll bar
              OBJID_WINDOW            == objectID ? " " "`OBJID_WINDOW`"            : // ->> Window
              "",
            ']'));

            // TODO (Lapys) -> https://learn.microsoft.com/en-us/windows/win32/winauto/handling-the-wm-getobject-message
            // static_cast<LPARAM>(objectID) == static_cast<LPARAM>(::UiaRootObjectId)
          }

          else if (WM_GETTEXT == message)
            messageTranslationMultilineFormat = not log::output(log::format('[', (number) parameter, ", ", reinterpret_cast<void*>(static_cast<uintptr_t>(subparameter)), ']'));

          // else if (WM_IME_NOTIFY == message) {} // TODO (Lapys)
          // else if (WM_IME_REQUEST == message) {} // TODO (Lapys)
          // else if (WM_IME_SETCONTEXT == message) {} // TODO (Lapys)

          else if (
            // CITE (Lapys) -> https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input
            WM_KEYDOWN    == message or
            WM_KEYUP      == message or
            WM_SYSKEYDOWN == message or
            WM_SYSKEYUP   == message
          ) {
            WORD const  keyFlags              = HIWORD(subparameter);                                            //
            BOOL const  extended              = KF_EXTENDED == (LOBYTE(keyFlags) & KF_EXTENDED);                 // ->> `TRUE` if `scanCode` has `0xE0` prefix
            BOOL const  context               = KF_ALTDOWN  == (keyFlags         & KF_ALTDOWN);                  // ->> `TRUE` on `VK_MENU`, always `FALSE` on `WM_KEYDOWN`
            BOOL const  previous              = KF_REPEAT   == (keyFlags         & KF_REPEAT);                   // ->> `TRUE` on auto-repeat
            WORD const  repeatCount           = LOWORD(subparameter);                                            //
            WORD const  scanCode              = extended ? MAKEWORD(LOBYTE(keyFlags), 0xE0u) : LOBYTE(keyFlags); //
            char const *scanCodeTranslation   = NULL;                                                            //
            BOOL const  transition            = KF_UP == (keyFlags & KF_UP);                                     // ->> `TRUE` on key release
            WORD const  virtualKey            = LOWORD(parameter);                                               //
            char const *virtualKeyTranslation = NULL;                                                            //

            // ... --> scanCodeTranslation = …;
            switch (scanCode) {
              case 0x0001u:               scanCodeTranslation = "Key"             ": Keyboard Escape";                                     break;
              case 0x0002u:               scanCodeTranslation = "Key"             ": Keyboard 1 and Bang";                                 break;
              case 0x0003u:               scanCodeTranslation = "Key"             ": Keyboard 2 and At";                                   break;
              case 0x0004u:               scanCodeTranslation = "Key"             ": Keyboard 3 And Hash";                                 break;
              case 0x0005u:               scanCodeTranslation = "Key"             ": Keyboard 4 and Dollar";                               break;
              case 0x0006u:               scanCodeTranslation = "Key"             ": Keyboard 5 and Percent";                              break;
              case 0x0007u:               scanCodeTranslation = "Key"             ": Keyboard 6 and Caret";                                break;
              case 0x0008u:               scanCodeTranslation = "Key"             ": Keyboard 7 and Ampersand";                            break;
              case 0x0009u:               scanCodeTranslation = "Key"             ": Keyboard 8 and Star";                                 break;
              case 0x000Au:               scanCodeTranslation = "Key"             ": Keyboard 9 and Left Bracket";                         break;
              case 0x000Bu:               scanCodeTranslation = "Key"             ": Keyboard 0 and Right Bracket";                        break;
              case 0x000Cu:               scanCodeTranslation = "Key"             ": Keyboard Dash and Underscore";                        break;
              case 0x000Du:               scanCodeTranslation = "Key"             ": Keyboard Equals and Plus";                            break;
              case 0x000Eu:               scanCodeTranslation = "Key"             ": Keyboard Delete";                                     break;
              case 0x000Fu:               scanCodeTranslation = "Key"             ": Keyboard Tab";                                        break;
              case 0x0010u:               scanCodeTranslation = "Key"             ": Keyboard Q";                                          break;
              case 0x0011u:               scanCodeTranslation = "Key"             ": Keyboard W";                                          break;
              case 0x0012u:               scanCodeTranslation = "Key"             ": Keyboard E";                                          break;
              case 0x0013u:               scanCodeTranslation = "Key"             ": Keyboard R";                                          break;
              case 0x0014u:               scanCodeTranslation = "Key"             ": Keyboard T";                                          break;
              case 0x0015u:               scanCodeTranslation = "Key"             ": Keyboard Y";                                          break;
              case 0x0016u:               scanCodeTranslation = "Key"             ": Keyboard U";                                          break;
              case 0x0017u:               scanCodeTranslation = "Key"             ": Keyboard I";                                          break;
              case 0x0018u:               scanCodeTranslation = "Key"             ": Keyboard O";                                          break;
              case 0x0019u:               scanCodeTranslation = "Key"             ": Keyboard P";                                          break;
              case 0x001Au:               scanCodeTranslation = "Key"             ": Keyboard Left Brace";                                 break;
              case 0x001Bu:               scanCodeTranslation = "Key"             ": Keyboard Right Brace";                                break;
              case 0x001Cu:               scanCodeTranslation = "Key"             ": Keyboard Return Enter";                               break;
              case 0x001Du:               scanCodeTranslation = "Key"             ": Keyboard Left Control";                               break;
              case 0x001Eu:               scanCodeTranslation = "Key"             ": Keyboard A";                                          break;
              case 0x001Fu:               scanCodeTranslation = "Key"             ": Keyboard S";                                          break;
              case 0x0020u:               scanCodeTranslation = "Key"             ": Keyboard D";                                          break;
              case 0x0021u:               scanCodeTranslation = "Key"             ": Keyboard F";                                          break;
              case 0x0022u:               scanCodeTranslation = "Key"             ": Keyboard G";                                          break;
              case 0x0023u:               scanCodeTranslation = "Key"             ": Keyboard H";                                          break;
              case 0x0024u:               scanCodeTranslation = "Key"             ": Keyboard J";                                          break;
              case 0x0025u:               scanCodeTranslation = "Key"             ": Keyboard K";                                          break;
              case 0x0026u:               scanCodeTranslation = "Key"             ": Keyboard L";                                          break;
              case 0x0027u:               scanCodeTranslation = "Key"             ": Keyboard SemiColon and Colon";                        break;
              case 0x0028u:               scanCodeTranslation = "Key"             ": Keyboard Apostrophe and Double Quotation Mark";       break;
              case 0x0029u:               scanCodeTranslation = "Key"             ": Keyboard Grave Accent and Tilde";                     break;
              case 0x002Au:               scanCodeTranslation = "Key"             ": Keyboard Left Shift";                                 break;
              case 0x002Bu:               scanCodeTranslation = "Key"             ": Keyboard Non-United States, Keyboard Pipe and Slash"; break;
              case 0x002Cu:               scanCodeTranslation = "Key"             ": Keyboard Z";                                          break;
              case 0x002Du:               scanCodeTranslation = "Key"             ": Keyboard X";                                          break;
              case 0x002Eu:               scanCodeTranslation = "Key"             ": Keyboard C";                                          break;
              case 0x002Fu:               scanCodeTranslation = "Key"             ": Keyboard V";                                          break;
              case 0x0030u:               scanCodeTranslation = "Key"             ": Keyboard B";                                          break;
              case 0x0031u:               scanCodeTranslation = "Key"             ": Keyboard N";                                          break;
              case 0x0032u:               scanCodeTranslation = "Key"             ": Keyboard M";                                          break;
              case 0x0033u:               scanCodeTranslation = "Key"             ": Keyboard Comma";                                      break;
              case 0x0034u:               scanCodeTranslation = "Key"             ": Keyboard Period";                                     break;
              case 0x0035u:               scanCodeTranslation = "Key"             ": Keyboard QuestionMark";                               break;
              case 0x0036u:               scanCodeTranslation = "Key"             ": Keyboard Right Shift";                                break;
              case 0x0037u:               scanCodeTranslation = "Key"             ": Keypad Star";                                         break;
              case 0x0038u:               scanCodeTranslation = "Key"             ": Keyboard Left Alternative";                           break;
              case 0x0039u:               scanCodeTranslation = "Key"             ": Keyboard Spacebar";                                   break;
              case 0x003Au:               scanCodeTranslation = "Key"             ": Keyboard Caps Lock";                                  break;
              case 0x003Bu:               scanCodeTranslation = "Key"             ": Keyboard Function 1";                                 break;
              case 0x003Cu:               scanCodeTranslation = "Key"             ": Keyboard Function 2";                                 break;
              case 0x003Du:               scanCodeTranslation = "Key"             ": Keyboard Function 3";                                 break;
              case 0x003Eu:               scanCodeTranslation = "Key"             ": Keyboard Function 4";                                 break;
              case 0x003Fu:               scanCodeTranslation = "Key"             ": Keyboard Function 5";                                 break;
              case 0x0040u:               scanCodeTranslation = "Key"             ": Keyboard Function 6";                                 break;
              case 0x0041u:               scanCodeTranslation = "Key"             ": Keyboard Function 7";                                 break;
              case 0x0042u:               scanCodeTranslation = "Key"             ": Keyboard Function 8";                                 break;
              case 0x0043u:               scanCodeTranslation = "Key"             ": Keyboard Function 9";                                 break;
              case 0x0044u:               scanCodeTranslation = "Key"             ": Keyboard Function 10";                                break;
              case 0x0045u:               scanCodeTranslation = "Key"             ": Keyboard Pause, Keypad Number Lock and Clear";        break;
              case 0xE045u:               scanCodeTranslation = "Key"             ": Keypad Number Lock and Clear";                        break;
              case 0xE046u:               scanCodeTranslation = "Key"             ": Keyboard Pause";                                      break; // --> 0xE11D45u
              case 0x0046u:               scanCodeTranslation = "Key"             ": Keyboard Scroll Lock";                                break;
              case 0x0047u:               scanCodeTranslation = "Key"             ": Keypad 7 and Home";                                   break;
              case 0x0048u:               scanCodeTranslation = "Key"             ": Keypad 8 and Up Arrow";                               break;
              case 0x0049u:               scanCodeTranslation = "Key"             ": Keypad 9 and Page Up";                                break;
              case 0x004Au:               scanCodeTranslation = "Key"             ": Keypad Dash";                                         break;
              case 0x004Bu:               scanCodeTranslation = "Key"             ": Keypad 4 and Left Arrow";                             break;
              case 0x004Cu:               scanCodeTranslation = "Key"             ": Keypad 5";                                            break;
              case 0x004Du:               scanCodeTranslation = "Key"             ": Keypad 6 and Right Arrow";                            break;
              case 0x004Eu:               scanCodeTranslation = "Key"             ": Keypad Plus";                                         break;
              case 0x004Fu:               scanCodeTranslation = "Key"             ": Keypad 1 and End";                                    break;
              case 0x0050u:               scanCodeTranslation = "Key"             ": Keypad 2 and Down Arrow";                             break;
              case 0x0051u:               scanCodeTranslation = "Key"             ": Keypad 3 and Page Down";                              break;
              case 0x0052u:               scanCodeTranslation = "Key"             ": Keypad 0 and Insert";                                 break;
              case 0x0053u:               scanCodeTranslation = "Key"             ": Keypad Period";                                       break;
              case 0x0054u: case 0xE037u: scanCodeTranslation = "Key"             ": Keyboard Print Screen";                               break;
              case 0x0056u:               scanCodeTranslation = "Key"             ": Keyboard Non-United States Slash Bar";                break;
              case 0x0057u:               scanCodeTranslation = "Key"             ": Keyboard Function 11";                                break;
              case 0x0058u:               scanCodeTranslation = "Key"             ": Keyboard Function 12";                                break;
              case 0x0059u:               scanCodeTranslation = "Key"             ": Keypad Equals";                                       break;
              case 0x005Cu:               scanCodeTranslation = "Key"             ": Keyboard International 6";                            break;
              case 0x0064u:               scanCodeTranslation = "Key"             ": Keyboard Function 13";                                break;
              case 0x0065u:               scanCodeTranslation = "Key"             ": Keyboard Function 14";                                break;
              case 0x0066u:               scanCodeTranslation = "Key"             ": Keyboard Function 15";                                break;
              case 0x0067u:               scanCodeTranslation = "Key"             ": Keyboard Function 16";                                break;
              case 0x0068u:               scanCodeTranslation = "Key"             ": Keyboard Function 17";                                break;
              case 0x0069u:               scanCodeTranslation = "Key"             ": Keyboard Function 18";                                break;
              case 0x006Au:               scanCodeTranslation = "Key"             ": Keyboard Function 19";                                break;
              case 0x006Bu:               scanCodeTranslation = "Key"             ": Keyboard Function 20";                                break;
              case 0x006Cu:               scanCodeTranslation = "Key"             ": Keyboard Function 21";                                break;
              case 0x006Du:               scanCodeTranslation = "Key"             ": Keyboard Function 22";                                break;
              case 0x006Eu:               scanCodeTranslation = "Key"             ": Keyboard Function 23";                                break;
              case 0x0070u:               scanCodeTranslation = "Key"             ": Keyboard International 2";                            break;
              case 0x0071u: case 0x00F1u: scanCodeTranslation = "Key"             ": Keyboard Language 2";                                 break;
              case 0x0072u: case 0x00F2u: scanCodeTranslation = "Key"             ": Keyboard Language 1";                                 break;
              case 0x0073u:               scanCodeTranslation = "Key"             ": Keyboard International 1";                            break;
              case 0x0076u:               scanCodeTranslation = "Key"             ": Keyboard Function 24, Keyboard Language 5";           break;
              case 0x0077u:               scanCodeTranslation = "Key"             ": Keyboard Language 4";                                 break;
              case 0x0078u:               scanCodeTranslation = "Key"             ": Keyboard Language 3";                                 break;
              case 0x0079u:               scanCodeTranslation = "Key"             ": Keyboard International 4";                            break;
              case 0x007Bu:               scanCodeTranslation = "Key"             ": Keyboard International 5";                            break;
              case 0x007Du:               scanCodeTranslation = "Key"             ": Keyboard International 3";                            break;
              case 0x007Eu:               scanCodeTranslation = "Key"             ": Keypad Comma";                                        break;
              case 0x00FFu:               scanCodeTranslation = "Key"             ": Error Roll Over";                                     break;
              case 0xE010u:               scanCodeTranslation = "Consumer"        ": Scan Previous Track";                                 break;
              case 0xE019u:               scanCodeTranslation = "Consumer"        ": Scan Next Track";                                     break;
              case 0xE01Cu:               scanCodeTranslation = "Key"             ": Keypad Enter";                                        break;
              case 0xE01Du:               scanCodeTranslation = "Key"             ": Keyboard Right Control";                              break;
              case 0xE020u:               scanCodeTranslation = "Consumer"        ": Mute";                                                break;
              case 0xE021u:               scanCodeTranslation = "Consumer"        ": AL Calculator";                                       break;
              case 0xE022u:               scanCodeTranslation = "Consumer"        ": Play/ Pause";                                         break;
              case 0xE024u:               scanCodeTranslation = "Consumer"        ": Stop";                                                break;
              case 0xE02Eu:               scanCodeTranslation = "Consumer"        ": Volume Decrement";                                    break;
              case 0xE030u:               scanCodeTranslation = "Consumer"        ": Volume Increment";                                    break;
              case 0xE032u:               scanCodeTranslation = "Consumer"        ": AC Home";                                             break;
              case 0xE035u:               scanCodeTranslation = "Key"             ": Keypad Slash";                                        break;
              case 0xE038u:               scanCodeTranslation = "Key"             ": Keyboard Right Alternatives";                         break;
              case 0xE047u:               scanCodeTranslation = "Key"             ": Keyboard Home";                                       break;
              case 0xE048u:               scanCodeTranslation = "Key"             ": Keyboard Up Arrow";                                   break;
              case 0xE049u:               scanCodeTranslation = "Key"             ": Keyboard Page Up";                                    break;
              case 0xE04Bu:               scanCodeTranslation = "Key"             ": Keyboard Left Arrow";                                 break;
              case 0xE04Du:               scanCodeTranslation = "Key"             ": Keyboard Right Arrow";                                break;
              case 0xE04Fu:               scanCodeTranslation = "Key"             ": Keyboard End";                                        break;
              case 0xE050u:               scanCodeTranslation = "Key"             ": Keyboard Down Arrow";                                 break;
              case 0xE051u:               scanCodeTranslation = "Key"             ": Keyboard Page Down";                                  break;
              case 0xE052u:               scanCodeTranslation = "Key"             ": Keyboard Insert";                                     break;
              case 0xE053u:               scanCodeTranslation = "Key"             ": Keyboard Delete Forward";                             break;
              case 0xE05Bu:               scanCodeTranslation = "Key"             ": Keyboard Left GUI";                                   break;
              case 0xE05Cu:               scanCodeTranslation = "Key"             ": Keyboard Right GUI";                                  break;
              case 0xE05Du:               scanCodeTranslation = "Key"             ": Keyboard Application";                                break;
              case 0xE05Eu:               scanCodeTranslation = "Generic Desktop" ": System Power Down, Key: Keyboard Power";              break;
              case 0xE05Fu:               scanCodeTranslation = "Generic Desktop" ": System Sleep";                                        break;
              case 0xE063u:               scanCodeTranslation = "Generic Desktop" ": System Wake Up";                                      break;
              case 0xE065u:               scanCodeTranslation = "Consumer"        ": AC Search";                                           break;
              case 0xE066u:               scanCodeTranslation = "Consumer"        ": AC Bookmarks";                                        break;
              case 0xE067u:               scanCodeTranslation = "Consumer"        ": AC Refresh";                                          break;
              case 0xE068u:               scanCodeTranslation = "Consumer"        ": AC Stop";                                             break;
              case 0xE069u:               scanCodeTranslation = "Consumer"        ": AC Forward";                                          break;
              case 0xE06Au:               scanCodeTranslation = "Consumer"        ": AC Back";                                             break;
              case 0xE06Bu:               scanCodeTranslation = "Consumer"        ": AL Local Machine Browser";                            break;
              case 0xE06Cu:               scanCodeTranslation = "Consumer"        ": AL Email Reader";                                     break;
              case 0xE06Du:               scanCodeTranslation = "Consumer"        ": AL Consumer Control Configuration";                   break;
            }

            // ... --> virtualKeyTranslation = …;
            if (
              virtualKey == 0xE1u or
              virtualKey == 0xE6u or
              (virtualKey >= 0x92u and virtualKey <= 0x96u) or
              (virtualKey >= 0xE3u and virtualKey <= 0xE4u) or
              (virtualKey >= 0xE9u and virtualKey <= 0xF5u)
            ) virtualKeyTranslation = "OEM-specific";

            else if (
              virtualKey == 0xE8u or
              (virtualKey >= 0x0Eu and virtualKey <= 0x0Fu) or
              (virtualKey >= 0x97u and virtualKey <= 0x9Fu)
            ) virtualKeyTranslation = "Unassigned";

            else if (virtualKey >= 0x3Au and virtualKey <= 0x40u)
              virtualKeyTranslation = "Undefined";

            else switch (virtualKey) {
              case 0x30u: virtualKeyTranslation = "0"; break;
              case 0x31u: virtualKeyTranslation = "1"; break;
              case 0x32u: virtualKeyTranslation = "2"; break;
              case 0x33u: virtualKeyTranslation = "3"; break;
              case 0x34u: virtualKeyTranslation = "4"; break;
              case 0x35u: virtualKeyTranslation = "5"; break;
              case 0x36u: virtualKeyTranslation = "6"; break;
              case 0x37u: virtualKeyTranslation = "7"; break;
              case 0x38u: virtualKeyTranslation = "8"; break;
              case 0x39u: virtualKeyTranslation = "9"; break;
              case 0x41u: virtualKeyTranslation = "A"; break;
              case 0x42u: virtualKeyTranslation = "B"; break;
              case 0x43u: virtualKeyTranslation = "C"; break;
              case 0x44u: virtualKeyTranslation = "D"; break;
              case 0x45u: virtualKeyTranslation = "E"; break;
              case 0x46u: virtualKeyTranslation = "F"; break;
              case 0x47u: virtualKeyTranslation = "G"; break;
              case 0x48u: virtualKeyTranslation = "H"; break;
              case 0x49u: virtualKeyTranslation = "I"; break;
              case 0x4Au: virtualKeyTranslation = "J"; break;
              case 0x4Bu: virtualKeyTranslation = "K"; break;
              case 0x4Cu: virtualKeyTranslation = "L"; break;
              case 0x4Du: virtualKeyTranslation = "M"; break;
              case 0x4Eu: virtualKeyTranslation = "N"; break;
              case 0x4Fu: virtualKeyTranslation = "O"; break;
              case 0x50u: virtualKeyTranslation = "P"; break;
              case 0x51u: virtualKeyTranslation = "Q"; break;
              case 0x52u: virtualKeyTranslation = "R"; break;
              case 0x53u: virtualKeyTranslation = "S"; break;
              case 0x54u: virtualKeyTranslation = "T"; break;
              case 0x55u: virtualKeyTranslation = "U"; break;
              case 0x56u: virtualKeyTranslation = "V"; break;
              case 0x57u: virtualKeyTranslation = "W"; break;
              case 0x58u: virtualKeyTranslation = "X"; break;
              case 0x59u: virtualKeyTranslation = "Y"; break;
              case 0x5Au: virtualKeyTranslation = "Z"; break;

              case VK_CONTROL: switch (LOWORD(::MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX))) {
                case VK_LCONTROL: virtualKeyTranslation = "Left"  " Control"; break;
                case VK_RCONTROL: virtualKeyTranslation = "Right" " Control"; break;
                default         : virtualKeyTranslation =          "Control";
              } break;

              case VK_MENU: switch (LOWORD(::MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX))) {
                case VK_LMENU: virtualKeyTranslation = "Left"  " Alternative"; break;
                case VK_RMENU: virtualKeyTranslation = "Right" " Alternative"; break;
                default      : virtualKeyTranslation =          "Alternative";
              } break;

              case VK_SHIFT: switch (LOWORD(::MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX))) {
                case VK_LSHIFT: virtualKeyTranslation = "Left"  " Shift"; break;
                case VK_RSHIFT: virtualKeyTranslation = "Right" " Shift"; break;
                default       : virtualKeyTranslation =          "Shift";
              } break;

              case VK_ACCEPT             : virtualKeyTranslation = "Input Method Encoding: Accept";              break;
              case VK_ADD                : virtualKeyTranslation = "Add";                                        break;
              case VK_APPS               : virtualKeyTranslation = "Application";                                break;
              case VK_ATTN               : virtualKeyTranslation = "Attention";                                  break;
              case VK_BACK               : virtualKeyTranslation = "Backspace";                                  break;
              case VK_BROWSER_BACK       : virtualKeyTranslation = "Browser: Back";                              break;
              case VK_BROWSER_FAVORITES  : virtualKeyTranslation = "Browser: Favorites";                         break;
              case VK_BROWSER_FORWARD    : virtualKeyTranslation = "Browser: Forward";                           break;
              case VK_BROWSER_HOME       : virtualKeyTranslation = "Browser: Start and Home";                    break;
              case VK_BROWSER_REFRESH    : virtualKeyTranslation = "Browser: Refresh";                           break;
              case VK_BROWSER_SEARCH     : virtualKeyTranslation = "Browser: Search";                            break;
              case VK_BROWSER_STOP       : virtualKeyTranslation = "Browser: Stop";                              break;
              case VK_CANCEL             : virtualKeyTranslation = "Cancel, Control-Break Processing";           break;
              case VK_CAPITAL            : virtualKeyTranslation = "Capital Lock";                               break;
              case VK_CLEAR              : virtualKeyTranslation = "Clear";                                      break;
              case VK_CONVERT            : virtualKeyTranslation = "Input Method Encoding: Convert";             break;
              case VK_CRSEL              : virtualKeyTranslation = "Cursor Select";                              break;
              case VK_DECIMAL            : virtualKeyTranslation = "Decimal";                                    break;
              case VK_DELETE             : virtualKeyTranslation = "Delete";                                     break;
              case VK_DIVIDE             : virtualKeyTranslation = "Divide";                                     break;
              case VK_DOWN               : virtualKeyTranslation = "Down Arrow";                                 break;
              case VK_END                : virtualKeyTranslation = "End";                                        break;
              case VK_EREOF              : virtualKeyTranslation = "Erase End-of-File";                          break;
              case VK_ESCAPE             : virtualKeyTranslation = "Escape";                                     break;
              case VK_EXECUTE            : virtualKeyTranslation = "Execute";                                    break;
              case VK_EXSEL              : virtualKeyTranslation = "Extend Selection";                           break;
              case VK_F1                 : virtualKeyTranslation = "Function 1";                                 break;
              case VK_F2                 : virtualKeyTranslation = "Function 2";                                 break;
              case VK_F3                 : virtualKeyTranslation = "Function 3";                                 break;
              case VK_F4                 : virtualKeyTranslation = "Function 4";                                 break;
              case VK_F5                 : virtualKeyTranslation = "Function 5";                                 break;
              case VK_F6                 : virtualKeyTranslation = "Function 6";                                 break;
              case VK_F7                 : virtualKeyTranslation = "Function 7";                                 break;
              case VK_F8                 : virtualKeyTranslation = "Function 8";                                 break;
              case VK_F9                 : virtualKeyTranslation = "Function 9";                                 break;
              case VK_F10                : virtualKeyTranslation = "Function 10";                                break;
              case VK_F11                : virtualKeyTranslation = "Function 11";                                break;
              case VK_F12                : virtualKeyTranslation = "Function 12";                                break;
              case VK_F13                : virtualKeyTranslation = "Function 13";                                break;
              case VK_F14                : virtualKeyTranslation = "Function 14";                                break;
              case VK_F15                : virtualKeyTranslation = "Function 15";                                break;
              case VK_F16                : virtualKeyTranslation = "Function 16";                                break;
              case VK_F17                : virtualKeyTranslation = "Function 17";                                break;
              case VK_F18                : virtualKeyTranslation = "Function 18";                                break;
              case VK_F19                : virtualKeyTranslation = "Function 19";                                break;
              case VK_F20                : virtualKeyTranslation = "Function 20";                                break;
              case VK_F21                : virtualKeyTranslation = "Function 21";                                break;
              case VK_F22                : virtualKeyTranslation = "Function 22";                                break;
              case VK_F23                : virtualKeyTranslation = "Function 23";                                break;
              case VK_F24                : virtualKeyTranslation = "Function 24";                                break;
              case VK_FINAL              : virtualKeyTranslation = "Input Method Encoding: Final";               break;
              case VK_HANGUL | VK_KANA   : virtualKeyTranslation = "Input Method Encoding: Hangul, Kana";        break;
              case VK_HANJA  | VK_KANJI  : virtualKeyTranslation = "Input Method Encoding: Hanja, Kanji";        break;
              case VK_HELP               : virtualKeyTranslation = "Help";                                       break;
              case VK_HOME               : virtualKeyTranslation = "Home";                                       break;
              case VK_IME_OFF            : virtualKeyTranslation = "Input Method Encoding: Off";                 break;
              case VK_IME_ON             : virtualKeyTranslation = "Input Method Encoding: On";                  break;
              case VK_INSERT             : virtualKeyTranslation = "Insert";                                     break;
              case VK_JUNJA              : virtualKeyTranslation = "Input Method Encoding: Junja";               break;
              case VK_LAUNCH_APP1        : virtualKeyTranslation = "Start Application 1";                        break;
              case VK_LAUNCH_APP2        : virtualKeyTranslation = "Start Application 2";                        break;
              case VK_LAUNCH_MAIL        : virtualKeyTranslation = "Start Mail";                                 break;
              case VK_LAUNCH_MEDIA_SELECT: virtualKeyTranslation = "Select Media";                               break;
              case VK_LBUTTON            : virtualKeyTranslation = "Left Mouse Button";                          break;
              case VK_LCONTROL           : virtualKeyTranslation = "Left Control";                               break;
              case VK_LEFT               : virtualKeyTranslation = "Left Arrow";                                 break;
              case VK_LMENU              : virtualKeyTranslation = "Left Alternative";                           break;
              case VK_LSHIFT             : virtualKeyTranslation = "Left Shift";                                 break;
              case VK_LWIN               : virtualKeyTranslation = "Left Windows";                               break;
              case VK_MBUTTON            : virtualKeyTranslation = "Middle Mouse Button";                        break;
              case VK_MEDIA_NEXT_TRACK   : virtualKeyTranslation = "Media: Next";                                break;
              case VK_MEDIA_PLAY_PAUSE   : virtualKeyTranslation = "Media: Play/ Pause";                         break;
              case VK_MEDIA_PREV_TRACK   : virtualKeyTranslation = "Media: Previous";                            break;
              case VK_MEDIA_STOP         : virtualKeyTranslation = "Media: Stop";                                break;
              case VK_MODECHANGE         : virtualKeyTranslation = "Input Method Encoding: Mode Change Request"; break;
              case VK_MULTIPLY           : virtualKeyTranslation = "Multiply";                                   break;
              case VK_NEXT               : virtualKeyTranslation = "Next, Page Down";                            break;
              case VK_NONAME             : /* Do nothing… */                                                    break;
              case VK_NONCONVERT         : virtualKeyTranslation = "Input Method Encoding: Non-Convert";         break;
              case VK_NUMLOCK            : virtualKeyTranslation = "Number Lock";                                break;
              case VK_NUMPAD0            : virtualKeyTranslation = "Number Keypad 0";                            break;
              case VK_NUMPAD1            : virtualKeyTranslation = "Number Keypad 1";                            break;
              case VK_NUMPAD2            : virtualKeyTranslation = "Number Keypad 2";                            break;
              case VK_NUMPAD3            : virtualKeyTranslation = "Number Keypad 3";                            break;
              case VK_NUMPAD4            : virtualKeyTranslation = "Number Keypad 4";                            break;
              case VK_NUMPAD5            : virtualKeyTranslation = "Number Keypad 5";                            break;
              case VK_NUMPAD6            : virtualKeyTranslation = "Number Keypad 6";                            break;
              case VK_NUMPAD7            : virtualKeyTranslation = "Number Keypad 7";                            break;
              case VK_NUMPAD8            : virtualKeyTranslation = "Number Keypad 8";                            break;
              case VK_NUMPAD9            : virtualKeyTranslation = "Number Keypad 9";                            break;
              case VK_OEM_1              : virtualKeyTranslation = "OEM-specific 1";                             break;
              case VK_OEM_2              : virtualKeyTranslation = "OEM-specific 2";                             break;
              case VK_OEM_3              : virtualKeyTranslation = "OEM-specific 3";                             break;
              case VK_OEM_4              : virtualKeyTranslation = "OEM-specific 4";                             break;
              case VK_OEM_5              : virtualKeyTranslation = "OEM-specific 5";                             break;
              case VK_OEM_6              : virtualKeyTranslation = "OEM-specific 6";                             break;
              case VK_OEM_7              : virtualKeyTranslation = "OEM-specific 7";                             break;
              case VK_OEM_8              : virtualKeyTranslation = "OEM-specific 8";                             break;
              case VK_OEM_102            : virtualKeyTranslation = "OEM-specific 102";                           break;
              case VK_OEM_CLEAR          : virtualKeyTranslation = "Clear";                                      break;
              case VK_OEM_COMMA          : virtualKeyTranslation = "OEM-specific Comma";                         break;
              case VK_OEM_MINUS          : virtualKeyTranslation = "OEM-specific Minus";                         break;
              case VK_OEM_PERIOD         : virtualKeyTranslation = "OEM-specific Period";                        break;
              case VK_OEM_PLUS           : virtualKeyTranslation = "OEM-specific Plus";                          break;
              case VK_PA1                : virtualKeyTranslation = "Program Action 1";                           break;
              case VK_PACKET             : virtualKeyTranslation = "Unicode Packet";                             break;
              case VK_PAUSE              : virtualKeyTranslation = "Pause";                                      break;
              case VK_PLAY               : virtualKeyTranslation = "Play";                                       break;
              case VK_PRINT              : virtualKeyTranslation = "Print";                                      break;
              case VK_PRIOR              : virtualKeyTranslation = "Prior, Page Up";                             break;
              case VK_PROCESSKEY         : virtualKeyTranslation = "Input Method Encoding: Process";             break;
              case VK_RBUTTON            : virtualKeyTranslation = "Right Mouse Button";                         break;
              case VK_RCONTROL           : virtualKeyTranslation = "Right Control";                              break;
              case VK_RETURN             : virtualKeyTranslation = "Return";                                     break;
              case VK_RIGHT              : virtualKeyTranslation = "Right Arrow";                                break;
              case VK_RMENU              : virtualKeyTranslation = "Right Alternative";                          break;
              case VK_RSHIFT             : virtualKeyTranslation = "Right Shift";                                break;
              case VK_RWIN               : virtualKeyTranslation = "Right Windows";                              break;
              case VK_SCROLL             : virtualKeyTranslation = "Scroll Lock";                                break;
              case VK_SELECT             : virtualKeyTranslation = "Select";                                     break;
              case VK_SEPARATOR          : virtualKeyTranslation = "Separator";                                  break;
              case VK_SLEEP              : virtualKeyTranslation = "Sleep";                                      break;
              case VK_SNAPSHOT           : virtualKeyTranslation = "Snapshot";                                   break;
              case VK_SPACE              : virtualKeyTranslation = "Space";                                      break;
              case VK_SUBTRACT           : virtualKeyTranslation = "Subtract";                                   break;
              case VK_TAB                : virtualKeyTranslation = "Tab";                                        break;
              case VK_UP                 : virtualKeyTranslation = "Up Arrow";                                   break;
              case VK_VOLUME_DOWN        : virtualKeyTranslation = "Volume: Down";                               break;
              case VK_VOLUME_MUTE        : virtualKeyTranslation = "Volume: Mute";                               break;
              case VK_VOLUME_UP          : virtualKeyTranslation = "Volume: Up";                                 break;
              case VK_XBUTTON1           : virtualKeyTranslation = "Extended Mouse Button 1";                    break;
              case VK_XBUTTON2           : virtualKeyTranslation = "Extended Mouse Button 2";                    break;
              case VK_ZOOM               : virtualKeyTranslation = "Zoom";
            }

            // ...
            messageTranslationMultilineFormat = true;

            (void) log::output(log::format('[',
              static_cast<undefined>(virtualKey), NULL != virtualKeyTranslation ? " ": "", NULL != virtualKeyTranslation ? "`" : "", NULL != virtualKeyTranslation ? virtualKeyTranslation : "", NULL != virtualKeyTranslation ? "`" : "",
              ", ", static_cast<undefined>(subparameter), " {"                                                                                                                                                    "\r\n"
              "  " "context   : ", static_cast<bool>(context),                                                                                                                                                "," "\r\n"
              "  " "extended  : ", static_cast<bool>(extended),                                                                                                                                               "," "\r\n"
              "  " "previous  : ", static_cast<bool>(previous),                                                                                                                                               "," "\r\n"
              "  " "repeat    : ", (number) repeatCount,                                                                                                                                                      "," "\r\n"
              "  " "scan      : ", static_cast<undefined>(scanCode), NULL != scanCodeTranslation ? " `" : "", NULL != scanCodeTranslation ? scanCodeTranslation : "", NULL != scanCodeTranslation ? "`" : "", "," "\r\n"
              "  " "transition: ", static_cast<bool>(transition),                                                                                                                                                 "\r\n"
              ""   "}"
            "]"));
          }

          // else if (WM_KILLFOCUS == message) {} // TODO (Lapys)
          // else if (WM_MOVE == message) {} // TODO (Lapys)
          // else if (WM_MOUSEFIRST == message) {} // TODO (Lapys)
          // else if (WM_NCACTIVATE == message) {} // TODO (Lapys)

          else if (
            WM_CREATE   == message or
            WM_NCCREATE == message
          ) {
            CREATESTRUCTW *const createStructure                  = static_cast<CREATESTRUCTW*>(reinterpret_cast<void*>(static_cast<uintptr_t>(subparameter))); // --> std::launder(…)
            ATOM           const createStructureClassID           = static_cast<ATOM>(reinterpret_cast<uintptr_t>(static_cast<void const*>(createStructure -> lpszClass)));
            LPCTSTR              createStructureClassName         = NULL;
            bool                 createStructureUsesExtendedStyle = (createStructure -> dwExStyle & WS_EX_ACCEPTFILES) or (createStructure -> dwExStyle & WS_EX_APPWINDOW) or (createStructure -> dwExStyle & WS_EX_CLIENTEDGE) or (createStructure -> dwExStyle & WS_EX_COMPOSITED) or (createStructure -> dwExStyle & WS_EX_CONTEXTHELP) or (createStructure -> dwExStyle & WS_EX_CONTROLPARENT) or (createStructure -> dwExStyle & WS_EX_DLGMODALFRAME) or (createStructure -> dwExStyle & WS_EX_LAYERED) or (createStructure -> dwExStyle & WS_EX_LAYOUTRTL) or (createStructure -> dwExStyle & WS_EX_LEFT) or (createStructure -> dwExStyle & WS_EX_LEFTSCROLLBAR) or (createStructure -> dwExStyle & WS_EX_LTRREADING) or (createStructure -> dwExStyle & WS_EX_MDICHILD) or (createStructure -> dwExStyle & WS_EX_NOACTIVATE) or (createStructure -> dwExStyle & WS_EX_NOINHERITLAYOUT) or (createStructure -> dwExStyle & WS_EX_NOPARENTNOTIFY) or (createStructure -> dwExStyle & WS_EX_OVERLAPPEDWINDOW) or (createStructure -> dwExStyle & WS_EX_PALETTEWINDOW) or (createStructure -> dwExStyle & WS_EX_RIGHT)    or (createStructure -> dwExStyle & WS_EX_RIGHTSCROLLBAR) or (createStructure -> dwExStyle & WS_EX_RTLREADING) or (createStructure -> dwExStyle & WS_EX_STATICEDGE) or (createStructure -> dwExStyle & WS_EX_TOOLWINDOW) or (createStructure -> dwExStyle & WS_EX_TOPMOST) or (createStructure -> dwExStyle & WS_EX_TRANSPARENT) or (createStructure -> dwExStyle & WS_EX_WINDOWEDGE);
            bool                 createStructureUsesStyle         = (createStructure -> style     & WS_BORDER)         or (createStructure -> style     & WS_CAPTION)      or (createStructure -> style     & WS_CHILD)         or (createStructure -> style     & WS_CHILDWINDOW)   or (createStructure -> style     & WS_CLIPCHILDREN)   or (createStructure -> style     & WS_CLIPSIBLINGS)     or (createStructure -> style     & WS_DISABLED)         or (createStructure -> style     & WS_DLGFRAME)   or (createStructure -> style     & WS_GROUP)        or (createStructure -> style     & WS_HSCROLL) or (createStructure -> style     & WS_ICONIC)           or (createStructure -> style     & WS_MAXIMIZE)      or (createStructure -> style     & WS_MAXIMIZEBOX) or (createStructure -> style     & WS_MINIMIZE)      or (createStructure -> style     & WS_MINIMIZEBOX)        or (createStructure -> style     & WS_OVERLAPPED)        or (createStructure -> style     & WS_OVERLAPPEDWINDOW)    or (createStructure -> style     & WS_POPUP)            or (createStructure -> style     & WS_POPUPWINDOW) or (createStructure -> style     & WS_SIZEBOX)           or (createStructure -> style     & WS_SYSMENU)       or (createStructure -> style     & WS_TABSTOP)       or (createStructure -> style     & WS_THICKFRAME)    or (createStructure -> style     & WS_TILED)      or (createStructure -> style     & WS_TILEDWINDOW)    or (createStructure -> style     & WS_VISIBLE) or (createStructure -> style & WS_VSCROLL);
            ULONG_PTR      const windowHandleClassID              = ::GetClassLongPtrW(windowHandle, GCW_ATOM);

            // ...
            #ifdef WS_EX_NOREDIRECTIONBITMAP
              createStructureUsesExtendedStyle |= createStructure -> dwExStyle & WS_EX_NOREDIRECTIONBITMAP;
            #endif

            #ifdef ACS_AUTOPLAY
              createStructureUsesStyle |= createStructure -> style & ACS_AUTOPLAY;
            #endif

            #ifdef ACS_CENTER
              createStructureUsesStyle |= createStructure -> style & ACS_CENTER;
            #endif

            #ifdef ACS_TIMER
              createStructureUsesStyle |= createStructure -> style & ACS_TIMER;
            #endif

            #ifdef ACS_TRANSPARENT
              createStructureUsesStyle |= createStructure -> style & ACS_TRANSPARENT;
            #endif

            #ifdef BS_3STATE
              createStructureUsesStyle |= createStructure -> style & BS_3STATE;
            #endif

            #ifdef BS_AUTO3STATE
              createStructureUsesStyle |= createStructure -> style & BS_AUTO3STATE;
            #endif

            #ifdef BS_AUTOCHECKBOX
              createStructureUsesStyle |= createStructure -> style & BS_AUTOCHECKBOX;
            #endif

            #ifdef BS_AUTORADIOBUTTON
              createStructureUsesStyle |= createStructure -> style & BS_AUTORADIOBUTTON;
            #endif

            #ifdef BS_BITMAP
              createStructureUsesStyle |= createStructure -> style & BS_BITMAP;
            #endif

            #ifdef BS_BOTTOM
              createStructureUsesStyle |= createStructure -> style & BS_BOTTOM;
            #endif

            #ifdef BS_CENTER
              createStructureUsesStyle |= createStructure -> style & BS_CENTER;
            #endif

            #ifdef BS_CHECKBOX
              createStructureUsesStyle |= createStructure -> style & BS_CHECKBOX;
            #endif

            #ifdef BS_COMMANDLINK
              createStructureUsesStyle |= createStructure -> style & BS_COMMANDLINK;
            #endif

            #ifdef BS_DEFCOMMANDLINK
              createStructureUsesStyle |= createStructure -> style & BS_DEFCOMMANDLINK;
            #endif

            #ifdef BS_DEFPUSHBUTTON
              createStructureUsesStyle |= createStructure -> style & BS_DEFPUSHBUTTON;
            #endif

            #ifdef BS_DEFSPLITBUTTON
              createStructureUsesStyle |= createStructure -> style & BS_DEFSPLITBUTTON;
            #endif

            #ifdef BS_GROUPBOX
              createStructureUsesStyle |= createStructure -> style & BS_GROUPBOX;
            #endif

            #ifdef BS_ICON
              createStructureUsesStyle |= createStructure -> style & BS_ICON;
            #endif

            #ifdef BS_FLAT
              createStructureUsesStyle |= createStructure -> style & BS_FLAT;
            #endif

            #ifdef BS_LEFT
              createStructureUsesStyle |= createStructure -> style & BS_LEFT;
            #endif

            #ifdef BS_LEFTTEXT
              createStructureUsesStyle |= createStructure -> style & BS_LEFTTEXT;
            #endif

            #ifdef BS_MULTILINE
              createStructureUsesStyle |= createStructure -> style & BS_MULTILINE;
            #endif

            #ifdef BS_NOTIFY
              createStructureUsesStyle |= createStructure -> style & BS_NOTIFY;
            #endif

            #ifdef BS_OWNERDRAW
              createStructureUsesStyle |= createStructure -> style & BS_OWNERDRAW;
            #endif

            #ifdef BS_PUSHBUTTON
              createStructureUsesStyle |= createStructure -> style & BS_PUSHBUTTON;
            #endif

            #ifdef BS_PUSHLIKE
              createStructureUsesStyle |= createStructure -> style & BS_PUSHLIKE;
            #endif

            #ifdef BS_RADIOBUTTON
              createStructureUsesStyle |= createStructure -> style & BS_RADIOBUTTON;
            #endif

            #ifdef BS_RIGHT
              createStructureUsesStyle |= createStructure -> style & BS_RIGHT;
            #endif

            #ifdef BS_RIGHTBUTTON
              createStructureUsesStyle |= createStructure -> style & BS_RIGHTBUTTON;
            #endif

            #ifdef BS_SPLITBUTTON
              createStructureUsesStyle |= createStructure -> style & BS_SPLITBUTTON;
            #endif

            #ifdef BS_TEXT
              createStructureUsesStyle |= createStructure -> style & BS_TEXT;
            #endif

            #ifdef BS_TOP
              createStructureUsesStyle |= createStructure -> style & BS_TOP;
            #endif

            #ifdef BS_TYPEMASK
              createStructureUsesStyle |= createStructure -> style & BS_TYPEMASK;
            #endif

            #ifdef BS_USERBUTTON
              createStructureUsesStyle |= createStructure -> style & BS_USERBUTTON;
            #endif

            #ifdef BS_VCENTER
              createStructureUsesStyle |= createStructure -> style & BS_VCENTER;
            #endif

            #ifdef BTNS_AUTOSIZE
              createStructureUsesStyle |= createStructure -> style & BTNS_AUTOSIZE;
            #endif

            #ifdef BTNS_BUTTON
              createStructureUsesStyle |= createStructure -> style & BTNS_BUTTON;
            #endif

            #ifdef BTNS_CHECK
              createStructureUsesStyle |= createStructure -> style & BTNS_CHECK;
            #endif

            #ifdef BTNS_CHECKGROUP
              createStructureUsesStyle |= createStructure -> style & BTNS_CHECKGROUP;
            #endif

            #ifdef BTNS_DROPDOWN
              createStructureUsesStyle |= createStructure -> style & BTNS_DROPDOWN;
            #endif

            #ifdef BTNS_GROUP
              createStructureUsesStyle |= createStructure -> style & BTNS_GROUP;
            #endif

            #ifdef BTNS_NOPREFIX
              createStructureUsesStyle |= createStructure -> style & BTNS_NOPREFIX;
            #endif

            #ifdef BTNS_SEP
              createStructureUsesStyle |= createStructure -> style & BTNS_SEP;
            #endif

            #ifdef BTNS_SHOWTEXT
              createStructureUsesStyle |= createStructure -> style & BTNS_SHOWTEXT;
            #endif

            #ifdef BTNS_WHOLEDROPDOWN
              createStructureUsesStyle |= createStructure -> style & BTNS_WHOLEDROPDOWN;
            #endif

            #ifdef TBS_AUTOTICKS
              createStructureUsesStyle |= createStructure -> style & TBS_AUTOTICKS;
            #endif

            #ifdef TBS_BOTH
              createStructureUsesStyle |= createStructure -> style & TBS_BOTH;
            #endif

            #ifdef TBS_BOTTOM
              createStructureUsesStyle |= createStructure -> style & TBS_BOTTOM;
            #endif

            #ifdef TBS_DOWNISLEFT
              createStructureUsesStyle |= createStructure -> style & TBS_DOWNISLEFT;
            #endif

            #ifdef TBS_ENABLESELRANGE
              createStructureUsesStyle |= createStructure -> style & TBS_ENABLESELRANGE;
            #endif

            #ifdef TBS_FIXEDLENGTH
              createStructureUsesStyle |= createStructure -> style & TBS_FIXEDLENGTH;
            #endif

            #ifdef TBS_HORZ
              createStructureUsesStyle |= createStructure -> style & TBS_HORZ;
            #endif

            #ifdef TBS_LEFT
              createStructureUsesStyle |= createStructure -> style & TBS_LEFT;
            #endif

            #ifdef TBS_NOTHUMB
              createStructureUsesStyle |= createStructure -> style & TBS_NOTHUMB;
            #endif

            #ifdef TBS_NOTICKS
              createStructureUsesStyle |= createStructure -> style & TBS_NOTICKS;
            #endif

            #ifdef TBS_NOTIFYBEFOREMOVE
              createStructureUsesStyle |= createStructure -> style & TBS_NOTIFYBEFOREMOVE;
            #endif

            #ifdef TBS_REVERSED
              createStructureUsesStyle |= createStructure -> style & TBS_REVERSED;
            #endif

            #ifdef TBS_RIGHT
              createStructureUsesStyle |= createStructure -> style & TBS_RIGHT;
            #endif

            #ifdef TBS_TOOLTIPS
              createStructureUsesStyle |= createStructure -> style & TBS_TOOLTIPS;
            #endif

            #ifdef TBS_TOP
              createStructureUsesStyle |= createStructure -> style & TBS_TOP;
            #endif

            #ifdef TBS_TRANSPARENTBKGND
              createStructureUsesStyle |= createStructure -> style & TBS_TRANSPARENTBKGND;
            #endif

            #ifdef TBS_VERT
              createStructureUsesStyle |= createStructure -> style & TBS_VERT;
            #endif

            #ifdef CBES_EX_CASESENSITIVE
              createStructureUsesStyle |= createStructure -> style & CBES_EX_CASESENSITIVE;
            #endif

            #ifdef CBES_EX_NOEDITIMAGE
              createStructureUsesStyle |= createStructure -> style & CBES_EX_NOEDITIMAGE;
            #endif

            #ifdef CBES_EX_NOEDITIMAGEINDENT
              createStructureUsesStyle |= createStructure -> style & CBES_EX_NOEDITIMAGEINDENT;
            #endif

            #ifdef CBES_EX_NOSIZELIMIT
              createStructureUsesStyle |= createStructure -> style & CBES_EX_NOSIZELIMIT;
            #endif

            #ifdef CBES_EX_PATHWORDBREAKPROC
              createStructureUsesStyle |= createStructure -> style & CBES_EX_PATHWORDBREAKPROC;
            #endif

            #ifdef CBES_EX_TEXTENDELLIPSIS
              createStructureUsesStyle |= createStructure -> style & CBES_EX_TEXTENDELLIPSIS;
            #endif

            #ifdef CBS_AUTOHSCROLL
              createStructureUsesStyle |= createStructure -> style & CBS_AUTOHSCROLL;
            #endif

            #ifdef CBS_DISABLENOSCROLL
              createStructureUsesStyle |= createStructure -> style & CBS_DISABLENOSCROLL;
            #endif

            #ifdef CBS_DROPDOWN
              createStructureUsesStyle |= createStructure -> style & CBS_DROPDOWN;
            #endif

            #ifdef CBS_DROPDOWNLIST
              createStructureUsesStyle |= createStructure -> style & CBS_DROPDOWNLIST;
            #endif

            #ifdef CBS_HASSTRINGS
              createStructureUsesStyle |= createStructure -> style & CBS_HASSTRINGS;
            #endif

            #ifdef CBS_LOWERCASE
              createStructureUsesStyle |= createStructure -> style & CBS_LOWERCASE;
            #endif

            #ifdef CBS_NOINTEGRALHEIGHT
              createStructureUsesStyle |= createStructure -> style & CBS_NOINTEGRALHEIGHT;
            #endif

            #ifdef CBS_OEMCONVERT
              createStructureUsesStyle |= createStructure -> style & CBS_OEMCONVERT;
            #endif

            #ifdef CBS_OWNERDRAWFIXED
              createStructureUsesStyle |= createStructure -> style & CBS_OWNERDRAWFIXED;
            #endif

            #ifdef CBS_OWNERDRAWVARIABLE
              createStructureUsesStyle |= createStructure -> style & CBS_OWNERDRAWVARIABLE;
            #endif

            #ifdef CBS_SIMPLE
              createStructureUsesStyle |= createStructure -> style & CBS_SIMPLE;
            #endif

            #ifdef CBS_SORT
              createStructureUsesStyle |= createStructure -> style & CBS_SORT;
            #endif

            #ifdef CBS_UPPERCASE
              createStructureUsesStyle |= createStructure -> style & CBS_UPPERCASE;
            #endif

            #ifdef DTS_APPCANPARSE
              createStructureUsesStyle |= createStructure -> style & DTS_APPCANPARSE;
            #endif

            #ifdef DTS_LONGDATEFORMAT
              createStructureUsesStyle |= createStructure -> style & DTS_LONGDATEFORMAT;
            #endif

            #ifdef DTS_RIGHTALIGN
              createStructureUsesStyle |= createStructure -> style & DTS_RIGHTALIGN;
            #endif

            #ifdef DTS_SHORTDATECENTURYFORMAT
              createStructureUsesStyle |= createStructure -> style & DTS_SHORTDATECENTURYFORMAT;
            #endif

            #ifdef DTS_SHORTDATEFORMAT
              createStructureUsesStyle |= createStructure -> style & DTS_SHORTDATEFORMAT;
            #endif

            #ifdef DTS_SHOWNONE
              createStructureUsesStyle |= createStructure -> style & DTS_SHOWNONE;
            #endif

            #ifdef DTS_TIMEFORMAT
              createStructureUsesStyle |= createStructure -> style & DTS_TIMEFORMAT;
            #endif

            #ifdef DTS_UPDOWN
              createStructureUsesStyle |= createStructure -> style & DTS_UPDOWN;
            #endif

            #ifdef ES_AUTOHSCROLL
              createStructureUsesStyle |= createStructure -> style & ES_AUTOHSCROLL;
            #endif

            #ifdef ES_AUTOVSCROLL
              createStructureUsesStyle |= createStructure -> style & ES_AUTOVSCROLL;
            #endif

            #ifdef ES_CENTER
              createStructureUsesStyle |= createStructure -> style & ES_CENTER;
            #endif

            #ifdef ES_DISABLENOSCROLL
              createStructureUsesStyle |= createStructure -> style & ES_DISABLENOSCROLL;
            #endif

            #ifdef ES_EX_NOCALLOLEINIT
              createStructureUsesStyle |= createStructure -> style & ES_EX_NOCALLOLEINIT;
            #endif

            #ifdef ES_LEFT
              createStructureUsesStyle |= createStructure -> style & ES_LEFT;
            #endif

            #ifdef ES_LOWERCASE
              createStructureUsesStyle |= createStructure -> style & ES_LOWERCASE;
            #endif

            #ifdef ES_MULTILINE
              createStructureUsesStyle |= createStructure -> style & ES_MULTILINE;
            #endif

            #ifdef ES_NOHIDESEL
              createStructureUsesStyle |= createStructure -> style & ES_NOHIDESEL;
            #endif

            #ifdef ES_NOIME
              createStructureUsesStyle |= createStructure -> style & ES_NOIME;
            #endif

            #ifdef ES_NOOLEDRAGDROP
              createStructureUsesStyle |= createStructure -> style & ES_NOOLEDRAGDROP;
            #endif

            #ifdef ES_NUMBER
              createStructureUsesStyle |= createStructure -> style & ES_NUMBER;
            #endif

            #ifdef ES_OEMCONVERT
              createStructureUsesStyle |= createStructure -> style & ES_OEMCONVERT;
            #endif

            #ifdef ES_PASSWORD
              createStructureUsesStyle |= createStructure -> style & ES_PASSWORD;
            #endif

            #ifdef ES_READONLY
              createStructureUsesStyle |= createStructure -> style & ES_READONLY;
            #endif

            #ifdef ES_RIGHT
              createStructureUsesStyle |= createStructure -> style & ES_RIGHT;
            #endif

            #ifdef ES_SAVESEL
              createStructureUsesStyle |= createStructure -> style & ES_SAVESEL;
            #endif

            #ifdef ES_SELECTIONBAR
              createStructureUsesStyle |= createStructure -> style & ES_SELECTIONBAR;
            #endif

            #ifdef ES_SELFIME
              createStructureUsesStyle |= createStructure -> style & ES_SELFIME;
            #endif

            #ifdef ES_SUNKEN
              createStructureUsesStyle |= createStructure -> style & ES_SUNKEN;
            #endif

            #ifdef ES_UPPERCASE
              createStructureUsesStyle |= createStructure -> style & ES_UPPERCASE;
            #endif

            #ifdef ES_VERTICAL
              createStructureUsesStyle |= createStructure -> style & ES_VERTICAL;
            #endif

            #ifdef ES_WANTRETURN
              createStructureUsesStyle |= createStructure -> style & ES_WANTRETURN;
            #endif

            #ifdef HDS_BUTTONS
              createStructureUsesStyle |= createStructure -> style & HDS_BUTTONS;
            #endif

            #ifdef HDS_CHECKBOXES
              createStructureUsesStyle |= createStructure -> style & HDS_CHECKBOXES;
            #endif

            #ifdef HDS_DRAGDROP
              createStructureUsesStyle |= createStructure -> style & HDS_DRAGDROP;
            #endif

            #ifdef HDS_FILTERBAR
              createStructureUsesStyle |= createStructure -> style & HDS_FILTERBAR;
            #endif

            #ifdef HDS_FLAT
              createStructureUsesStyle |= createStructure -> style & HDS_FLAT;
            #endif

            #ifdef HDS_FULLDRAG
              createStructureUsesStyle |= createStructure -> style & HDS_FULLDRAG;
            #endif

            #ifdef HDS_HIDDEN
              createStructureUsesStyle |= createStructure -> style & HDS_HIDDEN;
            #endif

            #ifdef HDS_HORZ
              createStructureUsesStyle |= createStructure -> style & HDS_HORZ;
            #endif

            #ifdef HDS_HOTTRACK
              createStructureUsesStyle |= createStructure -> style & HDS_HOTTRACK;
            #endif

            #ifdef HDS_NOSIZING
              createStructureUsesStyle |= createStructure -> style & HDS_NOSIZING;
            #endif

            #ifdef HDS_OVERFLOW
              createStructureUsesStyle |= createStructure -> style & HDS_OVERFLOW;
            #endif

            #ifdef LBS_COMBOBOX
              createStructureUsesStyle |= createStructure -> style & LBS_COMBOBOX;
            #endif

            #ifdef LBS_DISABLENOSCROLL
              createStructureUsesStyle |= createStructure -> style & LBS_DISABLENOSCROLL;
            #endif

            #ifdef LBS_EXTENDEDSEL
              createStructureUsesStyle |= createStructure -> style & LBS_EXTENDEDSEL;
            #endif

            #ifdef LBS_HASSTRINGS
              createStructureUsesStyle |= createStructure -> style & LBS_HASSTRINGS;
            #endif

            #ifdef LBS_MULTICOLUMN
              createStructureUsesStyle |= createStructure -> style & LBS_MULTICOLUMN;
            #endif

            #ifdef LBS_MULTIPLESEL
              createStructureUsesStyle |= createStructure -> style & LBS_MULTIPLESEL;
            #endif

            #ifdef LBS_NODATA
              createStructureUsesStyle |= createStructure -> style & LBS_NODATA;
            #endif

            #ifdef LBS_NOINTEGRALHEIGHT
              createStructureUsesStyle |= createStructure -> style & LBS_NOINTEGRALHEIGHT;
            #endif

            #ifdef LBS_NOREDRAW
              createStructureUsesStyle |= createStructure -> style & LBS_NOREDRAW;
            #endif

            #ifdef LBS_NOSEL
              createStructureUsesStyle |= createStructure -> style & LBS_NOSEL;
            #endif

            #ifdef LBS_NOTIFY
              createStructureUsesStyle |= createStructure -> style & LBS_NOTIFY;
            #endif

            #ifdef LBS_OWNERDRAWFIXED
              createStructureUsesStyle |= createStructure -> style & LBS_OWNERDRAWFIXED;
            #endif

            #ifdef LBS_OWNERDRAWVARIABLE
              createStructureUsesStyle |= createStructure -> style & LBS_OWNERDRAWVARIABLE;
            #endif

            #ifdef LBS_SORT
              createStructureUsesStyle |= createStructure -> style & LBS_SORT;
            #endif

            #ifdef LBS_STANDARD
              createStructureUsesStyle |= createStructure -> style & LBS_STANDARD;
            #endif

            #ifdef LBS_USETABSTOPS
              createStructureUsesStyle |= createStructure -> style & LBS_USETABSTOPS;
            #endif

            #ifdef LBS_WANTKEYBOARDINPUT
              createStructureUsesStyle |= createStructure -> style & LBS_WANTKEYBOARDINPUT;
            #endif

            #ifdef LVS_ALIGNLEFT
              createStructureUsesStyle |= createStructure -> style & LVS_ALIGNLEFT;
            #endif

            #ifdef LVS_ALIGNMASK
              createStructureUsesStyle |= createStructure -> style & LVS_ALIGNMASK;
            #endif

            #ifdef LVS_ALIGNTOP
              createStructureUsesStyle |= createStructure -> style & LVS_ALIGNTOP;
            #endif

            #ifdef LVS_AUTOARRANGE
              createStructureUsesStyle |= createStructure -> style & LVS_AUTOARRANGE;
            #endif

            #ifdef LVS_EDITLABELS
              createStructureUsesStyle |= createStructure -> style & LVS_EDITLABELS;
            #endif

            #ifdef LVS_ICON
              createStructureUsesStyle |= createStructure -> style & LVS_ICON;
            #endif

            #ifdef LVS_LIST
              createStructureUsesStyle |= createStructure -> style & LVS_LIST;
            #endif

            #ifdef LVS_NOCOLUMNHEADER
              createStructureUsesStyle |= createStructure -> style & LVS_NOCOLUMNHEADER;
            #endif

            #ifdef LVS_NOLABELWRAP
              createStructureUsesStyle |= createStructure -> style & LVS_NOLABELWRAP;
            #endif

            #ifdef LVS_NOSCROLL
              createStructureUsesStyle |= createStructure -> style & LVS_NOSCROLL;
            #endif

            #ifdef LVS_NOSORTHEADER
              createStructureUsesStyle |= createStructure -> style & LVS_NOSORTHEADER;
            #endif

            #ifdef LVS_OWNERDATA
              createStructureUsesStyle |= createStructure -> style & LVS_OWNERDATA;
            #endif

            #ifdef LVS_OWNERDRAWFIXED
              createStructureUsesStyle |= createStructure -> style & LVS_OWNERDRAWFIXED;
            #endif

            #ifdef LVS_REPORT
              createStructureUsesStyle |= createStructure -> style & LVS_REPORT;
            #endif

            #ifdef LVS_SHAREIMAGELISTS
              createStructureUsesStyle |= createStructure -> style & LVS_SHAREIMAGELISTS;
            #endif

            #ifdef LVS_SHOWSELALWAYS
              createStructureUsesStyle |= createStructure -> style & LVS_SHOWSELALWAYS;
            #endif

            #ifdef LVS_SINGLESEL
              createStructureUsesStyle |= createStructure -> style & LVS_SINGLESEL;
            #endif

            #ifdef LVS_SMALLICON
              createStructureUsesStyle |= createStructure -> style & LVS_SMALLICON;
            #endif

            #ifdef LVS_SORTASCENDING
              createStructureUsesStyle |= createStructure -> style & LVS_SORTASCENDING;
            #endif

            #ifdef LVS_SORTDESCENDING
              createStructureUsesStyle |= createStructure -> style & LVS_SORTDESCENDING;
            #endif

            #ifdef LVS_TYPEMASK
              createStructureUsesStyle |= createStructure -> style & LVS_TYPEMASK;
            #endif

            #ifdef LVS_TYPESTYLEMASK
              createStructureUsesStyle |= createStructure -> style & LVS_TYPESTYLEMASK;
            #endif

            #ifdef LWS_IGNORERETURN
              createStructureUsesStyle |= createStructure -> style & LWS_IGNORERETURN;
            #endif

            #ifdef LWS_NOPREFIX
              createStructureUsesStyle |= createStructure -> style & LWS_NOPREFIX;
            #endif

            #ifdef LWS_RIGHT
              createStructureUsesStyle |= createStructure -> style & LWS_RIGHT;
            #endif

            #ifdef LWS_TRANSPARENT
              createStructureUsesStyle |= createStructure -> style & LWS_TRANSPARENT;
            #endif

            #ifdef LWS_USECUSTOMTEXT
              createStructureUsesStyle |= createStructure -> style & LWS_USECUSTOMTEXT;
            #endif

            #ifdef LWS_USEVISUALSTYLE
              createStructureUsesStyle |= createStructure -> style & LWS_USEVISUALSTYLE;
            #endif

            #ifdef MCS_DAYSTATE
              createStructureUsesStyle |= createStructure -> style & MCS_DAYSTATE;
            #endif

            #ifdef MCS_MULTISELECT
              createStructureUsesStyle |= createStructure -> style & MCS_MULTISELECT;
            #endif

            #ifdef MCS_NOSELCHANGEONNAV
              createStructureUsesStyle |= createStructure -> style & MCS_NOSELCHANGEONNAV;
            #endif

            #ifdef MCS_NOTODAY
              createStructureUsesStyle |= createStructure -> style & MCS_NOTODAY;
            #endif

            #ifdef MCS_NOTODAYCIRCLE
              createStructureUsesStyle |= createStructure -> style & MCS_NOTODAYCIRCLE;
            #endif

            #ifdef MCS_NOTRAILINGDATES
              createStructureUsesStyle |= createStructure -> style & MCS_NOTRAILINGDATES;
            #endif

            #ifdef MCS_SHORTDAYSOFWEEK
              createStructureUsesStyle |= createStructure -> style & MCS_SHORTDAYSOFWEEK;
            #endif

            #ifdef MCS_WEEKNUMBERS
              createStructureUsesStyle |= createStructure -> style & MCS_WEEKNUMBERS;
            #endif

            #ifdef PBS_MARQUEE
              createStructureUsesStyle |= createStructure -> style & PBS_MARQUEE;
            #endif

            #ifdef PBS_SMOOTH
              createStructureUsesStyle |= createStructure -> style & PBS_SMOOTH;
            #endif

            #ifdef PBS_SMOOTHREVERSE
              createStructureUsesStyle |= createStructure -> style & PBS_SMOOTHREVERSE;
            #endif

            #ifdef PBS_VERTICAL
              createStructureUsesStyle |= createStructure -> style & PBS_VERTICAL;
            #endif

            #ifdef PGS_AUTOSCROLL
              createStructureUsesStyle |= createStructure -> style & PGS_AUTOSCROLL;
            #endif

            #ifdef PGS_DRAGNDROP
              createStructureUsesStyle |= createStructure -> style & PGS_DRAGNDROP;
            #endif

            #ifdef PGS_HORZ
              createStructureUsesStyle |= createStructure -> style & PGS_HORZ;
            #endif

            #ifdef PGS_VERT
              createStructureUsesStyle |= createStructure -> style & PGS_VERT;
            #endif

            #ifdef RBS_AUTOSIZE
              createStructureUsesStyle |= createStructure -> style & RBS_AUTOSIZE;
            #endif

            #ifdef RBS_BANDBORDERS
              createStructureUsesStyle |= createStructure -> style & RBS_BANDBORDERS;
            #endif

            #ifdef RBS_DBLCLKTOGGLE
              createStructureUsesStyle |= createStructure -> style & RBS_DBLCLKTOGGLE;
            #endif

            #ifdef RBS_FIXEDORDER
              createStructureUsesStyle |= createStructure -> style & RBS_FIXEDORDER;
            #endif

            #ifdef RBS_REGISTERDROP
              createStructureUsesStyle |= createStructure -> style & RBS_REGISTERDROP;
            #endif

            #ifdef RBS_TOOLTIPS
              createStructureUsesStyle |= createStructure -> style & RBS_TOOLTIPS;
            #endif

            #ifdef RBS_VARHEIGHT
              createStructureUsesStyle |= createStructure -> style & RBS_VARHEIGHT;
            #endif

            #ifdef RBS_VERTICALGRIPPER
              createStructureUsesStyle |= createStructure -> style & RBS_VERTICALGRIPPER;
            #endif

            #ifdef SBARS_SIZEGRIP
              createStructureUsesStyle |= createStructure -> style & SBARS_SIZEGRIP;
            #endif

            #ifdef SBARS_TOOLTIPS
              createStructureUsesStyle |= createStructure -> style & SBARS_TOOLTIPS;
            #endif

            #ifdef SBT_TOOLTIPS
              createStructureUsesStyle |= createStructure -> style & SBT_TOOLTIPS;
            #endif

            #ifdef SBS_BOTTOMALIGN
              createStructureUsesStyle |= createStructure -> style & SBS_BOTTOMALIGN;
            #endif

            #ifdef SBS_HORZ
              createStructureUsesStyle |= createStructure -> style & SBS_HORZ;
            #endif

            #ifdef SBS_LEFTALIGN
              createStructureUsesStyle |= createStructure -> style & SBS_LEFTALIGN;
            #endif

            #ifdef SBS_RIGHTALIGN
              createStructureUsesStyle |= createStructure -> style & SBS_RIGHTALIGN;
            #endif

            #ifdef SBS_SIZEBOX
              createStructureUsesStyle |= createStructure -> style & SBS_SIZEBOX;
            #endif

            #ifdef SBS_SIZEBOXBOTTOMRIGHTALIGN
              createStructureUsesStyle |= createStructure -> style & SBS_SIZEBOXBOTTOMRIGHTALIGN;
            #endif

            #ifdef SBS_SIZEBOXTOPLEFTALIGN
              createStructureUsesStyle |= createStructure -> style & SBS_SIZEBOXTOPLEFTALIGN;
            #endif

            #ifdef SBS_SIZEGRIP
              createStructureUsesStyle |= createStructure -> style & SBS_SIZEGRIP;
            #endif

            #ifdef SBS_TOPALIGN
              createStructureUsesStyle |= createStructure -> style & SBS_TOPALIGN;
            #endif

            #ifdef SBS_VERT
              createStructureUsesStyle |= createStructure -> style & SBS_VERT;
            #endif

            #ifdef SS_BITMAP
              createStructureUsesStyle |= createStructure -> style & SS_BITMAP;
            #endif

            #ifdef SS_BLACKFRAME
              createStructureUsesStyle |= createStructure -> style & SS_BLACKFRAME;
            #endif

            #ifdef SS_BLACKRECT
              createStructureUsesStyle |= createStructure -> style & SS_BLACKRECT;
            #endif

            #ifdef SS_CENTER
              createStructureUsesStyle |= createStructure -> style & SS_CENTER;
            #endif

            #ifdef SS_CENTERIMAGE
              createStructureUsesStyle |= createStructure -> style & SS_CENTERIMAGE;
            #endif

            #ifdef SS_EDITCONTROL
              createStructureUsesStyle |= createStructure -> style & SS_EDITCONTROL;
            #endif

            #ifdef SS_ENDELLIPSIS
              createStructureUsesStyle |= createStructure -> style & SS_ENDELLIPSIS;
            #endif

            #ifdef SS_ENHMETAFILE
              createStructureUsesStyle |= createStructure -> style & SS_ENHMETAFILE;
            #endif

            #ifdef SS_ETCHEDFRAME
              createStructureUsesStyle |= createStructure -> style & SS_ETCHEDFRAME;
            #endif

            #ifdef SS_ETCHEDHORZ
              createStructureUsesStyle |= createStructure -> style & SS_ETCHEDHORZ;
            #endif

            #ifdef SS_ETCHEDVERT
              createStructureUsesStyle |= createStructure -> style & SS_ETCHEDVERT;
            #endif

            #ifdef SS_GRAYFRAME
              createStructureUsesStyle |= createStructure -> style & SS_GRAYFRAME;
            #endif

            #ifdef SS_GRAYRECT
              createStructureUsesStyle |= createStructure -> style & SS_GRAYRECT;
            #endif

            #ifdef SS_ICON
              createStructureUsesStyle |= createStructure -> style & SS_ICON;
            #endif

            #ifdef SS_LEFT
              createStructureUsesStyle |= createStructure -> style & SS_LEFT;
            #endif

            #ifdef SS_LEFTNOWORDWRAP
              createStructureUsesStyle |= createStructure -> style & SS_LEFTNOWORDWRAP;
            #endif

            #ifdef SS_NOPREFIX
              createStructureUsesStyle |= createStructure -> style & SS_NOPREFIX;
            #endif

            #ifdef SS_NOTIFY
              createStructureUsesStyle |= createStructure -> style & SS_NOTIFY;
            #endif

            #ifdef SS_OWNERDRAW
              createStructureUsesStyle |= createStructure -> style & SS_OWNERDRAW;
            #endif

            #ifdef SS_PATHELLIPSIS
              createStructureUsesStyle |= createStructure -> style & SS_PATHELLIPSIS;
            #endif

            #ifdef SS_REALSIZECONTROL
              createStructureUsesStyle |= createStructure -> style & SS_REALSIZECONTROL;
            #endif

            #ifdef SS_REALSIZEIMAGE
              createStructureUsesStyle |= createStructure -> style & SS_REALSIZEIMAGE;
            #endif

            #ifdef SS_RIGHT
              createStructureUsesStyle |= createStructure -> style & SS_RIGHT;
            #endif

            #ifdef SS_RIGHTJUST
              createStructureUsesStyle |= createStructure -> style & SS_RIGHTJUST;
            #endif

            #ifdef SS_SIMPLE
              createStructureUsesStyle |= createStructure -> style & SS_SIMPLE;
            #endif

            #ifdef SS_SUNKEN
              createStructureUsesStyle |= createStructure -> style & SS_SUNKEN;
            #endif

            #ifdef SS_TYPEMASK
              createStructureUsesStyle |= createStructure -> style & SS_TYPEMASK;
            #endif

            #ifdef SS_WHITEFRAME
              createStructureUsesStyle |= createStructure -> style & SS_WHITEFRAME;
            #endif

            #ifdef SS_WHITERECT
              createStructureUsesStyle |= createStructure -> style & SS_WHITERECT;
            #endif

            #ifdef SS_WORDELLIPSIS
              createStructureUsesStyle |= createStructure -> style & SS_WORDELLIPSIS;
            #endif

            #ifdef TBSTYLE_ALTDRAG
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_ALTDRAG;
            #endif

            #ifdef TBSTYLE_AUTOSIZE
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_AUTOSIZE;
            #endif

            #ifdef TBSTYLE_BUTTON
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_BUTTON;
            #endif

            #ifdef TBSTYLE_CHECK
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_CHECK;
            #endif

            #ifdef TBSTYLE_CHECKGROUP
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_CHECKGROUP;
            #endif

            #ifdef TBSTYLE_CUSTOMERASE
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_CUSTOMERASE;
            #endif

            #ifdef TBSTYLE_DROPDOWN
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_DROPDOWN;
            #endif

            #ifdef TBSTYLE_FLAT
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_FLAT;
            #endif

            #ifdef TBSTYLE_GROUP
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_GROUP;
            #endif

            #ifdef TBSTYLE_LIST
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_LIST;
            #endif

            #ifdef TBSTYLE_NOPREFIX
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_NOPREFIX;
            #endif

            #ifdef TBSTYLE_REGISTERDROP
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_REGISTERDROP;
            #endif

            #ifdef TBSTYLE_SEP
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_SEP;
            #endif

            #ifdef TBSTYLE_TOOLTIPS
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_TOOLTIPS;
            #endif

            #ifdef TBSTYLE_TRANSPARENT
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_TRANSPARENT;
            #endif

            #ifdef TBSTYLE_WRAPABLE
              createStructureUsesStyle |= createStructure -> style & TBSTYLE_WRAPABLE;
            #endif

            #ifdef TCS_BOTTOM
              createStructureUsesStyle |= createStructure -> style & TCS_BOTTOM;
            #endif

            #ifdef TCS_BUTTONS
              createStructureUsesStyle |= createStructure -> style & TCS_BUTTONS;
            #endif

            #ifdef TCS_FIXEDWIDTH
              createStructureUsesStyle |= createStructure -> style & TCS_FIXEDWIDTH;
            #endif

            #ifdef TCS_FLATBUTTONS
              createStructureUsesStyle |= createStructure -> style & TCS_FLATBUTTONS;
            #endif

            #ifdef TCS_FOCUSNEVER
              createStructureUsesStyle |= createStructure -> style & TCS_FOCUSNEVER;
            #endif

            #ifdef TCS_FOCUSONBUTTONDOWN
              createStructureUsesStyle |= createStructure -> style & TCS_FOCUSONBUTTONDOWN;
            #endif

            #ifdef TCS_FORCEICONLEFT
              createStructureUsesStyle |= createStructure -> style & TCS_FORCEICONLEFT;
            #endif

            #ifdef TCS_FORCELABELLEFT
              createStructureUsesStyle |= createStructure -> style & TCS_FORCELABELLEFT;
            #endif

            #ifdef TCS_HOTTRACK
              createStructureUsesStyle |= createStructure -> style & TCS_HOTTRACK;
            #endif

            #ifdef TCS_MULTILINE
              createStructureUsesStyle |= createStructure -> style & TCS_MULTILINE;
            #endif

            #ifdef TCS_MULTISELECT
              createStructureUsesStyle |= createStructure -> style & TCS_MULTISELECT;
            #endif

            #ifdef TCS_OWNERDRAWFIXED
              createStructureUsesStyle |= createStructure -> style & TCS_OWNERDRAWFIXED;
            #endif

            #ifdef TCS_RAGGEDRIGHT
              createStructureUsesStyle |= createStructure -> style & TCS_RAGGEDRIGHT;
            #endif

            #ifdef TCS_RIGHT
              createStructureUsesStyle |= createStructure -> style & TCS_RIGHT;
            #endif

            #ifdef TCS_RIGHTJUSTIFY
              createStructureUsesStyle |= createStructure -> style & TCS_RIGHTJUSTIFY;
            #endif

            #ifdef TCS_SCROLLOPPOSITE
              createStructureUsesStyle |= createStructure -> style & TCS_SCROLLOPPOSITE;
            #endif

            #ifdef TCS_SINGLELINE
              createStructureUsesStyle |= createStructure -> style & TCS_SINGLELINE;
            #endif

            #ifdef TCS_TABS
              createStructureUsesStyle |= createStructure -> style & TCS_TABS;
            #endif

            #ifdef TCS_TOOLTIPS
              createStructureUsesStyle |= createStructure -> style & TCS_TOOLTIPS;
            #endif

            #ifdef TCS_VERTICAL
              createStructureUsesStyle |= createStructure -> style & TCS_VERTICAL;
            #endif

            #ifdef TTS_ALWAYSTIP
              createStructureUsesStyle |= createStructure -> style & TTS_ALWAYSTIP;
            #endif

            #ifdef TTS_BALLOON
              createStructureUsesStyle |= createStructure -> style & TTS_BALLOON;
            #endif

            #ifdef TTS_CLOSE
              createStructureUsesStyle |= createStructure -> style & TTS_CLOSE;
            #endif

            #ifdef TTS_NOANIMATE
              createStructureUsesStyle |= createStructure -> style & TTS_NOANIMATE;
            #endif

            #ifdef TTS_NOFADE
              createStructureUsesStyle |= createStructure -> style & TTS_NOFADE;
            #endif

            #ifdef TTS_NOPREFIX
              createStructureUsesStyle |= createStructure -> style & TTS_NOPREFIX;
            #endif

            #ifdef TTS_USEVISUALSTYLE
              createStructureUsesStyle |= createStructure -> style & TTS_USEVISUALSTYLE;
            #endif

            #ifdef TVS_CHECKBOXES
              createStructureUsesStyle |= createStructure -> style & TVS_CHECKBOXES;
            #endif

            #ifdef TVS_DISABLEDRAGDROP
              createStructureUsesStyle |= createStructure -> style & TVS_DISABLEDRAGDROP;
            #endif

            #ifdef TVS_EDITLABELS
              createStructureUsesStyle |= createStructure -> style & TVS_EDITLABELS;
            #endif

            #ifdef TVS_FULLROWSELECT
              createStructureUsesStyle |= createStructure -> style & TVS_FULLROWSELECT;
            #endif

            #ifdef TVS_HASBUTTONS
              createStructureUsesStyle |= createStructure -> style & TVS_HASBUTTONS;
            #endif

            #ifdef TVS_HASLINES
              createStructureUsesStyle |= createStructure -> style & TVS_HASLINES;
            #endif

            #ifdef TVS_INFOTIP
              createStructureUsesStyle |= createStructure -> style & TVS_INFOTIP;
            #endif

            #ifdef TVS_LINESATROOT
              createStructureUsesStyle |= createStructure -> style & TVS_LINESATROOT;
            #endif

            #ifdef TVS_NOHSCROLL
              createStructureUsesStyle |= createStructure -> style & TVS_NOHSCROLL;
            #endif

            #ifdef TVS_NONEVENHEIGHT
              createStructureUsesStyle |= createStructure -> style & TVS_NONEVENHEIGHT;
            #endif

            #ifdef TVS_NOSCROLL
              createStructureUsesStyle |= createStructure -> style & TVS_NOSCROLL;
            #endif

            #ifdef TVS_NOTOOLTIPS
              createStructureUsesStyle |= createStructure -> style & TVS_NOTOOLTIPS;
            #endif

            #ifdef TVS_RTLREADING
              createStructureUsesStyle |= createStructure -> style & TVS_RTLREADING;
            #endif

            #ifdef TVS_SHOWSELALWAYS
              createStructureUsesStyle |= createStructure -> style & TVS_SHOWSELALWAYS;
            #endif

            #ifdef TVS_SINGLEEXPAND
              createStructureUsesStyle |= createStructure -> style & TVS_SINGLEEXPAND;
            #endif

            #ifdef TVS_TRACKSELECT
              createStructureUsesStyle |= createStructure -> style & TVS_TRACKSELECT;
            #endif

            #ifdef UDS_ALIGNLEFT
              createStructureUsesStyle |= createStructure -> style & UDS_ALIGNLEFT;
            #endif

            #ifdef UDS_ALIGNRIGHT
              createStructureUsesStyle |= createStructure -> style & UDS_ALIGNRIGHT;
            #endif

            #ifdef UDS_ARROWKEYS
              createStructureUsesStyle |= createStructure -> style & UDS_ARROWKEYS;
            #endif

            #ifdef UDS_AUTOBUDDY
              createStructureUsesStyle |= createStructure -> style & UDS_AUTOBUDDY;
            #endif

            #ifdef UDS_HORZ
              createStructureUsesStyle |= createStructure -> style & UDS_HORZ;
            #endif

            #ifdef UDS_HOTTRACK
              createStructureUsesStyle |= createStructure -> style & UDS_HOTTRACK;
            #endif

            #ifdef UDS_NOTHOUSANDS
              createStructureUsesStyle |= createStructure -> style & UDS_NOTHOUSANDS;
            #endif

            #ifdef UDS_SETBUDDYINT
              createStructureUsesStyle |= createStructure -> style & UDS_SETBUDDYINT;
            #endif

            #ifdef UDS_WRAP
              createStructureUsesStyle |= createStructure -> style & UDS_WRAP;
            #endif

            createStructureClassName          = createStructureClassID == windowHandleClassID and 0x00u != windowHandleClassID and (0x0000u == HIWORD(createStructureClassID) and LOWORD(createStructureClassID) < 0xC000u) ? MAKEINTATOM(createStructureClassID) : reinterpret_cast<LPCTSTR>(createStructure -> lpszClass);
            messageTranslationMultilineFormat = true;

            if (log::output(log::format('[', static_cast<unused>(parameter), ", ", createStructure, " {" "\r\n" "  " "className    : ")))
            if (log::output(
              log::compare("BUTTON",         createStructureClassName) ? "\"BUTTON\""         "," "\r\n"  :
              log::compare("COMBOBOX",       createStructureClassName) ? "\"COMBOBOX\""       "," "\r\n"  :
              log::compare("EDIT",           createStructureClassName) ? "\"EDIT\""           "," "\r\n"  :
              log::compare("LISTBOX",        createStructureClassName) ? "\"LISTBOX\""        "," "\r\n"  :
              log::compare("MDICLIENT",      createStructureClassName) ? "\"MDICLIENT\""      "," "\r\n"  :
              log::compare("RichEdit",       createStructureClassName) ? "\"RichEdit\""       "," "\r\n"  :
              log::compare("RICHEDIT_CLASS", createStructureClassName) ? "\"RICHEDIT_CLASS\"" "," "\r\n"  :
              log::compare("SCROLLBAR",      createStructureClassName) ? "\"SCROLLBAR\""      "," "\r\n"  :
              log::compare("STATIC",         createStructureClassName) ? "\"STATIC\""         "," "\r\n"  :
              #ifdef ANIMATE_CLASS
                log::compare(ANIMATE_CLASS, createStructureClassName) ? log::format("`ANIMATE_CLASS` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef DATETIMEPICK_CLASS
                log::compare(DATETIMEPICK_CLASS, createStructureClassName) ? log::format("`DATETIMEPICK_CLASS`\"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef HOTKEY_CLASS
                log::compare(HOTKEY_CLASS, createStructureClassName) ? log::format("`HOTKEY_CLASS` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef LINK_CLASS
                log::compare(LINK_CLASS, createStructureClassName) ? log::format("`LINK_CLASS` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef MONTHCAL_CLASS
                log::compare(MONTHCAL_CLASS, createStructureClassName) ? log::format("`MONTHCAL_CLASS` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef MSFTEDIT_CLASS
                log::compare(MSFTEDIT_CLASS, createStructureClassName) ? log::format("`MSFTEDIT_CLASS` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef NATIVEFNTCTL_CLASS
                log::compare(NATIVEFNTCTL_CLASS, createStructureClassName) ? log::format("`NATIVEFNTCTL_CLASS`\"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef PROGRESS_CLASS
                log::compare(PROGRESS_CLASS, createStructureClassName) ? log::format("`PROGRESS_CLASS` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef REBARCLASSNAME
                log::compare(REBARCLASSNAME, createStructureClassName) ? log::format("`REBARCLASSNAME` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef STANDARD_CLASSES
                log::compare(STANDARD_CLASSES, createStructureClassName) ? log::format("`STANDARD_CLASSES` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef STATUSCLASSNAME
                log::compare(STATUSCLASSNAME, createStructureClassName) ? log::format("`STATUSCLASSNAME` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef TOOLBARCLASSNAME
                log::compare(TOOLBARCLASSNAME, createStructureClassName) ? log::format("`TOOLBARCLASSNAME` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef TOOLTIPS_CLASS
                log::compare(TOOLTIPS_CLASS, createStructureClassName) ? log::format("`TOOLTIPS_CLASS` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef TRACKBAR_CLASS
                log::compare(TRACKBAR_CLASS, createStructureClassName) ? log::format("`TRACKBAR_CLASS` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef UPDOWN_CLASS
                log::compare(UPDOWN_CLASS, createStructureClassName) ? log::format("`UPDOWN_CLASS` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_BUTTON
                log::compare(WC_BUTTON, createStructureClassName) ? log::format("`WC_BUTTON` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_COMBOBOX
                log::compare(WC_COMBOBOX, createStructureClassName) ? log::format("`WC_COMBOBOX` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_COMBOBOXEX
                log::compare(WC_COMBOBOXEX, createStructureClassName) ? log::format("`WC_COMBOBOXEX` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_EDIT
                log::compare(WC_EDIT, createStructureClassName) ? log::format("`WC_EDIT` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_HEADER
                log::compare(WC_HEADER, createStructureClassName) ? log::format("`WC_HEADER` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_IPADDRESS
                log::compare(WC_IPADDRESS, createStructureClassName) ? log::format("`WC_IPADDRESS` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_LINK
                log::compare(WC_LINK, createStructureClassName) ? log::format("`WC_LINK` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_LISTBOX
                log::compare(WC_LISTBOX, createStructureClassName) ? log::format("`WC_LISTBOX` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_LISTVIEW
                log::compare(WC_LISTVIEW, createStructureClassName) ? log::format("`WC_LISTVIEW` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_NATIVEFONTCTL
                log::compare(WC_NATIVEFONTCTL, createStructureClassName) ? log::format("`WC_NATIVEFONTCTL` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_PAGESCROLLER
                log::compare(WC_PAGESCROLLER, createStructureClassName) ? log::format("`WC_PAGESCROLLER` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_SCROLLBAR
                log::compare(WC_SCROLLBAR, createStructureClassName) ? log::format("`WC_SCROLLBAR` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_STATIC
                log::compare(WC_STATIC, createStructureClassName) ? log::format("`WC_STATIC` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_TABCONTROL
                log::compare(WC_TABCONTROL, createStructureClassName) ? log::format("`WC_TABCONTROL` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif
              #ifdef WC_TREEVIEW
                log::compare(WC_TREEVIEW, createStructureClassName) ? log::format("`WC_TREEVIEW` \"", createStructureClassName, "\"" "," "\r\n") :
              #endif

              NULL != createStructureClassName
              ? log::format("\"", createStructureClassName, "\""                    "," "\r\n")
              : log::format(static_cast<void const*>(createStructure -> lpszClass), "," "\r\n")
            )) if (log::output(log::format(
              "  " "extendedStyle: ", static_cast<undefined>(createStructure -> style), createStructureUsesExtendedStyle ? " `" : "",
                createStructure -> dwExStyle & WS_EX_ACCEPTFILES      ? "WS_EX_ACCEPTFILES"         " | " : "",
                createStructure -> dwExStyle & WS_EX_APPWINDOW        ? "WS_EX_APPWINDOW"           " | " : "",
                createStructure -> dwExStyle & WS_EX_CLIENTEDGE       ? "WS_EX_CLIENTEDGE"          " | " : "",
                createStructure -> dwExStyle & WS_EX_COMPOSITED       ? "WS_EX_COMPOSITED"          " | " : "",
                createStructure -> dwExStyle & WS_EX_CONTEXTHELP      ? "WS_EX_CONTEXTHELP"         " | " : "",
                createStructure -> dwExStyle & WS_EX_CONTROLPARENT    ? "WS_EX_CONTROLPARENT"       " | " : "",
                createStructure -> dwExStyle & WS_EX_DLGMODALFRAME    ? "WS_EX_DLGMODALFRAME"       " | " : "",
                createStructure -> dwExStyle & WS_EX_LAYERED          ? "WS_EX_LAYERED"             " | " : "",
                createStructure -> dwExStyle & WS_EX_LAYOUTRTL        ? "WS_EX_LAYOUTRTL"           " | " : "",
                createStructure -> dwExStyle & WS_EX_LEFT             ? "WS_EX_LEFT"                " | " : "",
                createStructure -> dwExStyle & WS_EX_LEFTSCROLLBAR    ? "WS_EX_LEFTSCROLLBAR"       " | " : "",
                createStructure -> dwExStyle & WS_EX_LTRREADING       ? "WS_EX_LTRREADING"          " | " : "",
                createStructure -> dwExStyle & WS_EX_MDICHILD         ? "WS_EX_MDICHILD"            " | " : "",
                createStructure -> dwExStyle & WS_EX_NOACTIVATE       ? "WS_EX_NOACTIVATE"          " | " : "",
                createStructure -> dwExStyle & WS_EX_NOINHERITLAYOUT  ? "WS_EX_NOINHERITLAYOUT"     " | " : "",
                createStructure -> dwExStyle & WS_EX_NOPARENTNOTIFY   ? "WS_EX_NOPARENTNOTIFY"      " | " : "",
                createStructure -> dwExStyle & WS_EX_OVERLAPPEDWINDOW ? "WS_EX_OVERLAPPEDWINDOW"    " | " : "",
                createStructure -> dwExStyle & WS_EX_PALETTEWINDOW    ? "WS_EX_PALETTEWINDOW"       " | " : "",
                createStructure -> dwExStyle & WS_EX_RIGHT            ? "WS_EX_RIGHT"               " | " : "",
                createStructure -> dwExStyle & WS_EX_RIGHTSCROLLBAR   ? "WS_EX_RIGHTSCROLLBAR"      " | " : "",
                createStructure -> dwExStyle & WS_EX_RTLREADING       ? "WS_EX_RTLREADING"          " | " : "",
                createStructure -> dwExStyle & WS_EX_STATICEDGE       ? "WS_EX_STATICEDGE"          " | " : "",
                createStructure -> dwExStyle & WS_EX_TOOLWINDOW       ? "WS_EX_TOOLWINDOW"          " | " : "",
                createStructure -> dwExStyle & WS_EX_TOPMOST          ? "WS_EX_TOPMOST"             " | " : "",
                createStructure -> dwExStyle & WS_EX_TRANSPARENT      ? "WS_EX_TRANSPARENT"         " | " : "",
                createStructure -> dwExStyle & WS_EX_WINDOWEDGE       ? "WS_EX_WINDOWEDGE"          " | " : "",
                #ifdef WS_EX_NOREDIRECTIONBITMAP
                  createStructure -> dwExStyle & WS_EX_NOREDIRECTIONBITMAP ? "WS_EX_NOREDIRECTIONBITMAP" " | " : "",
                #endif
              createStructureUsesExtendedStyle ? "…0x00`" : "",      "," "\r\n",
              "  " "height       : ", (number) createStructure -> cy, "," "\r\n"
              "  " "instance     : ", createStructure -> hInstance,   "," "\r\n"
              "  " "menu         : ", createStructure -> hMenu,       "," "\r\n"
            ))) if (log::output(
              parameter == static_cast<WPARAM>(reinterpret_cast<uintptr_t>(createStructure -> lpCreateParams))
              ? log::format("  " "parameters   : ", static_cast<special>(parameter),   "," "\r\n")
              : log::format("  " "parameters   : ", createStructure -> lpCreateParams, "," "\r\n")
            )) if (log::output(log::format(
              "  " "parentWindow : ", createStructure -> hwndParent, "," "\r\n"
              "  " "style        : ", static_cast<undefined>(createStructure -> style), createStructureUsesStyle ? " `" : "",
                createStructure -> style & WS_BORDER           ? "WS_BORDER"           " | " : "",
                createStructure -> style & WS_CAPTION          ? "WS_CAPTION"          " | " : "",
                createStructure -> style & WS_CHILD            ? "WS_CHILD"            " | " : "",
                createStructure -> style & WS_CHILDWINDOW      ? "WS_CHILDWINDOW"      " | " : "",
                createStructure -> style & WS_CLIPCHILDREN     ? "WS_CLIPCHILDREN"     " | " : "",
                createStructure -> style & WS_CLIPSIBLINGS     ? "WS_CLIPSIBLINGS"     " | " : "",
                createStructure -> style & WS_DISABLED         ? "WS_DISABLED"         " | " : "",
                createStructure -> style & WS_DLGFRAME         ? "WS_DLGFRAME"         " | " : "",
                createStructure -> style & WS_GROUP            ? "WS_GROUP"            " | " : "",
                createStructure -> style & WS_HSCROLL          ? "WS_HSCROLL"          " | " : "",
                createStructure -> style & WS_ICONIC           ? "WS_ICONIC"           " | " : "",
                createStructure -> style & WS_MAXIMIZE         ? "WS_MAXIMIZE"         " | " : "",
                createStructure -> style & WS_MAXIMIZEBOX      ? "WS_MAXIMIZEBOX"      " | " : "",
                createStructure -> style & WS_MINIMIZE         ? "WS_MINIMIZE"         " | " : "",
                createStructure -> style & WS_MINIMIZEBOX      ? "WS_MINIMIZEBOX"      " | " : "",
                createStructure -> style & WS_OVERLAPPED       ? "WS_OVERLAPPED"       " | " : "",
                createStructure -> style & WS_OVERLAPPEDWINDOW ? "WS_OVERLAPPEDWINDOW" " | " : "",
                createStructure -> style & WS_POPUP            ? "WS_POPUP"            " | " : "",
                createStructure -> style & WS_POPUPWINDOW      ? "WS_POPUPWINDOW"      " | " : "",
                createStructure -> style & WS_SIZEBOX          ? "WS_SIZEBOX"          " | " : "",
                createStructure -> style & WS_SYSMENU          ? "WS_SYSMENU"          " | " : "",
                createStructure -> style & WS_TABSTOP          ? "WS_TABSTOP"          " | " : "",
                createStructure -> style & WS_THICKFRAME       ? "WS_THICKFRAME"       " | " : "",
                createStructure -> style & WS_TILED            ? "WS_TILED"            " | " : "",
                createStructure -> style & WS_TILEDWINDOW      ? "WS_TILEDWINDOW"      " | " : "",
                createStructure -> style & WS_VISIBLE          ? "WS_VISIBLE"          " | " : "",
                createStructure -> style & WS_VSCROLL          ? "WS_VSCROLL"          " | " : "",
                #ifdef ACS_AUTOPLAY
                  createStructure -> style & ACS_AUTOPLAY ? "ACS_AUTOPLAY" " | " : "",
                #endif
                #ifdef ACS_CENTER
                  createStructure -> style & ACS_CENTER ? "ACS_CENTER" " | " : "",
                #endif
                #ifdef ACS_TIMER
                  createStructure -> style & ACS_TIMER ? "ACS_TIMER" " | " : "",
                #endif
                #ifdef ACS_TRANSPARENT
                  createStructure -> style & ACS_TRANSPARENT ? "ACS_TRANSPARENT" " | " : "",
                #endif
                #ifdef BS_3STATE
                  createStructure -> style & BS_3STATE ? "BS_3STATE" " | " : "",
                #endif
                #ifdef BS_AUTO3STATE
                  createStructure -> style & BS_AUTO3STATE ? "BS_AUTO3STATE" " | " : "",
                #endif
                #ifdef BS_AUTOCHECKBOX
                  createStructure -> style & BS_AUTOCHECKBOX ? "BS_AUTOCHECKBOX" " | " : "",
                #endif
                #ifdef BS_AUTORADIOBUTTON
                  createStructure -> style & BS_AUTORADIOBUTTON ? "BS_AUTORADIOBUTTON" " | " : "",
                #endif
                #ifdef BS_BITMAP
                  createStructure -> style & BS_BITMAP ? "BS_BITMAP" " | " : "",
                #endif
                #ifdef BS_BOTTOM
                  createStructure -> style & BS_BOTTOM ? "BS_BOTTOM" " | " : "",
                #endif
                #ifdef BS_CENTER
                  createStructure -> style & BS_CENTER ? "BS_CENTER" " | " : "",
                #endif
                #ifdef BS_CHECKBOX
                  createStructure -> style & BS_CHECKBOX ? "BS_CHECKBOX" " | " : "",
                #endif
                #ifdef BS_COMMANDLINK
                  createStructure -> style & BS_COMMANDLINK ? "BS_COMMANDLINK" " | " : "",
                #endif
                #ifdef BS_DEFCOMMANDLINK
                  createStructure -> style & BS_DEFCOMMANDLINK ? "BS_DEFCOMMANDLINK" " | " : "",
                #endif
                #ifdef BS_DEFPUSHBUTTON
                  createStructure -> style & BS_DEFPUSHBUTTON ? "BS_DEFPUSHBUTTON" " | " : "",
                #endif
                #ifdef BS_DEFSPLITBUTTON
                  createStructure -> style & BS_DEFSPLITBUTTON ? "BS_DEFSPLITBUTTON" " | " : "",
                #endif
                #ifdef BS_GROUPBOX
                  createStructure -> style & BS_GROUPBOX ? "BS_GROUPBOX" " | " : "",
                #endif
                #ifdef BS_ICON
                  createStructure -> style & BS_ICON ? "BS_ICON" " | " : "",
                #endif
                #ifdef BS_FLAT
                  createStructure -> style & BS_FLAT ? "BS_FLAT" " | " : "",
                #endif
                #ifdef BS_LEFT
                  createStructure -> style & BS_LEFT ? "BS_LEFT" " | " : "",
                #endif
                #ifdef BS_LEFTTEXT
                  createStructure -> style & BS_LEFTTEXT ? "BS_LEFTTEXT" " | " : "",
                #endif
                #ifdef BS_MULTILINE
                  createStructure -> style & BS_MULTILINE ? "BS_MULTILINE" " | " : "",
                #endif
                #ifdef BS_NOTIFY
                  createStructure -> style & BS_NOTIFY ? "BS_NOTIFY" " | " : "",
                #endif
                #ifdef BS_OWNERDRAW
                  createStructure -> style & BS_OWNERDRAW ? "BS_OWNERDRAW" " | " : "",
                #endif
                #ifdef BS_PUSHBUTTON
                  createStructure -> style & BS_PUSHBUTTON ? "BS_PUSHBUTTON" " | " : "",
                #endif
                #ifdef BS_PUSHLIKE
                  createStructure -> style & BS_PUSHLIKE ? "BS_PUSHLIKE" " | " : "",
                #endif
                #ifdef BS_RADIOBUTTON
                  createStructure -> style & BS_RADIOBUTTON ? "BS_RADIOBUTTON" " | " : "",
                #endif
                #ifdef BS_RIGHT
                  createStructure -> style & BS_RIGHT ? "BS_RIGHT" " | " : "",
                #endif
                #ifdef BS_RIGHTBUTTON
                  createStructure -> style & BS_RIGHTBUTTON ? "BS_RIGHTBUTTON" " | " : "",
                #endif
                #ifdef BS_SPLITBUTTON
                  createStructure -> style & BS_SPLITBUTTON ? "BS_SPLITBUTTON" " | " : "",
                #endif
                #ifdef BS_TEXT
                  createStructure -> style & BS_TEXT ? "BS_TEXT" " | " : "",
                #endif
                #ifdef BS_TOP
                  createStructure -> style & BS_TOP ? "BS_TOP" " | " : "",
                #endif
                #ifdef BS_TYPEMASK
                  createStructure -> style & BS_TYPEMASK ? "BS_TYPEMASK" " | " : "",
                #endif
                #ifdef BS_USERBUTTON
                  createStructure -> style & BS_USERBUTTON ? "BS_USERBUTTON" " | " : "",
                #endif
                #ifdef BS_VCENTER
                  createStructure -> style & BS_VCENTER ? "BS_VCENTER" " | " : "",
                #endif
                #ifdef BTNS_AUTOSIZE
                  createStructure -> style & BTNS_AUTOSIZE ? "BTNS_AUTOSIZE" " | " : "",
                #endif
                #ifdef BTNS_BUTTON
                  createStructure -> style & BTNS_BUTTON ? "BTNS_BUTTON" " | " : "",
                #endif
                #ifdef BTNS_CHECK
                  createStructure -> style & BTNS_CHECK ? "BTNS_CHECK" " | " : "",
                #endif
                #ifdef BTNS_CHECKGROUP
                  createStructure -> style & BTNS_CHECKGROUP ? "BTNS_CHECKGROUP" " | " : "",
                #endif
                #ifdef BTNS_DROPDOWN
                  createStructure -> style & BTNS_DROPDOWN ? "BTNS_DROPDOWN" " | " : "",
                #endif
                #ifdef BTNS_GROUP
                  createStructure -> style & BTNS_GROUP ? "BTNS_GROUP" " | " : "",
                #endif
                #ifdef BTNS_NOPREFIX
                  createStructure -> style & BTNS_NOPREFIX ? "BTNS_NOPREFIX" " | " : "",
                #endif
                #ifdef BTNS_SEP
                  createStructure -> style & BTNS_SEP ? "BTNS_SEP" " | " : "",
                #endif
                #ifdef BTNS_SHOWTEXT
                  createStructure -> style & BTNS_SHOWTEXT ? "BTNS_SHOWTEXT" " | " : "",
                #endif
                #ifdef BTNS_WHOLEDROPDOWN
                  createStructure -> style & BTNS_WHOLEDROPDOWN ? "BTNS_WHOLEDROPDOWN" " | " : "",
                #endif
                #ifdef CBES_EX_CASESENSITIVE
                  createStructure -> style & CBES_EX_CASESENSITIVE ? "CBES_EX_CASESENSITIVE" " | " : "",
                #endif
                #ifdef CBES_EX_NOEDITIMAGE
                  createStructure -> style & CBES_EX_NOEDITIMAGE ? "CBES_EX_NOEDITIMAGE" " | " : "",
                #endif
                #ifdef CBES_EX_NOEDITIMAGEINDENT
                  createStructure -> style & CBES_EX_NOEDITIMAGEINDENT ? "CBES_EX_NOEDITIMAGEINDENT" " | " : "",
                #endif
                #ifdef CBES_EX_NOSIZELIMIT
                  createStructure -> style & CBES_EX_NOSIZELIMIT ? "CBES_EX_NOSIZELIMIT" " | " : "",
                #endif
                #ifdef CBES_EX_PATHWORDBREAKPROC
                  createStructure -> style & CBES_EX_PATHWORDBREAKPROC ? "CBES_EX_PATHWORDBREAKPROC" " | " : "",
                #endif
                #ifdef CBES_EX_TEXTENDELLIPSIS
                  createStructure -> style & CBES_EX_TEXTENDELLIPSIS ? "CBES_EX_TEXTENDELLIPSIS" " | " : "",
                #endif
                #ifdef CBS_AUTOHSCROLL
                  createStructure -> style & CBS_AUTOHSCROLL ? "CBS_AUTOHSCROLL" " | " : "",
                #endif
                #ifdef CBS_DISABLENOSCROLL
                  createStructure -> style & CBS_DISABLENOSCROLL ? "CBS_DISABLENOSCROLL" " | " : "",
                #endif
                #ifdef CBS_DROPDOWN
                  createStructure -> style & CBS_DROPDOWN ? "CBS_DROPDOWN" " | " : "",
                #endif
                #ifdef CBS_DROPDOWNLIST
                  createStructure -> style & CBS_DROPDOWNLIST ? "CBS_DROPDOWNLIST" " | " : "",
                #endif
                #ifdef CBS_HASSTRINGS
                  createStructure -> style & CBS_HASSTRINGS ? "CBS_HASSTRINGS" " | " : "",
                #endif
                #ifdef CBS_LOWERCASE
                  createStructure -> style & CBS_LOWERCASE ? "CBS_LOWERCASE" " | " : "",
                #endif
                #ifdef CBS_NOINTEGRALHEIGHT
                  createStructure -> style & CBS_NOINTEGRALHEIGHT ? "CBS_NOINTEGRALHEIGHT" " | " : "",
                #endif
                #ifdef CBS_OEMCONVERT
                  createStructure -> style & CBS_OEMCONVERT ? "CBS_OEMCONVERT" " | " : "",
                #endif
                #ifdef CBS_OWNERDRAWFIXED
                  createStructure -> style & CBS_OWNERDRAWFIXED ? "CBS_OWNERDRAWFIXED" " | " : "",
                #endif
                #ifdef CBS_OWNERDRAWVARIABLE
                  createStructure -> style & CBS_OWNERDRAWVARIABLE ? "CBS_OWNERDRAWVARIABLE" " | " : "",
                #endif
                #ifdef CBS_SIMPLE
                  createStructure -> style & CBS_SIMPLE ? "CBS_SIMPLE" " | " : "",
                #endif
                #ifdef CBS_SORT
                  createStructure -> style & CBS_SORT ? "CBS_SORT" " | " : "",
                #endif
                #ifdef CBS_UPPERCASE
                  createStructure -> style & CBS_UPPERCASE ? "CBS_UPPERCASE" " | " : "",
                #endif
                #ifdef DTS_APPCANPARSE
                  createStructure -> style & DTS_APPCANPARSE ? "DTS_APPCANPARSE" " | " : "",
                #endif
                #ifdef DTS_LONGDATEFORMAT
                  createStructure -> style & DTS_LONGDATEFORMAT ? "DTS_LONGDATEFORMAT" " | " : "",
                #endif
                #ifdef DTS_RIGHTALIGN
                  createStructure -> style & DTS_RIGHTALIGN ? "DTS_RIGHTALIGN" " | " : "",
                #endif
                #ifdef DTS_SHORTDATECENTURYFORMAT
                  createStructure -> style & DTS_SHORTDATECENTURYFORMAT ? "DTS_SHORTDATECENTURYFORMAT" " | " : "",
                #endif
                #ifdef DTS_SHORTDATEFORMAT
                  createStructure -> style & DTS_SHORTDATEFORMAT ? "DTS_SHORTDATEFORMAT" " | " : "",
                #endif
                #ifdef DTS_SHOWNONE
                  createStructure -> style & DTS_SHOWNONE ? "DTS_SHOWNONE" " | " : "",
                #endif
                #ifdef DTS_TIMEFORMAT
                  createStructure -> style & DTS_TIMEFORMAT ? "DTS_TIMEFORMAT" " | " : "",
                #endif
                #ifdef DTS_UPDOWN
                  createStructure -> style & DTS_UPDOWN ? "DTS_UPDOWN" " | " : "",
                #endif
                #ifdef ES_AUTOHSCROLL
                  createStructure -> style & ES_AUTOHSCROLL ? "ES_AUTOHSCROLL" " | " : "",
                #endif
                #ifdef ES_AUTOVSCROLL
                  createStructure -> style & ES_AUTOVSCROLL ? "ES_AUTOVSCROLL" " | " : "",
                #endif
                #ifdef ES_CENTER
                  createStructure -> style & ES_CENTER ? "ES_CENTER" " | " : "",
                #endif
                #ifdef ES_DISABLENOSCROLL
                  createStructure -> style & ES_DISABLENOSCROLL ? "ES_DISABLENOSCROLL" " | " : "",
                #endif
                #ifdef ES_EX_NOCALLOLEINIT
                  createStructure -> style & ES_EX_NOCALLOLEINIT ? "ES_EX_NOCALLOLEINIT" " | " : "",
                #endif
                #ifdef ES_LEFT
                  createStructure -> style & ES_LEFT ? "ES_LEFT" " | " : "",
                #endif
                #ifdef ES_LOWERCASE
                  createStructure -> style & ES_LOWERCASE ? "ES_LOWERCASE" " | " : "",
                #endif
                #ifdef ES_MULTILINE
                  createStructure -> style & ES_MULTILINE ? "ES_MULTILINE" " | " : "",
                #endif
                #ifdef ES_NOHIDESEL
                  createStructure -> style & ES_NOHIDESEL ? "ES_NOHIDESEL" " | " : "",
                #endif
                #ifdef ES_NOIME
                  createStructure -> style & ES_NOIME ? "ES_NOIME" " | " : "",
                #endif
                #ifdef ES_NOOLEDRAGDROP
                  createStructure -> style & ES_NOOLEDRAGDROP ? "ES_NOOLEDRAGDROP" " | " : "",
                #endif
                #ifdef ES_NUMBER
                  createStructure -> style & ES_NUMBER ? "ES_NUMBER" " | " : "",
                #endif
                #ifdef ES_OEMCONVERT
                  createStructure -> style & ES_OEMCONVERT ? "ES_OEMCONVERT" " | " : "",
                #endif
                #ifdef ES_PASSWORD
                  createStructure -> style & ES_PASSWORD ? "ES_PASSWORD" " | " : "",
                #endif
                #ifdef ES_READONLY
                  createStructure -> style & ES_READONLY ? "ES_READONLY" " | " : "",
                #endif
                #ifdef ES_RIGHT
                  createStructure -> style & ES_RIGHT ? "ES_RIGHT" " | " : "",
                #endif
                #ifdef ES_SAVESEL
                  createStructure -> style & ES_SAVESEL ? "ES_SAVESEL" " | " : "",
                #endif
                #ifdef ES_SELECTIONBAR
                  createStructure -> style & ES_SELECTIONBAR ? "ES_SELECTIONBAR" " | " : "",
                #endif
                #ifdef ES_SELFIME
                  createStructure -> style & ES_SELFIME ? "ES_SELFIME" " | " : "",
                #endif
                #ifdef ES_SUNKEN
                  createStructure -> style & ES_SUNKEN ? "ES_SUNKEN" " | " : "",
                #endif
                #ifdef ES_UPPERCASE
                  createStructure -> style & ES_UPPERCASE ? "ES_UPPERCASE" " | " : "",
                #endif
                #ifdef ES_VERTICAL
                  createStructure -> style & ES_VERTICAL ? "ES_VERTICAL" " | " : "",
                #endif
                ""
            ))) if (log::output(log::format(
                #ifdef ES_WANTRETURN
                  createStructure -> style & ES_WANTRETURN ? "ES_WANTRETURN" " | " : "",
                #endif
                #ifdef HDS_BUTTONS
                  createStructure -> style & HDS_BUTTONS ? "HDS_BUTTONS" " | " : "",
                #endif
                #ifdef HDS_CHECKBOXES
                  createStructure -> style & HDS_CHECKBOXES ? "HDS_CHECKBOXES" " | " : "",
                #endif
                #ifdef HDS_DRAGDROP
                  createStructure -> style & HDS_DRAGDROP ? "HDS_DRAGDROP" " | " : "",
                #endif
                #ifdef HDS_FILTERBAR
                  createStructure -> style & HDS_FILTERBAR ? "HDS_FILTERBAR" " | " : "",
                #endif
                #ifdef HDS_FLAT
                  createStructure -> style & HDS_FLAT ? "HDS_FLAT" " | " : "",
                #endif
                #ifdef HDS_FULLDRAG
                  createStructure -> style & HDS_FULLDRAG ? "HDS_FULLDRAG" " | " : "",
                #endif
                #ifdef HDS_HIDDEN
                  createStructure -> style & HDS_HIDDEN ? "HDS_HIDDEN" " | " : "",
                #endif
                #ifdef HDS_HORZ
                  createStructure -> style & HDS_HORZ ? "HDS_HORZ" " | " : "",
                #endif
                #ifdef HDS_HOTTRACK
                  createStructure -> style & HDS_HOTTRACK ? "HDS_HOTTRACK" " | " : "",
                #endif
                #ifdef HDS_NOSIZING
                  createStructure -> style & HDS_NOSIZING ? "HDS_NOSIZING" " | " : "",
                #endif
                #ifdef HDS_OVERFLOW
                  createStructure -> style & HDS_OVERFLOW ? "HDS_OVERFLOW" " | " : "",
                #endif
                #ifdef LBS_COMBOBOX
                  createStructure -> style & LBS_COMBOBOX ? "LBS_COMBOBOX" " | " : "",
                #endif
                #ifdef LBS_DISABLENOSCROLL
                  createStructure -> style & LBS_DISABLENOSCROLL ? "LBS_DISABLENOSCROLL" " | " : "",
                #endif
                #ifdef LBS_EXTENDEDSEL
                  createStructure -> style & LBS_EXTENDEDSEL ? "LBS_EXTENDEDSEL" " | " : "",
                #endif
                #ifdef LBS_HASSTRINGS
                  createStructure -> style & LBS_HASSTRINGS ? "LBS_HASSTRINGS" " | " : "",
                #endif
                #ifdef LBS_MULTICOLUMN
                  createStructure -> style & LBS_MULTICOLUMN ? "LBS_MULTICOLUMN" " | " : "",
                #endif
                #ifdef LBS_MULTIPLESEL
                  createStructure -> style & LBS_MULTIPLESEL ? "LBS_MULTIPLESEL" " | " : "",
                #endif
                #ifdef LBS_NODATA
                  createStructure -> style & LBS_NODATA ? "LBS_NODATA" " | " : "",
                #endif
                #ifdef LBS_NOINTEGRALHEIGHT
                  createStructure -> style & LBS_NOINTEGRALHEIGHT ? "LBS_NOINTEGRALHEIGHT" " | " : "",
                #endif
                #ifdef LBS_NOREDRAW
                  createStructure -> style & LBS_NOREDRAW ? "LBS_NOREDRAW" " | " : "",
                #endif
                #ifdef LBS_NOSEL
                  createStructure -> style & LBS_NOSEL ? "LBS_NOSEL" " | " : "",
                #endif
                #ifdef LBS_NOTIFY
                  createStructure -> style & LBS_NOTIFY ? "LBS_NOTIFY" " | " : "",
                #endif
                #ifdef LBS_OWNERDRAWFIXED
                  createStructure -> style & LBS_OWNERDRAWFIXED ? "LBS_OWNERDRAWFIXED" " | " : "",
                #endif
                #ifdef LBS_OWNERDRAWVARIABLE
                  createStructure -> style & LBS_OWNERDRAWVARIABLE ? "LBS_OWNERDRAWVARIABLE" " | " : "",
                #endif
                #ifdef LBS_SORT
                  createStructure -> style & LBS_SORT ? "LBS_SORT" " | " : "",
                #endif
                #ifdef LBS_STANDARD
                  createStructure -> style & LBS_STANDARD ? "LBS_STANDARD" " | " : "",
                #endif
                #ifdef LBS_USETABSTOPS
                  createStructure -> style & LBS_USETABSTOPS ? "LBS_USETABSTOPS" " | " : "",
                #endif
                #ifdef LBS_WANTKEYBOARDINPUT
                  createStructure -> style & LBS_WANTKEYBOARDINPUT ? "LBS_WANTKEYBOARDINPUT" " | " : "",
                #endif
                #ifdef LVS_ALIGNLEFT
                  createStructure -> style & LVS_ALIGNLEFT ? "LVS_ALIGNLEFT" " | " : "",
                #endif
                #ifdef LVS_ALIGNMASK
                  createStructure -> style & LVS_ALIGNMASK ? "LVS_ALIGNMASK" " | " : "",
                #endif
                #ifdef LVS_ALIGNTOP
                  createStructure -> style & LVS_ALIGNTOP ? "LVS_ALIGNTOP" " | " : "",
                #endif
                #ifdef LVS_AUTOARRANGE
                  createStructure -> style & LVS_AUTOARRANGE ? "LVS_AUTOARRANGE" " | " : "",
                #endif
                #ifdef LVS_EDITLABELS
                  createStructure -> style & LVS_EDITLABELS ? "LVS_EDITLABELS" " | " : "",
                #endif
                #ifdef LVS_ICON
                  createStructure -> style & LVS_ICON ? "LVS_ICON" " | " : "",
                #endif
                #ifdef LVS_LIST
                  createStructure -> style & LVS_LIST ? "LVS_LIST" " | " : "",
                #endif
                #ifdef LVS_NOCOLUMNHEADER
                  createStructure -> style & LVS_NOCOLUMNHEADER ? "LVS_NOCOLUMNHEADER" " | " : "",
                #endif
                #ifdef LVS_NOLABELWRAP
                  createStructure -> style & LVS_NOLABELWRAP ? "LVS_NOLABELWRAP" " | " : "",
                #endif
                #ifdef LVS_NOSCROLL
                  createStructure -> style & LVS_NOSCROLL ? "LVS_NOSCROLL" " | " : "",
                #endif
                #ifdef LVS_NOSORTHEADER
                  createStructure -> style & LVS_NOSORTHEADER ? "LVS_NOSORTHEADER" " | " : "",
                #endif
                #ifdef LVS_OWNERDATA
                  createStructure -> style & LVS_OWNERDATA ? "LVS_OWNERDATA" " | " : "",
                #endif
                #ifdef LVS_OWNERDRAWFIXED
                  createStructure -> style & LVS_OWNERDRAWFIXED ? "LVS_OWNERDRAWFIXED" " | " : "",
                #endif
                #ifdef LVS_REPORT
                  createStructure -> style & LVS_REPORT ? "LVS_REPORT" " | " : "",
                #endif
                #ifdef LVS_SHAREIMAGELISTS
                  createStructure -> style & LVS_SHAREIMAGELISTS ? "LVS_SHAREIMAGELISTS" " | " : "",
                #endif
                #ifdef LVS_SHOWSELALWAYS
                  createStructure -> style & LVS_SHOWSELALWAYS ? "LVS_SHOWSELALWAYS" " | " : "",
                #endif
                #ifdef LVS_SINGLESEL
                  createStructure -> style & LVS_SINGLESEL ? "LVS_SINGLESEL" " | " : "",
                #endif
                #ifdef LVS_SMALLICON
                  createStructure -> style & LVS_SMALLICON ? "LVS_SMALLICON" " | " : "",
                #endif
                #ifdef LVS_SORTASCENDING
                  createStructure -> style & LVS_SORTASCENDING ? "LVS_SORTASCENDING" " | " : "",
                #endif
                #ifdef LVS_SORTDESCENDING
                  createStructure -> style & LVS_SORTDESCENDING ? "LVS_SORTDESCENDING" " | " : "",
                #endif
                #ifdef LVS_TYPEMASK
                  createStructure -> style & LVS_TYPEMASK ? "LVS_TYPEMASK" " | " : "",
                #endif
                #ifdef LVS_TYPESTYLEMASK
                  createStructure -> style & LVS_TYPESTYLEMASK ? "LVS_TYPESTYLEMASK" " | " : "",
                #endif
                #ifdef LWS_IGNORERETURN
                  createStructure -> style & LWS_IGNORERETURN ? "LWS_IGNORERETURN" " | " : "",
                #endif
                #ifdef LWS_NOPREFIX
                  createStructure -> style & LWS_NOPREFIX ? "LWS_NOPREFIX" " | " : "",
                #endif
                #ifdef LWS_RIGHT
                  createStructure -> style & LWS_RIGHT ? "LWS_RIGHT" " | " : "",
                #endif
                #ifdef LWS_TRANSPARENT
                  createStructure -> style & LWS_TRANSPARENT ? "LWS_TRANSPARENT" " | " : "",
                #endif
                #ifdef LWS_USECUSTOMTEXT
                  createStructure -> style & LWS_USECUSTOMTEXT ? "LWS_USECUSTOMTEXT" " | " : "",
                #endif
                #ifdef LWS_USEVISUALSTYLE
                  createStructure -> style & LWS_USEVISUALSTYLE ? "LWS_USEVISUALSTYLE" " | " : "",
                #endif
                #ifdef MCS_DAYSTATE
                  createStructure -> style & MCS_DAYSTATE ? "MCS_DAYSTATE" " | " : "",
                #endif
                #ifdef MCS_MULTISELECT
                  createStructure -> style & MCS_MULTISELECT ? "MCS_MULTISELECT" " | " : "",
                #endif
                #ifdef MCS_NOSELCHANGEONNAV
                  createStructure -> style & MCS_NOSELCHANGEONNAV ? "MCS_NOSELCHANGEONNAV" " | " : "",
                #endif
                #ifdef MCS_NOTODAY
                  createStructure -> style & MCS_NOTODAY ? "MCS_NOTODAY" " | " : "",
                #endif
                #ifdef MCS_NOTODAYCIRCLE
                  createStructure -> style & MCS_NOTODAYCIRCLE ? "MCS_NOTODAYCIRCLE" " | " : "",
                #endif
                #ifdef MCS_NOTRAILINGDATES
                  createStructure -> style & MCS_NOTRAILINGDATES ? "MCS_NOTRAILINGDATES" " | " : "",
                #endif
                #ifdef MCS_SHORTDAYSOFWEEK
                  createStructure -> style & MCS_SHORTDAYSOFWEEK ? "MCS_SHORTDAYSOFWEEK" " | " : "",
                #endif
                #ifdef MCS_WEEKNUMBERS
                  createStructure -> style & MCS_WEEKNUMBERS ? "MCS_WEEKNUMBERS" " | " : "",
                #endif
                #ifdef PBS_MARQUEE
                  createStructure -> style & PBS_MARQUEE ? "PBS_MARQUEE" " | " : "",
                #endif
                #ifdef PBS_SMOOTH
                  createStructure -> style & PBS_SMOOTH ? "PBS_SMOOTH" " | " : "",
                #endif
                #ifdef PBS_SMOOTHREVERSE
                  createStructure -> style & PBS_SMOOTHREVERSE ? "PBS_SMOOTHREVERSE" " | " : "",
                #endif
                #ifdef PBS_VERTICAL
                  createStructure -> style & PBS_VERTICAL ? "PBS_VERTICAL" " | " : "",
                #endif
                #ifdef PGS_AUTOSCROLL
                  createStructure -> style & PGS_AUTOSCROLL ? "PGS_AUTOSCROLL" " | " : "",
                #endif
                #ifdef PGS_DRAGNDROP
                  createStructure -> style & PGS_DRAGNDROP ? "PGS_DRAGNDROP" " | " : "",
                #endif
                #ifdef PGS_HORZ
                  createStructure -> style & PGS_HORZ ? "PGS_HORZ" " | " : "",
                #endif
                #ifdef PGS_VERT
                  createStructure -> style & PGS_VERT ? "PGS_VERT" " | " : "",
                #endif
                #ifdef RBS_AUTOSIZE
                  createStructure -> style & RBS_AUTOSIZE ? "RBS_AUTOSIZE" " | " : "",
                #endif
                #ifdef RBS_BANDBORDERS
                  createStructure -> style & RBS_BANDBORDERS ? "RBS_BANDBORDERS" " | " : "",
                #endif
                #ifdef RBS_DBLCLKTOGGLE
                  createStructure -> style & RBS_DBLCLKTOGGLE ? "RBS_DBLCLKTOGGLE" " | " : "",
                #endif
                #ifdef RBS_FIXEDORDER
                  createStructure -> style & RBS_FIXEDORDER ? "RBS_FIXEDORDER" " | " : "",
                #endif
                #ifdef RBS_REGISTERDROP
                  createStructure -> style & RBS_REGISTERDROP ? "RBS_REGISTERDROP" " | " : "",
                #endif
                #ifdef RBS_TOOLTIPS
                  createStructure -> style & RBS_TOOLTIPS ? "RBS_TOOLTIPS" " | " : "",
                #endif
                #ifdef RBS_VARHEIGHT
                  createStructure -> style & RBS_VARHEIGHT ? "RBS_VARHEIGHT" " | " : "",
                #endif
                #ifdef RBS_VERTICALGRIPPER
                  createStructure -> style & RBS_VERTICALGRIPPER ? "RBS_VERTICALGRIPPER" " | " : "",
                #endif
                #ifdef SBARS_SIZEGRIP
                  createStructure -> style & SBARS_SIZEGRIP ? "SBARS_SIZEGRIP" " | " : "",
                #endif
                #ifdef SBARS_TOOLTIPS
                  createStructure -> style & SBARS_TOOLTIPS ? "SBARS_TOOLTIPS" " | " : "",
                #endif
                #ifdef SBT_TOOLTIPS
                  createStructure -> style & SBT_TOOLTIPS ? "SBT_TOOLTIPS" " | " : "",
                #endif
                #ifdef SBS_BOTTOMALIGN
                  createStructure -> style & SBS_BOTTOMALIGN ? "SBS_BOTTOMALIGN" " | " : "",
                #endif
                #ifdef SBS_HORZ
                  createStructure -> style & SBS_HORZ ? "SBS_HORZ" " | " : "",
                #endif
                #ifdef SBS_LEFTALIGN
                  createStructure -> style & SBS_LEFTALIGN ? "SBS_LEFTALIGN" " | " : "",
                #endif
                #ifdef SBS_RIGHTALIGN
                  createStructure -> style & SBS_RIGHTALIGN ? "SBS_RIGHTALIGN" " | " : "",
                #endif
                #ifdef SBS_SIZEBOX
                  createStructure -> style & SBS_SIZEBOX ? "SBS_SIZEBOX" " | " : "",
                #endif
                #ifdef SBS_SIZEBOXBOTTOMRIGHTALIGN
                  createStructure -> style & SBS_SIZEBOXBOTTOMRIGHTALIGN ? "SBS_SIZEBOXBOTTOMRIGHTALIGN" " | " : "",
                #endif
                #ifdef SBS_SIZEBOXTOPLEFTALIGN
                  createStructure -> style & SBS_SIZEBOXTOPLEFTALIGN ? "SBS_SIZEBOXTOPLEFTALIGN" " | " : "",
                #endif
                #ifdef SBS_SIZEGRIP
                  createStructure -> style & SBS_SIZEGRIP ? "SBS_SIZEGRIP" " | " : "",
                #endif
                #ifdef SBS_TOPALIGN
                  createStructure -> style & SBS_TOPALIGN ? "SBS_TOPALIGN" " | " : "",
                #endif
                #ifdef SBS_VERT
                  createStructure -> style & SBS_VERT ? "SBS_VERT" " | " : "",
                #endif
                #ifdef SS_BITMAP
                  createStructure -> style & SS_BITMAP ? "SS_BITMAP" " | " : "",
                #endif
                #ifdef SS_BLACKFRAME
                  createStructure -> style & SS_BLACKFRAME ? "SS_BLACKFRAME" " | " : "",
                #endif
                #ifdef SS_BLACKRECT
                  createStructure -> style & SS_BLACKRECT ? "SS_BLACKRECT" " | " : "",
                #endif
                #ifdef SS_CENTER
                  createStructure -> style & SS_CENTER ? "SS_CENTER" " | " : "",
                #endif
                #ifdef SS_CENTERIMAGE
                  createStructure -> style & SS_CENTERIMAGE ? "SS_CENTERIMAGE" " | " : "",
                #endif
                #ifdef SS_EDITCONTROL
                  createStructure -> style & SS_EDITCONTROL ? "SS_EDITCONTROL" " | " : "",
                #endif
                #ifdef SS_ENDELLIPSIS
                  createStructure -> style & SS_ENDELLIPSIS ? "SS_ENDELLIPSIS" " | " : "",
                #endif
                #ifdef SS_ENHMETAFILE
                  createStructure -> style & SS_ENHMETAFILE ? "SS_ENHMETAFILE" " | " : "",
                #endif
                #ifdef SS_ETCHEDFRAME
                  createStructure -> style & SS_ETCHEDFRAME ? "SS_ETCHEDFRAME" " | " : "",
                #endif
                #ifdef SS_ETCHEDHORZ
                  createStructure -> style & SS_ETCHEDHORZ ? "SS_ETCHEDHORZ" " | " : "",
                #endif
                #ifdef SS_ETCHEDVERT
                  createStructure -> style & SS_ETCHEDVERT ? "SS_ETCHEDVERT" " | " : "",
                #endif
                #ifdef SS_GRAYFRAME
                  createStructure -> style & SS_GRAYFRAME ? "SS_GRAYFRAME" " | " : "",
                #endif
                #ifdef SS_GRAYRECT
                  createStructure -> style & SS_GRAYRECT ? "SS_GRAYRECT" " | " : "",
                #endif
                #ifdef SS_ICON
                  createStructure -> style & SS_ICON ? "SS_ICON" " | " : "",
                #endif
                #ifdef SS_LEFT
                  createStructure -> style & SS_LEFT ? "SS_LEFT" " | " : "",
                #endif
                #ifdef SS_LEFTNOWORDWRAP
                  createStructure -> style & SS_LEFTNOWORDWRAP ? "SS_LEFTNOWORDWRAP" " | " : "",
                #endif
                #ifdef SS_NOPREFIX
                  createStructure -> style & SS_NOPREFIX ? "SS_NOPREFIX" " | " : "",
                #endif
                #ifdef SS_NOTIFY
                  createStructure -> style & SS_NOTIFY ? "SS_NOTIFY" " | " : "",
                #endif
                #ifdef SS_OWNERDRAW
                  createStructure -> style & SS_OWNERDRAW ? "SS_OWNERDRAW" " | " : "",
                #endif
                #ifdef SS_PATHELLIPSIS
                  createStructure -> style & SS_PATHELLIPSIS ? "SS_PATHELLIPSIS" " | " : "",
                #endif
                #ifdef SS_REALSIZECONTROL
                  createStructure -> style & SS_REALSIZECONTROL ? "SS_REALSIZECONTROL" " | " : "",
                #endif
                #ifdef SS_REALSIZEIMAGE
                  createStructure -> style & SS_REALSIZEIMAGE ? "SS_REALSIZEIMAGE" " | " : "",
                #endif
                #ifdef SS_RIGHT
                  createStructure -> style & SS_RIGHT ? "SS_RIGHT" " | " : "",
                #endif
                #ifdef SS_RIGHTJUST
                  createStructure -> style & SS_RIGHTJUST ? "SS_RIGHTJUST" " | " : "",
                #endif
                #ifdef SS_SIMPLE
                  createStructure -> style & SS_SIMPLE ? "SS_SIMPLE" " | " : "",
                #endif
                #ifdef SS_SUNKEN
                  createStructure -> style & SS_SUNKEN ? "SS_SUNKEN" " | " : "",
                #endif
                #ifdef SS_TYPEMASK
                  createStructure -> style & SS_TYPEMASK ? "SS_TYPEMASK" " | " : "",
                #endif
                #ifdef SS_WHITEFRAME
                  createStructure -> style & SS_WHITEFRAME ? "SS_WHITEFRAME" " | " : "",
                #endif
                #ifdef SS_WHITERECT
                  createStructure -> style & SS_WHITERECT ? "SS_WHITERECT" " | " : "",
                #endif
                #ifdef SS_WORDELLIPSIS
                  createStructure -> style & SS_WORDELLIPSIS ? "SS_WORDELLIPSIS" " | " : "",
                #endif
                #ifdef TBS_AUTOTICKS
                  createStructure -> style & TBS_AUTOTICKS ? "TBS_AUTOTICKS" " | " : "",
                #endif
                #ifdef TBS_BOTH
                  createStructure -> style & TBS_BOTH ? "TBS_BOTH" " | " : "",
                #endif
                ""
            ))) (void) log::output(log::format(
                #ifdef TBS_BOTTOM
                  createStructure -> style & TBS_BOTTOM ? "TBS_BOTTOM" " | " : "",
                #endif
                #ifdef TBS_DOWNISLEFT
                  createStructure -> style & TBS_DOWNISLEFT ? "TBS_DOWNISLEFT" " | " : "",
                #endif
                #ifdef TBS_ENABLESELRANGE
                  createStructure -> style & TBS_ENABLESELRANGE ? "TBS_ENABLESELRANGE" " | " : "",
                #endif
                #ifdef TBS_FIXEDLENGTH
                  createStructure -> style & TBS_FIXEDLENGTH ? "TBS_FIXEDLENGTH" " | " : "",
                #endif
                #ifdef TBS_HORZ
                  createStructure -> style & TBS_HORZ ? "TBS_HORZ" " | " : "",
                #endif
                #ifdef TBS_LEFT
                  createStructure -> style & TBS_LEFT ? "TBS_LEFT" " | " : "",
                #endif
                #ifdef TBS_NOTHUMB
                  createStructure -> style & TBS_NOTHUMB ? "TBS_NOTHUMB" " | " : "",
                #endif
                #ifdef TBS_NOTICKS
                  createStructure -> style & TBS_NOTICKS ? "TBS_NOTICKS" " | " : "",
                #endif
                #ifdef TBS_NOTIFYBEFOREMOVE
                  createStructure -> style & TBS_NOTIFYBEFOREMOVE ? "TBS_NOTIFYBEFOREMOVE" " | " : "",
                #endif
                #ifdef TBS_REVERSED
                  createStructure -> style & TBS_REVERSED ? "TBS_REVERSED" " | " : "",
                #endif
                #ifdef TBS_RIGHT
                  createStructure -> style & TBS_RIGHT ? "TBS_RIGHT" " | " : "",
                #endif
                #ifdef TBS_TOOLTIPS
                  createStructure -> style & TBS_TOOLTIPS ? "TBS_TOOLTIPS" " | " : "",
                #endif
                #ifdef TBS_TOP
                  createStructure -> style & TBS_TOP ? "TBS_TOP" " | " : "",
                #endif
                #ifdef TBS_TRANSPARENTBKGND
                  createStructure -> style & TBS_TRANSPARENTBKGND ? "TBS_TRANSPARENTBKGND" " | " : "",
                #endif
                #ifdef TBS_VERT
                  createStructure -> style & TBS_VERT ? "TBS_VERT" " | " : "",
                #endif
                #ifdef TBSTYLE_ALTDRAG
                  createStructure -> style & TBSTYLE_ALTDRAG ? "TBSTYLE_ALTDRAG" " | " : "",
                #endif
                #ifdef TBSTYLE_AUTOSIZE
                  createStructure -> style & TBSTYLE_AUTOSIZE ? "TBSTYLE_AUTOSIZE" " | " : "",
                #endif
                #ifdef TBSTYLE_BUTTON
                  createStructure -> style & TBSTYLE_BUTTON ? "TBSTYLE_BUTTON" " | " : "",
                #endif
                #ifdef TBSTYLE_CHECK
                  createStructure -> style & TBSTYLE_CHECK ? "TBSTYLE_CHECK" " | " : "",
                #endif
                #ifdef TBSTYLE_CHECKGROUP
                  createStructure -> style & TBSTYLE_CHECKGROUP ? "TBSTYLE_CHECKGROUP" " | " : "",
                #endif
                #ifdef TBSTYLE_CUSTOMERASE
                  createStructure -> style & TBSTYLE_CUSTOMERASE ? "TBSTYLE_CUSTOMERASE" " | " : "",
                #endif
                #ifdef TBSTYLE_DROPDOWN
                  createStructure -> style & TBSTYLE_DROPDOWN ? "TBSTYLE_DROPDOWN" " | " : "",
                #endif
                #ifdef TBSTYLE_FLAT
                  createStructure -> style & TBSTYLE_FLAT ? "TBSTYLE_FLAT" " | " : "",
                #endif
                #ifdef TBSTYLE_GROUP
                  createStructure -> style & TBSTYLE_GROUP ? "TBSTYLE_GROUP" " | " : "",
                #endif
                #ifdef TBSTYLE_LIST
                  createStructure -> style & TBSTYLE_LIST ? "TBSTYLE_LIST" " | " : "",
                #endif
                #ifdef TBSTYLE_NOPREFIX
                  createStructure -> style & TBSTYLE_NOPREFIX ? "TBSTYLE_NOPREFIX" " | " : "",
                #endif
                #ifdef TBSTYLE_REGISTERDROP
                  createStructure -> style & TBSTYLE_REGISTERDROP ? "TBSTYLE_REGISTERDROP" " | " : "",
                #endif
                #ifdef TBSTYLE_SEP
                  createStructure -> style & TBSTYLE_SEP ? "TBSTYLE_SEP" " | " : "",
                #endif
                #ifdef TBSTYLE_TOOLTIPS
                  createStructure -> style & TBSTYLE_TOOLTIPS ? "TBSTYLE_TOOLTIPS" " | " : "",
                #endif
                #ifdef TBSTYLE_TRANSPARENT
                  createStructure -> style & TBSTYLE_TRANSPARENT ? "TBSTYLE_TRANSPARENT" " | " : "",
                #endif
                #ifdef TBSTYLE_WRAPABLE
                  createStructure -> style & TBSTYLE_WRAPABLE ? "TBSTYLE_WRAPABLE" " | " : "",
                #endif
                #ifdef TCS_BOTTOM
                  createStructure -> style & TCS_BOTTOM ? "TCS_BOTTOM" " | " : "",
                #endif
                #ifdef TCS_BUTTONS
                  createStructure -> style & TCS_BUTTONS ? "TCS_BUTTONS" " | " : "",
                #endif
                #ifdef TCS_FIXEDWIDTH
                  createStructure -> style & TCS_FIXEDWIDTH ? "TCS_FIXEDWIDTH" " | " : "",
                #endif
                #ifdef TCS_FLATBUTTONS
                  createStructure -> style & TCS_FLATBUTTONS ? "TCS_FLATBUTTONS" " | " : "",
                #endif
                #ifdef TCS_FOCUSNEVER
                  createStructure -> style & TCS_FOCUSNEVER ? "TCS_FOCUSNEVER" " | " : "",
                #endif
                #ifdef TCS_FOCUSONBUTTONDOWN
                  createStructure -> style & TCS_FOCUSONBUTTONDOWN ? "TCS_FOCUSONBUTTONDOWN" " | " : "",
                #endif
                #ifdef TCS_FORCEICONLEFT
                  createStructure -> style & TCS_FORCEICONLEFT ? "TCS_FORCEICONLEFT" " | " : "",
                #endif
                #ifdef TCS_FORCELABELLEFT
                  createStructure -> style & TCS_FORCELABELLEFT ? "TCS_FORCELABELLEFT" " | " : "",
                #endif
                #ifdef TCS_HOTTRACK
                  createStructure -> style & TCS_HOTTRACK ? "TCS_HOTTRACK" " | " : "",
                #endif
                #ifdef TCS_MULTILINE
                  createStructure -> style & TCS_MULTILINE ? "TCS_MULTILINE" " | " : "",
                #endif
                #ifdef TCS_MULTISELECT
                  createStructure -> style & TCS_MULTISELECT ? "TCS_MULTISELECT" " | " : "",
                #endif
                #ifdef TCS_OWNERDRAWFIXED
                  createStructure -> style & TCS_OWNERDRAWFIXED ? "TCS_OWNERDRAWFIXED" " | " : "",
                #endif
                #ifdef TCS_RAGGEDRIGHT
                  createStructure -> style & TCS_RAGGEDRIGHT ? "TCS_RAGGEDRIGHT" " | " : "",
                #endif
                #ifdef TCS_RIGHT
                  createStructure -> style & TCS_RIGHT ? "TCS_RIGHT" " | " : "",
                #endif
                #ifdef TCS_RIGHTJUSTIFY
                  createStructure -> style & TCS_RIGHTJUSTIFY ? "TCS_RIGHTJUSTIFY" " | " : "",
                #endif
                #ifdef TCS_SCROLLOPPOSITE
                  createStructure -> style & TCS_SCROLLOPPOSITE ? "TCS_SCROLLOPPOSITE" " | " : "",
                #endif
                #ifdef TCS_SINGLELINE
                  createStructure -> style & TCS_SINGLELINE ? "TCS_SINGLELINE" " | " : "",
                #endif
                #ifdef TCS_TABS
                  createStructure -> style & TCS_TABS ? "TCS_TABS" " | " : "",
                #endif
                #ifdef TCS_TOOLTIPS
                  createStructure -> style & TCS_TOOLTIPS ? "TCS_TOOLTIPS" " | " : "",
                #endif
                #ifdef TCS_VERTICAL
                  createStructure -> style & TCS_VERTICAL ? "TCS_VERTICAL" " | " : "",
                #endif
                #ifdef TTS_ALWAYSTIP
                  createStructure -> style & TTS_ALWAYSTIP ? "TTS_ALWAYSTIP" " | " : "",
                #endif
                #ifdef TTS_BALLOON
                  createStructure -> style & TTS_BALLOON ? "TTS_BALLOON" " | " : "",
                #endif
                #ifdef TTS_CLOSE
                  createStructure -> style & TTS_CLOSE ? "TTS_CLOSE" " | " : "",
                #endif
                #ifdef TTS_NOANIMATE
                  createStructure -> style & TTS_NOANIMATE ? "TTS_NOANIMATE" " | " : "",
                #endif
                #ifdef TTS_NOFADE
                  createStructure -> style & TTS_NOFADE ? "TTS_NOFADE" " | " : "",
                #endif
                #ifdef TTS_NOPREFIX
                  createStructure -> style & TTS_NOPREFIX ? "TTS_NOPREFIX" " | " : "",
                #endif
                #ifdef TTS_USEVISUALSTYLE
                  createStructure -> style & TTS_USEVISUALSTYLE ? "TTS_USEVISUALSTYLE" " | " : "",
                #endif
                #ifdef TVS_CHECKBOXES
                  createStructure -> style & TVS_CHECKBOXES ? "TVS_CHECKBOXES" " | " : "",
                #endif
                #ifdef TVS_DISABLEDRAGDROP
                  createStructure -> style & TVS_DISABLEDRAGDROP ? "TVS_DISABLEDRAGDROP" " | " : "",
                #endif
                #ifdef TVS_EDITLABELS
                  createStructure -> style & TVS_EDITLABELS ? "TVS_EDITLABELS" " | " : "",
                #endif
                #ifdef TVS_FULLROWSELECT
                  createStructure -> style & TVS_FULLROWSELECT ? "TVS_FULLROWSELECT" " | " : "",
                #endif
                #ifdef TVS_HASBUTTONS
                  createStructure -> style & TVS_HASBUTTONS ? "TVS_HASBUTTONS" " | " : "",
                #endif
                #ifdef TVS_HASLINES
                  createStructure -> style & TVS_HASLINES ? "TVS_HASLINES" " | " : "",
                #endif
                #ifdef TVS_INFOTIP
                  createStructure -> style & TVS_INFOTIP ? "TVS_INFOTIP" " | " : "",
                #endif
                #ifdef TVS_LINESATROOT
                  createStructure -> style & TVS_LINESATROOT ? "TVS_LINESATROOT" " | " : "",
                #endif
                #ifdef TVS_NOHSCROLL
                  createStructure -> style & TVS_NOHSCROLL ? "TVS_NOHSCROLL" " | " : "",
                #endif
                #ifdef TVS_NONEVENHEIGHT
                  createStructure -> style & TVS_NONEVENHEIGHT ? "TVS_NONEVENHEIGHT" " | " : "",
                #endif
                #ifdef TVS_NOSCROLL
                  createStructure -> style & TVS_NOSCROLL ? "TVS_NOSCROLL" " | " : "",
                #endif
                #ifdef TVS_NOTOOLTIPS
                  createStructure -> style & TVS_NOTOOLTIPS ? "TVS_NOTOOLTIPS" " | " : "",
                #endif
                #ifdef TVS_RTLREADING
                  createStructure -> style & TVS_RTLREADING ? "TVS_RTLREADING" " | " : "",
                #endif
                #ifdef TVS_SHOWSELALWAYS
                  createStructure -> style & TVS_SHOWSELALWAYS ? "TVS_SHOWSELALWAYS" " | " : "",
                #endif
                #ifdef TVS_SINGLEEXPAND
                  createStructure -> style & TVS_SINGLEEXPAND ? "TVS_SINGLEEXPAND" " | " : "",
                #endif
                #ifdef TVS_TRACKSELECT
                  createStructure -> style & TVS_TRACKSELECT ? "TVS_TRACKSELECT" " | " : "",
                #endif
                #ifdef UDS_ALIGNLEFT
                  createStructure -> style & UDS_ALIGNLEFT ? "UDS_ALIGNLEFT" " | " : "",
                #endif
                #ifdef UDS_ALIGNRIGHT
                  createStructure -> style & UDS_ALIGNRIGHT ? "UDS_ALIGNRIGHT" " | " : "",
                #endif
                #ifdef UDS_ARROWKEYS
                  createStructure -> style & UDS_ARROWKEYS ? "UDS_ARROWKEYS" " | " : "",
                #endif
                #ifdef UDS_AUTOBUDDY
                  createStructure -> style & UDS_AUTOBUDDY ? "UDS_AUTOBUDDY" " | " : "",
                #endif
                #ifdef UDS_HORZ
                  createStructure -> style & UDS_HORZ ? "UDS_HORZ" " | " : "",
                #endif
                #ifdef UDS_HOTTRACK
                  createStructure -> style & UDS_HOTTRACK ? "UDS_HOTTRACK" " | " : "",
                #endif
                #ifdef UDS_NOTHOUSANDS
                  createStructure -> style & UDS_NOTHOUSANDS ? "UDS_NOTHOUSANDS" " | " : "",
                #endif
                #ifdef UDS_SETBUDDYINT
                  createStructure -> style & UDS_SETBUDDYINT ? "UDS_SETBUDDYINT" " | " : "",
                #endif
                #ifdef UDS_WRAP
                  createStructure -> style & UDS_WRAP ? "UDS_WRAP" " | " : "",
                #endif
              createStructureUsesStyle ? "…0x00`" : "",                     "," "\r\n"
              "  " "text         : " "\"", createStructure -> lpszName, "\"" "," "\r\n"
              "  " "width        : ",      (number) createStructure -> cx,   "," "\r\n"
              "  " "x            : ",      (number) createStructure -> x,    "," "\r\n"
              "  " "y            : ",      (number) createStructure -> y,        "\r\n"
              ""   "}]"
            ));
          }

          else if (WM_NCCALCSIZE == message) {
            BOOL               const indicatesClientArea                    = parameter;
            RECT              *const rectangle                              = FALSE == indicatesClientArea ? reinterpret_cast<RECT*>             (subparameter) : NULL; // --> std::launder(…)
            NCCALCSIZE_PARAMS *const sizeCalculation                        = TRUE  == indicatesClientArea ? reinterpret_cast<NCCALCSIZE_PARAMS*>(subparameter) : NULL; // --> std::launder(…)
            bool               const sizeCalculationWindowPositionUsesFlags = NULL != sizeCalculation ? (sizeCalculation -> lppos -> flags & SWP_DRAWFRAME) or (sizeCalculation -> lppos -> flags & SWP_FRAMECHANGED) or (sizeCalculation -> lppos -> flags & SWP_HIDEWINDOW) or (sizeCalculation -> lppos -> flags & SWP_NOACTIVATE) or (sizeCalculation -> lppos -> flags & SWP_NOCOPYBITS) or (sizeCalculation -> lppos -> flags & SWP_NOMOVE) or (sizeCalculation -> lppos -> flags & SWP_NOOWNERZORDER) or (sizeCalculation -> lppos -> flags & SWP_NOREDRAW) or (sizeCalculation -> lppos -> flags & SWP_NOREPOSITION) or (sizeCalculation -> lppos -> flags & SWP_NOSENDCHANGING) or (sizeCalculation -> lppos -> flags & SWP_NOSIZE) or (sizeCalculation -> lppos -> flags & SWP_NOZORDER) or (sizeCalculation -> lppos -> flags & SWP_SHOWWINDOW) : false;

            // ...
            messageTranslationMultilineFormat = true;

            if (log::output(log::format(
              "[" "\r\n",
                "  ", static_cast<undefined>(static_cast<unsigned char>(parameter)), " {indicatesClientArea: ", static_cast<bool>(indicatesClientArea), "}" "," "\r\n"
                "  ", reinterpret_cast<void*>(static_cast<uintptr_t>(subparameter)), " {"                                                                       "\r\n"
            ))) (void) (indicatesClientArea ? log::output(log::format(
                "    "   "windowPosition     : ", sizeCalculation -> lppos, " {" "\r\n"
                "      " "flags      : ", static_cast<undefined>(sizeCalculation -> lppos -> flags), sizeCalculationWindowPositionUsesFlags ? " `" : "",
                  sizeCalculation -> lppos -> flags & SWP_DRAWFRAME      ? "SWP_DRAWFRAME"      " | " : "",
                  sizeCalculation -> lppos -> flags & SWP_FRAMECHANGED   ? "SWP_FRAMECHANGED"   " | " : "",
                  sizeCalculation -> lppos -> flags & SWP_HIDEWINDOW     ? "SWP_HIDEWINDOW"     " | " : "",
                  sizeCalculation -> lppos -> flags & SWP_NOACTIVATE     ? "SWP_NOACTIVATE"     " | " : "",
                  sizeCalculation -> lppos -> flags & SWP_NOCOPYBITS     ? "SWP_NOCOPYBITS"     " | " : "",
                  sizeCalculation -> lppos -> flags & SWP_NOMOVE         ? "SWP_NOMOVE"         " | " : "",
                  sizeCalculation -> lppos -> flags & SWP_NOOWNERZORDER  ? "SWP_NOOWNERZORDER"  " | " : "",
                  sizeCalculation -> lppos -> flags & SWP_NOREDRAW       ? "SWP_NOREDRAW"       " | " : "",
                  sizeCalculation -> lppos -> flags & SWP_NOREPOSITION   ? "SWP_NOREPOSITION"   " | " : "",
                  sizeCalculation -> lppos -> flags & SWP_NOSENDCHANGING ? "SWP_NOSENDCHANGING" " | " : "",
                  sizeCalculation -> lppos -> flags & SWP_NOSIZE         ? "SWP_NOSIZE"         " | " : "",
                  sizeCalculation -> lppos -> flags & SWP_NOZORDER       ? "SWP_NOZORDER"       " | " : "",
                  sizeCalculation -> lppos -> flags & SWP_SHOWWINDOW     ? "SWP_SHOWWINDOW"     " | " : "",
                sizeCalculationWindowPositionUsesFlags ? "…0x00`" : "",    "," "\r\n"
                "      " "height     : ", (number) sizeCalculation -> lppos -> cy, "," "\r\n"
                "      " "width      : ", (number) sizeCalculation -> lppos -> cx, "," "\r\n"
                "      " "window     : ", sizeCalculation -> lppos -> hwnd,        "," "\r\n"
            )) ? log::output(
              HWND_BOTTOM    == sizeCalculation -> lppos -> hwndInsertAfter ? log::format("     " "windowOnTop: `HWND_BOTTOM`"    "," "\r\n") :
              HWND_NOTOPMOST == sizeCalculation -> lppos -> hwndInsertAfter ? log::format("     " "windowOnTop: `HWND_NOTOPMOST`" "," "\r\n") :
              HWND_TOP       == sizeCalculation -> lppos -> hwndInsertAfter ? log::format("     " "windowOnTop: `HWND_TOP`"       "," "\r\n") :
              HWND_TOPMOST   == sizeCalculation -> lppos -> hwndInsertAfter ? log::format("     " "windowOnTop: `HWND_TOPMOST`"   "," "\r\n") :
              log::format("      " "windowOnTop: ", sizeCalculation -> lppos -> hwndInsertAfter, "," "\r\n")
            ) ? log::output(log::format(
              "      " "x          : ", (number) sizeCalculation -> lppos -> x, "," "\r\n"
              "      " "y          : ", (number) sizeCalculation -> lppos -> y, "," "\r\n"
              "    "   "}"                                                      "," "\r\n"
              "    "   "regionRectangles[3]: ["                                     "\r\n"
              "      " "newRegionRectangle    : {bottom: ", (number) sizeCalculation -> rgrc[0].bottom, ", left: ", (number) sizeCalculation -> rgrc[0].left, ", right: ", (number) sizeCalculation -> rgrc[0].right, ", top: ", (number) sizeCalculation -> rgrc[0].top, "}" "," "\r\n"
              "      " "oldRegionRectangle    : {bottom: ", (number) sizeCalculation -> rgrc[1].bottom, ", left: ", (number) sizeCalculation -> rgrc[1].left, ", right: ", (number) sizeCalculation -> rgrc[1].right, ", top: ", (number) sizeCalculation -> rgrc[1].top, "}" "," "\r\n"
              "      " "oldClientAreaRectangle: {bottom: ", (number) sizeCalculation -> rgrc[2].bottom, ", left: ", (number) sizeCalculation -> rgrc[2].left, ", right: ", (number) sizeCalculation -> rgrc[2].right, ", top: ", (number) sizeCalculation -> rgrc[2].top, "}" "," "\r\n"
              "    "   "]"                                                                                                                                                                                                                                                        "\r\n"
              "  "     "}"                                                                                                                                                                                                                                                        "\r\n"
              "]"
            )) : false : false : log::output(log::format(
              "    " "bottom: ", (number) rectangle -> bottom, "," "\r\n"
              "    " "left  : ", (number) rectangle -> left,   "," "\r\n"
              "    " "right : ", (number) rectangle -> right,  "," "\r\n"
              "    " "top   : ", (number) rectangle -> top,    "," "\r\n"
              "  "   "}"                                           "\r\n"
              "]"
            )));
          }

          // else if (WM_NCHITTEST == message) {} // TODO (Lapys)
          // else if (WM_NCLBUTTONDOWN == message) {} // TODO (Lapys)
          // else if (WM_MOUSELEAVE == message) {} // TODO (Lapys)
          // else if (WM_NCMOUSEMOVE == message) {} // TODO (Lapys)
          // else if (WM_NCPAINT == message) {} // TODO (Lapys)
          // else if (WM_PAINT == message) {} // TODO (Lapys)

          else if (WM_QUIT == message) switch (static_cast<int>(parameter)) {
            case EXIT_FAILURE: messageTranslationMultilineFormat = not log::output(log::format("[", static_cast<special>(parameter), " " "`EXIT_FAILURE`" ", ", static_cast<unused>(subparameter), "]")); break;
            case EXIT_SUCCESS: messageTranslationMultilineFormat = not log::output(log::format("[", static_cast<special>(parameter), " " "`EXIT_SUCCESS`" ", ", static_cast<unused>(subparameter), "]")); break;
            default:           messageTranslationMultilineFormat = not log::output(log::format("[", static_cast<undefined>(static_cast<int>(parameter)),  ", ", static_cast<unused>(subparameter), "]")); break;
          }

          // else if (WM_SETCURSOR == message) {} // TODO (Lapys)
          // else if (WM_SETFOCUS == message) {} // TODO (Lapys)
          // else if (WM_SHOWWINDOW == message) {} // TODO (Lapys)
          // else if (WM_SIZE == message) {} // TODO (Lapys)
          // else if (WM_SYSCOMMAND == message) {} // TODO (Lapys)
          // else if (WM_WINDOWPOSCHANGED == message) {} // TODO (Lapys)
          // else if (WM_WINDOWPOSCHANGING == message) {} // TODO (Lapys)

          // TODO (Lapys) -> The other messages
          else
            messageTranslationMultilineFormat = not log::output(log::format('[', static_cast<undefined>(parameter), ", ", static_cast<undefined>(subparameter), ']'));

          (void) log::output(log::format("}" "\r\n", messageTranslationMultilineFormat ? "\n" : ""));
        }

        messageTranslation = messageEventTranslation;
        while ('\0' != *(messageTranslation++)) continue;
      }

      // ... ->> Process the `message`
      switch (message) {
        case WM_CLOSE:
        case WM_DESTROY:
          ::PostQuitMessage(EXIT_SUCCESS);
          break;

        case WM_KEYDOWN:
          if (VK_ESCAPE == parameter)
          ::PostQuitMessage(EXIT_SUCCESS);

          break;

        case WM_QUIT:
          (void) ::DestroyWindow(windowHandle);
          break;
      }

      return ::DefWindowProcW(windowHandle, message, parameter, subparameter);
    }
  } window = {{}, NULL};

  // ...
  if (0x00u == ::GetModuleFileNameW(static_cast<HMODULE>(NULL), moduleFileName, MAX_PATH)) {
    (void) std::fputs("Unable to create window: Failed to retrieve filepath of this process module", stderr);
    return EXIT_FAILURE;
  }

  threadMessage.hwnd    = NULL; // --> window.handle
  threadMessage.lParam  = 0x00L;
  threadMessage.message = 0x00u;
  threadMessage.pt      = POINT();
  threadMessage.pt.x    = 0L;
  threadMessage.pt.y    = 0L;
  threadMessage.time    = 0u;
  threadMessage.wParam  = EXIT_SUCCESS;

  window.classInformation               = WNDCLASSEXW();
  window.classInformation.cbClsExtra    = 0;
  window.classInformation.cbSize        = sizeof(WNDCLASSEX);
  window.classInformation.cbWndExtra    = 0;
  window.classInformation.hbrBackground = ::GetSysColorBrush(COLOR_WINDOW);
  window.classInformation.hCursor       = ::LoadCursor      (static_cast<HINSTANCE>(NULL), IDC_ARROW); // --> static_cast<HCURSOR>(::LoadImage(NULL, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED));
  window.classInformation.hIcon         = ::ExtractIconW    (static_cast<HINSTANCE>(::GetCurrentProcess()), moduleFileName, 0u);
  window.classInformation.hIconSm       = static_cast<HICON>(NULL);
  window.classInformation.hInstance     =  instanceHandle;
  window.classInformation.lpfnWndProc   = &window.procedure;
  window.classInformation.lpszClassName = L"window";
  window.classInformation.lpszMenuName  = static_cast<LPCWSTR>(NULL);
  window.classInformation.style         = CS_HREDRAW | CS_VREDRAW;

  if (0x0000u == ::RegisterClassExW(&window.classInformation)) {
    (void) std::fputs("Unable to create window: Failed to register window class", stderr);
    return EXIT_FAILURE;
  }

  window.handle = ::CreateWindowExW(0x00u, window.classInformation.lpszClassName, L"Windows Event Messages", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 480, 320, static_cast<HWND>(NULL), static_cast<HMENU>(NULL), window.classInformation.hInstance, static_cast<LPVOID>(NULL));

  if (NULL == window.handle) {
    (void) std::fputs("Unable to create window", stderr);
    threadMessage.wParam = EXIT_FAILURE;
  }

  else {
    ::ShowWindow  (window.handle, SW_SHOW | (windowAppearance & SW_SHOW));
    ::UpdateWindow(window.handle);

    while (::GetMessageW(&threadMessage, static_cast<HWND>(NULL), 0x0u, 0x0u) > FALSE) {
      ::TranslateMessage(&threadMessage);
      ::DispatchMessageW(&threadMessage);
    }
  }

  ::UnregisterClassW(window.classInformation.lpszClassName, window.classInformation.hInstance);

  // ...
  return threadMessage.wParam;
}
