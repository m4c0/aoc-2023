#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

struct map {
  int orig[20];
  int trans[20];
};

int check(const int (&m)[20]) {
  for (auto i : m) {
    silog::log(silog::info, "%08x", i);
  }
  silog::log(silog::info, "-=-=-=-=-=-=");
  return 0;
}
int run(const map &m) { return check(m.orig) + check(m.trans); }

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  int res{};
  map m{};
  int r{};
  for (auto row : dt) {
    if (row == "") {
      res += run(m);
      m = {};
      r = 0;
      continue;
    }
    int c = 0;
    for (auto chr : row) {
      m.orig[r] = (m.orig[r] << 1) | (chr == '#' ? 1 : 0);
      m.trans[c] = (m.trans[c] << 1) | (chr == '#' ? 1 : 0);
      c++;
    }
    r++;
  }
  res += run(m);
  info("res", res);
}
