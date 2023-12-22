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

class fold_t {
  int data[150][150]{};

public:
  constexpr auto &operator[](auto idx) noexcept { return data[idx]; }

  constexpr auto result() const {
    int r{};
    for (auto &row : data) {
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

class solver {
  static constexpr const auto fold = 20;
  static constexpr const auto half_fold = fold / 2;
  fold_t dp[fold][fold]{};
  const data_map &map;
  const point s;

public:
  explicit solver(const data_map &m, point s) : map{m}, s{s} {}

  void grub(int max_steps) {
    const point cp{half_fold * map.cols, half_fold * map.rows};
    struct mark {
      point p;
      int steps;
    };
    hai::varray<mark> queue{1024000};
    queue.push_back(mark{s + cp, max_steps});

    for (auto i = 0; i < queue.size(); i++) {
      auto [p, steps] = queue[i];
      point fp{p.x / map.cols, p.y / map.rows};
      if (fp.x < 0 || fp.y < 0 || fp.x >= fold || fp.y >= fold)
        continue;

      point mp{p.x % map.cols, p.y % map.rows};
      auto &dpp = dp[fp.y][fp.x][mp.y][mp.x];
      if (dpp >= steps + 1) {
        continue;
      }
      if (map.at(mp) == '#')
        continue;

      mx(dpp, steps + 1);

      if (steps == 0) {
        continue;
      }

      for (auto c : cardinals) {
        queue.push_back(mark{p + step(c), steps - 1});
      }
    }
  }

  auto result() const {
    point cp{half_fold, half_fold};
    long res{};
    point p{};
    for (p.y = 0; p.y < fold; p.y++) {
      for (p.x = 0; p.x < fold; p.x++) {
        auto r = dp[p.y][p.x].result();
        auto c = p == cp ? 36 : 37;
        fprintf(stderr, "\e[%dm%5d\e[0m ", c, r);
        res += r;
      }
      fprintf(stderr, "\n");
    }
    return res;
  }
};

auto run(const auto &map, point s, int steps) {
  auto slv = hai::uptr<solver>::make(map, s);
  slv->grub(steps);
  info("res", slv->result());
  return slv;
}

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

  // part 1
  const auto p1s = argc == 1 ? 6 : 64;
  run(map, s, p1s);

  // part 2
  const auto p2s = argc == 1 ? 5000 : 26501365;
  const auto par = p2s % 2;
  run(map, s, p2s);
  auto slv = run(map, s, (p2s % map.rows) + map.rows * (4 + par));

  info("done", 0);
}
