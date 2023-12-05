#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

long cards[1000]{};
long sum{};

void run(jute::view line) {
  auto [l, r] = line.split(':');
  auto [wins, gots] = r.split('|');

  auto id = atoi(l.split(' ').after.trim());
  auto qty = cards[id] + 1;

  int worth = 0;
  for (auto g : atoi_it(gots)) {
    for (auto w : atoi_it(wins)) {
      if (g == w) {
        worth++;
        break;
      }
    }
  }
  if (worth == 0)
    info("zeroed", id);
  for (auto i = 0; i < worth; i++) {
    cards[id + i + 1] += qty;
  }
  sum += qty;
}

int main() {
  loop(run);
  info("sum", sum);
}
