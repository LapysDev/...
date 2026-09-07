#include <cmath>
#include <cstdio>

/* … → Circular interpolation */
struct coordinate_t {
  double _[2];
};

coordinate_t clerp(double percent, double (&&x)[2], double (&&y)[2], double (&&midpoint)[2], double (&&size)[2]) {
  double angle[] = {
    std::atan2((y[0] - midpoint[1]), (x[0] - midpoint[0])),
    std::atan2((y[1] - midpoint[1]), (x[1] - midpoint[0]))
  }, interpolation = angle[0] + (percent * (angle[1] - angle[0]));

  // …
  return {
    midpoint[0] + (size[0] * std::cos(interpolation)),
    midpoint[1] + (size[1] * std::sin(interpolation))
  };
}

/* Main */
int main(int, char*[]) /* noexcept */ {
  std::size_t const width                = 40u;
  std::size_t const height               = 20u;
  char              board[height][width] = {};

  // …
  for (char (*row)[width] = board; row  != board + (sizeof board / sizeof(char[width])); ++row)
  for (char *cell         = *row;  cell != *row  + (sizeof *row  / sizeof(char));        ++cell) {
    *cell = '_';
  }

  for (double percent = 0.0, precision = 0.01; percent <= 1.0; percent += precision) {
    coordinate_t interpolation = clerp(percent, {0.0, 1.0}, {0.0, 1.0}, {0.5, 0.5}, {0.25, 0.5});
    std::size_t  point[]       = {static_cast<std::size_t>(width * interpolation._[0]), static_cast<std::size_t>(height * interpolation._[1])}; // → -Wnarrowing

    // …
    board         [point[1]]             [point[0]]      = 'A';
    board[height - point[1] - 1u][width - point[0] - 1u] = 'B';

    if (point[0] == width - point[0] - 1u)
    if (point[1] == width - point[1] - 1u)
      board[point[1]][point[0]] = 'C';
  }

  for (char (*row)[width] = board; row != board + (sizeof board / sizeof(char[width])); ++row)
  std::printf("%1.*s" "\r\n", static_cast<int>(width), *row);
}
