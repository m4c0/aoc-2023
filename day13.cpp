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

int check(const int (&m)[20], int n) {
  for (auto i = 0; i < n - 1; i++) {
    silog::log(silog::debug, "%x %x %d/%d", m[i], m[i + 1], i, n);
    if (m[i] != m[i + 1])
      continue;

    auto j = 0;
    for (; j < i && j + i + 1 < n; j++) {
      silog::log(silog::debug, "%x %x %d %d", m[i - j], m[i + j + 1], j,
                 i + j + 1);
      if (m[i - j] != m[i + j + 1])
        break;
    }
    if (i + j + 1 == n) {
      info("i", i);
      return i + 1;
    }
  }
  info("meh", 0);
  return 0;
}
int run(const map &m) {
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
