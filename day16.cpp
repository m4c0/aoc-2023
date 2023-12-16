#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

enum dir { N, S, W, E };

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
repeat:
  // silog::log(silog::debug, "%ld %ld -> %d", p.x, p.y, d);

  if (vis(p) & (1 << d))
    return;

  vis(p) |= (1 << d);

  p = p + deltas[d];
  if (p.x < 0 || p.y < 0 || p.x >= cols || p.y >= rows) {
    // silog::log(silog::debug, "done");
    return;
  }

  auto c = mp(p);

  if (c == '.' || c == passthru[d])
    goto repeat;

  if (c == passthru[d ^ 2]) {
    d = static_cast<dir>(d ^ 2);
    move(p, static_cast<dir>(d ^ 1));
    goto repeat;
  }
  if (c == '/') {
    d = static_cast<dir>(d ^ 3);
    goto repeat;
  }
  if (c == '\\') {
    d = static_cast<dir>(d ^ 2);
    goto repeat;
  }
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  map = dt.raw();
  cols = map.index_of('\n');
  rows = map.size() / (cols + 1);

  visited.set_capacity(map.size());

  move({0, 0}, E);

  long res{};
  for (auto v : visited)
    if (v)
      res++;
  info("res", res);
}
