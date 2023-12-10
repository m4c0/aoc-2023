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

int run(point s, cardinal c, jute::view valids) {
  auto p = s + step(c);
  if (valids.index_of(at(p)) < 0)
    return 0;

  int n{1};
  while (s != p) {
    next(p, c);
    n++;
  }
  return n;
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

  int n{};

  mx(n, run(s, W, "LF-"));
  mx(n, run(s, E, "7J-"));
  mx(n, run(s, N, "7F|"));
  mx(n, run(s, S, "JL|"));

  info("n", n / 2);
}
