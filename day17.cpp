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

data_map map;

long astar() {
  const point start{};
  const point goal{map.cols - 1, map.rows - 1};

  const auto sz = static_cast<unsigned>(map.data.size());
  hai::varray<point> open{sz};
  open.push_back(start);

  constexpr const auto invalid = 9999999999L;
  struct node {
    point came_from{};
    cardinal dir_from{X};
    long f_score{invalid};
    long g_score{invalid};
    unsigned strg{};
  };
  hai::array<node> nodes{sz};

  const auto h = [&](auto p) {
    auto d = abs(goal - p);
    // worst case, we walk a manhattan's distance of a path with only "9"s
    return 9 * (d.x + d.y);
  };

  nodes[map.index(start)].g_score = 0;
  nodes[map.index(start)].f_score = h(start);

  while (open.size() > 0) {
    long min_f = invalid;
    point current;
    int min_i = -1;
    node *cur_n;
    for (auto i = 0; i < open.size(); i++) {
      auto &n = nodes[map.index(open[i])];
      if (n.f_score < min_f) {
        min_i = i;
        current = open[i];
        cur_n = &n;
        min_f = n.f_score;
      }
    }
    if (current == goal) {
      hai::array<char> dbg{static_cast<unsigned>(map.data.size() + 1)};
      for (auto i = 0; i < dbg.size(); i++) {
        dbg[i] = map.data[i];
      }
      while (current != start) {
        constexpr const char sym[4]{'^', 'v', '<', '>'};
        auto &n = nodes[map.index(current)];
        dbg[map.index(current)] = sym[n.dir_from];
        current = n.came_from;
      }
      fprintf(stderr, "%s", dbg.begin());
      return cur_n->g_score;
    }

    open[min_i] = open.pop_back();

    const auto next = [&](cardinal c) {
      auto nei = current + step(c);
      if (!map.inside(nei))
        return;
      if (cur_n->came_from == nei)
        return;
      if (cur_n->dir_from == c && cur_n->strg == 3)
        return;

      auto &nein = nodes[map.index(nei)];
      auto d = map.at(nei) - '0';
      auto tgs = cur_n->g_score + d;
      if (tgs < nein.g_score) {
        nein.came_from = current;
        nein.dir_from = c;
        nein.g_score = tgs;
        nein.f_score = tgs + h(nei);
        nein.strg = cur_n->strg + 1;

        for (auto p : open) {
          if (p == nei)
            return;
        }
        open.push_back(nei);
      }
    };

    next(N);
    next(S);
    next(W);
    next(E);
  }

  return invalid;
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);
  map = dt.map();

  info("res", astar());
}
