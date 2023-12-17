#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

#include <stdio.h>

enum cardinal { X = -1, N, S, W, E };
constexpr auto step(cardinal c) {
  constexpr const point vals[4]{{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
  return vals[c];
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);
  data_map map = dt.map();

  const point start{};
  const point goal{map.cols - 1, map.rows - 1};

  const unsigned sz = map.data.size();

  hai::array<char> dbg{sz};
  hai::array<long> dist{sz};
  hai::array<point> prev{sz};
  hai::varray<point> q{sz};

  for (auto y = 0; y < map.rows; y++) {
    for (auto x = 0; x < map.cols; x++) {
      point p{x, y};
      auto i = map.index(p);
      dist[i] = 9999999999;
      prev[i] = {-1, -1};
      dbg[i] = map.at(p);
      q.push_back(p);
    }
  }
  dist[map.index(start)] = 0;
  prev[map.index(start)] = {};

  while (q.size() > 0) {
    const auto min_dist_u = [&] {
      long min{999999999};
      int min_i;
      for (auto i = 0; i < q.size(); i++) {
        auto d = dist[map.index(q[i])];
        if (min < d)
          continue;

        min = d;
        min_i = i;
      }
      point u = q[min_i];
      q[min_i] = q.pop_back();
      return u;
    };
    const auto qhas = [&](point v) {
      for (auto qq : q) {
        if (qq == v)
          return true;
      }
      return false;
    };
    const auto next = [&](point u, cardinal c, int n) {
      auto v = u + step(c) * n;
      if (!map.inside(v))
        return;
      if (!qhas(v))
        return;

      auto alt = dist[map.index(u)];
      for (auto nn = 1; nn <= n; nn++) {
        alt += map.at(u + step(c) * nn) - '0';
      }
      if (alt < dist[map.index(v)]) {
        dist[map.index(v)] = alt;
        prev[map.index(v)] = u;
      }
    };
    const auto nexts = [&](point u, cardinal c) {
      for (auto i = 1; i <= 3; i++) {
        next(u, c, i);
      }
    };

    auto u = min_dist_u();
    auto d = u - prev[map.index(u)];
    if (d.x == 0) {
      nexts(u, E);
      nexts(u, W);
    }
    if (d.y == 0) {
      nexts(u, S);
      nexts(u, N);
    }
  }

  long res{};
  auto u = goal;
  while (u != start) {
    auto pu = prev[map.index(u)];
    while (u != pu) {
      res += map.at(u) - '0';
      dbg[map.index(u)] = '.';
      auto d = pu - u;
      if (d.x > 0)
        u = u + point{1, 0};
      if (d.x < 0)
        u = u - point{1, 0};
      if (d.y > 0)
        u = u + point{0, 1};
      if (d.y < 0)
        u = u - point{0, 1};
    }
  }
  for (auto i = 0; i < map.rows; i++) {
    silog::log(silog::debug, "%s", dbg.begin() + i * map.stride);
  }
  info("res", res);
}
