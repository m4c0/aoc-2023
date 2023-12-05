#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

struct seed {
  long start{};
  long end{};
  int map{};
};
constexpr bool operator==(const seed &a, const seed &b) {
  return a.start == b.start && a.end == b.end && a.map == b.map;
}

hai::varray<seed> seeds{1024};
bool got_seeds{};
int cur_map{};

struct spair {
  seed a;
  seed b;
};
constexpr bool operator==(const spair &a, const spair &b) {
  return a.a == b.a && a.b == b.b;
}
constexpr auto split(seed s, long p) {
  spair res{s, s};
  res.a.end = p;
  res.b.start = p + 1;
  res.b.end = s.end;
  return res;
}

void run(jute::view line) {
  if (!got_seeds) {
    auto it = atoi_it(line.split(':').after).begin();
    for (; it != atoi_it{}; ++it) {
      auto s = *it;
      auto l = *++it;
      seeds.push_back(seed{s, s + l - 1});
    }
    got_seeds = true;
    return;
  }
  if (line == "")
    return;
  if (line.index_of(':') > 0) {
    info("map", line);
    cur_map++;
    return;
  }
  auto it = atoi_it(line).begin();
  auto to_s = *it;
  auto from_s = *++it;
  auto len = *++it;
  auto from_e = from_s + len - 1;
  for (auto &s : seeds) {
    if (s.map == cur_map)
      continue;

    silog::log(silog::debug, "%d %d -- %ld %ld === %d", from_s, from_e, s.start,
               s.end, to_s);
    if (s.end < from_s)
      continue;
    if (from_e < s.start)
      continue;

    s.map = cur_map;
    if (from_s <= s.start && s.end <= from_e) {
      silog::log(silog::debug, "------ %ld %ld", s.start, s.end);
    } else if (from_s <= s.start) {
      auto [a, b] = split(s, from_e);
      silog::log(silog::debug, "------ << %ld %ld -- %ld %ld", a.start, a.end,
                 b.start, b.end);
      s = a;
      seeds.push_back(b);
    } else if (s.end <= from_e) {
      auto [a, b] = split(s, from_s - 1);
      silog::log(silog::debug, "------ >> %ld %ld -- %ld %ld", a.start, a.end,
                 b.start, b.end);
      s = b;
      seeds.push_back(a);
    } else {
      auto [a, b] = split(s, from_s - 1);
      auto [c, d] = split(b, from_e);
      silog::log(silog::debug, "------ %ld %ld -- %ld %ld -- %ld %ld", a.start,
                 a.end, c.start, c.end, d.start, d.end);
      s = c;
      seeds.push_back(a);
      seeds.push_back(d);
    }
    s.start = to_s + s.start - from_s;
    s.end = to_s + s.end - from_s;
  }
}

int main() {
  loop_e(run);

  long min{};
  for (auto s : seeds) {
    if (min == 0)
      min = s.start;
    mn(min, s.start);
  }
  info("res", min);
}
