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

auto next_opc() {
  auto res = opc[opcn];
  opcn = (opcn + 1) % opc.size();
  return res;
}

int find_zzz(int from) {
  if (from == zzz)
    return 0;

  bool left = next_opc() == 'L';
  const auto &[l, r] = insts[from];
  return 1 + find_zzz(left ? l : r);
}

int main() {
  auto dt = data::real();
  auto it = dt.begin();

  opc = *it++;

  while (++it != dt.end()) {
    auto [f, ff] = (*it).split('=');
    auto [l, ll] = ff.split('(').after.split(',');
    auto r = ll.split(')').before;
    insts[code_of(f)] = {code_of(l), code_of(r)};
  }

  info("res", find_zzz(code_of("AAA")));
}
