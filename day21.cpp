#pragma leco tool
import bruh;
import hai;
import jute;
import scanf;
import silog;

#include <stdio.h>

enum par_t { NONE, EVEN, ODD };

void print(par_t v) {
  switch (v) {
  case NONE:
    fprintf(stderr, ".");
    break;
  case EVEN:
    fprintf(stderr, "E");
    break;
  case ODD:
    fprintf(stderr, "O");
    break;
  }
}

class solver {
  int dpe[150][150]{};
  int dp[150][150][64]{};
  par_t parity[150][150]{};
  const data_map &map;

public:
  explicit solver(const data_map &m) : map{m} {}

  void grub(point p, int steps) {
    // if (dpe[p.y][p.x] >= steps) {
    if (dp[p.y][p.x][steps] != 0) {
      return;
    }
    if (!map.inside(p))
      return;

    if (map.at(p) == '#')
      return;

    parity[p.y][p.x] = steps % 2 == 0 ? EVEN : ODD;

    if (steps == 0) {
      return;
    }

    mx(dpe[p.y][p.x], steps);
    dp[p.y][p.x][steps] = 1;
    for (auto c : cardinals) {
      grub(p + step(c), steps - 1);
    }
  }

  auto result() const {
    int r{};
    for (auto y = 0; y < map.rows; y++) {
      for (auto x = 0; x < map.cols; x++) {
        auto b = parity[y][x];
        print(b);
        if (b == EVEN)
          r++;
      }
      fprintf(stderr, "\n");
    }
    return r;
  }
};

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  auto map = dt.map();

  point s{};
  for (point p{}; p.y < map.rows; p.y++) {
    for (p.x = 0; p.x < map.cols; p.x++) {
      if (map.at(p) == 'S')
        s = p;
    }
  }

  const auto steps = argc == 1 ? 6 : 64;
  solver slv{map};
  slv.grub(s, steps);

  info("res", slv.result());
}
