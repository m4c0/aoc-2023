#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

struct seed {
  long value{};
  int map{};
};
hai::varray<seed> seeds{1024};
bool got_seeds{};
int cur_map{};

void run(jute::view line) {
  if (!got_seeds) {
    for (auto s : atoi_it(line.split(':').after)) {
      seeds.push_back(seed{s});
    }
    got_seeds = true;
    return;
  }
  if (line == "")
    return;
  if (line.index_of(':') > 0) {
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
    if (from_s <= s.value && s.value <= from_e) {
      s.value = to_s + s.value - from_s;
      s.map = cur_map;
    }
  }
}

int main() {
  loop(run);

  long min{};
  for (auto s : seeds) {
    if (min == 0)
      min = s.value;
    mn(min, s.value);
  }
  info("res", min);
}
