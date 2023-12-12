export module bruh;
import jute;
import silog;
import sires;
import traits;

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

export constexpr long atoi(jute::view str) {
  long n = 0;
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
export void info(const char *label, long val) {
  silog::log(silog::info, "%s: %ld", label, val);
}
export void info(const char *label, jute::view val) {
  silog::log(silog::info, "%s: [%.*s]", label, (int)val.size(), val.data());
}

export constexpr int abs(int a) noexcept { return a >= 0 ? a : -a; }

export struct point {
  long x{};
  long y{};
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

export constexpr void mx(auto &a, auto b) noexcept {
  if (b > a)
    a = b;
}
export constexpr void mn(auto &a, auto b) noexcept {
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
export constexpr bool neg_digit(char c) { return digit(c) || (c == '-'); }

export constexpr bool starts_with(jute::view str, jute::view seed) {
  auto [l, r] = str.subview(seed.size());
  return l == seed;
}
static_assert(starts_with("one", "one"));
static_assert(!starts_with("one", "ones"));
static_assert(starts_with("onetwo", "one"));
static_assert(!starts_with("onetwo", "two"));

export class atoi_sit {
  jute::view m_data{};
  char m_split;

public:
  constexpr atoi_sit() = default;
  constexpr atoi_sit(jute::view v, char spl) : m_data{v}, m_split{spl} {}

  constexpr auto begin() const noexcept {
    return atoi_sit(m_data.trim(), m_split);
  }
  constexpr auto end() const noexcept { return atoi_sit{}; }

  constexpr bool operator!=(const atoi_sit &o) const noexcept {
    return m_data != o.m_data;
  }

  constexpr auto &operator++() noexcept {
    m_data = m_data.split(m_split).after.trim();
    return *this;
  }

  constexpr auto operator*() const noexcept { return atoi(m_data); }
};
static_assert([] {
  const auto fail = [] -> int { throw 1; };

  auto it = atoi_sit{" 1, 2, -3 ", ','}.begin();
  (*it) == 1 || fail();
  (*++it) == 2 || fail();
  (*++it) == -3 || fail();
  (++it != atoi_sit{}) && fail();

  return true;
}());

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
    while (m_data != "" && neg_digit(m_data[0])) {
      m_data = m_data.subview(1).after;
    }
    m_data = m_data.trim();
    return *this;
  }

  constexpr auto operator*() const noexcept { return atoi(m_data); }
};
static_assert([] {
  const auto fail = [] -> int { throw 1; };

  auto it = atoi_it{" 1 2 -3 "}.begin();
  (*it) == 1 || fail();
  (*++it) == 2 || fail();
  (*++it) == -3 || fail();
  (++it != atoi_it{}) && fail();

  return true;
}());

class row_it {
  jute::view line{};
  jute::view rest{};

public:
  constexpr row_it() = default;
  constexpr row_it(jute::view buf) {
    auto [l, r] = buf.split('\n');
    line = l;
    rest = r;
  }

  constexpr bool operator==(const row_it &o) const noexcept {
    return o.line.data() == line.data() ||
           (!line.size() && !o.line.size() && !o.rest.size() && !rest.size());
  }

  constexpr row_it &operator++() noexcept { return *this = row_it{rest}; }
  constexpr row_it operator++(int) noexcept {
    row_it res = *this;
    *this = row_it{rest};
    return res;
  }
  constexpr auto operator*() const noexcept { return line; }
};
export class data {
  hai::array<char> m_data;

  constexpr explicit data(auto d) : m_data{traits::move(d)} {}

public:
  static data real() { return data{slurp("../../data.real.txt")}; }
  static data fake() { return data{slurp("../../data.fake.txt")}; }
  static data of(int argc) { return argc == 1 ? fake() : real(); }

  auto begin() const noexcept {
    return row_it{{m_data.begin(), m_data.size()}};
  }
  auto end() const noexcept { return row_it{}; }
};
