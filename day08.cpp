#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

struct inst {
  int to_l{};
  int to_r{};
};

jute::view opc{};
int opcn{};
inst insts[26 * 26 * 26];

constexpr int code_of(jute::view v) {
  int res{};
  for (auto c : v.trim()) {
    res = res * 26 + c - 'A';
  }
  return res;
}
static_assert(code_of("AAA") == 0);
static_assert(code_of("AAZ") == 25);

constexpr auto zzz = code_of("ZZZ");
static_assert(zzz == 26 * 26 * 26 - 1);

constexpr bool matches(int code, char c) { return code % 26 == c - 'A'; }
static_assert(matches(code_of("AAA"), 'A'));
static_assert(matches(code_of("AAZ"), 'Z'));
static_assert(!matches(code_of("AAA"), 'Z'));
static_assert(!matches(code_of("AAZ"), 'A'));

auto next_opc() {
  auto res = opc[opcn];
  opcn = (opcn + 1) % opc.size();
  return res;
}

constexpr long gcd(long a, long b) {
  if (b == 0)
    return a;
  return (a > b) ? gcd(b, a % b) : gcd(a, b % a);
}
static_assert(gcd(54, 24) == 6);
static_assert(gcd(48, 18) == 6);
static_assert(gcd(15, 25) == 5);
static_assert(gcd(3, 2) == 1);
static_assert(gcd(3, 0) == 3);
static_assert(gcd(1, 1) == 1l);

auto find_zzz(int from) {
  struct {
    long res{};
    int stop_code{};
  } res;
  do {
    bool left = next_opc() == 'L';
    const auto &[l, r] = insts[from];
    from = left ? l : r;
    res.res++;
  } while (!matches(from, 'Z'));
  res.stop_code = from;
  return res;
}

int main() {
  auto dt = data::real();
  auto it = dt.begin();

  opc = *it++;

  hai::varray<int> from{10240};
  while (++it != dt.end()) {
    auto [f, ff] = (*it).split('=');
    auto [l, ll] = ff.split('(').after.split(',');
    auto r = ll.split(')').before;
    auto cf = code_of(f);
    insts[cf] = {code_of(l), code_of(r)};
    if (matches(cf, 'A'))
      from.push_back(cf);
  }

  long res{1};
  for (auto f : from) {
    long ff = find_zzz(f).res;
    info("gcd", ff / gcd(res, ff));
    res *= ff / gcd(res, ff);
    info("res", res);
  }
}
