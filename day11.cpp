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

  for (int y = p.y - 1; y > 0; y--) {
    if (rs[y] > 0)
      continue;

    for (auto &g : gals) {
      if (g.y >= y)
        g.y++;
    }
  }
  for (int x = p.x - 1; x > 0; x--) {
    if (cs[x] > 0)
      continue;

    for (auto &g : gals) {
      if (g.x >= x)
        g.x++;
    }
  }

  for (auto g : gals) {
    silog::log(silog::debug, "%d %d", g.x, g.y);
  }
}
