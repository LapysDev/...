#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

/* Main */
int main(void) {
  if (FILE *const file = std::fopen("file.txt", "rb" /* binary read mode */))
  while (true) /* --> not NULL */ {
    std::size_t count;
    char        text[64];

    // read 64 characters at a time
    count = std::fread(text, sizeof(char), 64u - /* NUL character */ 1u, file);

    // check for errors
    if (0 != std::ferror(file)) {
      std::fputs("[ERROR]: Could not read file", stderr);
      std::exit(EXIT_FAILURE);
    }

    // parse the read characters
    for (char *character = text + (sizeof(text) / sizeof(char)); character-- != text; )
    if (std::isupper(*character)) *character = std::tolower(*character);

    // print the read characters
    std::printf("%.*s", static_cast<int>(count), text);

    // stop reading characters
    if (0 != std::feof(file)) {
      std::fclose(file);
      break;
    }
  } else std::fputs("[ERROR]: Could not open file for reading" "\r\n", stderr);
}
