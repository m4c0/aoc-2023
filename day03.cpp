#pragma leco tool
import bruh;
import hai;
import jute;
import silog;
import traits;

struct num {
  unsigned start{};
  unsigned end{};
  int value{};
  bool marked{};
};
hai::varray<num> prev_nums{10240};
hai::varray<unsigned> prev_syms{10240};

constexpr void mark(hai::varray<num> &ns, int pos) {
  for (auto &n : ns) {
    // silog::log(silog::debug, "%d %d", n.value, pos);
    if (n.end < pos - 1 || pos + 1 < n.start)
      continue;
    n.marked = true;
  }
}
static_assert([] {
  hai::varray<num> p{1};
  p.push_back(num{2, 4, 592});
  mark(p, 5);
  return p[0].marked;
}());

int sum{};
void run(jute::view line) {
  hai::varray<num> curr{10240};

  num n{};
  for (n.start = 0; n.start < line.size(); n.start++) {
    if (!digit(line[n.start]))
      continue;

    for (n.end = n.start + 1; n.end < line.size(); n.end++) {
      if (digit(line[n.end]))
        continue;

      auto [a, sub, b] = line.subview(n.start, n.end);
      n.value = atoi(sub);

      n.end--; // inclusive range

      silog::log(silog::debug, "%d %d - %d", n.start, n.end, n.value);
      curr.push_back(n);
      n.start = n.end + 1;
      break;
    }
  }

  for (auto pos : prev_syms) {
    mark(curr, pos);
  }

  prev_syms.truncate(0);
  for (auto i = 0; i < line.size(); i++) {
    if (digit(line[i]) || line[i] == '.')
      continue;

    prev_syms.push_back(i);
    mark(prev_nums, i);
    mark(curr, i);
  }

  for (auto &n : prev_nums) {
    if (n.marked)
      sum += n.value;
  }
  prev_nums = traits::move(curr);
}

int main() {
  loop(run);
  run("");

  info("res", sum);
}
