#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

constexpr int digit(jute::view v) {
  constexpr const jute::view names[10] = {"zero",  "one",  "two", "three",
                                          "four",  "five", "six", "seven",
                                          "eight", "nine"};
  if (digit(v[0])) {
    return v[0] - '0';
  }
  for (auto i = 0; i < 10; i++) {
    if (starts_with(v, names[i])) {
      return i;
    }
  }
  return -1;
}

int main() {
  int sum{};
  loop([&](auto line) {
    int val{};
    while (line.size() > 0) {
      auto d = digit(line);
      if (d >= 0) {
        if (val == 0) {
          val = d * 10;
          val += d;
        } else {
          val -= val % 10;
          val += d;
        }
      }
      line = line.subview(1).after;
    }
    info("val", val);
    sum += val;
  });

  info("sum", sum);
}
