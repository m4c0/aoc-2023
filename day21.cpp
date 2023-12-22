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
  static constexpr const auto fold = 10;
  static constexpr const auto half_fold = fold / 2;
  int dp[fold * 150][fold * 150]{};
  const data_map &map;

public:
  explicit solver(const data_map &m) : map{m} {}

  void grub(point s, int max_steps) {
    const point cp{half_fold * map.cols, half_fold * map.rows};
    struct mark {
      point p;
      int steps;
    };
    hai::varray<mark> queue{1024000};
    queue.push_back(mark{s + cp, max_steps});

    for (auto i = 0; i < queue.size(); i++) {
      auto [p, steps] = queue[i];

      point mp{p.x % map.cols, p.y % map.rows};
      if (dp[p.y][p.x] >= steps + 1) {
        continue;
      }
      if (map.at(mp) == '#')
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
    for (auto y = 0; y < map.rows * fold; y++) {
      if (y % map.rows == 0)
        fprintf(stderr, "\n");
      for (auto x = 0; x < map.cols * fold; x++) {
        if (x % map.cols == 0)
          fprintf(stderr, " ");
        print(dp[y][x]);
      }
      fprintf(stderr, "\n");
    }

    int r{};
    for (auto &row : dp) {
      for (auto d : row) {
        d--;
        if (d >= 0 && (d % 2) == 0) {
          r++;
        }
      }
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

  auto slv = hai::uptr<solver>::make(map);
  slv->grub(s, steps);
  info("res", slv->result());

  {
    auto slv = hai::uptr<solver>::make(map);
    slv->grub(s, 44);
    info("res", slv->result());
  }
}
