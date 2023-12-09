#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

int run(const hai::varray<int> &row) {
  hai::varray<int> next{row.size()};
  int n = row[0];
  int sum{};
  for (auto i = 1; i < row.size(); i++) {
    auto nn = row[i];
    auto d = nn - n;
    next.push_back(d);
    n = nn;
    sum += d;
  }
  auto last = row[row.size() - 1];
  if (sum == 0) {
    return last;
  } else {
    return last + run(next);
  }
}

int main() {
  auto dt = data::fake();
  auto res{0};
  for (auto line : dt) {
    hai::varray<int> row{1024};
    for (auto num : atoi_it(line)) {
      row.push_back(num);
    }
    auto r = run(row);
    info("row", r);
    res += r;
  }
  info("res", res);
}
