#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

constexpr bool digit(char c) {
  if (c < '0')
    return false;
  if (c > '9')
    return false;
  return true;
}
int main() {
  int sum{};
  loop([&](auto line) {
    int val{};
    for (auto c : line) {
      if (!digit(c))
        continue;
      if (val == 0) {
        val = (c - '0') * 10;
        val += (c - '0');
      } else {
        val -= val % 10;
        val += (c - '0');
      }
    }
    info("val", val);
    sum += val;
  });

  info("sum", sum);
}
