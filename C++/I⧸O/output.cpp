/* Import */
#include <cstdio> // C Standard Input-Output --- NOTE (Lapys) -> For the `std::fprintf`, `std::fputc`, `std::fputs`, `std::fwrite`, `std::printf`, `std::putc`, `std::putchar` & `std::puts` methods.
#include <cstdlib> // C Standard Library --- NOTE (Lapys) -> For the `STDERR_FILENO` and `STDOUT_FILENO` macros.
#include <iomanip> // Input-Output Manipulation --- NOTE (Lapys) -> For the `std::setprecision` method.
#include <iostream> // Input-Output Stream --- NOTE (Lapys) -> For the `std::cerr`, `std::clog`, `std::cout`, `std::wcerr`, `std::wclog` & `std::wcout` objects.
#include <limits> // Limits --- NOTE (Lapys) -> For the `std::numeric_limits<...>digits10` data.
#include <unistd.h> // Unix Standard --- NOTE (Lapys) -> For the `::write` function.

/* Main */
int main(int, char*[]) /* noexcept */ {
  std::setprecision(std::numeric_limits<long double>::digits10);

  putw('[', stdout); putw('1', stdout); putw(']', stdout); putw(':', stdout); putw(' ', stdout); putw('H', stdout); putw('e', stdout); putw('l', stdout); putw('l', stdout); putw('o', stdout); putw(',', stdout); putw(' ', stdout); putw('W', stdout); putw('o', stdout); putw('r', stdout); putw('l', stdout); putw('d', stdout); putw('!', stdout); putw('\n', stdout); // --- NOTE (Lapys) -> Pretty trite for entry-level C/ C++ developers.
  ::write(STDOUT_FILENO, "[2]: Hello, World!\n", sizeof "[2]: Hello, World!\n" - 1); // NOTE (Lapys) -> `-1` to exclude the terminating null character (usually `\0`).
  std::fprintf(stderr, "%s", "[3]: Hello, World!\n");
  std::fprintf(stdout, "%s", "[4]: Hello, World!\n");
  std::fputc('[', stderr); std::fputc('5', stderr); std::fputc(']', stderr); std::fputc(':', stderr); std::fputc(' ', stderr); std::fputc('H', stderr); std::fputc('e', stderr); std::fputc('l', stderr); std::fputc('l', stderr); std::fputc('o', stderr); std::fputc(',', stderr); std::fputc(' ', stderr); std::fputc('W', stderr); std::fputc('o', stderr); std::fputc('r', stderr); std::fputc('l', stderr); std::fputc('d', stderr); std::fputc('!', stderr); std::fputc('\n', stderr);
  std::fputc('[', stdout); std::fputc('6', stdout); std::fputc(']', stdout); std::fputc(':', stdout); std::fputc(' ', stdout); std::fputc('H', stdout); std::fputc('e', stdout); std::fputc('l', stdout); std::fputc('l', stdout); std::fputc('o', stdout); std::fputc(',', stdout); std::fputc(' ', stdout); std::fputc('W', stdout); std::fputc('o', stdout); std::fputc('r', stdout); std::fputc('l', stdout); std::fputc('d', stdout); std::fputc('!', stdout); std::fputc('\n', stdout);
  std::fputs("[7]: Hello, World!\n", stderr);
  std::fputs("[8]: Hello, World!\n", stdout);
  std::fwrite("[9]: Hello, World!\n", STDERR_FILENO, sizeof "[9]: Hello, World!\n" - 1, stderr);
  std::fwrite("[10]: Hello, World!\n", STDOUT_FILENO, sizeof "[10]: Hello, World!\n" - 1, stdout);
  std::printf("%s", "[11]: Hello, World!\n");
  std::putc('[', stderr); std::putc('1', stderr); std::putc('2', stderr); std::putc(']', stderr); std::putc(':', stderr); std::putc(' ', stderr); std::putc('H', stderr); std::putc('e', stderr); std::putc('l', stderr); std::putc('l', stderr); std::putc('o', stderr); std::putc(',', stderr); std::putc(' ', stderr); std::putc('W', stderr); std::putc('o', stderr); std::putc('r', stderr); std::putc('l', stderr); std::putc('d', stderr); std::putc('!', stderr); std::putc('\n', stderr);
  std::putc('[', stdout); std::putc('1', stdout); std::putc('3', stdout); std::putc(']', stdout); std::putc(':', stdout); std::putc(' ', stdout); std::putc('H', stdout); std::putc('e', stdout); std::putc('l', stdout); std::putc('l', stdout); std::putc('o', stdout); std::putc(',', stdout); std::putc(' ', stdout); std::putc('W', stdout); std::putc('o', stdout); std::putc('r', stdout); std::putc('l', stdout); std::putc('d', stdout); std::putc('!', stdout); std::putc('\n', stdout);
  std::putchar('['); std::putchar('1'); std::putchar('4'); std::putchar(']'); std::putchar(':'); std::putchar(' '); std::putchar('H'); std::putchar('e'); std::putchar('l'); std::putchar('l'); std::putchar('o'); std::putchar(','); std::putchar(' '); std::putchar('W'); std::putchar('o'); std::putchar('r'); std::putchar('l'); std::putchar('d'); std::putchar('!'); std::putchar('\n');
  std::puts("[15]: Hello, World!");
  std::cerr << "[16]: Hello, World!" << std::endl;
  std::cerr.put('['); std::cerr.put('1'); std::cerr.put('8'); std::cerr.put(']'); std::cerr.put(':'); std::cerr.put(' '); std::cerr.put('H'); std::cerr.put('e'); std::cerr.put('l'); std::cerr.put('l'); std::cerr.put('o'); std::cerr.put(','); std::cerr.put(' '); std::cerr.put('W'); std::cerr.put('o'); std::cerr.put('r'); std::cerr.put('l'); std::cerr.put('d'); std::cerr.put('!'); std::cerr.put('\n');
  std::cout.write("[18]: Hello, World!\n", sizeof "[18]: Hello, World!\n" - 1);
  std::clog << "[19]: Hello, World!" << std::endl;
  std::clog.put('['); std::clog.put('2'); std::clog.put('0'); std::clog.put(']'); std::clog.put(':'); std::clog.put(' '); std::clog.put('H'); std::clog.put('e'); std::clog.put('l'); std::clog.put('l'); std::clog.put('o'); std::clog.put(','); std::clog.put(' '); std::clog.put('W'); std::clog.put('o'); std::clog.put('r'); std::clog.put('l'); std::clog.put('d'); std::clog.put('!'); std::clog.put('\n');
  std::clog.write("[21]: Hello, World!\n", sizeof "[21]: Hello, World!\n" - 1);
  std::cout << "[22]: Hello, World!" << std::endl;
  std::cout.put('['); std::cout.put('2'); std::cout.put('3'); std::cout.put(']'); std::cout.put(':'); std::cout.put(' '); std::cout.put('H'); std::cout.put('e'); std::cout.put('l'); std::cout.put('l'); std::cout.put('o'); std::cout.put(','); std::cout.put(' '); std::cout.put('W'); std::cout.put('o'); std::cout.put('r'); std::cout.put('l'); std::cout.put('d'); std::cout.put('!'); std::cout.put('\n');
  std::cout.write("[24]: Hello, World!\n", sizeof "[24]: Hello, World!\n" - 1);
  std::wcerr << "[25]: Hello, World!" << std::endl;
  std::wcerr.put('['); std::wcerr.put('2'); std::wcerr.put('6'); std::wcerr.put(']'); std::wcerr.put(':'); std::wcerr.put(' '); std::wcerr.put('H'); std::wcerr.put('e'); std::wcerr.put('l'); std::wcerr.put('l'); std::wcerr.put('o'); std::wcerr.put(','); std::wcerr.put(' '); std::wcerr.put('W'); std::wcerr.put('o'); std::wcerr.put('r'); std::wcerr.put('l'); std::wcerr.put('d'); std::wcerr.put('!'); std::wcerr.put('\n');
  std::cout.write("[27]: Hello, World!\n", sizeof "[27]: Hello, World!\n" - 1);
  std::wclog << "[28]: Hello, World!" << std::endl;
  std::wclog.put('['); std::wclog.put('2'); std::wclog.put('9'); std::wclog.put(']'); std::wclog.put(':'); std::wclog.put(' '); std::wclog.put('H'); std::wclog.put('e'); std::wclog.put('l'); std::wclog.put('l'); std::wclog.put('o'); std::wclog.put(','); std::wclog.put(' '); std::wclog.put('W'); std::wclog.put('o'); std::wclog.put('r'); std::wclog.put('l'); std::wclog.put('d'); std::wclog.put('!'); std::wclog.put('\n');
  std::wcout << "[30]: Hello, World!" << std::endl;
  std::wcout.put('['); std::wcout.put('3'); std::wcout.put('1'); std::wcout.put(']'); std::wcout.put(':'); std::wcout.put(' '); std::wcout.put('H'); std::wcout.put('e'); std::wcout.put('l'); std::wcout.put('l'); std::wcout.put('o'); std::wcout.put(','); std::wcout.put(' '); std::wcout.put('W'); std::wcout.put('o'); std::wcout.put('r'); std::wcout.put('l'); std::wcout.put('d'); std::wcout.put('!'); std::wcout.put('\n');

  return EXIT_SUCCESS;
}
