#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

int main(int argc, char **argv) {
  auto dt = data::of(argc);

  unsigned rows{};
  unsigned cols{};
  for (auto row : dt) {
    cols = row.size();
    rows++;
  }

  hai::array<long> pts{cols};
  for (auto &p : pts) {
    p = rows;
  }

  int r{};
  long res{};
  for (auto row : dt) {
    for (int c{}; c < cols; c++) {
      if (row[c] == 'O') {
        res += pts[c]--;
      } else if (row[c] == '#') {
        pts[c] = rows - r - 1;
      }
    }
    r++;
  }
  info("res", res);
}
