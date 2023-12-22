#pragma leco tool
import bruh;
import hai;
import jute;
import scanf;
import silog;

#include <stdio.h>

void print(int v) {
  if (v == 0) {
    fprintf(stderr, ".");
  } else if (v % 2 == 0) {
    fprintf(stderr, "O");
  } else {
    fprintf(stderr, "E");
  }
}

class solver {
  int dp[150][150]{};
  const data_map &map;

public:
  explicit solver(const data_map &m) : map{m} {}

  void grub(point s, int max_steps) {
    struct mark {
      point p;
      int steps;
    };
    hai::varray<mark> queue{1024000};
    queue.push_back(mark{s, max_steps});

    for (auto i = 0; i < queue.size(); i++) {
      auto [p, steps] = queue[i];

      if (!map.inside(p))
        continue;

      if (dp[p.y][p.x] >= steps + 1) {
        continue;
      }
      if (map.at(p) == '#')
        continue;

      mx(dp[p.y][p.x], steps + 1);

      if (steps == 0) {
        continue;
      }

      for (auto c : cardinals) {
        queue.push_back(mark{p + step(c), steps - 1});
      }
    }
  }

  auto result() const {
    int r{};
    for (auto y = 0; y < map.rows; y++) {
      for (auto x = 0; x < map.cols; x++) {
        auto d = dp[y][x] - 1;
        print(dp[y][x]);
        if (d >= 0 && (d % 2) == 0) {
          r++;
        }
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
