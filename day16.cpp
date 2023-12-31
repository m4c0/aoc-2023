#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

using dir = cardinal;

constexpr point deltas[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
constexpr char passthru[4] = {'|', '|', '-', '-'};

jute::view map;
long cols;
long rows;
hai::array<int> visited;

auto &vis(point p) { return visited[p.y * (cols + 1) + p.x]; }
auto mp(point p) { return map[p.y * (cols + 1) + p.x]; }

// TODO: detect cycles?
void move(point p, dir d) {
  p = p + deltas[d];
  if (p.x < 0 || p.y < 0 || p.x >= cols || p.y >= rows) {
    // silog::log(silog::debug, "done");
    return;
  }

  // silog::log(silog::debug, "%ld %ld -> %d", p.x, p.y, d);

  if (vis(p) & (1 << d))
    return;

  vis(p) |= (1 << d);

  auto c = mp(p);
  if (c == '.' || c == passthru[d]) {
  } else if (c == passthru[d ^ 2]) {
    d = static_cast<dir>(d ^ 2);
    move(p, static_cast<dir>(d ^ 1));
  } else if (c == '/') {
    d = static_cast<dir>(d ^ 3);
  } else if (c == '\\') {
    d = static_cast<dir>(d ^ 2);
  }

  move(p, d);
}

long run(point p, dir d) {
  for (auto &v : visited)
    v = {};

  move(p, d);

  long res{};
  for (auto v : visited)
    if (v)
      res++;
  return res;
}

#include <stdio.h>
int main(int argc, char **argv) {
  auto dt = data::of(argc);

  map = dt.raw();
  cols = map.index_of('\n');
  rows = map.size() / (cols + 1);

  visited.set_capacity(map.size());

  long res{};
  for (auto i = 0; i < rows; i++) {
    mx(res, run({-1, i}, E));
    mx(res, run({cols, i}, W));
  }
  for (auto i = 0; i < cols; i++) {
    mx(res, run({i, -1}, S));
    mx(res, run({i, rows}, N));
  }
  info("res", res);
  return 0;

  for (auto y = 0; y < rows; y++) {
    for (auto x = 0; x < cols; x++) {
      point p{x, y};
      if (vis(p))
        fprintf(stderr, "\e[7m");
      fprintf(stderr, "%c", mp(p));
      if (vis(p))
        fprintf(stderr, "\e[0m");
    }
    fprintf(stderr, "\n");
  }
}
