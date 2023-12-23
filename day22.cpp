#pragma leco tool
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

public:
  constexpr void push_back(Tp v) {
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
};
} // namespace ll

struct block {
  int x0;
  int y0;
  int z0;
  int x1;
  int y1;
  int z1;
};

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  ll::list<block> blocks{};
  for (auto line : dt) {
    block b{};
    if (!scan::f(line, "\v,\v,\v~\v,\v,\v", b.x0, b.y0, b.z0, b.x1, b.y1, b.z1))
      throw 0;

    blocks.push_back(b);
  }
  for (auto a = blocks.begin(); a != blocks.end(); ++a) {
    for (auto b = a + 1; b != blocks.end(); ++b) {
      if (a->z0 > b->z0) {
        block tmp = *a;
        *a = *b;
        *b = tmp;
      }
    }
  }
  for (auto &b : blocks) {
    silog::log(silog::info, "%d %d %d - %d %d %d", b.x0, b.y0, b.z0, b.x1, b.y1,
               b.z1);
  }

  info("done", 0);
}
