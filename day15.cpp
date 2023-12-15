#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

constexpr auto hash(jute::view v) {
  int res{};
  for (auto c : v) {
    res += c;
    res *= 17;
    res %= 256;
  }
  return res;
}
static_assert(hash("HASH") == 52);

int main(int argc, char **argv) {
  auto dt = data::of(argc);
  auto line = *(dt.begin());

  int sum{};
  while (line != "") {
    auto [l, r] = line.split(',');
    sum += hash(l);
    line = r;
  }
  info("sum", sum);
}
