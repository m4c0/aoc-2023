#pragma leco tool
#include <stdio.h>
import bruh;
import hai;
import jute;
import scanf;
import silog;
import traits;

data_map map;
bool vis[300][300]{};

long enter(point p, long sz) {
  if (p.x < 0 || p.y < 0)
    return 0;
  if (vis[p.y][p.x])
    return 0;
  if (map.at(p) == '#')
    return 0;
  if (p.y == map.rows - 1)
    return sz;

  long res{};

  vis[p.y][p.x] = true;
  for (auto c : cardinals) {
    auto np = p + step(c);
    mx(res, enter(np, sz + 1));
    // constexpr const char vs[]{'^', 'v', '<', '>'};
    // auto np = p + step(c);
    // auto mp = map.at(p);
    // if (mp == '.' || mp == vs[c]) {
    //   mx(res, enter(np, sz + 1));
    // }
  }
  vis[p.y][p.x] = false;
  return res;
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);
  map = dt.map();

  long res{};
  res = enter(point{1, 0}, 0);
  info("res", res);
}
