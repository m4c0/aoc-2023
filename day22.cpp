#pragma leco tool
#include <stdio.h>
import bruh;
import hai;
import jute;
import scanf;
import silog;

namespace ll {
template <typename Tp> class list {
  struct node {
    node *next{};
    node *prev{};
    Tp value{};
  };
  class it {
    node *m_node{};

  public:
    constexpr it() = default;
    constexpr explicit it(node *n) : m_node{n} {}

    constexpr bool operator==(const it &o) const noexcept {
      return m_node == o.m_node;
    }
    constexpr it &operator++() noexcept {
      if (m_node != nullptr)
        m_node = m_node->next;
      return *this;
    }
    constexpr Tp &operator*() noexcept { return m_node->value; }
    constexpr Tp *operator->() noexcept { return &m_node->value; }

    constexpr it operator+(unsigned n) noexcept {
      it res = *this;
      for (auto i = 0; i < n; i++)
        ++res;
      return res;
    }
  };

  node *m_head{};
  node *m_tail{};
  int m_size{};

public:
  constexpr void push_back(Tp v) {
    m_size++;

    auto n = new node{.value = v};
    if (m_head == nullptr) {
      m_head = m_tail = n;
      return;
    }
    n->prev = m_tail;
    m_tail->next = n;
    m_tail = n;
  }

  constexpr auto begin() noexcept { return it{m_head}; }
  constexpr auto end() noexcept { return it{}; }
  constexpr auto begin() const noexcept { return it{m_head}; }
  constexpr auto end() const noexcept { return it{}; }

  constexpr int size() const noexcept { return m_size; }
  constexpr bool empty() const noexcept { return m_head == nullptr; }
  constexpr bool contains(const Tp &v) const noexcept {
    for (auto &n : *this) {
      if (n == v)
        return true;
    }
    return false;
  }
};
} // namespace ll

struct block {
  int n{};
  int x0;
  int y0;
  int z0;
  int x1;
  int y1;
  int z1;
};
constexpr block fall1(block b) {
  if (b.z0 > 1) {
    b.z0--;
    b.z1--;
  }
  return b;
}
constexpr bool hits(const block &a, const block &b) {
  if (a.x0 > b.x1)
    return false;
  if (a.x1 < b.x0)
    return false;
  if (a.y0 > b.y1)
    return false;
  if (a.y1 < b.y0)
    return false;
  if (a.z0 > b.z1)
    return false;
  if (a.z1 < b.z0)
    return false;
  return true;
}

void dump(const auto &blocks) {
  for (auto &b : blocks) {
    silog::log(silog::info, "%d %d %d - %d %d %d", b.x0, b.y0, b.z0, b.x1, b.y1,
               b.z1);
  }
}
const char *str(int n) {
  if (n == 0)
    return "nil";
  char *buf = new char[10]{};
  char *res = buf;
  while (n > 0) {
    *res++ = ((n - 1) % 26) + 'A';
    n /= 26;
  }
  *res = 0;
  return buf;
}
void dump_g(const auto &graph) {
  for (auto i = 0; i < graph.size(); i++) {
    fprintf(stderr, "%4d %4s: ", i, str(i));
    for (auto j : graph[i]) {
      fprintf(stderr, "%4s ", str(j));
    }
    fprintf(stderr, "\n");
  }
  fprintf(stderr, "\n");
}

bool contains_all(const auto &blown, const auto &ids) {
  for (auto s : ids) {
    if (!blown[s]) {
      return false;
    }
  }
  return true;
}

auto bls(const auto &blown) {
  jute::heap res{};
  for (auto i = 0; i < blown.size(); i++) {
    if (blown[i]) {
      res = res + jute::view::unsafe(str(i)) + " ";
    }
  }
  return res;
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  hai::varray<block> blocks{10240};
  int n{};
  int max_dz{};
  for (auto line : dt) {
    block b{.n = ++n};
    if (!scan::f(line, "\v,\v,\v~\v,\v,\v", b.x0, b.y0, b.z0, b.x1, b.y1, b.z1))
      throw 0;

    if (b.x0 > b.x1 || b.y0 > b.y1 || b.z0 > b.z1)
      throw 0;

    mx(max_dz, b.z1 - b.z0);

    blocks.push_back(b);
  }
  info("max dz", max_dz);

  for (auto a = blocks.begin(); a != blocks.end(); ++a) {
    for (auto b = a + 1; b != blocks.end(); ++b) {
      if (a->z0 > b->z0) {
        block tmp = *a;
        *a = *b;
        *b = tmp;
      }
    }
  }

  bool changed{};
  do {
    changed = false;
    for (auto a = blocks.begin(); a != blocks.end(); ++a) {
      if (a->z0 == 1)
        continue;

      auto a1 = fall1(*a);
      bool hit{};
      for (auto b = blocks.begin(); b != blocks.end(); ++b) {
        if (a != b && hits(a1, *b)) {
          // silog::log(silog::info, "hit: %d(%d)-%d(%d)", a1.n, a1.z0, b->n,
          // b->z0);
          hit = true;
          break;
        }
      }
      if (!hit) {
        // info("fall", a1.n);
        changed = true;
        *a = a1;
      }
    }
  } while (changed);

  // dump(blocks);

  hai::array<ll::list<int>> sups{blocks.size() + 1};
  hai::array<ll::list<int>> is_sup_by{blocks.size() + 1};
  for (auto a = blocks.begin(); a != blocks.end(); ++a) {
    if (a->z0 == 1)
      continue;

    auto a1 = fall1(*a);
    for (auto b = blocks.begin(); b != blocks.end(); ++b) {
      if (a != b && hits(a1, *b)) {
        sups[b->n].push_back(a->n);
        is_sup_by[a->n].push_back(b->n);
      }
    }
  }

  // dump_g(sups);
  // dump_g(is_sup_by);

  long part1{blocks.size()};
  long part2{};
  for (auto &b : blocks) {
    for (auto s : sups[b.n]) {
      if (is_sup_by[s].size() == 1) {
        part1--;
        break;
      }
    }

    hai::array<bool> blown{blocks.size() + 1};
    hai::varray<int> queue{10000000};
    queue.push_back(b.n);
    blown[b.n] = true;
    for (auto i = 0; i < queue.size(); i++) {
      for (auto s : sups[queue[i]]) {
        if (contains_all(blown, is_sup_by[s])) {
          blown[s] = true;
          queue.push_back(s);
        } else {
          // silog::log(silog::info, "---> %s <> %s -- %s", str(b.n), str(s),
          //            (*bls(blown)).data());
          // for (auto nn : is_sup_by[s]) {
          //   silog::log(silog::info, "  s: %s", str(nn));
          // }
        }
      }
    }
    // info("this", jute::view::unsafe(str(b.n)));
    for (auto i = 0; i < blown.size(); i++) {
      if (i != b.n && blown[i]) {
        // info("    blew", jute::view::unsafe(str(i)));
        part2++;
      }
    }
  }
  info("part 1", part1);
  info("part 2", part2);
}
