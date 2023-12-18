#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

hai::varray<jute::view> lines{1024};
char at(point p) { return lines[p.y][p.x]; }

hai::array<char> purged{};
int pstride;
char &pat(point p) { return purged[p.y * pstride + p.x]; }

constexpr const cardinal cloop[4]{N, S, W, E};
constexpr auto step(cardinal c) {
  constexpr const point vals[4]{{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
  return vals[c];
}
constexpr const jute::view map[4]{
    "| 7F",
    " |JL",
    "LF- ",
    "J7 -",
};
constexpr void next(point &p, cardinal &card) {
  card = static_cast<cardinal>(map[card].index_of(at(p)));
  if (card == X) {
    silog::log(silog::error, "ops");
    throw 0;
  }
  p = p + step(card);
}

bool run(point s, cardinal c) {
  constexpr const jute::view valids[4]{"7F|", "JL|", "LF-", "7J-"};
  auto p = s + step(c);
  if (valids[c].index_of(at(p)) < 0)
    return false;

  for (auto cc : cloop) {
    if (valids[cc].index_of(at(s + step(cc))) < 0)
      continue;

    pat(s) = map[c ^ 1][cc];
  }

  int n{1};
  while (s != p) {
    pat(p) = at(p);
    next(p, c);
    n++;
  }
  info("n", n / 2);
  return true;
}

int main(int argc, char **argv) {
  using namespace jute::literals;

  auto dt = data::of(argc);

  point s{};
  for (auto l : dt) {
    auto i = l.index_of('S');
    if (i >= 0) {
      s.x = i;
      s.y = lines.size();
    }

    lines.push_back(l);
  }
  pstride = lines[0].size();
  purged.set_capacity(lines.size() * pstride);

  for (auto c : cloop)
    if (run(s, c))
      break;

  for (auto &c : purged)
    if (c == 0)
      c = ' ';

  int res{};
  char *p = purged.begin();
  for (auto i = 0; i < lines.size(); i++) {
    bool in{};
    char flip{};
    for (auto j = 0; j < pstride; j++, p++) {
      if (*p == '|') {
        in = !in;
      } else if (*p == flip) {
        in = !in;
        flip = 0;
      } else if (*p == ' ' && in) {
        *p = '#';
        res++;
      } else {
        constexpr const jute::view flips = "FJL7";
        auto idx = flips.index_of(*p);
        if (idx >= 0 && !flip) {
          flip = flips[idx ^ 1];
        } else if (idx >= 0) {
          flip = 0;
        }
      }
    }

    silog::log(silog::debug, "[%.*s]", pstride, p - pstride);
  }
  info("res", res);
}
