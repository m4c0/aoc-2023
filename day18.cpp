#pragma leco tool
import bruh;
import hai;
import jute;
import silog;
#include <stdio.h>

constexpr cardinal chr2c(char c) {
  switch (c) {
  case '0':
    return E;
  case '1':
    return S;
  case '2':
    return W;
  case '3':
    return N;
  default:
    throw 0;
  }
}

constexpr long hex2i(jute::view v) {
  long res{};
  for (auto i = 0; i < v.size(); i++) {
    res <<= 4;

    if (digit(v[i])) {
      res |= v[i] - '0';
    } else {
      res |= v[i] - 'a' + 10;
    }
  }
  return res;
}
static_assert(hex2i("000") == 0x0);
static_assert(hex2i("00a") == 0xa);
static_assert(hex2i("123") == 0x123);
static_assert(hex2i("abc") == 0xabc);

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  long dug{0};

  point p{};
  for (auto line : dt) {
    auto [hex, chr] = line.split('#').after.subview(5);

    auto c = chr2c(chr[0]);
    auto n = hex2i(hex);
    auto np = p + step(c) * n;

    dug += p.x * np.y;
    dug -= p.y * np.x;
    dug += n;

    p = np;
  }

  dug /= 2;
  info("dug", dug + 1);
}
