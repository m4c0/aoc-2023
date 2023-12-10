#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

hai::varray<jute::view> lines{1024};
char at(point p) { return lines[p.y][p.x]; }

enum cardinal { X = -1, N, S, W, E };
constexpr auto step(cardinal c) {
  constexpr const point vals[4]{{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
  return vals[c];
}
constexpr void next(point &p, cardinal &card) {
  constexpr const jute::view map[4]{
      "| 7F",
      " |JL",
      "LF- ",
      "J7 -",
  };
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

  int n{1};
  while (s != p) {
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

  run(s, W) || run(s, E) || run(s, N) || run(s, S);
}
