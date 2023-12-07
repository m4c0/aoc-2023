#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

char rank(char c) {
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
      return 11;
    case 'T':
      return 10;
    default:
      throw 0;
    }
  }
}
void rank(jute::view in, char *out) {
  for (auto c : in) {
    *out++ = rank(c);
  }
}

enum type_t { high, one_pair, two_pair, three, full_house, four, five };
int type(jute::view hand) {
  int qty[20]{};
  for (auto c : hand) {
    qty[(int)c]++;
  }
  int mx1{};
  int mx2{};
  for (auto q : qty) {
    if (q > mx1) {
      mx2 = mx1;
      mx1 = q;
    } else if (q > mx2) {
      mx2 = q;
    }
  }
  if (mx1 == 5)
    return five;
  if (mx1 == 4)
    return four;
  if (mx1 == 3 && mx2 == 2)
    return full_house;
  if (mx1 == 3)
    return three;
  if (mx1 == 2 && mx2 == 2)
    return two_pair;
  if (mx1 == 2)
    return one_pair;
  return high;
}

int idof(jute::view hand) {
  int res{type(hand)};
  for (auto c : hand) {
    res = (res << 4) | c;
  }
  return res;
}

struct bet {
  int hand;
  long bid;
};
int main() {
  auto dt = data::real();

  hai::varray<bet> bets{10240};
  for (auto t : dt) {
    auto [hand, bid_s] = t.split(' ');
    auto bid = atoi(bid_s);

    char rkd[6]{};
    rank(hand, rkd);

    auto id = idof(jute::view{rkd, 5});

    silog::log(silog::debug, "%.*s -- %ld -- %8x", (int)hand.size(),
               hand.data(), bid, id);
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
    silog::log(silog::debug, "%5x -- %ld @%d", bi.hand, bi.bid, i + 1);
    res += bi.bid * (i + 1);
  }
  info("res", res);
}
