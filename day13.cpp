#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

struct map {
  int orig[20];
  int trans[20];
  int r;
  int c;
};

constexpr auto pot(auto n) { return (n & (n - 1)) == 0; }

int check(const int (&m)[20], int n) {
  for (auto i = 0; i < n - 1; i++) {
    silog::log(silog::debug, "%x %x %d/%d", m[i], m[i + 1], i, n);

    auto ones = 0;
    auto j = 0;
    auto broken = false;
    for (; j <= i && j + i + 1 < n; j++) {
      auto bits = m[i - j] ^ m[i + j + 1];

      silog::log(silog::debug, "%x %x %d %d -- %d", m[i - j], m[i + j + 1],
                 i - j, i + j + 1, bits);
      if (bits == 0)
        continue;

      if ((bits & (bits - 1)) == 0) {
        if (++ones == 1)
          continue;
      }

      broken = true;
      break;
    }
    if (!broken && ones == 1) {
      for (auto k = i + j + 1; k < n; k++)
        silog::log(silog::debug, "%x -- %d %d", m[k], i, j);
      silog::log(silog::debug, "i:%d j:%d %d/%d", i, j, i + j + 1, n);
      return i + 1;
    }
  }
  info("meh", 0);
  return 0;
}
int run(const map &m) {
  info("mooo", 999999999999999);
  auto r = check(m.orig, m.r) * 100 + check(m.trans, m.c);
  if (r == 0)
    throw 0;
  return r;
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  int res{};
  map m{};
  for (auto row : dt) {
    if (row == "") {
      res += run(m);
      m = {};
      continue;
    }
    m.c = 0;
    for (auto chr : row) {
      m.orig[m.r] = (m.orig[m.r] << 1) | (chr == '#' ? 1 : 0);
      m.trans[m.c] = (m.trans[m.c] << 1) | (chr == '#' ? 1 : 0);
      m.c++;
    }
    m.r++;
  }
  res += run(m);
  info("res", res);
}
