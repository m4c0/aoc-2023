#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

long run(const hai::varray<long> &row) {
  hai::varray<long> next{row.size()};
  long n = row[0];
  bool all_zeroes{true};
  for (auto i = 1; i < row.size(); i++) {
    long nn = row[i];
    long d = nn - n;
    next.push_back(d);
    n = nn;
    if (d != 0)
      all_zeroes = false;
  }
  if (row.size() < 3)
    throw 0;

  auto last = row[row.size() - 1];
  return all_zeroes ? last : last + run(next);
}

int main() {
  auto dt = data::real();
  long res{0};
  for (auto line : dt) {
    hai::varray<long> row{1024};
    for (long num : atoi_it(line)) {
      row.push_back(num);
    }
    long r = run(row);
    res += r;
  }
  info("res", res);
}
