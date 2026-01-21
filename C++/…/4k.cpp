#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <inttypes.h>
#include <stdint.h>

/* Main */
int main(int, char*[]) {
  std::size_t const count = 500000u;
  std::FILE  *const file  = std::fopen("4k (output).cpp", "wb+");

  // ...
  if (NULL == file) {
    (void) std::perror("No file, mate");
    return EXIT_FAILURE;
  }

  // ...
  if (std::fwrite("#include <cstddef>\r\n\nint main() {}" "\r\n" "template <typename T, std::size_t _> struct N { static bool (&evaluate())[false + 1]; };", sizeof(char), 124u, file) < 124u) {
    (void) std::perror("Not even started yet...");
    return EXIT_FAILURE;
  }

  std::setbuf(stdout, static_cast<char*>(NULL));

  for (struct {
    std::size_t  count;
    std::clock_t timestamp;
  } progress = {1u, std::clock()}; count != progress.count; ++progress.count) {
    std::clock_t const current = std::clock();

    // ...
    if (std::fprintf(file, "%1.34s%" PRIdMAX "%1.46s", "template <typename T> struct N<T, ", static_cast<uintmax_t>(progress.count), "u> { static bool (&evaluate())[true + 1]; };" "\r\n") < 0) {
      (void) std::perror("Not complete, mate");
      return EXIT_FAILURE;
    }

    // ...
    if (current != static_cast<std::clock_t>(-1)) {
      if (progress.timestamp == static_cast<std::clock_t>(-1))
        progress.timestamp = current;

      else if (current - progress.timestamp >= CLOCKS_PER_SEC / static_cast<std::clock_t>(10u)) {
        progress.timestamp = current;
        (void) std::fprintf(stdout, "%1.10s%" PRIdMAX "%1.4s%" PRIdMAX "%1.2s%.2Lf%1.20s", "Generated ", static_cast<uintmax_t>(progress.count), " of ", static_cast<uintmax_t>(count), " (", (static_cast<long double>(progress.count) / count) * 100.0L, "%) specializations" "\r\n");
      }
    }
  }

  // ...
  (void) std::fwrite("Generated all specializations", sizeof(char), 29u, stdout);
  (void) std::fclose(file);

  return EXIT_SUCCESS;
}
