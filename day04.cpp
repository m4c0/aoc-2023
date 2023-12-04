#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

class atoi_it {
  jute::view m_data{};

public:
  constexpr atoi_it() = default;
  constexpr atoi_it(jute::view v) : m_data{v} {}

  constexpr auto begin() const noexcept { return atoi_it(m_data.trim()); }
  constexpr auto end() const noexcept { return atoi_it{}; }

  constexpr bool operator!=(const atoi_it &o) const noexcept {
    return m_data != o.m_data;
  }

  constexpr auto &operator++() noexcept {
    while (m_data != "" && digit(m_data[0])) {
      m_data = m_data.subview(1).after;
    }
    m_data = m_data.trim();
    return *this;
  }

  constexpr auto operator*() const noexcept { return atoi(m_data); }
};
static_assert([] {
  const auto fail = [] -> int { throw 1; };

  auto it = atoi_it{" 1 2 3 "}.begin();
  (*it) == 1 || fail();
  (*++it) == 2 || fail();
  (*++it) == 3 || fail();
  (++it != atoi_it{}) && fail();

  return true;
}());

long cards[1000]{};
long sum{};

void run(jute::view line) {
  auto [l, r] = line.split(':');
  auto [wins, gots] = r.split('|');

  auto id = atoi(l.split(' ').after.trim());
  auto qty = cards[id] + 1;

  int worth = 0;
  for (auto g : atoi_it(gots)) {
    for (auto w : atoi_it(wins)) {
      if (g == w) {
        worth++;
        break;
      }
    }
  }
  if (worth == 0)
    info("zeroed", id);
  for (auto i = 0; i < worth; i++) {
    cards[id + i + 1] += qty;
  }
  sum += qty;
}

int main() {
  loop(run);
  info("sum", sum);
}
