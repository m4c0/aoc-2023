#pragma leco tool
#include <stdio.h>
import bruh;
import hai;
import jute;
import scanf;
import silog;
import traits;

struct node {
  point p{};
  long sz{};
};
data_map map;
bool vis[150][150]{};
node paths[150][150][4]{};
bool gvis[150][150]{};

// Using 1-based + VIM-like coords
void dump(const char *msg, point a, point b, long sz) {
  silog::log(silog::info, "%s: %ld,%ld -> %ld,%ld = %ld", msg, a.y, a.x, b.y,
             b.x, sz);
}

void dump_map() {
  for (point p{}; p.y < map.rows; p.y++) {
    for (p.x = 0; p.x < map.cols; p.x++) {
      auto pp = p + point{1, 1};
      if (paths[pp.x][pp.y][0].sz > 0) {
        fprintf(stderr, "\e[7m");
      }

      if (map.at(p) == '#') {
        fprintf(stderr, "#");
      } else if (vis[pp.y][pp.x]) {
        fprintf(stderr, ".");
      } else {
        fprintf(stderr, " ");
      }

      fprintf(stderr, "\e[0m");
    }
    fprintf(stderr, "\n");
  }
}

char can_walk(point p) {
  if (p.x < 1 || p.y < 1 || p.x > map.cols || p.y > map.rows)
    return false;
  return map.at(p - point{1, 1}) != '#';
}
void link(point a, point b, long sz) {
  for (auto &n : paths[a.x][a.y]) {
    if (n.p == b) {
      dump("dup", a, b, sz);
      throw 0;
    }
    if (n.sz == 0) {
      n.p = b;
      n.sz = sz;
      dump("link", a, b, sz);
      return;
    }
  }
  dump("fail", a, b, sz);
  throw 0;
}

void trace(point from, point p, long sz) {
  if (!can_walk(p))
    return;
  if (vis[p.y][p.x])
    return;

  if (p.y == map.rows) {
    link(from, p, sz);
    return;
  }

  vis[p.y][p.x] = true;
  long paths{};
  for (auto c : cardinals) {
    if (can_walk(p + step(c)))
      paths++;
  }
  if (paths > 2) {
    link(from, p, sz);
    link(p, from, sz);
    from = p;
    sz = 0;
  }

  for (auto c : cardinals) {
    trace(from, p + step(c), sz + 1);
  }
  // vis[p.y][p.x] = false;
}

long walk(point p, long cost = 0) {
  if (p.y == map.rows)
    return cost;

  auto &gv = gvis[p.x][p.y];
  if (gv)
    return 0;
  gv = true;

  long res{};
  for (auto &n : paths[p.x][p.y]) {
    if (n.sz == 0)
      continue;

    long wtf = walk(n.p, n.sz);
    mx(res, cost + wtf);
    // dump("walk", p, n.p, n.sz);
    // silog::log(silog::info, "wtf: %ld %ld", wtf, res);
  }

  gv = false;
  return res;
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);
  map = dt.map();

  const point s{2, 1};
  trace(s, s, 0);

  info("res", walk(s));

  // dump_map();
}
