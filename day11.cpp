#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  hai::varray<point> gals{10240};
  int rs[1024]{};
  int cs[1024]{};
  point p{};
  for (auto row : dt) {
    for (p.x = 0; p.x < row.size(); p.x++) {
      rs[p.y] += row[p.x] == '#' ? 1 : 0;
      cs[p.x] += row[p.x] == '#' ? 1 : 0;
      if (row[p.x] == '#')
        gals.push_back(p);
    }
    p.y++;
  }

  const long incr = 1000000 - 1;
  for (int y = p.y - 1; y > 0; y--) {
    if (rs[y] > 0)
      continue;

    for (auto &g : gals) {
      if (g.y >= y)
        g.y += incr;
    }
  }
  for (int x = p.x - 1; x > 0; x--) {
    if (cs[x] > 0)
      continue;

    for (auto &g : gals) {
      if (g.x >= x) {
        g.x += incr;
      }
    }
  }

  long res{};
  for (auto i = 0; i < gals.size(); i++) {
    const auto &gi = gals[i];
    for (auto j = i + 1; j < gals.size(); j++) {
      const auto &gj = gals[j];
      auto d = abs(gi - gj);
      res += d.x + d.y;
    }
  }
  info("res", res);
}
