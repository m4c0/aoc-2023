#pragma leco tool
import bruh;
import hai;
import jute;
import scanf;
import silog;

void grub(auto &res, const auto &map, point p, int steps) {
  if (!map.inside(p))
    return;

  if (map.at(p) == '#')
    return;

  if (steps == 0) {
    res[map.index(p)] = true;
    return;
  }

  for (auto c : cardinals) {
    grub(res, map, p + step(c), steps - 1);
  }
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

  hai::array<bool> r{(unsigned)dt.raw().size()};

  const auto steps = argc == 1 ? 6 : 64;
  grub(r, map, s, steps);

  int res{};
  for (auto b : r) {
    if (b)
      res++;
  }
  info("res", res);
}
