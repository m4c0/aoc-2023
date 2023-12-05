export module bruh;
import jute;
import silog;
import sires;

export auto slurp(jute::view fn) {
  auto file = sires::slurp(fn).take([](auto e) {
    silog::log(silog::error, "failure: %s", e);
    throw 1;
  });
  silog::log(silog::info, "file size: %d", file.size());
  return file;
}
export auto slurp() { return slurp("../../data.txt"); }

export void loop(jute::view fn, auto &&func) {
  auto fd = slurp(fn);
  jute::view f{fd.begin(), fd.size()};
  while (f != "") {
    auto [line, rest] = f.split('\n');
    func(line);
    f = rest;
  }
}
export void loop(auto &&func) { loop("../../data.real.txt", func); }
export void loop_e(auto &&func) { loop("../../data.fake.txt", func); }

export constexpr unsigned atoi(jute::view str) {
  unsigned n = 0;
  auto st = str[0] == '-' ? 1 : 0;
  auto [l, r] = str.subview(st);
  for (auto c : r) {
    if (c < '0' || c > '9')
      break;
    n = n * 10 + (c - '0');
  }
  return n * (st * -2 + 1);
}
static_assert(atoi("10") == 10);
static_assert(atoi("123") == 123);
static_assert(atoi("-123") == -123);

export void info(const char *label, int val) {
  silog::log(silog::info, "%s: %d", label, val);
}
export void info(const char *label, jute::view val) {
  silog::log(silog::info, "%s: [%.*s]", label, (int)val.size(), val.data());
}

export constexpr int abs(int a) noexcept { return a >= 0 ? a : -a; }

export struct point {
  int x{};
  int y{};
};
export constexpr point operator+(const point &a, const point &b) noexcept {
  return {a.x + b.x, a.y + b.y};
}
export constexpr point operator-(const point &a, const point &b) noexcept {
  return {a.x - b.x, a.y - b.y};
}
export constexpr bool operator==(const point &a, const point &b) noexcept {
  return a.x == b.x && a.y == b.y;
}
export constexpr point abs(const point &a) noexcept {
  return {abs(a.x), abs(a.y)};
}

export constexpr void mx(int &a, int b) noexcept {
  if (b > a)
    a = b;
}
export constexpr void mn(int &a, int b) noexcept {
  if (b < a)
    a = b;
}

static int ic_n{};
static char ic_buf[1024]{};
export class indcounter {

public:
  indcounter() {
    ic_buf[ic_n++] = ' ';
    ic_buf[ic_n++] = ' ';
  }
  ~indcounter() {
    ic_buf[--ic_n] = 0;
    ic_buf[--ic_n] = 0;
  }

  const auto operator*() const noexcept { return ic_buf; }
};

export constexpr bool digit(char c) {
  if (c < '0')
    return false;
  if (c > '9')
    return false;
  return true;
}
export constexpr bool starts_with(jute::view str, jute::view seed) {
  auto [l, r] = str.subview(seed.size());
  return l == seed;
}
static_assert(starts_with("one", "one"));
static_assert(!starts_with("one", "ones"));
static_assert(starts_with("onetwo", "one"));
static_assert(!starts_with("onetwo", "two"));

export class atoi_it {
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
