#pragma leco tool
import bruh;
import hai;
import jute;
import scanf;
import silog;

class solver {
  bool res[150][150]{};
  int dp[150][150][64]{};
  const data_map &map;

public:
  explicit solver(const data_map &m) : map{m} {}

  void grub(point p, int steps) {
    if (dp[p.y][p.x][steps] > 0) {
      return;
    }
    if (!map.inside(p))
      return;

    if (map.at(p) == '#')
      return;

    if (steps == 0) {
      res[p.y][p.x] = true;
      return;
    }

    dp[p.y][p.x][steps] = 1;
    for (auto c : cardinals) {
      grub(p + step(c), steps - 1);
    }
  }

  auto result() const {
    int r{};
    for (auto &row : res) {
      for (auto b : row) {
        if (b)
          r++;
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
  solver slv{map};
  slv.grub(s, steps);

  info("res", slv.result());
}
