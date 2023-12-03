#pragma leco tool
#include <stdio.h>
import bruh;
import hai;
import jute;
import silog;
import traits;

struct num {
  unsigned start{};
  unsigned end{};
  unsigned row{};
  int value{};
  bool marked{};
};
hai::varray<num> prev_nums{10240};

struct gear {
  unsigned pos{};
  long value{1};
  int count{};
};
hai::varray<gear> prev_gears{10240};

constexpr void mark(hai::varray<num> &ns, gear &g) {
  for (auto &n : ns) {
    if (n.end < g.pos - 1 || g.pos + 1 < n.start)
      continue;

    g.value *= n.value;
    g.count++;
  }
}

int crow{1};

long sum{};
void run(jute::view line) {
  hai::varray<num> curr{10240};

  num n{};
  n.row = crow++;
  for (n.start = 0; n.start < line.size(); n.start++) {
    if (!digit(line[n.start]))
      continue;

    for (n.end = n.start + 1; n.end <= line.size(); n.end++) {
      if (digit(line[n.end]))
        continue;

      auto [a, sub, b] = line.subview(n.start, n.end);
      n.value = atoi(sub);

      n.end--; // inclusive range

      if (n.row == 11)
        silog::log(silog::debug, "%d %d - %d", n.start, n.end, n.value);
      curr.push_back(n);
      n.start = n.end + 1;
      break;
    }
  }

  for (auto &g : prev_gears) {
    mark(curr, g);
    if (g.count == 2)
      sum += g.value;
  }

  prev_gears.truncate(0);
  for (auto i = 0U; i < line.size(); i++) {
    if (digit(line[i]) || line[i] == '.')
      continue;

    gear g{.pos = i};
    mark(prev_nums, g);
    mark(curr, g);
    prev_gears.push_back(g);
  }

  prev_nums = traits::move(curr);
}

int main() {
  loop(run);
  run("");

  info("res", sum);
}
