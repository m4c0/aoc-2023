#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

constexpr char rank(char c) {
  if (c >= '2' && c <= '9') {
    return c - '0';
  } else {
    switch (c) {
    case 'A':
      return 14;
    case 'K':
      return 13;
    case 'Q':
      return 12;
    case 'J':
      return 1;
    case 'T':
      return 10;
    default:
      throw 0;
    }
  }
}
constexpr void rank(jute::view in, char *out) {
  for (auto c : in) {
    *out++ = rank(c);
  }
}

enum type_t { high, one_pair, two_pair, three, full_house, four, five };
constexpr int has(int (&qty)[20], int q, bool joker) {
  int j = joker ? qty[1] : 0;
  int res{};
  for (auto i = 2; i < 20; i++) {
    if (qty[i] + j == q)
      res++;
  }
  return res;
}
constexpr int type(jute::view hand) {
  int qty[20]{};
  for (auto c : hand) {
    qty[(int)c]++;
  }
  if (has(qty, 5, true))
    return five;
  if (has(qty, 4, true))
    return four;
  if (has(qty, 3, false) && has(qty, 2, false))
    return full_house;
  if (has(qty, 3, true) && has(qty, 2, false) == 2)
    return full_house;
  if (has(qty, 3, true))
    return three;
  if (has(qty, 2, false) == 2)
    return two_pair;
  if (has(qty, 2, true))
    return one_pair;
  return high;
}

constexpr auto idof(jute::view hand_s) {
  char rkd[6]{};
  rank(hand_s, rkd);

  jute::view hand{rkd, 5};
  int res{type(hand)};
  for (auto c : hand) {
    res = (res << 4) | c;
  }
  return res;
}

static_assert(idof("AAAAA") == 0x6eeeee);
static_assert(idof("AAJJJ") == 0x6ee111);
static_assert(idof("AJJJJ") == 0x6e1111);
static_assert(idof("JJJJJ") == 0x611111);
static_assert(idof("AA2AA") == 0x5ee2ee);
static_assert(idof("AA2JJ") == 0x5ee211);
static_assert(idof("22332") == 0x422332);
static_assert(idof("2233J") == 0x422331);
static_assert(idof("AJJ94") == 0x3e1194);
static_assert(idof("AAJ94") == 0x3ee194);
static_assert(idof("92399") == 0x392399);
static_assert(idof("AAKK2") == 0x2eedd2);
static_assert(idof("AA234") == 0x1ee234);
static_assert(idof("JA234") == 0x11e234);
static_assert(idof("23456") == 0x023456);

struct bet {
  int hand;
  long bid;
};
int main() {
  auto dt = data::real();

  hai::varray<bet> bets{10240};
  for (auto t : dt) {
    auto [hand_s, bid_s] = t.split(' ');
    auto bid = atoi(bid_s);

    auto id = idof(hand_s);

    silog::log(silog::debug, "%.*s -- %3ld -- %8x", (int)hand_s.size(),
               hand_s.data(), bid, id);
    bets.push_back(bet{id, bid});
  }

  long res{};
  for (auto i = 0; i < bets.size(); i++) {
    auto &bi = bets[i];
    for (auto j = i + 1; j < bets.size(); j++) {
      auto &bj = bets[j];
      if (bi.hand > bj.hand) {
        bet tmp = bi;
        bi = bj;
        bj = tmp;
      }
    }
    silog::log(silog::debug, "%6x -- %3ld @%4d", bi.hand, bi.bid, i + 1);
    res += bi.bid * (long)(i + 1);
  }
  info("res", res);
}
