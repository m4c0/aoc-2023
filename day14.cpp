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
  hai::array<char> map{rows * cols};
  auto p = map.begin();
  for (auto row : dt) {
    for (auto c : row)
      *p++ = c;
  }

  for (auto i = 0; i < rows; i++)
    silog::log(silog::debug, "%.*s", cols, map.begin() + i * cols);

  for (auto c = 0; c < cols; c++) {
    int rr = 0;
    for (auto r = 0; r < rows; r++) {
      switch (map[c + r * cols]) {
      case '.':
        break;
      case 'O':
        map[c + r * cols] = '.';
        map[c + rr * cols] = 'O';
        rr++;
        break;
      case '#':
        rr = r + 1;
        break;
      }
    }
  }

  silog::log(silog::debug, "");
  for (auto i = 0; i < rows; i++)
    silog::log(silog::debug, "%.*s", cols, map.begin() + i * cols);

  hai::array<long> pts{cols};
  for (auto &p : pts) {
    p = rows;
  }

  int r{};
  long res{};
  for (auto i = 0; i < rows; i++) {
    auto row = map.begin() + r * cols;
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
