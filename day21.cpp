#pragma leco tool
import bruh;
import hai;
import jute;
import scanf;
import silog;

#include <stdio.h>

class fold_t {
  long data[150][150]{};
  long res{-1};

public:
  constexpr auto &operator[](auto idx) noexcept { return data[idx]; }

  constexpr auto result() const { return res; }
  constexpr auto result(int par) {
    if (res >= 0)
      return res;

    res = 0;
    for (auto &row : data) {
      for (auto d : row) {
        d--;
        if (d >= 0 && (d % 2) == par) {
          res++;
        }
      }
    }
    return res;
  }
};

class solver {
  static constexpr const auto fold = 31;
  static constexpr const auto half_fold = fold / 2;
  fold_t dp[fold][fold]{};
  const long max_steps;
  const data_map &map;
  const point s;
  long res{};

public:
  explicit solver(const data_map &m, point s, long ms)
      : max_steps{ms}, map{m}, s{s} {}

  void grub() {
    const point cp{half_fold * map.cols, half_fold * map.rows};
    struct mark {
      point p;
      long steps;
    };
    unsigned qsize = 1024000;
    hai::array<mark> queue{qsize};
    int wr{};
    int rd{};
    queue[wr++] = mark{s + cp, max_steps};

    while (rd != wr) {
      auto [p, steps] = queue[rd];
      rd = (rd + 1) % qsize;
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
        queue[wr] = mark{p + step(c), steps - 1};
        wr = (wr + 1) % qsize;
      }
    }
  }

  auto part0() {
    long res{};
    for (auto a = 0; a <= 3; a++) {
      for (auto b = 2; b <= 3; b++) {
        auto q0 = dp[half_fold - a][half_fold - b].result();
        auto q1 = dp[half_fold - b][half_fold + a].result();
        auto q2 = dp[half_fold + a][half_fold + b].result();
        auto q3 = dp[half_fold + b][half_fold - a].result();
        res += q0 + q1 + q2 + q3;
      }
    }
    return res;
  }
  auto part1() {
    return dp[half_fold][half_fold].result() * 2 +
           dp[half_fold][half_fold - 1].result() * 2;
  }
  auto part2() {
    return dp[half_fold][half_fold].result() +
           dp[half_fold][half_fold - 1].result();
  }
  auto part3() {
    return dp[half_fold + 1][half_fold - 3].result() +
           dp[half_fold + 1][half_fold - 2].result() +
           dp[half_fold - 1][half_fold + 3].result() +
           dp[half_fold - 1][half_fold + 2].result() +
           dp[half_fold - 3][half_fold - 1].result() +
           dp[half_fold - 2][half_fold - 1].result() +
           dp[half_fold + 3][half_fold + 1].result() +
           dp[half_fold + 2][half_fold + 1].result();
  }
  auto centre() { return dp[half_fold][half_fold].result(); }
  auto sides() {
    long res{};
    for (auto i = -1; i <= 1; i++) {
      auto q0 = dp[half_fold - 4][half_fold + i].result();
      auto q1 = dp[half_fold + 4][half_fold + i].result();
      auto q2 = dp[half_fold + i][half_fold - 4].result();
      auto q3 = dp[half_fold + i][half_fold + 4].result();
      res += q0 + q1 + q2 + q3;
    }
    return res;
  }

  auto result() {
    if (res != 0)
      return res;

    point cp{half_fold, half_fold};
    point p{};
    for (p.y = 0; p.y < fold; p.y++) {
      for (p.x = 0; p.x < fold; p.x++) {
        auto r = dp[p.y][p.x].result(max_steps % 1);
        auto c = r == 0 ? 30 : (p == cp ? 36 : 37);
        fprintf(stderr, "\e[%dm%5ld\e[0m ", c, r);
        res += r;
      }
      fprintf(stderr, "\n");
    }
    return res;
  }
};

auto run(const auto &map, point s, long steps) {
  auto slv = hai::uptr<solver>::make(map, s, steps);
  slv->grub();
  silog::log(silog::info, "steps: %ld -- res: %ld -- parts: %ld", steps,
             slv->result(), steps / map.rows);
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
  const long p1s = argc == 1 ? 6 : 64;
  run(map, s, p1s);

  // part 2
  const long p2s = argc == 1 ? 141 : 26501365;
  const auto reps = p2s / map.rows;
  const auto rems = p2s % map.rows;

  if (argc == 1) {
    run(map, s, p2s);
    run(map, s, rems + map.rows * (7 - p2s % 2));
  } else {
    run(map, s, rems + map.rows * 12);
  }

  auto calc = rems + map.rows * 4;
  auto slv = run(map, s, calc);

  long p01 = (reps - 1) / 2;

  info("reps", reps);
  info("rems", rems);
  info("part0", slv->part0());
  info("part1", slv->part1());
  info("part2", slv->part2());
  info("centre", slv->centre());
  info("sides", slv->sides());

  long part0 = slv->part0() * p01;
  long part1 = 4 * slv->part1() * p01 * (p01 - 1) / 2;
  long part2 = 4 * slv->part2() * p01;
  long part3 = slv->part3();

  long done{part0 + part1 + part2 + part3 + slv->centre() + slv->sides()};
  info("done", done);
}
