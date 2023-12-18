#pragma leco tool
import bruh;
import hai;
import jute;
import silog;
#include <stdio.h>

constexpr cardinal chr2c(char c) {
  switch (c) {
  case 'R':
    return E;
  case 'L':
    return W;
  case 'D':
    return S;
  case 'U':
    return N;
  default:
    throw 0;
  }
}

constexpr const auto sz = 1000;
class map_t {
  char m_data[sz][sz]{};

  constexpr auto norm(int n) { return (n + sz / 2) % sz; }

public:
  char &at(int x, int y) { return m_data[norm(y)][norm(x)]; }
  const auto &data() const { return m_data; }
};

void fill(map_t &map, int x, int y) {
  struct st {
    int x1, x2;
    int y, dy;
  };
  hai::varray<st> s{sz * sz};

  const auto inside = [&](auto x, auto y) {
    if (x < -sz / 2 || x >= sz / 2 || y < -sz / 2 || y >= sz / 2) {
      return false;
    }
    return map.at(x, y) == 0;
  };
  const auto set = [&](auto x, auto y) { map.at(x, y) = '#'; };
  const auto add = [&](auto x1, auto x2, auto y, auto dy) {
    s.push_back(st{x1, x2, y, dy});
  };

  if (!inside(x, y))
    return;

  add(x, x, y, 1);
  add(x, x, y - 1, -1);
  while (s.size() > 0) {
    auto [x1, x2, y, dy] = s.pop_back();
    int x = x1;
    if (inside(x, y)) {
      while (inside(x - 1, y)) {
        set(x - 1, y);
        x--;
      }
      if (x < x1) {
        add(x, x1 - 1, y - dy, -dy);
      }
    }
    while (x1 <= x2) {
      while (inside(x1, y)) {
        set(x1, y);
        x1++;
      }
      if (x1 > x) {
        add(x, x1 - 1, y + dy, dy);
      }
      if (x1 - 1 > x2) {
        add(x2 + 1, x1 - 1, y - dy, -dy);
      }
      x1++;
      while (x1 < x2 && !inside(x1, y))
        x1++;
      x = x1;
    }
  }
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  map_t map{};
  point p{};

  point min{9999999, 9999999};
  point max{};
  for (auto line : dt) {
    auto [l, lr] = line.split(' ');
    auto [ns, cl] = lr.split(' ');

    auto c = chr2c(l[0]);
    auto n = atoi(ns);
    for (auto i = 0; i < n; i++) {
      p = p + step(c);
      mx(max.x, p.x);
      mx(max.y, p.y);
      mn(min.x, p.x);
      mn(min.y, p.y);
      map.at(p.x, p.y) = '#';
    }
  }
  info("min", min);
  info("max", max);
  info("delta", max - min);

  fill(map, 1, 1);

  long dug{};
  const auto pad = 10;
  for (auto y = min.y - pad; y <= max.y + pad; y++) {
    for (auto x = min.x - pad; x <= max.x + pad; x++) {
      auto c = map.at(x, y);
      fprintf(stderr, "%c", c ? c : ' ');
      if (c == '#')
        dug++;
    }
    fprintf(stderr, "\n");
  }
  info("dug", dug);
}
