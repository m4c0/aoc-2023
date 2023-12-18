#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

#include <stdio.h>

struct walk {
  point p{-1, -1};
  cardinal c{X};
  int s{-1};
};
constexpr bool operator==(const walk &a, const walk &b) noexcept {
  return a.p == b.p && a.c == b.c && a.s == b.s;
}
constexpr int hash(const walk &w) noexcept {
  return (w.p.x * 23) ^ (w.p.y * 17) ^ (w.c * 5) ^ w.s;
}

constexpr const auto invalid = 9999999999L;
struct node {
  walk data{};
  node *came_from{};
  long f_score{invalid};
  long g_score{invalid};
};

struct bucket {
  walk key;
  node val;
  bucket *next{};
  bucket *prev{};
};
struct blist {
  bucket *head{};
  bucket *tail{};
};
class hashmap {
  blist map[256 * 16]{};

public:
  node &operator[](walk k) {
    auto h = hash(k) % (256 * 16);
    auto &bl = map[h];
    auto p = bl.head;
    while (p) {
      if (p->key == k) {
        break;
      }
      p = p->next;
    }
    if (p) {
      return p->val;
    } else {
      auto n = new bucket{k, {}, nullptr, bl.tail};
      if (bl.tail) {
        bl.tail->next = n;
      } else {
        bl.head = n;
      }
      bl.tail = n;
      n->val.data = k;
      return n->val;
    }
  }
};

data_map map;

long astar(const walk &start) {
  const point goal{map.cols - 1, map.rows - 1};

  const auto sz = static_cast<unsigned>(map.data.size());
  hai::varray<node *> open{sz};

  hashmap nodes{};

  const auto h = [&](point p) {
    auto d = abs(goal - p);
    return 1 * (d.x + d.y);
  };

  nodes[start].g_score = 0;
  nodes[start].f_score = h(start.p);
  open.push_back(&nodes[start]);

  while (open.size() > 0) {
    long min_f = invalid;
    walk current;
    int min_i = -1;
    node *cur_n;
    for (auto i = 0; i < open.size(); i++) {
      auto &n = *(open[i]);
      if (n.f_score < min_f) {
        min_i = i;
        current = open[i]->data;
        cur_n = &n;
        min_f = n.f_score;
      }
    }
    if (current.p == goal) {
      hai::array<char> dbg{static_cast<unsigned>(map.data.size() + 1)};
      for (auto i = 0; i < dbg.size(); i++) {
        dbg[i] = map.data[i];
      }
      while (current.p != start.p) {
        constexpr const char sym[4]{'^', 'v', '<', '>'};
        auto &n = nodes[current];
        dbg[map.index(current.p)] = sym[current.c];
        current = n.came_from->data;
      }
      // fprintf(stderr, "%s", dbg.begin());
      return cur_n->g_score;
    }

    open[min_i] = open.pop_back();

    const auto next = [&](walk nei) {
      if (!map.inside(nei.p))
        return;

      auto &nein = nodes[nei];
      auto d = map.at(nei.p) - '0';
      auto tgs = cur_n->g_score + d;
      if (tgs < nein.g_score) {
        nein.came_from = cur_n;
        nein.g_score = tgs;
        nein.f_score = tgs + h(nei.p);

        for (auto p : open) {
          if (p->data == nei)
            return;
        }
        open.push_back(&nein);
      }
    };

    if (current.s < 10) {
      walk cont = current;
      cont.p = cont.p + step(cont.c);
      cont.s++;
      next(cont);
    }

    if (current.s >= 4) {
      walk turn{};
      turn.c = static_cast<cardinal>(current.c ^ 2);
      turn.p = current.p + step(turn.c);
      turn.s = 1;
      next(turn);

      turn.c = static_cast<cardinal>(current.c ^ 3);
      turn.p = current.p + step(turn.c);
      turn.s = 1;
      next(turn);
    }
  }

  return invalid;
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);
  map = dt.map();

  info("s", astar({{}, S, 1}));
  info("e", astar({{}, E, 1}));
}
