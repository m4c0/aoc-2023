#pragma leco tool
import bruh;
import hai;
import jute;
import silog;
#include <stdio.h>

constexpr cardinal chr2c(char c) {
  switch (c) {
  case 'R':
    return E;
  case 'L':
    return W;
  case 'D':
    return S;
  case 'U':
    return N;
  default:
    throw 0;
  }
}

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  long dug{0};

  point p{};
  for (auto line : dt) {
    auto [l, lr] = line.split(' ');
    auto [ns, cl] = lr.split(' ');

    auto c = chr2c(l[0]);
    auto n = atoi(ns);
    auto np = p + step(c) * n;

    dug += p.x * np.y;
    dug -= p.y * np.x;
    dug += n;

    p = np;
  }

  dug /= 2;
  info("dug", dug + 1);
}
