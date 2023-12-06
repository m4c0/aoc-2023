#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

constexpr long mehtoi(jute::view v) {
  long res{};
  for (auto c : v) {
    if (c >= '0' && c <= '9')
      res = res * 10 + (c - '0');
  }
  return res;
}
static_assert(mehtoi("  12   34   56  ") == 123456l);

int main() {
  auto dt = data::real();
  auto it = dt.begin();
  auto t = mehtoi((*it).split(':').after);
  auto d = mehtoi((*++it).split(':').after);

  int num{0};
  for (auto i = 0; i < t; i++) {
    if (i * (t - i) > d)
      num++;
  }
  info("num", num);
}
