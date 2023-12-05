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
      long s = *it;
      long l = *++it;
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
  auto delta_s = to_s - from_s;
  auto len = *++it;
  auto from_e = from_s + len - 1;
  for (auto &s : seeds) {
    if (s.map == cur_map)
      continue;

    silog::log(silog::debug, "%ld %ld -- %ld %ld === %ld", from_s, from_e,
               s.start, s.end, to_s);
    seed l = s;
    mn(l.end, from_s - 1);
    if (l.end >= l.start && l != s) {
      silog::log(silog::debug, "l: %ld %ld", l.start, l.end);
      seeds.push_back(l);
    }

    seed r = s;
    mx(r.start, from_e + 1);
    if (r.end >= r.start && r != s) {
      silog::log(silog::debug, "r: %ld %ld", r.start, r.end);
      seeds.push_back(r);
    }

    seed m = s;
    mx(m.start, from_s);
    mn(m.end, from_e);
    if (m.end >= m.start && l != s && r != s) {
      silog::log(silog::debug, "m: %ld %ld", m.start, m.end);
      s = m;
      s.map = cur_map;
      s.start += delta_s;
      s.end += delta_s;
    }
  }
}

int main() {
  loop(run);

  long min{};
  for (auto s : seeds) {
    silog::log(silog::debug, "vvvv %ld %ld", s.start, s.end);
    if (min == 0)
      min = s.start;
    mn(min, s.start);
  }
  info("res", min);
}
